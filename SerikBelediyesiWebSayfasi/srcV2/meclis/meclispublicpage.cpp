#include "meclispublicpage.h"



v2::MeclisPublicPage::MeclisPublicPage(SerikBLDCore::DB *db)
    :ContainerWidget(),SerikBLDCore::Meclis::MeclisManager (db),SerikBLDCore::Meclis::UyeManager(db)
{
    tcManager = new SerikBLDCore::TCManager(db);

    this->setMargin (90,Side::Top);
    setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Snow));


    this->Header()->setPadding( 20 , Side::Top );
    this->Header()->setMargin( 30 , Side::Bottom);



    auto meclistoplantiListContainer = this->Header()->addNew<WContainerWidget>();
    meclistoplantiListContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                               Bootstrap::Grid::Medium::col_md_6+
                                               Bootstrap::Grid::Small::col_sm_6+
                                               Bootstrap::Grid::ExtraSmall::col_xs_6);

    meclistoplantiListContainer->addStyleClass(CSSStyle::Button::grayButton);
    meclistoplantiListContainer->addNew<WText>("<h4>Meclis Toplantıları</h4>");
    meclistoplantiListContainer->setPadding(3,Side::Top|Side::Bottom);
    meclistoplantiListContainer->decorationStyle().setCursor(Cursor::PointingHand);



    auto meclisUyesiListesiContainer = this->Header()->addNew<WContainerWidget>();
    meclisUyesiListesiContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                               Bootstrap::Grid::Medium::col_md_6+
                                               Bootstrap::Grid::Small::col_sm_6+
                                               Bootstrap::Grid::ExtraSmall::col_xs_6);

    meclisUyesiListesiContainer->addStyleClass(CSSStyle::Button::grayButton);
    meclisUyesiListesiContainer->addNew<WText>("<h4>Meclis Üyeleri</h4>");
    meclisUyesiListesiContainer->setPadding(3,Side::Top|Side::Bottom);
    meclisUyesiListesiContainer->decorationStyle().setCursor(Cursor::PointingHand);
    meclisUyesiListesiContainer->clicked().connect([=]{

        SerikBLDCore::Meclis::MeclisUyesi sort;
        sort.append(SerikBLDCore::Meclis::UyeKey::partiAdi,1);

        SerikBLDCore::FindOptions findOptions;
        findOptions.setSort (sort);
        findOptions.setLimit (100);

        SerikBLDCore::Meclis::UyeManager::UpdateList (SerikBLDCore::Meclis::MeclisUyesi(),findOptions);

    });



//    auto komisyonListesiContainer = this->Header()->addNew<WContainerWidget>();
//    komisyonListesiContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+
//                                               Bootstrap::Grid::Medium::col_md_4+
//                                               Bootstrap::Grid::Small::col_sm_4+
//                                               Bootstrap::Grid::ExtraSmall::col_xs_12);

//    komisyonListesiContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail+CSSStyle::Button::grayButton);
//    komisyonListesiContainer->addNew<WText>("<h4>Komisyonlar</h4>");
//    komisyonListesiContainer->setPadding(5,Side::Top|Side::Bottom);
//    komisyonListesiContainer->decorationStyle().setCursor(Cursor::PointingHand);



    meclistoplantiListContainer->clicked().connect(this,&MeclisPublicPage::loadMeclisToplantilari);




    Content ()->setMargin (50,Side::Bottom);
}

