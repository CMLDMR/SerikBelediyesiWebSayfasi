#ifndef BASVRULISTITEMWIDGET_H
#define BASVRULISTITEMWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

namespace SerikBLDCore {
namespace BilgiEdinme {
class BilgiEdinmeItem;
}
}

class BasvruListItemWidget : public ContainerWidget
{
public:
    BasvruListItemWidget(SerikBLDCore::BilgiEdinme::BilgiEdinmeItem *_item);

    SerikBLDCore::BilgiEdinme::BilgiEdinmeItem *item;

};

#endif // BASVRULISTITEMWIDGET_H
