#include "mimariprojemanagerpage.h"

#include "mimarprojeview.h"


v2::MimariProjeManagerPage::MimariProjeManagerPage(SerikBLDCore::DB* _db , SerikBLDCore::TC *_tcUser)
    :ContainerWidget ("Başvurulan Projeler"),
      SerikBLDCore::Imar::MimariProjeManager (_db),
      SerikBLDCore::Imar::MimariLogManager(_db),
      mCurrentLogFilterIndex(0),
      mFirmaYetkilisi(_tcUser)
{

    SerikBLDCore::Imar::MimariProjeManager::setLimit (10000);
}

void v2::MimariProjeManagerPage::onList(const QVector<SerikBLDCore::Imar::MimariProje::MimariProje> *mlist)
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
        adaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                     Bootstrap::Grid::Medium::col_md_1+
                                     Bootstrap::Grid::Small::col_sm_1+
                                     Bootstrap::Grid::ExtraSmall::col_xs_1);

        auto parselContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        parselContainer->addWidget (cpp14::make_unique<WText>(std::to_string (item.parsel ())));
        parselContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                        Bootstrap::Grid::Medium::col_md_1+
                                        Bootstrap::Grid::Small::col_sm_1+
                                        Bootstrap::Grid::ExtraSmall::col_xs_1);

        auto mahalleContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mahalleContainer->addWidget (cpp14::make_unique<WText>(item.mahalle ()));
        mahalleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                         Bootstrap::Grid::Medium::col_md_3+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto onayContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        onayContainer->addWidget (cpp14::make_unique<WText>(item.onayli () ? "Onaylandı" : "Onaylanmadı!"));
        onayContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);
        if( item.onayli () ){
            fContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::Lime));
        }else{
            fContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::LightCoral));
        }

        auto aktifContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        aktifContainer->addWidget (cpp14::make_unique<WText>(item.projeAktif () ? "<b>Aktif</b>" : "<b>Pasif!</b>"));
        aktifContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                       Bootstrap::Grid::Medium::col_md_2+
                                       Bootstrap::Grid::Small::col_sm_2+
                                       Bootstrap::Grid::ExtraSmall::col_xs_2+
                                       Bootstrap::ImageShape::img_thumbnail);
        if( item.projeAktif () ){
            aktifContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::SeaGreen)+
                                               Style::color::color (Style::color::White::Snow));
        }else{
            aktifContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::IndianRed)+
                                               Style::color::color (Style::color::White::Snow));
        }

        fContainer->clicked ().connect ([=](){
            std::cout << "Firma CLicked Project" << std::endl;
            this->loadProject (fContainer->attributeValue (Style::dataoid).toUTF8 ());
        });

    }

    this->showPopUpMessage ("Proje Listesi Güncellendi");

    Footer ()->clear ();

}

void v2::MimariProjeManagerPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage ("Hata: " + errorText,"err");
}

void v2::MimariProjeManagerPage::onList(const QVector<SerikBLDCore::Imar::MimariLog> *mlist)
{
    Footer ()->clear ();



    for( auto item : *mlist ){



        switch (item.LogType ()) {
        case SerikBLDCore::Imar::MimariLog::Type::Log:
            if( mCurrentLogFilterIndex == 0 || mCurrentLogFilterIndex == 4){
                auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
                container->setPadding (20,Side::Top);
                container->setPadding (10,Side::Bottom);
                container->setMargin (5,Side::Top|Side::Bottom);
                container->setContentAlignment (AlignmentFlag::Left);

                auto sItem = item.toIslemLog ();
                container->setPositionScheme (PositionScheme::Relative);
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Right);

                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>(sItem.ekleyen () + " - " +
                                                                                              WDate::fromJulianDay (sItem.julianDay ()).toString ("dd/MM/yyyy").toUTF8 () + " / " +
                                                                                              QDateTime::fromSecsSinceEpoch (sItem.mSecEpoch ()).time ().toString ("hh:mm").toStdString ()));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Left);

                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>("<u>İşlem</u>"));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                auto text = container->addWidget (cpp14::make_unique<WText>(sItem.log ()));
                text->setAttributeValue (Style::style,Style::font::family::dosis);
                text->setPadding (5,Side::Left|Side::Right);
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Ivory)
                                              +Style::color::color (Style::color::Grey::DimGray));


            }

            break;

        case SerikBLDCore::Imar::MimariLog::Type::Dosya:
            if( mCurrentLogFilterIndex == 0 || mCurrentLogFilterIndex == 3){

                auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
                container->setPadding (20,Side::Top);
                container->setPadding (10,Side::Bottom);
                container->setMargin (5,Side::Top|Side::Bottom);
                container->setContentAlignment (AlignmentFlag::Left);

                auto sItem = item.toDosyaLog ();
                container->setPositionScheme (PositionScheme::Relative);
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Right);

                    //            absoluteContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>(sItem.ekleyen () + " - " +
                                                                                              WDate::fromJulianDay (sItem.julianDay ()).toString ("dd/MM/yyyy").toUTF8 () + " / " +
                                                                                              QDateTime::fromSecsSinceEpoch (sItem.mSecEpoch ()).time ().toString ("hh:mm").toStdString ()));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Left);

                    //            absoluteContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>("<u>Dosya</u>"));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);

                }
                auto text = container->addWidget (cpp14::make_unique<WText>(sItem.fileOid ()));
                text->setAttributeValue (Style::style,Style::font::family::dosis);
                text->setPadding (5,Side::Left|Side::Right);
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::SlateGray)
                                              +Style::color::color (Style::color::White::Snow));
                container->decorationStyle ().setCursor (Cursor::PointingHand);

                container->clicked ().connect ([=](){
                    auto fileUrl = SerikBLDCore::Imar::MimariLogManager::downloadFileWeb (sItem.fileOid ().c_str ());
                    this->doJavaScript ("window.open('"+fileUrl+"','_blank');");
                });

            }
            break;

        case SerikBLDCore::Imar::MimariLog::Type::Aciklama:
            if( mCurrentLogFilterIndex == 0 || mCurrentLogFilterIndex == 2){

                auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
                container->setPadding (20,Side::Top);
                container->setPadding (10,Side::Bottom);
                container->setMargin (5,Side::Top|Side::Bottom);
                container->setContentAlignment (AlignmentFlag::Left);


                auto sItem = item.toAciklamaLog ();
                container->setPositionScheme (PositionScheme::Relative);
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Right);

                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>(sItem.ekleyen () + " - " +
                                                                                              WDate::fromJulianDay (sItem.julianDay ()).toString ("dd/MM/yyyy").toUTF8 () + " / " +
                                                                                              QDateTime::fromSecsSinceEpoch (sItem.mSecEpoch ()).time ().toString ("hh:mm").toStdString ()));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Left);

                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>("<u>Açıklama</u>"));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                auto text = container->addWidget (cpp14::make_unique<WText>(sItem.aciklama ()));
                text->setAttributeValue (Style::style,Style::font::family::dosis);
                text->setPadding (5,Side::Left|Side::Right);
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::GhostWhite)
                                              +Style::color::color (Style::color::Grey::DimGray));


            }

            break;

        case SerikBLDCore::Imar::MimariLog::Type::Duzeltme:
            if( mCurrentLogFilterIndex == 0 || mCurrentLogFilterIndex == 1 ){

                auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
                container->setPadding (20,Side::Top);
                container->setPadding (10,Side::Bottom);
                container->setMargin (5,Side::Top|Side::Bottom);
                container->setContentAlignment (AlignmentFlag::Left);


                auto sItem = item.toDuzeltmeLog ();
                container->setPositionScheme (PositionScheme::Relative);
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Right);

                    //            absoluteContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>(sItem.ekleyen () + " - " +
                                                                                              WDate::fromJulianDay (sItem.julianDay ()).toString ("dd/MM/yyyy").toUTF8 () + " / " +
                                                                                              QDateTime::fromSecsSinceEpoch (sItem.mSecEpoch ()).time ().toString ("hh:mm").toStdString ()));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Left);

                    //            absoluteContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>(sItem.duzeltildi () ? "<u>Düzeltme Tamamlandı</u>" : "<u>Düzeltme Devam Ediyor</u>"));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);

                }
                auto text = container->addWidget (cpp14::make_unique<WText>(sItem.duzeltme ()));
                text->setAttributeValue (Style::style,Style::font::family::dosis);
                text->setPadding (5,Side::Left|Side::Right);
                if( sItem.duzeltildi () ){
                    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::LawnGreen)
                                                  +Style::color::color (Style::color::White::Snow));
                }else{
                    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)
                                                  +Style::color::color (Style::color::White::Snow));
                }

            }
            break;

        case SerikBLDCore::Imar::MimariLog::Type::bilinmeyen:
        {

            auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
            container->setPadding (20,Side::Top);
            container->setPadding (10,Side::Bottom);
            container->setMargin (5,Side::Top|Side::Bottom);
            container->setContentAlignment (AlignmentFlag::Left);

            container->setPositionScheme (PositionScheme::Relative);
            auto text = container->addWidget (cpp14::make_unique<WText>("Hatalı Log"));
            text->setAttributeValue (Style::style,Style::font::family::dosis);
            text->setPadding (5,Side::Left|Side::Right);
            container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::Black)
                                          +Style::color::color (Style::color::White::Snow));

        }
            break;
        default:
            break;
        }
    }
}

