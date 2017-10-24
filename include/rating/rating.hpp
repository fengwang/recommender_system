#ifndef PNVUIGMRKDWVRIXDWYWMTHFFIUYXBSJGDFVFIPMHDFXUUHWJLDPHGOEIJQYDXVFCAQWWMPFMM
#define PNVUIGMRKDWVRIXDWYWMTHFFIUYXBSJGDFVFIPMHDFXUUHWJLDPHGOEIJQYDXVFCAQWWMPFMM

#include <tuple>
#include <string>

namespace f
{
    //construct a class, with begin() and end() method, which connected to database
    //
    // for ( auto sth = rating.begin(); sth != rating.end(); ++sth )
    //     rating_matrix.insert( *sth.first, *sth.second, *sth.third );
    //
    // should leaving room for other implementations, i.e. mysql, mongodb, postsql, etd...

    /*
     * Field is [user_id, item_id, rating]
     */
    struct rating_typedef
    {
        typedef std::string                                             user_id_type;
        typedef std::string                                             item_id_type;
        typedef double                                                  rating_type;
        typedef std::tuple<user_id_type, item_id_type, rating_type>     rating_record_type;
        //typedef f::triplet<user_id_type, item_id_type, rating_type> rating_record_type;
    };

    template< typename Implementation, typename Iterator >
    struct rating
    {
        typedef Implementation                                      zen_type;
        typedef Iterator                                            iterator;
        typedef rating_typedef::user_id_type                        user_id_type;
        typedef rating_typedef::item_id_type                        item_id_type;
        typedef rating_typedef::rating_type                         rating_type;
        typedef rating_typedef::rating_record_type                  rating_record_type;

        rating_type query( const user_id_type& uid, const item_id_type& iid ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.query_impl(uid, iid);
        }

        //iterator for the whole rating matrix
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

        // idea: create a singleton for it
        //iterator for a specified user
        iterator user_begin( const user_id_type& id ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.user_begin_impl( id );
        }
        iterator user_end( const user_id_type& id ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.user_end_impl( id );
        }

        unsigned long user_rating_account( const user_id_type& user_id ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.user_rating_account_impl( user_id );
        }

        //iterator for a specified item
        iterator item_begin( const item_id_type& id ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.item_begin_impl( id );
        }
        iterator item_end( const item_id_type& id ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.item_end_impl( id );
        }

        unsigned long item_rating_account( const item_id_type& item_id ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.item_rating_account_impl( item_id );
        }

        //insert a new record, will override if old one exits
        void insert( const rating_record_type& rat ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            zen.insert_impl( rat );
        }

        //TODO: test this method
        //remove a record
        void remove( const user_id_type& user_id, const item_id_type& item_id ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            zen.remove_impl( user_id, item_id );
        }

        //TODO: test this method
        //update an existing record
        void update( const rating_record_type& rat_old, const rating_record_type& rat_new ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            zen.update_impl( rat_old, rat_new );
        }

        //total rating records in db
        unsigned long count() const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.count_impl();
        }

    };

}//namespace f

#include "impl/mongo_rating.tcc"


#endif//PNVUIGMRKDWVRIXDWYWMTHFFIUYXBSJGDFVFIPMHDFXUUHWJLDPHGOEIJQYDXVFCAQWWMPFMM

