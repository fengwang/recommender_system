#include <rating/rating.hpp>
#include <misc/print_tuple.hpp>

#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>
#include <f/variate_generator/variate_generator.hpp>
//"/home/feng/workspace/rc.data.set/set/movielens/ml-100k/ua.base"
void gg()
{

    f::mongo_rating mr( "localhost", "movelens.ml_100k_ua_base" );
    
    char const * const path = "/home/feng/workspace/rc.data.set/set/movielens/ml-100k/ua.base";

    std::ifstream ifs( path );

    unsigned long uid;
    unsigned long iid;
    double r;
    unsigned long dummy;

    while( ifs >> uid >> iid >> r >> dummy )
    {
        mr.insert( std::make_tuple( boost::lexical_cast<std::string>( uid ), boost::lexical_cast<std::string>( iid ), r ) );
    }

    std::cout << "\nthere are " << mr.count() << " rating records in db.\n";

}

int main()
{
    gg();

    return 0;
}

