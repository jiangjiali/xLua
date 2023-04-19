#include <f_math.h>
#include "boost/lexical_cast.hpp"

// 函数返回两个数中较大的那个数
int math::add(int num1, int num2) {
    int sum = num1 + num2;
    int a = boost::lexical_cast<int>("123");
    printf("%d\n",a);
    return sum;
}
