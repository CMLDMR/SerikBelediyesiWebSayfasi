#ifndef BILGIEDINMEITEM_H
#define BILGIEDINMEITEM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"

#include <boost/optional.hpp>
#include <QVector>

namespace BilgiEdinmeKEY {
static const std::string collection{"bilgiEdinmeBasvuru"};
static const std::string konu{"konu"};
static const std::string tarih{"tarih"};
static const std::string cevaplandi{"geriDönüş"};
}

class BilgiEdinmeItem : public DBClass
{
public:

    static boost::optional<BilgiEdinmeItem> Create_EmptyItem(mongocxx::database *_db);

    static boost::optional<BilgiEdinmeItem> LoadBilgiEdinmeItem( mongocxx::database* _db , const std::string &mOid );

    static QVector<boost::optional<BilgiEdinmeItem *> > GetList( mongocxx::database* _db );


    bool setKonu(const std::string &konu );

    const std::string Konu() const;

    bool setTarih( const std::string &tarih );

    const std::string Tarih() const;

    bsoncxx::oid oid() const;

    bool cevaplandi() const;
    bool setCevaplandi(bool cevaplandi);

private:
    explicit BilgiEdinmeItem( mongocxx::database* _db );
    BilgiEdinmeItem( mongocxx::database* _db , const bsoncxx::document::view &view );
    BilgiEdinmeItem(const BilgiEdinmeItem *item );

    bsoncxx::oid mOid;

    std::string mKonu;
    std::string mTarih;
    bool mCevaplandi;

    boost::optional<document> filter();

    template<typename T>
    bool setElement( const std::string &elementKey , const T &elementValue );

};

#endif // BILGIEDINMEITEM_H
