#include "mimariprojemanagerpage.h"

#include "mainprojeview.h"


v2::MimariProjeManagerPage::MimariProjeManagerPage(SerikBLDCore::DB* _db , SerikBLDCore::TC *_tcUser)
    :ContainerWidget ("Projeler"),
      SerikBLDCore::Imar::MainProjeManager (_db),
      mFirmaYetkilisi(_tcUser),
      mFirmaOid("")
{

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
        adaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                     Bootstrap::Grid::Medium::col_md_2+
                                     Bootstrap::Grid::Small::col_sm_2+
                                     Bootstrap::Grid::ExtraSmall::col_xs_2);

        auto parselContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        parselContainer->addWidget (cpp14::make_unique<WText>("<b>#Parsel</b>"));
        parselContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                        Bootstrap::Grid::Medium::col_md_2+
                                        Bootstrap::Grid::Small::col_sm_2+
                                        Bootstrap::Grid::ExtraSmall::col_xs_2);

        auto mahalleContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mahalleContainer->addWidget (cpp14::make_unique<WText>("<b>#Mahalle</b>"));
        mahalleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                         Bootstrap::Grid::Medium::col_md_3+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3);

        auto onayContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        onayContainer->addWidget (cpp14::make_unique<WText>("<b>#Onay</b>"));
        onayContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);

    }


    for( auto item : *mlist ){
        auto fContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        fContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        fContainer->setMargin (2,Side::Top|Side::Bottom);
        fContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        fContainer->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
        fContainer->setPadding (0,Side::Top|Side::Bottom);


        auto adiContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adiContainer->addWidget (cpp14::make_unique<WText>(item.adi ()));
        adiContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                     Bootstrap::Grid::Medium::col_md_3+
                                     Bootstrap::Grid::Small::col_sm_3+
                                     Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto adaContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adaContainer->addWidget (cpp14::make_unique<WText>(std::to_string (item.ada ())));
        adaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                     Bootstrap::Grid::Medium::col_md_2+
                                     Bootstrap::Grid::Small::col_sm_2+
                                     Bootstrap::Grid::ExtraSmall::col_xs_2);

        auto parselContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        parselContainer->addWidget (cpp14::make_unique<WText>(std::to_string (item.parsel ())));
        parselContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                        Bootstrap::Grid::Medium::col_md_2+
                                        Bootstrap::Grid::Small::col_sm_2+
                                        Bootstrap::Grid::ExtraSmall::col_xs_2);

        auto mahalleContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mahalleContainer->addWidget (cpp14::make_unique<WText>(item.mahalle ()));
        mahalleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                         Bootstrap::Grid::Medium::col_md_3+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto onayContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        onayContainer->addWidget (cpp14::make_unique<WText>(item.onay () ? "Onaylı" : "Onaylanmamış"))->setAttributeValue (Style::style,Style::font::family::dosis);
        onayContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2+
                                      CSSStyle::Shadows::shadow8px);
        onayContainer->addStyleClass (item.onay () ? CSSStyle::Gradient::greenGradient : CSSStyle::Gradient::redGradient);
        onayContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (0,50),
                                                                                      this->getRandom (25,75),
                                                                                      this->getRandom (50,125)));

        fContainer->clicked ().connect ([=](){
            this->loadProject (item.oid ().value ().to_string ());
        });

    }

    auto controllerListContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    controllerListContainer->setMargin (5,AllSides);
    controllerListContainer->setWidth (WLength("100%"));
    auto hLayout = controllerListContainer->setLayout (cpp14::make_unique<WHBoxLayout>());
    auto backContainer = hLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Left);
    backContainer->setPadding (10,Side::Top|Side::Bottom);
    backContainer->setPadding (20,Side::Right|Side::Left);
    backContainer->addWidget (cpp14::make_unique<WText>("Geri"));
    backContainer->addStyleClass (CSSStyle::Button::blueButton+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    backContainer->clicked ().connect ([=](){
        if( mFirmaOid.empty () ){
            SerikBLDCore::Imar::MainProjeManager::next (SerikBLDCore::Imar::MimariProje::MainProje());
        }else{
            SerikBLDCore::Imar::MainProjeManager::next (SerikBLDCore::Imar::MimariProje::MainProje().setFirmaOid (bsoncxx::oid{mFirmaOid}));
        }
    });

    auto nextContainer = hLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Right);
    nextContainer->setPadding (10,Side::Top|Side::Bottom);
    nextContainer->setPadding (20,Side::Right|Side::Left);
    nextContainer->addWidget (cpp14::make_unique<WText>("İleri"));
    nextContainer->addStyleClass (CSSStyle::Button::blueButton+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    nextContainer->clicked ().connect ([=](){
        if( mFirmaOid.empty () ){
            SerikBLDCore::Imar::MainProjeManager::next (SerikBLDCore::Imar::MimariProje::MainProje());
        }else{
            SerikBLDCore::Imar::MainProjeManager::next (SerikBLDCore::Imar::MimariProje::MainProje().setFirmaOid (bsoncxx::oid{mFirmaOid}));
        }
    });

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

SerikBLDCore::TC *v2::MimariProjeManagerPage::User()
{
    return mFirmaYetkilisi;
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




v2::BireyselMimariProjeManagerPage::BireyselMimariProjeManagerPage(SerikBLDCore::DB *_db,
                                                                   SerikBLDCore::TC *_tcUser)
    :MimariProjeManagerPage (_db,_tcUser)
{

    Content ()->setMargin (25,Side::Bottom|Side::Top);

    auto containerListProject = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    containerListProject->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                         Bootstrap::Grid::Medium::col_md_6+
                                         Bootstrap::Grid::Small::col_sm_6+
                                         Bootstrap::Grid::ExtraSmall::col_xs_6+
                                         CSSStyle::Button::blueButton);
    containerListProject->decorationStyle ().setCursor (Cursor::PointingHand);
    containerListProject->addWidget (cpp14::make_unique<WText>("<h5><b>Projeleri Listele</b></h5>"));
    containerListProject->setHeight (40);
    containerListProject->clicked ().connect (this,&BireyselMimariProjeManagerPage::loadList );


    auto containerListMainProject = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    containerListMainProject->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                         Bootstrap::Grid::Medium::col_md_6+
                                         Bootstrap::Grid::Small::col_sm_6+
                                         Bootstrap::Grid::ExtraSmall::col_xs_6+
                                         CSSStyle::Button::blueButton);
    containerListMainProject->decorationStyle ().setCursor (Cursor::PointingHand);
    containerListMainProject->addWidget (cpp14::make_unique<WText>("<h5><b>Yapıları Listele</b></h5>"));
    containerListMainProject->setHeight (40);
    containerListMainProject->clicked ().connect (this,&BireyselMimariProjeManagerPage::loadYapiList );
    this->loadList ();

}

