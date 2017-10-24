#ifndef NKSRXURRVMIGELFKSLWKJDPMOHNLHHSOYVRMOBLXOQIPCKUAKCRBMCCXKWALGQGXXQXNIUOSB
#define NKSRXURRVMIGELFKSLWKJDPMOHNLHHSOYVRMOBLXOQIPCKUAKCRBMCCXKWALGQGXXQXNIUOSB

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
    struct mongo_item_iterator
    {
        typedef item_typedef::item_id_type                          item_id_type;
        typedef item_typedef::title_type                            title_type;
        typedef item_typedef::description_type                      description_type;
        typedef item_typedef::item_record_type                      item_record_type;
        typedef item_record_type                                    value_type;
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

        mongo_item_iterator( cursor_pointer cursor_, bool empty_flag_ ): cursor( cursor_ ), empty_flag( empty_flag_ ) {}

        value_type const operator *() 
        {
            item_id_type            id;
            title_type              title;
            description_type        description;

            assert( (*cursor).more() );
            auto const& p = (*cursor).next();
            //p["_id"].Val( id );
            id = p["_id"].OID().str();
            p["title"].Val( title );
            p["description"].Val( description );

            return std::make_tuple( id, title, description );
        }

        mongo_item_iterator& operator ++()
        {
            if ( !(*cursor).more() )
                empty_flag = true;

            return (*this);
        }

        friend bool operator != ( const mongo_item_iterator& lhs, const mongo_item_iterator& rhs )
        {
            return lhs.empty_flag ^ rhs.empty_flag;
        }
                     
    };//mongo_item_iterator 

    struct mongo_item : item<mongo_item, mongo_item_iterator>
    {
        typedef item_typedef::item_id_type                          item_id_type;
        typedef item_typedef::title_type                            title_type;
        typedef item_typedef::description_type                      description_type;
        typedef item_typedef::item_record_type                      item_record_type;
        typedef item_record_type                                    value_type;

        typedef mongo_item_iterator                                 iterator;
        typedef std::string                                         string_type;
        typedef mongo::DBClientConnection                           connection_type;
        typedef mongo::DBClientCursor                               cursor_type;
        typedef std::shared_ptr<connection_type>                    connection_pointer;
        typedef std::shared_ptr<cursor_type>                        cursor_pointer;

        string_type                     host;
        string_type                     db;
        connection_pointer              c;

        mongo_item( string_type const& host_, string_type const& db_ ) : host( host_ ), db( db_ )
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

        bool title_to_id_impl( const title_type& title, item_id_type& id ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "title" << title ) );
            if ( (*p).more() )
            {
                auto const& obj = (*p).next();
                id = obj["_id"].OID().str();
                return true;
            }
            return false;
        }

        bool id_to_title_impl( const item_id_type& id, title_type& title ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "_id" << mongo::OID(id) ) );
            if ( (*p).more() )
            {
                auto const& obj = (*p).next();
                obj["title"].Val(title);
                return true;
            }
            return false;
        }
/*
        bool verify_impl( const title_type& title, const description_type& description ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "title" << title ) );
            if ( (*p).more() )
            {
                auto const& obj = (*p).next();
                description_type description_in_db;
                obj["description"].Val(description_in_db);
                if ( description == description_in_db ) 
                    return true;
            }
            return false;
        }
        */

        void insert_impl( const item_record_type& item_record ) const
        {
            auto const& p = mongo::BSONObjBuilder().append("_id", mongo::OID(std::get<0>(item_record))).append("title", std::get<1>(item_record)).append("description", std::get<2>(item_record)).obj();
            (*c).insert( db, p );
            (*c).ensureIndex(db, BSON( "title" << 1 ), true, "", true, true);
        }

        void insert_impl( const title_type& title, const description_type& description ) const
        {
            auto const& p = mongo::BSONObjBuilder().genOID().append("title", title).append("description", description).obj();
            (*c).insert( db, p );
            (*c).ensureIndex(db, BSON( "title" << 1 ), true, "", true, true);
        }

        bool remove_impl( const title_type& title ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "title" << title ) );
            if ( (*p).more() )
            {
                (*c).remove( db, mongo::BSONObj{BSON("title" << title)} );
                return true;
            }
            return false;
        }

        void update_impl( const title_type& title, const description_type& description ) const
        {
            item_id_type id;
            title_to_id_impl( title, id );

            cursor_pointer p = (*c).query( db, QUERY( "title" << title ) );
            if ( (*p).more() ) //if record found in data base, then remove old record then update news
            {
                auto const& obj = (*p).next();
                auto const& new_record = mongo::BSONObjBuilder().append("_id", obj["_id"]).append("title", title).append("description", description).obj();
                (*c).remove( db, mongo::BSONObj{BSON("title" << title)} );
                (*c).insert( db, new_record );
                (*c).ensureIndex(db, BSON( "title" << 1 ), true, "", true, true);
            }
            else //otherwise simple insert a new record
                insert_impl( title, description );
        }

        bool query_impl( const title_type& title ) const
        {
            cursor_pointer p = (*c).query( db, QUERY( "title" << title ) );
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

#endif//NKSRXURRVMIGELFKSLWKJDPMOHNLHHSOYVRMOBLXOQIPCKUAKCRBMCCXKWALGQGXXQXNIUOSB

