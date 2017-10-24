#ifndef BHMRPVACBXKOFBVSAEYNCHJJIFUCWMDFYDFXFMPDYMWOLKHIWRPOVWOUIIMRQNLWYXQPOLKQG
#define BHMRPVACBXKOFBVSAEYNCHJJIFUCWMDFYDFXFMPDYMWOLKHIWRPOVWOUIIMRQNLWYXQPOLKQG

#include <cmath>
#include <numeric>
#include <vector>
#include <algorithm>
#include <iterator>

namespace f
{
    template< typename Input_Iterator_1, typename Input_Iterator_2, typename T >
    T cosine_linear_correlation( Input_Iterator_1 first1, Input_Iterator_1 last1, Input_Iterator_2 first2, T initial )
    {
        unsigned long const n = std::distance( first1, last1 );
        T const dot = std::inner_product( first1, last1, first2, initial );
        T const norm1_2 = std::inner_product( first1, last1, first1, initial );
        T const norm2_2 = std::inner_product( first2, first2+n, first2, initial );

        if ( norm1_2 == T() )
        {
            if ( norm2_2 == T() )
                return T(1);
            return T();
        }

        return dot / std::sqrt( norm1_2 * norm2_2 );
    }

    template< typename Input_Iterator_1, typename Input_Iterator_2, typename T >
    T pearson_linear_correlation( Input_Iterator_1 first1, Input_Iterator_1 last1, Input_Iterator_2 first2, T initial )
    {
        std::vector<T> a( first1, last1 );
        unsigned long const n = a.size();
        std::vector<T> b( first2, first2+n );

        std::vector<T> diff_ab;
        std::transform( a.begin(), a.end(), b.begin(), std::back_inserter( diff_ab ), []( T const& x, T const& y ){ return x-y; } );
        if ( std::inner_product( diff_ab.begin(), diff_ab.end(), diff_ab.begin(), T() ) == T() ) return T(1);

        std::vector<T> sum_ab;
        std::transform( a.begin(), a.end(), b.begin(), std::back_inserter( sum_ab ), []( T const& x, T const& y ){ return x+y; } );
        if ( std::inner_product( sum_ab.begin(), sum_ab.end(), sum_ab.begin(), T() ) == T() ) return T(-1);

        T const a_ = std::accumulate( a.begin(), a.end(), initial ) / n;
        T const b_ = std::accumulate( b.begin(), b.end(), initial ) / n;

        std::for_each( a.begin(), a.end(), [&a_]( T& x ) { x -= a_; } );
        std::for_each( b.begin(), b.end(), [&b_]( T& x ) { x -= b_; } );

        return cosine_linear_correlation( a.begin(), a.end(), b.begin(), initial );
    }

}//namespace f

#endif//BHMRPVACBXKOFBVSAEYNCHJJIFUCWMDFYDFXFMPDYMWOLKHIWRPOVWOUIIMRQNLWYXQPOLKQG

