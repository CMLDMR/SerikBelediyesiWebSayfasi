#ifndef WIDGET_STYLESHEET_PROPERTIES_H
#define WIDGET_STYLESHEET_PROPERTIES_H

#include <string>


namespace Widget {
namespace StyleSheet {

namespace Properties {

#define Key inline const std::string




namespace Color{
Key color{"color"};
}

namespace BackGround {
Key color{"background-color"};
Key image{"background-image"};
Key repeat{"background-repeat"};
Key position{"background-position"};
Key attachment{"background-attachment"};
}


}


} // namespace StyleSheet
} // namespace Widget

#endif // WIDGET_STYLESHEET_PROPERTIES_H
