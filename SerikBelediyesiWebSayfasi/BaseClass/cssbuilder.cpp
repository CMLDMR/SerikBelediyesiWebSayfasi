
#include "cssbuilder.h"


CSS::CSSItem::CSSItem(const std::string &_className)
    :mClassName(_className)
{

}

CSS::CSSItem &CSS::CSSItem::append(const std::string &property, const std::string &value)
{

    mPropertyList.push_back(std::make_tuple(property,value));

    return *this;
}

std::string CSS::CSSItem::toString() const
{


    std::string mValue;
    mValue += "."+mClassName+"{\n";

    for( const auto &[property,value] : mPropertyList ){
        mValue += property + ":"+value+";\n";
    }



    mValue += "}";
    return mValue;
}


namespace CSS {
std::string CSSItem::className() const
{
    return mClassName;
}

CSSBuilder::CSSBuilder()
{

}

std::list<CSSItem> CSSBuilder::list() const
{
    return mList;
}

}
