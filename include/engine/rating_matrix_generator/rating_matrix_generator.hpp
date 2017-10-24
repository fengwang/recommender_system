#ifndef QHRMSRJJNLYPOUSSUMAXLWXVYIWPECSQGKKEAPATHEQBMMTMTTUPQNCYAHCORTITEUXMIGXIR
#define QHRMSRJJNLYPOUSSUMAXLWXVYIWPECSQGKKEAPATHEQBMMTMTTUPQNCYAHCORTITEUXMIGXIR

#include <rating/rating.hpp>
#include <f/matrix/matrix.hpp>

//100,000

#include <vector>

namespace f
{
    
    template< typename Implement >
    struct rating_matrix_generator
    {
        typedef rating_typedef::user_id_type       user_id_type;
        typedef rating_typedef::item_id_type       item_id_type;
        typedef rating_typedef::rating_type        rating_type;
        typedef std::vector<user_id_type>          user_array_type;
        typedef std::vector<item_id_type>          item_array_type;
        typedef f::matrix<rating_type>             rating_matrix_type;

        user_array_type         user_array;
        item_array_type         item_array;
        rating_matrix_type      rating_matrix;

        void operator()( user_id_type const& uid )
        {
            Implement& zen = static_cast<Implement&>(*this);
            zen.operator_impl( uid );
        }

    };//rating_matrix_generator

}//namespace f

#include "impl/mongo_rating_matrix_generator.tcc"
#include "impl/item_based_mongo_rating_matrix_generator.tcc"

#endif//QHRMSRJJNLYPOUSSUMAXLWXVYIWPECSQGKKEAPATHEQBMMTMTTUPQNCYAHCORTITEUXMIGXIR

