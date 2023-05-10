#ifndef WIDGET_CSS_BUTTON_H
#define WIDGET_CSS_BUTTON_H

#include <string>


#include <Wt/WPushButton.h>
#include <Wt/WContainerWidget.h>


namespace Widget {


namespace CSS {

namespace Button
{
inline constexpr std::string abstractBtn{"abstractBtn"};
inline constexpr std::string defult{"defaultBtn"};
inline constexpr std::string primaryBtn{"primaryBtn"};
inline constexpr std::string successBtn{"successBtn"};
inline constexpr std::string warningBtn{"warningBtn"};

}
} // namespace CSS


namespace Button{
class Abstract : public Wt::WContainerWidget
{
public:
    Abstract(const std::string &name);
};

class Default : public Abstract
{
public:
    Default(const std::string &name);
};

class Primary : public Abstract
{
public:
    Primary(const std::string &name);
};

class Success : public Abstract
{
public:
    Success(const std::string &name);
};

class Warning : public Abstract
{
public:
    Warning(const std::string &name);
};







}












} // namespace Widget

#endif // WIDGET_CSS_BUTTON_H
