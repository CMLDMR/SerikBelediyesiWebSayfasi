#ifndef SIKAYETITEM_H
#define SIKAYETITEM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/itembase.h"
#include <QVector>


namespace Sikayet {


namespace KEY {
static const std::string collection{"Sikayet"};

static const std::string julianDay{"julianDay"};
static const std::string konu{"Konu"};
static const std::string mahalle{"Mahalle"};
static const std::string birim{"Birim"};
static const std::string durum{"Durum"};
static const std::string talepSahibi{"Şikayet/Talep Sahibi"};
static const std::string adSoyad{"adSoyad"};
static const std::string saat{"Saat"};
static const std::string tarih{"Tarih"};
static const std::string tamAdress{"Tam Adres"};
static const std::string cagriMerkeziPersonel{"CagriMerkeziPersoneli"};
static const std::string kategori{"Kategori"};
static const std::string asama{"ASAMA"};
static const std::string gorevli{"Görevli Personeller"};

}
class SikayetItem : public ItemBase
{
public:
    static boost::optional<SikayetItem> Create_Sikayet(mongocxx::database* _db);

    static boost::optional<SikayetItem> Load_Sikayet( mongocxx::database* _db , const bsoncxx::oid &oid);

    static QVector<SikayetItem*> GetList(mongocxx::database* _db,
                                  bsoncxx::builder::basic::document filter = document{} ,
                                  mongocxx::options::find findOptions = mongocxx::options::find{} );


private:
    explicit SikayetItem( mongocxx::database* _db );
    SikayetItem(mongocxx::database* _db , bsoncxx::document::view view);
};

}



#endif // SIKAYETITEM_H
