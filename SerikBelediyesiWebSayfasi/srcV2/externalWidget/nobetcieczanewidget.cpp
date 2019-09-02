#include "nobetcieczanewidget.h"

NobetciEczaneWidget::NobetciEczaneWidget()
{
    this->Header()->setMargin(100,Side::Top);
    this->Header()->addWidget(cpp14::make_unique<WText>("<h3>Nöbetçi Eczaneler</h3>"));
    this->Header()->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    this->Header()->setWidth(WLength("100%"));

    this->Content()->setContentAlignment(AlignmentFlag::Center);
    this->Content()->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    this->Content()->setWidth(WLength("100%"));
    auto container = this->Content()->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setWidth(400);
    container->setHeight(400);
    container->setOverflow(Overflow::Hidden);

    container->addWidget(cpp14::make_unique<WText>("<iframe src=\"https://www.eczaneler.gen.tr/iframe.php?lokasyon=1616\" style=\"border:0px #ffffff none;\" name=\"myiFrame\" scrolling=\"no\""
                                                         "frameborder=\"1\" marginheight=\"0px\" marginwidth=\"0px\" height=\"400px\" width=\"100%\" allowfullscreen></iframe>",TextFormat::UnsafeXHTML));


    auto link = cpp14::make_unique<WLink>("https://www.eczaneler.gen.tr/nobetci-antalya-serik");
    link.get()->setTarget(LinkTarget::NewWindow);


    this->Footer()->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    this->Footer()->setWidth(WLength("100%"));
    this->Footer()->addWidget(cpp14::make_unique<WAnchor>(*link.get(),"Haritalı Adres"));


}
