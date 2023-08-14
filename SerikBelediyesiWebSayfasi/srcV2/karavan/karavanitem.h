#ifndef V2_KARAVAN_KARAVANITEM_H
#define V2_KARAVAN_KARAVANITEM_H

#include "listitem.h"


namespace v2 {


namespace Karavan {


namespace Key{
inline const std::string Collection{"KaravanYonetim"};
inline const std::string TCOid{"tcoid"};
inline const std::string tcno{"tcno"};
inline const std::string plakaNo{"plaka"};
inline const std::string baslangicDate{"baslangicDate"};
inline const std::string bitisDate{"bitisDate"};
inline const std::string personelOid{"personelOid"};
inline const std::string adsoyad{"adsoyad"};
inline const std::string telefon{"telefon"};
}

//TODO: Key ler eklenip Çıkarılacak
class KaravanItem : public SerikBLDCore::Item
{
public:
    KaravanItem();

    KaravanItem& settcOid( const std::string &oid );
    KaravanItem& setplakaNo(const std::string &plaka );
    KaravanItem& setbaslangicDate( const std::int64_t &baslangicDate );
    KaravanItem& setbitisDate( const std::int64_t &bitisDate );

    KaravanItem& setPersonelOid( const bsoncxx::oid &perOid );

    KaravanItem& setAdsoyad( const std::string &adsoyad );
    KaravanItem& setTelefon( const std::string &telefon );
    KaravanItem& setTCNO( const std::string &tcno );

    std::string getPlakaNo() const;
    std::int64_t getBaslangic() const;
    std::int64_t getBitis() const;

    std::string getAdSoyad() const;
    std::string getTelefon() const;
    std::string getTcNO() const;

};



class KaravanManager : public SerikBLDCore::ListItem<KaravanItem>
{
public:
    explicit KaravanManager(SerikBLDCore::DB* _mdb);

    virtual void onList( const QVector<KaravanItem> *mlist ) override;

    virtual void errorOccured( const std::string &errorText ) override;
};




} // namespace Karavan
} // namespace v2

#endif // V2_KARAVAN_KARAVANITEM_H
