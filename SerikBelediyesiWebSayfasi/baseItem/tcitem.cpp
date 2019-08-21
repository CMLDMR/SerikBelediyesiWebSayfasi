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

boost::optional<TC::TCItem *> TC::TCItem::LoadByTC(mongocxx::database *_db, std::string _tcno)
{
    auto filter = document{};

    try {
        filter.append(kvp(KEY::tcno,_tcno));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    auto item = ItemBase::LoadItem<TC::TCItem>(_db,TC::collection,std::move(filter));

    if( item )
    {
        return item;
    }else{
        return boost::none;
    }

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
