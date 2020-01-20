#ifndef FOOTER_H
#define FOOTER_H

#include <QtCore/qglobal.h>

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
#include <Wt/WLink.h>

#include <Wt/WMessageBox.h>
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include "bootstrap.h"
#include "inlinestyle.h"

#include <iostream>
#include <memory>

using namespace Wt;


namespace Footer {

    class Footer : public WContainerWidget
    {
    public:
        Footer();

        void showMessage( std::string title , std::string msg );


        double getWidth() const;
        void setwidth(double width);

        double getHeight() const;
        void setheight(double height);

    private:
        double _width;
        double _height;

    };

}


#endif // FOOTER_H