void v2::MimariProjeManagerPage::setFirmaOid(const std::string &firmaOid)
{
    mFirmaOid = firmaOid;

    SerikBLDCore::Imar::MimariProje::MimariProje filter;
    filter.setFirmaOid (bsoncxx::oid{firmaOid});

    SerikBLDCore::Imar::MimariProjeManager::UpdateList (filter);
}

void v2::MimariProjeManagerPage::loadProject(const std::string &projeOid)
{
    Content ()->clear ();

    mProjectListContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    mProjectListContainer->addStyleClass (Bootstrap::Grid::col_full_12);


    mProjectInformationContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    mProjectInformationContainer->addStyleClass (Bootstrap::Grid::row);
    mProjectInformationContainer->setMargin (15,Side::Bottom);


    mProjectHeaderContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    mProjectHeaderContainer->addStyleClass (Bootstrap::Grid::col_full_12);


    mProjectDetailContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    mProjectDetailContainer->addStyleClass (Bootstrap::Grid::container_fluid);



    SerikBLDCore::Imar::MimariProje::MimariProje filter;
    filter.setOid (projeOid);
    auto projectItem = SerikBLDCore::Imar::MimariProjeManager::FindOneItem (filter);
    auto projectView = mProjectDetailContainer->addWidget (cpp14::make_unique<Comman::MimarProjeView>(projectItem,true));
    projectView->addStyleClass (Bootstrap::Grid::col_full_12);

    {


        auto rBackContainer = mProjectListContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        rBackContainer->addStyleClass (Bootstrap::Grid::row);


        auto backBtnContainer = rBackContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        backBtnContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                         Bootstrap::Grid::Medium::col_md_2+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_4);
        auto backBtn = backBtnContainer->addWidget (cpp14::make_unique<WPushButton>("Proje Listesi"));
        backBtn->clicked ().connect ([=](){
            SerikBLDCore::Imar::MimariProje::MimariProje filter;
            filter.setFirmaOid (bsoncxx::oid{mFirmaOid});
            SerikBLDCore::Imar::MimariProjeManager::UpdateList (filter);
        });




        auto activateContainer = rBackContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        activateContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                         Bootstrap::Grid::Medium::col_md_2+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_4);
        if( projectView->projeAktif () ){
            auto activateBtn = activateContainer->addWidget (cpp14::make_unique<WText>("Proje Aktif"));
            activateBtn->addStyleClass (Bootstrap::Label::info);
        }else{
            auto activateBtn = activateContainer->addWidget (cpp14::make_unique<WPushButton>("Aktif Et"));
            activateBtn->addStyleClass (Bootstrap::Button::Success);
            activateBtn->clicked ().connect ([=](){
                SerikBLDCore::Imar::MimariProje::MimariProje filter;
                filter.setOid (projeOid);
                filter.setProjectAktif (true);


                auto upt = SerikBLDCore::Imar::MimariProjeManager::UpdateItem (filter);

                if( upt ){
                    this->showPopUpMessage ("Mimari Proje Aktif Edildi","msg");
                    SerikBLDCore::Imar::IslemLog islemLog;
                    islemLog.setLog ("Mimari Proje Aktif Edildi");
                    islemLog.setMimariProjeOid (projectView->oid ().value ());
                    islemLog.setEkleyen ("Firma Yetkilisi " + mFirmaYetkilisi->AdSoyad ().toStdString ());
                    islemLog.setJulianDay (WDate::currentDate ().toJulianDay ());
                    islemLog.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                    auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (islemLog);
                    if( ins.size () ){
                        SerikBLDCore::Imar::MimariLog Filter;
                        Filter.setMimariProjeOid (projectView->oid ().value ());
                        SerikBLDCore::Imar::MimariLogManager::UpdateList (Filter);

                        activateContainer->clear ();
                        auto activateBtn_ = activateContainer->addWidget (cpp14::make_unique<WText>("Proje Aktif"));
                        activateBtn_->addStyleClass (Bootstrap::Label::info);
                    }

                }else{
                    this->showPopUpMessage ("Mimari Proje Aktif Edilemedi!\nLütfen Sayfayı Yenileyiniz","err");
                }
            });
        }


    }


    // Proje Ada Parsel Mahalle Bilgileri
    {
        mProjectInformationContainer->clear ();
        auto adaContainer = mProjectInformationContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                     Bootstrap::Grid::Medium::col_md_2+
                                     Bootstrap::Grid::Small::col_sm_2+
                                     Bootstrap::Grid::ExtraSmall::col_xs_2);
        adaContainer->addWidget (cpp14::make_unique<WText>("Ada No: "+std::to_string (projectView->ada ())))->setAttributeValue (Style::style,Style::font::family::dosis);

        auto parselContainer = mProjectInformationContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        parselContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                        Bootstrap::Grid::Medium::col_md_2+
                                        Bootstrap::Grid::Small::col_sm_2+
                                        Bootstrap::Grid::ExtraSmall::col_xs_2);
        parselContainer->addWidget (cpp14::make_unique<WText>("Parsel No: "+std::to_string (projectView->parsel ())))->setAttributeValue (Style::style,Style::font::family::dosis);

        auto mahalleContainer = mProjectInformationContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mahalleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                         Bootstrap::Grid::Medium::col_md_3+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3);
        mahalleContainer->addWidget (cpp14::make_unique<WText>(projectView->mahalle ()))->setAttributeValue (Style::style,Style::font::family::dosis);

        auto tarihContainer = mProjectInformationContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        tarihContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                       Bootstrap::Grid::Medium::col_md_3+
                                       Bootstrap::Grid::Small::col_sm_3+
                                       Bootstrap::Grid::ExtraSmall::col_xs_3);
        tarihContainer->addWidget (cpp14::make_unique<WText>("Başvuru Tarihi: " + WDate::fromJulianDay (projectView->basvuruJulianDay ()).toString ("dd/MM/yyyy").toUTF8 ()))->setAttributeValue (Style::style,Style::font::family::dosis);

        auto onayContainer = mProjectInformationContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        onayContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);
        onayContainer->addWidget (cpp14::make_unique<WText>(projectView->onayli () ? "<b>Onaylandı</b>" : "<b>Onaylanmadı</b>"))->setAttributeValue (Style::style,Style::font::family::dosis);
        onayContainer->setAttributeValue (Style::style,Style::background::color::color (projectView->onayli () ? Style::color::Green::Lime : Style::color::Red::LightCoral));

    }





    // Mimari Proje Butonu
    {
        mMimariProjectContainer = mProjectHeaderContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mMimariProjectContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                                Bootstrap::Grid::Medium::col_md_3+
                                                Bootstrap::Grid::Small::col_sm_3+
                                                Bootstrap::Grid::ExtraSmall::col_xs_6);
        mMimariProjectContainer->addStyleClass ("boxShadow");


        auto mimariDwgLayout = mMimariProjectContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
        mimariDwgLayout->addWidget (cpp14::make_unique<WText>("<b>Mimari Proje</b>"),0,AlignmentFlag::Center)->setAttributeValue (Style::style,Style::font::family::dosis);
        auto mimariProjeLinkImage = mimariDwgLayout->addWidget (cpp14::make_unique<WImage>(WLink("icon/dwgicon.png")),0,AlignmentFlag::Center);
        mimariProjeLinkImage->decorationStyle ().setCursor (Cursor::PointingHand);
        mimariProjeLinkImage->clicked ().connect ([=](){
            projectView->initMimariProject ();
        });

        {
            mMimariOnayTextContainer = mimariDwgLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
            mMimariOnayTextContainer->addStyleClass ("boxShadow");
            mMimariOnayTextContainer->setAttributeValue (Style::style,Style::background::color::color (projectView->mimariProjeOnay () ? Style::color::Green::SeaGreen : Style::color::Red::FireBrick));
            mMimariOnayText = mMimariOnayTextContainer->addWidget (cpp14::make_unique<WText>(projectView->mimariProjeOnay () ? "<b>Onaylandı</b>" : "<b><i>Beklemede</i></b>"));
            mMimariOnayText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);

            std::string personelName = projectView->assignedPersonelName ();

            auto personelAssignmedContainer = mimariDwgLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
            personelAssignmedContainer->addStyleClass ("boxShadow");
            personelAssignmedContainer->setPositionScheme (PositionScheme::Relative);

            if( personelName.empty () ){
                personelAssignmedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::FloralWhite));
                auto personelAssignmedText = personelAssignmedContainer->addWidget (cpp14::make_unique<WText>( "Proje Boşta" ));
                personelAssignmedText->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black)+Style::font::family::dosis);
            }else{
                personelAssignmedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray));
                auto personelAssignmedText = personelAssignmedContainer->addWidget (cpp14::make_unique<WText>( "<i>"+personelName+"</i>" ));
                personelAssignmedText->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black)+Style::font::family::dosis);
            }
        }
    }



    // Statik Proje Görüntüleme
    {
        mStatikProjectContainer = mProjectHeaderContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mStatikProjectContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                                Bootstrap::Grid::Medium::col_md_3+
                                                Bootstrap::Grid::Small::col_sm_3+
                                                Bootstrap::Grid::ExtraSmall::col_xs_6);
        mStatikProjectContainer->addStyleClass ("boxShadow");

        auto mimariDwgLayout = mStatikProjectContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
        mimariDwgLayout->addWidget (cpp14::make_unique<WText>("<b>Statik Proje</b>"),0,AlignmentFlag::Center)->setAttributeValue (Style::style,Style::font::family::dosis);
        auto mimariProjeLinkImage = mimariDwgLayout->addWidget (cpp14::make_unique<WImage>(WLink("icon/staticicon.png")),0,AlignmentFlag::Center);
        mimariProjeLinkImage->decorationStyle ().setCursor (Cursor::PointingHand);
        mimariProjeLinkImage->clicked ().connect ([=](){
            //            projectView->initMimariProject ();
        });

        {
            mStatikOnayTextContainer = mimariDwgLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
            mStatikOnayTextContainer->addStyleClass ("boxShadow");
            mStatikOnayTextContainer->setAttributeValue (Style::style,Style::background::color::color (false ? Style::color::Green::SeaGreen : Style::color::Red::FireBrick));
            mStatikOnayText = mStatikOnayTextContainer->addWidget (cpp14::make_unique<WText>(false ? "<b>Onaylandı</b>" : "<b><i>Beklemede</i></b>"));
            mStatikOnayText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);

            std::string personelName = "";

            auto personelAssignmedContainer = mimariDwgLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
            personelAssignmedContainer->addStyleClass ("boxShadow");
            personelAssignmedContainer->setPositionScheme (PositionScheme::Relative);

            if( personelName.empty () ){
                personelAssignmedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::FloralWhite));
                auto personelAssignmedText = personelAssignmedContainer->addWidget (cpp14::make_unique<WText>( "Proje Boşta" ));
                personelAssignmedText->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black)+Style::font::family::dosis);
            }else{
                personelAssignmedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray));
                auto personelAssignmedText = personelAssignmedContainer->addWidget (cpp14::make_unique<WText>( "<i>"+personelName+"</i>" ));
                personelAssignmedText->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black)+Style::font::family::dosis);
            }
        }
    }







    projectView->RequestMimariProje ().connect ([=]( const bsoncxx::oid& projeOid ){
        auto url = SerikBLDCore::Imar::MimariProjeManager::downloadFileWeb (projeOid.to_string ().c_str ());
        this->doJavaScript ("window.open('"+url+"','_blank');");
    });


    projectView->RequestLog ().connect ([=]( const bsoncxx::oid& _projectOid , const int& logFilter ){
        mCurrentLogFilterIndex = logFilter;
        SerikBLDCore::Imar::MimariLog Filter;
        Filter.setMimariProjeOid (_projectOid);
        SerikBLDCore::Imar::MimariLogManager::UpdateList (Filter);
    });

}


















