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
#ifndef MGRASSIA_HPP_INCLUDED_SDKLJSLKJ4O88UISDFLK4OIUSDFKLJGFLSJASKJHSDFJ34O8USFKL4OUISDFKLU4OIUSFOIUEISFKLJFJKSLKJDLKJF
#define MGRASSIA_HPP_INCLUDED_SDKLJSLKJ4O88UISDFLK4OIUSDFKLJGFLSJASKJHSDFJ34O8USFKL4OUISDFKLU4OIUSFOIUEISFKLJFJKSLKJDLKJF

#include <f/variate_generator/vg/distribution/gamma.hpp>
#include <f/singleton/singleton.hpp>

#include <cmath>
#include <cassert>

namespace f
{

    template < typename Return_Type, typename Engine >
    struct grassia : private gamma<Return_Type, Engine>
    {
            typedef gamma<Return_Type, Engine>                  gamma_type;
            typedef Return_Type                         		return_type;
            typedef Engine                              		engine_type;
            typedef return_type                                 value_type;
            typedef typename engine_type::size_type          	size_type;
            typedef typename engine_type::final_type    	    final_type;
            typedef typename engine_type::seed_type     	    seed_type;

            value_type          a_;
            value_type          b_;
            engine_type&        e_;

            explicit grassia(	const value_type a = value_type( 1 ), const value_type b = value_type( 1 ), const seed_type sd = 0 )
                : a_( a ), b_( b ), e_( singleton<engine_type>::instance() )
            {
                assert( a > 0 );
                assert( b > 0 );
                e_.reset_seed( sd );
            }

            return_type
            operator()() const
            {
                return do_generation( a_, b_ );
            }

        protected:
            return_type
            do_generation( const final_type A, const final_type B ) const 
            {
                return direct_impl_method( A, B );
            }

        private:
            return_type
            direct_impl_method( const final_type A, const final_type B ) const 
            {
                const final_type x = gamma_type::do_generation( A, B );
                const final_type ans = std::exp( -x );
                return ans;
            }

    };

}//vg

#endif//_GRASSIA_HPP_INCLUDED_SDKLJSLKJ4O88UISDFLK4OIUSDFKLJGFLSJASKJHSDFJ34O8USFKL4OUISDFKLU4OIUSFOIUEISFKLJFJKSLKJDLKJF

