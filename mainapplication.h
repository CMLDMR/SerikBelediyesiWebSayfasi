#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H



#include <Necessary>

#include "mongoheaders.h"
#include "SerikBelediyesiWebSayfasi/src/body.h"


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
    MainApplication(const WEnvironment& env);
    ~MainApplication();


    ///
    /// \brief start initializing www.serikbelediyesi.com
    ///
    void init();


    void ShowForeGround();



private:

    mongocxx::client* mClient;

    mongocxx::database db;

    mongocxx::gridfs::bucket Bucket;

    ///
    /// \brief p_wtTheme
    /// Bootstrap Theme
    std::shared_ptr<Wt::WBootstrapTheme> p_wtTheme;


};

#endif // MAINAPPLICATION_H
