#include "karavanitem.h"

namespace v2 {
namespace Karavan {

KaravanItem::KaravanItem()
    :SerikBLDCore::Item(Key::Collection)
{

}

KaravanItem &KaravanItem::settcOid(const std::string &oid)
{
    this->append(Key::TCOid,oid);
    return *this;
}

KaravanItem &KaravanItem::setplakaNo(const std::string &plaka)
{
    this->append(Key::plakaNo,plaka);
    return *this;
}

KaravanItem &KaravanItem::setbaslangicDate(const int64_t &baslangicDate)
{
    this->append(Key::baslangicDate,baslangicDate);
    return *this;
}

KaravanItem &KaravanItem::setbitisDate(const int64_t &bitisDate)
{
    this->append(Key::bitisDate,bitisDate);
    return *this;
}

KaravanItem &KaravanItem::setPersonelOid(const bsoncxx::oid &perOid)
{
    this->append(Key::personelOid,perOid);
    return *this;
}

KaravanItem &KaravanItem::setAdsoyad(const std::string &adsoyad)
{
    this->append(Key::adsoyad,adsoyad);
    return *this;
}

KaravanItem &KaravanItem::setTelefon(const std::string &telefon)
{
    this->append(Key::telefon,telefon);
    return *this;
}

KaravanItem &KaravanItem::setTCNO(const std::string &tcno)
{
    this->append(Key::tcno,tcno);
    return *this;
}

std::string KaravanItem::getPlakaNo() const
{
    auto val = this->element(Key::plakaNo);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::int64_t KaravanItem::getBaslangic() const
{
    auto val = this->element(Key::baslangicDate);
    if( val ){
        return val.value().view().get_int64().value;
    }
    return 0;
}

int64_t KaravanItem::getBitis() const
{
    auto val = this->element(Key::bitisDate);
    if( val ){
        return val.value().view().get_int64().value;
    }
    return 0;
}

std::string KaravanItem::getAdSoyad() const
{
    auto val = this->element(Key::adsoyad);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::string KaravanItem::getTelefon() const
{
    auto val = this->element(Key::telefon);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::string KaravanItem::getTcNO() const
{
    auto val = this->element(Key::tcno);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

KaravanManager::KaravanManager(DB *_mdb)
    :SerikBLDCore::ListItem<KaravanItem>(_mdb)
{

}

void KaravanManager::onList(const QVector<KaravanItem> *mlist)
{

}

void KaravanManager::errorOccured(const std::string &errorText)
{

}

} // namespace Karavan
} // namespace v2
