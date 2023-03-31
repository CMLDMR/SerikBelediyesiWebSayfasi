
#ifndef CSSBUILDER_H
#define CSSBUILDER_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <tuple>
#include <string>
#include <list>

namespace CSS {

namespace Properties{
namespace A {
inline const char* accent_color{"accent-color"};
inline const char* align_content{"align-content"};
inline const char* align_items{"align-items"};
inline const char* align_self{"align-self"};
inline const char* all{"all"};
inline const char* animation{"animation"};
inline const char* animation_delay{"animation-delay"};
inline const char* animation_direction{"animation-direction"};
inline const char* animation_duration{"animation-duration"};
inline const char* animation_fill_mode{"animation-fill-mode"};
inline const char* animation_iteration_count{"animation-iteration-count"};
inline const char* animation_name{"animation-name"};
inline const char* animation_play_state{"animation-play-state"};
inline const char* animation_timing_function{"animation-timing-function"};
inline const char* aspect_ratio{"aspect-ratio"};
}

namespace B {
inline const char* background_color{"background-color"};
inline const char* background_position{"background-position"};
inline const char* background_origin{"background-origin"};
inline const char* background_image{"background-image"};
}
}

class CSSBuilder;


class CSSItem
{
public:
    CSSItem& append( const std::string &property, const std::string &value );
    std::string toString() const;
    std::string className() const;

private:
    explicit CSSItem( const std::string &_className );
    friend class CSSBuilder;

    std::string mClassName;
    std::list<std::tuple<std::string,std::string>> mPropertyList;
};

class CSSBuilder
{
public:
    CSSBuilder();

    CSS::CSSItem &createItem( const std::string &className ){
        CSSItem item(className);
        mList.push_back(item);
        return mList.back();
    }

    std::list<CSSItem> list() const;

private:
    std::list<CSSItem> mList;
};


}



#endif // CSSBUILDER_H
