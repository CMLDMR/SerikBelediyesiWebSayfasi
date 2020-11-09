#include "birimislericontainer.h"


#include "SerikBelediyesiWebSayfasi/srcV2/firma/firmamanagerpage.h"
#include "SerikBelediyesiWebSayfasi/srcV2/imar/mimariprojemanagerpage.h"
#include "SerikBelediyesiWebSayfasi/srcV2/calismalar/calismamanagercontainer.h"


v2::BirimIsleriContainer::BirimIsleriContainer(SerikBLDCore::User *_user)
    :mUser(_user),ContainerWidget ("Birim İşleri")
{

    Header ()->setMargin (10,Side::Bottom);

    if( this->mUser->Birimi () == "İmar ve Şehircilik Müdürlüğü" ){
        this->initImar ();
    }

    //Çalışma Yönetimi
    {
        auto menuFirma = createMenu ( "Çalışmalar" , Cursor::PointingHand );
        menuFirma->clicked ().connect ( [=](){


            this->initCalismalar ();


        } );
        Header ()->addWidget (std::move(menuFirma));
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
