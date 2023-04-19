
#ifndef TODOLIST_TEKLIFITEM_H
#define TODOLIST_TEKLIFITEM_H

#include "baseitem.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"



namespace TodoList {

class TeklifItem : public TodoList::BaseItem, public ContainerWidget
{
public:
    TeklifItem();

    TeklifItem( const TeklifItem &other);
    TeklifItem( TeklifItem &&other );

    TeklifItem( const BaseItem &other );

    // Item interface
public:
    virtual void errorOccured(const std::string &errorText) override;

    // BaseItem interface
public:
    virtual void initWidget() override;
};

} // namespace TodoList

#endif // TODOLIST_TEKLIFITEM_H
