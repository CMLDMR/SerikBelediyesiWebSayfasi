
#ifndef TODOLIST_RESIMITEM_H
#define TODOLIST_RESIMITEM_H

#include "baseitem.h"

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

namespace TodoList {

namespace Key {

namespace AKIS {
inline const std::string resim{"resim"};


}

}

class ResimItem : public TodoList::BaseItem, public ContainerWidget
{
public:
    ResimItem();
    ResimItem( const ResimItem &other);
    ResimItem( ResimItem &&other );

    ResimItem( const BaseItem &other );

    ResimItem &setResimOid( const std::string &resimOid );
    std::string getResimOid() const;

    // Item interface
public:
    virtual void errorOccured(const std::string &errorText) override;

    // BaseItem interface
public:
    virtual void initWidget() override;
};

} // namespace TodoList

#endif // TODOLIST_RESIMITEM_H
