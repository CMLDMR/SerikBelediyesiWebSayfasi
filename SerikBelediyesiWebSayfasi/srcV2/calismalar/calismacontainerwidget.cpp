#include "calismacontainerwidget.h"

v2::CalismaContainerWidget::CalismaContainerWidget(const std::vector<std::string> &imgLinkList)
{

    std::copy(imgLinkList.begin (),imgLinkList.end (),std::back_inserter(ImgLinkList));


    setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::BlueViolet)+
                       Style::color::color (Style::color::White::AliceBlue)+
                       Style::font::family::dosis+Style::font::size::s14px);

    setMargin (25,Side::Top);
    setPadding (25,Side::Bottom|Side::Top);
    addStyleClass (CSSStyle::Radius::radius3px);

}

void v2::CalismaContainerWidget::initCalismaWidget()
{

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addWidget (cpp14::make_unique<WText>(this->CalismaAdi ()));
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addWidget (cpp14::make_unique<WText>(this->Mahalle ()));
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addWidget (cpp14::make_unique<WText>(this->IsTipii ()));
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addWidget (cpp14::make_unique<WText>(std::to_string (this->Miktar ())+" "+this->IsBirim ()));
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setMargin (15,Side::Top);
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->addWidget (cpp14::make_unique<WText>(this->Aciklama()));
    }


    Footer ()->setMargin (25,Side::Top);
    for( auto item : ImgLinkList ){
        auto imgContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());

        imgContainer->setAttributeValue (Style::style,Style::background::url (item)+
                                         Style::background::size::cover+
                                         Style::background::repeat::norepeat+
                                         Style::background::position::center_center);
        imgContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                     Bootstrap::Grid::Medium::col_md_3+
                                     Bootstrap::Grid::Small::col_sm_6+
                                     Bootstrap::Grid::ExtraSmall::col_xs_6);
        imgContainer->setHeight (200);
        imgContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        imgContainer->decorationStyle ().setCursor (Cursor::PointingHand);

        imgContainer->clicked ().connect ([=](){
           imgContainer->doJavaScript ("window.open('"+ item +"','_blank');");
        });

    }
}

void v2::CalismaContainerWidget::errorOccured(const std::string &errorText)
{

    this->showPopUpMessage ("Hata: " + errorText,"err");

}
