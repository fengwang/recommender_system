//#include <engine/user_based_filtering/user_based_filtering.hpp>
//#include <engine/rating_matrix_generator/rating_matrix_generator.hpp>
#include <engine/prediction/prediction.hpp>
//#include <rating/rating.hpp>
#include <misc/print_tuple.hpp>
#include <f/matrix/matrix.hpp>

#include <f/singleton/singleton.hpp>

#include <iostream>
#include <fstream>

#include <boost/lexical_cast.hpp>

void gg()
{
    //f::mongo_rating_matrix_generator mrmg( "localhost", "movelens.ml_100k_ua_base" );
    f::mongo_prediction<> mp( "localhost", "movelens.ml_100k_ua_base" );
    const std::string uid( "1" );
    auto const& prediction = mp( uid );
    std::cout << "\nthe prediction for uid " << uid << " is:\n";

    for ( auto const& pred : prediction )
        std::cout << pred.first << "\t" << pred.second << "\n";
}

int main()
{
    gg();

    return 0;
}

