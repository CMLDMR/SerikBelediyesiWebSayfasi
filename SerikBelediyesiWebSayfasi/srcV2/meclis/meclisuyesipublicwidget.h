#ifndef MECLISUYESIPUBLICWIDGET_H
#define MECLISUYESIPUBLICWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "meclis/meclismanager.h"
#include "tcmanager.h"

namespace v2{

class MeclisUyesiPublicWidget : public ContainerWidget, public SerikBLDCore::Meclis::UyeManager
{
public:
    MeclisUyesiPublicWidget( SerikBLDCore::DB* _mDB );


private:
    SerikBLDCore::TCManagerV2* mTCManager;
};

}


#endif // MECLISUYESIPUBLICWIDGET_H
