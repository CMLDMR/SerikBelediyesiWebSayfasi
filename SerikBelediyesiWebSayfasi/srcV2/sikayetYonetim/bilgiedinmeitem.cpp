#include "bilgiedinmeitem.h"
#include <iostream>

boost::optional<BilgiEdinmeItem> BilgiEdinmeItem::Create_EmptyItem(mongocxx::database* _db)
{
    return BilgiEdinmeItem(_db);
}

boost::optional<BilgiEdinmeItem> BilgiEdinmeItem::LoadBilgiEdinmeItem(mongocxx::database *_db, const std::string &mOid)
{
    auto filter = document{};

    try {
        filter.append(kvp("_id",bsoncxx::oid{mOid}));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
        return boost::none;
    }



    try {
        auto val = _db->collection(BilgiEdinmeKEY::collection).find_one(filter.view());

        if( val )
        {
            auto item = BilgiEdinmeItem::Create_EmptyItem(_db);

            std::string konu;

            try {
                konu = (val.value().view()[BilgiEdinmeKEY::konu].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
                return boost::none;
            }

            try {
                item->setKonu(konu);
            } catch (std::string &e) {
                std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e;
                std::cout << str << std::endl;
                return boost::none;
            }

            return item;

        }

    } catch (mongocxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
        return boost::none;
    }

}

QVector<boost::optional<BilgiEdinmeItem*> > BilgiEdinmeItem::GetList(mongocxx::database *_db)
{

    QVector<boost::optional<BilgiEdinmeItem*>> list;

    mongocxx::options::find findOptions;

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp("_id",-1));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    findOptions.sort(sortDoc.view());

    try {
        auto cursor = _db->collection(BilgiEdinmeKEY::collection).find(document{}.view(),findOptions);

        for( auto doc : cursor )
        {
            BilgiEdinmeItem *item = new BilgiEdinmeItem(_db,doc);
            list.push_back(item);
        }

    } catch (mongocxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }
    return list;

}

bool BilgiEdinmeItem::setKonu(const std::string &konu)
{


    if( konu == mKonu )
    {
        return true;
    }

    auto setDoc = document{};

    try {
        setDoc.append(kvp("$set",make_document(kvp(BilgiEdinmeKEY::konu,konu))));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
        throw e.what();
    }


    try {
        auto upt = this->db()->collection(BilgiEdinmeKEY::collection).update_one(this->filter()->view(),setDoc.view());

        if( upt )
        {
            if( upt.value().modified_count() )
            {
                mKonu = konu;
                return true;
            }else{
                return false;
            }
        }else{
            throw ("no value Returned");
        }

    } catch (mongocxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
        throw (str);
    }

}

const std::string BilgiEdinmeItem::Konu() const
{

    return mKonu;

}

bool BilgiEdinmeItem::setTarih(const std::string &tarih)
{
    if( tarih == mTarih )
    {
        return true;
    }

    if( this->setElement(BilgiEdinmeKEY::tarih,tarih) ){
        this->mTarih = tarih;
        return true;
    }else {
        return false;
    }

}

const std::string BilgiEdinmeItem::Tarih() const
{
    return mTarih;
}



BilgiEdinmeItem::BilgiEdinmeItem(mongocxx::database *_db)
    :DBClass (_db)
{

}

BilgiEdinmeItem::BilgiEdinmeItem(mongocxx::database *_db, const bsoncxx::document::view &view)
    :DBClass (_db)
{

    try {
        mKonu = view[BilgiEdinmeKEY::konu].get_utf8().value.to_string();
    } catch (std::string &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e;
        std::cout << str << std::endl;
    }

    try {
        mTarih = view[BilgiEdinmeKEY::tarih].get_utf8().value.to_string();
    } catch (std::string &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e;
        std::cout << str << std::endl;
    }


    try {
        mCevaplandi = view[BilgiEdinmeKEY::cevaplandi].get_bool().value;
    } catch (std::string &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e;
        std::cout << str << std::endl;
    }


}

BilgiEdinmeItem::BilgiEdinmeItem(const BilgiEdinmeItem *item )
    :DBClass (item->db()),mOid(item->oid())
{

    this->mKonu = item->Konu();


}

bool BilgiEdinmeItem::cevaplandi() const
{
    return mCevaplandi;
}

bool BilgiEdinmeItem::setCevaplandi(bool cevaplandi)
{
    if( mCevaplandi == cevaplandi )
    {
        return true;
    }

    if( this->setElement(BilgiEdinmeKEY::cevaplandi,cevaplandi) )
    {
        mCevaplandi = cevaplandi;
        return true;
    }else{
        return false;
    }
}

bsoncxx::oid BilgiEdinmeItem::oid() const
{
    return mOid;
}

boost::optional<bsoncxx::builder::basic::document> BilgiEdinmeItem::filter()
{

    auto filter = document{};

    try {
        filter.append(kvp("_id",mOid));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
        return boost::none;
    }


    return std::move(filter);

}

template<typename T>
bool BilgiEdinmeItem::setElement(const std::string &elementKey, const T &elementValue)
{
    auto setDoc = document{};

    try {
        setDoc.append(kvp("$set",make_document(kvp(elementKey,elementValue))));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
        throw e.what();
    }


    try {
        auto upt = this->db()->collection(BilgiEdinmeKEY::collection).update_one(this->filter()->view(),setDoc.view());

        if( upt )
        {
            if( upt.value().modified_count() )
            {
                return true;
            }else{
                return false;
            }
        }else{
            throw ("no value Returned");
        }

    } catch (mongocxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
        throw (str);
    }
}
