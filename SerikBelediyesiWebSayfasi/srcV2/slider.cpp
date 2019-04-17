#include "slider.h"

Slider::Slider(mongocxx::database *_db)
    :DataBaseWidget (_db),_height(this,"_height")
{


//    std::cout << "------------------: " << WApplication::instance()->environment().screenHeight() << std::endl;

//    _height.connect([=](std::string str){
//        std::cout << "-----------_HEIGHT " << str << std::endl;
//    });


//    auto testScript = "var myHeight = 0; " + _height.createCall({"myHeight"}) ;

//    std::cout << "----------------- VAR HEIGHT: " << testScript << std::endl;

//    this->doJavaScript( testScript );


//    this->doJavaScript("  var myWidth = 0, myHeight = 0;"
//                       "if( typeof( window.innerWidth ) == 'number' ) {"
//                         //Non-IE
//                         "myWidth = window.innerWidth;"
//                         "myHeight = window.innerHeight;"
//                       "} else if( document.documentElement && ( document.documentElement.clientWidth || document.documentElement.clientHeight ) ) {"
//                         //IE 6+ in 'standards compliant mode'
//                         "myWidth = document.documentElement.clientWidth;"
//                         "myHeight = document.documentElement.clientHeight;"
//                       "} else if( document.body && ( document.body.clientWidth || document.body.clientHeight ) ) {"
//                         //IE 4 compatible
//                         "myWidth = document.body.clientWidth;"
//                         "myHeight = document.body.clientHeight;"
//                       "}"
//                       "console.log(myWidth);"
//                       "console.log(myHeight);" + _height.createCall({" myHeight "}) + ";");


    this->init();

}

Slider::~Slider()
{
    timer->stop();
    delete timer;
}

