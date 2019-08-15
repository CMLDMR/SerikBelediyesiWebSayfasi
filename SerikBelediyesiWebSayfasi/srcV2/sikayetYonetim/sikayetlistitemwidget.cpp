#include "sikayetlistitemwidget.h"




SikayetListItemWidget::SikayetListItemWidget(bsoncxx::oid _oid, const std::string durum, const std::string tarih, const std::string mahalle, const std::string birim, const std::string kategori, const std::string adsoyad)
{
    mOid = _oid;
    this->setContainerStyle(ContainerStyleType::ROW);
    this->setWidth(WLength("100%"));
    this->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    this->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(175,225),this->getRandom(175,225),this->getRandom(175,225)));

    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                 Bootstrap::Grid::Medium::col_md_2+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        container->addWidget(cpp14::make_unique<WText>(durum));
    }
}

Signal<bsoncxx::oid> &SikayetListItemWidget::ClickItem()
{
    return _ClickItem;
}
