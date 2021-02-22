#ifndef STOKCONTAINERWIDGET_H
#define STOKCONTAINERWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "stok/stokv2manager.h"
#include "personelmanager.h"
#include "user.h"

namespace v2{

namespace StokWidget {


struct PipeLineStokItem{
    std::string kategoriOid;
    double miktarGiris;
    double miktarCikis;
    std::string metric;
    std::string name;

    bool operator==( const PipeLineStokItem& other ){
        if( kategoriOid == other.kategoriOid ){
            return true;
        }else{
            return false;
        }
    }



    void setValues( const PipeLineStokItem &other ){
        this->kategoriOid = other.kategoriOid;
        this->miktarCikis = other.miktarCikis;
        this->miktarGiris = other.miktarGiris;
        this->metric = other.metric;
        this->name = other.name;
    }
};





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
    SerikBLDCore::PersonelManager* mPersonelManager;
    ContainerWidget* mSubMenuBarContainer;

    void initAyarlar();

    void importMazeleme();
    void exportMalzeme( const double &maxMiktar , const PipeLineStokItem &item);

    void initlastState();
    void initimportMenu();
    void initexportMenu();

    void initViewMalzeme(const bsoncxx::oid &malzemeOid );

    CurrentPage mCurrentPage = CurrentPage::SonDurumPage;


    std::vector<PipeLineStokItem> getGirisList();
    std::vector<PipeLineStokItem> getCikisList();

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
