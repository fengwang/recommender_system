#ifndef NCSKAWCDJTICLRTBFGWUQLNOXODLKULKLWJHKGJLGYQMIIQCKKEFHXHCPVARLUTWNJLYJLJCV
#define NCSKAWCDJTICLRTBFGWUQLNOXODLKULKLWJHKGJLGYQMIIQCKKEFHXHCPVARLUTWNJLYJLJCV

#include <iostream>

#include <f/triplet/triplet.hpp>
#include <f/timer/timer.hpp>

#include <cstdint>
#include <string>
#include <memory>
#include <cassert>
#include <iterator>
#include <cstddef>

#include <mongo/client/dbclient.h>
#include <mongo/bson/bsonobj.h>
#include <mongo/bson/bsonobjbuilder.h>

namespace f
{
    struct mongo_rating_iterator
    {
        typedef rating_typedef::user_id_type                        user_id_type;
        typedef rating_typedef::item_id_type                        item_id_type;
        typedef rating_typedef::rating_type                         rating_type;
        typedef rating_typedef::rating_record_type                  rating_record_type;
        typedef rating_record_type                                  value_type;
        typedef void                                                pointer;
        typedef void                                                reference;
        typedef std::size_t                                         size_type;
        typedef std::ptrdiff_t                                      difference_type;
        typedef std::input_iterator_tag                             iterator_category;
        typedef mongo::DBClientConnection                           connection_type;
        typedef mongo::DBClientCursor                               cursor_type;
        typedef std::shared_ptr<connection_type>                    connection_pointer;
        typedef std::shared_ptr<cursor_type>                        cursor_pointer;

        cursor_pointer      cursor;
        bool                empty_flag;

        mongo_rating_iterator( cursor_pointer cursor_, bool empty_flag_ ): cursor( cursor_ ), empty_flag( empty_flag_ ) {}

        value_type const operator *() 
        {
            assert( (*cursor).more() );
            auto const& p = (*cursor).next();

            user_id_type id;
            item_id_type item;
            rating_type rating;
            p["user_id"].Val(id);
            p["item_id"].Val(item);
            p["rating"].Val(rating);

            return std::make_tuple( id, item, rating );
            //return make_triplet( id, item, rating );
        }

        mongo_rating_iterator& operator ++()
        {
            if ( !(*cursor).more() )
                empty_flag = true;

            return (*this);
        }

        friend bool operator != ( const mongo_rating_iterator& lhs, const mongo_rating_iterator& rhs )
        {
            return lhs.empty_flag ^ rhs.empty_flag;
        }
                     
    };//mongo_rating_iterator 


    struct mongo_rating : rating<mongo_rating, mongo_rating_iterator>
    {
        typedef rating_typedef::user_id_type                        user_id_type;
        typedef rating_typedef::item_id_type                        item_id_type;
        typedef rating_typedef::rating_type                         rating_type;
        typedef rating_typedef::rating_record_type                  rating_record_type;
        typedef rating_record_type                                  value_type;

        typedef mongo_rating_iterator                               iterator;
        typedef std::string                                         string_type;
        typedef mongo::DBClientConnection                           connection_type;
        typedef mongo::DBClientCursor                               cursor_type;
        typedef std::shared_ptr<connection_type>                    connection_pointer;
        typedef std::shared_ptr<cursor_type>                        cursor_pointer;

        string_type                     host;
        string_type                     db;
        connection_pointer              c;
        bool                            has_been_initialized;

        mongo_rating(): has_been_initialized( false ) { }
        

        mongo_rating( string_type const& host_, string_type const& db_ ) 
        {
            initialize( host_, db_ ); 
        }

        void initialize( string_type const& host_, string_type const& db_ )
        {
            if ( has_been_initialized ) return;

            host = host_;
            db = db_;
            //connection with autoconneciton true
            c = std::make_shared<connection_type>(true);
            string_type error_message;
            if( !(*c).connect( host, error_message ) )
            {
                //show some error here
                assert( !error_message.c_str() );
            }
            has_been_initialized = true;
        }

        rating_type query_impl( const user_id_type& uid, const item_id_type& iid ) const
        {
            cursor_pointer cursor = (*c).query( db, QUERY( "user_id" << uid << "item_id" << iid ) );
            if ( (*cursor).more() )
            {
                auto const& p = (*cursor).next();
                rating_type rating;
                p["rating"].Val(rating);
                return rating;
            }
            return rating_type(-1);
        }

        iterator begin_impl() const
        {
            cursor_pointer p = (*c).query( db, mongo::BSONObj() );
            if ( (*p).more() )
            {
                return iterator( p, false );
            }
            return iterator( p, true );
        }

        iterator end_impl() const
        {
            return iterator( nullptr, true );
        }

        iterator user_begin_impl( const user_id_type& user_id ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "user_id" << user_id ) );
            if ( (*p).more() )
                return iterator( p, false );
            return iterator( p, true );
        }

        iterator user_end_impl( const user_id_type& ) const
        {
            return iterator( nullptr, true );
        }

        unsigned long user_rating_account_impl( const user_id_type& user_id ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "user_id" << user_id ) );
            return (*p).itcount();
        }

        iterator item_begin_impl( const item_id_type& item_id ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "item_id" << item_id ) );
            if ( (*p).more() )
                return iterator( p, false );
            return iterator( p, true );
        }

        iterator item_end_impl( const item_id_type& ) const
        {
            return iterator( nullptr, true );
        }

        unsigned long item_rating_account_impl( const item_id_type& item_id ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "item_id" << item_id ) );
            return (*p).itcount();
        }

        void insert_impl( const rating_record_type& rat ) const
        {
            remove_impl( std::get<0>( rat ), std::get<1>( rat ) );
            auto const& p = mongo::BSONObjBuilder().genOID().append("user_id", std::get<0>(rat)).append("item_id",std::get<1>(rat)).append("rating", std::get<2>(rat)).obj();
            (*c).insert( db, p );
        }
        
        void remove_impl( const user_id_type& user_id, const item_id_type& item_id ) const
        {
            (*c).remove( db, mongo::BSONObj{BSON("user_id" << user_id << "item_id" << item_id)} );
        }

        void update_impl( const rating_record_type& rat_old, const rating_record_type& rat_new ) const
        {
            auto const& p = mongo::BSONObjBuilder().genOID().append("user_id", std::get<0>(rat_new)).append("item_id", std::get<1>(rat_new)).append("rating", std::get<2>(rat_new)).obj();
            (*c).update( db, mongo::BSONObj{BSON("user_id" << std::get<0>(rat_old) << "item_id" << std::get<1>(rat_old))}, p );
        }

        unsigned long count_impl( ) const
        {
            cursor_pointer p = (*c).query( db, mongo::BSONObj() );
            return (*p).itcount();
        }
    
    };

}//namespace f

#endif//NCSKAWCDJTICLRTBFGWUQLNOXODLKULKLWJHKGJLGYQMIIQCKKEFHXHCPVARLUTWNJLYJLJCV
