#ifndef GIRISWIDGET_H
#define GIRISWIDGET_H


#include "SerikBelediyesiWebSayfasi/Giris/gelenmesajlarwidget.h"
#include "SerikBelediyesiWebSayfasi/reporter/reportresource.h"



namespace Giris {


class GirisWidget : public WContainerWidget
{
public:
    GirisWidget(mongocxx::database* _db);

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
};



class SivilWidget : public WContainerWidget
{
public:
    SivilWidget(mongocxx::database* _db, bsoncxx::document::value* _user);

private:
    mongocxx::database* db;
    bsoncxx::document::value* UserValue;

    WContainerWidget* mMainContainer;
    WContainerWidget* mMenuContainer;
    void initMenu();
    WContainerWidget* mContentContainer;
    WContainerWidget* mHeaderContainer;
    void initHeader();

    WStackedWidget* mContentWidget;
};

class Bilgilerim : public WContainerWidget
{
public:
    Bilgilerim(mongocxx::database* _db , bsoncxx::document::value* _user);

private:
    mongocxx::database* db;
    bsoncxx::document::value* user;
};

class Basvuru : public WContainerWidget
{
public:
    Basvuru(mongocxx::database *_db,bsoncxx::document::value *_user);


    Signal<NoClass> &clickBasvuru();
private:
    mongocxx::database* db;
    bsoncxx::document::value *user;

    Signal<NoClass> _mClickBasvuru_;

    void save();

    void showMessage( std::string title , std::string msg );

    WTextEdit* mesajEdit;
    WLineEdit* lineEditKonu,*lineEditAdres,*lineEditTel,*lineEditmail,*lineEditad,*lineEdittc;
    WComboBox* comboBoxGeriDonus;

    void ShowGeriDonusUyari(WString mesaj);

    void resetForm();

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

class Talep : public WContainerWidget
{
public:
    Talep(mongocxx::database* _db , bsoncxx::document::value *_user);

private:
    mongocxx::database* db;
    bsoncxx::document::value *user;

    WContainerWidget* mMainContainer;

    WLineEdit* mTcNO;
    WLineEdit* mAdSoyad;
    WComboBox* mMahalle;
    WLineEdit* mTelefon;
    WLineEdit* mAdres;
    WPushButton* mSave;

    WComboBox* mTalepMahalle;
    WTextEdit* mEdit;


    void showMessage( std::string title , std::string msg );

    void Save();

};

class Taleplerim : public WContainerWidget
{
public:
    Taleplerim(mongocxx::database* _db, bsoncxx::document::value* _user);

private:
    mongocxx::database* db;
    bsoncxx::document::value* user;

    WContainerWidget* mMainContainer;
    WContainerWidget* toolBarContainer;

    WContainerWidget* mContentContainer;

    WContainerWidget* mAsamaContainer;

    void initTalepler(std::string filterKey = "Hepsi");
    void setDetail(bsoncxx::oid oid);


    struct oidListitem
    {
        std::string oid;
        std::string durum;
        std::string mahalle;
        std::string birim;
        std::string saat;
        std::string tarih;
    };
    std::vector<oidListitem> oidList;

public:
    class TalepHeader : public WContainerWidget
    {
    public:
        TalepHeader(std::string konu,std::string tarih,std::string saat,std::string mahalle,std::string adres,std::string durum,std::string birim,std::string cagriMerkeziPersoneli);
    };



    class DegisimWidget : public WContainerWidget
    {
    public:
        DegisimWidget(bsoncxx::document::view view);
    };

    class AciklamaWidget : public WContainerWidget
    {
    public:
        AciklamaWidget(bsoncxx::document::view view);
        AciklamaWidget(std::string aciklama);


    };

    class AciklamaEkle : public WContainerWidget
    {
    public:
        AciklamaEkle(mongocxx::database* _db , std::string _oid, WContainerWidget *widget, bsoncxx::document::value *user, bool isVatandas = true);
        AciklamaEkle(mongocxx::database* _db , std::string _oid, WContainerWidget *widget, bsoncxx::document::value &user, bool isVatandas = true);

    private:
        mongocxx::database* db;
        std::string oid;
        void showMessage( std::string title , std::string msg );

        WTextEdit* textEdit;
    };

    class GorselWidget : public WContainerWidget
    {
    public:
        GorselWidget(mongocxx::database* db ,bsoncxx::document::view view);
    };




private:
    std::unique_ptr<Wt::WPushButton> createColorButton(const char *className,
                                                       const Wt::WString& text)
    {
        auto button = Wt::cpp14::make_unique<Wt::WPushButton>();
        button->setTextFormat(Wt::TextFormat::XHTML);
        button->setText(text);
        button->addStyleClass(className);
        return button;
    }


    void showMessage( std::string title , std::string msg );
    void showMessage(std::string title , std::string msg , bsoncxx::exception &e);
    void showMessage(std::string title , std::string msg , mongocxx::exception &e);


    Signal<bsoncxx::oid> _clickTalep;
};

class LoginWidget : public WContainerWidget
{
public:
    LoginWidget(mongocxx::database* _db);

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

