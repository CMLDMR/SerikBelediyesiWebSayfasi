#include "mainprojeview.h"



v2::MainProjeView::MainProjeView(MainProje &mainProje, SerikBLDCore::DB *_db, SerikBLDCore::TC *_mtcUser)
    :ContainerWidget (mainProje.adi ()),
      SerikBLDCore::Imar::MimariProje::MainProje (mainProje),
      SerikBLDCore::Imar::BaseProjeManager (_db),
      SerikBLDCore::Imar::MimariLogManager (_db),
      mCurrentLogFilterIndex(0),
      mSelectedProjectOid(mainProje.oid ().value ().to_string ()),
      mTCUser(_mtcUser)
{
    Header ()->setMargin (15,Side::Top|Side::Bottom);
}

void v2::MainProjeView::initHeader()
{
    Header ()->clear ();
    auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    container->addStyleClass (Bootstrap::Grid::col_full_12+"boxShadow");

    auto gridLayout = container->setLayout (cpp14::make_unique<WGridLayout>());


    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,0,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Ada</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,1,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Parsel</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,2,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Mahalle</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,3,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Başvuru Tarihi</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);

    auto yeniProjeBtn = gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,4,2,0,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WPushButton>(WString("Proje Ekle")));
    yeniProjeBtn->addStyleClass (Bootstrap::Button::Primary);
    yeniProjeBtn->clicked ().connect ([=](){

        auto mDialog = createDialog ("Yeni Proje Ekle");

        auto projeTipiComboBox = mDialog->contents ()->addWidget (cpp14::make_unique<WComboBox>());
        projeTipiComboBox->addItem ("Mimari");      //0
        projeTipiComboBox->addItem ("Statik");      //1
        projeTipiComboBox->addItem ("Elektrik");    //2
        projeTipiComboBox->addItem ("Mekanik");     //3
        projeTipiComboBox->addItem ("Ruhsat");      //4
        projeTipiComboBox->addItem ("İskele");      //5
        projeTipiComboBox->addItem ("İskan");       //6
        projeTipiComboBox->addItem ("Zemin Etüdü"); //7
        projeTipiComboBox->addItem ("Asansör");     //8

        auto ekleBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Başla"));
        ekleBtn->addStyleClass (Bootstrap::Button::Primary);
        ekleBtn->clicked ().connect ([=](){
            mfilePathTitleMap.clear ();
            this->addNewProje (projeTipiComboBox->currentIndex ());
            this->removeDialog (mDialog);
        });

    });





    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,0,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (this->ada ())))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,1,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (this->parsel ())))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,2,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (this->mahalle ())))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,3,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (WDate::fromJulianDay (this->basvuruJulianDay ()).toString ("dd/MMMM/yyyy"))))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,4,AlignmentFlag::Justify);

    SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (this->oid ().value ()));
    SerikBLDCore::Imar::BaseProjeManager::UpdateList (SerikBLDCore::Imar::MimariProje::BaseProject().setMainProjeOid (this->oid ().value ()));
}

void v2::MainProjeView::addNewProje(const int &projeType)
{
    bool projectExist = false;
    std::string projectAddError = "";
    {
        auto list = SerikBLDCore::Imar::BaseProjeManager::UpdateList (SerikBLDCore::Imar::MimariProje::BaseProject().setMainProjeOid (this->oid ().value ()));
        for( auto item : list ){
            if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Mimari && projeType == 0 ){
                projectExist = true;
                projectAddError = "Mimari Proje var. Var Olan Projeyi Güncelleyiniz!";
            }

            if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Statik && projeType == 1 ){
                projectExist = true;
                projectAddError = "Statik Proje var. Var Olan Projeyi Güncelleyiniz!";
            }

            if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Elektrik && projeType == 2 ){
                projectExist = true;
                projectAddError = "Elektrik Proje var. Var Olan Projeyi Güncelleyiniz!";
            }

            if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Mekanik && projeType == 3 ){
                projectExist = true;
                projectAddError = "Mekanik Proje var. Var Olan Projeyi Güncelleyiniz!";
            }

            if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Ruhsat && projeType == 4 ){
                projectExist = true;
                projectAddError = "Ruhsat var. Var Olan Projeyi Güncelleyiniz!";
            }

            if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Iskele && projeType == 5 ){
                projectExist = true;
                projectAddError = "İskele Projesi var. Var Olan Projeyi Güncelleyiniz!";
            }

            if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Iskan && projeType == 6 ){
                projectExist = true;
                projectAddError = "İskan Projesi var. Var Olan Projeyi Güncelleyiniz!";
            }

            if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::ZeminEtudu && projeType == 7 ){
                projectExist = true;
                projectAddError = "Zemin Etüdü var. Var Olan Projeyi Güncelleyiniz!";
            }

            if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Asansor && projeType == 8 ){
                projectExist = true;
                projectAddError = "Asansör Projesi var. Var Olan Projeyi Güncelleyiniz!";
            }
        }
    }

    if( projectExist ){
        this->showPopUpMessage (projectAddError,"msg");
        return;
    }



    Footer ()->clear ();

    for( auto item : mfilePathTitleMap ){
        auto projeDosyaContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        projeDosyaContainer->addStyleClass (Bootstrap::Grid::col_full_12+"boxShadow boxRadius");
        projeDosyaContainer->setMargin (5,Side::Top|Side::Bottom);
        projeDosyaContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (150,255),
                                                                                            this->getRandom (150,255),
                                                                                            this->getRandom (150,255)));

        auto container = projeDosyaContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::container_fluid);
        container->setPositionScheme (PositionScheme::Relative);

        auto delContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        delContainer->addStyleClass ("boxRadius boxShadow");
        delContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)+
                                         Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);
        delContainer->setPositionScheme (PositionScheme::Absolute);
        delContainer->setOffsets (0,Side::Top|Side::Right);
        delContainer->addWidget (cpp14::make_unique<WText>("<b> X </b>"));
        delContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        delContainer->setWidth (20);
        delContainer->clicked ().connect ([=](){
            mfilePathTitleMap.erase (item.first);
            this->addNewProje (projeType);
        });

        {
            auto text = container->addWidget (cpp14::make_unique<WText>(item.first));
            text->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);
        }
        container->addWidget (cpp14::make_unique<WBreak>());
        {
            auto text = container->addWidget (cpp14::make_unique<WText>(item.second));
            text->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);
        }
    }


    auto projeAdiContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    projeAdiContainer->setMargin (15,Side::Top|Side::Bottom);
    auto projeAdiLabel = projeAdiContainer->addWidget (cpp14::make_unique<WLabel>(""));
    projeAdiLabel->addStyleClass (Bootstrap::Label::info);
    projeAdiLabel->setAttributeValue (Style::style,Style::font::size::s18px+Style::font::family::dosis+Style::font::weight::lighter);


    switch (projeType) {
    case 0:
        projeAdiLabel->setText ("Mimari Proje Dosyaları/Bilgileri Ekle");
        break;
    case 1:
        projeAdiLabel->setText ("Statik Proje Dosyaları/Bilgileri Ekle");
        break;
    case 2:
        projeAdiLabel->setText ("Elektrik Proje Dosyaları/Bilgileri Ekle");
        break;
    case 3:
        projeAdiLabel->setText ("Mekanik Proje Dosyaları/Bilgileri Ekle");
        break;
    case 4:
        projeAdiLabel->setText ("Ruhsat Dosyaları/Bilgileri Ekle");
        break;
    case 5:
        projeAdiLabel->setText ("İskele Proje Dosyaları/Bilgileri Ekle");
        break;
    case 6:
        projeAdiLabel->setText ("Iskan Dosyaları/Bilgileri Ekle");
        break;
    case 7:
        projeAdiLabel->setText ("Zemin Etüdü Dosyaları/Bilgileri Ekle");
        break;
    case 8:
        projeAdiLabel->setText ("Asansör Proje Dosyaları/Bilgileri Ekle");
        break;
    default:
        break;
    }







    auto projeAdiLineEditContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    projeAdiLineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_7+
                                              Bootstrap::Grid::Medium::col_md_7+
                                              Bootstrap::Grid::Small::col_sm_6+
                                              Bootstrap::Grid::ExtraSmall::col_xs_6);
    projeAdiLineEditContainer->setMargin (15,Side::Top|Side::Bottom);
    auto projeAdiLineEdit = projeAdiLineEditContainer->addWidget (cpp14::make_unique<WLineEdit>());
    projeAdiLineEdit->setPlaceholderText ("Proje Dosyasının Adını Giriniz ( Örnek: Mimari, Zemin Etüdü, Elektrik ... )");



    auto fileUploaderContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    fileUploaderContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                          Bootstrap::Grid::Medium::col_md_5+
                                          Bootstrap::Grid::Small::col_sm_6+
                                          Bootstrap::Grid::ExtraSmall::col_xs_6);
    fileUploaderContainer->setContentAlignment (AlignmentFlag::Center);
    auto fileUploader = fileUploaderContainer->addWidget (cpp14::make_unique<FileUploaderWidget>("Dosya Yükle"));



    auto ekleBtnContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    ekleBtnContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    ekleBtnContainer->setMargin (10,Side::Top|Side::Bottom);

    auto ekleBtn = ekleBtnContainer->addWidget (cpp14::make_unique<WPushButton>("Projeyi Ekle"));

    ekleBtn->clicked ().connect ([=](){
        if( !fileUploader->isUploaded () ){
            this->showPopUpMessage ("Dosya Yüklemediniz","err");
            return;
        }
        if( projeAdiLineEdit->text ().toUTF8 ().size () < 5 ){
            this->showPopUpMessage ("Proje Adı Yeterli Değil","err");
            return;
        }

        bool exist_ = false;
        for( auto _item : mfilePathTitleMap ){
            if( _item.first == projeAdiLineEdit->text ().toUTF8 () ){
                exist_ = true;
            }
        }

        if( !exist_ ){
            mfilePathTitleMap.insert (std::pair<std::string,std::string>(projeAdiLineEdit->text ().toUTF8 (),fileUploader->fileLocation ().toUtf8 ()));
            this->addNewProje (projeType);
        }else{
            this->showPopUpMessage ("Bu Dosya Adı Zaten Var!","msg");
        }

    });

    auto saveBtnContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    saveBtnContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    saveBtnContainer->setMargin (5,Side::Top|Side::Bottom);
    auto saveBtn = saveBtnContainer->addWidget (cpp14::make_unique<WPushButton>("Projeyi Kaydet"));
    saveBtn->addStyleClass (Bootstrap::Button::Primary);

    saveBtn->clicked ().connect ([=](){

        switch (projeType) {
        case 0:
            //            Mimari Proje
        {
            SerikBLDCore::Imar::MimariProje::MimariProje proje;
            for( auto item : mfilePathTitleMap ){
                auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (item.second.c_str ());

                SerikBLDCore::Imar::MimariProje::FileProject fileProject;
                fileProject.setOnay (false);
                fileProject.setFileOid (val.view().get_oid ().value);
                fileProject.setProjeAdi (item.first);

                proje.addProje (fileProject);
            }
            proje.setMainProjeOid (this->oid ().value ());
            proje.setOnay (false);

            auto ins = SerikBLDCore::Imar::BaseProjeManager::InsertItem (proje);
            if( ins.size () ){
                this->showPopUpMessage ("Mimari Proje Eklendi");


                SerikBLDCore::Imar::IslemLog log;
                log.setEkleyen (this->mTCUser->AdSoyad ().toStdString ());
                log.setJulianDay (WDate::currentDate ().toJulianDay ());
                log.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                log.setProjeOid (bsoncxx::oid{ins});
                log.setLog ("Mimari Proje Dosyası Eklendi");

                auto logIns = SerikBLDCore::Imar::MimariLogManager::InsertItem (log);
                if( logIns.size () ){
                    SerikBLDCore::Imar::MimariLog filter;
                    filter.setProjeOid (this->oid ().value ());
                    SerikBLDCore::Imar::MimariLogManager::UpdateList (filter);
                }
            }else{
                this->showPopUpMessage ("Mimari Proje Eklenemedi. Bir Hata Oluştu");
            }

        }
            break;
        case 1:
            //            Statik Proje
        {
            SerikBLDCore::Imar::MimariProje::StatikProje proje;
            for( auto item : mfilePathTitleMap ){
                auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (item.second.c_str ());

                SerikBLDCore::Imar::MimariProje::FileProject fileProject;
                fileProject.setOnay (false);
                fileProject.setFileOid (val.view().get_oid ().value);
                fileProject.setProjeAdi (item.first);

                proje.addProje (fileProject);
            }
            proje.setMainProjeOid (this->oid ().value ());
            proje.setOnay (false);

            auto ins = SerikBLDCore::Imar::BaseProjeManager::InsertItem (proje);
            if( ins.size () ){
                this->showPopUpMessage ("Statik Proje Eklendi");

                SerikBLDCore::Imar::IslemLog log;
                log.setEkleyen (this->mTCUser->AdSoyad ().toStdString ());
                log.setJulianDay (WDate::currentDate ().toJulianDay ());
                log.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                log.setProjeOid (bsoncxx::oid{ins});
                log.setLog ("Statik Proje Dosyası Eklendi");

                auto logIns = SerikBLDCore::Imar::MimariLogManager::InsertItem (log);
                if( logIns.size () ){
                    SerikBLDCore::Imar::MimariLog filter;
                    filter.setProjeOid (this->oid ().value ());
                    SerikBLDCore::Imar::MimariLogManager::UpdateList (filter);
                }
            }else{
                this->showPopUpMessage ("Statik Proje Eklenemedi. Bir Hata Oluştu");
            }

        }
            break;
        case 2:
            //            Eleketrik Proje
        {
            SerikBLDCore::Imar::MimariProje::ElektrikProje proje;
            for( auto item : mfilePathTitleMap ){
                auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (item.second.c_str ());

                SerikBLDCore::Imar::MimariProje::FileProject fileProject;
                fileProject.setOnay (false);
                fileProject.setFileOid (val.view().get_oid ().value);
                fileProject.setProjeAdi (item.first);

                proje.addProje (fileProject);
            }
            proje.setMainProjeOid (this->oid ().value ());
            proje.setOnay (false);

            auto ins = SerikBLDCore::Imar::BaseProjeManager::InsertItem (proje);
            if( ins.size () ){
                this->showPopUpMessage ("Elektrik Proje Eklendi");

                SerikBLDCore::Imar::IslemLog log;
                log.setEkleyen (this->mTCUser->AdSoyad ().toStdString ());
                log.setJulianDay (WDate::currentDate ().toJulianDay ());
                log.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                log.setProjeOid (bsoncxx::oid{ins});
                log.setLog ("Elektrik Proje Dosyası Eklendi");

                auto logIns = SerikBLDCore::Imar::MimariLogManager::InsertItem (log);
                if( logIns.size () ){
                    SerikBLDCore::Imar::MimariLog filter;
                    filter.setProjeOid (this->oid ().value ());
                    SerikBLDCore::Imar::MimariLogManager::UpdateList (filter);
                }
            }else{
                this->showPopUpMessage ("Elektrik Proje Eklenemedi. Bir Hata Oluştu");
            }

        }
            break;
        case 3:
            //            Mekanik Proje
        {
            SerikBLDCore::Imar::MimariProje::MechanicProje proje;
            for( auto item : mfilePathTitleMap ){
                auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (item.second.c_str ());

                SerikBLDCore::Imar::MimariProje::FileProject fileProject;
                fileProject.setOnay (false);
                fileProject.setFileOid (val.view().get_oid ().value);
                fileProject.setProjeAdi (item.first);

                proje.addProje (fileProject);
            }
            proje.setMainProjeOid (this->oid ().value ());
            proje.setOnay (false);

            auto ins = SerikBLDCore::Imar::BaseProjeManager::InsertItem (proje);
            if( ins.size () ){
                this->showPopUpMessage ("Mekanik Proje Eklendi");

                SerikBLDCore::Imar::IslemLog log;
                log.setEkleyen (this->mTCUser->AdSoyad ().toStdString ());
                log.setJulianDay (WDate::currentDate ().toJulianDay ());
                log.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                log.setProjeOid (bsoncxx::oid{ins});
                log.setLog ("Mekanik Proje Dosyası Eklendi");

                auto logIns = SerikBLDCore::Imar::MimariLogManager::InsertItem (log);
                if( logIns.size () ){
                    SerikBLDCore::Imar::MimariLog filter;
                    filter.setProjeOid (this->oid ().value ());
                    SerikBLDCore::Imar::MimariLogManager::UpdateList (filter);
                }
            }else{
                this->showPopUpMessage ("Mekanik Proje Eklenemedi. Bir Hata Oluştu");
            }

        }
            break;
        case 4:
            //            Ruhsat
        {
            SerikBLDCore::Imar::MimariProje::RuhsatProje proje;
            for( auto item : mfilePathTitleMap ){
                auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (item.second.c_str ());

                SerikBLDCore::Imar::MimariProje::FileProject fileProject;
                fileProject.setOnay (false);
                fileProject.setFileOid (val.view().get_oid ().value);
                fileProject.setProjeAdi (item.first);

                proje.addProje (fileProject);
            }
            proje.setMainProjeOid (this->oid ().value ());
            proje.setOnay (false);

            auto ins = SerikBLDCore::Imar::BaseProjeManager::InsertItem (proje);
            if( ins.size () ){
                this->showPopUpMessage ("Ruhsat Eklendi");

                SerikBLDCore::Imar::IslemLog log;
                log.setEkleyen (this->mTCUser->AdSoyad ().toStdString ());
                log.setJulianDay (WDate::currentDate ().toJulianDay ());
                log.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                log.setProjeOid (bsoncxx::oid{ins});
                log.setLog ("Ruhsat Dosyası Eklendi");

                auto logIns = SerikBLDCore::Imar::MimariLogManager::InsertItem (log);
                if( logIns.size () ){
                    SerikBLDCore::Imar::MimariLog filter;
                    filter.setProjeOid (this->oid ().value ());
                    SerikBLDCore::Imar::MimariLogManager::UpdateList (filter);
                }
            }else{
                this->showPopUpMessage ("Ruhsat Eklenemedi. Bir Hata Oluştu");
            }

        }
            break;
        case 5:
            //            Iskele
        {
            SerikBLDCore::Imar::MimariProje::IskeleProje proje;
            for( auto item : mfilePathTitleMap ){
                auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (item.second.c_str ());

                SerikBLDCore::Imar::MimariProje::FileProject fileProject;
                fileProject.setOnay (false);
                fileProject.setFileOid (val.view().get_oid ().value);
                fileProject.setProjeAdi (item.first);

                proje.addProje (fileProject);
            }
            proje.setMainProjeOid (this->oid ().value ());
            proje.setOnay (false);

            auto ins = SerikBLDCore::Imar::BaseProjeManager::InsertItem (proje);
            if( ins.size () ){
                this->showPopUpMessage ("İskele Projesi Eklendi");

                SerikBLDCore::Imar::IslemLog log;
                log.setEkleyen (this->mTCUser->AdSoyad ().toStdString ());
                log.setJulianDay (WDate::currentDate ().toJulianDay ());
                log.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                log.setProjeOid (bsoncxx::oid{ins});
                log.setLog ("İskele Projesi Dosyası Eklendi");

                auto logIns = SerikBLDCore::Imar::MimariLogManager::InsertItem (log);
                if( logIns.size () ){
                    SerikBLDCore::Imar::MimariLog filter;
                    filter.setProjeOid (this->oid ().value ());
                    SerikBLDCore::Imar::MimariLogManager::UpdateList (filter);
                }
            }else{
                this->showPopUpMessage ("İskele Projesi Eklenemedi. Bir Hata Oluştu");
            }

        }
            break;
        case 6:
            //            Iskan
        {
            SerikBLDCore::Imar::MimariProje::IskanProje proje;
            for( auto item : mfilePathTitleMap ){
                auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (item.second.c_str ());

                SerikBLDCore::Imar::MimariProje::FileProject fileProject;
                fileProject.setOnay (false);
                fileProject.setFileOid (val.view().get_oid ().value);
                fileProject.setProjeAdi (item.first);

                proje.addProje (fileProject);
            }
            proje.setMainProjeOid (this->oid ().value ());
            proje.setOnay (false);

            auto ins = SerikBLDCore::Imar::BaseProjeManager::InsertItem (proje);
            if( ins.size () ){
                this->showPopUpMessage ("Iskan Projesi Eklendi");

                SerikBLDCore::Imar::IslemLog log;
                log.setEkleyen (this->mTCUser->AdSoyad ().toStdString ());
                log.setJulianDay (WDate::currentDate ().toJulianDay ());
                log.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                log.setProjeOid (bsoncxx::oid{ins});
                log.setLog ("Iskan Projesi Dosyası Eklendi");

                auto logIns = SerikBLDCore::Imar::MimariLogManager::InsertItem (log);
                if( logIns.size () ){
                    SerikBLDCore::Imar::MimariLog filter;
                    filter.setProjeOid (this->oid ().value ());
                    SerikBLDCore::Imar::MimariLogManager::UpdateList (filter);
                }
            }else{
                this->showPopUpMessage ("Iskan Projesi Eklenemedi. Bir Hata Oluştu");
            }

        }
            break;
        case 7:
            //            Zemin Etüdü
        {
            SerikBLDCore::Imar::MimariProje::ZeminEtudu proje;
            for( auto item : mfilePathTitleMap ){
                auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (item.second.c_str ());

                SerikBLDCore::Imar::MimariProje::FileProject fileProject;
                fileProject.setOnay (false);
                fileProject.setFileOid (val.view().get_oid ().value);
                fileProject.setProjeAdi (item.first);

                proje.addProje (fileProject);
            }
            proje.setMainProjeOid (this->oid ().value ());
            proje.setOnay (false);

            auto ins = SerikBLDCore::Imar::BaseProjeManager::InsertItem (proje);
            if( ins.size () ){
                this->showPopUpMessage ("Zemin Etüdü Eklendi");

                SerikBLDCore::Imar::IslemLog log;
                log.setEkleyen (this->mTCUser->AdSoyad ().toStdString ());
                log.setJulianDay (WDate::currentDate ().toJulianDay ());
                log.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                log.setProjeOid (bsoncxx::oid{ins});
                log.setLog ("Zemin Etüdü Dosyası Eklendi");

                auto logIns = SerikBLDCore::Imar::MimariLogManager::InsertItem (log);
                if( logIns.size () ){
                    SerikBLDCore::Imar::MimariLog filter;
                    filter.setProjeOid (this->oid ().value ());
                    SerikBLDCore::Imar::MimariLogManager::UpdateList (filter);
                }
            }else{
                this->showPopUpMessage ("Zemin Etüdü Eklenemedi. Bir Hata Oluştu");
            }

        }
            break;
        case 8:
            //            Asansor
        {
            SerikBLDCore::Imar::MimariProje::AsansorProje proje;
            for( auto item : mfilePathTitleMap ){
                auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (item.second.c_str ());

                SerikBLDCore::Imar::MimariProje::FileProject fileProject;
                fileProject.setOnay (false);
                fileProject.setFileOid (val.view().get_oid ().value);
                fileProject.setProjeAdi (item.first);

                proje.addProje (fileProject);
            }
            proje.setMainProjeOid (this->oid ().value ());
            proje.setOnay (false);

            auto ins = SerikBLDCore::Imar::BaseProjeManager::InsertItem (proje);
            if( ins.size () ){
                this->showPopUpMessage ("Asansör Eklendi");

                SerikBLDCore::Imar::IslemLog log;
                log.setEkleyen (this->mTCUser->AdSoyad ().toStdString ());
                log.setJulianDay (WDate::currentDate ().toJulianDay ());
                log.setSecEpoch (QDateTime::currentSecsSinceEpoch ());
                log.setProjeOid (bsoncxx::oid{ins});
                log.setLog ("Asansör Dosyası Eklendi");

                auto logIns = SerikBLDCore::Imar::MimariLogManager::InsertItem (log);
                if( logIns.size () ){
                    SerikBLDCore::Imar::MimariLog filter;
                    filter.setProjeOid (this->oid ().value ());
                    SerikBLDCore::Imar::MimariLogManager::UpdateList (filter);
                }
            }else{
                this->showPopUpMessage ("Asansör Eklenemedi. Bir Hata Oluştu");
            }

        }
            break;
        default:
            break;
        }

        SerikBLDCore::Imar::BaseProjeManager::UpdateList (SerikBLDCore::Imar::MimariProje::BaseProject().setMainProjeOid (this->oid ().value ()));


    });




}




