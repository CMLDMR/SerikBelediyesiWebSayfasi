#include "sikayetlistitemwidget.h"




SikayetListItemWidget::SikayetListItemWidget(bsoncxx::oid _oid, const std::string durum, const std::string tarih, const std::string mahalle, const std::string birim, const std::string kategori, const std::string adsoyad)
{
    mOid = _oid;
    this->addStyleClass(Bootstrap::Grid::col_full_12);
    this->setWidth(WLength("100%"));
    this->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    this->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(175,225),this->getRandom(175,225),this->getRandom(175,225)));
    this->decorationStyle().setCursor(Cursor::PointingHand);
    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                 Bootstrap::Grid::Medium::col_md_2+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addWidget(cpp14::make_unique<WText>(durum));
    }
    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                 Bootstrap::Grid::Medium::col_md_2+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::left::border("2px solid white"));
        container->addWidget(cpp14::make_unique<WText>(tarih));
    }
    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                 Bootstrap::Grid::Medium::col_md_2+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::left::border("2px solid white"));

        container->addWidget(cpp14::make_unique<WText>(mahalle));
    }
    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                 Bootstrap::Grid::Medium::col_md_2+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::left::border("2px solid white"));

        container->addWidget(cpp14::make_unique<WText>(birim));
    }
    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                 Bootstrap::Grid::Medium::col_md_2+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::left::border("2px solid white"));

        container->addWidget(cpp14::make_unique<WText>(kategori));
    }
    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                 Bootstrap::Grid::Medium::col_md_2+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setAttributeValue(Style::style,Style::Border::left::border("2px solid white"));

        container->addWidget(cpp14::make_unique<WText>(adsoyad));
    }

    this->clicked().connect([=](){
        _ClickItem.emit(this->mOid);
    });
}

Signal<bsoncxx::oid> &SikayetListItemWidget::ClickItem()
{
    return _ClickItem;
}
