#ifndef MAINPAGE_H
#define MAINPAGE_H

//#include "SerikBelediyesiWebSayfasi/BaseClass/databasewidget.h"
#include "headerpage.h"
#include "SerikBelediyesiWebSayfasi/src/footer.h"
#include "db.h"


class MainPage : public WContainerWidget, public SerikBLDCore::DB
{
public:
    explicit MainPage(mongocxx::database* _db);

    WContainerWidget* mContentWidget;

    void init();

    void initHaberler();

    void initCalismalar();

    void initProjeler();

    void initEtkinlikler();

    void initBilgiEdinme();

    void initGiris();

    void initMeclis();

    void initHakkinda();

    void initIletisim();

    void initAnounceList();

    void initBaskan();

    bool initMeclisCanliYayin();

    void initNobetciEczane();

    void initBaskanYardimcilari();

    void initMeclisUyeleri();

    void initNostSerik();

    void initKVKK();


    JSignal<int,int> _signal;

    Footer::Footer* footer;
};

#endif // MAINPAGE_H