void v2::MainProjeView::onList(const QVector<SerikBLDCore::Imar::MimariProje::BaseProject> *mlist)
{
    Content ()->clear ();
    for( auto item : *mlist ){

        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->setHeight (180);
        container->setMargin (10,Side::Top|Side::Bottom);
        container->addStyleClass ("boxShadow");
        container->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
        container->clicked ().connect ([=](){
            this->loadProject (item.oid ().value ());
        });

        WVBoxLayout* layout = container->setLayout (cpp14::make_unique<WVBoxLayout>());
        WContainerWidget* imgContainer = layout->addWidget (cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);


        if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Mimari ){
            auto mimariProject = item.toMimariProje ();
            imgContainer->setAttributeValue (Style::style,Style::background::url (mimariProject.iconPath ())+Style::background::size::contain+
                                             Style::background::repeat::norepeat+
                                             Style::background::position::center_center);
            layout->addWidget (cpp14::make_unique<WText>("Mimari Proje"),0,AlignmentFlag::Bottom)->setAttributeValue (Style::style,Style::font::family::dosis);
        }

        if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Statik ){
            auto mimariProject = item.toStatikProje ();
            imgContainer->setAttributeValue (Style::style,Style::background::url (mimariProject.iconPath ())+Style::background::size::contain+
                                             Style::background::repeat::norepeat+
                                             Style::background::position::center_center);
            layout->addWidget (cpp14::make_unique<WText>("Statik Proje"),0,AlignmentFlag::Bottom)->setAttributeValue (Style::style,Style::font::family::dosis);

        }

        if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Elektrik ){
            auto mimariProject = item.toElectricProje ();
            imgContainer->setAttributeValue (Style::style,Style::background::url (mimariProject.iconPath ())+Style::background::size::contain+
                                             Style::background::repeat::norepeat+
                                             Style::background::position::center_center);
            layout->addWidget (cpp14::make_unique<WText>("Elektrik Proje"),0,AlignmentFlag::Bottom)->setAttributeValue (Style::style,Style::font::family::dosis);
        }

        if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Mekanik ){
            auto mimariProject = item.toMechanicProjec ();
            imgContainer->setAttributeValue (Style::style,Style::background::url (mimariProject.iconPath ())+Style::background::size::contain+
                                             Style::background::repeat::norepeat+
                                             Style::background::position::center_center);
            layout->addWidget (cpp14::make_unique<WText>("Mekanik Proje"),0,AlignmentFlag::Bottom)->setAttributeValue (Style::style,Style::font::family::dosis);

        }

        if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Ruhsat ){
            auto mimariProject = item.toRuhsat ();
            imgContainer->setAttributeValue (Style::style,Style::background::url (mimariProject.iconPath ())+Style::background::size::contain+
                                             Style::background::repeat::norepeat+
                                             Style::background::position::center_center);
            layout->addWidget (cpp14::make_unique<WText>("Ruhsat"),0,AlignmentFlag::Bottom)->setAttributeValue (Style::style,Style::font::family::dosis);
        }

        if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Iskele ){
            auto mimariProject = item.toIskele ();
            imgContainer->setAttributeValue (Style::style,Style::background::url (mimariProject.iconPath ())+Style::background::size::contain+
                                             Style::background::repeat::norepeat+
                                             Style::background::position::center_center);
            layout->addWidget (cpp14::make_unique<WText>("İskele Proje"),0,AlignmentFlag::Bottom)->setAttributeValue (Style::style,Style::font::family::dosis);
        }

        if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Iskan ){
            auto mimariProject = item.toIskan ();
            imgContainer->setAttributeValue (Style::style,Style::background::url (mimariProject.iconPath ())+Style::background::size::contain+
                                             Style::background::repeat::norepeat+
                                             Style::background::position::center_center);
            layout->addWidget (cpp14::make_unique<WText>("İskan Proje"),0,AlignmentFlag::Bottom)->setAttributeValue (Style::style,Style::font::family::dosis);
        }

        if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::ZeminEtudu ){
            auto mimariProject = item.toZeminEtudu ();
            imgContainer->setAttributeValue (Style::style,Style::background::url (mimariProject.iconPath ())+Style::background::size::contain+
                                             Style::background::repeat::norepeat+
                                             Style::background::position::center_center);
            layout->addWidget (cpp14::make_unique<WText>("Zemin Etüdü"),0,AlignmentFlag::Bottom)->setAttributeValue (Style::style,Style::font::family::dosis);
        }

        if( item.type () == SerikBLDCore::Imar::MimariProje::BaseProject::ProjectType::Asansor ){
            auto mimariProject = item.toAsansor ();
            imgContainer->setAttributeValue (Style::style,Style::background::url (mimariProject.iconPath ())+Style::background::size::contain+
                                             Style::background::repeat::norepeat+
                                             Style::background::position::center_center);
            layout->addWidget (cpp14::make_unique<WText>("Asansör"),0,AlignmentFlag::Bottom)->setAttributeValue (Style::style,Style::font::family::dosis);
        }

        auto onayText = layout->addWidget (cpp14::make_unique<WText>( !item.onay () ? "<b><i>Beklemede</i></b>" : "<b><i>Onaylandı</i></b>"),0,AlignmentFlag::Bottom);
        onayText->setAttributeValue (Style::style,Style::background::color::color (item.onay () ? Style::color::Green::Lime : Style::color::Red::FireBrick)+
                                     Style::font::family::dosis+Style::color::color (Style::color::White::White));

        if( item.assignedPersonelName ().size () ){
            auto assignedPersonelText = layout->addWidget (cpp14::make_unique<WText>( "<i>"+item.assignedPersonelName ()+"</i>"),0,AlignmentFlag::Bottom);
            assignedPersonelText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::SeaGreen )+
                                                     Style::font::family::dosis+Style::color::color (Style::color::White::White));
        }else{
            auto assignedPersonelText = layout->addWidget (cpp14::make_unique<WText>("<i>Personel Atanmamış</i>"),0,AlignmentFlag::Bottom);
            assignedPersonelText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Salmon)+
                                                     Style::font::family::dosis+Style::color::color (Style::color::White::White));
        }


    }


}

