#ifndef BASVRULISTITEMWIDGET_H
#define BASVRULISTITEMWIDGET_H

//#include "bilgiEdinme/bilgiedinmeitem.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

namespace SerikBLDCore {
class BilgiEdinmeItem;
}

class BasvruListItemWidget : public ContainerWidget
{
public:
    BasvruListItemWidget(SerikBLDCore::BilgiEdinmeItem *_item);



    Signal<std::string> &ClickItem();
private:
    SerikBLDCore::BilgiEdinmeItem *item;


    Signal<std::string> _ClickItem;
};

#endif // BASVRULISTITEMWIDGET_H
