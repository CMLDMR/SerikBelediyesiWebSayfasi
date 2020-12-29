#ifndef CALISMAMANAGERCONTAINER_H
#define CALISMAMANAGERCONTAINER_H

#include "user.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "calisma/calismamanager.h"
#include "calismacontainerwidget.h"

namespace v2{


class CalismaKategoriManager;


class CalismaManagerContainer : public ContainerWidget , public SerikBLDCore::CalismaManager
{
public:
    explicit CalismaManagerContainer(SerikBLDCore::User *_user );

    void onList( const QVector<SerikBLDCore::Calisma::Calisma> *mlist ) override;


private:
    void initHeader();
    void initFooter();
    void initIstatistik();
    void initKategoriler();

    void initYeniCalismaEkle();
    WLineEdit* mCalismaAdiLineEdit;
    WComboBox* mMahalleComboBox;
    WText* mBirimComboBox;
    WComboBox* mTipComboBox;

    WDoubleSpinBox* mMiktar;

    WDateEdit* mDateEdit;
    WTextEdit* mAciklamaTextEdit;
    WContainerWidget* mResimAlaniContainerWidget;
    std::vector<std::string> mUploadedPictureList;

    std::string mCurrentSortFilterStr;
    std::string mCurrentMahalleFilterStr;



    SerikBLDCore::User* mUser;


    void loadCalisma( const std::string &calismaOid );


    SerikBLDCore::CalismaKategoriManager* mCalismaKategoriManager;

    enum class CurrentPage{
        Calismalar = 0,
        Istatistik,
        Kategoriler,
        YeniEkle
    };


    CurrentPage mCurrentPage = CurrentPage::Calismalar;


};



class CalismaKategoriManager : public ContainerWidget , public SerikBLDCore::CalismaKategoriManager
{
public:
    explicit CalismaKategoriManager( SerikBLDCore::User *_mUser );

    void onList( const QVector<SerikBLDCore::Calisma::Kategori> *mlist ) override;

private:
    SerikBLDCore::User* mUser;
};



}



#endif // CALISMAMANAGERCONTAINER_H
