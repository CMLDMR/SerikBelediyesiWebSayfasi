#include "link.h"

#include <Wt/WAnchor.h>
#include <Wt/WLink.h>

#include "inlinestyle.h"

using namespace Wt;

namespace v2 {
namespace KVKK {

Link::Link(const std::string &title, const std::string &url)
{
    setPadding(5);
    addWidget(std::make_unique<Wt::WAnchor>(url, title));
    setAttributeValue(Style::style,Style::background::color::rgb( this->getRandom(250,255),
                                                                  this->getRandom(250,255),
                                                                  this->getRandom(250,255)));
}



} // namespace KVKK
} // namespace v2
