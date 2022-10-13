#ifndef MECLISUYESIPUBLICWIDGET_H
#define MECLISUYESIPUBLICWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "meclis/meclismanager.h"
#include "tcmanager.h"

namespace v2{

class MeclisUyesiPublicWidget : public ContainerWidget, public SerikBLDCore::Meclis::UyeManager
{
public:
    explicit MeclisUyesiPublicWidget( SerikBLDCore::DB* _mDB );

    virtual void onList(const QVector<SerikBLDCore::Meclis::MeclisUyesi> *mlist ) override;


private:
    SerikBLDCore::TCManagerV2* mTCManager;
};

}


#endif // MECLISUYESIPUBLICWIDGET_H
