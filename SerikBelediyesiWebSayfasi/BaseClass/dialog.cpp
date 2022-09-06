#include "dialog.h"

Dialog::Dialog(mongocxx::database *_db, const bsoncxx::document::value &userValue, const std::string &title)
    :SerikBLDCore::DB (_db),UserClass (userValue)
{
    this->titleBar()->addWidget(cpp14::make_unique<WText>(title));
    this->titleBar()->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);




    auto kptBtn = this->footer()->addWidget(cpp14::make_unique<WPushButton>("Kapat"));
    kptBtn->addStyleClass(Bootstrap::Button::Primary);
    kptBtn->clicked().connect([=](){
        wApp->instance()->root()->removeChild(this);
    });


    this->footer()->addStyleClass(Bootstrap::ContextualBackGround::bg_info);

}
