#ifndef MAINPAGECONTROLLER_H
#define MAINPAGECONTROLLER_H


#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

using namespace Wt;


///
/// \brief The MainPageController class
/// Ana Sayfa Kontrol Buttonları
/// 1. Haber
/// 2. Çalışmalar
/// 3. Projeler
/// 4. Birimler
/// 5. e-Belediye
/// 6. Etkinlikler
/// 7. Galeri
/// 8. Başvurular

class MainPageController : public ContainerWidget
{
public:
    explicit MainPageController();

    Signal<NoClass> &ClickHaber();

    Signal<NoClass> &ClickCalisma();

    Signal<NoClass> &ClickProjeler();

    Signal<NoClass> &ClickEtkinlikler();

    Signal<NoClass> &ClickBilgiEdinme();

    Signal<NoClass> &ClickGiris();

    Signal<NoClass> &ClickMeclis();

    Signal<std::string> &ClickAnounce();

    Signal<NoClass> &ClickIletisim();

    Signal<NoClass> &ClickHakkinda();

    Signal<NoClass> &ClickDuyurular();

    Signal<NoClass> &ClickNobetciEczane();

    Signal<NoClass> &ClickNostSerik();

    Signal<NoClass> &ClickYonetim();

private:
    Signal<NoClass> _Haber;

    Signal<NoClass> _Calisma;

    Signal<NoClass> _Projeler;

    Signal<NoClass> _Etkinlikler;

    Signal<NoClass> _BilgiEdinme;

    Signal<NoClass> _Giris;

    Signal<NoClass> _Meclis;

    Signal<std::string> _Announce;

    Signal<NoClass> _iletisim;

    Signal<NoClass> _hakkinda;

    Signal<NoClass> _duyurular;

    Signal<NoClass> _NobetciEczane;

    Signal<NoClass> _NostSerik;

    Signal<NoClass> _Yonetim;


};

#endif // MAINPAGECONTROLLER_H
