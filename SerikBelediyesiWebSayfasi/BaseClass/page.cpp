#include "page.h"

namespace Comman {

Page::Page(const std::string &pageName)
{

    setPadding(90,Side::Top);

    auto container = addNew<WContainerWidget>();
    container->addStyleClass(Bootstrap::Grid::col_full_12);
    container->setContentAlignment(AlignmentFlag::Center);

    auto titleContainer = container->addNew<WContainerWidget>();
    titleContainer->setContentAlignment(AlignmentFlag::Center);
    titleContainer->setPadding(15,Side::Top);
    titleContainer->addNew<WText>("<h3>"+pageName+"</h3>");
    titleContainer->setHeight(90);
    titleContainer->setAttributeValue(Style::style,Style::background::url("img/baskanbackGround.jpg")
                                                        +Style::background::size::cover
                                                        +Style::background::repeat::norepeat
                                                        +Style::color::color(Style::color::White::Snow));
    titleContainer->setMaximumSize(1280,WLength::Auto);


    auto __content = container->addNew<WContainerWidget>();
    __content->addStyleClass(Bootstrap::Grid::container_fluid);
    __content->setMaximumSize(1280,WLength::Auto);

    __content->setContentAlignment(AlignmentFlag::Center);

    mContent = __content->addNew<ContainerWidget>();
    mContent->addStyleClass(Bootstrap::Grid::col_full_12);
    mContent->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::White));

    mContent->setMargin(25,Side::Bottom);
}

WContainerWidget *Page::header()
{
    return mContent->Header();
}

WContainerWidget *Page::content()
{
    return mContent->Content();
}

WContainerWidget *Page::footer()
{
    return mContent->Footer();
}

} // namespace Comman
