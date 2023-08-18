#include "footer.h"
#include "bootstrap.h"
#include "inlinestyle.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"


Footer::Footer::Footer()
{

//    setPositionScheme(PositionScheme::Absolute);
//    setOffsets(0,Side::Bottom);
//    setWidth(WLength("100%"));

    this->addStyleClass(Bootstrap::Grid::col_full_12);

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    mMainContainer->setContentAlignment(AlignmentFlag::Center);

    mMainContainer->setAttributeValue(Style::style,Style::background::color::rgba(25,25,25));


    auto rContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    rContainer->setMaximumSize(1024,WLength::Auto);



    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->addStyleClass("footerText");

        container->setPadding(0,AllSides);
        auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
        layout->addWidget(cpp14::make_unique<WText>("Serik Belediye Başkanlığı"));
    }




    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->addStyleClass("footerText");

        auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto text = layout->addWidget(cpp14::make_unique<WText>("Bu Web Sayfası Serik Belediyesi Bilgi İşlem Müdürlüğü Tarafından Açık Kaynak Kodlu Olarak Geliştirilmektedir."));
        text->addStyleClass("footerText");
    }



    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->addStyleClass("footerText");

        auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
        layout->addStretch(1);
        layout->addWidget(cpp14::make_unique<WText>("© Copyright 2019  "))->addStyleClass("footerText");
        {
            Wt::WLink link = Wt::WLink("http://www.webtoolkit.eu/");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,"Wt");

            layout->addWidget(std::move(anchor));
        }
        layout->addWidget(cpp14::make_unique<WText>(","))->addStyleClass("footerText");
        {
            Wt::WLink link = Wt::WLink("http://www.qt.io/");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,"Qt");

            layout->addWidget(std::move(anchor));
        }
        layout->addWidget(cpp14::make_unique<WText>(WString::fromUTF8(" & "),TextFormat::Plain))->addStyleClass("footerText");
        {
            Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/SerikBelediyesiWebSayfasi");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,"gitHub");

            layout->addWidget(std::move(anchor));
        }

        layout->addWidget(cpp14::make_unique<WText>(QString("   yapım@%1.%2.%3.%4").arg (MAJOR).arg (MINOR).arg (REV).arg (COM).toStdString ()))->addStyleClass("footerText");
        layout->addStretch(1);
    }
}



double Footer::Footer::getWidth() const
{
    return _width;
}

void Footer::Footer::setwidth(double width)
{
    _width = width;
}

double Footer::Footer::getHeight() const
{
    return _height;
}

void Footer::Footer::setheight(double height)
{
    _height = height;
}
