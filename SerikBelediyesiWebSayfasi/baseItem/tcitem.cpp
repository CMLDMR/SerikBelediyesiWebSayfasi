#include "tcitem.h"



std::optional<TC::TCItem> TC::TCItem::Create_TC(mongocxx::database *_db)
{
    TCItem item(_db);

    if( item.isValid() )
    {
        return std::move(item);
    }
    return std::nullopt;
}

std::optional<TC::TCItem *> TC::TCItem::LoadByTC(mongocxx::database *_db, const std::string &_tcno)
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
        return std::nullopt;
    }

}

std::optional<TC::TCItem *> TC::TCItem::LoadByTel(mongocxx::database *_db, const std::string &_ceptel)
{
    auto filter = document{};

    try {
        filter.append(kvp(KEY::cepTelefonu,_ceptel));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    auto item = ItemBase::LoadItem<TC::TCItem>(_db,TC::collection,std::move(filter));

    if( item )
    {
        return item;
    }else{
        return std::nullopt;
    }
}

std::optional<TC::TCItem *> TC::TCItem::LoadByOid(mongocxx::database *_db, const std::string &_oid)
{
    auto filter = document{};

    try {
        filter.append(kvp("_id",bsoncxx::oid{_oid}));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    auto item = ItemBase::LoadItem<TC::TCItem>(_db,TC::collection,std::move(filter));

    if( item )
    {
        return item;
    }else{
        return std::nullopt;
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