Kurumsal::v2::MimariProjeManagerPage::MimariProjeManagerPage(SerikBLDCore::User *_user)
    :ContainerWidget ("Projeler"),
      SerikBLDCore::Imar::MimariProjeManager (_user->getDB ()),
      SerikBLDCore::Imar::MimariLogManager(_user->getDB ()),
      mUser(_user),
      mCurrentLogFilterIndex(0)
{
    SerikBLDCore::Imar::MimariProjeManager::setLimit (1000);
}

void Kurumsal::v2::MimariProjeManagerPage::onList(const QVector<SerikBLDCore::Imar::MimariProje::MimariProje> *mlist)
{
    Content ()->clear ();
    {
        auto fContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        fContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        fContainer->setMargin (10,Side::Top|Side::Bottom);
        fContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::DimGray)+Style::color::color (Style::color::White::Snow));

        auto adiContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adiContainer->addWidget (cpp14::make_unique<WText>("<b>#Proje Adı</b>"))->setAttributeValue (Style::style,Style::font::family::dosis);
        adiContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                     Bootstrap::Grid::Medium::col_md_3+
                                     Bootstrap::Grid::Small::col_sm_3+
                                     Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto adaContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adaContainer->addWidget (cpp14::make_unique<WText>("<b>#Ada</b>"))->setAttributeValue (Style::style,Style::font::family::dosis);
        adaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                     Bootstrap::Grid::Medium::col_md_1+
                                     Bootstrap::Grid::Small::col_sm_1+
                                     Bootstrap::Grid::ExtraSmall::col_xs_1);

        auto parselContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        parselContainer->addWidget (cpp14::make_unique<WText>("<b>#Parsel</b>"))->setAttributeValue (Style::style,Style::font::family::dosis);
        parselContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                        Bootstrap::Grid::Medium::col_md_1+
                                        Bootstrap::Grid::Small::col_sm_1+
                                        Bootstrap::Grid::ExtraSmall::col_xs_1);

        auto mahalleContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mahalleContainer->addWidget (cpp14::make_unique<WText>("<b>#Mahalle</b>"))->setAttributeValue (Style::style,Style::font::family::dosis);
        mahalleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                         Bootstrap::Grid::Medium::col_md_3+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3);

        auto onayContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        onayContainer->addWidget (cpp14::make_unique<WText>("<b>#Onay</b>"))->setAttributeValue (Style::style,Style::font::family::dosis);
        onayContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);


        auto durumContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        durumContainer->addWidget (cpp14::make_unique<WText>("<b>#Durum</b>"))->setAttributeValue (Style::style,Style::font::family::dosis);
        durumContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
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

        auto adiContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adiContainer->addWidget (cpp14::make_unique<WText>("<b>"+item.adi ()+"</b>"))->setAttributeValue (Style::style,Style::font::family::dosis+
                                                                                                          Style::color::color (Style::color::White::Snow));
        adiContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                     Bootstrap::Grid::Medium::col_md_3+
                                     Bootstrap::Grid::Small::col_sm_3+
                                     Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto adaContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adaContainer->addWidget (cpp14::make_unique<WText>("<b>"+std::to_string (item.ada ())+"</b>"))->setAttributeValue (Style::style,Style::font::family::dosis+
                                                                                                                           Style::color::color (Style::color::White::Snow));
        adaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                     Bootstrap::Grid::Medium::col_md_1+
                                     Bootstrap::Grid::Small::col_sm_1+
                                     Bootstrap::Grid::ExtraSmall::col_xs_1);

        auto parselContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        parselContainer->addWidget (cpp14::make_unique<WText>("<b>"+std::to_string (item.parsel ())+"</b>"))->setAttributeValue (Style::style,Style::font::family::dosis+
                                                                                                                                 Style::color::color (Style::color::White::Snow));
        parselContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                        Bootstrap::Grid::Medium::col_md_1+
                                        Bootstrap::Grid::Small::col_sm_1+
                                        Bootstrap::Grid::ExtraSmall::col_xs_1);

        auto mahalleContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mahalleContainer->addWidget (cpp14::make_unique<WText>("<b>"+item.mahalle ()+"</b>"))->setAttributeValue (Style::style,Style::font::family::dosis+
                                                                                                                  Style::color::color (Style::color::White::Snow));
        mahalleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                         Bootstrap::Grid::Medium::col_md_3+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto onayContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        onayContainer->addWidget (cpp14::make_unique<WText>(item.onayli () ? "<b>Onaylandı</b>" : "<b>Onaylanmadı!</b>"))->setAttributeValue (Style::style,
                                                                                                                                              Style::font::family::dosis+
                                                                                                                                              Style::color::color (Style::color::White::Snow));
        onayContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);
        if( item.onayli () ){
            fContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::Lime));
        }else{
            fContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::LightCoral));
        }

        auto aktifContainer = fContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        aktifContainer->addWidget (cpp14::make_unique<WText>(item.projeAktif () ? "<b>Aktif</b>" : "<b>Pasif!</b>"))->setAttributeValue (Style::style,Style::font::family::dosis+
                                                                                                                                         Style::color::color (Style::color::White::Snow));
        aktifContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                       Bootstrap::Grid::Medium::col_md_2+
                                       Bootstrap::Grid::Small::col_sm_2+
                                       Bootstrap::Grid::ExtraSmall::col_xs_2+
                                       Bootstrap::ImageShape::img_thumbnail);
        if( item.projeAktif () ){
            aktifContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::SeaGreen)+
                                               Style::color::color (Style::color::White::Snow));
        }else{
            aktifContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::IndianRed)+
                                               Style::color::color (Style::color::White::Snow));
        }

        fContainer->clicked ().connect ([=](){
            this->loadProject (fContainer->attributeValue (Style::dataoid).toUTF8 ());
        });

    }

    this->showPopUpMessage ("Proje Listesi Güncellendi");
    Footer ()->clear ();
}