void Slider::init()
{
    setZIndex(-1);
    auto cContainer = addWidget(cpp14::make_unique<WContainerWidget>());
//    cContainer->decorationStyle().setBorder(WBorder(BorderStyle::Solid));

    cContainer->setWidth(WLength("100%"));
    cContainer->setHeight(768);
    cContainer->setPositionScheme(PositionScheme::Absolute);

    auto slider = cContainer->addWidget(cpp14::make_unique<WStackedWidget>());
    slider->setWidth(WLength("100%"));
    slider->setHeight(768);

    {
        auto container = slider->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setPositionScheme(PositionScheme::Relative);
        container->setWidth(WLength("100%"));
        container->setHeight(768);
        container->setContentAlignment(AlignmentFlag::Center);
        container->setAttributeValue(Style::style,Style::background::url("v2/slide/1.jpg")
                                     +Style::background::size::cover
                                     +Style::background::repeat::norepeat
                                     +Style::background::position::center_center);

//        auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        textContainer->setMaximumSize(1270,WLength::Auto);
//        textContainer->setHeight(WLength("100%"));

//        auto vLayout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//        textContainer->setZIndex(11);
//        vLayout->addStretch(1);
//        auto title = vLayout->addWidget(cpp14::make_unique<WText>("Eski Köprü"),0,AlignmentFlag::Left);
//        title->setAttributeValue(Style::style,Style::font::family::tahoma+Style::color::color(Style::color::White::Snow)+Style::font::size::s48px);
//        title->setPositionScheme(PositionScheme::Absolute);
//        title->setOffsets(WLength("30%"),Side::Top);
//        vLayout->addStretch(1);
    }

    {
        auto container = slider->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setPositionScheme(PositionScheme::Relative);
        container->setWidth(WLength("100%"));
        container->setHeight(WLength("100%"));

        container->setAttributeValue(Style::style,Style::background::url("v2/slide/2.jpg")
                                     +Style::background::size::cover
                                     +Style::background::repeat::norepeat
                                     +Style::background::position::center_center);

//        auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        textContainer->setPositionScheme(PositionScheme::Absolute);
//        textContainer->setOffsets(WLength("30%"),Side::Top);
//        textContainer->setOffsets(WLength("20%"),Side::Left);

//        auto title = textContainer->addWidget(cpp14::make_unique<WText>("Baraj"));
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s48px);
    }

    {
        auto container = slider->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setPositionScheme(PositionScheme::Relative);
        container->setWidth(WLength("100%"));
        container->setHeight(WLength("100%"));

        container->setAttributeValue(Style::style,Style::background::url("v2/slide/3.jpg")
                                     +Style::background::size::cover
                                     +Style::background::repeat::norepeat
                                     +Style::background::position::center_center);

//        auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        textContainer->setPositionScheme(PositionScheme::Absolute);
//        textContainer->setOffsets(WLength("30%"),Side::Top);
//        textContainer->setOffsets(WLength("20%"),Side::Left);
//        auto title = textContainer->addWidget(cpp14::make_unique<WText>("Aspendos"));
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s48px);
    }

    {
        auto container = slider->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setPositionScheme(PositionScheme::Relative);
        container->setWidth(WLength("100%"));
        container->setHeight(WLength("100%"));

        container->setAttributeValue(Style::style,Style::background::url("v2/slide/4.jpg")
                                     +Style::background::size::cover
                                     +Style::background::repeat::norepeat
                                     +Style::background::position::center_center);

//        auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        textContainer->setPositionScheme(PositionScheme::Absolute);
//        textContainer->setOffsets(WLength("30%"),Side::Top);
//        textContainer->setOffsets(WLength("20%"),Side::Left);
//        auto title = textContainer->addWidget(cpp14::make_unique<WText>("Golf"));
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s48px);
    }

    {
        auto container = slider->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setPositionScheme(PositionScheme::Relative);
        container->setWidth(WLength("100%"));
        container->setHeight(WLength("100%"));

        container->setAttributeValue(Style::style,Style::background::url("v2/slide/5.jpg")
                                     +Style::background::size::cover
                                     +Style::background::repeat::norepeat
                                     +Style::background::position::center_center);

//        auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        textContainer->setPositionScheme(PositionScheme::Absolute);
//        textContainer->setOffsets(WLength("30%"),Side::Top);
//        textContainer->setOffsets(WLength("20%"),Side::Left);
//        auto title = textContainer->addWidget(cpp14::make_unique<WText>("Deniz"));
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s48px);
    }

    {
        auto container = slider->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setPositionScheme(PositionScheme::Relative);
        container->setWidth(WLength("100%"));
        container->setHeight(WLength("100%"));

        container->setAttributeValue(Style::style,Style::background::url("v2/slide/6.jpg")
                                     +Style::background::size::cover
                                     +Style::background::repeat::norepeat
                                     +Style::background::position::center_center);

//        auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        textContainer->setPositionScheme(PositionScheme::Absolute);
//        textContainer->setOffsets(WLength("30%"),Side::Top);
//        textContainer->setOffsets(WLength("20%"),Side::Left);
//        auto title = textContainer->addWidget(cpp14::make_unique<WText>("Su Kemeri"));
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow)+Style::font::size::s48px);

    }




    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->setPositionScheme(PositionScheme::Relative);
        container->setWidth(WLength("100%"));
        container->setHeight(768);
        container->setAttributeValue(Style::style,Style::background::url("v2/slide/Slidefilter.png")
                                     +Style::background::repeat::repeat);
    }


    slider->setTransitionAnimation(WAnimation(AnimationEffect::Fade,TimingFunction::EaseInOut,2000));

    timer = new WTimer();

    timer->timeout().connect([=](){
        auto index = (slider->currentIndex() + 1 )%6;
        slider->setCurrentIndex(index);
    });

    timer->setInterval(std::chrono::milliseconds(5000));

    timer->start();
}

void Slider::slot()
{
    std::cout << "-------Hegiht : " << "mString" << std::endl;
}

