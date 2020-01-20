#ifndef STOKKATEGORIPAGE_H
#define STOKKATEGORIPAGE_H

#include "stok/stokkategorimanager.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "user.h"

namespace v2{

class StokKategoriPage : public ContainerWidget , public SerikBLDCore::Stok::StokKategoriManager
{
public:
    explicit StokKategoriPage( SerikBLDCore::DB* _db , SerikBLDCore::User* _mUser );

    virtual void onList(const QVector<SerikBLDCore::Stok::StokKategori> *mlist) override;

    virtual void errorOccured(const std::string &errorText) override;


private:
    SerikBLDCore::User* mUser;

    WLineEdit* mKategoriAdiLineEdit;
};


class StokKodPage : public ContainerWidget , public SerikBLDCore::Stok::StokKodManager
{
public:
    explicit StokKodPage( SerikBLDCore::DB* _db );

    virtual void onList(const QVector<SerikBLDCore::Stok::StokKodItem> *mlist) override;

    virtual void errorOccured(const std::string &errorText) override;

private:

    WLineEdit* mKodAdiLineEdit;
    WLineEdit* mBirimTipiLineEdit;
};

}



#endif // STOKKATEGORIPAGE_H