    WPushButton* mGirisButton;
    WPushButton* mUnuttumButton;
    WPushButton* mkayitOlButton;

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

    std::unique_ptr<Wt::WPushButton> createColorButton(const char *className, const Wt::WString& text);

    mongocxx::cursor find(std::string collection , const bsoncxx::document::view &view = document{}.view() ,mongocxx::options::find options = mongocxx::options::find());

    mongocxx::stdx::optional<bsoncxx::document::value> find_one(std::string collection , const bsoncxx::document::view &view = document{}.view(), mongocxx::options::find options = mongocxx::options::find());

    std::int64_t count(std::string collection , bsoncxx::document::view &filter);
//    std::int64_t count(std::string collection , document &filter = document{} );
    std::int64_t count(std::string collection , bsoncxx::document::value val );


private:
    mongocxx::database* __db;
    bsoncxx::document::value _User;
    void setUser(const bsoncxx::document::value &User);

};

class ContainerWidget : public BaseWidget
{
public:
    ContainerWidget(mongocxx::database* _db , bsoncxx::document::value _user , std::string ContainerTitle = "NOCONTAINER");

    WContainerWidget* getRowContainer() const{
        return this->mRowContainer;
    }

    inline int getRandom(int begin = 0 , int end = 127 )
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(begin,end);
        return dist(mt);
    }

private:
    WContainerWidget* mRowContainer;
};

class PersonelWidget : public BaseWidget
{
public:
    PersonelWidget(mongocxx::database* _db , std::string tel);

private:

    WContainerWidget* mMainContainer;
    WContainerWidget* mHeaderContainer;
    WContainerWidget* mMenuContainer;
    WContainerWidget* mContentContainer;
    WStackedWidget* mContentWidget;

    void initMenu();
    void initHeader(WContainerWidget *_row);

};

//class Yenilikler : public WContainerWidget
//{
//public:
//    Yenilikler();
//};


class Bilgilerim : public BaseWidget
{
public:
    Bilgilerim(mongocxx::database* _db , bsoncxx::document::value _user);
};

class Taleplerim : public BaseWidget
{
public:
    Taleplerim(mongocxx::database* _db , bsoncxx::document::value _user);

    WContainerWidget* toolBarContainer;

    WContainerWidget* mContentContainer;

    WContainerWidget* mAsamaContainer;

    void toolbarReFresh();


    void initTalepler(std::string filterKey = "Hepsi");
    const int limit = 15;
    int skip;
    std::int64_t TotalTalep;
    std::int64_t countTalepler(std::string filterKey = "Hepsi");


    void setDetail(bsoncxx::oid oid);

    std::unique_ptr<WContainerWidget> addControlPanel(bsoncxx::document::value &value);

    void setNewDurum( std::string oid , std::string oldPos , std::string newPos);
    void setPersonel( std::string oid );






private:

    struct oidListitem
    {
        std::string oid;
        std::string durum;
        std::string mahalle;
        std::string birim;
        std::string saat;
        std::string tarih;
    };
    std::vector<oidListitem> oidList;


    Signal<bsoncxx::oid> _clickTalep;
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

class Basvurularim : public BaseWidget
{
public:
    Basvurularim(mongocxx::database* _db , bsoncxx::document::value _user );

    WContainerWidget* mMainContainer;
    WContainerWidget* mContentContainer;

    WContainerWidget* toolBarContainer;
    void toolbarReFresh();
    std::int64_t countTalepler(std::string filterKey = "Hepsi");


    void initList(std::string filterKey = "Hepsi");

    void setBasvuruDetail(std::string oid);


    struct basvuruStruct
    {
        std::string oid;
        std::string basvuruSahibi;
        std::string konu;
        std::string tarih;
        std::string cevap;
    };

    std::vector<basvuruStruct> list;


};

class ProjeWidget : public ContainerWidget
{
public:
    ProjeWidget(mongocxx::database* _database , bsoncxx::document::value _user);

    void initProjectState();

    void initMenu(); // Mahalle Mahalle Proje İstatistikleri

    WContainerWidget* mMainContainer;

    void initMahallelerdeProje();
    struct MahalleItem
    {
        std::string mahalle;
        std::string oid;
    };

    std::vector<MahalleItem> getMahalleList() const;

    void initBirimlerdeProje();
    struct BirimItem
    {
        std::string birim;
        std::string oid;
    };
    std::vector<BirimItem> getBirimList() const;


    void initBirimMahalleProje();

    void initMahalleYilProje();

    void setProjeDetail(std::string oid);

    void setProjeList(bsoncxx::document::view view, std::int32_t year = 0);

    void setList(std::string projectDurum = "Toplam" );
    void addList(WContainerWidget* mrContainer , bsoncxx::document::view filter );
};

class CalismaWidget : public ContainerWidget
{
public:
    CalismaWidget(mongocxx::database* _database , bsoncxx::document::value _user);


    void TotalState();

    ///
    /// \brief ZamanDongusu
    /// 1 Günlük , 1 Haftalık , 1 Aylık , 3 Aylık
    void ZamanDongusu();

    void initMenu();

    WContainerWidget* mMainContainer;





