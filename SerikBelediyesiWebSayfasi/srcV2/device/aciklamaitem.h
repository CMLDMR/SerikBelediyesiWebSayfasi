
#ifndef TODOLIST_ACIKLAMAITEM_H
#define TODOLIST_ACIKLAMAITEM_H

#include "baseitem.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"



namespace TodoList {

class AciklamaItem : public TodoList::BaseItem, public ContainerWidget
{
public:
    AciklamaItem();

    AciklamaItem( const AciklamaItem &other);
    AciklamaItem( AciklamaItem &&other );

    AciklamaItem( const BaseItem &other );

    // Item interface
public:
    virtual void errorOccured(const std::string &errorText) override;

    // BaseItem interface
public:
    virtual void initWidget() override;
};

} // namespace TodoList

#endif // TODOLIST_ACIKLAMAITEM_H
