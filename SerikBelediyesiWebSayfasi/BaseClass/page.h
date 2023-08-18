#ifndef COMMAN_PAGE_H
#define COMMAN_PAGE_H

#include "containerwiget.h"


namespace Comman {


///
/// \brief The Page class public sayfalar için
///
class Page : public WContainerWidget
{
public:
    Page(const std::string &pageName = "");


    WContainerWidget* header();
    WContainerWidget* content();
    WContainerWidget* footer();

private:

    ContainerWidget* mContent;
};

} // namespace Comman

#endif // COMMAN_PAGE_H
