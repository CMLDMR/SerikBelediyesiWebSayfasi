#include "sikayetitem.h"



boost::optional<Sikayet::SikayetItem> Sikayet::SikayetItem::Create_Sikayet(mongocxx::database *_db)
{
    SikayetItem item(_db);
    if( item.isValid() )
    {
        return std::move(item);
    }else{
        return boost::none;
    }
}

QVector<Sikayet::SikayetItem *> Sikayet::SikayetItem::GetList(mongocxx::database *_db,
                                                              bsoncxx::builder::basic::document filter,
                                                              mongocxx::options::find findOptions)
{
    QVector<Sikayet::SikayetItem*> list;
    try {
        auto cursor = _db->collection(KEY::collection).find(filter.view(),findOptions);

        for( auto doc : cursor )
        {
            SikayetItem* item = new SikayetItem(_db,doc);
            list.push_back(item);
        }

    } catch (mongocxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }
    return list;
}

Sikayet::SikayetItem::SikayetItem(mongocxx::database *_db)
    :ItemBase (_db,KEY::collection)
{

}

Sikayet::SikayetItem::SikayetItem(mongocxx::database *_db, bsoncxx::document::view view)
    :ItemBase (_db,KEY::collection,view)
{




}
