#include "mimariprojemanagerpage.h"

#include "mainprojeview.h"


v2::MimariProjeManagerPage::MimariProjeManagerPage(SerikBLDCore::DB* _db , SerikBLDCore::TC *_tcUser)
    :ContainerWidget ("Başvurulan Projeler"),
      SerikBLDCore::Imar::MainProjeManager (_db),
      mFirmaYetkilisi(_tcUser)
{

    SerikBLDCore::Imar::MainProjeManager::setLimit (10000);
}

void v2::MimariProjeManagerPage::onList(const QVector<SerikBLDCore::Imar::MimariProje::MainProje> *mlist)
{
    Content ()->clear ();
    {
        auto fContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        fContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        fContainer->setMargin (10,Side::Top|Side::Bottom);
        fContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::DimGray)+Style::color::color (Style::color::White::Snow));

        auto adiContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adiContainer->addWidget (cpp14::make_unique<WText>("<b>#Proje Adı</b>"));
        adiContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                     Bootstrap::Grid::Medium::col_md_3+
                                     Bootstrap::Grid::Small::col_sm_3+
                                     Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto adaContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adaContainer->addWidget (cpp14::make_unique<WText>("<b>#Ada</b>"));
        adaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                     Bootstrap::Grid::Medium::col_md_3+
                                     Bootstrap::Grid::Small::col_sm_3+
                                     Bootstrap::Grid::ExtraSmall::col_xs_3);

        auto parselContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        parselContainer->addWidget (cpp14::make_unique<WText>("<b>#Parsel</b>"));
        parselContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                        Bootstrap::Grid::Medium::col_md_3+
                                        Bootstrap::Grid::Small::col_sm_3+
                                        Bootstrap::Grid::ExtraSmall::col_xs_3);

        auto mahalleContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mahalleContainer->addWidget (cpp14::make_unique<WText>("<b>#Mahalle</b>"));
        mahalleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                         Bootstrap::Grid::Medium::col_md_3+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3);

    }


    for( auto item : *mlist ){
        auto fContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        fContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        fContainer->setMargin (2,Side::Top|Side::Bottom);
        fContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        fContainer->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());

        auto adiContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adiContainer->addWidget (cpp14::make_unique<WText>(item.adi ()));
        adiContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                     Bootstrap::Grid::Medium::col_md_3+
                                     Bootstrap::Grid::Small::col_sm_3+
                                     Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto adaContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adaContainer->addWidget (cpp14::make_unique<WText>(std::to_string (item.ada ())));
        adaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                     Bootstrap::Grid::Medium::col_md_3+
                                     Bootstrap::Grid::Small::col_sm_3+
                                     Bootstrap::Grid::ExtraSmall::col_xs_3);

        auto parselContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        parselContainer->addWidget (cpp14::make_unique<WText>(std::to_string (item.parsel ())));
        parselContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                        Bootstrap::Grid::Medium::col_md_3+
                                        Bootstrap::Grid::Small::col_sm_3+
                                        Bootstrap::Grid::ExtraSmall::col_xs_3);

        auto mahalleContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mahalleContainer->addWidget (cpp14::make_unique<WText>(item.mahalle ()));
        mahalleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                         Bootstrap::Grid::Medium::col_md_3+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3);


        fContainer->clicked ().connect ([=](){
            this->loadProject (item.oid ().value ().to_string ());
        });

    }

    this->showPopUpMessage ("Proje Listesi Güncellendi");

    Footer ()->clear ();

}

void v2::MimariProjeManagerPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage ("Hata: " + errorText,"err");
}





void v2::MimariProjeManagerPage::setFirmaOid(const std::string &firmaOid)
{
    mFirmaOid = firmaOid;

    SerikBLDCore::Imar::MimariProje::MainProje filter;
    filter.setFirmaOid (bsoncxx::oid{firmaOid});

    SerikBLDCore::Imar::MainProjeManager::UpdateList (filter);
}

void v2::MimariProjeManagerPage::loadProject(const std::string &projectOid)
{

    SerikBLDCore::Imar::MimariProje::MainProje filter;
    filter.setOid (projectOid);

    auto val = this->FindOneItem (filter);
    if( !val.view ().empty () ){
        Content ()->clear ();

        auto proje = Content ()->addWidget (cpp14::make_unique<v2::MainProjeView>(val,this->getDB (),mFirmaYetkilisi));
        proje->addStyleClass (Bootstrap::Grid::col_full_12);
        proje->initHeader ();

    }else{
        errorOccured ("Bu Proje Yüklenemiyor");
    }


}














v2::KurumsalMimariProjeManagerPage::KurumsalMimariProjeManagerPage(SerikBLDCore::User *_user)
    :MimariProjeManagerPage (_user->getDB (),nullptr),mUser(_user)
{

}

void v2::KurumsalMimariProjeManagerPage::loadProject(const std::string &projectOid)
{

    SerikBLDCore::Imar::MimariProje::MainProje filter;
    filter.setOid (projectOid);

    auto val = this->FindOneItem (filter);
    if( !val.view ().empty () ){
        Content ()->clear ();

        auto proje = Content ()->addWidget (cpp14::make_unique<v2::KurumsalProjeView>(val,this->mUser));
        proje->addStyleClass (Bootstrap::Grid::col_full_12);
        proje->initHeader ();
    }else{
        errorOccured ("Bu Proje Yüklenemiyor");
    }

}
