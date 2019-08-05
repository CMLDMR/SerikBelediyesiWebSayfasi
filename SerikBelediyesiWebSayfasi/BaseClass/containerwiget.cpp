#include "containerwiget.h"

ContainerWiget::ContainerWiget()
{

}

void ContainerWiget::setContainerStyle(ContainerWiget::ContainerStyleType type)
{
    switch (type) {
    case ROW:
        this->addStyleClass(Bootstrap::Grid::row);
        break;

    case CONTAINERFLUID:
        this->addStyleClass(Bootstrap::Grid::container_fluid);
        break;
    default:
        break;
    }
}

void ContainerWiget::showMessage(std::string title, std::string msg, std::string btnText)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, StandardButton::Ok));
    if( btnText != "Tamam" )
    {
        auto btn = messageBox->button(StandardButton::Ok);
        btn->setText(btnText);
    }


        messageBox->buttonClicked().connect([=] {
            this->removeChild(messageBox);
        });

        messageBox->webWidget()->setZIndex(1000);
        messageBox->show();
}
