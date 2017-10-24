#ifndef NQGXRYAXFFHOUEMTHWIXBBQNPVYXUGKCDUSINVHGEGMPGSUEFFDXPLFKMKEJKEQXAABEXSXCQ
#define NQGXRYAXFFHOUEMTHWIXBBQNPVYXUGKCDUSINVHGEGMPGSUEFFDXPLFKMKEJKEQXAABEXSXCQ

#include <string>
#include <map>

namespace f
{

    // Matrix_Generator: 
    //                      should have a constructor interface with arguments:
    //                          string const& host, string const& db
    //                      should have three members
    //                          user_array
    //                          item_array
    //                          rating_matrix  //-1 for non-rated user-item pair
    //                       which represents the user(row), the item(col) and the ratings
    // Prediction_Maker:
    //                      should have a constructor interface with a matrix ( (row-user) (col-item) )
    //                      should have a operator() with or without arguments
    //                          returns a prediction matrix
    template<   typename Matrix_Generator = item_based_mongo_rating_matrix_generator, 
                typename Prediction_Maker = user_based_filtering
            >
    struct mongo_prediction : prediction< mongo_prediction< Matrix_Generator, Prediction_Maker >, Matrix_Generator, Prediction_Maker >
    {

        typedef rating_typedef::user_id_type                user_id_type;
        typedef rating_typedef::item_id_type                item_id_type;
        typedef rating_typedef::rating_type                 rating_type;

        Matrix_Generator mg;
        mongo_prediction( const std::string& host, const std::string& db ) : mg( host, db ) { }

        std::vector<std::pair<user_id_type, rating_type> > const operator_impl( const user_id_type& uid )
        {
            mg( uid );
            auto const& prediction_matrix = (Prediction_Maker( mg.rating_matrix ))();

            std::multimap< rating_type, item_id_type > prediction_for_unrated_item;
            //if rating_matrix value is negative, put the prediction-itemid to the map
            for ( unsigned long c = 0; c != mg.rating_matrix.col(); ++ c )
                if ( mg.rating_matrix[0][c] < rating_type(0) )
                    prediction_for_unrated_item.insert( std::make_pair( prediction_matrix[0][c], mg.item_array[c] ) );
            
            //copy to the vector
            std::vector<std::pair<user_id_type, rating_type> > ans;

            for ( auto itor = prediction_for_unrated_item.rbegin(); itor != prediction_for_unrated_item.rend(); ++itor )
                ans.push_back( std::make_pair((*itor).second, (*itor).first) );

            return ans;
        }
    
    };

}//namespace f

#endif//NQGXRYAXFFHOUEMTHWIXBBQNPVYXUGKCDUSINVHGEGMPGSUEFFDXPLFKMKEJKEQXAABEXSXCQ
