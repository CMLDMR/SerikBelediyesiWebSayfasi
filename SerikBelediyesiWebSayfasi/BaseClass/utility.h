#ifndef UTILITY_UTILITY_H
#define UTILITY_UTILITY_H

#include <random>


namespace Utility {

class Utility
{
public:
    Utility();


    int getRandom(int begin = 0 , int end = 127 ) const;
};

} // namespace Utility

#endif // UTILITY_UTILITY_H
