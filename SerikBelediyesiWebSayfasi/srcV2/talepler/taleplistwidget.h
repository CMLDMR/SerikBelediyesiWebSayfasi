#ifndef TALEPLISTWIDGET_H
#define TALEPLISTWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

class TalepListWidget : public ContainerWidget
{
public:
    TalepListWidget(const std::string& oid,
                    const std::string& mahalle,
                    const std::string& tarih,
                    const std::string& durum,
                    const std::string& durumColor);


    Signal<std::string> &ClickItem();

private:
    std::string mOid;

    std::string mMahalle;
    std::string mTarih;
    std::string mDurum;
    std::string mDurumColor;


    Signal<std::string> _clickItem;
};

#endif // TALEPLISTWIDGET_H