void v2::MeclisPublicPage::onList(const QVector<SerikBLDCore::Meclis::MeclisItem> *mlist)
{
    Content ()->clear ();
    Content ()->setContentAlignment (AlignmentFlag::Center);

    auto contentContainer = Content()->addNew<WContainerWidget>();
    contentContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    contentContainer->addWidget(cpp14::make_unique<WText>("<h4>Meclis Toplantıları</h4>"));


    std::int32_t year = mlist->first().yil();
    int colorBeginRange = 210 , colorEndRange = 255;
    std::string color = Style::background::color::rgb (this->getRandom (colorBeginRange,colorEndRange),this->getRandom (colorBeginRange,colorEndRange),this->getRandom (colorBeginRange,colorEndRange));

    WContainerWidget* yilContainerWidget = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    yilContainerWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
    yilContainerWidget->setMargin(10,Side::Top);
    yilContainerWidget->addWidget(cpp14::make_unique<WText>("<h4>"+std::to_string(year)+"</h4>"));
    yilContainerWidget->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
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

            yilContainerWidget->addWidget(cpp14::make_unique<WText>("<h4>"+std::to_string(year)+"</h4>"));
            yilContainerWidget->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

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

void v2::MeclisPublicPage::onList(const QVector<SerikBLDCore::Meclis::MeclisUyesi> *mlist)
{
    Content()->clear ();
    Content ()->setMargin (25,Side::Top);
    Content()->setContentAlignment(AlignmentFlag::Center);
    auto mContent = Content()->addNew<WContainerWidget>();
    mContent->addStyleClass(Bootstrap::Grid::row);
    mContent->setMaximumSize(1024,WLength::Auto);

//    {
//        auto container = Content ()->addWidget (cpp14::make_unique<ContainerWidget>("Meclis Üyeleri"));
//        container->addStyleClass (Bootstrap::Grid::col_full_12);
//    }


    std::string currentParti = "";

    std::string currentBackColor = Style::background::color::rgb (this->getRandom (245,255),this->getRandom (200,255),this->getRandom (220,255));

    for( const auto &item : *mlist )
    {


        if( currentParti != item.partiAdi ().toStdString () ){
            currentParti = item.partiAdi ().toStdString ();
            currentBackColor = Style::background::color::rgb (this->getRandom (245,255),this->getRandom (200,255),this->getRandom (220,255));
            auto containerBreak = mContent->addWidget (cpp14::make_unique<WContainerWidget>());
            containerBreak->addStyleClass (Bootstrap::Grid::col_full_12);
            containerBreak->addWidget (cpp14::make_unique<WText>("<h4>"+item.partiAdi ().toStdString ()+"</h4>"));
            containerBreak->setContentAlignment (AlignmentFlag::Center);
            containerBreak->setAttributeValue (Style::style,currentBackColor);
            containerBreak->setMargin(25,Side::Top);
        }



        auto tcItem = tcManager->Load_byOID (item.tcOid ().toStdString ());

        auto container = mContent->addWidget (cpp14::make_unique<WContainerWidget>());
//        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setHeight(160);


        container->setAttributeValue (Style::style,currentBackColor);


        auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);


        {
            auto mPhotoText = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            auto filePath = this->tcManager->downloadFileWeb (tcItem.value ()->FotoOid ());
            mPhotoText->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                       Bootstrap::Grid::Medium::col_md_4+
                                       Bootstrap::Grid::Small::col_sm_4+
                                       Bootstrap::Grid::ExtraSmall::col_xs_12);
            mPhotoText->setAttributeValue (Style::style,Style::background::url (filePath)+
                                           Style::background::size::cover+
                                           Style::background::position::center_center+
                                           Style::background::repeat::norepeat);
            mPhotoText->setHeight (140);

            mPhotoText->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        }

        auto infoContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        infoContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+
                                      Bootstrap::Grid::Medium::col_md_8+
                                      Bootstrap::Grid::Small::col_sm_8+
                                      Bootstrap::Grid::ExtraSmall::col_xs_12);
        infoContainer->setPadding(0,Side::Top|Side::Bottom);

        auto vLayout = infoContainer->setLayout (cpp14::make_unique<WVBoxLayout>());

        {
            auto mAdSoyadText = vLayout->addWidget (cpp14::make_unique<WText>(tcItem.value ()->AdSoyad ().toStdString ()),1,AlignmentFlag::Center);
            mAdSoyadText->setAttributeValue (Style::style,Style::font::size::s11px);
            mAdSoyadText->setMargin (0);

        }

        {
            auto mPartiText = vLayout->addWidget (cpp14::make_unique<WText>("<b>"+item.partiAdi ().toStdString ()+"</b>"),1,AlignmentFlag::Center);
            mPartiText->setAttributeValue (Style::style,Style::font::size::s10px);
        }

        {
            auto mDonemText = vLayout->addWidget (cpp14::make_unique<WText>("<i><b>"+item.donemAdi ().toStdString ()+"</b></i>"),1,AlignmentFlag::Center);
            mDonemText->setAttributeValue (Style::style,Style::font::size::s10px);
        }

        for( const auto &__item : item.komisyonUyelikleri () )
        {
            auto mKomisyonText = vLayout->addWidget (cpp14::make_unique<WText>("● <i>"+__item.toStdString ()+"</i>"),1,AlignmentFlag::Center);
            mKomisyonText->setAttributeValue (Style::style,Style::font::size::s10px);
            mKomisyonText->setMargin (0);
        }
        container->setMargin (5,Side::Top);
    }
}

void v2::MeclisPublicPage::loadMeclisToplantilari()
{

    SerikBLDCore::FindOptions findOptions;

    findOptions.setSort (SerikBLDCore::Meclis::MeclisItem().append(SerikBLDCore::Meclis::Key::julianDate,-1));
    findOptions.setLimit(10000);

    SerikBLDCore::Meclis::MeclisManager::UpdateList (SerikBLDCore::Meclis::MeclisItem(),findOptions);


}
