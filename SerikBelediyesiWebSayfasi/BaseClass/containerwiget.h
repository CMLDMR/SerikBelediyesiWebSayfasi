#ifndef CONTAINERWIGET_H
#define CONTAINERWIGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/wtheaders.h"



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


    void showMessage( std::string title , std::string msg , std::string btnText = "Tamam");



};

#endif // CONTAINERWIGET_H