void v2::BireyselMimariProjeManagerPage::loadList()
{

    Content ()->clear ();
    SerikBLDCore::Imar::MimariProje::BaseProject filter;
    filter.append(SerikBLDCore::Imar::MimariProje::BaseProject::keyOwnerOid,this->User ()->oid ().value ());

    mTotalItemCount = SerikBLDCore::DB::countItem (filter);

    auto cursor = SerikBLDCore::DB::find (filter,limit,skip);

    if( cursor ){
        for( auto item : cursor.value () ){

            SerikBLDCore::Imar::MimariProje::BaseProject project;
            project.setDocumentView (item);

            SerikBLDCore::Imar::MimariProje::MainProje mainfilter;
            mainfilter.setOid (project.mainProjectOid ());

            auto mainProje = SerikBLDCore::Imar::MainProjeManager::FindOneItem (mainfilter);

            auto _container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_3+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
            _container->setMargin (15,Side::Top);


            auto container = _container->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::col_full_12+
                                      CSSStyle::Shadows::shadow8px+
                                      Bootstrap::Button::Primary);
            container->setContentAlignment (AlignmentFlag::Center);
            container->decorationStyle ().setCursor (Cursor::PointingHand);


            container->addWidget (cpp14::make_unique<WText>("<b>"+mainProje.adi ()+"</b>"));
            container->addWidget (cpp14::make_unique<WBreak>());


            container->setAttributeValue (Style::dataoid,project.mainProjectOid ());
            container->clicked ().connect ([=](){
               this->loadProject (container->attributeValue (Style::dataoid).toUTF8 ());
            });

            switch (project.type ()) {
            case SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Mimari:
            {
                auto mimariProject = project.toMimariProje ();
                auto projectTitle = container->addWidget (cpp14::make_unique<WText>("<i>Mimari Proje</i>"));
                container->addWidget (cpp14::make_unique<WBreak>());
                auto projectIcon = container->addWidget (cpp14::make_unique<WImage>(WLink(mimariProject.iconPath ())));
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                auto projectOnay = container->addWidget (cpp14::make_unique<WText>( mimariProject.onay () ? "Onaylandı" : "Beklemede" ) );
                projectOnay->setAttributeValue (Style::style,Style::background::color::color ( !mimariProject.onay () ? Style::color::Red::DarkRed : Style::color::Green::DarkCyan) );
                projectOnay->setPadding (10,AllSides);
            }
                break;

            case SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Statik:
            {
                auto mimariProject = project.toStatikProje ();
                auto projectTitle = container->addWidget (cpp14::make_unique<WText>("Statik"));
                container->addWidget (cpp14::make_unique<WBreak>());
                auto projectIcon = container->addWidget (cpp14::make_unique<WImage>(WLink(mimariProject.iconPath ())));
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                auto projectOnay = container->addWidget (cpp14::make_unique<WText>( mimariProject.onay () ? "Onaylandı" : "Beklemede" ) );
                projectOnay->setAttributeValue (Style::style,Style::background::color::color ( !mimariProject.onay () ? Style::color::Red::DarkRed : Style::color::Green::DarkCyan) );
                projectOnay->setPadding (10,AllSides);
            }
                break;

            case SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Elektrik:
            {
                auto mimariProject = project.toElectricProje ();
                auto projectTitle = container->addWidget (cpp14::make_unique<WText>("Elektrik"));
                container->addWidget (cpp14::make_unique<WBreak>());
                auto projectIcon = container->addWidget (cpp14::make_unique<WImage>(WLink(mimariProject.iconPath ())));
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                auto projectOnay = container->addWidget (cpp14::make_unique<WText>( mimariProject.onay () ? "Onaylandı" : "Beklemede" ) );
                projectOnay->setAttributeValue (Style::style,Style::background::color::color ( !mimariProject.onay () ? Style::color::Red::DarkRed : Style::color::Green::DarkCyan) );
                projectOnay->setPadding (10,AllSides);
            }
                break;

            case SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Mekanik:
            {
                auto mimariProject = project.toMechanicProjec ();
                auto projectTitle = container->addWidget (cpp14::make_unique<WText>("Mekanik"));
                container->addWidget (cpp14::make_unique<WBreak>());
                auto projectIcon = container->addWidget (cpp14::make_unique<WImage>(WLink(mimariProject.iconPath ())));
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                auto projectOnay = container->addWidget (cpp14::make_unique<WText>( mimariProject.onay () ? "Onaylandı" : "Beklemede" ) );
                projectOnay->setAttributeValue (Style::style,Style::background::color::color ( !mimariProject.onay () ? Style::color::Red::DarkRed : Style::color::Green::DarkCyan) );
                projectOnay->setPadding (10,AllSides);
            }
                break;


            case SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Asansor:
            {
                auto mimariProject = project.toMechanicProjec ();
                auto projectTitle = container->addWidget (cpp14::make_unique<WText>("Asansör"));
                container->addWidget (cpp14::make_unique<WBreak>());
                auto projectIcon = container->addWidget (cpp14::make_unique<WImage>(WLink(mimariProject.iconPath ())));
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                auto projectOnay = container->addWidget (cpp14::make_unique<WText>( mimariProject.onay () ? "Onaylandı" : "Beklemede" ) );
                projectOnay->setAttributeValue (Style::style,Style::background::color::color ( !mimariProject.onay () ? Style::color::Red::DarkRed : Style::color::Green::DarkCyan) );
                projectOnay->setPadding (10,AllSides);
            }
                break;


            case SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::ZeminEtudu:
            {
                auto mimariProject = project.toZeminEtudu ();
                auto projectTitle = container->addWidget (cpp14::make_unique<WText>("Zemin Etüdü"));
                container->addWidget (cpp14::make_unique<WBreak>());
                auto projectIcon = container->addWidget (cpp14::make_unique<WImage>(WLink(mimariProject.iconPath ())));
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                auto projectOnay = container->addWidget (cpp14::make_unique<WText>( mimariProject.onay () ? "Onaylandı" : "Beklemede" ) );
                projectOnay->setAttributeValue (Style::style,Style::background::color::color ( !mimariProject.onay () ? Style::color::Red::DarkRed : Style::color::Green::DarkCyan) );
                projectOnay->setPadding (10,AllSides);
            }
                break;


            case SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Iskele:
            {
                auto mimariProject = project.toIskele ();
                auto projectTitle = container->addWidget (cpp14::make_unique<WText>("İskele"));
                container->addWidget (cpp14::make_unique<WBreak>());
                auto projectIcon = container->addWidget (cpp14::make_unique<WImage>(WLink(mimariProject.iconPath ())));
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                auto projectOnay = container->addWidget (cpp14::make_unique<WText>( mimariProject.onay () ? "Onaylandı" : "Beklemede" ) );
                projectOnay->setAttributeValue (Style::style,Style::background::color::color ( !mimariProject.onay () ? Style::color::Red::DarkRed : Style::color::Green::DarkCyan) );
                projectOnay->setPadding (10,AllSides);
            }
                break;


            case SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Ruhsat:
            {
                auto mimariProject = project.toRuhsat ();
                auto projectTitle = container->addWidget (cpp14::make_unique<WText>("Ruhsat"));
                container->addWidget (cpp14::make_unique<WBreak>());
                auto projectIcon = container->addWidget (cpp14::make_unique<WImage>(WLink(mimariProject.iconPath ())));
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                auto projectOnay = container->addWidget (cpp14::make_unique<WText>( mimariProject.onay () ? "Onaylandı" : "Beklemede" ) );
                projectOnay->setAttributeValue (Style::style,Style::background::color::color ( !mimariProject.onay () ? Style::color::Red::DarkRed : Style::color::Green::DarkCyan) );
                projectOnay->setPadding (10,AllSides);
            }
                break;


            case SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Iskan:
            {
                auto mimariProject = project.toIskan ();
                auto projectTitle = container->addWidget (cpp14::make_unique<WText>("İskan"));
                container->addWidget (cpp14::make_unique<WBreak>());
                auto projectIcon = container->addWidget (cpp14::make_unique<WImage>(WLink(mimariProject.iconPath ())));
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (10);
                container->addWidget (cpp14::make_unique<WBreak>())->setHeight (10);
                auto projectOnay = container->addWidget (cpp14::make_unique<WText>( mimariProject.onay () ? "Onaylandı" : "Beklemede" ) );
                projectOnay->setAttributeValue (Style::style,Style::background::color::color ( !mimariProject.onay () ? Style::color::Red::DarkRed : Style::color::Green::DarkCyan) );
                projectOnay->setPadding (10,AllSides);
            }
                break;


            default:
                break;
            }


//            container->setHeight (100);


        }
    }


    auto controllerListContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    controllerListContainer->setMargin (5,AllSides);
    controllerListContainer->setWidth (WLength("100%"));
    auto hLayout = controllerListContainer->setLayout (cpp14::make_unique<WHBoxLayout>());
    auto backContainer = hLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Left);
    backContainer->setPadding (10,Side::Top|Side::Bottom);
    backContainer->setPadding (20,Side::Right|Side::Left);
    backContainer->addWidget (cpp14::make_unique<WText>("Geri"));
    backContainer->addStyleClass (CSSStyle::Button::blueButton+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    backContainer->clicked ().connect ([=](){

        if( skip > 0 ){
            skip -= limit;
            skip = skip < 0 ? 0 : skip;
            this->loadList ();
        }

    });

    auto nextContainer = hLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Right);
    nextContainer->setPadding (10,Side::Top|Side::Bottom);
    nextContainer->setPadding (20,Side::Right|Side::Left);
    nextContainer->addWidget (cpp14::make_unique<WText>("İleri"));
    nextContainer->addStyleClass (CSSStyle::Button::blueButton+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    nextContainer->clicked ().connect ([=](){
        if( skip < (mTotalItemCount - limit) ){
            skip += limit;
            this->loadList ();
        }
    });

    this->showPopUpMessage ("Proje Listesi Güncellendi");

}

