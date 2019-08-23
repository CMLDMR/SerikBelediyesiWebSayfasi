#include "tcitemwidget.h"



TCItemWidget::TCItemWidget(mongocxx::database *_db, bsoncxx::document::value &userValue)
    :DBClass (_db),UserClass (userValue),mNewTCItem(true)
{
    addWidget(cpp14::make_unique<WText>("TC Widget For New"));
}

TCItemWidget::TCItemWidget(mongocxx::database *_db, bsoncxx::document::value &userValue, TC::TCItem *_mTCItem)
    :DBClass (_db),UserClass (userValue),mTCItem(_mTCItem), mNewTCItem(false)
{
    addWidget(cpp14::make_unique<WText>("TC Widget For Exist"));
    addWidget(cpp14::make_unique<WBreak>());
    addWidget(cpp14::make_unique<WText>(mTCItem->Element(TC::KEY::adsoyad)->get_utf8().value.to_string()));

}
