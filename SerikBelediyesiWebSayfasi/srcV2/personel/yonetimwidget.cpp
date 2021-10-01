#include "yonetimwidget.h"



v2::YonetimWidget::YonetimWidget(SerikBLDCore::DB *_mDB)
    :SerikBLDCore::PersonelManager (_mDB),
      mBirimManager(new SerikBLDCore::BirimManager(_mDB))
{
    this->setMargin(90,Side::Top);

    this->setAttributeValue (Style::style,Style::background::color::rgba (this->getRandom (240,255),this->getRandom (240,255),this->getRandom (240,255),0.85));

    Header ()->addWidget (cpp14::make_unique<WText>("<h3>Yönetim Şeması</h3>"))->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black));
    Header ()->setMargin (15,Side::Top|Side::Bottom);
    Content ()->clear ();

    this->initBaskan ();

    this->initBaskanYrd ();

    this->initMudurler ();
}

void v2::YonetimWidget::initBaskan()
{

    SerikBLDCore::IK::Personel filter;
    filter.setStatu (SerikBLDCore::IK::Statu::Baskan.c_str ());

    auto baskan = this->FindOneItem (filter);

    if( baskan.view ().empty () ){
        this->showPopUpMessage ("Hata: " + this->getLastError ().toStdString (),"err");
        return;
    }


    auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    container->addStyleClass (Bootstrap::Grid::col_full_12);
//    container->setAttributeValue (Style::style,Style::color::color (Style::color::White::AliceBlue));

    auto fotoContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
    fotoContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

    auto fotoPath = this->getDB ()->downloadFileWeb (baskan.FotoOid ());

    fotoContainer->setAttributeValue (Style::style,Style::background::url (fotoPath)+Style::background::repeat::norepeat+Style::background::size::cover+Style::background::position::center_center);

    fotoContainer->setHeight (190);
    fotoContainer->setWidth (160);

    container->addWidget (cpp14::make_unique<WBreak>());

    auto baskanAdi = container->addWidget (cpp14::make_unique<WText>(baskan.AdSoyad ().toStdString ()));
    baskanAdi->setAttributeValue (Style::style,Style::font::size::s16px);

    container->addWidget (cpp14::make_unique<WBreak>());
    auto statuText = container->addWidget (cpp14::make_unique<WText>(baskan.statu ().toStdString ()));
    statuText->setAttributeValue (Style::style,Style::font::size::s16px);


}

void v2::YonetimWidget::initBaskanYrd()
{
    SerikBLDCore::IK::Personel filter;
    filter.setStatu (SerikBLDCore::IK::Statu::BaskanYardimcisi.c_str ());

    auto list = this->List (filter);

    if( list.empty () ){
        this->showPopUpMessage ("Hata: " + this->getLastError ().toStdString (),"err");
        return;
    }


    for( const auto &item : list ){


        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
//        container->setAttributeValue (Style::style,Style::color::color (Style::color::White::AliceBlue));

        auto fotoContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        fotoContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

        auto fotoPath = this->getDB ()->downloadFileWeb (item.FotoOid ());

        fotoContainer->setAttributeValue (Style::style,Style::background::url (fotoPath)+Style::background::repeat::norepeat+Style::background::size::cover+Style::background::position::center_center);

        fotoContainer->setHeight (190/10*7);
        fotoContainer->setWidth (160/10*7);

        container->addWidget (cpp14::make_unique<WBreak>());

        auto baskanAdi = container->addWidget (cpp14::make_unique<WText>(item.AdSoyad ().toStdString ()));
        container->addWidget (cpp14::make_unique<WBreak>());
        auto statuText = container->addWidget (cpp14::make_unique<WText>(item.statu ().toStdString ()));

    }


    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setHeight (100);
    }


}

void v2::YonetimWidget::initMudurler()
{
    SerikBLDCore::IK::Personel filter;
    filter.setStatu (SerikBLDCore::IK::Statu::Mudur.c_str ());

    auto list = this->List (filter);

    if( list.empty () ){
        this->showPopUpMessage ("Hata: " + this->getLastError ().toStdString (),"err");
        return;
    }




    for( const auto &item : qAsConst(list) ){


        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
//        container->setAttributeValue (Style::style,Style::color::color (Style::color::White::AliceBlue));

        auto hLayout = container->setLayout (cpp14::make_unique<WHBoxLayout>());


        auto fotoContainer = hLayout->addWidget (cpp14::make_unique<WContainerWidget>());
        fotoContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

        auto fotoPath = this->getDB ()->downloadFileWeb (item.FotoOid ());

        fotoContainer->setAttributeValue (Style::style,Style::background::url (fotoPath)+Style::background::repeat::norepeat+Style::background::size::cover+Style::background::position::center_center);

        fotoContainer->setMinimumSize (160/10*5,190/10*5);
        fotoContainer->setMaximumSize (160/10*5,190/10*5);

        fotoContainer->setHeight (190/10*5);
        fotoContainer->setWidth (160/10*5);
        fotoContainer->setMargin (0,AllSides);

        auto infoContainer = hLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vLayout = infoContainer->setLayout (cpp14::make_unique<WVBoxLayout>());

//        container->addWidget (cpp14::make_unique<WBreak>());

        auto baskanAdi = vLayout->addWidget (cpp14::make_unique<WText>(item.AdSoyad ().toStdString ()),0,AlignmentFlag::Left);
        baskanAdi->setAttributeValue (Style::style,Style::font::size::s12px);
//        container->addWidget (cpp14::make_unique<WBreak>());
        auto statuText = vLayout->addWidget (cpp14::make_unique<WText>(item.statu ().toStdString ()),0,AlignmentFlag::Left);
        statuText->setAttributeValue (Style::style,Style::font::size::s11px);
//        container->addWidget (cpp14::make_unique<WBreak>());
        auto birimText = vLayout->addWidget (cpp14::make_unique<WText>("• "+item.Birim ().toStdString ()),0,AlignmentFlag::Left);
        birimText->setAttributeValue (Style::style,Style::font::size::s11px);

        if( item.mudurlukList ().size () ){

            for( const auto &altItemOid : item.mudurlukList () ){

                SerikBLDCore::IK::BirimItem filter;
                filter.setOid (altItemOid);



                auto val = mBirimManager->FindOneItem (filter);

                if( !val.view ().empty () ){
                    auto birim1Text = vLayout->addWidget (cpp14::make_unique<WText>("• "+val.birimAdi ().toStdString ()),1,AlignmentFlag::Left);
                    birim1Text->setAttributeValue (Style::style,Style::font::size::s11px);
                    birim1Text->setTextAlignment (AlignmentFlag::Left);
                }else{
                    auto birim1Text = vLayout->addWidget (cpp14::make_unique<WText>("• birimYok: "+altItemOid.to_string ()),1,AlignmentFlag::Left);
                    birim1Text->setAttributeValue (Style::style,Style::font::size::s11px);
                    birim1Text->setTextAlignment (AlignmentFlag::Left);
                }
            }


        }

//        container->setHeight (200);
//        container->setMargin (15,Side::Bottom);

    }

}
