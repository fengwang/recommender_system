#include <f/matrix/matrix.hpp>
#include <f/variate_generator/variate_generator.hpp>
#include <engine/dimension_reduction/left_matrix_update.hpp>

#include <iostream>
#include <algorithm>
#include <numeric>
#include <iomanip>

double diff( const f::matrix<double>& u, const f::matrix<double>& v, const f::matrix<double>& m )
{
    auto const& d = u*v-m;
    return std::inner_product( d.begin(), d.end(), d.begin(), double(0) );
}

int main()
{
    f::variate_generator<unsigned long> vg( 5, 10 );
    unsigned long const u_row = vg();
    unsigned long const u_col = vg();
    unsigned long const v_col = vg();

    f::matrix<double> u( u_row, u_col );
    f::matrix<double> U( u_row, u_col );
    f::matrix<double> v( u_col, v_col );
    f::matrix<double> V( u_col, v_col );

    f::variate_generator<double> vx( 0.0, 1.00 );

    std::generate( u.begin(), u.end(), vx );
    std::generate( U.begin(), U.end(), vx );
    std::generate( v.begin(), v.end(), vx );
    f::matrix<double> const& m = u * v;

    f::matrix<double> p = m;

    f::variate_generator<unsigned long> mr( 0, p.row() );
    f::variate_generator<unsigned long> mc( 0, p.col() );

    //const unsigned long sparse = mc * mr / 20;
    //for ( unsigned long i = 0; i != sparse; ++i )
    //    p[mr()][mc()] = -1.0;

    f::left_matrix_update<double> lmu( U, v, p );

    const unsigned long loops = 400;
/*
    for ( unsigned long i = 0; i != loops; ++i )
    for ( unsigned long r = 0; r != U.row(); ++r )
        for ( unsigned long c = 0; c != U.col(); ++c )
        {
            lmu( r, c );
            std::cout << diff( U, v, m ) << "\n"; 
        }
*/
    f::variate_generator<unsigned long> lr( 0, U.row() );
    f::variate_generator<unsigned long> lc( 0, U.col() );

    for ( unsigned long i = 0; i != loops; ++i )
    {
        lmu( lr(), lc() );
        std::cout.precision( 15 );
        std::cout << diff( U, v, m ) << "\n"; 
    }

    return 0;
}

