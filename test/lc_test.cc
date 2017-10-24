#include <engine/numeric/linear_correlation.hpp>
#include <f/variate_generator/variate_generator.hpp>

#include <iostream>
#include <vector>

int main()
{
    double low = -1.0;
    double high = 1.0;
    f::variate_generator<double> vg( low, high );

    unsigned long const n = 10;

    std::vector<double> a( vg.begin(), vg.begin()+n );
    std::vector<double> b( vg.begin(), vg.begin()+n );

    std::cout << "\nthe array a is \n";
    std::copy( a.begin(), a.end(), std::ostream_iterator<double>( std::cout, " " ) );
    std::cout << "\nthe array b is \n";
    std::copy( b.begin(), b.end(), std::ostream_iterator<double>( std::cout, " " ) );

    std::cout << "\nthe cosine correlation of a/b is: \t" << f::cosine_linear_correlation( a.begin(), a.end(), b.begin(), double() ) << "\n"; 
    std::cout << "\nthe pearson correlation of a/b is: \t" << f::pearson_linear_correlation( a.begin(), a.end(), b.begin(), double() ) << "\n"; 

    std::fill( a.begin(), a.end(), 1.0 );
    std::fill( b.begin(), b.end(), -1.0 );

    std::cout << "\nthe array a is \n";
    std::copy( a.begin(), a.end(), std::ostream_iterator<double>( std::cout, " " ) );
    std::cout << "\nthe array b is \n";
    std::copy( b.begin(), b.end(), std::ostream_iterator<double>( std::cout, " " ) );

    std::cout << "\nthe cosine correlation of a/b is: \t" << f::cosine_linear_correlation( a.begin(), a.end(), b.begin(), double() ) << "\n"; 
    std::cout << "\nthe pearson correlation of a/b is: \t" << f::pearson_linear_correlation( a.begin(), a.end(), b.begin(), double() ) << "\n"; 

    std::fill( a.begin(), a.end(), 1.0 );
    std::fill( b.begin(), b.end(), 1.0 );

    std::cout << "\nthe array a is \n";
    std::copy( a.begin(), a.end(), std::ostream_iterator<double>( std::cout, " " ) );
    std::cout << "\nthe array b is \n";
    std::copy( b.begin(), b.end(), std::ostream_iterator<double>( std::cout, " " ) );

    std::cout << "\nthe cosine correlation of a/b is: \t" << f::cosine_linear_correlation( a.begin(), a.end(), b.begin(), double() ) << "\n"; 
    std::cout << "\nthe pearson correlation of a/b is: \t" << f::pearson_linear_correlation( a.begin(), a.end(), b.begin(), double() ) << "\n"; 

    std::fill( a.begin(), a.end(), 0.0 );
    std::fill( b.begin(), b.end(), 1.0 );

    std::cout << "\nthe array a is \n";
    std::copy( a.begin(), a.end(), std::ostream_iterator<double>( std::cout, " " ) );
    std::cout << "\nthe array b is \n";
    std::copy( b.begin(), b.end(), std::ostream_iterator<double>( std::cout, " " ) );

    std::cout << "\nthe cosine correlation of a/b is: \t" << f::cosine_linear_correlation( a.begin(), a.end(), b.begin(), double() ) << "\n"; 
    std::cout << "\nthe pearson correlation of a/b is: \t" << f::pearson_linear_correlation( a.begin(), a.end(), b.begin(), double() ) << "\n"; 

    return 0;
}

