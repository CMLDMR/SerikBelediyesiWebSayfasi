#ifndef GIRISWIDGET_H
#define GIRISWIDGET_H


#include "SerikBelediyesiWebSayfasi/Giris/gelenmesajlarwidget.h"

#include "SerikBelediyesiWebSayfasi/srcV2/talepler/talepwidget.h"


#include "user.h"
#include "SerikBelediyesiWebSayfasi/srcV2/sms/smsmanager.h"

#include "firma/firmamanager.h"


namespace Giris {


class GirisWidget : public WContainerWidget
{
public:
    explicit GirisWidget(mongocxx::database* _db);

private:
    mongocxx::database* db;

    void showMessage( std::string title , std::string msg );

    WContainerWidget* mMainContainer,*mHeaderContainer,*mContentContainer;

    void initHeader();

    void initLoginScreen();

    void initMenu(bsoncxx::document::value vatandas);
    void initPersonelMenu(bsoncxx::document::value vatandas);

    bsoncxx::document::value User;

    WText* mPageTitle;

    void initOption();

    void initSivil();

    void initPersonel();

    SerikBLDCore::User* mUser;

    SerikBLDCore::BirimManager* mBirimManager;
};



class SivilWidget : public ContainerWidget
{
public:
    SivilWidget(mongocxx::database* _db, bsoncxx::document::value* _user);

private:
    mongocxx::database* db;
    bsoncxx::document::value* UserValue;
    SerikBLDCore::TC mTCUser;
    void initMenu();
    void initHeader();
};


class FirmaMenuWidget : public ContainerWidget , public SerikBLDCore::DB {
public:
    explicit FirmaMenuWidget( SerikBLDCore::DB* _db , SerikBLDCore::TC* _tcuser );

private:
    SerikBLDCore::TC* mTCUser;
    SerikBLDCore::Firma::FirmaManager* mFirmaManager;
    SerikBLDCore::Firma::FirmaItem* mFirma;

    void initMenu();
    void initHeader();
    void Basvurular();
    void initYeniBasvuru();
};



class Basvurularim : public WContainerWidget
{
public:
    Basvurularim(mongocxx::database* _db , bsoncxx::document::value* _user);

private:
    mongocxx::database* db;
    bsoncxx::document::value *user;

    WContainerWidget* mMainContainer;
    WContainerWidget* toolBarContainer;

    WContainerWidget* mContentContainer;


    std::unique_ptr<Wt::WPushButton> createColorButton(const char *className, const Wt::WString& text);

    enum Cevaplanma
    {
        cevaplandi = 0,
        cevaplanmadi,
        hepsi
    };

    void initBasvurular(Cevaplanma durum = hepsi );

    void showMessage( std::string title , std::string msg );


    struct oidListItem
    {
        bsoncxx::oid oid;
        std::string durum;
        std::string konu;
        std::string saat;
        std::string tarih;
    };

    std::vector<oidListItem> oidList;

    Signal<bsoncxx::oid> _clickBasvuruDetail;

    void setBasvuruDetail(bsoncxx::oid oid);

};









class LoginWidgetV2 : public ContainerWidget
{
public:
    explicit LoginWidgetV2(SerikBLDCore::DB* _db);

    void initLoginScreen();

    WLineEdit* mTelefonNumarasi,*mSifre;
    WPushButton* mGirisButton, *mUnuttumButton, *mkayitOlButton;

    SMSManager* smsManager;

    void sendNewPassword();

};





class LoginWidget : public WContainerWidget
{
public:
    explicit LoginWidget(mongocxx::database* _db);

    bool getUserisVatandas() const;

    bool getUserisPersonel() const;


    Signal<bsoncxx::document::value> &getClickLoginSucces();

    Signal<bsoncxx::document::value> &getClickLogicPersonelSucces();

private:
    mongocxx::database* db;

    void initLoginScreen();

    void sendtempPasswordSMS();
    void sendSMS(std::string numara,std::string sms);

    void showMessage( std::string title , std::string msg );
    void showMessage(std::string title , std::string msg , bsoncxx::exception &e);
    void showMessage(std::string title , std::string msg , mongocxx::exception &e);


    bool checkTCCollection(std::string tcno);
    bool checkPersonelCollection(std::string tcno);

    bool UserisVatandas;
    void setUserisVatandas(bool value);

    bool UserisPersonel;
    void setUserisPersonel(bool value);



    void ConfirmLogin();

    void ConfirmVatandas();
    void ConfirmPersonel();

    void CreatePassword(std::string numara);
    void CreatePersonelPassword(std::string numara);


    std::string confirmationCODE;
    std::string verificationCODE;

    WLineEdit* mTelefonNumarasi;
    std::string mtelefonNumarasi;
    QString sms;
    WLineEdit* mSifre;

