#include "databasewidget.h"



void DataBaseWidget::showMessage(std::string title, std::string msg, std::string btnText)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, StandardButton::Ok));
    if( btnText != "OK" )
    {
        auto btn = messageBox->button(StandardButton::Ok);
        btn->setText(btnText);
    }


        messageBox->buttonClicked().connect([=] {
            this->removeChild(messageBox);
        });

        messageBox->show();
}

void DataBaseWidget::showMessage(std::string title, std::string msg, bsoncxx::exception &e, Icon icon)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 WString("{1} : {2}").arg(msg.c_str()).arg(e.what()).toUTF8(),
                 icon, Wt::StandardButton::Ok));

        messageBox->buttonClicked().connect([=] {
            this->removeChild(messageBox);
        });

        messageBox->show();
}

void DataBaseWidget::showMessage(std::string title, std::string msg, mongocxx::exception &e, Icon icon)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 WString("{1} : {2}").arg(msg.c_str()).arg(e.what()).toUTF8(),
                 icon, Wt::StandardButton::Ok));

        messageBox->buttonClicked().connect([=] {
            this->removeChild(messageBox);
        });

        messageBox->show();
}


int64_t DataBaseWidget::count(std::string collection, bsoncxx::document::view &filter)
{
    std::int64_t count = 0;
    try {
        count = this->db->collection(collection).count(filter);
    } catch (mongocxx::exception &e) {

    }
    return count;

}

int64_t DataBaseWidget::count(std::string collection, const bsoncxx::builder::basic::document &filter)
{
    std::int64_t count = 0;
    try {
        count = this->db->collection(collection).count(filter.view());
    } catch (mongocxx::exception &e) {

    }
    return count;
}

int64_t DataBaseWidget::count(std::string collection, bsoncxx::document::value val)
{
    std::int64_t count = 0;
    try {
        count = this->db->collection(collection).count(val.view());
    } catch (mongocxx::exception &e) {

    }
    return count;
}