void v2::MainProjeView::onList(const QVector<SerikBLDCore::Imar::MimariLog> *mlist)
{
    Footer ()->clear ();
    Footer ()->setMargin (20,Side::Top);
    Footer ()->addStyleClass ("boxShadow");

    auto aciklamaEkleContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    aciklamaEkleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                          Bootstrap::Grid::Medium::col_md_4+
                                          Bootstrap::Grid::Small::col_sm_4+
                                          Bootstrap::Grid::ExtraSmall::col_xs_4+
                                          Bootstrap::Button::Primary);
    aciklamaEkleContainer->setMargin (10,Side::Bottom);
    aciklamaEkleContainer->setPadding (10,AllSides);
    aciklamaEkleContainer->decorationStyle ().setCursor (Cursor::PointingHand);
    auto aciklamaEkleText = aciklamaEkleContainer->addWidget (cpp14::make_unique<WText>("Açıklama Ekle"));
    aciklamaEkleText->setAttributeValue (Style::style,Style::font::size::s14px+Style::font::family::dosis);
    aciklamaEkleContainer->clicked ().connect ([=](){
        auto mDialog = createDialog ("Açıklama Ekle");

        auto textEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WTextEdit>());
        textEdit->setWidth (WLength("100%"));
        textEdit->setHeight (300);


        auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);
        saveBtn->clicked ().connect ([=](){

            if( textEdit->text ().toUTF8 ().size () < 50 ){
                this->showPopUpMessage ("Açıklamanız Yeterli Değil!.");
                return;
            }

            SerikBLDCore::Imar::AciklamaLog aciklama;
            aciklama.setAciklama (textEdit->text ().toUTF8 ());
            aciklama.setProjeOid (bsoncxx::oid{this->selectedProjectOid ()});
            aciklama.setEkleyen (mTCUser->AdSoyad ().toStdString ());
            aciklama.setCurrentDateTime ();

            auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (aciklama);
            if( !ins.empty () ){

                SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (bsoncxx::oid{this->selectedProjectOid ()}));


            }else{
                this->showPopUpMessage ("Açıklama Eklenemedi");
            }

            removeDialog (mDialog);

        });

        mDialog->show ();
    });

    auto dosyaEkleContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    dosyaEkleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                       Bootstrap::Grid::Medium::col_md_4+
                                       Bootstrap::Grid::Small::col_sm_4+
                                       Bootstrap::Grid::ExtraSmall::col_xs_4+
                                       Bootstrap::Button::info);
    dosyaEkleContainer->setMargin (10,Side::Bottom);
    dosyaEkleContainer->setPadding (10,AllSides);
    dosyaEkleContainer->decorationStyle ().setCursor (Cursor::PointingHand);
    auto dosyaaEkleText = dosyaEkleContainer->addWidget (cpp14::make_unique<WText>("Dosya Ekle"));
    dosyaaEkleText->setAttributeValue (Style::style,Style::font::size::s14px+Style::font::family::dosis);
    dosyaEkleContainer->clicked ().connect ([=](){
        auto mDialog = createDialog ("Dosya Ekle");

        auto fileNameTextEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
        fileNameTextEdit->setPlaceholderText ("Dosya Adı/Tanımı Giriniz");

        auto fileUploader = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>("Dosya Yükle"));



        auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);
        saveBtn->clicked ().connect ([=](){

            if( fileNameTextEdit->text ().toUTF8 ().size () < 10 ){
                this->showPopUpMessage ("Dosya Adı Yeterli Uzunlukta Değil");
                return;
            }

            if( !fileUploader->isUploaded () ){
                this->showPopUpMessage ("Dosya Yüklemediniz!","err");
                return;
            }

            auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (fileUploader->fileLocation ());


            SerikBLDCore::Imar::DosyaLog dosyaLog;
            dosyaLog.setFileOid (val.view().get_oid ().value);
            dosyaLog.setProjeOid (bsoncxx::oid{this->selectedProjectOid ()});
            dosyaLog.setEkleyen (mTCUser->AdSoyad ().toStdString ());
            dosyaLog.setFileName (fileNameTextEdit->text ().toUTF8 ());
            dosyaLog.setCurrentDateTime ();
            auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (dosyaLog);

            if( !ins.empty () ){
                SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (bsoncxx::oid{this->selectedProjectOid ()}));
            }else{
                this->showPopUpMessage ("Dosya Eklenemedi");
            }

            removeDialog (mDialog);

        });

        mDialog->show ();
    });



    auto duzeltmeEkleContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    duzeltmeEkleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                          Bootstrap::Grid::Medium::col_md_4+
                                          Bootstrap::Grid::Small::col_sm_4+
                                          Bootstrap::Grid::ExtraSmall::col_xs_4+
                                          Bootstrap::Button::Danger);
    duzeltmeEkleContainer->setMargin (10,Side::Bottom);
    duzeltmeEkleContainer->setPadding (10,AllSides);
    duzeltmeEkleContainer->decorationStyle ().setCursor (Cursor::PointingHand);
    auto duzeltmeEkleText = duzeltmeEkleContainer->addWidget (cpp14::make_unique<WText>("Düzeltme Ekle"));
    duzeltmeEkleText->setAttributeValue (Style::style,Style::font::size::s14px+Style::font::family::dosis);
    duzeltmeEkleContainer->clicked ().connect ([=](){
        auto mDialog = createDialog ("Düzeltme Ekle");

        auto fileNameTextEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
        fileNameTextEdit->setPlaceholderText ("Düzeltme Adı/Tanımı Giriniz");


        auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);
        saveBtn->clicked ().connect ([=](){

            if( fileNameTextEdit->text ().toUTF8 ().size () < 10 ){
                this->showPopUpMessage ("Düzeltme Adı Yeterli Uzunlukta Değil");
                return;
            }


            SerikBLDCore::Imar::DuzeltmeLog duzeltmeLog;
            duzeltmeLog.setDuzeltme (fileNameTextEdit->text ().toUTF8 ());
            duzeltmeLog.setProjeOid (bsoncxx::oid{this->selectedProjectOid ()});
            duzeltmeLog.setEkleyen (mTCUser->AdSoyad ().toStdString ());
            duzeltmeLog.setCurrentDateTime ();

            auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (duzeltmeLog);

            if( !ins.empty () ){
                SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (bsoncxx::oid{this->selectedProjectOid ()}));
            }else{
                this->showPopUpMessage ("Düzeltme Eklenemedi");
            }

            removeDialog (mDialog);

        });

        mDialog->show ();
    });


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
                auto text = container->addWidget (cpp14::make_unique<WText>(sItem.fileName ()));
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
                    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::DarkCyan)
                                                  +Style::color::color (Style::color::White::Snow));
                }else{
                    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)
                                                  +Style::color::color (Style::color::White::Snow));
                }

                if( !sItem.duzeltildi () ){
                    container->decorationStyle ().setCursor (Cursor::PointingHand);
                    container->clicked ().connect ([=](){
                        auto mDialog = createDialog ("Düzeltmeyi Onayla!");

                        auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Onayla"));
                        saveBtn->addStyleClass (Bootstrap::Button::Primary);

                        saveBtn->clicked ().connect ([=](){

                            SerikBLDCore::Imar::DuzeltmeLog filter;
                            filter.setOid (sItem.oid ().value ().to_string ());

                            filter.setDuzeltildi (true);

                            auto upt = SerikBLDCore::Imar::MimariLogManager::UpdateItem (filter);
                            if( upt ){
                                SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (this->oid ().value ()));
                                this->showPopUpMessage ("<p>Düzeltme <u>Onaylandı</u></p>");
                            }else{
                                this->showPopUpMessage ("<p>Bir Hata Oluştu</p>"
                                                        "<p>Düzeltme <u>Onaylanamadı/Yapılamadı</u><p>");
                            }

                            removeDialog (mDialog);

                        });

                    });
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



    auto controllerListContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    controllerListContainer->setMargin (5,AllSides);
    controllerListContainer->setWidth (WLength("100%"));
    auto hLayout = controllerListContainer->setLayout (cpp14::make_unique<WHBoxLayout>());
    auto backContainer = hLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Left);
    backContainer->setPadding (10,Side::Top|Side::Bottom);
    backContainer->setPadding (20,Side::Right|Side::Left);
    backContainer->addWidget (cpp14::make_unique<WText>("Geri"));
    backContainer->addStyleClass (CSSStyle::Button::blueButton+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    backContainer->clicked ().connect ([=](){
        SerikBLDCore::Imar::MimariLogManager::back (SerikBLDCore::Imar::MimariLog().setProjeOid (bsoncxx::oid{this->selectedProjectOid ()}));
    });

    auto nextContainer = hLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Right);
    nextContainer->setPadding (10,Side::Top|Side::Bottom);
    nextContainer->setPadding (20,Side::Right|Side::Left);
    nextContainer->addWidget (cpp14::make_unique<WText>("İleri"));
    nextContainer->addStyleClass (CSSStyle::Button::blueButton+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    nextContainer->clicked ().connect ([=](){
        SerikBLDCore::Imar::MimariLogManager::next (SerikBLDCore::Imar::MimariLog().setProjeOid (bsoncxx::oid{this->selectedProjectOid ()}));
    });

}

void v2::MainProjeView::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage ("Hata: " + errorText , "err");
}

std::string v2::MainProjeView::selectedProjectOid() const
{
    return mSelectedProjectOid;
}

int v2::MainProjeView::currentLogFilterIndex() const
{
    return mCurrentLogFilterIndex;
}

void v2::MainProjeView::addIslemLog(const std::string& log , const bsoncxx::oid& projeOid )
{
    SerikBLDCore::Imar::IslemLog islemlog;
    islemlog.setLog (log);
    islemlog.setCurrentDateTime ();
    islemlog.setEkleyen (mTCUser->AdSoyad ().toStdString ());
    islemlog.setProjeOid (projeOid);

    auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (islemlog);
    if( !ins.empty () ){
        //        SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (projeOid));
        this->showPopUpMessage ("Log Güncellendi");
    }else{
        this->showPopUpMessage ("Log Eklenemedi","err");
    }
}

void v2::MainProjeView::setSelectedProjectOid(const std::string &selectedProjectOid)
{
    mSelectedProjectOid = selectedProjectOid;
}

SerikBLDCore::TC *v2::MainProjeView::User() const
{
    return mTCUser;
}

