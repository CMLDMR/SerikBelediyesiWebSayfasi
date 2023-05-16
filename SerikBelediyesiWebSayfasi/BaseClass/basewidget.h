#ifndef BASEWIDGET_H
#define BASEWIDGET_H

#include <QtCore/qglobal.h>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDate>
#include <QString>
#include <QTime>
#include <QImage>
#include <QString>
#include <QXmlStreamWriter>
#include <QByteArray>
#include <QDate>
#include <QTime>
#include <QStringList>
#include <QUuid>
#include <QImage>
#include <QImageReader>
#include <QDebug>
#include <QTextDocument>


#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WEvent.h>
#include <Wt/WSignal.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WImage.h>
#include <Wt/WLink.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WPushButton.h>
#include <Wt/WDate.h>
#include <Wt/WDateEdit.h>
#include <Wt/WMediaPlayer.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WTable.h>
#include <Wt/WTableCell.h>
#include <Wt/WMessageBox.h>
#include <Wt/WAnchor.h>
#include <Wt/WGridLayout.h>
#include <Wt/WLineEdit.h>
#include <Wt/WComboBox.h>
#include <Wt/WTextEdit.h>
#include <Wt/WMessageBox.h>
#include <Wt/Http/Client.h>
#include <Wt/WDialog.h>
#include <Wt/WTimer.h>
#include <Wt/WToolBar.h>
#include <Wt/WCheckBox.h>
#include <Wt/WLabel.h>
#include <Wt/WEnvironment.h>
#include <Wt/WSpinBox.h>
#include <Wt/WLineF.h>
#include <Wt/WCalendar.h>
#include <Wt/WDate.h>
#include <Wt/WFileUpload.h>
#include <Wt/WProgressBar.h>
#include <Wt/Http/Request.h>
#include <Wt/WProgressBar.h>
#include <Wt/WSignal.h>
#include <Wt/WTime.h>
#include <Wt/WJavaScript.h>
#include <Wt/WCalendar.h>







#include "bootstrap.h"
#include "inlinestyle.h"
#include "mongoheaders.h"
#include "serikbeltr.h"

#include <iostream>
#include <memory>
#include <iterator>
#include <random>
#include <chrono>




const int RandomImageCount = 15;


using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;

using namespace Wt;

namespace BaseClass {


class BaseWidget : public WContainerWidget
{
public:
    BaseWidget(mongocxx::database* _db , std::string tel);
    BaseWidget(mongocxx::database* _db , bsoncxx::document::value _user);

    void showMessage( std::string title , std::string msg , std::string btnText = "OK");
    void showMessage( std::string title , std::string msg , bsoncxx::exception &e , Wt::Icon icon = Icon::Warning);
    void showMessage( std::string title , std::string msg , mongocxx::exception &e , Wt::Icon icon = Icon::Warning);

    mongocxx::database* db() const;

    bsoncxx::document::value User() const;

    std::unique_ptr<Wt::WPushButton> createColorButton(const char *className, const Wt::WString& text);

//    mongocxx::cursor find(std::string collection , const bsoncxx::document::view &view = document{}.view() ,mongocxx::options::find options = mongocxx::options::find());

//    mongocxx::stdx::optional<bsoncxx::document::value> find_one(std::string collection , const bsoncxx::document::view &view = document{}.view(), mongocxx::options::find options = mongocxx::options::find());

    std::int64_t count(std::string collection , bsoncxx::document::view &filter);
    std::int64_t count(std::string collection , bsoncxx::document::value val );
//    std::int64_t count(std::string collection , document &filter = document{} );

    const std::string downloadFile(const std::string &oid , const bool &forceFilename = false);

    const bsoncxx::types::bson_value::value uploadfile( QString filepath );


    std::string UserBirim(){
        return this->User().view()[SBLDKeys::Personel::birimi].get_string().value.data();
    }

private:
    mongocxx::database* __db;
    bsoncxx::document::value _User;
    void setUser(const bsoncxx::document::value &User);
};



class ContainerWidget : public BaseWidget
{
public:
    ContainerWidget(mongocxx::database* _db , bsoncxx::document::value _user , std::string ContainerTitle = "NOCONTAINER");

    WContainerWidget* getHeaderRowContainer() const{
        return this->mHeaderRowContainer;
    }
    WContainerWidget* getContentRowContainer() const{
        return this->mContentRowContainer;
    }
    WContainerWidget* getFooterRowContainer() const{
        return this->mFooterRowContainer;
    }

    inline int getRandom(int begin = 0 , int end = 127 ) const
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(begin,end);
        return dist(mt);
    }

    std::string getBackGroundImg() const;

    Signal<NoClass> &getReinit(){ return _mClickReinit; }
private:
    WContainerWidget* mHeaderRowContainer;
    WContainerWidget* mContentRowContainer;
    WContainerWidget* mFooterRowContainer;

    Signal<NoClass> _mClickReinit;
};


}



#endif // BASEWIDGET_H