    struct BirimItem
    {
        std::string birim;
        std::string oid;
    };
    std::vector<BirimItem> getBirimList() const;

    ///
    /// \brief birimDongusu
    /// Bugün Başlanan - Bu Hafta Başlanan - Bu Ay Başlanan - Son 6 Ay Başlanan
    void birimDongusu();
    void birimDongusuAdd(WContainerWidget* _rowContainer , const std::vector<BirimItem> birimList);



    struct MahalleItem
    {
        std::string mahalle;
        std::string oid;
    };

    std::vector<MahalleItem> getMahalleList() const;


    void zamanTuneli();

    void addZamanTuneli(WContainerWidget* _rowContainer , WContainerWidget* LeftRowContainer, WContainerWidget*  RightRowContainer);
    int skip = 0;
    bool LeftSide;
    void addCalismaItemToZamanTuneli(WContainerWidget* widget, bsoncxx::document::view doc );

    void setCalismaDetail(std::string oid);



    void mahalleMahalle();
    void mahalleMahalleAdd(WContainerWidget* rowContainer, const std::vector<MahalleItem> mahList);


    void listCalismalar(bsoncxx::document::view filterView , std::string title );
    void addCalismalartoList(WContainerWidget* rowContainer,bsoncxx::document::value filterValue);

    void setReportMenu();
    void reportlist(bsoncxx::document::view filterView, std::string title);

};

class CalismaGirWidget : public ContainerWidget
{
public:
    CalismaGirWidget(mongocxx::database* _database , bsoncxx::document::value _user);

    void initTotalState();



    void ListCalismalar( bsoncxx::document::value filterValue );
    void ListCalismalarAdd(WContainerWidget* rowContainer , bsoncxx::document::value filterValue );

    void setCalismaDetail(std::string oid);
    void addUploader(WContainerWidget* rContainer);
    std::vector<std::string> filePathList;

    void YeniEkle();
    QString newFileNamePath;

    void Ayarlar();
    void ListKategori(WContainerWidget* listContainer);

private:
    std::string CurrentBirim;

    int skip;
    const int limit = 25;


    WContainerWidget* mMainContainer;

    QString rootnewFileName;

};

class CagriMerkezi : public ContainerWidget
{
public:
    CagriMerkezi(mongocxx::database* _database , bsoncxx::document::value _user);

    void initialState();

    void listSikayet(bsoncxx::document::view filterView);

    void initReportMenu();

    void initReportList(WContainerWidget *rowContainer, bsoncxx::document::view filterView);

    void addlist(WContainerWidget* rContainer, bsoncxx::document::value filterValue);

    void setDetail(std::string oid);

    WContainerWidget* mMainContainer;


private:
    int skip = 0;
    const int limit = 20;
};

class BaskanMesajlar : public ContainerWidget
{
public:
    BaskanMesajlar(mongocxx::database* _database , bsoncxx::document::value _user);

    void init();

    WContainerWidget* mMainContainer;

    void listMesajlar(bsoncxx::document::view filterView, std::string titleText);

    void addList(WContainerWidget* rContainer, bsoncxx::document::view filterView);

    void setMesajDetail(std::string oid);

    const int limit = 20;
    int skip;

    class BirimGorevlendirmeList : public ContainerWidget
    {
    public:
        BirimGorevlendirmeList( mongocxx::database* _database , bsoncxx::document::value _user );

        Signal<NoClass> &ClickBack(){ return _mClickBack;}
        Signal<std::string> &ClickBirim(){ return mClickBirim;}
    private:
        Signal<NoClass> _mClickBack;

        Signal<std::string> mClickBirim;

        struct BirimItem
        {
            std::string birim;
            std::string oid;
        };
        std::vector<BirimItem> getBirimList() const;

    };


    class CevapYazWidget : public ContainerWidget
    {
    public:
        CevapYazWidget(mongocxx::database* _database , bsoncxx::document::value _user , std::string mesajoid);

        Signal<NoClass> &SuccessCevap(){ return _SuccessCevap; }

    private:
        std::string mesajOid;

        Signal<NoClass> _SuccessCevap;
    };



};


class ArizaKaydi : public ContainerWidget
{

public:
    ArizaKaydi(mongocxx::database* _database , bsoncxx::document::value _user);


private:
    WContainerWidget* mMainContainer;
    WContainerWidget* mContentContainer;
};

class GirisCikisWidget : public ContainerWidget
{
public:
    GirisCikisWidget(mongocxx::database* _db , bsoncxx::document::value _user );
    WComboBox* mYearSelect;
    WComboBox* mMounthSelect;
    WPushButton* mGosterBtn;
    WContainerWidget* mContentContainer;

    void initGirisCikis();

    void initPersonelMenu();


    void addRow(QDate date);



private:
    void addRow(QString personeloid , qint64 julianDay);
    void addNinRow(QString personeloid , QString BirimName);
    void initBaskanMenu();
    WPushButton* mBugunBtn;
    void initBugun();
    WComboBox* mCurrentBirim;

    Wt::WCalendar *mCalender;
    void initSelectedDay(qint64 julianDay);
};


}

}



#endif // GIRISWIDGET_H
