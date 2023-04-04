
#ifndef TODOLIST_SUBITEM_H
#define TODOLIST_SUBITEM_H

#include "item.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/cssbuilder.h"
#include "user.h"



namespace TodoList {

namespace Key {

namespace AKIS {
inline const std::string aciklama{"aciklama"};
inline const std::string resim{"resim"};
inline const std::string julianDay{"julianDay"};
inline const std::string epcohTime{"epochTime"};
inline const std::string month{"month"};
inline const std::string year{"year"};
inline const std::string uuid{"uuid"};
inline const std::string personelOid{"personelOid"};
inline const std::string personelName{"personelName"};
inline const std::string malzemeList{"malzemeList"};
inline const std::string type{"type"};
inline const std::string mudurOnay{"mudurOnay"};
inline const std::string baskanYrdOnay{"baskanYrdOnay"};
namespace MALZEMELIST{
inline const std::string malzemeAdi{"malzemeAdi"};
inline const std::string miktari{"miktari"};
inline const std::string metric{"metric"};
}
}

}


class MalzemeItem : public SerikBLDCore::Item{
public:
    explicit MalzemeItem(const std::string &malzemeAdi, const double &miktar, const std::string &metric ):SerikBLDCore::Item(""){
        this->append(Key::AKIS::MALZEMELIST::malzemeAdi,malzemeAdi);
        this->append(Key::AKIS::MALZEMELIST::miktari,miktar);
        this->append(Key::AKIS::MALZEMELIST::metric,metric);
    }
    MalzemeItem( const bsoncxx::document::view &view ): SerikBLDCore::Item(""){
        this->setDocumentView(view);
    }

    MalzemeItem( const MalzemeItem &other ):SerikBLDCore::Item(""){this->setDocumentView(other.view());}
    MalzemeItem( MalzemeItem &&other ):SerikBLDCore::Item(""){this->setDocumentView(other.view());}


    MalzemeItem& operator=( const MalzemeItem &other){ this->setDocumentView(other.view()); return *this;}
    MalzemeItem& operator=( MalzemeItem &&other){ this->setDocumentView(other.view()); return *this;}

    std::string getMalzemeAdi() const;
    double getMiktar() const;
    std::string getMetric() const;

private:
    std::string mMalzemeAdi;
    double mMalzemeMiktari;
    std::string mMetric;
};

class SubItem : public SerikBLDCore::Item, public ContainerWidget
{
public:
    enum class Type{
        ACIKLAMA = 0,
        RESIM,
        MALZEME,
        TEKLIF,
        TAMAMLANDI,
        ONAYLANDI,
        UNKNOWN = 999
    };

    enum class Onay{
        Beklemede = 0,
        Red,
        Onayli,
        Bilinmeyen
    };


    explicit SubItem(const Type &type = Type::ACIKLAMA);
    SubItem( const SubItem &other );
    SubItem( SubItem &&other );


    SubItem &setPersonel( const std::string &personelOid , const std::string &personelName );
    SubItem &setAciklama( const std::string &aciklamaText );
    SubItem &setResimOid( const std::string &resimOid );
    SubItem &setMudurOnay( const Onay &onay );
    SubItem &setBaskanYrdOnay( const Onay &onay );

    SubItem &addMalzeme(const std::string &malzemeAdi , const double &miktar , const std::string &metric);

    std::vector<MalzemeItem> getMalzemeList() const;
    SubItem::Type getType() const;
    std::string uuidString() const;
    std::string getAciklama() const;
    std::string getPersoneName() const;
    std::string getDateString() const;
    std::string getTimeString() const;
    std::string getMudurOnayString() const;
    Onay getMudurOnay() const;
    std::string getBaskanYrdOnayString() const;
    Onay getBaskanYrdOnay() const;
    std::string getResimOid() const;



    void setUser(SerikBLDCore::User *newUser);

    SerikBLDCore::User *user() const;


    Signal<SubItem::Onay> &mudurOnayClicked();
private:

    void initWidget();
    void initMalzemeList();

    std::unique_ptr<WContainerWidget> createBtn(const std::string &btnName , const std::string &backColor);

    SerikBLDCore::User* mUser = nullptr;

    Signal<SubItem::Onay> _mMudurOnayClicked;
};




} // namespace TodoList

#endif // TODOLIST_SUBITEM_H
