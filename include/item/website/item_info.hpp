#ifndef NXMUUBITNCKTCQEXHCOCMEDJLXUTDHOVXUOOVKBQTPABTNYQYROJWMMQAQGGFWRWIUTAVQYQD
#define NXMUUBITNCKTCQEXHCOCMEDJLXUTDHOVXUOOVKBQTPABTNYQYROJWMMQAQGGFWRWIUTAVQYQD

#include <urdl/istream.hpp>

#include <string>
#include <iostream>

namespace f
{

    struct item_info
    {
        typedef std::string         value_type;

        value_type                  the_url;
        value_type                  the_title;
        value_type                  the_author;
        value_type                  the_category;
        value_type                  the_summary;
        value_type                  the_img_url;

        item_info( value_type const& url_ ) : the_url( url_ )
        {
            do_construction();
        }
        
        int do_construction()
        {
            urdl::istream is{ urdl::url{ the_url } };

            if (!is)
            {
                std::cerr << "\nError: Failed to open url -- " << the_url << " --\n";
                return -1;
            }

            //post process here
            std::string line;
            while (std::getline(is, line))
            {
              std::cout << line << std::endl;
            }
        }


    };

}//namespace f

#endif//NXMUUBITNCKTCQEXHCOCMEDJLXUTDHOVXUOOVKBQTPABTNYQYROJWMMQAQGGFWRWIUTAVQYQD
