#ifndef IKOOFSUELUQQQJIWGVVCVYHJMEFEHYECYSQNYRVGDOJHOSDQWQSMEVAFKUWQTDIEQBGITPTOX
#define IKOOFSUELUQQQJIWGVVCVYHJMEFEHYECYSQNYRVGDOJHOSDQWQSMEVAFKUWQTDIEQBGITPTOX

#include <f/timer/timer.hpp>

#include <rating/rating.hpp>
#include <rating/rating_manager.hpp>
#include <f/singleton/singleton.hpp>

#include <boost/flyweight.hpp>

#include <tuple>
#include <string>
#include <set>
#include <ctime>
#include <algorithm>
#include <map>

#include <iostream>
#include <iomanip>

namespace f
{

    struct item_based_mongo_rating_matrix_generator : rating_matrix_generator< item_based_mongo_rating_matrix_generator >
    {
        typedef rating_typedef::user_id_type        user_id_type;
        typedef rating_typedef::item_id_type        item_id_type;
        typedef rating_typedef::rating_type         rating_type;

        item_based_mongo_rating_matrix_generator( const std::string& host, const std::string& db )
        {
            auto& rating_db = singleton<mongo_rating>::instance();
            rating_db.initialize( host, db );
            auto& manager = singleton<mongo_rating_manager>::instance(); 
            manager.initialize( host, db );
        }

        void operator_impl( user_id_type const& uid )
        {
            select_item_first_step( uid );
            select_user( uid );
            //select_item( 512 );
            //select_item( 1024 );
            select_item( );
            make_matrix();
        }

        void make_matrix()
        {
            const unsigned long row = user_array.size();
            const unsigned long col = item_array.size();
            rating_matrix.resize( row, col );

            std::fill( rating_matrix.begin(), rating_matrix.end(), rating_type( -1 ) );

            auto& manager = singleton<mongo_rating_manager>::instance(); 

            std::map< item_id_type, unsigned long> item_offset;
            for ( unsigned long i = 0; i != item_array.size(); ++i )
                item_offset[ item_array[i] ] = i;

            for ( unsigned long i = 0; i != row; ++i )
            {
                auto const& user_item_range = manager.user_item_record.equal_range( user_array[i] );

                for ( auto itor = user_item_range.first; itor != user_item_range.second; ++itor )
                {
                    auto const& iid = (*itor).second;
                    auto item_iter = item_offset.find( iid );
                    if ( item_iter != item_offset.end() )
                    {
                        const unsigned long j = (*item_iter).second;
                        rating_matrix[i][j] = manager.query( user_array[i], iid );
                    }
                }
            }
        }

        //select user who have rated any one of the items in item_array
        void select_user( const user_id_type& uid ) //first user in array should be uid
        {
            user_array.clear();
            user_array.push_back( uid );

            auto& manager = singleton<mongo_rating_manager>::instance(); 
            std::set<user_id_type> user_collect;

            //collect all user that has rated the items
            for ( auto const& iid : item_array )
            {
                auto const& range = manager.item_user_record.equal_range( iid );
                for ( auto itor = range.first; itor != range.second; ++itor )
                    user_collect.insert( (*itor).second );
            }

            const unsigned long threshold = 256;
            //const unsigned long threshold = 512;

            if ( user_collect.size() > threshold )
            {
                //create a map [uint, uid]o
                std::multimap<unsigned long, user_id_type> user_account;

                for ( auto const& uuid : user_collect )
                {
                    user_account.insert( std::make_pair( manager.user_rating_account[uuid], uuid ) );
                }

                unsigned long counter = 0;
                for ( auto itor = user_account.rbegin(); itor != user_account.rend(); ++itor )
                {
                    if ( (*itor).second != uid )
                    {
                        if ( counter++ == threshold-1 ) break; //TODO fix the bug with duplicated uuid
                        user_array.push_back( (*itor).second );
                    }
                }

                return; 
            }

            for ( auto const& uuid : user_collect )
            {
                if ( uid == uuid ) continue;
                user_array.push_back( uuid );
            }
        }

        //select all items that this user has evaluated
        void select_item_first_step( const user_id_type& uid )
        {
            item_array.clear();

            auto& manager = singleton<mongo_rating_manager>::instance(); 
            auto const& range = manager.user_item_record.equal_range( uid );

            for ( auto it = range.first; it != range.second; ++it )
                item_array.push_back( (*it).second );
        }

        //for all the user in user_array, select all the items they has been selected
        void select_item( const unsigned long max_item )
        {
            item_array.clear();
            std::set< item_id_type > selected_item;
             
            auto& manager = singleton<mongo_rating_manager>::instance(); 

            for ( auto const& uid : user_array )
            {
                auto const& range = manager.user_item_record.equal_range( uid );

                for ( auto itor = range.first; itor != range.second; ++itor )
                    selected_item.insert( (*itor).second );
            }

            std::multimap< unsigned long, item_id_type> count_iid_record;

            for ( auto const& iid : selected_item )
                count_iid_record.insert( std::make_pair( manager.item_rating_account[iid], iid ) );

            unsigned long counter = 0;
            for ( auto itor = count_iid_record.rbegin(); itor != count_iid_record.rend(); ++itor )
            {
                if ( counter++ == max_item ) break;
                item_array.push_back( (*itor).second );
            }
        }

        //for all the user in user_array, select all the items they has been selected
        void select_item()
        {
            item_array.clear();
            std::set< item_id_type > selected_item;
             
            auto& manager = singleton<mongo_rating_manager>::instance(); 

            for ( auto const& uid : user_array )
            {
                auto const& range = manager.user_item_record.equal_range( uid );

                for ( auto itor = range.first; itor != range.second; ++itor )
                    selected_item.insert( (*itor).second );
            }

            for ( auto const& item : selected_item )
                item_array.push_back( item );
        }


    
    };//struct item_based_mongo_rating_matrix_generator

}//namespace f

#endif//IKOOFSUELUQQQJIWGVVCVYHJMEFEHYECYSQNYRVGDOJHOSDQWQSMEVAFKUWQTDIEQBGITPTOX

