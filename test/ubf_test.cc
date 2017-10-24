#include <engine/user_based_filtering/user_based_filtering.hpp>
#include <f/matrix/matrix.hpp>

#include <iostream>
#include <iomanip>

int main()
{
   double array[] = {   4, -1,  4,  5, -1, -1,  2,
                        5,  5, -1, -1,  1,  1,  1, 
                        1,  1, -1,  2,  4,  5, -1,
                       -1, -1,  1, -1,  5, -1,  4 };

   f::matrix<double> a( 4, 7, array, array+28 );

   //f::user_based_filtering<double> ubf( a );
   f::user_based_filtering ubf( a );

   std::cout.precision(15);
   //std::cout << std::setw(20);
   std::cout << std::fixed;
   std::cout << "\nmatrix a is \n" << a << "\n";
   std::cout << "\nprediction matrix(10) is \n" <<  ubf(10) << "\n";
   /*
   std::cout << "\nprediction matrix(20) is \n" <<  ubf(20) << "\n";
   std::cout << "\nprediction matrix(30) is \n" <<  ubf(30) << "\n";
   std::cout << "\nprediction matrix(40) is \n" <<  ubf(40) << "\n";
   */

    return 0;
}

