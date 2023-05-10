
#ifndef TODOLIST_MALZEMEITEM_H
#define TODOLIST_MALZEMEITEM_H

#include "SerikBelediyesiWebSayfasi/srcV2/device/baseitem.h"

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include "malzemelistitem.h"


namespace TodoList {
namespace Key {
namespace AKIS {
inline const std::string mudurOnay{"mudurOnay"};
inline const std::string baskanYrdOnay{"baskanYrdOnay"};
inline const std::string malzemeList{"malzemeList"};
}
}

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

    void addTeklif();

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
