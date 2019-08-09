#ifndef BILGIEDINMEITEM_H
#define BILGIEDINMEITEM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"

#include <boost/optional.hpp>
#include <QVector>

namespace BilgiEdinmeKEY {
static const std::string collection{"bilgiEdinmeBasvuru"};
static const std::string tarih{"tarih"};
static const std::string cevaplandi{"geriDönüş"};
static const std::string saat{"saat"};
static const std::string julianDate{"julianDate"};
static const std::string tcno{"TCNO"};
static const std::string adsoyad{"adsoyad"};
static const std::string eposta{"eposta"};
static const std::string telefon{"telefon"};
static const std::string adres{"adres"};
static const std::string konu{"konu"};
static const std::string mesaj{"mesaj"};
static const std::string birim{"birim"};
static const std::string cevap{"cevap"};
namespace Cevap {
static const std::string cevapoid{"cevapOid"};
static const std::string saat{"saat"};
static const std::string tarih{"tarih"};
}
}

class BilgiEdinmeItem : public DBClass
{
public:

    static boost::optional<BilgiEdinmeItem> Create_EmptyItem(mongocxx::database *_db);

    static boost::optional<BilgiEdinmeItem*> LoadBilgiEdinmeItem( mongocxx::database* _db , const std::string &mOid );

    static QVector<boost::optional<BilgiEdinmeItem *> > GetList( mongocxx::database* _db );


    struct Cevap{
        std::string mCevapOid;
        std::string mSaat;
        std::int64_t mTarih;
        bsoncxx::builder::basic::document Document() const;

        bool operator==( const Cevap& other );
        Cevap &operator=( const Cevap& other );
        Cevap &operator=( const bsoncxx::document::view &view);
    };


    bool setKonu(const std::string &konu );

    const std::string Konu() const;

    bool setTarih( const std::string &tarih );

    const std::string Tarih() const;

    bsoncxx::oid oid() const;

    bool cevaplandi() const;
    bool setCevaplandi(bool cevaplandi);

    std::string saat() const;
    bool setSaat(const std::string &saat);

    std::int64_t julianDate() const;
    bool setjulianDate(const std::int64_t &value);

    std::string TCNO() const;
    bool setTCNO(const std::string &tCNO);

    std::string AdSoyad() const;
    bool setAdSoyad(const std::string &adSoyad);

    std::string ePosta() const;
    bool setePosta(const std::string &value);

    std::string telefon() const;
    bool setTelefon(const std::string &telefon_);

    Cevap cevap() const;
    bool setCevap(const Cevap &cevap);

    std::string adres() const;
    bool setAdres(const std::string &adres);

    std::string mesaj() const;
    bool setMesaj(const std::string &mesaj);

    std::string birim() const;
    bool setBirim(const std::string &birim);

private:
    explicit BilgiEdinmeItem( mongocxx::database* _db );
    BilgiEdinmeItem( mongocxx::database* _db , const bsoncxx::document::view &view );
    BilgiEdinmeItem(const BilgiEdinmeItem *item );

    bsoncxx::oid mOid;

    std::string mKonu;//
    std::string mTarih;
    std::string mSaat;
    std::int64_t mjulianDate;
    bool mCevaplandi;
    std::string mTCNO;
    std::string mAdSoyad;
    std::string mePosta;
    std::string mTelefon;
    std::string mAdres;
    std::string mMesaj;
    Cevap mCevap;
    std::string mBirim;

    boost::optional<document> filter();

    template<typename T>
    bool setElement( const std::string &elementKey , const T &elementValue );

    void LoadFromDocumentView( const bsoncxx::document::view &view);

};

#endif // BILGIEDINMEITEM_H
