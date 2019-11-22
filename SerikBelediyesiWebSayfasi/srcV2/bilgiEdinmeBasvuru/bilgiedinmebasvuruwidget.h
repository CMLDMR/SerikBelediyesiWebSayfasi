#ifndef BILGIEDINMEBASVURUWIDGET_H
#define BILGIEDINMEBASVURUWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "bilgiEdinme/bilgiedinmemanager.h"
#include "tc.h"
#include "tcmanager.h"

class BilgiEdinmeBasvuruWidget : public ContainerWidget , SerikBLDCore::BilgiEdinme::BilgiEdinmeManager
{
public:
    BilgiEdinmeBasvuruWidget(SerikBLDCore::DB* _db);
    BilgiEdinmeBasvuruWidget(SerikBLDCore::DB* _db , const SerikBLDCore::TC* mTCUser);


private:
    void initHeader();
    void initWithTC();

    void initPublic();

    const SerikBLDCore::TC* mTC;

    SerikBLDCore::TCManager* mTCManager;

};

#endif // BILGIEDINMEBASVURUWIDGET_H
