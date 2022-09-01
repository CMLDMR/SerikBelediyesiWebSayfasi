#include "meclispublicpage.h"



v2::MeclisPublicPage::MeclisPublicPage(SerikBLDCore::DB *db)
    :ContainerWidget("<h4>Meclis Bilgileri</h4>"),SerikBLDCore::Meclis::MeclisManager (db)
{
    this->setMargin (90,Side::Top);
    setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Snow));

    wApp->setInternalPath (WString(MeclisLinks::mainPage).toUTF8 ());


    SerikBLDCore::FindOptions findOptions;

    findOptions.setSort (SerikBLDCore::Meclis::MeclisItem().append(SerikBLDCore::Meclis::Key::julianDate,-1));
    findOptions.setLimit(10000);

    this->titleBar()->clicked().connect([=](){
        SerikBLDCore::FindOptions _findOptions;
        _findOptions.setSort (SerikBLDCore::Meclis::MeclisItem().append(SerikBLDCore::Meclis::Key::julianDate,-1));
        _findOptions.setLimit(10000);
        this->UpdateList (SerikBLDCore::Meclis::MeclisItem(),_findOptions);
    });

    this->titleBar()->decorationStyle().setCursor(Cursor::PointingHand);

    this->UpdateList (SerikBLDCore::Meclis::MeclisItem(),findOptions);

    Content ()->setMargin (50,Side::Bottom);
}

void v2::MeclisPublicPage::onList(const QVector<SerikBLDCore::Meclis::MeclisItem> *mlist)
{
    Content ()->clear ();
    Content ()->setContentAlignment (AlignmentFlag::Center);

    auto contentContainer = Content()->addNew<WContainerWidget>();
    contentContainer->addStyleClass(Bootstrap::Grid::col_full_12);



    std::int32_t year = mlist->first().yil();
    int colorBeginRange = 175 , colorEndRange = 225;
    std::string color = Style::background::color::rgb (this->getRandom (colorBeginRange,colorEndRange),this->getRandom (colorBeginRange,colorEndRange),this->getRandom (colorBeginRange,colorEndRange));

    WContainerWidget* yilContainerWidget = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    yilContainerWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
    yilContainerWidget->setMargin(10,Side::Top);
//    yilContainerWidget->addWidget(cpp14::make_unique<WText>("<h4>"+std::to_string(year)+"</h4>"));
//    yilContainerWidget->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    int i = 1;
    for( const auto &item : *mlist )
    {

        if( year != item.yil () )
        {
            year = item.yil ();
            if( i%4 == 0 ){
                contentContainer = Content()->addNew<WContainerWidget>();
                contentContainer->addStyleClass(Bootstrap::Grid::col_full_12);
            }
            i++;


            color = Style::background::color::rgb (this->getRandom (colorBeginRange,colorEndRange),this->getRandom (colorBeginRange,colorEndRange),this->getRandom (colorBeginRange,colorEndRange));
            yilContainerWidget = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            yilContainerWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
            yilContainerWidget->setMargin(10,Side::Top);

//            yilContainerWidget->addWidget(cpp14::make_unique<WText>("<h4>"+std::to_string(year)+"</h4>"));
//            yilContainerWidget->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

        }




        auto _container = yilContainerWidget->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->setContentAlignment (AlignmentFlag::Center);
        auto container = _container->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setMaximumSize (1024,WLength::Auto);

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
