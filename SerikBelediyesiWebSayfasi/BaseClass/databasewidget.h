#ifndef DATABASEWIDGET_H
#define DATABASEWIDGET_H

#include <QtCore/qglobal.h>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDate>
#include <QString>

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







#include "bootstrap.h"
#include "inlinestyle.h"
#include "mongoheaders.h"
#include "serikbeltr.h"
#include "SerikBelediyesiWebSayfasi/chart/donutchart.h"

#include <iostream>
#include <memory>
#include <iterator>
#include <random>
#include <chrono>
#include <stdexcept>

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




using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;

using namespace Wt;

class DataBaseWidget : public WContainerWidget
{
    mongocxx::database* db;
public:
    DataBaseWidget(mongocxx::database* _db )
        :db(_db)
    {
        Bucket = this->db->gridfs_bucket();
    }

    mongocxx::collection collection(std::string collection) const{
        return this->db->collection(collection);
    }

    mongocxx::gridfs::bucket bucket() const{
        return Bucket;
    }

    mongocxx::database* getDB(){
        return this->db;
    }

    inline int getRandom(int begin = 0 , int end = 255 )
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(begin,end);
        return dist(mt);
    }


    void showMessage( std::string title , std::string msg , std::string btnText = "OK");
    void showMessage( std::string title , std::string msg , bsoncxx::exception &e , Wt::Icon icon = Icon::Warning);
    void showMessage( std::string title , std::string msg , mongocxx::exception &e , Wt::Icon icon = Icon::Warning);

    std::int64_t count(std::string collection , bsoncxx::document::view &filter ) ;
    std::int64_t count(std::string collection , const document &filter = document{} ) ;
    std::int64_t count(std::string collection , bsoncxx::document::value val ) ;

private:
    mongocxx::gridfs::bucket Bucket;

};

#endif // DATABASEWIDGET_H
