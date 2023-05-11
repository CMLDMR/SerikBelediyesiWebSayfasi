#ifndef WIDGET_STYLESHEET_H
#define WIDGET_STYLESHEET_H

#include <Wt/WCssStyleSheet.h>

#include <map>


namespace Widget {

namespace StyleSheet {


class StyleSheet
{
public:
    StyleSheet(const std::string &selectorName);



    void installStyle();

    std::string selector() const;



private:
    std::string mSelector;
    std::string mSelectorHover;
    std::string mSelectorActive;


    std::unique_ptr<Wt::WCssTextRule> mStyleSheet;
    std::unique_ptr<Wt::WCssTextRule> mStyleHoverSheet;
    std::unique_ptr<Wt::WCssTextRule> mStyleActiveSheet;

    std::map<std::string,std::string> mPropertyList;
    std::map<std::string,std::string> mHoverList;
    std::map<std::string,std::string> mActiveList;


    std::unique_ptr<Wt::WCssTextRule> getRule();
    std::unique_ptr<Wt::WCssTextRule> getHover();
    std::unique_ptr<Wt::WCssTextRule> getActive();


};

}




} // namespace Widget

#endif // WIDGET_STYLESHEET_H
