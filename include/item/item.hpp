#ifndef CADLHNPSCQPLVMNGOCYUBXLGTETUYSFXNEAOBXLKUQRDCNFJBMDAJYKELYKRLQHRVXTRJYAFB
#define CADLHNPSCQPLVMNGOCYUBXLGTETUYSFXNEAOBXLKUQRDCNFJBMDAJYKELYKRLQHRVXTRJYAFB

//#include <f/triplet/triplet.hpp>
#include <string>
#include <tuple>

namespace f
{

    /*
     * Field is [ _id, title, description ]
     */
    struct item_typedef
    {
        typedef std::string                                                 item_id_type;
        typedef std::string                                                 title_type;
        typedef std::string                                                 description_type;

        typedef std::tuple<item_id_type, title_type, description_type>   item_record_type;
        //typedef f::triplet<item_id_type, title_type, description_type> item_record_type;
    };

    template< typename Implementation, typename Iterator >
    struct item
    {
        typedef Implementation                                      zen_type;
        typedef Iterator                                            iterator;
        typedef item_typedef::item_id_type                          item_id_type;
        typedef item_typedef::title_type                            title_type;
        typedef item_typedef::description_type                       description_type;
        typedef item_typedef::item_record_type                      item_record_type;

        //iterator for the whole item list
        iterator begin() const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.begin_impl();
        }
        
        iterator end() const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.end_impl();
        }

        //convert title address to id, returns false if failed
        bool title_to_id( const title_type& title, item_id_type& id ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.title_to_id_impl( title, id );
        }

        //convert id to title address, returns false if failed
        bool id_to_title( const item_id_type& id, title_type& title ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.id_to_title_impl( id, title );
        }
/*
        //check if title-password match or not
        bool verify( const title_type& title, const description_type& description ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.verify_impl( title, description );
        }
        */

        //insert a new item 
        //void insert( const title_type& title, const description_type& description ) const
        void insert( const item_record_type& item_record ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            zen.insert_impl( item_record );
        }

        void insert( const title_type& title, const description_type& description ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            zen.insert_impl( title, description );
        }

        //remove a item
        bool remove( const title_type& title ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.remove_impl( title );
        }

        //update the description of an existing item
        void update( const title_type& title, const description_type& description ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            zen.update_impl( title, description );
        }

        //query if an title has been registered or not
        bool query( const title_type& title )
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.query_impl( title );
        }

        //return the numbers of all items
        int count() const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.count_impl();
        }

    };

}//namespace f

#include "impl/mongo_item.tcc"

#endif//CADLHNPSCQPLVMNGOCYUBXLGTETUYSFXNEAOBXLKUQRDCNFJBMDAJYKELYKRLQHRVXTRJYAFB

