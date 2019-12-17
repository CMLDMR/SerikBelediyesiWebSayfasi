#ifndef BASVURULARIM_H
#define BASVURULARIM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "bilgiEdinme/bilgiedinmemanager.h"
#include "tc.h"
#include "basvrulistitemwidget.h"
#include "bilgiEdinme/bilgiedinmeitem.h"




namespace V2 {
class Basvurularim : public ContainerWidget , public SerikBLDCore::BilgiEdinmeManager
{
public:
    Basvurularim(DB* _db , const SerikBLDCore::TC& _tcUser);
    virtual ~Basvurularim();


    void initControlWidget();

    virtual void onList(const QVector<SerikBLDCore::BilgiEdinmeItem> *mlist) final;

    std::unique_ptr<SerikBLDCore::BilgiEdinmeItem> mCurrentFilter;

    void initItem( const std::string& itemOid );

};





}


#endif // BASVURULARIM_H
