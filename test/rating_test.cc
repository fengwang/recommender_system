#include <rating/rating.hpp>
#include <misc/print_tuple.hpp>
#include <iostream>

#include <f/variate_generator/variate_generator.hpp>

void gg()
{

    f::mongo_rating mr( "localhost", "test.rating_test_oid" );

    f::variate_generator<long> r1( 1, 3 );
    f::variate_generator<char> r2( 'a', 'z'+1 );
    f::variate_generator<double> r3( 0.0, 10.0 );

    std::string last_user;
    std::string last_item;
    for ( unsigned long i = 0; i != 1000; ++i )
    {
        std::string uid( r1(), ' ');
        std::string iid( r1(), ' ');

        std::generate( uid.begin(), uid.end(), r2 );
        std::generate( iid.begin(), iid.end(), r2 );

        mr.insert( std::make_tuple( uid, iid, r3() ) );

        last_user = uid;
        last_item = iid;
    }

    for ( auto i = mr.begin(); i != mr.end(); ++i )
    {
        std::cout << *i << "\n";
    }

    std::cout << "\nfor user " << last_user << ", there are " << mr.user_rating_account( last_user ) << " records.\n";
    for ( auto i = mr.user_begin(last_user); i != mr.user_end(last_user); ++i )
    {
        std::cout << *i << "\n";
    }

    std::cout << "\nfor item " << last_item << ", there are " << mr.item_rating_account( last_item ) << " records.\n";
    for ( auto i = mr.item_begin(last_item); i != mr.item_end(last_item); ++i )
    {
        std::cout << *i << "\n";
    }

}

int main()
{
    gg();

    return 0;
}

