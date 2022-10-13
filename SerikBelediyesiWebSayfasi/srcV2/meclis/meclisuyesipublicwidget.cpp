#include "meclisuyesipublicwidget.h"



v2::MeclisUyesiPublicWidget::MeclisUyesiPublicWidget(SerikBLDCore::DB *_mDB)
    :SerikBLDCore::Meclis::UyeManager (_mDB),
      mTCManager(new SerikBLDCore::TCManagerV2(_mDB))
{
    this->setMargin(90,Side::Top);

    this->setLimit(100);

    SerikBLDCore::Meclis::MeclisUyesi sort;
    sort.append(SerikBLDCore::Meclis::UyeKey::partiAdi,1);

    SerikBLDCore::FindOptions findOptions;
    findOptions.setSort (sort);
    findOptions.setLimit (100);

    this->UpdateList(SerikBLDCore::Meclis::MeclisUyesi(),findOptions);

//    this->setAttributeValue (Style::style,Style::background::color::rgba (this->getRandom (240,255),this->getRandom (240,255),this->getRandom (240,255),0.55));
//    Header ()->addWidget (cpp14::make_unique<WText>("<h3>Meclis Üyelerimiz</h3>"))->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black));


//    auto list = this->List (SerikBLDCore::Meclis::MeclisUyesi().setDonemAdi ("2019 - 2024"),SerikBLDCore::FindOptions().setLimit (100));



//    QStringList partiList;

//    for( const auto &item : list ){
//        partiList.append (item.partiAdi ());
//    }

//    partiList.removeDuplicates ();

//    for( const auto &partiAdi : partiList ){

//        auto _container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
//        _container->addStyleClass (Bootstrap::Grid::col_full_12);
//        auto rContainer = _container->addWidget (cpp14::make_unique<WContainerWidget>());
//        rContainer->addStyleClass (Bootstrap::Grid::row);
////        if( partiAdi == "Adalet ve Kalkınma Partisi");
//        rContainer->setAttributeValue (Style::style,Style::background::color::rgba (this->getRandom (0,255),this->getRandom (0,255),this->getRandom (0,255),0.75));
//        rContainer->addStyleClass (CSSStyle::Gradient::blueGradient+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
//        rContainer->setMargin (55,Side::Bottom);


//        {
//            auto partiContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
//            partiContainer->addStyleClass (Bootstrap::Grid::col_full_12);
//            auto text = partiContainer->addWidget (cpp14::make_unique<WText>("<h4>"+partiAdi.toStdString ()+"</h4>"));
//        }

//        for( const auto &item : list ){
//            if( item.partiAdi () == partiAdi ){
//                auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
//                container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
//                                          Bootstrap::Grid::Medium::col_md_2+
//                                          Bootstrap::Grid::Small::col_sm_3+
//                                          Bootstrap::Grid::ExtraSmall::col_xs_4);

////                container->setHeight (100);

//                auto fotoContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());

//                auto tcItem = mTCManager->Load_byOID (item.tcOid ().toStdString ());

//                auto fotoPath = this->getDB ()->downloadFileWeb (tcItem.value ()->FotoOid ());

//                fotoContainer->setAttributeValue (Style::style,Style::background::url (fotoPath)+
//                                                  Style::background::size::cover+
//                                                  Style::background::position::center_center+
//                                                  Style::background::repeat::norepeat);
//                container->addWidget (cpp14::make_unique<WBreak>());
//                fotoContainer->setWidth (160-16*3);
//                fotoContainer->setHeight (190-19*3);

//                fotoContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

//                partiList.append (item.partiAdi ());

//                container->addWidget (cpp14::make_unique<WText>(tcItem.value ()->AdSoyad ().toStdString ()));
//            }
//        }
////        {
////            auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
////            container->addStyleClass (Bootstrap::Grid::col_full_12);
////            container->setHeight (20);
////        }
//    }






}

void v2::MeclisUyesiPublicWidget::onList(const QVector<SerikBLDCore::Meclis::MeclisUyesi> *mlist)
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

    std::string currentBackColor = Style::background::color::rgba (this->getRandom (145,205),this->getRandom (220,255),this->getRandom (245,255));

    for( const auto &item : *mlist )
    {


        if( currentParti != item.partiAdi ().toStdString () ){
            currentParti = item.partiAdi ().toStdString ();
            currentBackColor = Style::background::color::rgba (this->getRandom (145,205),this->getRandom (220,255),this->getRandom (245,255));
            auto containerBreak = mContent->addWidget (cpp14::make_unique<WContainerWidget>());
            containerBreak->addStyleClass (Bootstrap::Grid::col_full_12);
            containerBreak->addWidget (cpp14::make_unique<WText>("<h4>"+item.partiAdi ().toStdString ()+"</h4>"));
            containerBreak->setContentAlignment (AlignmentFlag::Center);
            containerBreak->setAttributeValue (Style::style,currentBackColor);
            containerBreak->setMargin(25,Side::Top);
        }



        auto tcItem = mTCManager->Load_byOID (item.tcOid ().toStdString ());

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
            auto filePath = this->mTCManager->downloadFileWeb (tcItem.value ()->FotoOid ());
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
