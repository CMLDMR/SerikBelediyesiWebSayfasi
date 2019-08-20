#ifndef TCITEM_H
#define TCITEM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/itembase.h"
#include <QVector>

namespace TC {
static const std::string collection{"TC"};
namespace KEY {

}


class TCItem : public ItemBase
{
public:
    static boost::optional<TCItem> Create_TC(mongocxx::database* _db);




//protected:
    explicit TCItem(mongocxx::database* _db);
    TCItem(mongocxx::database* _db , bsoncxx::document::view _view);
    TCItem(mongocxx::database* _db , const std::string &_collection , bsoncxx::document::view _view);
};


}



#endif // TCITEM_H