void v2::MainProjeView::loadProject(const bsoncxx::oid &projectOid )
{
    mSelectedProjectOid = projectOid.to_string ();
    SerikBLDCore::Imar::MimariProje::BaseProject filter;
    filter.setOid (projectOid);

    auto mProje = SerikBLDCore::Imar::BaseProjeManager::FindOneItem (filter);

    if( mProje.view ().empty () ){
        this->showPopUpMessage ("Proje Yüklenemedi","err");
        return;
    }

    Content ()->clear ();

    SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (mProje.oid ().value ()));

    auto backContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    backContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                  Bootstrap::Grid::Medium::col_md_1+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_2+
                                  Bootstrap::Button::info);
    auto backText = backContainer->addWidget (cpp14::make_unique<WText>("<b>Diğer Projeler</b>"));
    backText->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px+Style::color::color (Style::color::White::Snow));
    backContainer->decorationStyle ().setCursor (Cursor::PointingHand);
    backContainer->clicked ().connect ([=](){
        mSelectedProjectOid = this->oid ().value ().to_string ();
        SerikBLDCore::Imar::MimariProje::BaseProject filter;
        filter.setMainProjeOid (this->oid ().value ());
        SerikBLDCore::Imar::BaseProjeManager::UpdateList(filter);
        SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (this->oid ().value ()));
    });


    // Proje Başlığı
    auto titleContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    titleContainer->addStyleClass (Bootstrap::Grid::col_full_12+
                                   Bootstrap::ImageShape::img_thumbnail);

    auto titleText = titleContainer->addWidget (cpp14::make_unique<WText>("<b>Proje Detayları</b>"));
    titleContainer->setMargin (20,Side::Top|Side::Bottom);
    titleText->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s16px);


    // Proje Çizim Saihibi
    auto ownerContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    ownerContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                   Bootstrap::Grid::Medium::col_md_3+
                                   Bootstrap::Grid::Small::col_sm_4+
                                   Bootstrap::Grid::ExtraSmall::col_xs_5+
                                   Bootstrap::ImageShape::img_thumbnail);
    ownerContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::DimGray)+
                                       Style::font::family::dosis+
                                       Style::font::size::s14px+
                                       Style::color::color (Style::color::White::Snow));
    ownerContainer->setHeight (140);
    ownerContainer->addWidget (cpp14::make_unique<WText>("<i><u>Proje Sahibi</u></i>"));

    if( mProje.ownerOid ().empty () ){
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());
        ownerContainer->addWidget (cpp14::make_unique<WText>("İlgili Kişi Atanmamış"));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());
        auto ilgiliAtaBtn = ownerContainer->addWidget (cpp14::make_unique<WPushButton>("Atama Yap!"));
        ilgiliAtaBtn->addStyleClass (Bootstrap::Button::Danger);
        ilgiliAtaBtn->clicked ().connect ([=](){

            auto mDialog = createDialog ("İlgili Proje Sahibi Ata");

            auto tcfilterLineEditContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
            tcfilterLineEditContainer->addStyleClass (Bootstrap::Grid::container_fluid);

            auto hLayout = tcfilterLineEditContainer->setLayout (cpp14::make_unique<WHBoxLayout>());

            auto tcLineEditFilter = hLayout->addWidget (cpp14::make_unique<WLineEdit>(),1,AlignmentFlag::Justify);
            tcLineEditFilter->setPlaceholderText ("İsim Soyisin Giriniz( En az 3 Harf )");

            auto listBtn = hLayout->addWidget (cpp14::make_unique<WPushButton>("Listele"),0,AlignmentFlag::Middle);


            auto listContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
            listContainer->setHeight (350);
            listContainer->addStyleClass (Bootstrap::Grid::container_fluid+"boxShadow");
            listContainer->setOverflow (Overflow::Scroll,Orientation::Vertical);



            listBtn->clicked ().connect ([=](){
                if( tcLineEditFilter->text ().toUTF8 ().size () < 3 ){
                    this->showPopUpMessage ("İsim Soyisim uzunluğu Yeterli Değil!","msg");
                    return;
                }

                SerikBLDCore::Item regex("");
                regex.append("$regex",tcLineEditFilter->text ().toUTF8 ());
                regex.append("$options","i");

                SerikBLDCore::TC filter;
                filter.append(SerikBLDCore::TC::KeyAdSoyad,regex);

                auto listVal = SerikBLDCore::Imar::BaseProjeManager::getDB ()->find (filter);

                if( !listVal ){
                    this->showPopUpMessage ("Kayıt Bulunamadı!");
                    return;
                }

                listContainer->clear ();


                for( auto item : listVal.value () ){
                    SerikBLDCore::TC tcItem;
                    tcItem.setDocumentView (item);

                    auto container = listContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                    container->setWidth (WLength("100%"));
                    container->setHeight (50);
                    container->addStyleClass ("boxShadow boxRadius");

                    auto hLayout = container->setLayout (cpp14::make_unique<WHBoxLayout>());

                    auto nameText = hLayout->addWidget (cpp14::make_unique<WText>(tcItem.AdSoyad ().toStdString ()),0,AlignmentFlag::Justify);
                    nameText->setWidth (175);
                    nameText->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

                    auto telText = hLayout->addWidget (cpp14::make_unique<WText>(tcItem.CepTelefonu ().toStdString ()),0,AlignmentFlag::Justify);
                    telText->setWidth (50);
                    telText->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

                    auto tcText = hLayout->addWidget (cpp14::make_unique<WText>(tcItem.TCNO ().toStdString ()),0,AlignmentFlag::Justify);
                    tcText->setWidth (50);
                    tcText->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

                    auto ataBtn = hLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);
                    ataBtn->addStyleClass (Bootstrap::Button::Primary);
                    ataBtn->addWidget (cpp14::make_unique<WText>("Atama Yap"))->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);
                    ataBtn->decorationStyle ().setCursor (Cursor::PointingHand);
                    ataBtn->setPadding (5,Side::Right|Side::Left);

                    ataBtn->clicked ().connect ([=](){

                        SerikBLDCore::Imar::MimariProje::BaseProject filter;
                        filter.setOid (mProje.oid ().value ());
                        filter.setProjectOwner (tcItem.oid ().value (),tcItem.AdSoyad ().toStdString (),tcItem.CepTelefonu ().toStdString ());

                        if( SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter) ){
                            this->loadProject (mProje.oid ().value ());
                            this->showPopUpMessage ("Kayıt Yapıldı","msg");

                            this->removeDialog (mDialog);
                        }else{
                            this->showPopUpMessage ("Atama Yapılamadı! Daha Sonra Tekrar Deneyiniz","err");
                        }


                    });

                }





            });


            mDialog->show ();
        });
    }else{
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());

        ownerContainer->addWidget (cpp14::make_unique<WText>("AdSoyad: " + mProje.ownerAdSoyad ()));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());

        ownerContainer->addWidget (cpp14::make_unique<WText>("Telefon: " + mProje.ownerTelefon ()));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());

        ownerContainer->addWidget (cpp14::make_unique<WText>(mProje.ownerOid ()));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());
    }


    // Proje Belediye Yetkili Kişisi
    auto assignedContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    assignedContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_5+
                                      Bootstrap::ImageShape::img_thumbnail+
                                      Bootstrap::Grid::Offset::Large::col_lg_6+
                                      Bootstrap::Grid::Offset::Medium::col_md_6+
                                      Bootstrap::Grid::Offset::Small::col_sm_4+
                                      Bootstrap::Grid::Offset::ExtraSmall::col_xs_2);
    assignedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::Gainsboro)+
                                          Style::font::family::dosis+
                                          Style::font::size::s14px+
                                          Style::color::color (Style::color::Grey::Black));
    assignedContainer->setHeight (140);
    assignedContainer->addWidget (cpp14::make_unique<WText>("<i><u>Yetkili Kişi</u></i>"));

    if( mProje.assignedPersonelName ().empty () ){
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());
        auto nameText = assignedContainer->addWidget (cpp14::make_unique<WText>("<b>Yetkili Personel Henüz Yok</b>"));
        nameText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Red)+Style::color::color (Style::color::White::Snow));
        nameText->setPadding (5,AllSides);
    }else{
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());

        assignedContainer->addWidget (cpp14::make_unique<WText>(mProje.assignedPersonelName ()));
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());

        assignedContainer->addWidget (cpp14::make_unique<WText>(mProje.assignedPersonelOid ()));
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());
    }



    for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
        auto projectFileContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        projectFileContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                             Bootstrap::Grid::Medium::col_md_2+
                                             Bootstrap::Grid::Small::col_sm_3+
                                             Bootstrap::Grid::ExtraSmall::col_xs_4+
                                             Bootstrap::ImageShape::img_thumbnail);
        projectFileContainer->addStyleClass (Bootstrap::Button::Primary);
        projectFileContainer->setMargin (10,Side::Top);
        projectFileContainer->setHeight (200);


        projectFileContainer->addWidget (cpp14::make_unique<WText>(mProje[i].value ().projeAdi ()));
        projectFileContainer->addWidget (cpp14::make_unique<WBreak>());

        auto iconContainer = projectFileContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        iconContainer->setAttributeValue (Style::style,Style::background::url ("icon/fileicon.png")+
                                          Style::background::size::contain+
                                          Style::background::repeat::norepeat+
                                          Style::background::position::center_center);
        iconContainer->setHeight (80);




        projectFileContainer->addWidget (cpp14::make_unique<WBreak>());
        auto fileoidText = projectFileContainer->addWidget (cpp14::make_unique<WText>(mProje[i].value ().fileOid ()));
        fileoidText->setAttributeValue (Style::style,Style::font::family::dosis+ Style::font::size::s12px);
        projectFileContainer->addWidget (cpp14::make_unique<WBreak>());
        auto onayText = projectFileContainer->addWidget (cpp14::make_unique<WText>(mProje[i]->onay () ? "Onaylanmış" : "Onaylanmamış"));
        auto backColor = !mProje[i]->onay () ? Style::background::color::color (Style::color::Red::FireBrick) : Style::background::color::color (Style::color::Green::DarkCyan);
        onayText->setAttributeValue (Style::style,Style::font::family::dosis + backColor +Style::font::size::s16px);
        onayText->setPadding (3,AllSides);
        onayText->setMargin (5,Side::Top|Side::Bottom);

        projectFileContainer->setMargin (5,Side::Top|Side::Bottom);
        projectFileContainer->decorationStyle ().setCursor (Cursor::PointingHand);

        projectFileContainer->clicked ().connect ([=](){

            auto mDialog = createDialog ("Proje Detayları");

            if( mProje[i].value ().onay () ){

                auto text = mDialog->contents ()->addWidget (cpp14::make_unique<WText>("<b>Bu Proje Onaylanmış.Değişiklik Yapamazsınız!</b>"));
                text->setPadding (5,AllSides);
                text->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)+Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);

                mDialog->contents ()->addWidget (cpp14::make_unique<WBreak>());
                mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (15);
                auto indirText = mDialog->contents ()->addWidget (cpp14::make_unique<WText>("<u>Dosyayı İndir</u>"));
                indirText->setPadding (10,AllSides);
                indirText->decorationStyle ().setCursor (Cursor::PointingHand);
                indirText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+
                                              Style::font::family::dosis+Style::font::size::s14px);
                indirText->clicked ().connect ([=](){
                    auto fileUrl = SerikBLDCore::Imar::BaseProjeManager::downloadFileWeb (mProje[i].value ().fileOid ().c_str ());
                    this->doJavaScript ("window.open('"+fileUrl+"','_blank');");
                    removeDialog (mDialog);
                });
                return;
            }



            auto projeDosyaDegistirBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Proje Dosyasını Değiştir"));
            projeDosyaDegistirBtn->setMargin (5,Side::Right|Side::Left);
            projeDosyaDegistirBtn->clicked ().connect ([=](){

                mDialog->contents ()->clear ();


                mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Dosyasını Değiştir"));

                auto fileuploader = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>("Yeni Proje Dosyası"));


                auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
                svBtn->clicked ().connect ([=](){



                    if( !fileuploader->isUploaded () ){
                        this->showPopUpMessage ("Yeni Dosya Yüklemediniz!");
                        return;
                    }
                    if( !SerikBLDCore::Imar::BaseProjeManager::getDB ()->deleteGridFS (fileoidText->text ().toUTF8 ().c_str ()) ){
                        this->showPopUpMessage ("<p>Eski Dosya Silinemedi!</p>"
                                                "<p>Tekrar Deneyiniz</p>");
                        return;
                    }
                    SerikBLDCore::Imar::MimariProje::BaseProject filter;
                    filter.setOid (mProje.oid ().value ());

                    for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                        filter.addProje (mProje[i].value ());
                    }
                    filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});
                    SerikBLDCore::Imar::MimariProje::FileProject newFileProject;
                    newFileProject.setOnay (false);
                    newFileProject.setProjeAdi (mProje[i].value ().projeAdi ());
                    auto newFileOid = SerikBLDCore::Imar::BaseProjeManager::getDB ()->uploadfile (fileuploader->fileLocation ().toStdString ().c_str ());
                    newFileProject.setFileOid (newFileOid.view().get_oid ().value);
                    filter.addProje (newFileProject);
                    auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                    if( !upt ){
                        this->showPopUpMessage ("<p>Dosya Güncellenemedi</p><p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                        return;
                    }
                    this->addIslemLog ("<u><b>"+mProje[i].value ().projeAdi () + "</b></u> Dosyası Değiştirildi",mProje.oid ().value ());

                    this->removeDialog (mDialog);
                    this->loadProject (mProje.oid ().value ());
                });
            });


            auto projeAdiniDegistirBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Proje Adını Değiştir"));
            projeAdiniDegistirBtn->setMargin (5,Side::Left|Side::Right);
            projeAdiniDegistirBtn->clicked ().connect ([=](){

                mDialog->contents ()->clear ();


                mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Adını Değiştir"));

                auto projeYeniAdiLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
                projeYeniAdiLineEdit->setPlaceholderText ("Proje Dosyasının Yeni Adını Giriniz");


                auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
                svBtn->clicked ().connect ([=](){

                    SerikBLDCore::Imar::MimariProje::BaseProject filter;
                    filter.setOid (mProje.oid ().value ());

                    for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                        filter.addProje (mProje[i].value ());
                    }
                    filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});
                    SerikBLDCore::Imar::MimariProje::FileProject newFileProject;
                    newFileProject.setOnay (false);
                    newFileProject.setProjeAdi (projeYeniAdiLineEdit->text ().toUTF8 ());
                    newFileProject.setFileOid (bsoncxx::oid{fileoidText->text ().toUTF8 ()});

                    filter.addProje (newFileProject);

                    auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                    if( !upt ){
                        this->showPopUpMessage ("<p>Dosya Güncellenemedi</p>"
                                                "<p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                        return;
                    }
                    this->addIslemLog (mProje[i].value ().projeAdi () + " Adı Değiştirildi. Yeni Adı: " + projeYeniAdiLineEdit->text ().toUTF8 () ,mProje.oid ().value ());

                    this->removeDialog (mDialog);
                    this->loadProject (mProje.oid ().value ());
                });
            });



            auto projeSilBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Projeyi Sil"));
            projeSilBtn->setMargin (5,Side::Left|Side::Right);
            projeSilBtn->addStyleClass (Bootstrap::Button::Danger);
            projeSilBtn->clicked ().connect ([=](){

                mDialog->contents ()->clear ();


                mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Sil"));


                auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Sil"));
                svBtn->clicked ().connect ([=](){

                    SerikBLDCore::Imar::MimariProje::BaseProject filter;
                    filter.setOid (mProje.oid ().value ());

                    if( mProje.projeCount () == 1 ){

                        if( SerikBLDCore::Imar::BaseProjeManager::getDB ()->removeField (filter,SerikBLDCore::Imar::MimariProje::BaseProject::keyProje) ){
                            this->addIslemLog (mProje[i].value ().projeAdi () + " Dosyası Silindi",mProje.oid ().value ());
                            this->loadProject (mProje.oid ().value ());
                            this->showPopUpMessage ("<p>Proje Dosyası Silindi</p>");
                            this->removeDialog (mDialog);
                        }

                    }else{
                        for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                            filter.addProje (mProje[i].value ());
                        }
                        filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});

                        auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                        if( !upt ){
                            this->showPopUpMessage ("<p>Dosya Güncellenemedi</p>"
                                                    "<p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                            return;
                        }
                        this->addIslemLog (mProje[i].value ().projeAdi () + " Dosyası Silindi",mProje.oid ().value ());

                        this->removeDialog (mDialog);
                        this->loadProject (mProje.oid ().value ());
                    }


                });
            });

            auto projeDosyaIndirlBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Dosyayı İndir"));
            projeDosyaIndirlBtn->setMargin (5,Side::Left|Side::Right);
            projeDosyaIndirlBtn->addStyleClass (Bootstrap::Button::Link);
            projeDosyaIndirlBtn->clicked ().connect ([=](){
                auto fileUrl = SerikBLDCore::Imar::BaseProjeManager::downloadFileWeb (mProje[i].value ().fileOid ().c_str ());
                this->doJavaScript ("window.open('"+fileUrl+"','_blank');");
                removeDialog (mDialog);
            });



            mDialog->show ();

        });




    }


    {
        auto projectFileContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        projectFileContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                             Bootstrap::Grid::Medium::col_md_2+
                                             Bootstrap::Grid::Small::col_sm_3+
                                             Bootstrap::Grid::ExtraSmall::col_xs_4+
                                             Bootstrap::ImageShape::img_thumbnail);
        projectFileContainer->addStyleClass (Bootstrap::Button::info);
        projectFileContainer->setMargin (5,Side::Top);
        projectFileContainer->setHeight (200);
        auto yeniEkleText = projectFileContainer->addWidget (cpp14::make_unique<WText>("<b>Yeni Proje Dosyası Ekle</b>"));
        yeniEkleText->setAttributeValue (Style::style,Style::font::family::dosis+
                                         Style::color::color (Style::color::White::Snow));
        projectFileContainer->decorationStyle ().setCursor (Cursor::PointingHand);


        // TODO: yeni Proje Dosyası Ekleme
        projectFileContainer->clicked ().connect ([=](){

            auto mDialog = createDialog ("Yeni Dosya Ekle");

            mDialog->contents ()->clear ();


            mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Dosyasını Ekle"));
            auto fileNameLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
            fileNameLineEdit->setPlaceholderText ("Proje Adını Giriniz");

            auto fileuploader = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>("Yeni Proje Dosyası"));


            auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
            svBtn->clicked ().connect ([=](){
                if( !fileuploader->isUploaded () ){
                    this->showPopUpMessage ("Yeni Dosya Yüklemediniz!");
                    return;
                }


                SerikBLDCore::Imar::MimariProje::BaseProject filter;
                filter.setOid (mProje.oid ().value ());

                for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                    filter.addProje (mProje[i].value ());
                }

                SerikBLDCore::Imar::MimariProje::FileProject newFileProject;
                newFileProject.setOnay (false);
                auto newFileOid = SerikBLDCore::Imar::BaseProjeManager::getDB ()->uploadfile (fileuploader->fileLocation ().toStdString ().c_str ());
                newFileProject.setFileOid (newFileOid.view().get_oid ().value);
                newFileProject.setProjeAdi (fileNameLineEdit->text ().toUTF8 ());
                filter.addProje (newFileProject);
                auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                if( !upt ){
                    this->showPopUpMessage ("<p>Dosya Güncellenemedi</p><p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                    return;
                }else{
                    this->addIslemLog (fileNameLineEdit->text ().toUTF8 () + " Dosyası Eklendi",mProje.oid ().value ());
                }
                this->removeDialog (mDialog);
                this->loadProject (mProje.oid ().value ());
            });

            mDialog->show ();

        });
    }


}









