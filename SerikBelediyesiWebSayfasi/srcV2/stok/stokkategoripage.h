#ifndef STOKKATEGORIPAGE_H
#define STOKKATEGORIPAGE_H

#include "stok/stokkategorimanager.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "user.h"
#include "tcmanager.h"
#include <QMap>



class VatandasWidget;



namespace v2{

class StokKategoriPage : public ContainerWidget , public SerikBLDCore::Stok::StokKategoriManager
{
public:
    explicit StokKategoriPage( SerikBLDCore::DB* _db , SerikBLDCore::User* _mUser );

    virtual void onList(const QVector<SerikBLDCore::Stok::StokKategori> *mlist) override;

    virtual void errorOccured(const std::string &errorText) override;

    Signal<std::string,std::string> &SelectedKategoriOid();

private:
    SerikBLDCore::User* mUser;

    WLineEdit* mKategoriAdiLineEdit;

    Signal<std::string,std::string> _SelectedKategoriOid;
};


class StokKodPage : public ContainerWidget , public SerikBLDCore::Stok::StokKodManager
{
public:
    explicit StokKodPage( SerikBLDCore::DB* _db );

    virtual void onList(const QVector<SerikBLDCore::Stok::StokKodItem> *mlist) override;

    virtual void errorOccured(const std::string &errorText) override;

    void selectedKategoriOid( const std::string& selectedKategoriOid , const std::string& kategoriAdi );

private:


    std::string mCurrentKategoriOid;

    WText* mCurrentKategoriOidText;
    WLineEdit* mKodAdiLineEdit;
    WLineEdit* mBirimTipiLineEdit;
};




class StokManagerPage : public ContainerWidget , public SerikBLDCore::Stok::StokManager
{
public:
    explicit StokManagerPage( const std::string& stokTitle , SerikBLDCore::User* _mUser );

    virtual void onList(const QVector<SerikBLDCore::Stok::Stok> *mlist) override;

    virtual void onList(const QVector<SerikBLDCore::Stok::StokKodItem> *mlist) override;

    virtual void onList(const QVector<SerikBLDCore::Stok::StokKategori> *mlist) override;

    virtual void errorOccured(const std::string &errorText) override;

    SerikBLDCore::TCManagerV2* mTCManager;

private:
    WComboBox* mKategoriComboBox;
    WComboBox* mStokItemComboBox;

    WSpinBox* mMiktarSpinBox;

    QMap<std::string,bsoncxx::oid> mStokKodList;
    QMap<std::string,bsoncxx::oid> mKategoriList;

    SerikBLDCore::TC* mCurrentTC;
    VatandasWidget* mTCViewWidget;

    WContainerWidget* mSearchContentContainer;
};






class StokManagerCikisPage : public ContainerWidget , public SerikBLDCore::Stok::StokManager
{
public:
    explicit StokManagerCikisPage( const std::string& stokTitle , SerikBLDCore::User* _mUser );

    virtual void onList(const QVector<SerikBLDCore::Stok::Stok> *mlist) override;

    virtual void onList(const QVector<SerikBLDCore::Stok::StokKodItem> *mlist) override;

    virtual void onList(const QVector<SerikBLDCore::Stok::StokKategori> *mlist) override;

    virtual void errorOccured(const std::string &errorText) override;

    SerikBLDCore::TCManagerV2* mTCManager;

private:
    WComboBox* mKategoriComboBox;
    WComboBox* mStokItemComboBox;

    QMap<std::string,bsoncxx::oid> mStokKodList;
    QMap<std::string,bsoncxx::oid> mKategoriList;

    SerikBLDCore::TC* mCurrentTC;
    VatandasWidget* mTCViewWidget;
    void teslimler(); // Daha Önce Teslim Edilenler

    WContainerWidget* mSearchContentContainer;

    WContainerWidget* mTeslimAlinanContainer;
};



}



#endif // STOKKATEGORIPAGE_H
