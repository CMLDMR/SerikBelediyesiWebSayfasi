#include "birimislericontainer.h"


#include "SerikBelediyesiWebSayfasi/srcV2/firma/firmamanagerpage.h"
#include "SerikBelediyesiWebSayfasi/srcV2/imar/mimariprojemanagerpage.h"
#include "SerikBelediyesiWebSayfasi/srcV2/calismalar/calismamanagercontainer.h"
#include "SerikBelediyesiWebSayfasi/srcV2/faaliyet/faaliyetraporcontainer.h"
#include "SerikBelediyesiWebSayfasi/srcV2/nostserik.h"
#include "SerikBelediyesiWebSayfasi/srcV2/bilgiislem/firewallcontainer.h"
#include "SerikBelediyesiWebSayfasi/srcV2/duyuruyonetim.h"
#include "SerikBelediyesiWebSayfasi/srcV2/stok/stokcontainerwidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/karavan/karavancontrolwidget.h"

v2::BirimIsleriContainer::BirimIsleriContainer(SerikBLDCore::User *_user)
    :ContainerWidget ("Birim İşleri"),mUser(_user)
{

    Header ()->setMargin (10,Side::Bottom);
    Header()->setMargin(15,Side::Bottom);

    if( this->mUser->Birimi () == "İmar ve Şehircilik Müdürlüğü" ){
        this->initImar ();
    }

    //GALERI
    if( this->mUser->Birimi () == "Bilgi İşlem Müdürlüğü" ){
        auto menuFirma = createMenu ( "Galeri" , Cursor::PointingHand );
        menuFirma->clicked ().connect ( [=](){
            this->initSerikGaleri ();
        } );
        Header ()->addWidget (std::move(menuFirma));

        auto menufireWallLog = createMenu ( "FireWallLog" , Cursor::PointingHand );
        menufireWallLog->clicked ().connect ( [=](){
            this->initFireWallLog ();
        } );
        Header ()->addWidget (std::move(menufireWallLog));

        auto menuDuyuruItem = createMenu ( "Duyurular" , Cursor::PointingHand );
        menuDuyuruItem->clicked ().connect ( [=](){
            Content ()->clear ();
            Content ()->addWidget (cpp14::make_unique<v2::Duyuru::DuyuruYonetim>(this->mUser->getDB()));
        } );
        Header ()->addWidget (std::move(menuDuyuruItem));

        auto menuKaravanItem = createMenu ( "Karavan" , Cursor::PointingHand );
        menuKaravanItem->clicked ().connect ( [=](){
            Content ()->clear ();
            Content ()->addWidget (cpp14::make_unique<v2::Karavan::MainWidget>(this->mUser));
        } );
        Header ()->addWidget (std::move(menuKaravanItem));
    }

    //Çalışma Yönetimi
    {
        auto menuFirma = createMenu ( "Çalışmalar" , Cursor::PointingHand );
        menuFirma->clicked ().connect ( [=](){
            this->initCalismalar ();
        } );
        Header ()->addWidget (std::move(menuFirma));
    }

    //Depo Yönetimi
    {
        auto menuFirma = createMenu ( "Stok" , Cursor::PointingHand );
        menuFirma->clicked ().connect ( [=](){
            this->initStok();
        } );
        Header ()->addWidget (std::move(menuFirma));
    }

    //Çalışma Yönetimi
    {
        auto menuFirma = this->createButton ( "Faaliyet Raporu" , Cursor::PointingHand );
        menuFirma->clicked ().connect ( [=](){
            this->initFaaliyetRaporlari ();
        } );
        Header ()->addWidget (std::move(menuFirma));
    }

    //KARAVAN
    if( this->mUser->Birimi () == "Zabıta Müdürlüğü" ){

        auto menuKaravanItem = createMenu ( "Karavan" , Cursor::PointingHand );
        menuKaravanItem->clicked ().connect ( [=](){
            Content ()->clear ();
            Content ()->addWidget (cpp14::make_unique<v2::Karavan::MainWidget>(this->mUser));
        } );
        Header ()->addWidget (std::move(menuKaravanItem));
    }

}

void v2::BirimIsleriContainer::initFirmaYonetimi()
{
    Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<v2::FirmaManagerPage>(this->mUser));
}

void v2::BirimIsleriContainer::initImar()
{

    //1 Firma Yönetimi
    {
        auto menuFirma = createMenu ( "Firma Yönetimi" , Cursor::PointingHand );
        menuFirma->clicked ().connect ( this , &v2::BirimIsleriContainer::initFirmaYonetimi );
        Header ()->addWidget (std::move(menuFirma));
    }

    //2 Mimari Proje
    {
        auto menuFirma = createMenu ( "Mimari Projeler" , Cursor::PointingHand );
        menuFirma->clicked ().connect ( [=](){
            Content ()->clear ();
            auto mimariPrjeler = Content ()->addWidget (cpp14::make_unique<v2::KurumsalMimariProjeManagerPage>(this->mUser));

            SerikBLDCore::Imar::MimariProje::MainProje filter;
//            filter.setProjectAktif (true);
            mimariPrjeler->UpdateList (filter);

        } );
        Header ()->addWidget (std::move(menuFirma));
    }

    //2 İstatistikler
    {
        auto menuFirma = createMenu ( "Proje İstatistikleri" , Cursor::PointingHand );
        menuFirma->clicked ().connect ( [=](){


        } );
        Header ()->addWidget (std::move(menuFirma));
    }


    //2 Proje Ara
    {
        auto menuFirma = createMenu ( "Proje Ara" , Cursor::PointingHand );
        menuFirma->clicked ().connect ( [=](){


        } );
        Header ()->addWidget (std::move(menuFirma));
    }


}

void v2::BirimIsleriContainer::initCalismalar()
{

    Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<v2::CalismaManagerContainer>(this->mUser));

}

void v2::BirimIsleriContainer::initFaaliyetRaporlari()
{

    Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<v2::Faaliyet::FaaliyetRaporContainer>(this->mUser));

}

void v2::BirimIsleriContainer::initSerikGaleri()
{
    Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<v2::NostSerikManager>(this->mUser->getDB()));
}

void v2::BirimIsleriContainer::initFireWallLog()
{

    Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<FireWall::FireWallContainer>());

}

void v2::BirimIsleriContainer::initStok()
{
    this->Content ()->clear ();
    this->Content ()->addWidget (Wt::cpp14::make_unique<v2::StokWidget::StokContainerWidget>(mUser));
}

void v2::BirimIsleriContainer::initKaravanYonetim()
{
    this->Content ()->clear ();
    this->Content ()->addWidget (Wt::cpp14::make_unique<v2::StokWidget::StokContainerWidget>(mUser));
}

std::unique_ptr<WContainerWidget>  v2::BirimIsleriContainer::createMenu(const std::string &menuName , Cursor cursor )
{
    auto container = cpp14::make_unique<WContainerWidget>();
    container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                              Bootstrap::Grid::Medium::col_md_2+
                              Bootstrap::Grid::Small::col_sm_3+
                              Bootstrap::Grid::ExtraSmall::col_xs_3+
                              Bootstrap::ImageShape::img_thumbnail);
    container->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (),
                                                                              this->getRandom (),
                                                                              this->getRandom ()));
    auto btnText = container->addWidget (cpp14::make_unique<WText>("<b>"+menuName+"</b>"));
    btnText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
    container->decorationStyle ().setCursor (cursor);
    container->addStyleClass (CSSStyle::Button::blueButton);
    container->setHeight (40);
    return container;
}