v2::KurumsalProjeView::KurumsalProjeView(MainProje &mainProje, SerikBLDCore::User *_user)
    :MainProjeView(mainProje,_user->getDB (),nullptr),mUser(_user)
{

}

void v2::KurumsalProjeView::initHeader()
{

    Header ()->clear ();
    auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    container->addStyleClass (Bootstrap::Grid::Large::col_lg_10+
                              Bootstrap::Grid::Medium::col_md_10+
                              Bootstrap::Grid::Small::col_sm_10+
                              Bootstrap::Grid::ExtraSmall::col_xs_9+
                              CSSStyle::Shadows::shadow8px);




    auto gridLayout = container->setLayout (cpp14::make_unique<WGridLayout>());


    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,0,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Ada</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,1,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Parsel</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,2,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Mahalle</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,3,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Başvuru Tarihi</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);


    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,0,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (this->ada ())))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,1,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (this->parsel ())))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,2,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (this->mahalle ())))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,3,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (WDate::fromJulianDay (this->basvuruJulianDay ()).toString ("dd/MMMM/yyyy"))))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);


    auto onaylacontainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    onaylacontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                    Bootstrap::Grid::Medium::col_md_2+
                                    Bootstrap::Grid::Small::col_sm_2+
                                    Bootstrap::Grid::ExtraSmall::col_xs_3+
                                    CSSStyle::Shadows::shadow8px);
    onaylacontainer->setHeight (64);
    auto onaylaVLayout = onaylacontainer->setLayout (cpp14::make_unique<WVBoxLayout>());

    if( this->onay () ){
        onaylacontainer->addStyleClass (CSSStyle::Button::redButton);
        auto onaylaText = onaylaVLayout->addWidget (cpp14::make_unique<WText>("<b>ONAYI KALDIR</b>"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
        onaylaText->setAttributeValue (Style::style,Style::font::family::dosis+
                                       Style::font::size::s16px);
    }else {
        onaylacontainer->addStyleClass (CSSStyle::Button::blueButton);
        auto onaylaText = onaylaVLayout->addWidget (cpp14::make_unique<WText>("<b>ONAYLA</b>"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
        onaylaText->setAttributeValue (Style::style,Style::font::family::dosis+
                                       Style::font::size::s16px);
    }

    onaylacontainer->clicked ().connect ([=](){

        if( !this->onay () ){
            auto list = SerikBLDCore::Imar::BaseProjeManager::UpdateList (SerikBLDCore::Imar::MimariProje::BaseProject().setMainProjeOid (this->oid ().value ()));

            bool onaylanaBilir = true;
            for( auto item : list ){
                if( !item.onay () ){
                    onaylanaBilir = false;
                }
            }

            if( !onaylanaBilir ){
                this->showPopUpMessage ("<p>Alt Projelerden Onaylanmayan Alt Projeler Var.</p>","err");
            }else{
                SerikBLDCore::Imar::MimariProje::MainProje filter;
                filter.setOid (this->oid ().value ().to_string ());
                filter.setOnay (true);
                auto upt = SerikBLDCore::Imar::BaseProjeManager::getDB ()->updateItem (filter);
                if( upt ){
                    if( upt.value ().modified_count () ){
                        this->setOnay (true);
                        this->showPopUpMessage ("Ana Proje Onayladı");
                        this->initHeader ();
                    }else{
                        this->showPopUpMessage ("Bir Hata Oluştu. Ana Proje Onaylanamadı");
                    }
                }else{
                    this->showPopUpMessage ("Bir Hata Oluştu. Ana Proje Onaylanamadı");
                }
            }
        }else{
            SerikBLDCore::Imar::MimariProje::MainProje filter;
            filter.setOid (this->oid ().value ().to_string ());
            filter.setOnay (false);
            auto upt = SerikBLDCore::Imar::BaseProjeManager::getDB ()->updateItem (filter);
            if( upt ){
                if( upt.value ().modified_count () ){
                    this->setOnay (false);
                    this->showPopUpMessage ("Ana Proje Onayı İptal Edildi");
                    this->initHeader ();
                }else{
                    this->showPopUpMessage ("Bir Hata Oluştu. Ana Proje Onay İptal Edilemedi");
                }
            }else{
                this->showPopUpMessage ("Bir Hata Oluştu. Ana Proje Onay İptal Edilemedi");
            }
        }


    });


    SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (this->oid ().value ()));
    SerikBLDCore::Imar::BaseProjeManager::UpdateList (SerikBLDCore::Imar::MimariProje::BaseProject().setMainProjeOid (this->oid ().value ()));
}

void v2::KurumsalProjeView::loadProject(const bsoncxx::oid &projectOid)
{
    this->setSelectedProjectOid (projectOid.to_string ());
    SerikBLDCore::Imar::MimariProje::BaseProject filter;
    filter.setOid (projectOid);

    auto mProje = SerikBLDCore::Imar::BaseProjeManager::FindOneItem (filter);

    if( mProje.view ().empty () ){
        this->showPopUpMessage ("Proje Yüklenemedi","err");
        return;
    }

    Content ()->clear ();

    SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (mProje.oid ().value ()));

    auto backContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    backContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                  Bootstrap::Grid::Medium::col_md_1+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_2+
                                  Bootstrap::Button::info);
    auto backText = backContainer->addWidget (cpp14::make_unique<WText>("<b>Diğer Projeler</b>"));
    backText->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px+Style::color::color (Style::color::White::Snow));
    backContainer->decorationStyle ().setCursor (Cursor::PointingHand);
    backContainer->clicked ().connect ([=](){
        this->setSelectedProjectOid ( this->oid ().value ().to_string () );
        SerikBLDCore::Imar::MimariProje::BaseProject filter;
        filter.setMainProjeOid (this->oid ().value ());
        SerikBLDCore::Imar::BaseProjeManager::UpdateList(filter);
        SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (this->oid ().value ()));
    });


    // Proje Başlığı
    auto titleContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    titleContainer->addStyleClass (Bootstrap::Grid::col_full_12+
                                   Bootstrap::ImageShape::img_thumbnail);

    auto titleText = titleContainer->addWidget (cpp14::make_unique<WText>("<b>Proje Detayları</b>"));
    titleContainer->setMargin (20,Side::Top|Side::Bottom);
    titleText->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s16px);


    // Proje Çizim Saihibi
    auto ownerContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    ownerContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                   Bootstrap::Grid::Medium::col_md_3+
                                   Bootstrap::Grid::Small::col_sm_4+
                                   Bootstrap::Grid::ExtraSmall::col_xs_4+
                                   Bootstrap::ImageShape::img_thumbnail);
    ownerContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::DimGray)+
                                       Style::font::family::dosis+
                                       Style::font::size::s14px+
                                       Style::color::color (Style::color::White::Snow));
    ownerContainer->setHeight (140);
    ownerContainer->addWidget (cpp14::make_unique<WText>("<i><u>Proje Sahibi</u></i>"));

    if( mProje.ownerOid ().empty () ){
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());
        ownerContainer->addWidget (cpp14::make_unique<WText>("İlgili Kişi Atanmamış"));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());
        ownerContainer->addWidget (cpp14::make_unique<WText>("Firma Yetkilisi Atama Yapacak"));
    }else{
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());

        ownerContainer->addWidget (cpp14::make_unique<WText>("AdSoyad: " + mProje.ownerAdSoyad ()));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());

        ownerContainer->addWidget (cpp14::make_unique<WText>("Telefon: " + mProje.ownerTelefon ()));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());

        ownerContainer->addWidget (cpp14::make_unique<WText>(mProje.ownerOid ()));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());
    }



    // Proje Onaylama Alanı
    auto onaylaContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    onaylaContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                    Bootstrap::Grid::Medium::col_md_2+
                                    Bootstrap::Grid::Small::col_sm_4+
                                    Bootstrap::Grid::ExtraSmall::col_xs_4+
                                    Bootstrap::Grid::Offset::Large::col_lg_2+
                                    Bootstrap::Grid::Offset::Medium::col_md_2);
    onaylaContainer->setAttributeValue (Style::style,Style::font::family::dosis+
                                        Style::font::size::s14px+
                                        Style::color::color (Style::color::White::Snow));
    onaylaContainer->setHeight (140);

    if( mProje.onay () ){

        if( this->mUser->oid ().value ().to_string () == mProje.assignedPersonelOid () ){
            auto onaykaldirText = onaylaContainer->addWidget (cpp14::make_unique<WText>("<b>Onayı Kaldır</b>"));
            onaykaldirText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)+
                                               Style::font::family::dosis+Style::font::size::s18px+
                                               Style::color::color (Style::color::White::Snow));
            onaykaldirText->addStyleClass (CSSStyle::Shadows::shadow8px+CSSStyle::Radius::radius3px);
            onaykaldirText->setPadding (10,AllSides);
            onaykaldirText->decorationStyle ().setCursor (Cursor::PointingHand);

            onaykaldirText->clicked ().connect ([=](){

                if( this->onay () ){
                    this->showPopUpMessage ("İLk Önce Ana Projenin Onayını Kaldırınız","err");
                }else{
                    SerikBLDCore::Imar::MimariProje::BaseProject filter;
                    filter.setOid (mProje.oid ().value ());

                    filter.setOnay (false);

                    auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);

                    if( upt ){
                        this->addIslemLog (mProje.title () + " Onayı Kaldırıldı",mProje.oid ().value ());
                        this->loadProject (mProje.oid ().value ());
                    }else{
                        this->showPopUpMessage ("Projeyi Onayı Kaldırılamadı!. Daha Sonra Tekrar Deneyiniz","err");
                    }
                }


            });
        }


    }else{
        if( this->mUser->oid ().value ().to_string () == mProje.assignedPersonelOid () ){
            auto onaykaldirText = onaylaContainer->addWidget (cpp14::make_unique<WText>("<b>ONAYLA</b>"));
            onaykaldirText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::DarkGreen)+
                                               Style::font::family::dosis+Style::font::size::s18px+
                                               Style::color::color (Style::color::White::Snow));
            onaykaldirText->addStyleClass (CSSStyle::Shadows::shadow8px+CSSStyle::Radius::radius3px);
            onaykaldirText->setPadding (10,AllSides);
            onaykaldirText->decorationStyle ().setCursor (Cursor::PointingHand);

            onaykaldirText->clicked ().connect ([=](){

                if( mProje.onaylanabilir () ){
                    SerikBLDCore::Imar::MimariProje::BaseProject filter;
                    filter.setOid (mProje.oid ().value ());

                    filter.setOnay (true);

                    auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);

                    if( upt ){
                        this->addIslemLog (mProje.title () + " Onaylandı" , mProje.oid ().value ());
                        this->loadProject (mProje.oid ().value ());
                    }else{
                        this->showPopUpMessage ("Projeyi Onaylanamadı!. Daha Sonra Tekrar Deneyiniz","err");
                    }
                }else{
                    this->showPopUpMessage ("<p>Bazı Çizim Dosyaları Onaylanmamış!</p>"
                                            "<p>Önce Onaylanmayan Çizim Dosyalarını Onaylayınız</p>","msg");
                }


            });

        }
    }




    // Proje Belediye Yetkili Kişisi
    auto assignedContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    assignedContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_4+
                                      Bootstrap::ImageShape::img_thumbnail+
                                      Bootstrap::Grid::Offset::Large::col_lg_2+
                                      Bootstrap::Grid::Offset::Medium::col_md_2);
    assignedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightSlateGray)+
                                          Style::font::family::dosis+
                                          Style::font::size::s14px+
                                          Style::color::color (Style::color::White::Snow));
    assignedContainer->setHeight (140);
    assignedContainer->addWidget (cpp14::make_unique<WText>("<i><u>Yetkili Kişi</u></i>"));

    if( mProje.assignedPersonelName ().empty () ){
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());
        auto nameText = assignedContainer->addWidget (cpp14::make_unique<WText>("<b>Yetkili Personel Henüz Yok</b>"));
        nameText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Red)+Style::color::color (Style::color::White::Snow));
        nameText->setPadding (5,AllSides);
        nameText->setMargin (15,Side::Bottom);

        assignedContainer->addWidget (cpp14::make_unique<WBreak>());
        assignedContainer->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (20);

        auto ilgiliAtaBtn = assignedContainer->addWidget (cpp14::make_unique<WText>("Projeyi Al!"));
        ilgiliAtaBtn->setMargin (15,Side::Top);
        ilgiliAtaBtn->setPadding (10,AllSides);
        ilgiliAtaBtn->decorationStyle ().setCursor (Cursor::PointingHand);
        ilgiliAtaBtn->addStyleClass (Bootstrap::Button::Success);
        ilgiliAtaBtn->clicked ().connect ([=](){

            SerikBLDCore::Imar::MimariProje::BaseProject filter;
            filter.setOid (mProje.oid ().value ());

            filter.assignPersonel (this->mUser->oid ().value (),this->mUser->AdSoyad ());

            auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);

            if( upt ){
                this->loadProject (mProje.oid ().value ());
            }else{
                this->showPopUpMessage ("Projeyi Alamadınız!. Daha Sonra Tekrar Deneyiniz");
            }


        });

    }else{
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());

        assignedContainer->addWidget (cpp14::make_unique<WText>(mProje.assignedPersonelName ()));
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());

        assignedContainer->addWidget (cpp14::make_unique<WText>(mProje.assignedPersonelOid ()));
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());

        if( this->mUser->oid ().value ().to_string () == mProje.assignedPersonelOid () ){
            auto ilgiliAtaBtn = assignedContainer->addWidget (cpp14::make_unique<WPushButton>("Projeyi Bırak!"));
            ilgiliAtaBtn->addStyleClass (Bootstrap::Button::Danger);
            ilgiliAtaBtn->clicked ().connect ([=](){

                SerikBLDCore::Imar::MimariProje::BaseProject filter;
                filter.setOid (mProje.oid ().value ());

                filter.assignPersonel (this->mUser->oid ().value (),this->mUser->AdSoyad ());

                auto upt = SerikBLDCore::Imar::BaseProjeManager::getDB ()->removeField (filter,SerikBLDCore::Imar::MimariProje::BaseProject::keyAssignPersonelOid);
                if( upt ){
                    upt = SerikBLDCore::Imar::BaseProjeManager::getDB ()->removeField (filter,SerikBLDCore::Imar::MimariProje::BaseProject::keyAssignPersonelName);
                    if( upt ){
                        this->loadProject (mProje.oid ().value ());
                    }else{
                        this->showPopUpMessage ("Projeyi Bırakamadınız!. Daha Sonra Tekrar Deneyiniz");
                    }
                }
            });
        }

    }



    for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
        auto projectFileContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        projectFileContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                             Bootstrap::Grid::Medium::col_md_2+
                                             Bootstrap::Grid::Small::col_sm_3+
                                             Bootstrap::Grid::ExtraSmall::col_xs_4+
                                             Bootstrap::ImageShape::img_thumbnail);
        projectFileContainer->addStyleClass (Bootstrap::Button::Primary);
        projectFileContainer->setMargin (10,Side::Top);
        projectFileContainer->setHeight (200);


        projectFileContainer->addWidget (cpp14::make_unique<WText>(mProje[i].value ().projeAdi ()));
        projectFileContainer->addWidget (cpp14::make_unique<WBreak>());

        auto iconContainer = projectFileContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        iconContainer->setAttributeValue (Style::style,Style::background::url ("icon/fileicon.png")+
                                          Style::background::size::contain+
                                          Style::background::repeat::norepeat+
                                          Style::background::position::center_center);
        iconContainer->setHeight (80);
        projectFileContainer->addWidget (cpp14::make_unique<WBreak>());
        auto fileoidText = projectFileContainer->addWidget (cpp14::make_unique<WText>(mProje[i].value ().fileOid ()));
        fileoidText->setAttributeValue (Style::style,Style::font::family::dosis+ Style::font::size::s12px);
        projectFileContainer->addWidget (cpp14::make_unique<WBreak>());
        auto onayText = projectFileContainer->addWidget (cpp14::make_unique<WText>(mProje[i]->onay () ? "Onaylanmış" : "Onaylanmamış"));
        auto backColor = !mProje[i]->onay () ? Style::background::color::color (Style::color::Red::FireBrick) : Style::background::color::color (Style::color::Green::DarkCyan);
        onayText->setAttributeValue (Style::style,Style::font::family::dosis + backColor +Style::font::size::s16px);
        onayText->setPadding (3,AllSides);
        onayText->setMargin (5,Side::Top|Side::Bottom);

        projectFileContainer->setMargin (5,Side::Top|Side::Bottom);
        projectFileContainer->decorationStyle ().setCursor (Cursor::PointingHand);

        projectFileContainer->clicked ().connect ([=](){

            auto mDialog = createDialog ("Proje Detayları");

            if( mProje[i].value ().onay () ){

                auto text = mDialog->contents ()->addWidget (cpp14::make_unique<WText>("<b>Bu Proje Onaylanmış.Değişiklik Yapamazsınız!</b>"));
                text->setPadding (5,AllSides);
                text->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)+Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);

                mDialog->contents ()->addWidget (cpp14::make_unique<WBreak>());
                mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (15);
                auto indirText = mDialog->contents ()->addWidget (cpp14::make_unique<WText>("<u>Dosyayı İndir</u>"));
                indirText->setPadding (10,AllSides);
                indirText->decorationStyle ().setCursor (Cursor::PointingHand);
                indirText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+
                                              Style::font::family::dosis+Style::font::size::s14px);
                indirText->clicked ().connect ([=](){
                    auto fileUrl = SerikBLDCore::Imar::BaseProjeManager::downloadFileWeb (mProje[i].value ().fileOid ().c_str ());
                    this->doJavaScript ("window.open('"+fileUrl+"','_blank');");
                    removeDialog (mDialog);
                });


                auto projeOnaylaBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Onayı Kaldır"));
                projeOnaylaBtn->setMargin (5,Side::Left|Side::Right);
                projeOnaylaBtn->addStyleClass (Bootstrap::Button::Danger);
                projeOnaylaBtn->clicked ().connect ([=](){

                    if( mProje.onay () ){
                        this->showPopUpMessage ("İlk Önce Ana Projenin Onayını Kaldırınız","err");
                        return;
                    }

                    mDialog->contents ()->clear ();

                    auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Onayı Kaldır"));
                    svBtn->addStyleClass (Bootstrap::Button::Danger);
                    svBtn->clicked ().connect ([=](){

                        SerikBLDCore::Imar::MimariProje::BaseProject filter;
                        filter.setOid (mProje.oid ().value ());

                        for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                            filter.addProje (mProje[i].value ());
                        }
                        filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});
                        SerikBLDCore::Imar::MimariProje::FileProject newFileProject;
                        newFileProject.setOnay (false);
                        newFileProject.setProjeAdi (mProje[i].value ().projeAdi ());
                        newFileProject.setFileOid (bsoncxx::oid{fileoidText->text ().toUTF8 ()});

                        filter.addProje (newFileProject);

                        auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                        if( !upt ){
                            this->showPopUpMessage ("<p>Dosya Güncellenemedi</p>"
                                                    "<p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                            return;
                        }
                        this->addIslemLog ("<b><u>"+mProje[i].value ().projeAdi () + "</u></b> Proje Onayı Kaldırıldı." ,mProje.oid ().value ());

                        this->removeDialog (mDialog);
                        this->loadProject (mProje.oid ().value ());
                    });
                });

                return;
            }



            if( this->mUser->oid ().value ().to_string () == mProje.assignedPersonelOid () ){

                auto projeDosyaDegistirBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Dosyayı Değiştir"));
                projeDosyaDegistirBtn->setMargin (5,Side::Right|Side::Left);
                projeDosyaDegistirBtn->clicked ().connect ([=](){

                    mDialog->contents ()->clear ();


                    mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Dosyasını Değiştir"));

                    auto fileuploader = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>("Yeni Proje Dosyası"));


                    auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
                    svBtn->clicked ().connect ([=](){



                        if( !fileuploader->isUploaded () ){
                            this->showPopUpMessage ("Yeni Dosya Yüklemediniz!");
                            return;
                        }
                        if( !SerikBLDCore::Imar::BaseProjeManager::getDB ()->deleteGridFS (fileoidText->text ().toUTF8 ().c_str ()) ){
                            this->showPopUpMessage ("<p>Eski Dosya Silinemedi!</p>"
                                                    "<p>Tekrar Deneyiniz</p>");
                            return;
                        }
                        SerikBLDCore::Imar::MimariProje::BaseProject filter;
                        filter.setOid (mProje.oid ().value ());

                        for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                            filter.addProje (mProje[i].value ());
                        }
                        filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});
                        SerikBLDCore::Imar::MimariProje::FileProject newFileProject;
                        newFileProject.setOnay (false);
                        newFileProject.setProjeAdi (mProje[i].value ().projeAdi ());
                        auto newFileOid = SerikBLDCore::Imar::BaseProjeManager::getDB ()->uploadfile (fileuploader->fileLocation ().toStdString ().c_str ());
                        newFileProject.setFileOid (newFileOid.view().get_oid ().value);
                        filter.addProje (newFileProject);
                        auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                        if( !upt ){
                            this->showPopUpMessage ("<p>Dosya Güncellenemedi</p><p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                            return;
                        }
                        this->addIslemLog ("<u><b>"+mProje[i].value ().projeAdi () + "</b></u> Dosyası Değiştirildi",mProje.oid ().value ());

                        this->removeDialog (mDialog);
                        this->loadProject (mProje.oid ().value ());
                    });
                });


                auto projeAdiniDegistirBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Adını Değiştir"));
                projeAdiniDegistirBtn->setMargin (5,Side::Left|Side::Right);
                projeAdiniDegistirBtn->clicked ().connect ([=](){

                    mDialog->contents ()->clear ();


                    mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Adını Değiştir"));

                    auto projeYeniAdiLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
                    projeYeniAdiLineEdit->setPlaceholderText ("Proje Dosyasının Yeni Adını Giriniz");


                    auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
                    svBtn->clicked ().connect ([=](){

                        SerikBLDCore::Imar::MimariProje::BaseProject filter;
                        filter.setOid (mProje.oid ().value ());

                        for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                            filter.addProje (mProje[i].value ());
                        }
                        filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});
                        SerikBLDCore::Imar::MimariProje::FileProject newFileProject;
                        newFileProject.setOnay (false);
                        newFileProject.setProjeAdi (projeYeniAdiLineEdit->text ().toUTF8 ());
                        newFileProject.setFileOid (bsoncxx::oid{fileoidText->text ().toUTF8 ()});

                        filter.addProje (newFileProject);

                        auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                        if( !upt ){
                            this->showPopUpMessage ("<p>Dosya Güncellenemedi</p>"
                                                    "<p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                            return;
                        }
                        this->addIslemLog (mProje[i].value ().projeAdi () + " Adı Değiştirildi. Yeni Adı: " + projeYeniAdiLineEdit->text ().toUTF8 () ,mProje.oid ().value ());

                        this->removeDialog (mDialog);
                        this->loadProject (mProje.oid ().value ());
                    });
                });



                auto projeOnaylaBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Onayla"));
                projeOnaylaBtn->setMargin (5,Side::Left|Side::Right);
                projeOnaylaBtn->addStyleClass (Bootstrap::Button::Success);
                projeOnaylaBtn->clicked ().connect ([=](){

                    mDialog->contents ()->clear ();

                    auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Onayla"));
                    svBtn->addStyleClass (Bootstrap::Button::Success);
                    svBtn->clicked ().connect ([=](){

                        SerikBLDCore::Imar::MimariProje::BaseProject filter;
                        filter.setOid (mProje.oid ().value ());

                        for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                            filter.addProje (mProje[i].value ());
                        }
                        filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});
                        SerikBLDCore::Imar::MimariProje::FileProject newFileProject;
                        newFileProject.setOnay (true);
                        newFileProject.setProjeAdi (mProje[i].value ().projeAdi ());
                        newFileProject.setFileOid (bsoncxx::oid{fileoidText->text ().toUTF8 ()});

                        filter.addProje (newFileProject);

                        auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                        if( !upt ){
                            this->showPopUpMessage ("<p>Dosya Güncellenemedi</p>"
                                                    "<p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                            return;
                        }
                        this->addIslemLog ("<b><u>"+mProje[i].value ().projeAdi () + "</u></b> Proje Onaylandı." ,mProje.oid ().value ());

                        this->removeDialog (mDialog);
                        this->loadProject (mProje.oid ().value ());
                    });
                });



                auto projeSilBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Sil"));
                projeSilBtn->setMargin (5,Side::Left|Side::Right);
                projeSilBtn->addStyleClass (Bootstrap::Button::Danger);
                projeSilBtn->clicked ().connect ([=](){

                    mDialog->contents ()->clear ();


                    mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Sil"));


                    auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Sil"));
                    svBtn->clicked ().connect ([=](){

                        SerikBLDCore::Imar::MimariProje::BaseProject filter;
                        filter.setOid (mProje.oid ().value ());

                        if( mProje.projeCount () == 1 ){

                            if( SerikBLDCore::Imar::BaseProjeManager::getDB ()->removeField (filter,SerikBLDCore::Imar::MimariProje::BaseProject::keyProje) ){
                                this->addIslemLog (mProje[i].value ().projeAdi () + " Dosyası Silindi",mProje.oid ().value ());
                                this->loadProject (mProje.oid ().value ());
                                this->showPopUpMessage ("<p>Proje Dosyası Silindi</p>");
                                this->removeDialog (mDialog);
                            }

                        }else{
                            for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                                filter.addProje (mProje[i].value ());
                            }
                            filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});

                            auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                            if( !upt ){
                                this->showPopUpMessage ("<p>Dosya Güncellenemedi</p>"
                                                        "<p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                                return;
                            }
                            this->addIslemLog (mProje[i].value ().projeAdi () + " Dosyası Silindi",mProje.oid ().value ());

                            this->removeDialog (mDialog);
                            this->loadProject (mProje.oid ().value ());
                        }


                    });
                });

            }


            auto projeDosyaIndirlBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Dosyayı İndir"));
            projeDosyaIndirlBtn->setMargin (5,Side::Left|Side::Right);
            projeDosyaIndirlBtn->addStyleClass (Bootstrap::Button::Link);
            projeDosyaIndirlBtn->clicked ().connect ([=](){
                auto fileUrl = SerikBLDCore::Imar::BaseProjeManager::downloadFileWeb (mProje[i].value ().fileOid ().c_str ());
                this->doJavaScript ("window.open('"+fileUrl+"','_blank');");
                removeDialog (mDialog);
            });



            mDialog->show ();

        });


    }
}

