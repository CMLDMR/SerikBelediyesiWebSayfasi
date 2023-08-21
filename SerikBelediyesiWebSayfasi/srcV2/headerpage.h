#ifndef HEADERPAGE_H
#define HEADERPAGE_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

class HeaderPage : public ContainerWidget
{
public:
    HeaderPage();


    Signal<NoClass> &ClickAnaSayfa();

    Signal<NoClass> &ClickHaber();

    Signal<NoClass> &ClickCalisma();

    Signal<NoClass> &ClickProjeler();

    Signal<NoClass> &ClickEtkinlikler();

    Signal<NoClass> &ClickBilgiEdinme();

    Signal<NoClass> &ClickGiris();

    Signal<NoClass> &ClickMeclis();

    Signal<NoClass> &ClickHakkinda();

    Signal<NoClass> &ClickIletisim();

    Signal<NoClass> &ClickBaskan();

    Signal<NoClass> &ClickBaskanYrd();

    Signal<NoClass> &ClickMeclisUyeleri();

    Signal<NoClass> &ClickKvkkMetinleri();


private:
    Signal<NoClass> _Anasayfa;

    Signal<NoClass> _Haber;

    Signal<NoClass> _Calisma;

    Signal<NoClass> _Projeler;

    Signal<NoClass> _Etkinlikler;

    Signal<NoClass> _BilgiEdinme;

    Signal<NoClass> _Giris;

    Signal<NoClass> _Meclis;

    Signal<NoClass> _Hakkinda;

    Signal<NoClass> _Iletisim;

    Signal<NoClass> _baskan;

    Signal<NoClass> _baskanYrd;

    Signal<NoClass> _MeclisUyeleriPage;

    Signal<NoClass> _kvkkMetinleri;


};

#endif // HEADERPAGE_H
