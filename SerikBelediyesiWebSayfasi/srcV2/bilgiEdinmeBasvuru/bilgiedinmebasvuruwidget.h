#ifndef BILGIEDINMEBASVURUWIDGET_H
#define BILGIEDINMEBASVURUWIDGET_H




#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"


#include "bilgiEdinme/bilgiedinmemanager.h"

//namespace SerikBLDCore {
//class BilgiEdinmeItem;
//}

#include "tc.h"
#include "tcmanager.h"


class BilgiEdinmeBasvuruWidget : public ContainerWidget , public SerikBLDCore::BilgiEdinmeManager
{
public:
    BilgiEdinmeBasvuruWidget(SerikBLDCore::DB* _db);
    BilgiEdinmeBasvuruWidget(SerikBLDCore::DB* _db , const SerikBLDCore::TC* mTCUser);


private:
    void initHeader();
    void initWithTC();

    void initPublic();

//    virtual void onList(const QVector<SerikBLDCore::BilgiEdinmeItem> *mlist) override;

    const SerikBLDCore::TC* mTC;

    SerikBLDCore::TCManager* mTCManager;

};

#endif // BILGIEDINMEBASVURUWIDGET_H
