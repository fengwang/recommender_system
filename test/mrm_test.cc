#include <rating/rating_manager.hpp>

#include <f/singleton/singleton.hpp>

#include <iostream>
int main()
{
    auto& manager = f::singleton<f::mongo_rating_manager>::instance();
    manager.initialize( "localhost", "movelens.ml_100k_ua_base" ); 
    
    std::cout << "\ncurrently there are " << manager.user_rating_record.size() << " user records loaded.\n";
    std::cout << "\ncurrently there are " << manager.item_rating_record.size() << " item records loaded.\n";
    std::cout << "\ncurrently there are " << manager.item_rating_account.size() << " items loaded.\n";
    std::cout << "\ncurrently there are " << manager.user_rating_account.size() << " users loaded.\n";

    std::cout << "\nthe one who has rated most of the items is " << (*(manager.user_rating_account.rbegin())).first << ":-:" << (*(manager.user_rating_account.rbegin())).second << "\n";
    std::cout << "\nthe item which was rated most is " << (*(manager.item_rating_account.rbegin())).first << ":-:" << (*(manager.item_rating_account.rbegin())).second << "\n";

    return 0;
}

