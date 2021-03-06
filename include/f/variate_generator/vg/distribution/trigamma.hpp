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
#ifndef MTRIGAMMA_HPP_INCLUDED_SDFOIJSLKJ4OIUASFD2SKLJSALKJSFLKJ4OUISDFLKJ54OIJFLKJSFDOIJ4LKJFSDOIJ4LKJFSDOIJRT
#define MTRIGAMMA_HPP_INCLUDED_SDFOIJSLKJ4OIUASFD2SKLJSALKJSFLKJ4OUISDFLKJ54OIJFLKJSFDOIJ4LKJFSDOIJ4LKJFSDOIJRT

#include <f/variate_generator/vg/distribution/generalized_hypergeometric_b3.hpp>
#include <f/variate_generator/vg/distribution/digamma.hpp>
#include <f/singleton/singleton.hpp>
#include <f/variate_generator/vg/utility/proxy.hpp>

#include <cmath>
#include <algorithm>
#include <cassert>

namespace f
{

    template < typename Return_Type, typename Engine >
    struct trigamma : private proxy<generalized_hypergeometric_b3<Return_Type, Engine>, 1>,
                      private proxy<digamma<Return_Type, Engine>, 2>
    {
            typedef proxy<generalized_hypergeometric_b3< Return_Type, Engine> > GHgB3_type;
            typedef proxy<digamma< Return_Type, Engine > >   digamma_type;
            typedef typename Engine::final_type     final_type;
            typedef Return_Type                     return_type;
            typedef Return_Type                     value_type;
            typedef typename Engine::seed_type      seed_type;
            typedef std::size_t                     size_type;
            typedef Engine                          engine_type;

            value_type           c_;
            engine_type&         e_;

            explicit trigamma( value_type c = 3, seed_type s = 0 ) 
                : c_( c ), e_( singleton<engine_type>::instance() )
            {
                assert( c > 0 );
                e_.reset_seed( s );
            }

            return_type
            operator()() const
            {
                return do_generation( c_ );
            }

        protected:
            //L. Devroye, Random variate generation for the trigamma and trigamma distributions,
            //Journal of Statistical Computation and Simulation, vol. 43, pp. 197-216, 1992.
            return_type
            do_generation( const value_type c ) const
            {
                if ( c > final_type( 1.14 ) )
                    return large_c_rejection_impl( c );
                return small_c_rejection_impl( c );
            }

        private:
            return_type
            large_c_rejection_impl( const value_type c ) const
            {
                for ( ;; )
                    {
                        const final_type x = final_type( 1 ) + GHgB3_type::do_generation( final_type( 1 ), final_type( 1 ), c - final_type( 1 ) );
                        const final_type u = e_();

                        if ( x * u < final_type( 1 ) )
                            { return x; }
                    }
            }

            return_type
            small_c_rejection_impl( const value_type c ) const
            {
                return digamma_type::do_generation( 0, c );
            }

    };

}//namespace f

#endif//_TRIGAMMA_HPP_INCLUDED_SDFOIJSLKJ4OIUASFD2SKLJSALKJSFLKJ4OUISDFLKJ54OIJFLKJSFDOIJ4LKJFSDOIJ4LKJFSDOIJRT 

