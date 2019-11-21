#include "dilekcelistwidgetitem.h"

DilekceListWidgetItem::DilekceListWidgetItem(const SerikBLDCore::Dilekce dilekce )
{
    addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    setMargin (5,Side::Top);
    decorationStyle ().setCursor (Cursor::PointingHand);
    addStyleClass ("boxShadow");
    addStyleClass (Bootstrap::ContextualBackGround::bg_success);


    auto rContainer = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass (Bootstrap::Grid::row);

    {
        auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setContentAlignment (AlignmentFlag::Left);

        container->addWidget (cpp14::make_unique<WText>(dilekce.konu ().toStdString ()));
    }

    {
        auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setContentAlignment (AlignmentFlag::Center);

        container->addWidget (cpp14::make_unique<WText>(dilekce.tarihText ().toStdString ()));
    }

    {
        auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setContentAlignment (AlignmentFlag::Center);

        container->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (dilekce.sayi ()).toUTF8 ()));
    }

    {
        auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setContentAlignment (AlignmentFlag::Right);

        container->addWidget (cpp14::make_unique<WText>(dilekce.Durum ().toStdString ()));
    }

    clicked ().connect ([=](){
        _ClickDilekce.emit (dilekce.oid ().value ().to_string ());
    });







}

Signal<std::string> &DilekceListWidgetItem::ClickDilekceWidget()
{
    return _ClickDilekce;
}

