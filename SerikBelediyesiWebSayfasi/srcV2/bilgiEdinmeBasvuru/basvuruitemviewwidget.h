#ifndef BASVURUITEMVIEWWIDGET_H
#define BASVURUITEMVIEWWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "bilgiEdinme/bilgiedinmeitem.h"


class BasvuruItemViewWidget : public ContainerWidget
{
public:
    BasvuruItemViewWidget(const SerikBLDCore::BilgiEdinmeItem& item);



    Signal<NoClass> &Closed();

    Signal<std::string> &ClickedCevapFile();


private:
    SerikBLDCore::BilgiEdinmeItem mItem;

    Signal<NoClass> _Closed;

    Signal<std::string> _ClickedCevapFile;
};

#endif // BASVURUITEMVIEWWIDGET_H
