#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H



#include <Necessary>

#include "mongoheaders.h"
#include "SerikBelediyesiWebSayfasi/src/body.h"

#include <random>


using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;

using namespace Wt;




///
/// \brief The MainApplication class
/// This is Web Application Server. Contruct and Manage Page. Deliver the some neccessary variabla from platform or host
class MainApplication : public WApplication
{
public:
    explicit MainApplication(const WEnvironment& env);
    ~MainApplication();


    ///
    /// \brief start initializing www.serikbelediyesi.com
    ///
    void init();

    void showYapilandirmaEmlak();


    void ShowForeGround();

    bool loadDilekce( const std::string &oid );

    bool loadTalep( const std::string &oid );

    bool loadGundem( const std::string& oid );





private:

    mongocxx::client* mClient;

    mongocxx::database db;

    mongocxx::gridfs::bucket Bucket;

    ///
    /// \brief p_wtTheme
    /// Bootstrap Theme
    std::shared_ptr<Wt::WBootstrapTheme> p_wtTheme;





};



class tWidget : public WContainerWidget
{
public:
    tWidget() {
        setContentAlignment(AlignmentFlag::Center);



//        decorationStyle().setBorder(WBorder(BorderStyle::Solid));

        setAttributeValue(Style::style,Style::background::url("img/01.jpg")+Style::background::size::contain
                          +Style::background::repeat::norepeat
                          +Style::background::position::center_center);

        setHeight(450);


//        addWidget(cpp14::make_unique<WText>("Yeni Web Sayfamız Çok Yakında..."));
        setMargin(20,Side::Bottom|Side::Top);
//        setMargin(50,Side::Bottom);



    }
};

#endif // MAINAPPLICATION_H
