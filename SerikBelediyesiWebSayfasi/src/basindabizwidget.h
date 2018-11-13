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




    Signal<NoClass> &getTumilanlar();

    Signal<NoClass> &ClickAnayfa();

    ///
    /// \brief footerRowContainer
    /// \return
    ///
    WContainerWidget *footerRowContainer() const;

private:
    bool mInitForBody;

    WContainerWidget *rowContainer() const;


    WContainerWidget* mRowContainer;

    WContainerWidget* mHeaderRowContainer;

    WContainerWidget *headerRowContainer() const;

    WContainerWidget* mFooterRowContainer;


    Signal<NoClass> m_TumBasiliYayinlar;

    Signal<NoClass> _ClickBack;


    std::int64_t skip;

    std::int64_t itemCount;

    void loadListMore( const int& limit = 12 );

};



class BasinItem : public DataBaseWidget
{

public:

    BasinItem(mongocxx::database* _db , const bsoncxx::document::view &view);

    Signal<std::string> &getClick();

    void setGazetefileoid(const std::string &value);

    void setGazeteTitle(const std::string &value);

    void setTarihTitle(const std::string &value);

    std::string getGazetefileoid() const;

    std::string getGazeteTitle() const;

    std::string getTarihTitle() const;

private:

    Signal<std::string> _Click;


    std::string gazetefileoid;

    std::string gazeteTitle;

    std::string tarihTitle;

//    WContainerWidget* baskiContainer;


};

#endif // BASINDABIZWIDGET_H
