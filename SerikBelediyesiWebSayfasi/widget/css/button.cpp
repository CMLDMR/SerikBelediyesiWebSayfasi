#include "button.h"
#include <Wt/WText.h>


namespace Widget {

Button::Abstract::Abstract(const std::string &name)
{
    addStyleClass(CSS::Button::abstractBtn);
    addWidget(Wt::cpp14::make_unique<Wt::WText>(name));
    this->setContentAlignment(Wt::AlignmentFlag::Center);
}

Button::Default::Default(const std::string &name)
    :Abstract(name)
{
    addStyleClass(CSS::Button::defult);
}

Button::Primary::Primary(const std::string &name)
    :Abstract(name)
{
    addStyleClass(CSS::Button::primaryBtn);
}

Button::Success::Success(const std::string &name)
    :Abstract(name)
{
    addStyleClass(CSS::Button::successBtn);
}

Button::Warning::Warning(const std::string &name)
    :Abstract(name)
{
    addStyleClass(CSS::Button::warningBtn);
}




} // namespace Widget
