#include "bilgiedinmeitem.h"
#include <iostream>

boost::optional<BilgiEdinmeItem> BilgiEdinmeItem::Create_EmptyItem(mongocxx::database* _db)
{
    return BilgiEdinmeItem(_db);
}

boost::optional<BilgiEdinmeItem *> BilgiEdinmeItem::LoadBilgiEdinmeItem(mongocxx::database *_db, const std::string &mOid)
{
    auto _filter = document{};

    try {
        _filter.append(kvp("_id",bsoncxx::oid{mOid}));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
        return boost::none;
    }




    try {
        auto val = _db->collection(BilgiEdinmeKEY::collection).find_one(_filter.view());

        if( val )
        {
            auto item = new BilgiEdinmeItem(_db);
            item->LoadFromDocumentView(val.value().view());
            return item;
        }else{
            std::cout << "No Value Returned: " << bsoncxx::to_json(_filter.view()) << std::endl;
            return boost::none;
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
    this->LoadFromDocumentView(view);
}

BilgiEdinmeItem::BilgiEdinmeItem(const BilgiEdinmeItem *item )
    :DBClass (item->db()),mOid(item->oid())
{
    this->mKonu = item->Konu();
    this->mTarih = item->Tarih();
    this->mCevaplandi = item->cevaplandi();
    this->mSaat = item->saat();
    this->mTCNO = item->TCNO();
    this->mAdSoyad = item->AdSoyad();
    this->mjulianDate = item->julianDate();
    this->mePosta = item->ePosta();
    this->mTelefon = item->telefon();
    this->mAdres = item->adres();
    this->mMesaj = item->mesaj();
    this->mCevap = item->cevap();
    this->mBirim = item->birim();

}

std::string BilgiEdinmeItem::birim() const
{
    return mBirim;
}

bool BilgiEdinmeItem::setBirim(const std::string &birim)
{
    if( mBirim == birim ){
        return true;
    }

    if( setElement(BilgiEdinmeKEY::birim,birim ) )
    {
        mBirim = birim;
        return true;
    }
    return false;
}





std::string BilgiEdinmeItem::mesaj() const
{
    return mMesaj;
}

bool BilgiEdinmeItem::setMesaj(const std::string &mesaj)
{
    if( mMesaj == mesaj ){
        return true;
    }

    if( this->setElement(BilgiEdinmeKEY::mesaj,mesaj) )
    {
        mMesaj = mesaj;
        return true;
    }

    return false;
}

std::string BilgiEdinmeItem::adres() const
{
    return mAdres;
}

bool BilgiEdinmeItem::setAdres(const std::string &adres)
{

    if( mAdres == adres ){
        return true;
    }

    if( this->setElement(BilgiEdinmeKEY::adres,adres) )
    {
        mAdres = adres;
        return true;
    }

    return false;
}

BilgiEdinmeItem::Cevap BilgiEdinmeItem::cevap() const
{
    return mCevap;
}

bool BilgiEdinmeItem::setCevap(const Cevap &cevap)
{


    if( mCevap == cevap )
    {
        return true;
    }

    std::cout << "Set Element" << bsoncxx::to_json(cevap.Document().view()) << std::endl;

    auto setDoc = document{};

    try {
        setDoc.append(kvp("$set",make_document(kvp(BilgiEdinmeKEY::cevap,cevap.Document()),kvp(BilgiEdinmeKEY::cevaplandi,true))));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
//        throw e.what();
    }

    std::cout << bsoncxx::to_json(setDoc.view()) << std::endl;


    try {
        auto upt = this->db()->collection(BilgiEdinmeKEY::collection).update_one(this->filter()->view(),setDoc.view());

        if( upt )
        {
            if( upt.value().modified_count() )
            {
                this->mCevap = cevap;
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
//        throw (str);
    }


    return false;
}

std::string BilgiEdinmeItem::telefon() const
{
    return mTelefon;
}

bool BilgiEdinmeItem::setTelefon(const std::string &telefon_)
{
    if( mTelefon == telefon_ )
    {
        return true;
    }

    if( setElement(BilgiEdinmeKEY::telefon,telefon_ ) )
    {
        mTelefon = telefon_;
        return true;
    }
    return false;
}

std::string BilgiEdinmeItem::ePosta() const
{
    return mePosta;
}

bool BilgiEdinmeItem::setePosta(const std::string &value)
{
    if( mePosta == value )
    {
        return true;
    }

    if( setElement(BilgiEdinmeKEY::eposta,value) )
    {
        mePosta = value;
        return true;
    }

    return false;
}





std::string BilgiEdinmeItem::AdSoyad() const
{
    return mAdSoyad;
}

bool BilgiEdinmeItem::setAdSoyad(const std::string &adSoyad)
{

    if( mAdSoyad == adSoyad )
    {
        return true;
    }
    if( this->setElement(BilgiEdinmeKEY::cevap,adSoyad) )
    {
        mAdSoyad = adSoyad;
        return true;
    }

    return false;
}

std::string BilgiEdinmeItem::TCNO() const
{
    return mTCNO;
}

bool BilgiEdinmeItem::setTCNO(const std::string &tCNO)
{
    if( mTCNO == tCNO ){
        return true;
    }

    if( this->setElement(BilgiEdinmeKEY::tcno,tCNO) )
    {
        mTCNO = tCNO;
        return true;
    }else{
        return false;
    }
}

std::int64_t BilgiEdinmeItem::julianDate() const
{
    return mjulianDate;
}

bool BilgiEdinmeItem::setjulianDate(const std::int64_t &value)
{
    if( mjulianDate == value ){
        return true;
    }

    if( setElement(BilgiEdinmeKEY::julianDate,value) )
    {
        this->mjulianDate = value;
        return true;
    }else{
        return false;
    }
}

std::string BilgiEdinmeItem::saat() const
{
    return mSaat;
}

bool BilgiEdinmeItem::setSaat(const std::string &saat)
{
    if( mSaat == saat )
    {
        return true;
    }

    if( this->setElement(BilgiEdinmeKEY::saat,saat) )
    {
        mSaat = saat;
        return true;
    }else{
        return false;
    }

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

void BilgiEdinmeItem::LoadFromDocumentView(const bsoncxx::document::view &view)
{

    try {
        mOid = view["_id"].get_oid().value;
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        mKonu = view[BilgiEdinmeKEY::konu].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        mTarih = view[BilgiEdinmeKEY::tarih].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }


    try {
        mCevaplandi = view[BilgiEdinmeKEY::cevaplandi].get_bool().value;
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        mSaat = view[BilgiEdinmeKEY::saat].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        mjulianDate = view[BilgiEdinmeKEY::julianDate].get_int64().value;
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        mTCNO = view[BilgiEdinmeKEY::tcno].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }



    try {
        mAdSoyad = view[BilgiEdinmeKEY::adsoyad].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        mePosta = view[BilgiEdinmeKEY::eposta].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        mTelefon = view[BilgiEdinmeKEY::telefon].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        mAdres = view[BilgiEdinmeKEY::adres].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }


    try {
        mBirim = view[BilgiEdinmeKEY::birim].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        mMesaj = view[BilgiEdinmeKEY::mesaj].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    if ( cevaplandi() )
    {
        try {
            mCevap= view[BilgiEdinmeKEY::cevap].get_document().value;
        } catch (bsoncxx::exception &e) {
            std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what() ;
            std::cout << str << std::endl;
        }
    }




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





document BilgiEdinmeItem::Cevap::Document() const
{
    auto doc = document{};

    try {
        doc.append(kvp(BilgiEdinmeKEY::Cevap::saat,mSaat));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }
    try {
        doc.append(kvp(BilgiEdinmeKEY::Cevap::tarih,mTarih));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        doc.append(kvp(BilgiEdinmeKEY::Cevap::cevapoid,bsoncxx::oid{mCevapOid}));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }
    return doc;
}

bool BilgiEdinmeItem::Cevap::operator==(const BilgiEdinmeItem::Cevap &other)
{
    bool returnValue = true;

    if( this->mSaat != other.mSaat )
    {
        returnValue = false;
    }

    if( this->mTarih != other.mTarih )
    {
        returnValue = false;
    }

    if( this->mCevapOid != other.mCevapOid )
    {
        returnValue = false;
    }

    return returnValue;
}

BilgiEdinmeItem::Cevap &BilgiEdinmeItem::Cevap::operator=(const BilgiEdinmeItem::Cevap &other)
{
    this->mSaat = other.mSaat;
    this->mCevapOid = other.mCevapOid;
    this->mTarih = other.mTarih;
    return *this;
}

BilgiEdinmeItem::Cevap &BilgiEdinmeItem::Cevap::operator=(const bsoncxx::document::view &view)
{
    try {
        this->mSaat = view[BilgiEdinmeKEY::Cevap::saat].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        this->mTarih = view[BilgiEdinmeKEY::Cevap::tarih].get_int64().value;
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    try {
        this->mCevapOid = view[BilgiEdinmeKEY::Cevap::cevapoid].get_oid().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }

    return *this;
}
