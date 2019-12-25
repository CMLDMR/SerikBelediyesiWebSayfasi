#ifndef MECLISUYESIPAGE_H
#define MECLISUYESIPAGE_H


#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "meclis/meclismanager.h"
#include "meclis/meclisuyesi.h"
#include "tcmanager.h"
#include "SerikBelediyesiWebSayfasi/srcV2/vatandas/vatandaswidget.h"

namespace v2 {

class MeclisYonetimPage : public ContainerWidget
{
public:
    explicit MeclisYonetimPage(SerikBLDCore::DB* _db);

    void initController();

    void meclisUyeleri();

    void meclisDonemi();

    void partiler();

    void komisyonlar();

private:
    SerikBLDCore::DB* mDB;

};


class MeclisUyeleriPage : public SerikBLDCore::Meclis::UyeManager , public ContainerWidget
{
public:
    explicit MeclisUyeleriPage(DB* _db);

    void onList(const QVector<SerikBLDCore::Meclis::MeclisUyesi> *mlist) override;

    void yeniKayit();

    SerikBLDCore::TCManager* tcManager;
};


class MeclisDonemPage : public SerikBLDCore::Meclis::DonemManager , public ContainerWidget
{
public:
    explicit MeclisDonemPage(DB* _db);

    void onList(const QVector<SerikBLDCore::Meclis::MeclisDonemi> *mlist) override;

    void yeniKayit();

};

class PartiManagerPage : public SerikBLDCore::Meclis::PartiManager , public ContainerWidget
{
public:
    explicit PartiManagerPage(DB* _db);

    void onList(const QVector<SerikBLDCore::Meclis::PartiItem> *mlist) override;

    void yeniKayit();

};






}



#endif // MECLISUYESIPAGE_H
