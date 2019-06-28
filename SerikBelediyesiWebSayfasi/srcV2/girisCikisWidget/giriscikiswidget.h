#ifndef GIRISCIKISWIDGET_H
#define GIRISCIKISWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/basewidget.h"

class GirisCikisWidget : public BaseClass::ContainerWidget
{
public:
    GirisCikisWidget(mongocxx::database* _db , bsoncxx::document::value _user );
    WComboBox* mYearSelect;
    WComboBox* mMounthSelect;
    WPushButton* mGosterBtn;
    WContainerWidget* mContentContainer;

    void initGirisCikis();

    void initPersonelMenu();


    void addRow(QDate date);



private:
    void addRow(QString personeloid , qint64 julianDay);
    void addNinRow(QString personeloid , QString BirimName);
    void initBaskanMenu();
    WPushButton* mBugunBtn;
    void initBugun();
    WComboBox* mCurrentBirim;

    Wt::WCalendar *mCalender;
    void initSelectedDay(qint64 julianDay);
};

#endif // GIRISCIKISWIDGET_H
