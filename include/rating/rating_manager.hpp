#ifndef ASLVEXCFOAQYOECHHQPUEQIBBGCBXCKFUKBRNXFKUJOWALGXNUFRTXNTWAUMJEADEHJQDPBIC
#define ASLVEXCFOAQYOECHHQPUEQIBBGCBXCKFUKBRNXFKUJOWALGXNUFRTXNTWAUMJEADEHJQDPBIC

#include <rating/rating.hpp>

#include <map>
#include <string>

namespace f
{
   
    //should be a singleton
    template< typename Implementation >
    struct rating_manager
    {
        //rating_typedef defined in file <rating/rating.hpp>
        typedef Implementation                              zen_type;
        typedef rating_typedef::user_id_type                user_id_type;
        typedef rating_typedef::item_id_type                item_id_type;
        typedef rating_typedef::rating_type                 rating_type;
        typedef rating_typedef::rating_record_type          rating_record_type;

        std::multimap<user_id_type, rating_type>            user_rating_record;
        std::map<user_id_type, unsigned long>               user_rating_account;
        std::multimap<item_id_type, rating_type>            item_rating_record;
        std::map<item_id_type, unsigned long>               item_rating_account;
        std::multimap<user_id_type, item_id_type>           user_item_record;
        std::multimap<item_id_type, user_id_type>           item_user_record;
        std::map<std::string, rating_type>                  useritem_rating_record;

        void increase_user_rating_record( const user_id_type& uid, const item_id_type& iid, const rating_type& val )
        {
            auto& zen = static_cast<zen_type&>( *this );
            zen.increase_user_rating_record_impl( uid, iid, val );
        }

        void remove_user_rating_record( const user_id_type& uid, const item_id_type& iid, const rating_type& val )
        {
            auto& zen = static_cast<zen_type&>( *this );
            zen.remove_user_rating_record_impl( uid, iid, val );
        }

        void remove_user( const user_id_type& uid )
        {
            auto& zen = static_cast<zen_type&>( *this );
            zen.remove_user_impl( uid );
        }

        void remove_item( const item_id_type& iid )
        {
            auto& zen = static_cast<zen_type&>( *this );
            zen.remove_item_impl( iid );
        }

        rating_type query( const user_id_type& uid, const item_id_type& iid )
        {
            auto& zen = static_cast<zen_type&>( *this );
            return zen.query_impl( uid, iid );
        }
    
    };

}//namespace f

#include "impl/mongo_rating_manager.tcc"

#endif//ASLVEXCFOAQYOECHHQPUEQIBBGCBXCKFUKBRNXFKUJOWALGXNUFRTXNTWAUMJEADEHJQDPBIC
