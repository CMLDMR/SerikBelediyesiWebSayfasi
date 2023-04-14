
#ifndef TODOLIST_BASEITEM_H
#define TODOLIST_BASEITEM_H

#include "item.h"
#include "user.h"

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

namespace TodoList {

namespace Key {

namespace AKIS {
inline const std::string aciklama{"aciklama"};
inline const std::string julianDay{"julianDay"};
inline const std::string epcohTime{"epochTime"};
inline const std::string month{"month"};
inline const std::string year{"year"};
inline const std::string uuid{"uuid"};
inline const std::string personelOid{"personelOid"};
inline const std::string personelName{"personelName"};
inline const std::string type{"type"};


}

}


class BaseItem : public SerikBLDCore::Item
{
public:
    enum class Type;

    BaseItem( const Type &type , SerikBLDCore::User* _mUser = nullptr );
    BaseItem( const BaseItem &other );
    BaseItem( BaseItem &&other );

    virtual void initWidget() = 0;

    BaseItem::Type getType() const;

    std::string uuidString() const;
    std::string getAciklama() const;
    std::string getPersoneName() const;
    std::string getDateString() const;
    std::string getTimeString() const;

    void setUser(SerikBLDCore::User *newUser);
    SerikBLDCore::User *user() const;
    void setTaskItemOid(const std::string &newTaskItemOid);

    enum class Type{
        ACIKLAMA = 0,
        RESIM,
        MALZEME,
        TEKLIF,
        TAMAMLANDI,
        ONAYLANDI,
        UNKNOWN = 999
    };



private:
    std::unique_ptr<WContainerWidget> createBtn(const std::string &btnName , const std::string &backColor);

    SerikBLDCore::User* mUser = nullptr;
    std::string mTaskItemOid{};

    friend class MalzemeItem;

};

} // namespace TodoList

#endif // TODOLIST_BASEITEM_H
