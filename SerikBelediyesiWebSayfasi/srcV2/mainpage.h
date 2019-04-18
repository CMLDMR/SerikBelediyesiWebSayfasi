#ifndef MAINPAGE_H
#define MAINPAGE_H

#include "SerikBelediyesiWebSayfasi/BaseClass/databasewidget.h"
#include "headerpage.h"

class MainPage : public DataBaseWidget
{
public:
    MainPage(mongocxx::database* _db);

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

    void initAnounceDetail( std::string mOid );

    std::string downloadifNotExist( bsoncxx::types::value oid , bool forceFilename = false );
    std::string downloadifNotExist( std::string oid , bool forceFilename = false );
};

#endif // MAINPAGE_H
