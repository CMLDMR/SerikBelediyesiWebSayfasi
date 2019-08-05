#ifndef CONTAINERWIGET_H
#define CONTAINERWIGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/wtheaders.h"
#include <random>



class ContainerWiget : public WContainerWidget
{




public:
    ContainerWiget();



    enum ContainerStyleType
    {
        ROW = 0,
        CONTAINERFLUID
    };

    void setContainerStyle(ContainerStyleType type);

    inline int getRandom(int begin = 0 , int end = 127 ) const
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(begin,end);
        return dist(mt);
    }


    void showMessage( std::string title , std::string msg , std::string btnText = "Tamam");



};

#endif // CONTAINERWIGET_H
