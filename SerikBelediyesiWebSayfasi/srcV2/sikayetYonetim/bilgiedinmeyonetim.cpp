#include "bilgiedinmeyonetim.h"



BilgiEdinmeYonetim::BilgiEdinmeYonetim(mongocxx::database *_db, const bsoncxx::document::value &_userValue)
    :DBClass (_db),UserClass (_userValue)
{
    setContainerStyle(ContainerStyleType::ROW);

    auto list = BilgiEdinmeItem::GetList(this->db());

    for( auto item : list )
    {
        if( item )
        {
            auto container = addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::col_full_12);
            container->addWidget(cpp14::make_unique<BilgiEdinmeWidget>(item.get()));
        }
    }

}

BilgiEdinmeWidget::BilgiEdinmeWidget( const BilgiEdinmeItem *_item)
    :DBClass (_item->db()),item(_item)
{
    setWidth(WLength("100%"));
    setContainerStyle(ContainerStyleType::ROW);
    addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    decorationStyle().setCursor(Cursor::PointingHand);
    setMargin(10,Side::Top);

    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4);
        container->addWidget(cpp14::make_unique<WText>(this->item->Konu()));
    }

    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4);
        container->addWidget(cpp14::make_unique<WText>(this->item->Tarih()));
    }

    {
        auto container = addWidget(cpp14::make_unique<ContainerWiget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4);
        if( this->item->cevaplandi() )
        {
            container->addWidget(cpp14::make_unique<WText>("Cevaplandı"));
            setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(125,150),
                                                                                    this->getRandom(225,250),
                                                                                    this->getRandom(125,150)));
        }else{
            container->addWidget(cpp14::make_unique<WText>("Cevaplanmadı"));
            setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(225,250),
                                                                                    this->getRandom(125,150),
                                                                                    this->getRandom(125,150)));
        }
    }

}
