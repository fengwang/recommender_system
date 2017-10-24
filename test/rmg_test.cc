#include <engine/user_based_filtering/user_based_filtering.hpp>
#include <engine/rating_matrix_generator/rating_matrix_generator.hpp>
#include <rating/rating.hpp>
#include <misc/print_tuple.hpp>
#include <f/matrix/matrix.hpp>

#include <f/singleton/singleton.hpp>

#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>

void gg()
{
    //f::mongo_rating_matrix_generator mrmg( "localhost", "movelens.ml_100k_ua_base" );
    f::item_based_mongo_rating_matrix_generator mrmg( "localhost", "movelens.ml_100k_ua_base" );

    mrmg( "943" );

    std::cout << "\nthe matrix generate is \n" << mrmg.rating_matrix.row() << " by " << mrmg.rating_matrix.col() << "\n";

    //f::user_based_filtering<double> ubf( mrmg.rating_matrix );
    f::user_based_filtering ubf( mrmg.rating_matrix );

    auto const& pred_a = ubf();
    std::cout << "\nprediction matrix calculated.\n";

    char const * const path = "/home/feng/workspace/rc.data.set/set/movielens/ml-100k/ua.test";

    std::ifstream ifs( path );

    unsigned long uid;
    unsigned long iid;
    double r;
    unsigned long dummy;

    double diff_sum = 0.0;

    auto& manager = f::singleton<f::mongo_rating_manager>::instance();
    manager.initialize( "localhost", "movelens.ml_100k_ua_base" );

    while( ifs >> uid >> iid >> r >> dummy )
    {
        if ( 943 == uid )
        {
            const unsigned long order = std::lower_bound( mrmg.item_array.begin(), mrmg.item_array.end(), boost::lexical_cast<std::string>(iid) ) - mrmg.item_array.begin();
            double const pred =  pred_a[0][order];
            double const diff = r - pred_a[0][order];
            diff_sum += diff*diff;
            std::cout.precision( 10 );
            std::cout << r << "\t\t" << pred << "\t\t" << manager.query(boost::lexical_cast<std::string>(uid), boost::lexical_cast<std::string>(iid)) << "\n";
        }
    }

    std::cout << "\nprediction error is " << std::sqrt( diff_sum / 9429.0 ) << "\n";
}

int main()
{
    gg();

    return 0;
}

