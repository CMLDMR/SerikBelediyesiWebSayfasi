#ifndef BASKANYARDIMCILARIWIDGET_H
#define BASKANYARDIMCILARIWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "personelmanager.h"



namespace v2{
class BaskanYardimcilariWidget : public ContainerWidget, public SerikBLDCore::PersonelManager
{
public:
    BaskanYardimcilariWidget( SerikBLDCore::DB* _mDB );

    virtual void onList(const QVector<SerikBLDCore::IK::Personel> *mlist) override;

    void initWidget();
};
}



#endif // BASKANYARDIMCILARIWIDGET_H
