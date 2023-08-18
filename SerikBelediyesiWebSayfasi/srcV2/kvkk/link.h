#ifndef V2_KVKK_LINK_H
#define V2_KVKK_LINK_H

#include <Wt/WContainerWidget.h>
#include "SerikBelediyesiWebSayfasi/BaseClass/utility.h"



namespace v2 {
namespace KVKK {

class Link : public Wt::WContainerWidget, public Utility::Utility
{
public:
    Link(const std::string &title, const std::string &url );

private:

};
} // namespace KVKK
} // namespace v2

#endif // V2_KVKK_LINK_H
