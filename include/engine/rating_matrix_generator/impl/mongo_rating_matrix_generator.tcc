#ifndef FYXUSXYKCVGQXGWEVPCXTPLYYDMVIVUBEAOWUOFEBNOIRTGGTGCDDOBETXFVCKEWUSCPODXOV
#define FYXUSXYKCVGQXGWEVPCXTPLYYDMVIVUBEAOWUOFEBNOIRTGGTGCDDOBETXFVCKEWUSCPODXOV

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

#include <iostream>
#include <iomanip>

namespace f
{

    struct mongo_rating_matrix_generator : rating_matrix_generator< mongo_rating_matrix_generator >
    {
        typedef rating_typedef::user_id_type        user_id_type;
        typedef rating_typedef::item_id_type        item_id_type;
        typedef rating_typedef::rating_type         rating_type;

        mongo_rating_matrix_generator( const std::string& host, const std::string& db )
        {
            auto& rating_db = singleton<mongo_rating>::instance();
            rating_db.initialize( host, db );
            auto& manager = singleton<mongo_rating_manager>::instance(); 
            manager.initialize( host, db );
        }

        void operator_impl( user_id_type const& uid )
        {
            select_user( uid );
            select_item();
            make_matrix();
        }

        void make_matrix()
        {
            timer t( "make matrix" );

            const unsigned long row = user_array.size();
            const unsigned long col = item_array.size();
            rating_matrix.resize( row, col );

            std::fill( rating_matrix.begin(), rating_matrix.end(), rating_type( -1 ) );

            //auto& rating_db = singleton<mongo_rating>::instance();
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
                        //rating_matrix[i][j] = rating_db.query( user_array[i], iid ); //hotspot
                        rating_matrix[i][j] = manager.query( user_array[i], iid );

                        //std::cout.precision(10);
                        //std::cout << rating_db.query( user_array[i], iid ) - rating_matrix[i][j] << std::endl;
                    }
                }

                /*
                for ( unsigned long j = 0; j != col; ++j )
                {
                    for ( auto itor = user_item_range.first; itor != user_item_range.second; ++itor )
                    {
                        if ( (*itor).second == item_array[j] )
                        {
                            rating_matrix[i][j] = rating_db.query( user_array[i], item_array[j] );
                            continue;
                        }
                    }
                }
                */

            }
        }

        void select_user( const user_id_type& uid ) //first user in array should be uid
        {
            timer t( "select user" );

            user_array.clear();
            user_array.push_back( uid );

            auto& manager = singleton<mongo_rating_manager>::instance(); 

            unsigned long select = 256;

            //if total user number is less than select, then select all of them
            if ( manager.user_rating_account.size() < select )
            {
                for ( auto itor = manager.user_rating_account.rbegin(); itor != manager.user_rating_account.rend(); ++itor )
                {
                    auto const& id = (*itor).first;
                    if ( id == uid ) continue;
                    user_array.push_back( (*itor).first );
                }
                return;
            }
           
            //should I move this to struct?
            std::multimap<unsigned long, user_id_type> collect;
            for ( auto itor = manager.user_rating_account.begin(); itor != manager.user_rating_account.end(); ++itor )
                collect.insert( std::make_pair( (*itor).second, (*itor).first ) );

            unsigned long random_select = std::min( select+select/2, manager.user_rating_account.size() );

            for ( auto itor = collect.rbegin(); random_select && itor != collect.rend(); ++itor, --random_select )
            {
                auto const& id = (*itor).second;
                if ( id == uid ) continue; //skip the current working uid

                user_array.push_back( (*itor).second );
            }

            std::srand ( unsigned ( std::time(0) ) ); 
            std::random_shuffle( user_array.begin()+1, user_array.end() );
            user_array.resize( select );
        }

        void select_item()
        {
            timer t( "select item" );

            item_array.clear();
            std::set< item_id_type > selected_item;
             
/*
            auto& mr = singleton<mongo_rating>::instance(); 
            for ( auto const& user : user_array )
                for ( auto itor = mr.user_begin( user ); itor != mr.user_end( user ); ++itor )
                    selected_item.insert( std::get<1>( *itor ) );
*/
            //map<uid, map<iid, rat>> user_item_record



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
    
    };//struct mongo_rating_matrix_generator

}//namespace f

#endif//FYXUSXYKCVGQXGWEVPCXTPLYYDMVIVUBEAOWUOFEBNOIRTGGTGCDDOBETXFVCKEWUSCPODXOV

