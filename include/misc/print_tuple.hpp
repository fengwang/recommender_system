#ifndef _PRINT_TUPLE_HPP_INCLUDED_DPIOJAS23089UWEAFSDOIJN4EKLJNSAFLKJSFANDUHSKDJ
#define _PRINT_TUPLE_HPP_INCLUDED_DPIOJAS23089UWEAFSDOIJN4EKLJNSAFLKJSFANDUHSKDJ

#include <tuple>
#include <iostream>

namespace print_tuple_impl_private
{
template<std::size_t N>
struct tuple_print_impl
{
    template< typename... Types >
    void operator()( std::ostream& os, std::tuple<Types...> const& tp ) const
    {
        static_assert( N < sizeof...(Types), "exceed the template size" );
        tuple_print_impl<N-1>()( os, tp );
        os << std::get<N>( tp ) << ", ";
    }
};

template<>
struct tuple_print_impl<0>
{
    template< typename... Types >
    void operator()( std::ostream& os, std::tuple<Types...> const& tp ) const
    {
        os << std::get<0>( tp ) << ", ";
    }
};
}

template<typename Type>
std::ostream& operator << ( std::ostream& os, std::tuple<Type> const& tp )
{
    os << "(" << std::get<0>( tp ) << ")";
    return os;
}

template<typename TypeL, typename TypeR>
std::ostream& operator << ( std::ostream& os, std::tuple<TypeL, TypeR> const& tp )
{
    os << "(" << std::get<0>( tp ) << ", " << std::get<1>( tp ) << ")";
    return os;
}

template<typename... Types>
std::ostream& operator << ( std::ostream& os, std::tuple<Types...> const& tp )
{
    os << "(";
    print_tuple_impl_private::tuple_print_impl<sizeof...(Types)-2>()( os, tp );
    os << std::get<sizeof...(Types)-1>( tp );
    os << ")";
    return os;
}

#endif//_PRINT_TUPLE_HPP_INCLUDED_DPIOJAS23089UWEAFSDOIJN4EKLJNSAFLKJSFANDUHSKDJ

