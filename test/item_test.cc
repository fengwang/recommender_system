#include <item/item.hpp>
#include <misc/print_tuple.hpp>

#include <iostream>
#include <algorithm>
#include <f/variate_generator/variate_generator.hpp>

static void gg()
{

    f::mongo_item mu( "localhost", "test.item_test_oid" );

    f::variate_generator<char> rc( 'a', 'z'+1);           //generate char
    f::variate_generator<long long> r1( 1, 10 );        //title/description length

    std::string last_title;
    std::string last_description;

    for ( unsigned long i = 0; i != 100; ++i )
    {
        unsigned long title_length = r1();
        unsigned long description_length = r1();
        std::string title( title_length, ' ' );
        std::string description( description_length, ' ' );
        std::generate( title.begin(), title.end(), rc );
        std::generate( description.begin(), description.end(), rc );
        mu.insert( title, description );
        last_title = title;
        last_description = description;
    }

    /*
    if ( mu.verify( last_title, last_description ) )
        std::cout << "\nverify works good.\n";
    else
        std::cout << "\nverify does not work.\n";
        */

    for ( auto i = mu.begin(); i != mu.end(); ++i )
    {
        std::cout << *i << "\n";
    }

    std::string id;
    std::string title;

    //test id_to_title and title_to_id
    mu.title_to_id( last_title, id );
    std::cout << "\nid-" << id << " : title-" << last_title << "\n";
    mu.id_to_title( id, title );
    std::cout << "\nid-" << id << " : title-" << title << "\n";
    mu.title_to_id( title, id );
    std::cout << "\nid-" << id << " : title-" << title << "\n";
    
    mu.remove( title );
    if ( !mu.query( title ) )
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

