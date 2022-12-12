#ifndef DUYURUYONETIM_H
#define DUYURUYONETIM_H

#include "item.h"
#include "listitem.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"




class DuyuruItem : public SerikBLDCore::Item
{
public:
    explicit DuyuruItem():SerikBLDCore::Item("collection"){}
};




class DuyuruYonetim : public SerikBLDCore::ListItem<DuyuruItem>
{
public:
    DuyuruYonetim();
};

#endif // DUYURUYONETIM_H