void Kurumsal::v2::MimariProjeManagerPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage ("Hata: " + errorText,"err");
}

void Kurumsal::v2::MimariProjeManagerPage::onList(const QVector<SerikBLDCore::Imar::MimariLog> *mlist)
{
    Footer ()->clear ();



    for( auto item : *mlist ){



        switch (item.LogType ()) {
        case SerikBLDCore::Imar::MimariLog::Type::Log:
            if( mCurrentLogFilterIndex == 0 || mCurrentLogFilterIndex == 4){
                auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
                container->setPadding (20,Side::Top);
                container->setPadding (10,Side::Bottom);
                container->setMargin (5,Side::Top|Side::Bottom);
                container->setContentAlignment (AlignmentFlag::Left);

                auto sItem = item.toIslemLog ();
                container->setPositionScheme (PositionScheme::Relative);
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Right);

                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>(sItem.ekleyen () + " - " +
                                                                                              WDate::fromJulianDay (sItem.julianDay ()).toString ("dd/MM/yyyy").toUTF8 () + " / " +
                                                                                              QDateTime::fromSecsSinceEpoch (sItem.mSecEpoch ()).time ().toString ("hh:mm").toStdString ()));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Left);

                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>("<u>İşlem</u>"));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                auto text = container->addWidget (cpp14::make_unique<WText>(sItem.log ()));
                text->setAttributeValue (Style::style,Style::font::family::dosis);
                text->setPadding (5,Side::Left|Side::Right);
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Ivory)
                                              +Style::color::color (Style::color::Grey::DimGray));


            }

            break;

        case SerikBLDCore::Imar::MimariLog::Type::Dosya:
            if( mCurrentLogFilterIndex == 0 || mCurrentLogFilterIndex == 3){

                auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
                container->setPadding (20,Side::Top);
                container->setPadding (10,Side::Bottom);
                container->setMargin (5,Side::Top|Side::Bottom);
                container->setContentAlignment (AlignmentFlag::Left);

                auto sItem = item.toDosyaLog ();
                container->setPositionScheme (PositionScheme::Relative);
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Right);

                    //            absoluteContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>(sItem.ekleyen () + " - " +
                                                                                              WDate::fromJulianDay (sItem.julianDay ()).toString ("dd/MM/yyyy").toUTF8 () + " / " +
                                                                                              QDateTime::fromSecsSinceEpoch (sItem.mSecEpoch ()).time ().toString ("hh:mm").toStdString ()));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Left);

                    //            absoluteContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>("<u>Dosya</u>"));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);

                }
                auto text = container->addWidget (cpp14::make_unique<WText>(sItem.fileOid ()));
                text->setAttributeValue (Style::style,Style::font::family::dosis);
                text->setPadding (5,Side::Left|Side::Right);
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::SlateGray)
                                              +Style::color::color (Style::color::White::Snow));
                container->decorationStyle ().setCursor (Cursor::PointingHand);

                container->clicked ().connect ([=](){
                    auto fileUrl = SerikBLDCore::Imar::MimariLogManager::downloadFileWeb (sItem.fileOid ().c_str ());
                    this->doJavaScript ("window.open('"+fileUrl+"','_blank');");
                });

            }
            break;

        case SerikBLDCore::Imar::MimariLog::Type::Aciklama:
            if( mCurrentLogFilterIndex == 0 || mCurrentLogFilterIndex == 2){

                auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
                container->setPadding (20,Side::Top);
                container->setPadding (10,Side::Bottom);
                container->setMargin (5,Side::Top|Side::Bottom);
                container->setContentAlignment (AlignmentFlag::Left);


                auto sItem = item.toAciklamaLog ();
                container->setPositionScheme (PositionScheme::Relative);
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Right);

                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>(sItem.ekleyen () + " - " +
                                                                                              WDate::fromJulianDay (sItem.julianDay ()).toString ("dd/MM/yyyy").toUTF8 () + " / " +
                                                                                              QDateTime::fromSecsSinceEpoch (sItem.mSecEpoch ()).time ().toString ("hh:mm").toStdString ()));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Left);

                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>("<u>Açıklama</u>"));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                auto text = container->addWidget (cpp14::make_unique<WText>(sItem.aciklama ()));
                text->setAttributeValue (Style::style,Style::font::family::dosis);
                text->setPadding (5,Side::Left|Side::Right);
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::GhostWhite)
                                              +Style::color::color (Style::color::Grey::DimGray));


            }

            break;

        case SerikBLDCore::Imar::MimariLog::Type::Duzeltme:
            if( mCurrentLogFilterIndex == 0 || mCurrentLogFilterIndex == 1 ){

                auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
                container->setPadding (20,Side::Top);
                container->setPadding (10,Side::Bottom);
                container->setMargin (5,Side::Top|Side::Bottom);
                container->setContentAlignment (AlignmentFlag::Left);


                auto sItem = item.toDuzeltmeLog ();
                container->setPositionScheme (PositionScheme::Relative);
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Right);

                    //            absoluteContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>(sItem.ekleyen () + " - " +
                                                                                              WDate::fromJulianDay (sItem.julianDay ()).toString ("dd/MM/yyyy").toUTF8 () + " / " +
                                                                                              QDateTime::fromSecsSinceEpoch (sItem.mSecEpoch ()).time ().toString ("hh:mm").toStdString ()));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);
                }
                {
                    auto absoluteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
                    absoluteContainer->setPositionScheme (PositionScheme::Absolute);
                    absoluteContainer->setOffsets (0,Side::Top);
                    absoluteContainer->setOffsets (3,Side::Left);

                    //            absoluteContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                    auto loggerText = absoluteContainer->addWidget (cpp14::make_unique<WText>(sItem.duzeltildi () ? "<u>Düzeltme Tamamlandı</u>" : "<u>Düzeltme Devam Ediyor</u>"));
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::weight::bold+Style::font::family::dosis);

                }
                auto text = container->addWidget (cpp14::make_unique<WText>(sItem.duzeltme ()));
                text->setAttributeValue (Style::style,Style::font::family::dosis);
                text->setPadding (5,Side::Left|Side::Right);
                if( sItem.duzeltildi () ){
                    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::LawnGreen)
                                                  +Style::color::color (Style::color::White::Snow));
                }else{
                    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)
                                                  +Style::color::color (Style::color::White::Snow));
                }

            }
            break;

        case SerikBLDCore::Imar::MimariLog::Type::bilinmeyen:
        {

            auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
            container->setPadding (20,Side::Top);
            container->setPadding (10,Side::Bottom);
            container->setMargin (5,Side::Top|Side::Bottom);
            container->setContentAlignment (AlignmentFlag::Left);

            container->setPositionScheme (PositionScheme::Relative);
            auto text = container->addWidget (cpp14::make_unique<WText>("Hatalı Log"));
            text->setAttributeValue (Style::style,Style::font::family::dosis);
            text->setPadding (5,Side::Left|Side::Right);
            container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::Black)
                                          +Style::color::color (Style::color::White::Snow));

        }
            break;
        default:
            break;
        }
    }
}

