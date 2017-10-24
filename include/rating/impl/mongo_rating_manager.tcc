#ifndef IQMINAAFJPITVMAVVIJAPKMKNKKXSWALDFQVAQKGYJQUACMAQQDYHNRWNHRRYJUCQXCBMWIUQ
#define IQMINAAFJPITVMAVVIJAPKMKNKKXSWALDFQVAQKGYJQUACMAQQDYHNRWNHRRYJUCQXCBMWIUQ

#include <f/singleton/singleton.hpp>

#include <string>
#include <tuple>

namespace f
{
    //TODO:    mutex 
    struct mongo_rating_manager : rating_manager< mongo_rating_manager >
    {
        typedef rating_typedef::user_id_type                        user_id_type;
        typedef rating_typedef::item_id_type                        item_id_type;
        typedef rating_typedef::rating_type                         rating_type;
        typedef rating_typedef::rating_record_type                  rating_record_type;

        bool has_been_initialized;

        mongo_rating_manager()
        {
            has_been_initialized = false; 
        }

        mongo_rating_manager( std::string const& host, std::string const& db )
        {
            initialize( host, db );
        }

        void reinitialize( std::string const& host, std::string const& db )
        {
            //TODO
            //clear everything

            has_been_initialized = false;
            initialize( host, db );
        }

        void initialize( std::string const& host, std::string const& db )
        {
            if( has_been_initialized ) return;

            auto& mr = singleton<mongo_rating>::instance();
            mr.initialize( host, db );
            //mongo_rating mr( host, db );

            for ( auto itor = mr.begin(); itor != mr.end(); ++itor )
            {
                auto const& record = *itor;
                increase_user_rating_record_impl( std::get<0>( record ), std::get<1>( record), std::get<2>( record ) );
            }
            has_been_initialized = true;
        }

        void increase_user_rating_record_impl( const user_id_type& uid, const item_id_type& iid, const rating_type& val )
        {
            user_rating_record.insert( std::make_pair( uid, val ) );
            user_rating_account[uid]++;
            item_rating_record.insert( std::make_pair( iid, val ) );
            item_rating_account[iid]++;

            user_item_record.insert( std::make_pair( uid, iid ) );
            item_user_record.insert( std::make_pair( iid, uid ) );

            //useritem_rating_record.insert(  ); //just this case
            useritem_rating_record.insert( std::make_pair( std::string( uid + std::string( "<:-:>" ) + iid ), val ) ); //just this case
        }

        rating_type query_impl( const user_id_type& uid, const item_id_type& iid )
        {
            //lookup in useritem_rating_record
            const std::string& key = std::string( uid + std::string( "<:-:>" ) + iid );
            auto const& iter = useritem_rating_record.find( key );
            if ( iter ==  useritem_rating_record.end() ) return rating_type( -1 );
            return (*iter).second;
        }

        void remove_user_rating_record_impl( const user_id_type& uid, const item_id_type& iid, const rating_type& val )
        {
            //remove user rating record
            auto const& user_range = user_rating_record.equal_range( uid );
            unsigned long const user_count = std::distance( user_range.first, user_range.second );

            if ( user_count )
            {
                if ( 1 == user_count )
                {
                    user_rating_record.erase( uid );
                    user_rating_account.erase( uid );
                }
                else
                {
                    for ( auto itor = user_range.first; itor != user_range.second; ++itor )
                        if ( (*itor).second == val )
                        {
                            user_rating_record.erase( itor );
                            break;
                        }
                    user_rating_account[uid]--;
                }
            }

            //remove item rating record
            auto const& item_range = item_rating_record.equal_range( iid );
            unsigned long const item_count = std::distance( item_range.first, item_range.second );
            if ( item_count )
            {
                if ( 1 == item_count )
                {
                    item_rating_record.erase( iid );
                    item_rating_account.erase( iid );
                }
                else
                {
                    for ( auto itor = item_range.first; itor != item_range.second; ++itor )
                        if ( (*itor).second == val )
                        {
                            item_rating_record.erase( itor );
                            break;
                        }
                    item_rating_account[iid]--;
                }
            }

            //remove in user_item_record
            auto const& user_item_range = user_item_record.equal_range( uid );
            for ( auto itor = user_item_range.first; itor != user_item_range.second; ++itor )
            {
                if ( (*itor).second == iid )
                {
                    user_item_record.erase( itor );
                    break;
                }
            }

            //TODO remove in useritem_rating_record
        }

        void remove_user_impl( const user_id_type& uid )
        {
            user_rating_record.erase( uid );
            user_rating_account.erase( uid );
            user_item_record.erase( uid );
        }

        void remove_item_impl( const item_id_type& iid )
        {
            item_rating_record.erase( iid );
            item_rating_account.erase( iid );
            //TODO: remove record in user_item_record
            //TODO: remove record in useritem_rating_record
        }
         
    };//struct mongo_rating_manager
    
}//namespace f

#endif//IQMINAAFJPITVMAVVIJAPKMKNKKXSWALDFQVAQKGYJQUACMAQQDYHNRWNHRRYJUCQXCBMWIUQ

