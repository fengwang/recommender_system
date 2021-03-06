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
#ifndef MWALD_HPP_INCLUDED_SDLKJ3OIUSDFLSIJDLKJSFDLKC4OIJSLKJSFADKJLSFLDEJKSLKJDLKJVNVKDJSLKALKEOIDOIJ4O98DFILIKJSLKJDOIU4LKJSFOIJ4OISFOIJE4OIJS
#define MWALD_HPP_INCLUDED_SDLKJ3OIUSDFLSIJDLKJSFDLKC4OIJSLKJSFADKJLSFLDEJKSLKJDLKJVNVKDJSLKALKEOIDOIJ4O98DFILIKJSLKJDOIU4LKJSFOIJ4OISFOIJE4OIJS

#include <f/variate_generator/vg/distribution/inverse_gaussian.hpp>
#include <f/singleton/singleton.hpp>

#include <cassert>

namespace f
{

    template < typename Return_Type, typename Engine >
    struct wald : private inverse_gaussian<Return_Type, Engine>
    {
            typedef inverse_gaussian<Return_Type, Engine>       inverse_gaussian_type;
            typedef Return_Type                                 return_type;
            typedef Engine                                      engine_type;
            typedef typename inverse_gaussian_type::final_type  final_type;
            typedef typename inverse_gaussian_type::seed_type   seed_type;

            return_type     mu_;
            return_type     lambda_;
            engine_type&    e_;     //this engine make sense though not used within this distribution

            explicit wald( const return_type mu = 1, const return_type lambda = 1, const seed_type sd = 0 )
                : mu_( mu ), lambda_( lambda ), e_( singleton<engine_type>::instance() )
            {
                assert( mu > 0 );
                assert( lambda > 0 );
                e_.reset_seed( sd ); //rest seed here for later use in inverse_gaussian distribution generation
            }

            return_type
            operator()() const
            {
                return do_generation( lambda_, mu_ );
            }

        protected:
            return_type
            do_generation( const final_type Lambda, const final_type Mu ) const
            {
                return direct_impl( Lambda, Mu );
            }
        private:
            return_type
            direct_impl( const final_type Lambda, const final_type Mu ) const
            {
                return inverse_gaussian_type::do_generation( Lambda, Mu );
            }
    };

}//vg

#endif//_WALD_HPP_INCLUDED_SDLKJ3OIUSDFLSIJDLKJSFDLKC4OIJSLKJSFADKJLSFLDEJKSLKJDLKJVNVKDJSLKALKEOIDOIJ4O98DFILIKJSLKJDOIU4LKJSFOIJ4OISFOIJE4OIJS

