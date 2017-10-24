#ifndef SF09U4OISHFLKAJSHAKLJH49Y8SAFDKLJHASLKJAHKLJVNAOUIFH9WREHSFDKHSFDUHWEUIH
#define SF09U4OISHFLKAJSHAKLJH49Y8SAFDKLJHASLKJAHKLJVNAOUIFH9WREHSFDKHSFDUHWEUIH 

//#include <f/triplet/triplet.hpp>
#include <string>
#include <tuple>

namespace f
{

    /*
     * Field is [ _id, email_address, passwd ]
     */
    struct user_typedef
    {
        typedef std::string                                                 user_id_type;
        typedef std::string                                                 email_address_type;
        typedef std::string                                                 passwd_type;

        typedef std::tuple<user_id_type, email_address_type, passwd_type>   user_record_type;
        //typedef f::triplet<user_id_type, email_address_type, passwd_type> user_record_type;
    };

    template< typename Implementation, typename Iterator >
    struct user
    {
        typedef Implementation                                      zen_type;
        typedef Iterator                                            iterator;
        typedef user_typedef::user_id_type                          user_id_type;
        typedef user_typedef::email_address_type                    email_address_type;
        typedef user_typedef::passwd_type                           passwd_type;
        typedef user_typedef::user_record_type                      user_record_type;

        //iterator for the whole user list
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

        //convert email address to id, returns false if failed
        bool email_to_id( const email_address_type& email, user_id_type& id ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.email_to_id_impl( email, id );
        }

        //convert id to email address, returns false if failed
        bool id_to_email( const user_id_type& id, email_address_type& email ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.id_to_email_impl( id, email );
        }

        //check if email_address-password match or not
        bool verify( const email_address_type& email, const passwd_type& passwd ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.verify_impl( email, passwd );
        }

        //insert a new user 
        //void insert( const email_address_type& email, const passwd_type& passwd ) const
        void insert( const user_record_type& user_record ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            zen.insert_impl( user_record );
        }

        void insert( const email_address_type& email, const passwd_type& passwd ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            zen.insert_impl( email, passwd );
        }

        //remove a user
        bool remove( const email_address_type& email ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.remove_impl( email );
        }

        //update the passwd of an existing user
        void update( const email_address_type& email, const passwd_type& passwd ) const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            zen.update_impl( email, passwd );
        }

        //query if an email has been registered or not
        bool query( const email_address_type& email )
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.query_impl( email );
        }

        //return the numbers of all users
        int count() const
        {
            auto const& zen = static_cast<zen_type const&>(*this);
            return zen.count_impl();
        }

    };

}//namespace f

#include "impl/mongo_user.tcc"

#endif//SF09U4OISHFLKAJSHAKLJH49Y8SAFDKLJHASLKJAHKLJVNAOUIFH9WREHSFDKHSFDUHWEUIH 

