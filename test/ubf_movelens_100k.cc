#include <engine/user_based_filtering/user_based_filtering.hpp>
#include <rating/rating.hpp>
#include <misc/print_tuple.hpp>
#include <f/matrix/matrix.hpp>

#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>

void gg()
{
    const unsigned long n_user = 943;
    const unsigned long n_item = 1682;
    f::matrix<double> a( n_user, n_item, -1.0 );

    f::mongo_rating mr( "localhost", "movelens.ml_100k_ua_base" );

    unsigned long counter = 1;
    for ( auto itor = mr.begin(); itor != mr.end(); ++itor )
    {
        auto const& record = *itor;
        unsigned long uid = boost::lexical_cast<unsigned long>( std::get<0>( record ) ); 
        unsigned long iid = boost::lexical_cast<unsigned long>( std::get<1>( record ) ); 
        double rating = std::get<2>( record );

        if ( uid < n_user+1 && iid < n_item+1 )
            a[uid-1][iid-1] = rating;
        else
            std::cerr << "\ninvalid record <" << uid << "," << iid << "," << rating << "> found.\n";

        if ( !counter++ & 0xff )
            std::cout << "\nthe " << counter << "th element loaded from db.";

    }

    std::cout << "\nmatrix A loaded.\n";

    //f::user_based_filtering<double> ubf( a );
    f::user_based_filtering ubf( a );
    auto const& pred_a = ubf();
    std::cout << "\nprediction matrix calculated.\n";

    char const * const path = "/home/feng/workspace/rc.data.set/set/movielens/ml-100k/ua.test";

    std::ifstream ifs( path );

    unsigned long uid;
    unsigned long iid;
    double r;
    unsigned long dummy;

    double diff_sum = 0.0;

    while( ifs >> uid >> iid >> r >> dummy )
    {
        double const diff = r - pred_a[uid-1][iid-1];
        diff_sum += diff*diff;
        std::cout.precision( 10 );
        std::cout << r << "\t\t" << pred_a[uid-1][iid-1] << "\n";
    }

    std::cout << "\nprediction error is " << std::sqrt( diff_sum / 9429.0 ) << "\n";
}

int main()
{
    gg();

    return 0;
}

