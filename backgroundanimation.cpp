#include "backgroundanimation.h"
#include <random>
#include "Wt/WApplication.h"
#include "inlinestyle.h"


#include <Wt/WCssStyleSheet.h>
#include <Wt/WApplication.h>



using namespace Wt;


namespace Background {

BackgroundAnimation::BackgroundAnimation()
{




    auto container = addWidget(cpp14::make_unique<WContainerWidget>());
    //        container->addStyleClass (Bootstrap::Grid::Hidden::hidden_xs+Bootstrap::Grid::Hidden::hidden_sm);

    container->setWidth(WLength("100%"));
    container->setHeight(WLength("100%"));
    container->setPositionScheme(PositionScheme::Fixed);

    container->addStyleClass("backanimation");



    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(1,35);
        auto index = dist(mt);
        std::string photoUrl = "v2/slide/"+std::to_string (index)+".jpg";
        container->setAttributeValue (Style::style,Style::background::url (photoUrl)+
                                                       Style::background::size::cover+
                                                       Style::background::repeat::norepeat);
    }

    container->setZIndex(-100);


    auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
    _container->setWidth(WLength("100%"));
    _container->setHeight(WLength("100%"));
    _container->setPositionScheme(PositionScheme::Absolute);
//    _container->setAttributeValue(Style::style,Style::background::url("v2/slide/Slidefilter.png")
//                                                    +Style::background::repeat::repeat);
    _container->setZIndex(-99);
    auto styleRule = std::make_unique<Wt::WCssTextRule>(".backAnimator ", "background: rgb(0,77,134); background: linear-gradient(0deg, rgba(0,77,134,1) 0%, rgba(0,115,200,0.9023984593837535) 13%, rgba(0,149,180,0.22248249299719892) 100%);");
    Wt::WApplication::instance()->styleSheet().addRule(std::move(styleRule));
    _container->addStyleClass("backAnimator");
}

} // namespace Background
