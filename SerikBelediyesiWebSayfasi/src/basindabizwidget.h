#ifndef BASINDABIZWIDGET_H
#define BASINDABIZWIDGET_H

#include <QtCore/qglobal.h>

#include "SerikBelediyesiWebSayfasi/BaseClass/databasewidget.h"

class BasindaBizWidget : public DataBaseWidget
{
public:
    ///
    /// \brief BasindaBizWidget
    /// \param _db
    /// \param initForBody
    /// initForBody if true this construct for Main page else its own page
    BasindaBizWidget(mongocxx::database* _db , bool initForBody = true );


    ///
    /// \brief initForBody
    /// init Basinda Biz Content For Body Page
    void initForBody();

    ///
    /// \brief initForBasin
    /// init Basinda Biz Content Basin Page
    void initForBasin();




private:
    bool mInitForBody;

    WContainerWidget *rowContainer() const;


    WContainerWidget* mRowContainer;

    WContainerWidget* mHeaderRowContainer;

    WContainerWidget *headerRowContainer() const;

};



class BasinItem : public DataBaseWidget
{
public:
    BasinItem(mongocxx::database* _db);
};

#endif // BASINDABIZWIDGET_H