void v2::KurumsalProjeView::onList(const QVector<SerikBLDCore::Imar::MimariLog> *mlist)
{
    Footer ()->clear ();
    Footer ()->setMargin (20,Side::Top);
    Footer ()->addStyleClass ("boxShadow");

    auto aciklamaEkleContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    aciklamaEkleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                          Bootstrap::Grid::Medium::col_md_4+
                                          Bootstrap::Grid::Small::col_sm_4+
                                          Bootstrap::Grid::ExtraSmall::col_xs_4+
                                          Bootstrap::Button::Primary);
    aciklamaEkleContainer->setMargin (10,Side::Bottom);
    aciklamaEkleContainer->setPadding (10,AllSides);
    aciklamaEkleContainer->decorationStyle ().setCursor (Cursor::PointingHand);
    auto aciklamaEkleText = aciklamaEkleContainer->addWidget (cpp14::make_unique<WText>("Açıklama Ekle"));
    aciklamaEkleText->setAttributeValue (Style::style,Style::font::size::s14px+Style::font::family::dosis);
    aciklamaEkleContainer->clicked ().connect ([=](){
        auto mDialog = createDialog ("Açıklama Ekle");

        auto textEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WTextEdit>());
        textEdit->setWidth (WLength("100%"));
        textEdit->setHeight (300);


        auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);
        saveBtn->clicked ().connect ([=](){

            if( textEdit->text ().toUTF8 ().size () < 50 ){
                this->showPopUpMessage ("Açıklamanız Yeterli Değil!.");
                return;
            }

            SerikBLDCore::Imar::AciklamaLog aciklama;
            aciklama.setAciklama (textEdit->text ().toUTF8 ());
            aciklama.setProjeOid (bsoncxx::oid{this->selectedProjectOid ()});
            aciklama.setEkleyen (mUser->AdSoyad ());
            aciklama.setCurrentDateTime ();

            auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (aciklama);
            if( !ins.empty () ){

                SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (bsoncxx::oid{this->selectedProjectOid ()}));


            }else{
                this->showPopUpMessage ("Açıklama Eklenemedi");
            }

            removeDialog (mDialog);

        });

        mDialog->show ();
    });

    auto dosyaEkleContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    dosyaEkleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                       Bootstrap::Grid::Medium::col_md_4+
                                       Bootstrap::Grid::Small::col_sm_4+
                                       Bootstrap::Grid::ExtraSmall::col_xs_4+
                                       Bootstrap::Button::info);
    dosyaEkleContainer->setMargin (10,Side::Bottom);
    dosyaEkleContainer->setPadding (10,AllSides);
    dosyaEkleContainer->decorationStyle ().setCursor (Cursor::PointingHand);
    auto dosyaaEkleText = dosyaEkleContainer->addWidget (cpp14::make_unique<WText>("Dosya Ekle"));
    dosyaaEkleText->setAttributeValue (Style::style,Style::font::size::s14px+Style::font::family::dosis);
    dosyaEkleContainer->clicked ().connect ([=](){
        auto mDialog = createDialog ("Dosya Ekle");

        auto fileNameTextEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
        fileNameTextEdit->setPlaceholderText ("Dosya Adı/Tanımı Giriniz");

        auto fileUploader = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>("Dosya Yükle"));



        auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);
        saveBtn->clicked ().connect ([=](){

            if( fileNameTextEdit->text ().toUTF8 ().size () < 10 ){
                this->showPopUpMessage ("Dosya Adı Yeterli Uzunlukta Değil");
                return;
            }

            if( !fileUploader->isUploaded () ){
                this->showPopUpMessage ("Dosya Yüklemediniz!","err");
                return;
            }

            auto val = SerikBLDCore::Imar::BaseProjeManager::uploadfile (fileUploader->fileLocation ());


            SerikBLDCore::Imar::DosyaLog dosyaLog;
            dosyaLog.setFileOid (val.view().get_oid ().value);
            dosyaLog.setProjeOid (bsoncxx::oid{this->selectedProjectOid ()});
            dosyaLog.setEkleyen (mUser->AdSoyad ());
            dosyaLog.setFileName (fileNameTextEdit->text ().toUTF8 ());
            dosyaLog.setCurrentDateTime ();

            auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (dosyaLog);

            if( !ins.empty () ){
                SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (bsoncxx::oid{this->selectedProjectOid ()}));
            }else{
                this->showPopUpMessage ("Dosya Eklenemedi");
            }

            removeDialog (mDialog);

        });

        mDialog->show ();
    });



    auto duzeltmeEkleContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    duzeltmeEkleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                          Bootstrap::Grid::Medium::col_md_4+
                                          Bootstrap::Grid::Small::col_sm_4+
                                          Bootstrap::Grid::ExtraSmall::col_xs_4+
                                          Bootstrap::Button::Danger);
    duzeltmeEkleContainer->setMargin (10,Side::Bottom);
    duzeltmeEkleContainer->setPadding (10,AllSides);
    duzeltmeEkleContainer->decorationStyle ().setCursor (Cursor::PointingHand);
    auto duzeltmeEkleText = duzeltmeEkleContainer->addWidget (cpp14::make_unique<WText>("Düzeltme Ekle"));
    duzeltmeEkleText->setAttributeValue (Style::style,Style::font::size::s14px+Style::font::family::dosis);
    duzeltmeEkleContainer->clicked ().connect ([=](){
        auto mDialog = createDialog ("Düzeltme Ekle");

        auto fileNameTextEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
        fileNameTextEdit->setPlaceholderText ("Düzeltme Adı/Tanımı Giriniz");


        auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);
        saveBtn->clicked ().connect ([=](){

            if( fileNameTextEdit->text ().toUTF8 ().size () < 10 ){
                this->showPopUpMessage ("Düzeltme Adı Yeterli Uzunlukta Değil");
                return;
            }


            SerikBLDCore::Imar::DuzeltmeLog duzeltmeLog;
            duzeltmeLog.setDuzeltme (fileNameTextEdit->text ().toUTF8 ());
            duzeltmeLog.setProjeOid (bsoncxx::oid{this->selectedProjectOid ()});
            duzeltmeLog.setEkleyen (mUser->AdSoyad ());
            duzeltmeLog.setCurrentDateTime ();

            auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (duzeltmeLog);

            if( !ins.empty () ){
                SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (bsoncxx::oid{this->selectedProjectOid ()}));
            }else{
                this->showPopUpMessage ("Düzeltme Eklenemedi");
            }

            removeDialog (mDialog);

        });

        mDialog->show ();
    });


    for( auto item : *mlist ){
        switch (item.LogType ()) {
        case SerikBLDCore::Imar::MimariLog::Type::Log:
            if( currentLogFilterIndex () == 0 || currentLogFilterIndex () == 4){
                auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail+CSSStyle::Gradient::blueGradient);
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
                    loggerText->setAttributeValue (Style::style,Style::font::size::s11px+Style::font::family::dosis);
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
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Ivory));


            }

            break;

        case SerikBLDCore::Imar::MimariLog::Type::Dosya:
            if( currentLogFilterIndex () == 0 || currentLogFilterIndex () == 3){

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
                auto text = container->addWidget (cpp14::make_unique<WText>(sItem.fileName ()));
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
            if( currentLogFilterIndex ()== 0 || currentLogFilterIndex () == 2){

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
            if( currentLogFilterIndex () == 0 || currentLogFilterIndex () == 1 ){

                auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
                container->setPadding (20,Side::Top);
                container->setPadding (10,Side::Bottom);
                container->setMargin (5,Side::Top|Side::Bottom);
                container->setContentAlignment (AlignmentFlag::Left);
                container->decorationStyle ().setCursor (Cursor::PointingHand);


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
                    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::DarkCyan)
                                                  +Style::color::color (Style::color::White::Snow));
                }else{
                    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)
                                                  +Style::color::color (Style::color::White::Snow));
                }

                container->clicked ().connect ([=](){
                    auto mDialog = createDialog ("Düzeltmeyi Onayla!");

                    auto duzeltmeLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
                    duzeltmeLineEdit->setPlaceholderText ("Yapılan Düzeltme Tanımını Giriniz!");

                    auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Onayla"));
                    saveBtn->addStyleClass (Bootstrap::Button::Primary);

                    saveBtn->clicked ().connect ([=](){

                        SerikBLDCore::Imar::DuzeltmeLog filter;
                        filter.setOid (sItem.oid ().value ().to_string ());

                        filter.setDuzeltildi (true);

                        auto upt = SerikBLDCore::Imar::MimariLogManager::UpdateItem (filter);
                        if( upt ){
                            SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (this->oid ().value ()));

                        }else{
                            this->showPopUpMessage ("<p>Bir Hata Oluştu</p>"
                                                    "<p>Düzeltme <u>Onaylanamadı/Yapılamadı</u><p>");
                        }

                        removeDialog (mDialog);

                    });

                });

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




    auto controllerListContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    controllerListContainer->setMargin (5,AllSides);
    controllerListContainer->setWidth (WLength("100%"));
    auto hLayout = controllerListContainer->setLayout (cpp14::make_unique<WHBoxLayout>());
    auto backContainer = hLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Left);
    backContainer->setPadding (10,Side::Top|Side::Bottom);
    backContainer->setPadding (20,Side::Right|Side::Left);
    backContainer->addWidget (cpp14::make_unique<WText>("Geri"));
    backContainer->addStyleClass (CSSStyle::Button::blueButton+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    backContainer->clicked ().connect ([=](){
        SerikBLDCore::Imar::MimariLogManager::back (SerikBLDCore::Imar::MimariLog().setProjeOid (bsoncxx::oid{this->selectedProjectOid ()}));
    });

    auto nextContainer = hLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Right);
    nextContainer->setPadding (10,Side::Top|Side::Bottom);
    nextContainer->setPadding (20,Side::Right|Side::Left);
    nextContainer->addWidget (cpp14::make_unique<WText>("İleri"));
    nextContainer->addStyleClass (CSSStyle::Button::blueButton+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    nextContainer->clicked ().connect ([=](){
        SerikBLDCore::Imar::MimariLogManager::next (SerikBLDCore::Imar::MimariLog().setProjeOid (bsoncxx::oid{this->selectedProjectOid ()}));
    });
}

