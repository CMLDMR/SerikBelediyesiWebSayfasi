#include "tcitemwidget.h"



TCItemWidget::TCItemWidget(mongocxx::database *_db, bsoncxx::document::value &userValue)
    :SerikBLDCore::DB (_db),UserClass (userValue),mTCItem(nullptr),mNewTCItem(true)
{
    addWidget(cpp14::make_unique<WText>("TC Widget For New"));
}

TCItemWidget::TCItemWidget(mongocxx::database *_db, const bsoncxx::document::value &userValue, TC::TCItem *_mTCItem)
    :SerikBLDCore::DB (_db),UserClass (userValue),mTCItem(_mTCItem), mNewTCItem(false)
{
    addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    setWidth(WLength("100%"));
    Header()->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    Header()->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);
    Header()->setWidth(WLength("100%"));

    Content()->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    Content()->addStyleClass(Bootstrap::ContextualBackGround::bg_info);
    Content()->setWidth(WLength("100%"));


    Header()->addWidget(cpp14::make_unique<WText>("<h4><b>Vatandaş Bilgileri</b></h4>"));
    Header()->addWidget(cpp14::make_unique<WBreak>());

    Content()->addWidget(cpp14::make_unique<WText>(mTCItem->Element(TC::KEY::adsoyad)->get_string().value.data()));
    Content()->addWidget(cpp14::make_unique<WBreak>());

    Content()->addWidget(cpp14::make_unique<WText>(mTCItem->Element(TC::KEY::tcno)->get_string().value.data()));
    Content()->addWidget(cpp14::make_unique<WBreak>());

    Content()->addWidget(cpp14::make_unique<WText>(mTCItem->Element(TC::KEY::cepTelefonu)->get_string().value.data()));
    Content()->addWidget(cpp14::make_unique<WBreak>());

    Content()->addWidget(cpp14::make_unique<WText>(mTCItem->Element(TC::KEY::mahalle)->get_string().value.data()));
    Content()->addWidget(cpp14::make_unique<WBreak>());

    Content()->addWidget(cpp14::make_unique<WText>(mTCItem->Element(TC::KEY::tamAdres)->get_string().value.data()));
    Content()->addWidget(cpp14::make_unique<WBreak>());

    setMargin(15,Side::Bottom);
}

bsoncxx::oid TCItemWidget::oid()
{
    return mTCItem->oid();
}
