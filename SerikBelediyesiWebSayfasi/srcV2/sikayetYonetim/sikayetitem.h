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
static const std::string asama{"AŞAMA"};

namespace ASAMAKEY {

static const std::string tip_utf8{"Tip"};
static const std::string tarih_utf8{"Tarih"};
static const std::string saat_utf8{"Saat"};
static const std::string birim_utf8{"Birim"};
static const std::string degisim_utf8{"Değişim"};
static const std::string personel_doc{"Personel"};
static const std::string personel_doc_adsoyad{"ad soyad"};
static const std::string aciklama_utf8{"Açıklama"};

namespace TIP {
static const std::string degisiklik{"Değişiklik"};
static const std::string aciklama{"Açıklama"};
}

}

static const std::string gorevli{"Görevli Personeller"};

}
class SikayetItem : public ItemBase
{
public:
    static boost::optional<SikayetItem> Create_Sikayet(mongocxx::database* _db);

    static boost::optional<SikayetItem*> Load_Sikayet( mongocxx::database* _db , const bsoncxx::oid &oid);

    static QVector<SikayetItem*> GetList(mongocxx::database* _db,
                                  bsoncxx::builder::basic::document filter = document{} ,
                                  mongocxx::options::find findOptions = mongocxx::options::find{} );



private:
    explicit SikayetItem( mongocxx::database* _db );
    SikayetItem(mongocxx::database* _db , bsoncxx::document::view view);
};


class ASAMA
{
public:
    ASAMA(bsoncxx::document::view &_view);

    std::string tarih();

    std::string tip();

    std::string saat();

    std::string birim();

    std::string aciklama();

    std::string degisiklik();

    std::string personelAdi();

private:
    bsoncxx::document::view mView;
};

}



#endif // SIKAYETITEM_H