void v2::KurumsalProjeView::addIslemLog(const std::string& log, const bsoncxx::oid &projeOid)
{
    SerikBLDCore::Imar::IslemLog islemlog;
    islemlog.setLog (log);
    islemlog.setCurrentDateTime ();
    islemlog.setEkleyen (mUser->AdSoyad ());
    islemlog.setProjeOid (projeOid);

    auto ins = SerikBLDCore::Imar::MimariLogManager::InsertItem (islemlog);
    if( !ins.empty () ){
        //        SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (projeOid));
        this->showPopUpMessage ("Log Güncellendi");
    }else{
        this->showPopUpMessage ("Log Eklenemedi","err");
    }
}







v2::BireyselProjeView::BireyselProjeView(SerikBLDCore::Imar::MimariProje::MainProje &mainProje, DB *_db, SerikBLDCore::TC *_tcUser)
    :MainProjeView (mainProje,_db,_tcUser)
{

}

void v2::BireyselProjeView::initHeader()
{


    Header ()->clear ();
    auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    container->addStyleClass (Bootstrap::Grid::col_full_12+"boxShadow");

    auto gridLayout = container->setLayout (cpp14::make_unique<WGridLayout>());


    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,0,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Ada</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,1,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Parsel</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,2,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Mahalle</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),0,3,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("<b>Başvuru Tarihi</b>")))
            ->setAttributeValue (Style::style,Style::font::family::dosis+
                                 Style::font::size::s14px);


    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,0,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (this->ada ())))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,1,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (this->parsel ())))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,2,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (this->mahalle ())))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    gridLayout->addWidget (cpp14::make_unique<WContainerWidget>(),1,3,AlignmentFlag::Justify)
            ->addWidget (cpp14::make_unique<WText>(WString("{1}").arg (WDate::fromJulianDay (this->basvuruJulianDay ()).toString ("dd/MMMM/yyyy"))))
            ->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px);

    SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (this->oid ().value ()));
    SerikBLDCore::Imar::BaseProjeManager::UpdateList (SerikBLDCore::Imar::MimariProje::BaseProject().setMainProjeOid (this->oid ().value ()));
}

