#include "basvrulistitemwidget.h"
#include "bilgiEdinme/bilgiedinmeitem.h"

BasvruListItemWidget::BasvruListItemWidget(SerikBLDCore::BilgiEdinme::BilgiEdinmeItem *_item)
    :ContainerWidget (""),item(_item)
{
    addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    addWidget (cpp14::make_unique<WText>(item->konu ().toStdString ()));
    addWidget (cpp14::make_unique<WBreak>());
    addWidget (cpp14::make_unique<WText>(item->saat ().toStdString ()));
}
