#include "tcitemwidget.h"



TCItemWidget::TCItemWidget(mongocxx::database *_db, bsoncxx::document::value &userValue)
    :DBClass (_db),UserClass (userValue)
{
    addWidget(cpp14::make_unique<WText>("TC Widget"));
}
