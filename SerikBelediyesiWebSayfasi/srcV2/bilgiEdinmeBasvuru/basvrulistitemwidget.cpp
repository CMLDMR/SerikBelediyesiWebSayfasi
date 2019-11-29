#include "basvrulistitemwidget.h"
#include "bilgiEdinme/bilgiedinmeitem.h"

BasvruListItemWidget::BasvruListItemWidget(SerikBLDCore::BilgiEdinmeItem *_item)
    :ContainerWidget (""),item(_item)
{
    addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    addWidget (cpp14::make_unique<WText>(item->birim ().toStdString ()));
    addWidget (cpp14::make_unique<WBreak>());
    addWidget (cpp14::make_unique<WText>(item->tarih ().toStdString ()));
    addWidget (cpp14::make_unique<WBreak>());
    addWidget (cpp14::make_unique<WText>(item->saat ().toStdString ()));
    addWidget (cpp14::make_unique<WBreak>());
    addWidget (cpp14::make_unique<WText>(item->konu ().toStdString ()));
    setMargin (5,Side::Bottom);


    if( this->item->cevaplandi () )
    {
        addStyleClass (Bootstrap::ContextualBackGround::bg_info);
    }else{
        addStyleClass (Bootstrap::ContextualBackGround::bg_warning);
    }


    decorationStyle ().setCursor (Cursor::PointingHand);
    clicked ().connect ([&](){
        this->_ClickItem.emit (item->oid ().value ().to_string ());
    });
}

Signal<std::string> &BasvruListItemWidget::ClickItem()
{
    return _ClickItem;
}
