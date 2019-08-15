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



    return list;
}

Sikayet::SikayetItem::SikayetItem(mongocxx::database *_db)
    :ItemBase (_db,KEY::collection)
{

}