void v2::BireyselMimariProjeManagerPage::loadYapiList()
{
    SerikBLDCore::Imar::MimariProje::BaseProject filter;
    filter.append(SerikBLDCore::Imar::MimariProje::BaseProject::keyOwnerOid,this->User ()->oid ().value ());

    auto cursor = SerikBLDCore::DB::find (filter);

    SerikBLDCore::Imar::MimariProje::MainProje orFilter;

    if( cursor ){
        for( auto item : cursor.value () ){
            SerikBLDCore::Imar::MimariProje::BaseProject project;
            project.setDocumentView (item);
            SerikBLDCore::Imar::MimariProje::MainProje mainfilter;
            mainfilter.setOid (project.mainProjectOid ());
            orFilter.pushArray("$or",mainfilter.view ());
        }
    }
    SerikBLDCore::Imar::MainProjeManager::UpdateList (orFilter);
}

void v2::BireyselMimariProjeManagerPage::loadProject(const std::string &projectOid)
{
    SerikBLDCore::Imar::MimariProje::MainProje filter;
    filter.setOid (projectOid);

    auto val = this->FindOneItem (filter);
    if( !val.view ().empty () ){
        Content ()->clear ();

        auto proje = Content ()->addWidget (cpp14::make_unique<v2::BireyselProjeView>(val,this->getDB (),this->User ()));
        proje->addStyleClass (Bootstrap::Grid::col_full_12);
        proje->initHeader ();

    }else{
        errorOccured ("Bu Proje Yüklenemiyor");
    }

}
