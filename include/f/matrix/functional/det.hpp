#ifndef MDET_HPP_INCLUDED_SDOFJIWEOUIJASFLDKJSAFLJKASFOIJSAFDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDF
#define MDET_HPP_INCLUDED_SDOFJIWEOUIJASFLDKJSAFLJKASFOIJSAFDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDF

#include <f/matrix/matrix.hpp>

namespace f
{

    template<typename T, std::size_t D, typename A>
    T const det( const matrix<T,D,A>& m )
    {
        return m.det();
    }

}//namespace f

#endif//_DET_HPP_INCLUDED_SDOFJIWEOUIJASFLDKJSAFLJKASFOIJSAFDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDF

