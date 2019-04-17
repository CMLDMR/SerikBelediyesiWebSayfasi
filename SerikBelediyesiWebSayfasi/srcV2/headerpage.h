#ifndef HEADERPAGE_H
#define HEADERPAGE_H

#include "SerikBelediyesiWebSayfasi/BaseClass/databasewidget.h"


class HeaderPage : public DataBaseWidget
{
public:
    HeaderPage(mongocxx::database* _db);


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
};

#endif // HEADERPAGE_H
