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

    QStringList mSelectedKomisyon;
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

class KomisyonManagerPage : public SerikBLDCore::Meclis::KomisyonManager , public ContainerWidget
{
public:
    explicit KomisyonManagerPage(DB* _db);

    void onList(const QVector<SerikBLDCore::Meclis::KomisyonItem> *mlist) override;

    void yeniKayit();

};

class MeclisUyesiProfilPage : public ContainerWidget , public SerikBLDCore::Meclis::MeclisUyesi
{
public:
    explicit MeclisUyesiProfilPage(const MeclisUyesi &uye , SerikBLDCore::DB* _db );


private:
    SerikBLDCore::DB* mDB;
    SerikBLDCore::TCManager* mTcManager;
    SerikBLDCore::TC* mTC;
    SerikBLDCore::Meclis::UyeManager* mUyeManager;


    void meclisUyesiBilgileri();

    QVector<std::string> mSelectedKomisyon;


};


}



#endif // MECLISUYESIPAGE_H
