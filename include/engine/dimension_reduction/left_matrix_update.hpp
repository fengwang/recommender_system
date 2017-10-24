#ifndef TVTJJSTIVMQLBCGJAJCSWVAKGLEIFDUAJRHOISGSSXFJJGBOOWACUBOSMCBTJBCCSSGRRAHKU
#define TVTJJSTIVMQLBCGJAJCSWVAKGLEIFDUAJRHOISGSSXFJJGBOOWACUBOSMCBTJBCCSSGRRAHKU

#include <f/matrix/matrix.hpp>

#include <numeric>
#include <cassert>
#include <vector>

namespace f
{
    //
    //update for    UV = M
    //
    template< typename T >
    struct left_matrix_update
    {
        typedef T                       value_type;
        typedef matrix<value_type>      left_matrix_type;
        typedef matrix<value_type>      right_matrix_type;

        left_matrix_type&               u;   
        left_matrix_type const&         v;   
        right_matrix_type const&        m;

        //if one element in matrix m is less than 0, then consider it as a blank value
        left_matrix_update( left_matrix_type& u_, left_matrix_type const& v_, right_matrix_type const& m_ ) : u( u_ ), v( v_ ), m( m_ ) 
        {
            assert( u.row() == m.row() );
            assert( v.col() == m.col() );
            assert( u.col() == v.row() );
        }

        value_type operator()( const unsigned long row, const unsigned long col ) const
        {
            assert( row < u.row() );
            assert( col < u.col() );

            std::vector<unsigned long> mj_array; //stores index j where element m[row][j] is not blank
            for ( unsigned long j = 0; j != m.col(); ++j )
                if ( m[row][j] > value_type(0) ) 
                    mj_array.push_back( j );
            
            value_type divider( 0 );
            for ( unsigned long i = 0; i != mj_array.size(); ++i )
                divider += v[col][mj_array[i]] * m[col][mj_array[i]]; 
            if ( divider == value_type(0) )
            {
                u[row][col] = divider;
                return divider; //in case of 0
            }

            value_type factor( 0 );
            for ( unsigned long i = 0; i != mj_array.size(); ++i )
            {
                unsigned long j = mj_array[i];
                
                value_type tmp = m[row][j] + u[row][col] * v[col][j] - std::inner_product( u.row_begin(row), u.row_end(row), v.col_begin(j), value_type(0) );
                factor += v[col][j] * tmp;
            }
            
            value_type x = factor / divider;

            if ( x > 1.0 ) x = 1.0;

            u[row][col] = x;

            return x;
        }

    };//left_matrix_update 

}//namespace f

#endif//TVTJJSTIVMQLBCGJAJCSWVAKGLEIFDUAJRHOISGSSXFJJGBOOWACUBOSMCBTJBCCSSGRRAHKU
