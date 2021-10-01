#include "meclisuyesipublicwidget.h"



v2::MeclisUyesiPublicWidget::MeclisUyesiPublicWidget(SerikBLDCore::DB *_mDB)
    :SerikBLDCore::Meclis::UyeManager (_mDB),
      mTCManager(new SerikBLDCore::TCManagerV2(_mDB))
{
    this->setMargin(90,Side::Top);

    this->setAttributeValue (Style::style,Style::background::color::rgba (this->getRandom (240,255),this->getRandom (240,255),this->getRandom (240,255),0.55));
    Header ()->addWidget (cpp14::make_unique<WText>("<h3>Meclis Üyelerimiz</h3>"))->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black));


    auto list = this->List (SerikBLDCore::Meclis::MeclisUyesi().setDonemAdi ("2019 - 2024"),SerikBLDCore::FindOptions().setLimit (100));



    QStringList partiList;

    for( const auto &item : list ){
        partiList.append (item.partiAdi ());
    }

    partiList.removeDuplicates ();

    for( const auto &partiAdi : partiList ){

        auto _container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto rContainer = _container->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);
//        if( partiAdi == "Adalet ve Kalkınma Partisi");
        rContainer->setAttributeValue (Style::style,Style::background::color::rgba (this->getRandom (0,255),this->getRandom (0,255),this->getRandom (0,255),0.75));
        rContainer->addStyleClass (CSSStyle::Gradient::blueGradient+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
        rContainer->setMargin (55,Side::Bottom);


        {
            auto partiContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            partiContainer->addStyleClass (Bootstrap::Grid::col_full_12);
            auto text = partiContainer->addWidget (cpp14::make_unique<WText>("<h4>"+partiAdi.toStdString ()+"</h4>"));
        }

        for( const auto &item : list ){
            if( item.partiAdi () == partiAdi ){
                auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                          Bootstrap::Grid::Medium::col_md_2+
                                          Bootstrap::Grid::Small::col_sm_3+
                                          Bootstrap::Grid::ExtraSmall::col_xs_4);

//                container->setHeight (100);

                auto fotoContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());

                auto tcItem = mTCManager->Load_byOID (item.tcOid ().toStdString ());

                auto fotoPath = this->getDB ()->downloadFileWeb (tcItem.value ()->FotoOid ());

                fotoContainer->setAttributeValue (Style::style,Style::background::url (fotoPath)+
                                                  Style::background::size::cover+
                                                  Style::background::position::center_center+
                                                  Style::background::repeat::norepeat);
                container->addWidget (cpp14::make_unique<WBreak>());
                fotoContainer->setWidth (160-16*3);
                fotoContainer->setHeight (190-19*3);

                fotoContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

                partiList.append (item.partiAdi ());

                container->addWidget (cpp14::make_unique<WText>(tcItem.value ()->AdSoyad ().toStdString ()));
            }
        }
//        {
//            auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass (Bootstrap::Grid::col_full_12);
//            container->setHeight (20);
//        }
    }






}
