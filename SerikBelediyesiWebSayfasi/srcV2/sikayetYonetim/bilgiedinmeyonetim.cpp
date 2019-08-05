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
            container->addWidget(cpp14::make_unique<BilgiEdinmeListWidget>(item.get()))->ClickBilgiEdinme().connect(this,&BilgiEdinmeYonetim::initBilgiEdinme);
        }
    }

}

void BilgiEdinmeYonetim::initBilgiEdinme(const bsoncxx::oid &oid)
{
    clear();
    addWidget(cpp14::make_unique<BilgiEdinmeWidget>(this->db(),oid,this->User()));
}

BilgiEdinmeListWidget::BilgiEdinmeListWidget( const BilgiEdinmeItem *_item)
    :DBClass (_item->db()),item(_item)
{
    setWidth(WLength("100%"));
    setContainerStyle(ContainerStyleType::ROW);
    addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    decorationStyle().setCursor(Cursor::PointingHand);
    setMargin(10,Side::Top);

    setHeight(50);
    setOverflow(Overflow::Hidden);

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


    clicked().connect([=](){
        _ClickBilgiEdinme.emit(item->oid());
    });

}

Signal<bsoncxx::oid> &BilgiEdinmeListWidget::ClickBilgiEdinme()
{
    return _ClickBilgiEdinme;
}


BilgiEdinmeWidget::BilgiEdinmeWidget(mongocxx::database *_db, const bsoncxx::oid &_oid, const UserClass &user)
    :DBClass (_db),UserClass (user)
{


    auto val = BilgiEdinmeItem::LoadBilgiEdinmeItem(this->db(),_oid.to_string());

    if( val )
    {
        mItem = (val.value());
        addWidget(cpp14::make_unique<WText>(mItem->Konu()));
    }else {
        std::cout << "ERROR: " << std::endl;
    }
}
