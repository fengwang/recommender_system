#ifndef NCSKAWCDJTICLRTBFGWUQLNOXODLKULKLWJHKGJLGYQMIIQCKKEFHXHCPVARLUTWNJLYJLJCV
#define NCSKAWCDJTICLRTBFGWUQLNOXODLKULKLWJHKGJLGYQMIIQCKKEFHXHCPVARLUTWNJLYJLJCV

//#include <f/triplet/triplet.hpp>
#include <tuple>

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
    struct mongo_user_iterator
    {
        typedef user_typedef::user_id_type                          user_id_type;
        typedef user_typedef::email_address_type                    email_address_type;
        typedef user_typedef::passwd_type                           passwd_type;
        typedef user_typedef::user_record_type                      user_record_type;
        typedef user_record_type                                    value_type;
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

        mongo_user_iterator( cursor_pointer cursor_, bool empty_flag_ ): cursor( cursor_ ), empty_flag( empty_flag_ ) {}

        value_type const operator *() 
        {
            user_id_type            id;
            email_address_type      email;
            passwd_type             passwd;

            assert( (*cursor).more() );
            auto const& p = (*cursor).next();
            //p["_id"].Val( id );
            id = p["_id"].OID().str();
            p["email_address"].Val( email );
            p["passwd"].Val( passwd );

            return std::make_tuple( id, email, passwd );
            //return make_triplet( static_cast<user_id_type>(p["_id"].numberLong()), static_cast<email_address_type>(p["email_address"].String()), static_cast<passwd_type>(p["passwd"].String()) );
        }

        mongo_user_iterator& operator ++()
        {
            if ( !(*cursor).more() )
                empty_flag = true;

            return (*this);
        }

        friend bool operator != ( const mongo_user_iterator& lhs, const mongo_user_iterator& rhs )
        {
            return lhs.empty_flag ^ rhs.empty_flag;
        }
                     
    };//mongo_user_iterator 

    struct mongo_user : user<mongo_user, mongo_user_iterator>
    {
        typedef user_typedef::user_id_type                          user_id_type;
        typedef user_typedef::email_address_type                    email_address_type;
        typedef user_typedef::passwd_type                           passwd_type;
        typedef user_typedef::user_record_type                      user_record_type;
        typedef user_record_type                                    value_type;

        typedef mongo_user_iterator                                 iterator;
        typedef std::string                                         string_type;
        typedef mongo::DBClientConnection                           connection_type;
        typedef mongo::DBClientCursor                               cursor_type;
        typedef std::shared_ptr<connection_type>                    connection_pointer;
        typedef std::shared_ptr<cursor_type>                        cursor_pointer;

        string_type                     host;
        string_type                     db;
        connection_pointer              c;

        mongo_user( string_type const& host_, string_type const& db_ ) : host( host_ ), db( db_ )
        {
            c = std::make_shared<connection_type>(true);
            string_type error_message;
            if( !(*c).connect( host, error_message ) ) assert( !error_message.c_str() );
        }

        iterator begin_impl() const
        {
            cursor_pointer p = (*c).query( db, mongo::BSONObj() );
            if ( (*p).more() ) return iterator( p, false );
            return iterator( p, true );
        }

        iterator end_impl() const
        {
            return iterator( nullptr, true );
        }

        bool email_to_id_impl( const email_address_type& email, user_id_type& id ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "email_address" << email ) );
            if ( (*p).more() )
            {
                auto const& obj = (*p).next();
                id = obj["_id"].OID().str();
                return true;
            }
            return false;
        }

        bool id_to_email_impl( const user_id_type& id, email_address_type& email ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "_id" << mongo::OID(id) ) );
            if ( (*p).more() )
            {
                auto const& obj = (*p).next();
                obj["email_address"].Val(email);
                return true;
            }
            return false;
        }

        bool verify_impl( const email_address_type& email, const passwd_type& passwd ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "email_address" << email ) );
            if ( (*p).more() )
            {
                auto const& obj = (*p).next();
                passwd_type passwd_in_db;
                obj["passwd"].Val(passwd_in_db);
                if ( passwd == passwd_in_db ) 
                    return true;
            }
            return false;
        }

        void insert_impl( const user_record_type& user_record ) const
        {
            auto const& p = mongo::BSONObjBuilder().append("_id", mongo::OID(std::get<0>(user_record))).append("email_address", std::get<1>(user_record)).append("passwd", std::get<2>(user_record)).obj();
            (*c).insert( db, p );
            (*c).ensureIndex(db, BSON( "email" << 1 ), true, "", true, true);
        }

        void insert_impl( const email_address_type& email, const passwd_type& passwd ) const
        {
            auto const& p = mongo::BSONObjBuilder().genOID().append("email_address", email).append("passwd", passwd).obj();
            (*c).insert( db, p );
            (*c).ensureIndex(db, BSON( "email_address" << 1 ), true, "", true, true);
        }

        bool remove_impl( const email_address_type& email ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "email_address" << email ) );
            if ( (*p).more() )
            {
                (*c).remove( db, mongo::BSONObj{BSON("email_address" << email)} );
                return true;
            }
            return false;
        }

        void update_impl( const email_address_type& email, const passwd_type& passwd ) const
        {
            user_id_type id;
            email_to_id_impl( email, id );

            cursor_pointer p = (*c).query( db, QUERY( "email_address" << email ) );
            if ( (*p).more() ) //if record found in data base, then remove old record then update news
            {
                auto const& obj = (*p).next();
                auto const& new_record = mongo::BSONObjBuilder().append("_id", obj["_id"]).append("email_address", email).append("passwd", passwd).obj();
                (*c).remove( db, mongo::BSONObj{BSON("email_address" << email)} );
                (*c).insert( db, new_record );
                (*c).ensureIndex(db, BSON( "email" << 1 ), true, "", true, true);
            }
            else //otherwise simple insert a new record
                insert_impl( email, passwd );
        }

        bool query_impl( const email_address_type& email ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "email_address" << email ) );
            if ( (*p).more() )
                return true;
            return false;
        }
   
        int count_impl() const
        {
            cursor_pointer p = (*c).query( db, mongo::BSONObj() );
            return (*p).itcount();
        }

    };

}//namespace f 

#endif//NCSKAWCDJTICLRTBFGWUQLNOXODLKULKLWJHKGJLGYQMIIQCKKEFHXHCPVARLUTWNJLYJLJCV