void Kurumsal::v2::MimariProjeManagerPage::loadProject(const std::string &projeOid)
{
    Content ()->clear ();

    mProjectListContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    mProjectListContainer->addStyleClass (Bootstrap::Grid::col_full_12);




    mProjectInformationContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    mProjectInformationContainer->addStyleClass (Bootstrap::Grid::row);
    mProjectInformationContainer->setMargin (15,Side::Bottom|Side::Top);


    mProjectHeaderContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    mProjectHeaderContainer->addStyleClass (Bootstrap::Grid::col_full_12);


    mProjectDetailContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    mProjectDetailContainer->addStyleClass (Bootstrap::Grid::container_fluid);

    SerikBLDCore::Imar::MimariProje::MimariProje filter;
    filter.setOid (projeOid);
    auto projectItem = SerikBLDCore::Imar::MimariProjeManager::FindOneItem (filter);
    auto projectView = mProjectDetailContainer->addWidget (cpp14::make_unique<Comman::MimarProjeView>(projectItem));
    projectView->addStyleClass (Bootstrap::Grid::col_full_12);




    {
        auto rBackContainer = mProjectListContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        rBackContainer->addStyleClass (Bootstrap::Grid::row);

        auto backBtnContainer = rBackContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        backBtnContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                         Bootstrap::Grid::Medium::col_md_2+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_4);
        auto backBtn = backBtnContainer->addWidget (cpp14::make_unique<WPushButton>("Proje Listesi"));
        backBtn->clicked ().connect ([=](){
            SerikBLDCore::Imar::MimariProje::MimariProje filter;
            SerikBLDCore::Imar::MimariProjeManager::UpdateList (filter);
        });

        auto activateContainer = rBackContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        activateContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                         Bootstrap::Grid::Medium::col_md_2+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_4);
        if( !projectView->projeAktif () ){
            auto activateBtn = activateContainer->addWidget (cpp14::make_unique<WText>("Proje Pasif"));
            activateBtn->addStyleClass (Bootstrap::Label::Warning);
        }else{
            auto activateBtn = activateContainer->addWidget (cpp14::make_unique<WPushButton>("Pasif Et"));
            activateBtn->addStyleClass (Bootstrap::Button::Danger);
            activateBtn->clicked ().connect ([=](){
                SerikBLDCore::Imar::MimariProje::MimariProje filter;
                filter.setOid (projectView->oid ().value ().to_string ());
                filter.setProjectAktif (false);
                auto upt = SerikBLDCore::Imar::MimariProjeManager::UpdateItem (filter);
                if( upt ){
                    this->showPopUpMessage ("Proje Pasif Hale Getirildi","msg");
                    SerikBLDCore::Imar::IslemLog islemLog;
                    islemLog.setLog ("Proje Pasif Hale Getirildi");
                    islemLog.setMimariProjeOid (projectView->oid ().value ());
                    islemLog.setEkleyen (mUser->AdSoyad ());
                    islemLog.setJulianDay (WDate::currentDate ().toJulianDay ());
                    islemLog.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                    auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (islemLog);
                    if( ins.size () ){
                        SerikBLDCore::Imar::MimariLog Filter;
                        Filter.setMimariProjeOid (projectView->oid ().value ());
                        SerikBLDCore::Imar::MimariLogManager::UpdateList (Filter);

                        activateContainer->clear ();
                        auto activateBtn_ = activateContainer->addWidget (cpp14::make_unique<WText>("Proje Pasif"));
                        activateBtn_->addStyleClass (Bootstrap::Label::info);
                    }

                }else{
                    this->showPopUpMessage ("Proje Pasif Edilemedi!\nLütfen Sayfayı Yenileyiniz","err");
                }
            });
        }


    }

    // Proje Ada Parsel Mahalle Bilgileri
    {
        mProjectInformationContainer->clear ();
        auto adaContainer = mProjectInformationContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        adaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                     Bootstrap::Grid::Medium::col_md_2+
                                     Bootstrap::Grid::Small::col_sm_2+
                                     Bootstrap::Grid::ExtraSmall::col_xs_2);
        adaContainer->addWidget (cpp14::make_unique<WText>("Ada No: "+std::to_string (projectView->ada ())))->setAttributeValue (Style::style,Style::font::family::dosis);

        auto parselContainer = mProjectInformationContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        parselContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                        Bootstrap::Grid::Medium::col_md_2+
                                        Bootstrap::Grid::Small::col_sm_2+
                                        Bootstrap::Grid::ExtraSmall::col_xs_2);
        parselContainer->addWidget (cpp14::make_unique<WText>("Parsel No: "+std::to_string (projectView->parsel ())))->setAttributeValue (Style::style,Style::font::family::dosis);

        auto mahalleContainer = mProjectInformationContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mahalleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                         Bootstrap::Grid::Medium::col_md_3+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3);
        mahalleContainer->addWidget (cpp14::make_unique<WText>(projectView->mahalle ()))->setAttributeValue (Style::style,Style::font::family::dosis);

        auto tarihContainer = mProjectInformationContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        tarihContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                       Bootstrap::Grid::Medium::col_md_3+
                                       Bootstrap::Grid::Small::col_sm_3+
                                       Bootstrap::Grid::ExtraSmall::col_xs_3);
        tarihContainer->addWidget (cpp14::make_unique<WText>("Başvuru Tarihi: " + WDate::fromJulianDay (projectView->basvuruJulianDay ()).toString ("dd/MM/yyyy").toUTF8 ()))->setAttributeValue (Style::style,Style::font::family::dosis);

        auto onayContainer = mProjectInformationContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        onayContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);
        onayContainer->addWidget (cpp14::make_unique<WText>(projectView->onayli () ? "<b>Onaylandı</b>" : "<b>Onaylanmadı</b>"))->setAttributeValue (Style::style,Style::font::family::dosis);
        onayContainer->setAttributeValue (Style::style,Style::background::color::color (projectView->onayli () ? Style::color::Green::Lime : Style::color::Red::LightCoral));

    }


    // Mimari Proje Butonu
    {
        mMimariProjectContainer = mProjectHeaderContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mMimariProjectContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                                Bootstrap::Grid::Medium::col_md_3+
                                                Bootstrap::Grid::Small::col_sm_3+
                                                Bootstrap::Grid::ExtraSmall::col_xs_6);
        mMimariProjectContainer->addStyleClass ("boxShadow");

        this->loadMimariProjectBtn (projectView);

    }


    // Statik Proje Görüntüleme
    {
        mStatikProjectContainer = mProjectHeaderContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mStatikProjectContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                                Bootstrap::Grid::Medium::col_md_3+
                                                Bootstrap::Grid::Small::col_sm_3+
                                                Bootstrap::Grid::ExtraSmall::col_xs_6);
        mStatikProjectContainer->addStyleClass ("boxShadow");

        auto mimariDwgLayout = mStatikProjectContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
        mimariDwgLayout->addWidget (cpp14::make_unique<WText>("<b>Statik Proje</b>"),0,AlignmentFlag::Center)->setAttributeValue (Style::style,Style::font::family::dosis);
        auto mimariProjeLinkImage = mimariDwgLayout->addWidget (cpp14::make_unique<WImage>(WLink("icon/staticicon.png")),0,AlignmentFlag::Center);
        mimariProjeLinkImage->decorationStyle ().setCursor (Cursor::PointingHand);
        mimariProjeLinkImage->clicked ().connect ([=](){
            //            projectView->initMimariProject ();
        });

        {
            mStatikOnayTextContainer = mimariDwgLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
            mStatikOnayTextContainer->addStyleClass ("boxShadow");
            mStatikOnayTextContainer->setAttributeValue (Style::style,Style::background::color::color (false ? Style::color::Green::SeaGreen : Style::color::Red::FireBrick));
            mStatikOnayText = mStatikOnayTextContainer->addWidget (cpp14::make_unique<WText>(false ? "<b>Onaylandı</b>" : "<b><i>Beklemede</i></b>"));
            mStatikOnayText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);

            std::string personelName = "";

            auto personelAssignmedContainer = mimariDwgLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
            personelAssignmedContainer->addStyleClass ("boxShadow");
            personelAssignmedContainer->setPositionScheme (PositionScheme::Relative);

            if( personelName.empty () ){
                personelAssignmedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::FloralWhite));
                auto personelAssignmedText = personelAssignmedContainer->addWidget (cpp14::make_unique<WText>( "Proje Boşta" ));
                personelAssignmedText->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black)+Style::font::family::dosis);
            }else{
                personelAssignmedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray));
                auto personelAssignmedText = personelAssignmedContainer->addWidget (cpp14::make_unique<WText>( "<i>"+personelName+"</i>" ));
                personelAssignmedText->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black)+Style::font::family::dosis);
            }
        }
    }




    projectView->RequestMimariProje ().connect ([=]( const bsoncxx::oid& projeOid ){
        auto url = SerikBLDCore::Imar::MimariProjeManager::downloadFileWeb (projeOid.to_string ().c_str ());
        this->doJavaScript ("window.open('"+url+"','_blank');");
    });



    projectView->insertDuzeltmeClicked ().connect ([=](const bsoncxx::oid& projeOid , const std::string& duzeltme ){

        SerikBLDCore::Imar::DuzeltmeLog duzeltmeLog;
        duzeltmeLog.setDuzeltme (duzeltme);
        duzeltmeLog.setMimariProjeOid (projeOid);
        duzeltmeLog.setEkleyen (mUser->AdSoyad ());
        duzeltmeLog.setJulianDay (WDate::currentDate ().toJulianDay ());
        duzeltmeLog.setSecEpoch (QDateTime::currentSecsSinceEpoch ());


        auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (duzeltmeLog);
        if( ins.size () ){
            SerikBLDCore::Imar::MimariLog Filter;
            Filter.setMimariProjeOid (projeOid);
            SerikBLDCore::Imar::MimariLogManager::UpdateList (Filter);
        }
    });


    projectView->insertAciklama ().connect ([=](const bsoncxx::oid& projeOid , const std::string& aciklama ){

        SerikBLDCore::Imar::AciklamaLog aciklamaLog;
        aciklamaLog.setAciklama (aciklama);
        aciklamaLog.setMimariProjeOid (projeOid);
        aciklamaLog.setEkleyen (mUser->AdSoyad ());
        aciklamaLog.setJulianDay (WDate::currentDate ().toJulianDay ());
        aciklamaLog.setSecEpoch (QDateTime::currentSecsSinceEpoch ());


        auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (aciklamaLog);
        if( ins.size () ){
            SerikBLDCore::Imar::MimariLog Filter;
            Filter.setMimariProjeOid (projeOid);
            SerikBLDCore::Imar::MimariLogManager::UpdateList (Filter);
        }
    });


    projectView->insertDosya ().connect ([=](const bsoncxx::oid& projeOid , const std::string& filePath ){

        auto fileOid = SerikBLDCore::Imar::MimariLogManager::uploadfile (filePath.c_str ());
        SerikBLDCore::Imar::DosyaLog dosyaLog;
        dosyaLog.setFileOid (fileOid.get_oid ().value);
        dosyaLog.setMimariProjeOid (projeOid);
        dosyaLog.setEkleyen (mUser->AdSoyad ());
        dosyaLog.setJulianDay (WDate::currentDate ().toJulianDay ());
        dosyaLog.setSecEpoch (QDateTime::currentSecsSinceEpoch ());


        auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (dosyaLog);
        if( ins.size () ){
            SerikBLDCore::Imar::MimariLog Filter;
            Filter.setMimariProjeOid (projeOid);
            SerikBLDCore::Imar::MimariLogManager::UpdateList (Filter);
        }
    });


    projectView->MimariProjeOnayla ().connect ([=](const bsoncxx::oid& projeOid , const bool& projeOnay ){

        if( mUser->oid ().value ().to_string () != projectView->assignedPersonelOid () ){
            this->showPopUpMessage ("Bu Projenin Onaylama İşlemini Sadece " + projectView->assignedPersonelName () + " Verebilir.","err");
            return;
        }else{
            SerikBLDCore::Imar::MimariProje::MimariProje filter;
            filter.setOid (projectView->oid ().value ().to_string ());
            filter.setMimariProjeOnay (projeOnay);

            auto upt = SerikBLDCore::Imar::MimariProjeManager::UpdateItem (filter);

            if( upt ){

                if( projeOnay ){
                    this->showPopUpMessage ("Mimari Proje Onaylandı","msg");
                    mMimariOnayText->setText ("<b>Onaylandı</b>");
                    mMimariOnayTextContainer->setAttributeValue (Style::style,Style::background::color::color ( Style::color::Green::SeaGreen) );
                    projectView->setMimariProjeOnay (true);

                }else{
                    this->showPopUpMessage ("Mimari Proje Onayı Kaldırıldı","msg");
                    mMimariOnayTextContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::FireBrick));
                    mMimariOnayText->setText ("<b><i>Beklemede</i></b>");
                    projectView->setMimariProjeOnay (false);
                }

                SerikBLDCore::Imar::IslemLog islemLog;
                islemLog.setLog (projeOnay ? "Mimari Proje Onaylandı" : "Mimari Projenin Onayı Kaldırıldı");
                islemLog.setMimariProjeOid (projectView->oid ().value ());
                islemLog.setEkleyen (mUser->AdSoyad ());
                islemLog.setJulianDay (WDate::currentDate ().toJulianDay ());
                islemLog.setSecEpoch (QDateTime::currentSecsSinceEpoch ());


                auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (islemLog);
                if( ins.size () ){
                    SerikBLDCore::Imar::MimariLog Filter;
                    Filter.setMimariProjeOid (projeOid);
                    SerikBLDCore::Imar::MimariLogManager::UpdateList (Filter);
                }

            }else{
                this->showPopUpMessage ("Mimari Proje Onaylanmadı!\nLütfen Sayfayı Yenileyiniz","err");
            }
        }



    });


    projectView->RequestLog ().connect ([=]( const bsoncxx::oid& _projectOid , const int& logFilter ){
        mCurrentLogFilterIndex = logFilter;
        SerikBLDCore::Imar::MimariLog Filter;
        Filter.setMimariProjeOid (_projectOid);
        SerikBLDCore::Imar::MimariLogManager::UpdateList (Filter);
    });



}

