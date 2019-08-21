#include "tcitem.h"



boost::optional<TC::TCItem> TC::TCItem::Create_TC(mongocxx::database *_db)
{
    TCItem item(_db);

    if( item.isValid() )
    {
        return std::move(item);
    }
    return boost::none;
}


TC::TCItem::TCItem(mongocxx::database *_db)
    :ItemBase (_db,collection)
{

}

TC::TCItem::TCItem(mongocxx::database *_db, bsoncxx::document::view _view)
    :ItemBase (_db,collection,_view)
{

}

TC::TCItem::TCItem(mongocxx::database *_db, const std::string &_collection, bsoncxx::document::view _view)
    :ItemBase (_db,_collection,_view)
{

}
