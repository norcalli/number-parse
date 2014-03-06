#ifdef _MSC_VER
    #define _SECURE_SCL 0
    #define _CRT_SECURE_NO_DEPRECATE 1
    #define WIN32_LEAN_AND_MEAN
    #define VC_EXTRALEAN
    #define NOMINMAX
#endif

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "cycle.h"

static const int N = 100000;
// static const size_t R = 7;
static const size_t R = 100;

void PrintStats(std::vector<double> timings) {
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "[";
    for (size_t i = 1 ; i<timings.size()-1 ; ++i) {
        std::cout << timings[i] << ",";
    }
    std::cout << timings.back();
    std::cout << "]";

    double sum = 0.0;
    for (size_t i = 1 ; i<timings.size() ; ++i) {
        sum += timings[i];
    }
    double avg = sum / double(timings.size()-1);

    sum = 0.0;
    for (size_t i = 1 ; i<timings.size() ; ++i) {
        timings[i] = pow(timings[i]-avg, 2);
        sum += timings[i];
    }
    double var = sum/(timings.size()-2);
    double sdv = sqrt(var);

    std::cout << " with average " << avg << ", stddev " << sdv;
}

int naive(const char *p) {
    int x = 0;
    bool neg = false;
    if (*p == '-') {
        neg = true;
        ++p;
    }
    while (*p >= '0' && *p <= '9') {
        x = (x*10) + (*p - '0');
        ++p;
    }
    if (neg) {
        x = -x;
    }
    return x;
}

#define TEST_CASE(name, init, test)                     \
  {                                                     \
    int tsum = 0;                                       \
    std::vector<double> timings;                        \
    timings.reserve(R);                                 \
    for (size_t r=0 ; r<R ; ++r) {                      \
      ticks start = getticks();                         \
      init;                                             \
      for (size_t i=0 ; i<nums.size() ; ++i) {          \
        test;                                           \
        tsum += x;                                      \
      }                                                 \
      ticks end = getticks();                           \
      double timed = elapsed(end, start);               \
      timings.push_back(timed);                         \
    }                                                   \
                                                        \
    std::cout << name ": ";                             \
    PrintStats(timings);                                \
    std::cout << std::endl;                             \
    std::cout << tsum << std::endl;                     \
  }                                                     \

#include "number-parse.h"
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

int main() {
    std::vector<std::string> nums;
    nums.reserve(N);
    for (int i=0-(N/2) ; i<N/2 ; ++i) {
        std::string y = boost::lexical_cast<std::string>(i);
        nums.push_back(y);
    }

    TEST_CASE("lexical_cast",
              ,
              int x = boost::lexical_cast<int>(nums[i])
              );
    TEST_CASE("stringstream",
              ,
              std::stringstream ss(nums[i]);
              int x = 0;
              ss >> x;
              );
    TEST_CASE("stringstream reused",
              std::stringstream ss,
              ss.str(nums[i]);
              ss.clear();
              int x = 0;
              ss >> x;
              );
    TEST_CASE("atoi()",
              ,
              int x = atoi(nums[i].c_str());
              );

    TEST_CASE("atol()",
              ,
              int x = atol(nums[i].c_str());
              );
    TEST_CASE("strtol()",
              ,
              int x = strtol(nums[i].c_str(), 0, 10);
              );
    TEST_CASE("naive",
              ,
              int x = naive(nums[i].c_str());
              );
    TEST_CASE("ExtractInt",
              ,
              int x = ExtractInt<int>(nums[i].c_str());
              );
    TEST_CASE("spirit::qi",
              ,
              int x = 0;
              std::string &s = nums[i];
              qi::parse(s.begin(), s.end(), qi::int_, x);
              );
}
