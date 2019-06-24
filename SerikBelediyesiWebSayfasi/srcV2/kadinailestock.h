#ifndef KADINAILESTOCK_H
#define KADINAILESTOCK_H

#include "SerikBelediyesiWebSayfasi/BaseClass/basewidget.h"

#include <vector>
#include <memory>

class StokYardim;

class KadinAileStock : public BaseClass::ContainerWidget
{
public:
    KadinAileStock( mongocxx::database* _db , bsoncxx::document::value _user );
    ~KadinAileStock();

    void initAyarlar();

    void initTCKayit();

    void initStockGiris();

    void initStockCikis();


private:
    StokYardim* mYardim;

};




class StokYardim : public BaseClass::BaseWidget
{

    struct StokKategoriDoc
    {
        std::string mKategoriName;
        std::string mKategoriOid;
    };

    struct StokMalzeme
    {
        std::string mMalzemeOid;
        std::string mKategori;
        std::string mMalzemeAdi;
        std::string mVerenTCNO;
        std::int64_t mVerilisTarihi;
        std::string mAlanTCNO;
        std::int64_t mAlanisTarihi;
        bool mStokta;
    };

public:
    StokYardim( mongocxx::database* _db , bsoncxx::document::value _user );
    ~StokYardim();

    bool addKategoriName( const std::string &kategoriName );
    bool deleteKategoriName(const std::string &kategoriOid );
    std::vector<StokKategoriDoc> getKategoriList();

    bool addMalzeme( const std::string &kategoriadi , const std::string &malzemeadi , const std::string &tcno);
    bool deleteMalzeme( const std::string &malzemeOid );
    std::vector<StokMalzeme> getMalzemeList( const std::string &tcno );

    std::vector<StokMalzeme> getMalzemeAlanList( const std::string &alanTCNO );

    std::vector<StokMalzeme> getStoktaList( const std::string &mkategori = "");

    bool setMalzemeTeslim( const std::string &aliciTCNO , const std::int64_t &alisTarihno , const std::string& mOid );

    // STOK KEY
    std::string getVerenTCNOKEY() const;

private:
    const std::string mKategoriColl = "stokyardimKat";
    const std::string mTCColl = "TC";

    const std::string mKategoriKEY{"kategori"};
    const std::string mOidKEY{"_id"};

    const std::string mMalzemeColl{"stokyardim"};
    const std::string mMalzemeAdiKEY{"malzemeadi"};
    const std::string mVerenTCNOKEY{"verentcno"};
    const std::string mAlanTCNOKEY{"alantcno"};             //*
    const std::string mAlinisTarihiKEY{"alinisTarihi"};     //*
    const std::string mVerilisTarihiKEY{"verilisTarihi"};
    const std::string mStokta{"stokta"};

};

class StokKategori : public BaseClass::ContainerWidget
{
public:
    StokKategori( StokYardim* _mStok , mongocxx::database* _db , bsoncxx::document::value _user);

private:
    StokYardim* mStok;

    void initKategoriler();
    WContainerWidget* mKategorilerContainer;

    void initKategoriOptions();
    WContainerWidget* mKategorilerOptions;


    WLineEdit* inputLine;
    void addKategoriName(const std::string &mKategoriName);

};


//05056730146


class TC : public BaseClass::BaseWidget
{
public:
    TC( mongocxx::database* _db , bsoncxx::document::value _user );

    std::string tCNO() const;
    void setTCNO(const std::string &tCNO);

    std::string isimSoyisim() const;
    void setIsimSoyisim(const std::string &isimSoyisim);

    std::string tel() const;
    void setTel(const std::string &tel);

    std::string address() const;
    void setAddress(const std::string &address);



    bool LoadTC(std::string mTCno);
    bool LoadTel(std::string mTelNo);
    bool LoadOid(bsoncxx::oid oid);

    bool SaveDB();

    bool isValid() const;

    std::string mahalle() const;
    void setMahalle(const std::string &mahalle);

    std::string oid() const;

    void setClear();

private:



    std::string mTCNO;
    std::string mIsimSoyisim;
    std::string mTel;
    std::string mAddress;
    std::string mMahalle;
    std::string mOid;

    bool mValid;

};

class TCKayit : public BaseClass::ContainerWidget
{
public:
    TCKayit(mongocxx::database* _db , bsoncxx::document::value _user);

    void Save();

private:
    WLineEdit* mtcinput;
    WLineEdit* misiminput;
    WLineEdit* mtelinput;
    WLineEdit* maddressinput;
    WComboBox* mMahalle;

    TC* mTC;
};


class TCWidget : public BaseClass::ContainerWidget
{
public:

    TCWidget( mongocxx::database* _db , bsoncxx::document::value _user );

    ~TCWidget();

    void LoadTCNO(const std::string& mtcno);

    void setClear();


    std::string getOid() const;
    std::string getTCNO() const;
private:
    TC* mTC;

};

class StockGiris : public BaseClass::ContainerWidget
{
public:
    StockGiris( mongocxx::database* _db , bsoncxx::document::value _user );



    void ListCurrentTCNOList( const std::string & tcno);


private:
    TC* mTC;
    StokYardim* mStok;

    WLineEdit *mTCNOLineEdit;
    WLineEdit* mExtraBilgiLineEdit;
    WComboBox* mKategoriComboBox;

    TCWidget* mTCWidget;

};

class StockCikis : public BaseClass::ContainerWidget
{
public:
    StockCikis( mongocxx::database* _db , bsoncxx::document::value _user );



    void ListCurrentTCNOList( const std::string & tcno);

    void ListStokList();


private:
    TC* mTC;
    StokYardim* mStok;

    WLineEdit *mTCNOLineEdit;
    WLineEdit* mExtraBilgiLineEdit;
    WComboBox* mKategoriComboBox;

    TCWidget* mTCWidget;

    WContainerWidget* mTeslimEdilenWidget;
    WContainerWidget* mMalzemeListWidget;

};



#endif // KADINAILESTOCK_H
