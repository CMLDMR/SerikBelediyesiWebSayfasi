#include "stylesheet.h"
#include <Wt/WApplication.h>

namespace Widget {

StyleSheet::StyleSheet::StyleSheet(const std::string &selectorName)
    :mSelector{selectorName},
    mSelectorHover{selectorName+":hover"},
    mSelectorActive{selectorName+":active"}
{
    mPropertyList.insert(std::make_pair("background-color","green"));
    mPropertyList.insert(std::make_pair("height","100px"));
    mPropertyList.insert(std::make_pair("width","100px"));
    mPropertyList.insert(std::make_pair("cursor","pointer"));
    mPropertyList.insert(std::make_pair("transition","background-color 250ms"));

    mHoverList.insert(std::make_pair("background-color","red"));
    mActiveList.insert(std::make_pair("background-color","blue"));

}

std::unique_ptr<Wt::WCssTextRule> StyleSheet::StyleSheet::getRule()
{
    std::string str;
    for( const auto &[key,value] : mPropertyList ){
        str += key + ":" + value + ";";
    }

    mStyleSheet = std::make_unique<Wt::WCssTextRule>("."+mSelector,str);
    return std::move(mStyleSheet);
}

std::unique_ptr<Wt::WCssTextRule> StyleSheet::StyleSheet::getHover()
{
    std::string str;
    for( const auto &[key,value] : mHoverList ){
        str += key + ":" + value + ";";
    }

    mStyleHoverSheet = std::make_unique<Wt::WCssTextRule>("."+mSelectorHover,str);
    return std::move(mStyleHoverSheet);
}

std::unique_ptr<Wt::WCssTextRule> StyleSheet::StyleSheet::getActive()
{
    std::string str;
    for( const auto &[key,value] : mActiveList ){
        str += key + ":" + value + ";";
    }

    mStyleActiveSheet = std::make_unique<Wt::WCssTextRule>("."+mSelectorActive,str);
    return std::move(mStyleActiveSheet);
}

void StyleSheet::StyleSheet::installStyle()
{
    Wt::WApplication::instance()->styleSheet().addRule(std::move(getHover()));
    Wt::WApplication::instance()->styleSheet().addRule(std::move(getRule()));
    Wt::WApplication::instance()->styleSheet().addRule(std::move(getActive()));
}


std::string StyleSheet::StyleSheet::selector() const
{
    return mSelector;
}

} // namespace Widget
