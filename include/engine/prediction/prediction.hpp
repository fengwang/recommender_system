#ifndef GUFFLAXTXBGMPMLFJSTUDMGGVUWGJKTSSBYBCFYTVMNBVMTMMJSEHJFMBDFJWSTEFTYNQYKVG
#define GUFFLAXTXBGMPMLFJSTUDMGGVUWGJKTSSBYBCFYTVMNBVMTMMJSEHJFMBDFJWSTEFTYNQYKVG

#include <rating/rating.hpp>
#include <engine/user_based_filtering/user_based_filtering.hpp>
#include <engine/rating_matrix_generator/rating_matrix_generator.hpp>

#include <vector>
#include <utility>

namespace f
{
    template< typename Implement, typename Matrix_Generator, typename Prediction_Maker >
    struct prediction
    {
        typedef Implement                           zen_type;
        typedef rating_typedef::user_id_type        user_id_type;
        typedef rating_typedef::item_id_type        item_id_type;
        typedef rating_typedef::rating_type         rating_type;

        std::vector<std::pair<item_id_type, rating_type> > const operator()( const user_id_type& uid )
        {
            auto& zen = static_cast<zen_type&>( *this );
            return zen.operator_impl( uid );
        }
    };

}//namespace f


#include "impl/mongo_prediction.tcc"

#endif//GUFFLAXTXBGMPMLFJSTUDMGGVUWGJKTSSBYBCFYTVMNBVMTMMJSEHJFMBDFJWSTEFTYNQYKVG

