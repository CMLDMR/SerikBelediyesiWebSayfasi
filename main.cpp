#include "mainapplication.h"
#include <mongocxx/instance.hpp>

#include <QApplication>

#include "db.h"





///https://www.flaticon.com/packs/essential-collection
/// https://codepen.io/asfarmed/pen/cfslr

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    WString::setDefaultEncoding(Wt::CharEncoding::UTF8);

    mongocxx::instance ins{};




    return Wt::WRun(argc,argv,[](const Wt::WEnvironment &env){
        return Wt::cpp14::make_unique<MainApplication>(env);
    });
}
