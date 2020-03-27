#ifndef TALEPLISTWIDGET_H
#define TALEPLISTWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"




///
/// \brief The TalepListWidget class : TalepListWidget, Talep ler Bu Widget ile Listelenir.
/// <b>TalepOid</b><BR>
/// <b>Mahalle</b><BR>
/// <b>Tarih</b><BR>
/// <b>Durum</b><BR>
/// <b>DurumColor</b><BR> Bilgilerini Yapıcı Fonksiyon Parametresinde İster.
///
class TalepListWidget : public ContainerWidget
{
public:
    TalepListWidget(const std::string& oid,
                    const std::string& mahalle,
                    const std::string& tarih,
                    const std::string& durum,
                    const std::string& durumColor,
                    const std::string& kategoriName );


    ///
    /// \brief ClickItem: Bu Signal Widget Tıklandığı Zaman Tetiklenir.
    /// \return
    ///
    Signal<std::string> &ClickItem();

private:
    std::string mOid;

    std::string mMahalle;
    std::string mTarih;
    std::string mDurum;
    std::string mDurumColor;
    std::string mKategoriName;

    Signal<std::string> _clickItem;
};

#endif // TALEPLISTWIDGET_H
