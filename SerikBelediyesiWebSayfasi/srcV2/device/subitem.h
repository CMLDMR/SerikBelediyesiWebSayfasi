
#ifndef TODOLIST_SUBITEM_H
#define TODOLIST_SUBITEM_H

#include "baseitem.h"

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"



namespace TodoList {

namespace Key {

namespace AKIS {
inline const std::string mudurOnay{"mudurOnay"};
inline const std::string baskanYrdOnay{"baskanYrdOnay"};
inline const std::string malzemeList{"malzemeList"};

namespace MALZEMELIST{
inline const std::string malzemeAdi{"malzemeAdi"};
inline const std::string miktari{"miktari"};
inline const std::string metric{"metric"};
}
}

}




class MalzemeListItem : public SerikBLDCore::Item{
public:
    explicit MalzemeListItem(const std::string &malzemeAdi, const double &miktar, const std::string &metric ):SerikBLDCore::Item(""){
        this->append(Key::AKIS::MALZEMELIST::malzemeAdi,malzemeAdi);
        this->append(Key::AKIS::MALZEMELIST::miktari,miktar);
        this->append(Key::AKIS::MALZEMELIST::metric,metric);
    }
    MalzemeListItem( const bsoncxx::document::view &view ): SerikBLDCore::Item(""){
        this->setDocumentView(view);
    }

    MalzemeListItem( const MalzemeListItem &other ):SerikBLDCore::Item(""){this->setDocumentView(other.view());}
    MalzemeListItem( MalzemeListItem &&other ):SerikBLDCore::Item(""){this->setDocumentView(other.view());}


    MalzemeListItem& operator=( const MalzemeListItem &other){ this->setDocumentView(other.view()); return *this;}
    MalzemeListItem& operator=( MalzemeListItem &&other){ this->setDocumentView(other.view()); return *this;}

    std::string getMalzemeAdi() const;
    double getMiktar() const;
    std::string getMetric() const;

private:
    std::string mMalzemeAdi;
    double mMalzemeMiktari;
    std::string mMetric;
};







class MalzemeItem : public BaseItem, public ContainerWidget
{
public:
    enum class Onay;

    explicit MalzemeItem( SerikBLDCore::User* _mUser = nullptr );
    MalzemeItem( const MalzemeItem &other );
    MalzemeItem( MalzemeItem &&other );

    MalzemeItem( const BaseItem &other );



    MalzemeItem &setMudurOnay( const Onay &onay );
    MalzemeItem &setBaskanYrdOnay( const Onay &onay );

    MalzemeItem &addMalzeme(const std::string &malzemeAdi , const double &miktar , const std::string &metric);

    std::vector<MalzemeListItem> getMalzemeList() const;

    std::string getMudurOnayString() const;
    Onay getMudurOnay() const;
    std::string getBaskanYrdOnayString() const;
    Onay getBaskanYrdOnay() const;


    void editContent();
    void reListMalzeme( WContainerWidget* mMalzemeListContainer , QList<MalzemeListItem> *mList );

    Signal<MalzemeItem::Onay> &mudurOnayClicked();
    Signal<MalzemeItem::Onay> &baskanYrdOnayClicked();
    Signal<NoClass> &reloadClicked();


    enum class Onay{
        Beklemede = 0,
        Red,
        Onayli,
        YetersizAciklama,
        Bilinmeyen = 999
    };

    virtual void initWidget() override;

private:

    void initMalzemeList();

    Signal<MalzemeItem::Onay> _mMudurOnayClicked;
    Signal<MalzemeItem::Onay> _mBaskanYrdOnayClicked;
    Signal<NoClass> _mReloadClicked;

    void setOnay(const Onay &onay );

};




} // namespace TodoList

#endif // TODOLIST_SUBITEM_H
