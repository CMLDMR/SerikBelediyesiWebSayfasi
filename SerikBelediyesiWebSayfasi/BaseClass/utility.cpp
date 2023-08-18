#include "utility.h"

namespace Utility {

Utility::Utility()
{

}

int Utility::getRandom(int begin, int end) const
{

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(begin,end);
    return dist(mt);
}

} // namespace Utility
