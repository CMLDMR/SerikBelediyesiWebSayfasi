#ifndef STOKCONTAINERWIDGET_H
#define STOKCONTAINERWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "stok/stokv2manager.h"
#include "personelmanager.h"
#include "user.h"

namespace v2{

namespace StokWidget {


class StokContainerWidget : public ContainerWidget, public SerikBLDCore::Stokv2::Stokv2Manager
{

    enum class CurrentPage : int
    {
        SonDurumPage = 0,
        MalzemeGirisPage,
        MalzemeCikisPage,
        Istatistik,
        Ayarlar
    };



public:
    explicit StokContainerWidget(SerikBLDCore::User* _mUser);



    void initMenuBar();



    virtual void onList( const QVector<SerikBLDCore::Stokv2::Stok> *mlist ) override;
    virtual void onList( const QVector<SerikBLDCore::Stokv2::Kategori> *mlist ) override;

    virtual void errorOccured(const std::string &errorText) override;



private:
    SerikBLDCore::User* mUser;
    ContainerWidget* mSubMenuBarContainer;

    void initAyarlar();

    void importMazeleme();

    void initimportMenu();
    void initexportMenu();

    void initViewMalzeme(const bsoncxx::oid &malzemeOid );

    CurrentPage mCurrentPage = CurrentPage::SonDurumPage;

};



class MalzemeGirisWidget : public ContainerWidget , public SerikBLDCore::Stokv2::Giris
{
public:
    explicit MalzemeGirisWidget(const SerikBLDCore::Stokv2::Stok &girisItem);

    void asListItem();

    void asFullPage();

    Signal<bsoncxx::oid> &ClickedWidget();

private:
    Signal<bsoncxx::oid> _clickWidget;
};





class MalzemeGirisDialog : public ContainerWidget, public SerikBLDCore::PersonelManager
{
public:
    explicit MalzemeGirisDialog( SerikBLDCore::User *_mUser , const QVector<SerikBLDCore::Stokv2::Kategori> &_mKategoriList);

    void initWidget();


    std::string getCurrentMalzemeGirenOid() const;
    double getCurrentGirenMiktar() const;
    std::string getCurrentKategoriOid() const;
    std::string getCurrentMalzemeAdi() const;
    std::string getCurruntMalzemeMetric() const;


    std::int64_t getJulianDay() const;

private:
    QVector<SerikBLDCore::Stokv2::Kategori> mKategoriList;
    SerikBLDCore::User* mUser;

    WComboBox* mKalemComboBox;
    WDoubleSpinBox* mGirenMiktarSpinBoxValue;
    WText* mCurrentGirenOid;
    WDateEdit* mDateEditWidget;


    std::int64_t mJulianDay;



};






}












}



#endif // STOKCONTAINERWIDGET_H
