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

//QVector<TC::TCItem> TC::TCItem::GetList(mongocxx::database *_db,
//                                        bsoncxx::builder::basic::document filter,
//                                        mongocxx::options::find findOptions)
//{
//    QVector<TCItem> list;

//    try {
//        auto cursor = _db->collection(collection).find(filter.view(),findOptions);
//    } catch (mongocxx::exception &e) {
//        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
//        std::cout << str << std::endl;
//    }


//    return list;
//}

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
