#ifndef DILEKCELISTWIDGETITEM_H
#define DILEKCELISTWIDGETITEM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "dilekce.h"

class DilekceListWidgetItem : public ContainerWidget
{
public:
    DilekceListWidgetItem( const SerikBLDCore::Dilekce dilekce );


    Signal<std::string> &ClickDilekceWidget();
private:
    Signal<std::string> _ClickDilekce;
};

#endif // DILEKCELISTWIDGETITEM_H
