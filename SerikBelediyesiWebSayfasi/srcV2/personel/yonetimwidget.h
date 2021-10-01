#ifndef YONETIMWIDGET_H
#define YONETIMWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "personelmanager.h"


namespace v2{

class YonetimWidget : public ContainerWidget, public SerikBLDCore::PersonelManager
{
public:
    YonetimWidget( SerikBLDCore::DB* _mDB );


    void initBaskan();

    void initBaskanYrd();

    void initMudurler();

    SerikBLDCore::BirimManager* mBirimManager;
};


}


#endif // YONETIMWIDGET_H