void v2::BireyselProjeView::loadProject(const bsoncxx::oid &projectOid)
{
    setSelectedProjectOid (projectOid.to_string ());
    SerikBLDCore::Imar::MimariProje::BaseProject filter;
    filter.setOid (projectOid);

    auto mProje = SerikBLDCore::Imar::BaseProjeManager::FindOneItem (filter);

    if( mProje.view ().empty () ){
        this->showPopUpMessage ("Proje Yüklenemedi","err");
        return;
    }

    Content ()->clear ();

    SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (mProje.oid ().value ()));

    auto backContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    backContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                  Bootstrap::Grid::Medium::col_md_1+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_2+
                                  Bootstrap::Button::info);
    auto backText = backContainer->addWidget (cpp14::make_unique<WText>("<b>Diğer Projeler</b>"));
    backText->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s14px+Style::color::color (Style::color::White::Snow));
    backContainer->decorationStyle ().setCursor (Cursor::PointingHand);
    backContainer->clicked ().connect ([=](){
        setSelectedProjectOid( this->oid ().value ().to_string ());
        SerikBLDCore::Imar::MimariProje::BaseProject filter;
        filter.setMainProjeOid (this->oid ().value ());
        SerikBLDCore::Imar::BaseProjeManager::UpdateList(filter);
        SerikBLDCore::Imar::MimariLogManager::UpdateList (SerikBLDCore::Imar::MimariLog().setProjeOid (this->oid ().value ()));
    });


    // Proje Başlığı
    auto titleContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    titleContainer->addStyleClass (Bootstrap::Grid::col_full_12+
                                   Bootstrap::ImageShape::img_thumbnail);

    auto titleText = titleContainer->addWidget (cpp14::make_unique<WText>("<b>Proje Detayları</b>"));
    titleContainer->setMargin (20,Side::Top|Side::Bottom);
    titleText->setAttributeValue (Style::style,Style::font::family::dosis+Style::font::size::s16px);


    // Proje Çizim Saihibi
    auto ownerContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    ownerContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                   Bootstrap::Grid::Medium::col_md_3+
                                   Bootstrap::Grid::Small::col_sm_4+
                                   Bootstrap::Grid::ExtraSmall::col_xs_5+
                                   Bootstrap::ImageShape::img_thumbnail);
    ownerContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::DimGray)+
                                       Style::font::family::dosis+
                                       Style::font::size::s14px+
                                       Style::color::color (Style::color::White::Snow));
    ownerContainer->setHeight (140);
    ownerContainer->addWidget (cpp14::make_unique<WText>("<i><u>Proje Sahibi</u></i>"));


    ownerContainer->addWidget (cpp14::make_unique<WBreak>());

    if( mProje.ownerOid ().empty () ){
        ownerContainer->addWidget (cpp14::make_unique<WText>("Bu Proje ile İlgili Bir Atama Yapılmamış"));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());
    }else{
        ownerContainer->addWidget (cpp14::make_unique<WText>("AdSoyad: " + mProje.ownerAdSoyad ()));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());

        ownerContainer->addWidget (cpp14::make_unique<WText>("Telefon: " + mProje.ownerTelefon ()));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());

        ownerContainer->addWidget (cpp14::make_unique<WText>(mProje.ownerOid ()));
        ownerContainer->addWidget (cpp14::make_unique<WBreak>());

    }




    // Proje Belediye Yetkili Kişisi
    auto assignedContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    assignedContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_5+
                                      Bootstrap::ImageShape::img_thumbnail+
                                      Bootstrap::Grid::Offset::Large::col_lg_6+
                                      Bootstrap::Grid::Offset::Medium::col_md_6+
                                      Bootstrap::Grid::Offset::Small::col_sm_4+
                                      Bootstrap::Grid::Offset::ExtraSmall::col_xs_2);
    assignedContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::Gainsboro)+
                                          Style::font::family::dosis+
                                          Style::font::size::s14px+
                                          Style::color::color (Style::color::Grey::Black));
    assignedContainer->setHeight (140);
    assignedContainer->addWidget (cpp14::make_unique<WText>("<i><u>Yetkili Kişi</u></i>"));

    if( mProje.assignedPersonelName ().empty () ){
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());
        auto nameText = assignedContainer->addWidget (cpp14::make_unique<WText>("<b>Yetkili Personel Henüz Yok</b>"));
        nameText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Red)+Style::color::color (Style::color::White::Snow));
        nameText->setPadding (5,AllSides);
    }else{
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());

        assignedContainer->addWidget (cpp14::make_unique<WText>(mProje.assignedPersonelName ()));
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());

        assignedContainer->addWidget (cpp14::make_unique<WText>(mProje.assignedPersonelOid ()));
        assignedContainer->addWidget (cpp14::make_unique<WBreak>());
    }



    for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
        auto projectFileContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        projectFileContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                             Bootstrap::Grid::Medium::col_md_2+
                                             Bootstrap::Grid::Small::col_sm_3+
                                             Bootstrap::Grid::ExtraSmall::col_xs_4+
                                             Bootstrap::ImageShape::img_thumbnail);
        projectFileContainer->addStyleClass (Bootstrap::Button::Primary);
        projectFileContainer->setMargin (10,Side::Top);
        projectFileContainer->setHeight (200);


        projectFileContainer->addWidget (cpp14::make_unique<WText>(mProje[i].value ().projeAdi ()));
        projectFileContainer->addWidget (cpp14::make_unique<WBreak>());

        auto iconContainer = projectFileContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        iconContainer->setAttributeValue (Style::style,Style::background::url ("icon/fileicon.png")+
                                          Style::background::size::contain+
                                          Style::background::repeat::norepeat+
                                          Style::background::position::center_center);
        iconContainer->setHeight (80);




        projectFileContainer->addWidget (cpp14::make_unique<WBreak>());
        auto fileoidText = projectFileContainer->addWidget (cpp14::make_unique<WText>(mProje[i].value ().fileOid ()));
        fileoidText->setAttributeValue (Style::style,Style::font::family::dosis+ Style::font::size::s12px);
        projectFileContainer->addWidget (cpp14::make_unique<WBreak>());
        auto onayText = projectFileContainer->addWidget (cpp14::make_unique<WText>(mProje[i]->onay () ? "Onaylanmış" : "Onaylanmamış"));
        auto backColor = !mProje[i]->onay () ? Style::background::color::color (Style::color::Red::FireBrick) : Style::background::color::color (Style::color::Green::DarkCyan);
        onayText->setAttributeValue (Style::style,Style::font::family::dosis + backColor +Style::font::size::s16px);
        onayText->setPadding (3,AllSides);
        onayText->setMargin (5,Side::Top|Side::Bottom);

        projectFileContainer->setMargin (5,Side::Top|Side::Bottom);
        projectFileContainer->decorationStyle ().setCursor (Cursor::PointingHand);

        projectFileContainer->clicked ().connect ([=](){

            auto mDialog = createDialog ("Proje Detayları");

            if( mProje[i].value ().onay () ){

                auto text = mDialog->contents ()->addWidget (cpp14::make_unique<WText>("<b>Bu Proje Onaylanmış.Değişiklik Yapamazsınız!</b>"));
                text->setPadding (5,AllSides);
                text->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)+Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);

                mDialog->contents ()->addWidget (cpp14::make_unique<WBreak>());
                mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (15);
                auto indirText = mDialog->contents ()->addWidget (cpp14::make_unique<WText>("<u>Dosyayı İndir</u>"));
                indirText->setPadding (10,AllSides);
                indirText->decorationStyle ().setCursor (Cursor::PointingHand);
                indirText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+
                                              Style::font::family::dosis+Style::font::size::s14px);
                indirText->clicked ().connect ([=](){
                    auto fileUrl = SerikBLDCore::Imar::BaseProjeManager::downloadFileWeb (mProje[i].value ().fileOid ().c_str ());
                    this->doJavaScript ("window.open('"+fileUrl+"','_blank');");
                    removeDialog (mDialog);
                });
                return;
            }


            if( this->User ()->oid ().value ().to_string () != mProje.ownerOid () ){

                auto text = mDialog->contents ()->addWidget (cpp14::make_unique<WText>("<b>Bu Proje de Yetkili Değilsiniz!</b>"));
                text->setPadding (5,AllSides);
                text->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)+Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);

                mDialog->contents ()->addWidget (cpp14::make_unique<WBreak>());
                mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>())->setHeight (15);
                auto indirText = mDialog->contents ()->addWidget (cpp14::make_unique<WText>("<u>Dosyayı İndir</u>"));
                indirText->setPadding (10,AllSides);
                indirText->decorationStyle ().setCursor (Cursor::PointingHand);
                indirText->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+
                                              Style::font::family::dosis+Style::font::size::s14px);
                indirText->clicked ().connect ([=](){
                    auto fileUrl = SerikBLDCore::Imar::BaseProjeManager::downloadFileWeb (mProje[i].value ().fileOid ().c_str ());
                    this->doJavaScript ("window.open('"+fileUrl+"','_blank');");
                    removeDialog (mDialog);
                });
                return;
            }



            auto projeDosyaDegistirBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Proje Dosyasını Değiştir"));
            projeDosyaDegistirBtn->setMargin (5,Side::Right|Side::Left);
            projeDosyaDegistirBtn->clicked ().connect ([=](){

                mDialog->contents ()->clear ();


                mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Dosyasını Değiştir"));

                auto fileuploader = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>("Yeni Proje Dosyası"));


                auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
                svBtn->clicked ().connect ([=](){



                    if( !fileuploader->isUploaded () ){
                        this->showPopUpMessage ("Yeni Dosya Yüklemediniz!");
                        return;
                    }
                    if( !SerikBLDCore::Imar::BaseProjeManager::getDB ()->deleteGridFS (fileoidText->text ().toUTF8 ().c_str ()) ){
                        this->showPopUpMessage ("<p>Eski Dosya Silinemedi!</p>"
                                                "<p>Tekrar Deneyiniz</p>");
                        return;
                    }
                    SerikBLDCore::Imar::MimariProje::BaseProject filter;
                    filter.setOid (mProje.oid ().value ());

                    for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                        filter.addProje (mProje[i].value ());
                    }
                    filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});
                    SerikBLDCore::Imar::MimariProje::FileProject newFileProject;
                    newFileProject.setOnay (false);
                    newFileProject.setProjeAdi (mProje[i].value ().projeAdi ());
                    auto newFileOid = SerikBLDCore::Imar::BaseProjeManager::getDB ()->uploadfile (fileuploader->fileLocation ().toStdString ().c_str ());
                    newFileProject.setFileOid (newFileOid.view().get_oid ().value);
                    filter.addProje (newFileProject);
                    auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                    if( !upt ){
                        this->showPopUpMessage ("<p>Dosya Güncellenemedi</p><p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                        return;
                    }
                    this->addIslemLog ("<u><b>"+mProje[i].value ().projeAdi () + "</b></u> Dosyası Değiştirildi",mProje.oid ().value ());

                    this->removeDialog (mDialog);
                    this->loadProject (mProje.oid ().value ());
                });
            });


            auto projeAdiniDegistirBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Proje Adını Değiştir"));
            projeAdiniDegistirBtn->setMargin (5,Side::Left|Side::Right);
            projeAdiniDegistirBtn->clicked ().connect ([=](){

                mDialog->contents ()->clear ();


                mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Adını Değiştir"));

                auto projeYeniAdiLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
                projeYeniAdiLineEdit->setPlaceholderText ("Proje Dosyasının Yeni Adını Giriniz");


                auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
                svBtn->clicked ().connect ([=](){

                    SerikBLDCore::Imar::MimariProje::BaseProject filter;
                    filter.setOid (mProje.oid ().value ());

                    for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                        filter.addProje (mProje[i].value ());
                    }
                    filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});
                    SerikBLDCore::Imar::MimariProje::FileProject newFileProject;
                    newFileProject.setOnay (false);
                    newFileProject.setProjeAdi (projeYeniAdiLineEdit->text ().toUTF8 ());
                    newFileProject.setFileOid (bsoncxx::oid{fileoidText->text ().toUTF8 ()});

                    filter.addProje (newFileProject);

                    auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                    if( !upt ){
                        this->showPopUpMessage ("<p>Dosya Güncellenemedi</p>"
                                                "<p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                        return;
                    }
                    this->addIslemLog (mProje[i].value ().projeAdi () + " Adı Değiştirildi. Yeni Adı: " + projeYeniAdiLineEdit->text ().toUTF8 () ,mProje.oid ().value ());

                    this->removeDialog (mDialog);
                    this->loadProject (mProje.oid ().value ());
                });
            });



            auto projeSilBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Projeyi Sil"));
            projeSilBtn->setMargin (5,Side::Left|Side::Right);
            projeSilBtn->addStyleClass (Bootstrap::Button::Danger);
            projeSilBtn->clicked ().connect ([=](){

                mDialog->contents ()->clear ();


                mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Sil"));


                auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Sil"));
                svBtn->clicked ().connect ([=](){

                    SerikBLDCore::Imar::MimariProje::BaseProject filter;
                    filter.setOid (mProje.oid ().value ());

                    if( mProje.projeCount () == 1 ){

                        if( SerikBLDCore::Imar::BaseProjeManager::getDB ()->removeField (filter,SerikBLDCore::Imar::MimariProje::BaseProject::keyProje) ){
                            this->addIslemLog (mProje[i].value ().projeAdi () + " Dosyası Silindi",mProje.oid ().value ());
                            this->loadProject (mProje.oid ().value ());
                            this->showPopUpMessage ("<p>Proje Dosyası Silindi</p>");
                            this->removeDialog (mDialog);
                        }

                    }else{
                        for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                            filter.addProje (mProje[i].value ());
                        }
                        filter.removeProje (bsoncxx::oid{fileoidText->text ().toUTF8 ()});

                        auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                        if( !upt ){
                            this->showPopUpMessage ("<p>Dosya Güncellenemedi</p>"
                                                    "<p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                            return;
                        }
                        this->addIslemLog (mProje[i].value ().projeAdi () + " Dosyası Silindi",mProje.oid ().value ());

                        this->removeDialog (mDialog);
                        this->loadProject (mProje.oid ().value ());
                    }


                });
            });

            auto projeDosyaIndirlBtn = mDialog->contents ()->addWidget (cpp14::make_unique<WPushButton>("Dosyayı İndir"));
            projeDosyaIndirlBtn->setMargin (5,Side::Left|Side::Right);
            projeDosyaIndirlBtn->addStyleClass (Bootstrap::Button::Link);
            projeDosyaIndirlBtn->clicked ().connect ([=](){
                auto fileUrl = SerikBLDCore::Imar::BaseProjeManager::downloadFileWeb (mProje[i].value ().fileOid ().c_str ());
                this->doJavaScript ("window.open('"+fileUrl+"','_blank');");
                removeDialog (mDialog);
            });



            mDialog->show ();

        });




    }


    {
        auto projectFileContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        projectFileContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                             Bootstrap::Grid::Medium::col_md_2+
                                             Bootstrap::Grid::Small::col_sm_3+
                                             Bootstrap::Grid::ExtraSmall::col_xs_4+
                                             Bootstrap::ImageShape::img_thumbnail);
        projectFileContainer->addStyleClass (Bootstrap::Button::info);
        projectFileContainer->setMargin (5,Side::Top);
        projectFileContainer->setHeight (200);
        auto yeniEkleText = projectFileContainer->addWidget (cpp14::make_unique<WText>("<b>Yeni Proje Dosyası Ekle</b>"));
        yeniEkleText->setAttributeValue (Style::style,Style::font::family::dosis+
                                         Style::color::color (Style::color::White::Snow));
        projectFileContainer->decorationStyle ().setCursor (Cursor::PointingHand);


        // TODO: yeni Proje Dosyası Ekleme
        projectFileContainer->clicked ().connect ([=](){

            auto mDialog = createDialog ("Yeni Dosya Ekle");

            mDialog->contents ()->clear ();


            mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Proje Dosyasını Ekle"));
            auto fileNameLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
            fileNameLineEdit->setPlaceholderText ("Proje Adını Giriniz");

            auto fileuploader = mDialog->contents ()->addWidget (cpp14::make_unique<FileUploaderWidget>("Yeni Proje Dosyası"));


            auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
            svBtn->clicked ().connect ([=](){
                if( !fileuploader->isUploaded () ){
                    this->showPopUpMessage ("Yeni Dosya Yüklemediniz!");
                    return;
                }


                SerikBLDCore::Imar::MimariProje::BaseProject filter;
                filter.setOid (mProje.oid ().value ());

                for( auto i = 0 ; i < mProje.projeCount () ; i++ ){
                    filter.addProje (mProje[i].value ());
                }

                SerikBLDCore::Imar::MimariProje::FileProject newFileProject;
                newFileProject.setOnay (false);
                auto newFileOid = SerikBLDCore::Imar::BaseProjeManager::getDB ()->uploadfile (fileuploader->fileLocation ().toStdString ().c_str ());
                newFileProject.setFileOid (newFileOid.view().get_oid ().value);
                newFileProject.setProjeAdi (fileNameLineEdit->text ().toUTF8 ());
                filter.addProje (newFileProject);
                auto upt = SerikBLDCore::Imar::BaseProjeManager::UpdateItem (filter);
                if( !upt ){
                    this->showPopUpMessage ("<p>Dosya Güncellenemedi</p><p>Sürekli Aynı Hatayı Alıyorsanız Lütfen İlgili Kişi ile İrtibata Geçiniz</p>");
                    return;
                }else{
                    this->addIslemLog (fileNameLineEdit->text ().toUTF8 () + " Dosyası Eklendi",mProje.oid ().value ());
                }
                this->removeDialog (mDialog);
                this->loadProject (mProje.oid ().value ());
            });

            mDialog->show ();

        });
    }

}
