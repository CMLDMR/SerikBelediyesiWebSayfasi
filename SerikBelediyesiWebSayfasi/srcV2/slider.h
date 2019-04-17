#ifndef SLIDER_H
#define SLIDER_H

#include "SerikBelediyesiWebSayfasi/BaseClass/databasewidget.h"

class Slider : public DataBaseWidget
{
public:
    Slider( mongocxx::database* _db );

    virtual ~Slider();

    void init();

    JSignal<std::string> _height;

private:
    WTimer* timer;
    void slot();


};

#endif // SLIDER_H
