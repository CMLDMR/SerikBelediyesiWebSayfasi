#include "sikayetitem.h"



std::optional<Sikayet::SikayetItem> Sikayet::SikayetItem::Create_Sikayet(mongocxx::database *_db)
{
    SikayetItem item(_db);
    if( item.isValid() )
    {
        return std::move(item);
    }else{
        return std::nullopt;
    }
}

std::optional<Sikayet::SikayetItem *> Sikayet::SikayetItem::Load_Sikayet(mongocxx::database *_db, const bsoncxx::oid &oid)
{
    auto filter = document{};

    try {
        filter.append(kvp("_id",oid));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
        return std::nullopt;
    }

    try {
        auto val = _db->collection(KEY::collection).find_one(filter.view());

        if( val )
        {
            SikayetItem* item = new SikayetItem(_db,val.value().view());
            return std::move(item);
        }else{
            return std::nullopt;
        }

    } catch (mongocxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
        return std::nullopt;
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

Sikayet::ASAMA::ASAMA(bsoncxx::document::view &_view)
    :mView(_view)
{

}

std::string Sikayet::ASAMA::tarih()
{
    try {
        return mView[Sikayet::KEY::ASAMAKEY::tarih_utf8].get_string().value.data();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        return str;
    }
}

std::string Sikayet::ASAMA::tip()
{
    try {
        return mView[Sikayet::KEY::ASAMAKEY::tip_utf8].get_string().value.data();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        return str;
    }
}

std::string Sikayet::ASAMA::saat()
{
    try {
        return mView[Sikayet::KEY::ASAMAKEY::saat_utf8].get_string().value.data();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        return str;
    }
}

std::string Sikayet::ASAMA::birim()
{
    try {
        return mView[Sikayet::KEY::ASAMAKEY::birim_utf8].get_string().value.data();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        return str;
    }
}

std::string Sikayet::ASAMA::aciklama()
{
    try {
        return mView[Sikayet::KEY::ASAMAKEY::aciklama_utf8].get_string().value.data();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        return str;
    }
}

std::string Sikayet::ASAMA::degisiklik()
{
    try {
        return mView[Sikayet::KEY::ASAMAKEY::degisim_utf8].get_string().value.data();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        return str;
    }
}

std::string Sikayet::ASAMA::personelAdi()
{
    try {
        return mView[Sikayet::KEY::ASAMAKEY::personel_doc].get_document().view()[Sikayet::KEY::ASAMAKEY::personel_doc_adsoyad].get_string().value.data();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        return str;
    }
}























