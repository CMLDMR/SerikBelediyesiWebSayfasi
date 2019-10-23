#ifndef SIKAYETLISTITEMWIDGET_H
#define SIKAYETLISTITEMWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

class SikayetListItemWidget : public ContainerWidget
{
public:
    SikayetListItemWidget(bsoncxx::oid _oid ,
                          const std::string durum,
                          const std::string tarih,
                          const std::string mahalle,
                          const std::string birim,
                          const std::string kategori,
                          const std::string adsoyad);


    Signal<bsoncxx::oid> &ClickItem();

private:

    Signal<bsoncxx::oid> _ClickItem;

    bsoncxx::oid mOid;
    std::string mDurum;
    std::string mTarih;
    std::string mMahalle;
    std::string mBirim;
    std::string mKategori;
    std::string mAdSoyad;
};

#endif // SIKAYETLISTITEMWIDGET_H
