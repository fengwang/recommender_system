#include <iostream>
#include <salt_generator.hpp>

int main()
{
    for ( long long i = 0; i != 100; ++i )
    {
        std::string str = f::salt_generator()( 123897435 );
        const std::uint32_t* x = reinterpret_cast<const std::uint32_t*>( str.c_str() );

        for ( unsigned long i = 0; i != 5; ++i )
            std::cout << x[i] << "\t";
        std::cout << "\n";
    }

    return 0;
}

