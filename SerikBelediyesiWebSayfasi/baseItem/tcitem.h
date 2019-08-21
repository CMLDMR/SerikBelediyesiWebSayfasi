#ifndef TCITEM_H
#define TCITEM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/itembase.h"
#include <QVector>

namespace TC {
static const std::string collection{"TC"};
namespace KEY {
static const std::string tcno{"TCNO"};
static const std::string adsoyad{"İsimSoyisim"};
static const std::string cepTelefonu{"Cep Telefonu"};
static const std::string normalTelefon{"Norma Telefonu"};
static const std::string mahalle{"Mahalle"};
static const std::string tamAdres{"Tam Adres"};
static const std::string password{"password"};
}


class TCItem : private ItemBase
{
public:
    static boost::optional<TCItem> Create_TC(mongocxx::database* _db);


    static boost::optional<TCItem*> LoadByTC(mongocxx::database* _db , std::string _tcno);



private:
    explicit TCItem(mongocxx::database* _db);
    TCItem(mongocxx::database* _db , bsoncxx::document::view _view);
    TCItem(mongocxx::database* _db , const std::string &_collection , bsoncxx::document::view _view);
};


}



#endif // TCITEM_H
