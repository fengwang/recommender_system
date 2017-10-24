#include <fstream>
#include <functional>
#include <algorithm>
#include <string>
#include <map>

struct qidian_flag{};
struct zongheng_flag{};

/*
 12 ��վ����    ÿ��IP      ÿ��PV
 11 ���        522000      53440800
 10 ����쳾    147600      11756440
  9 ������Ժ    144000      12707680
  8 ������ѧ��  102000      9800450
  7 ������      98400       8950565
  6 ��������    75000       9720700
  5 17K         64800       21667440
  4 ����        59000       850400
  3 С˵�Ķ���  54600       6889500
  2 �ݺ�        48600       8325620
  1 ������      47000       5785225
  0 3G���      21450       4550040                                                                                                                                                            
  1 ���        14400       2584400
  2 ĥ��        10800       3214200
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



