#include "mainapplication.h"
#include <mongocxx/instance.hpp>

#include <QApplication>

#include "db.h"

#include <Wt/WServer.h>
#include <Wt/WResource.h>
#include <Wt/Http/Request.h>
#include <Wt/Http/Response.h>

#include <string>
#include <bitset>

#include <ranges>

#include "SerikBelediyesiWebSayfasi/widget/cssbuilder.h"

///https://www.flaticon.com/packs/essential-collection
/// https://codepen.io/asfarmed/pen/cfslr
///
///

//std::unique_ptr<Wt::WApplication> createApplication(const Wt::WEnvironment& env)
//{
//    auto app = cpp14::make_unique<MainApplication>(env);
//    return app;
//}


//class PlaintextResource : public Wt::WResource {
//    int counter = 0;


//public:
//    PlaintextResource(){

//        this->dataChanged ().connect ([=](){
//            std::cout << "DATA Changed" << std::endl;
//        });

//        this->dataExceeded ().connect ([=]( const uint64_t size ){
//            std::cout << "File Size Exceed: " << size << std::endl;
//        });

//        this->dataReceived ().connect ([=](const uint64_t& size, const uint64_t& data ){
//            std::cout << "File Size: " << size << " Size DataSize: " << data << std::endl;
//        });
//    }

////    std::string fromIstream(std::istream &stream)
////    {
////        std::istreambuf_iterator<char> eos;
////         std::cout << std::string(std::istreambuf_iterator<char>(stream), eos) << "\n";
////        return "";
////    }

//    QByteArray fromIstream(std::istream &stream)
//    {
//        QByteArray ar;
//        char c;
//        while(stream.read(&c,1) && !stream.eof() ){
//            ar.append(c);
//        }
//        return ar;
//    }

//    virtual void handleRequest(const Wt::Http::Request &request, Wt::Http::Response &response) {


//        std::cout << "Method: "<<request.method () << std::endl;

//        long long length = 0;

//        for( const auto &item : request.headers () ){
//            std::cout << "name: "<<item.name () << " value: " << item.value () << std::endl;
//            if( item.name() == "Content-Length" ){
//                length = std::stoull(item.value());
//            }
//        }

//        auto var = fromIstream(request.in());


//        bsoncxx::document::view view{(std::uint8_t*)var.data(),static_cast<size_t>(var.size())};


//        std::cout << "path: " << request.path () << std::endl;

//        for( const auto &item : request.getParameterMap () ){
//            auto str = item.first;
//            std::cout << str << "  " << item.second.size() << std::endl;
//            for( const auto &_char : item.second ){
//                std::cout << _char;
//            }
//            std::cout << "\n";
//            std::cout << str.c_str()<<"\n";
////             response.out() << str;
//        }


//        response.out().write((const char*)view.data(),view.length());


//    }
//};



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mongocxx::instance ins{};

    Widget::CSSBuilder::instance();


    SerikBLDCore::DB::setUrl(_url);

    std::cout << __cplusplus << "\n";


//    try {
//        int argc = 6;
//        auto argv1 = std::unique_ptr<char*[]>(new char*[argc]);

//        argv1[0] = (char *) "multiple";
//        argv1[1] = (char *) "--http-address=0.0.0.0";
//        argv1[2] = (char *) "--http-port=8080";
//        argv1[3] = (char *) "--deploy-path=/";
//        argv1[4] = (char *) "--docroot=docroot";
//        argv1[5] = (char *) "--approot=approot";

//        WServer server1(argc, argv1.get(), WTHTTP_CONFIGURATION);

//        PlaintextResource plaintextResource;
//        server1.addResource(&plaintextResource, "/plaintext");

//        server1.addEntryPoint(EntryPointType::Application, createApplication);

//        if (server1.start()) {
//            WServer::waitForShutdown();
//            server1.stop();
//        }
//    } catch (WServer::Exception& e) {
//        std::cerr << e.what() << std::endl;
//    } catch (std::exception &e) {
//        std::cerr << "exception: " << e.what() << std::endl;
//    }







        return Wt::WRun(argc,argv,[](const Wt::WEnvironment &env){
            return Wt::cpp14::make_unique<MainApplication>(env);
        });
}
