#ifndef PLGTTKERHXXYTJAKBGBJVXOAFNKLBLCBSBNEEYVYTOCSFQTDYMHPRRNNREGOGFVHDJVBDJQWG
#define PLGTTKERHXXYTJAKBGBJVXOAFNKLBLCBSBNEEYVYTOCSFQTDYMHPRRNNREGOGFVHDJVBDJQWG

#include <rating/rating.hpp>
#include <engine/numeric/linear_correlation.hpp>
#include <f/matrix/matrix.hpp>
#include <f/least_square_fit/linear/linear_least_square_fit.hpp>


#include <limits>

#include <iostream>

namespace f
{
    struct user_based_filtering
    {
        typedef rating_typedef::rating_type value_type;
        typedef f::matrix<value_type>       matrix_type;

        matrix_type const& a; //rating matrix, if a[i][j] is negative, then it is not rated

        user_based_filtering( const matrix_type& a_ ): a( a_ ) { }

        matrix_type const operator()( const unsigned long loops = 8 ) const
        {
            matrix_type A = a;
            unsigned long const row = A.row();//users
            unsigned long const col = A.col();//items
            value_type const zero = value_type();

            //normalize the matrix
            for ( unsigned long i = 0; i != row; ++i )
                normalize( A.row_begin( i ), A.row_end( i ) );

            std::vector<std::function<value_type(value_type)> > fit_function;
            fit_function.push_back( [](value_type){ return value_type(1); } );
            fit_function.push_back( [](value_type x){ return x; } );
            value_type index_a[2];
            std::vector<value_type> x;
            std::vector<value_type> y;

            matrix_type A_ = A;
            matrix_type Wu( row, row );
            std::fill( Wu.diag_begin(), Wu.diag_end(), value_type( 1 ) );
            for ( unsigned long i = 0; i != loops; ++i )
            {
                //evaluate correlation coefficient matrix Wc
                for ( unsigned long i = 0; i != row; ++i )
                    for ( unsigned long j = 0; j < i; ++j )
                    {
                        Wu[i][j] = ( pearson_linear_correlation( A_.row_begin( i ), A_.row_end( i ), A_.row_begin( j ), zero ) + value_type(1) )/ value_type(2);
                        Wu[j][i] = Wu[i][j];
                    }

                //generate prediction matrix
                A_ = Wu * A_;

                //normalize the prediction matrix
                for ( unsigned long i = 0; i != row; ++i )
                    normalize( A_.row_begin( i ), A_.row_end( i ) );

                //replace the evaluated element in A_ with old elements in A
                for ( unsigned long r = 0; r != row; ++r )
                    for ( unsigned long c = 0; c != col; ++c )
                        if ( a[r][c] >= zero )
                            A_[r][c] = A[r][c];

                // using linear fitting algorithm to predict the missing element
                for ( unsigned long r = 0; r != row; ++r )
                {
                    x.clear();
                    y.clear();

                    for ( unsigned long c = 0; c != col; ++c )
                        if ( a[r][c] >= zero )
                        {
                            x.push_back( A_[r][c] );
                            y.push_back( a[r][c] );
                        }
                   
                    linear_least_square_fit( x.begin(), x.end(), y.begin(), fit_function.begin(), fit_function.end(), index_a );

                    for ( unsigned long c = 0; c != col; ++c )
                    {
                        if ( a[r][c] < zero )
                            A_[r][c] = index_a[0] + index_a[1] * A_[r][c];
                        else
                            A_[r][c] = a[r][c];
                    }
                }

            }
            return A_;
        }

        template<typename Iterator>
        void normalize( Iterator first, Iterator last ) const
        {
            value_type const zero = value_type();
            unsigned long const non_zero = std::count_if( first, last, [&zero]( value_type const & x ) { return x >= zero; } );
            //too small element?
            if ( non_zero < 2 )
            {
                std::fill( first, last, value_type( 0.5 ) );
                return;
            }
            //find max/min
            value_type current_max = std::numeric_limits<value_type>::min();
            value_type current_min = std::numeric_limits<value_type>::max();
            for ( auto itor = first; itor != last; ++itor )
            {
                value_type const v = *itor;
                if ( v >= zero )
                {
                    if ( v > current_max ) current_max = v;
                    if ( v < current_min ) current_min = v;
                }
            }
            //normalize rated element
            value_type const diff = current_max - current_min;
            value_type sum = zero;
            for ( auto itor = first; itor != last; ++itor )
            {
                value_type const v = *itor;
                if ( v >= zero )
                {
                    ( *itor ) = ( v - current_min ) / diff;
                    sum += *itor;
                }
            }
            //normalize unrated element
            value_type const average = sum / non_zero;
            for ( auto itor = first; itor != last; ++itor )
            {
                value_type const v = *itor;
                if ( v < zero )
                    ( *itor ) = average;
                    //( *itor ) = zero;
            }
        }

    };

}//namespace f

#endif//PLGTTKERHXXYTJAKBGBJVXOAFNKLBLCBSBNEEYVYTOCSFQTDYMHPRRNNREGOGFVHDJVBDJQWG
