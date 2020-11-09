#include "meclispublicpage.h"



v2::MeclisPublicPage::MeclisPublicPage(SerikBLDCore::DB *db)
    :ContainerWidget("Meclis v2"),SerikBLDCore::Meclis::MeclisManager (db)
{
    this->setMargin (90,Side::Top);
    setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Snow));

    wApp->setInternalPath (WString(MeclisLinks::mainPage).toUTF8 ());


    SerikBLDCore::FindOptions findOptions;

    findOptions.setSort (SerikBLDCore::Meclis::MeclisItem().append(SerikBLDCore::Meclis::Key::julianDate,-1));


    this->UpdateList (SerikBLDCore::Meclis::MeclisItem(),findOptions);

    Content ()->setMargin (50,Side::Bottom);
}

void v2::MeclisPublicPage::onList(const QVector<SerikBLDCore::Meclis::MeclisItem> *mlist)
{

    Content ()->clear ();
    Content ()->setContentAlignment (AlignmentFlag::Center);

    std::int32_t year = 0;
    int colorBeginRange = 175 , colorEndRange = 225;
    std::string color = Style::background::color::rgb (this->getRandom (colorBeginRange,colorEndRange),this->getRandom (colorBeginRange,colorEndRange),this->getRandom (colorBeginRange,colorEndRange));
    for( auto item : *mlist )
    {
        auto _container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::col_full_12);
        _container->setContentAlignment (AlignmentFlag::Center);
        auto container = _container->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setMaximumSize (1024,WLength::Auto);
        if( year != item.yil () )
        {
            year = item.yil ();
            color = Style::background::color::rgb (this->getRandom (colorBeginRange,colorEndRange),this->getRandom (colorBeginRange,colorEndRange),this->getRandom (colorBeginRange,colorEndRange));
            container->setMargin (15,Side::Top);
        }
        container->setAttributeValue (Style::style,color);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setWidth (WLength("100%"));



        container->setContentAlignment (AlignmentFlag::Center);
        WLink link = WLink(LinkType::Url,"?type=gundem&_id="+item.oid ().value ().to_string ());
        link.setTarget(Wt::LinkTarget::NewWindow);

        std::unique_ptr<Wt::WAnchor> anchor =
                Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                std::to_string (item.yil ())+" "+item.ay ().toStdString ());
        container->addWidget (std::move(anchor));

        container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);

    }


}
