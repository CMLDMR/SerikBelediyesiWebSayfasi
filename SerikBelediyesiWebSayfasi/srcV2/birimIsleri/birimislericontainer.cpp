#include "birimislericontainer.h"


#include "SerikBelediyesiWebSayfasi/srcV2/firma/firmamanagerpage.h"


v2::BirimIsleriContainer::BirimIsleriContainer(SerikBLDCore::User *_user)
    :mUser(_user),ContainerWidget ("Birim İşleri")
{

    Header ()->setMargin (10,Side::Bottom);

    if( this->mUser->Birimi () == "İmar ve Şehircilik Müdürlüğü" ){
        this->initImar ();
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

    //2 Başvurular
    {
        auto menuFirma = createMenu ( "Başvurular" , Cursor::PointingHand );
//        menuFirma->clicked ().connect ( this , &v2::BirimIsleriContainer::initFirmaYonetimi );
        Header ()->addWidget (std::move(menuFirma));
    }

    //3 Mimari Proje
    {
        auto menuFirma = createMenu ( "Mimari Projeler" , Cursor::PointingHand );
//        menuFirma->clicked ().connect ( this , &v2::BirimIsleriContainer::initFirmaYonetimi );
        Header ()->addWidget (std::move(menuFirma));
    }


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
    return container;
}
