#include <fstream>
#include <functional>
#include <algorithm>
#include <string>
#include <map>

struct qidian_flag{};
struct zongheng_flag{};

/*
 12 书站名字    每日IP      每日PV
 11 起点        522000      53440800
 10 烟雨红尘    147600      11756440
  9 潇湘书院    144000      12707680
  8 晋江文学城  102000      9800450
  7 逐浪网      98400       8950565
  6 红袖添香    75000       9720700
  5 17K         64800       21667440
  4 创世        59000       850400
  3 小说阅读网  54600       6889500
  2 纵横        48600       8325620
  1 看书网      47000       5785225
  0 3G书城      21450       4550040                                                                                                                                                            
  1 多酷        14400       2584400
  2 磨铁        10800       3214200
*/
struct address_prefix
{
    
    std::string const operator()( qidian_flag const& ) const
    {
        return std::string{ "http://www.qidian.com/book/" };
    }

    std::string const operator()( zongheng_flag const& ) const
    {
        return std::string{ "http://book.zongheng.com/book/" };
    }

    //others
};

struct address_suffix
{
    std::string const operator()( qidian_flag const& ) const
    {
        return std::string{ ".aspx" };
    }

    std::string const operator()( zongheng_flag const& ) const
    {
        return std::string{ ".html" };
    }

    //others
};



