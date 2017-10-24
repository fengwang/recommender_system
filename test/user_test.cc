#include <user/user.hpp>
#include <misc/print_tuple.hpp>

#include <iostream>
#include <algorithm>
#include <f/variate_generator/variate_generator.hpp>

static void gg()
{

    f::mongo_user mu( "localhost", "test.user_test_oid" );

    f::variate_generator<char> rc( 'a', 'z'+1);           //generate char
    f::variate_generator<long long> r1( 1, 10 );        //email/passwd length

    std::string last_email;
    std::string last_passwd;

    for ( unsigned long i = 0; i != 100; ++i )
    {
        unsigned long email_length = r1();
        unsigned long passwd_length = r1();
        std::string email( email_length, ' ' );
        std::string passwd( passwd_length, ' ' );
        std::generate( email.begin(), email.end(), rc );
        std::generate( passwd.begin(), passwd.end(), rc );
        mu.insert( email, passwd );
        last_email = email;
        last_passwd = passwd;
    }

    if ( mu.verify( last_email, last_passwd ) )
        std::cout << "\nverify works good.\n";
    else
        std::cout << "\nverify does not work.\n";

    for ( auto i = mu.begin(); i != mu.end(); ++i )
    {
        std::cout << *i << "\n";
    }

    std::string id;
    std::string email;

    //test id_to_email and email_to_id
    mu.email_to_id( last_email, id );
    std::cout << "\nid-" << id << " : email-" << last_email << "\n";
    mu.id_to_email( id, email );
    std::cout << "\nid-" << id << " : email-" << email << "\n";
    mu.email_to_id( email, id );
    std::cout << "\nid-" << id << " : email-" << email << "\n";
    

    mu.update( email, last_passwd );
    if( mu.verify( email, last_passwd ) )
        std::cout << "\nupdate works.\n";
    else
        std::cout << "\nupdate does not work.\n";
    mu.remove( email );
    if ( !mu.query( email ) )
        std::cout << "\nremove/query works good.\n";
    else
        std::cout << "\nremove/query does not work.\n";

    std::cout << "\ncurrent there are " << mu.count() << " records in database.\n";
}

int main()
{
    gg();

    return 0;
}

