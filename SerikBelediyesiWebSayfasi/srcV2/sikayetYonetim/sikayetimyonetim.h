#ifndef SIKAYETIMYONETIM_H
#define SIKAYETIMYONETIM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/basewidget.h"


class BilgiEdinmeYonetim;

class SikayetimYonetim : public BaseClass::ContainerWidget
{
public:
    SikayetimYonetim(mongocxx::database* _db , bsoncxx::document::value _user);


private:
    WContainerWidget* mContentContainer;

    void initBilgiEdinme();

    void initSikayetler();
};

#endif // SIKAYETIMYONETIM_H