    WContainerWidget* mGirisButton;
    WContainerWidget* mUnuttumButton;
    WContainerWidget* mkayitOlButton;

    WDialog* dialog;
    void removeDialog();

    void kayitOld();

    WLineEdit* lineEditTc;
    WLineEdit* lineEditAdSoyad;
    WLineEdit* lineEditCepTelefonu;
    WLineEdit* lineEditNormalTelefonu;
    WComboBox* comboBoxMahalle;
    WLineEdit* lineEditTamAdres;

    void CheckDatabase();
    void ComfirmCepTelefonu();
    void SaveNewTC();


    Signal<bsoncxx::document::value> _ClickLogicSucces;
    Signal<bsoncxx::document::value> _ClickLogicPersonelSucces;
};




namespace Personel {

class BaseWidget : public WContainerWidget
{
public:
    BaseWidget(mongocxx::database* _db , std::string tel);
    BaseWidget(mongocxx::database* _db , bsoncxx::document::value _user);

    void showMessage( std::string title , std::string msg , std::string btnText = "OK");
    void showMessage( std::string title , std::string msg , bsoncxx::exception &e , Wt::Icon icon = Icon::Warning);
    void showMessage( std::string title , std::string msg , mongocxx::exception &e , Wt::Icon icon = Icon::Warning);

    mongocxx::database* db() const;

    bsoncxx::document::value User() const;

    QVector<QString> UserYetki() const;

    std::unique_ptr<Wt::WPushButton> createColorButton(const char *className, const Wt::WString& text);

    mongocxx::cursor find(std::string collection , const bsoncxx::document::view &view = document{}.view() ,mongocxx::options::find options = mongocxx::options::find());

    mongocxx::stdx::optional<bsoncxx::document::value> find_one(std::string collection , const bsoncxx::document::view &view = document{}.view(), mongocxx::options::find options = mongocxx::options::find());

    std::int64_t count(std::string collection , const bsoncxx::document::view &filter);
//    std::int64_t count(std::string collection , const document &filter = document{} );
    std::int64_t count(std::string collection , bsoncxx::document::value val );


private:
    mongocxx::database* __db;
    bsoncxx::document::value _User;
    void setUser(const bsoncxx::document::value &User);

};





class PersonelWidget : public ContainerWidget
{
public:
    explicit PersonelWidget(mongocxx::database* _db , bsoncxx::document::value* _user );

private:

    WContainerWidget* mMainContainer;

    WContainerWidget* mMenuContainer;
    WContainerWidget* mMobilMenuContainer;


    void initMenu();
    void initMobilMenu();
    void initHeader(WContainerWidget *_row);

    SerikBLDCore::User *mUser;

};









class PersonelGorevlendirWidget : public BaseWidget
{
    std::string _personelAdi;
    bsoncxx::oid _personelOid;
    bsoncxx::oid _docoid;
    bsoncxx::oid _photoOid;
    Signal<NoClass> _clickOk;
    std::string _tel;
public:
    PersonelGorevlendirWidget(mongocxx::database* db, std::string tel, bsoncxx::oid photoOid, bsoncxx::oid docOid , std::string personelAdi, bsoncxx::oid personelOid, std::string iconPath , bsoncxx::document::value user);
    Signal<NoClass> &clickOk();
};

class EvrakArsiv : public BaseWidget
{

public:
    EvrakArsiv(mongocxx::database* _db , bsoncxx::document::value _user );

    WContainerWidget* mMainContainer;
    WContainerWidget* mContentContainer;
    WContainerWidget* mEvrakContainer;


    WContainerWidget* kategoriContainer;
    WComboBox* kateogoricombobox;
    void initMenu();
    Signal<NoClass> &clickInitMenu(){ return this->_initMenu;}

    WContainerWidget* TipContainer;
    WComboBox* Tipcombobox;
    void initEvrakTip(WString kategoriName);

    WContainerWidget* ArsivKeysContainer;
    WLineEdit* lineEditArsivAdi;
    WLineEdit* lineEditAnahtar;
    WSpinBox* spinValueSayi;
    WSpinBox* spinValueLimitValue;

    void initEvrakArama();
    void initList();
    std::int64_t listCount(const bsoncxx::document::view &filter = document{}.view());

    const bsoncxx::document::value filter();

    int skip;

    std::vector<bsoncxx::oid> oidList;
    void setArsiv(bsoncxx::oid oid);
    void setEvrak(bsoncxx::oid fileOid);
    void setForDownload(bsoncxx::oid oid);
    WContainerWidget* msetEvrakWidget;
    WImage* mEvrakCanvas;



    void initTumEvraklar();

private:
    Signal<NoClass> _initMenu;

    Signal<NoClass> _initEvrakArama;
    Signal<NoClass> _initTumEvraklar;



};


}

}



#endif // GIRISWIDGET_H