void Kurumsal::v2::MimariProjeManagerPage::loadMimariProjectBtn(Comman::MimarProjeView* projectView)
{

    mMimariProjectContainer->clear ();
    auto mimariDwgLayout = mMimariProjectContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
    mimariDwgLayout->addWidget (cpp14::make_unique<WText>("<b>Mimari Proje</b>"),0,AlignmentFlag::Center)->setAttributeValue (Style::style,Style::font::family::dosis);
    auto mimariProjeLinkImage = mimariDwgLayout->addWidget (cpp14::make_unique<WImage>(WLink("icon/dwgicon.png")),0,AlignmentFlag::Center);
    mimariProjeLinkImage->decorationStyle ().setCursor (Cursor::PointingHand);
    mimariProjeLinkImage->clicked ().connect ([=](){
        projectView->initMimariProject ();
    });

    {
        mMimariOnayTextContainer = mimariDwgLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
        mMimariOnayTextContainer->addStyleClass ("boxShadow");
        mMimariOnayTextContainer->setAttributeValue (Style::style,Style::background::color::color (projectView->mimariProjeOnay () ? Style::color::Green::SeaGreen : Style::color::Red::FireBrick));
        mMimariOnayText = mMimariOnayTextContainer->addWidget (cpp14::make_unique<WText>(projectView->mimariProjeOnay () ? "<b>Onaylandı</b>" : "<b><i>Beklemede</i></b>"));
        mMimariOnayText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);

        std::string personelName = projectView->assignedPersonelName ();

        auto personelAssignmedContainer = mimariDwgLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
        personelAssignmedContainer->addStyleClass ("boxShadow");
        personelAssignmedContainer->setPositionScheme (PositionScheme::Relative);


        if( this->mUser->oid ().value ().to_string () == projectView->assignedPersonelOid () ){
            auto deAssignPersonelContainer = personelAssignmedContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            deAssignPersonelContainer->setPositionScheme (PositionScheme::Absolute);
            deAssignPersonelContainer->setOffsets (0,Side::Top|Side::Right);
            deAssignPersonelContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::DeepPink)+Style::color::color (Style::color::White::Snow));
            deAssignPersonelContainer->addWidget (cpp14::make_unique<WText>("Bırak"))->setAttributeValue (Style::style,Style::font::family::dosis);
            deAssignPersonelContainer->clicked ().connect ([=](){
                SerikBLDCore::Imar::MimariProje::MimariProje filter;
                filter.setOid (projectView->oid ().value ().to_string ());

                auto upt1 = SerikBLDCore::Imar::MimariProjeManager::removeField (filter,SerikBLDCore::Imar::MimariProje::MimariProje::keyAssignedMimariOid);

                if( upt1 ){
                    auto upt = SerikBLDCore::Imar::MimariProjeManager::removeField (filter,SerikBLDCore::Imar::MimariProje::MimariProje::keyAssignedMimariName);
                    if( upt ){
                        this->showPopUpMessage ("Bu Projeyi Bıraktınız","msg");
                        SerikBLDCore::Imar::IslemLog islemLog;
                        islemLog.setLog ("Bu Projeyi, " + this->mUser->AdSoyad () + " Bıraktı");
                        islemLog.setMimariProjeOid (projectView->oid ().value ());
                        islemLog.setEkleyen (mUser->AdSoyad ());
                        islemLog.setJulianDay (WDate::currentDate ().toJulianDay ());
                        islemLog.setSecEpoch (QDateTime::currentSecsSinceEpoch ());

                        auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (islemLog);
                        if( ins.size () ){
                            SerikBLDCore::Imar::MimariLog Filter;
                            Filter.setMimariProjeOid (projectView->oid ().value ());
                            SerikBLDCore::Imar::MimariLogManager::UpdateList (Filter);
                            projectView->deAssignPersonel (this->mUser->oid ().value ());
                            this->loadMimariProjectBtn (projectView);
                            projectView->initMimariProject ();
                        }

                    }else{
                        this->showPopUpMessage ("Projeden Ayrılamadınız!\nLütfen Sayfayı Yenileyiniz","err");
                    }
                }else{
                    this->showPopUpMessage ("Projeden Ayrılamadınız!\nLütfen Sayfayı Yenileyiniz","err");
                }
            });
            deAssignPersonelContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        }


        if( personelName.empty () ){
            personelAssignmedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::FloralWhite));
            auto personelAssignmedText = personelAssignmedContainer->addWidget (cpp14::make_unique<WText>( "Projeyi Al" ));
            personelAssignmedText->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black)+Style::font::family::dosis);
            personelAssignmedContainer->decorationStyle ().setCursor (Cursor::PointingHand);
            personelAssignmedContainer->clicked ().connect ([=](){
                SerikBLDCore::Imar::MimariProje::MimariProje filter;
                filter.setOid (projectView->oid ().value ().to_string ());
                filter.assignPersonel (this->mUser->oid ().value (),this->mUser->AdSoyad ());

                auto upt = SerikBLDCore::Imar::MimariProjeManager::UpdateItem (filter);

                if( upt ){
                    this->showPopUpMessage ("Bu Projeye Dahil Oldunuz","msg");

                    SerikBLDCore::Imar::IslemLog islemLog;
                    islemLog.setLog ("Bu Projeyi, " + this->mUser->AdSoyad () + " Aldı");
                    islemLog.setMimariProjeOid (projectView->oid ().value ());
                    islemLog.setEkleyen (mUser->AdSoyad ());
                    islemLog.setJulianDay (WDate::currentDate ().toJulianDay ());
                    islemLog.setSecEpoch (QDateTime::currentSecsSinceEpoch ());


                    auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (islemLog);
                    if( ins.size () ){
                        SerikBLDCore::Imar::MimariLog Filter;
                        Filter.setMimariProjeOid (projectView->oid ().value ());
                        SerikBLDCore::Imar::MimariLogManager::UpdateList (Filter);
                        projectView->assignPersonel (this->mUser->oid ().value (),this->mUser->AdSoyad ());
                        this->loadMimariProjectBtn (projectView);
                        projectView->initMimariProject ();
                    }else{
                        this->showPopUpMessage ("Projeye Dahil Olamadınız!\nLütfen Sayfayı Yenileyiniz","err");
                    }

                }else{
                    this->showPopUpMessage ("Projeye Dahil Olamadınız!\nLütfen Sayfayı Yenileyiniz","err");
                }
            });
        }else{
            personelAssignmedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray));
            auto personelAssignmedText = personelAssignmedContainer->addWidget (cpp14::make_unique<WText>( "<i>"+personelName+"</i>" ));
            personelAssignmedText->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black)+Style::font::family::dosis);
        }
    }
}
