#ifndef BASVURUITEMVIEWWIDGET_H
#define BASVURUITEMVIEWWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "bilgiEdinme/bilgiedinmeitem.h"


class BasvuruItemViewWidget : public ContainerWidget
{
public:
    BasvuruItemViewWidget(const SerikBLDCore::BilgiEdinmeItem& item);



    Signal<NoClass> &Closed();

private:
    SerikBLDCore::BilgiEdinmeItem mItem;

    Signal<NoClass> _Closed;
};

#endif // BASVURUITEMVIEWWIDGET_H
