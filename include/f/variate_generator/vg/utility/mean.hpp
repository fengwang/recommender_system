/*
vg -- random variate generator library 
Copyright (C) 2010-2012  Feng Wang (feng.wang@uni-ulm.de) 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by 
the Free Software Foundation, either version 3 of the License, or 
(at your option) any later version. 

This program is distributed in the hope that it will be useful, 
but WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
GNU General Public License for more details. 

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef MMEAN_HPP_INCLUDED_SDOIFJASPOIHJ4EPO9ASFOIUH3089YUAFSOIJHDKSVJNVBIOFAH98
#define MMEAN_HPP_INCLUDED_SDOIFJASPOIHJ4EPO9ASFOIUH3089YUAFSOIJHDKSVJNVBIOFAH98

#include <iterator>
#include <numeric>

namespace f 
{

    template< typename Input_Iterator >
    typename std::iterator_traits<Input_Iterator>::value_type 
    mean( Input_Iterator first, Input_Iterator last )
    {
        typedef typename std::iterator_traits<Input_Iterator>::value_type  value_type;
        value_type const n = std::distance( first, last );
        return std::accumulate( first, last, value_type(0) ) / n;
    }

}//namespace f

#endif//_MEAN_HPP_INCLUDED_SDOIFJASPOIHJ4EPO9ASFOIUH3089YUAFSOIJHDKSVJNVBIOFAH98

