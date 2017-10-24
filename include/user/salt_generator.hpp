#ifndef SBWUTCWFGPAYUIYBORVJMWBLEJAQCAFVRXYOEKBOJDKQRDDDCDXEKMQVIYVEWMEQGWNDPUXKP
#define SBWUTCWFGPAYUIYBORVJMWBLEJAQCAFVRXYOEKBOJDKQRDDDCDXEKMQVIYVEWMEQGWNDPUXKP

#include <f/variate_generator/variate_generator.hpp>

#include <algorithm>
#include <string>
#include <limits>



namespace f
{
    //TODO : not working
    struct salt_generator
    {
        std::string const operator()( const long long id ) const
        {
            variate_generator<char> vg( std::numeric_limits<char>::min(), std::numeric_limits<char>::max(), id );
            std::string salt( 20, '\0' );
            std::generate( salt.begin(), salt.end(), vg );
            return salt;
        }
    };

}//namespace f

#endif//SBWUTCWFGPAYUIYBORVJMWBLEJAQCAFVRXYOEKBOJDKQRDDDCDXEKMQVIYVEWMEQGWNDPUXKP
