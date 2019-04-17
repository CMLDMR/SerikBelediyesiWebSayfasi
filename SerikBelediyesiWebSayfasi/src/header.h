#ifndef HEADER_H
#define HEADER_H

#include <QtCore/qglobal.h>
#include <QFile>
#include <QDate>

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
#include <Wt/WAnchor.h>
#include <Wt/WNavigationBar.h>
#include <Wt/WPopupMenu.h>
#include <Wt/WPopupMenuItem.h>
#include <Wt/WPushButton.h>

#include <Wt/WMessageBox.h>


#include "bootstrap.h"
#include "inlinestyle.h"

#include <iostream>
#include <memory>

using namespace Wt;




namespace Header {


    class Header : public WContainerWidget
    {
    public:
        Header();



        Signal<NoClass> &mClickAnaSayfa();
        Signal<NoClass> &mClickYonetim();
        Signal<NoClass> &mClickMeclis();
        Signal<NoClass> &mClickProje();
        Signal<NoClass> &mClickHaber();
        Signal<NoClass> &mClickEtkinlik();
        Signal<NoClass> &mClickVideo();
        Signal<NoClass> &mClickBilgiEdin();
        Signal<NoClass> &mClickIletisim();
        Signal<NoClass> &mClickGiris();
        Signal<NoClass> &mClickHakkinda();
        Signal<NoClass> &mClickCalisma();
        Signal<NoClass> &mClickMeclisUyeleri();

    private:
        Signal<NoClass> mClickAnaSayfa_;
        Signal<NoClass> mClickYonetim_;
        Signal<NoClass> mClickMeclis_;
        Signal<NoClass> mClickProje_;
        Signal<NoClass> mClickHaber_;
        Signal<NoClass> mClickEtkinlik_;
        Signal<NoClass> mClickVideo_;
        Signal<NoClass> mClickBilgiEdin_;
        Signal<NoClass> mClickIletisim_;
        Signal<NoClass> mClickGiris_;
        Signal<NoClass> mClickHakkinda_;
        Signal<NoClass> mClickCalisma_;
        Signal<NoClass> mClickMeclisUyeleri_;

        Signal<NoClass> mClickENG;


        void showMessage( std::string title , std::string msg );



    };


}



#endif // HEADER_H
