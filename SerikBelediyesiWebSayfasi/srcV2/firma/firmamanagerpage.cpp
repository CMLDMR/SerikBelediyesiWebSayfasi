#include "firmamanagerpage.h"


v2::FirmaManagerPage::FirmaManagerPage(SerikBLDCore::User *_user)
    :SerikBLDCore::Firma::FirmaManager (_user->getDB ()),
      ContainerWidget ("Firma Yönetimi"),
      mUser(_user)
{

    mTCManager = new SerikBLDCore::TCManagerV2(mUser->getDB ());

    {
        auto yeniKayitBtn = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        yeniKayitBtn->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                     Bootstrap::Grid::Medium::col_md_2+
                                     Bootstrap::Grid::Small::col_sm_3+
                                     Bootstrap::Grid::ExtraSmall::col_xs_3);
        auto yeniBtn = yeniKayitBtn->addWidget (cpp14::make_unique<WPushButton>("Yeni Kayıt"));

        yeniBtn->clicked ().connect (this,&v2::FirmaManagerPage::yeniFirmaKayitDialog );
    }

    {
        auto aramaTipiContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        aramaTipiContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                           Bootstrap::Grid::Medium::col_md_4+
                                           Bootstrap::Grid::Small::col_sm_3+
                                           Bootstrap::Grid::ExtraSmall::col_xs_3);
        auto aramaTipiComboBox = aramaTipiContainer->addWidget (cpp14::make_unique<WComboBox>());
        aramaTipiComboBox->addItem ("Firma Adına Göre");
        aramaTipiComboBox->addItem ("Telefona Göre");
        aramaTipiComboBox->addItem ("eMaile Göre");
        aramaTipiComboBox->addItem ("VergiNoya Göre");


        auto yeniKayitBtn = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        yeniKayitBtn->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                     Bootstrap::Grid::Medium::col_md_4+
                                     Bootstrap::Grid::Small::col_sm_3+
                                     Bootstrap::Grid::ExtraSmall::col_xs_3);
        auto firmaAraLineEdit = yeniKayitBtn->addWidget (cpp14::make_unique<WLineEdit>());
        firmaAraLineEdit->setPlaceholderText ("Aranacak Firma Adını Giriniz");

        auto araBtn = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        araBtn->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                               Bootstrap::Grid::Medium::col_md_2+
                               Bootstrap::Grid::Small::col_sm_3+
                               Bootstrap::Grid::ExtraSmall::col_xs_3);
        auto yeniBtn = araBtn->addWidget (cpp14::make_unique<WPushButton>("Ara"));
        yeniBtn->addStyleClass (Bootstrap::Button::Primary);
        yeniBtn->clicked ().connect ([=](){

            SerikBLDCore::Firma::FirmaItem filter;

            if( aramaTipiComboBox->currentIndex () == 0 )
            {
                filter.append(SerikBLDCore::Firma::Key::firmaAdi,make_document(kvp("$regex",firmaAraLineEdit->text ().toUTF8 ()),kvp("$options","i")));
            }
            if( aramaTipiComboBox->currentIndex () == 1 )
            {
                filter.append(SerikBLDCore::Firma::Key::telefon,make_document(kvp("$regex",firmaAraLineEdit->text ().toUTF8 ()),kvp("$options","i")));
            }
            if( aramaTipiComboBox->currentIndex () == 2 )
            {
                filter.append(SerikBLDCore::Firma::Key::email,make_document(kvp("$regex",firmaAraLineEdit->text ().toUTF8 ()),kvp("$options","i")));
            }
            if( aramaTipiComboBox->currentIndex () == 3 )
            {
                filter.append(SerikBLDCore::Firma::Key::vergiNo,make_document(kvp("$regex",firmaAraLineEdit->text ().toUTF8 ()),kvp("$options","i")));
            }

            this->UpdateList (filter);

        });

    }

    setLimit (10000);

    this->UpdateList ();

}

void v2::FirmaManagerPage::onList(const QVector<SerikBLDCore::Firma::FirmaItem> *mlist)
{

    Content ()->clear ();

    {
        auto rContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row+
                                   " boxShadow");
        rContainer->setMargin (10,Side::Top|Side::Bottom);
        rContainer->setPadding (5,Side::Top|Side::Bottom);
        rContainer->setHeight (44);


        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
            container->addWidget (cpp14::make_unique<WText>("<b>#Firma Adı</b>"));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_5+
                                      Bootstrap::Grid::ExtraSmall::col_xs_5);
            container->addWidget (cpp14::make_unique<WText>("<b>#Telefon</b>"));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Hidden::hidden_sm+
                                      Bootstrap::Grid::Hidden::hidden_xs);
            container->addWidget (cpp14::make_unique<WText>("<b>#eMail</b>"));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Hidden::hidden_sm+
                                      Bootstrap::Grid::Hidden::hidden_xs);
            container->addWidget (cpp14::make_unique<WText>("<b>#VergiNo</b>"));
        }


    }


    for( auto item : *mlist ){

        auto rContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row+
                                   " boxShadow");
        rContainer->setMargin (10,Side::Top|Side::Bottom);
        rContainer->setPadding (5,Side::Top|Side::Bottom);
        rContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (210,240),
                                                                                   this->getRandom (215,245),
                                                                                   this->getRandom (225,255)));
        rContainer->setHeight (45);
        rContainer->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
            container->addWidget (cpp14::make_unique<WText>(item.name ()));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_5+
                                      Bootstrap::Grid::ExtraSmall::col_xs_5);
            container->addWidget (cpp14::make_unique<WText>(item.telefon ()));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Hidden::hidden_sm+
                                      Bootstrap::Grid::Hidden::hidden_xs);
            container->addWidget (cpp14::make_unique<WText>(item.mail ()));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Hidden::hidden_sm+
                                      Bootstrap::Grid::Hidden::hidden_xs);
            container->addWidget (cpp14::make_unique<WText>(item.vergiNo ()));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                      Bootstrap::Grid::Medium::col_md_1+
                                      Bootstrap::Grid::Small::col_sm_1+
                                      Bootstrap::Grid::ExtraSmall::col_xs_1);
            auto popmenu =  (cpp14::make_unique<WPopupMenu>());

            auto silmenu = popmenu->addItem("Sil");
            silmenu->decorationStyle ().setCursor (Cursor::PointingHand);
            silmenu->triggered().connect([=] {
                auto btn = askConfirm ("Silmek İstediğinize Eminmisiniz");
                btn->clicked ().connect ([=](){

                    SerikBLDCore::Firma::FirmaItem filter;
                    filter.setOid (rContainer->attributeValue (Style::dataoid).toUTF8 ());

                    if( this->DeleteItem (filter) ){
                        this->showPopUpMessage ("Silindi");
                        this->UpdateList ();
                    }else{
                        this->showPopUpMessage ("Bir Hata Oluştu Firma Silinmedi","err");
                    }
                });
            });

            auto degistirmenu = popmenu->addItem("**Bilgileri Değiştir");
            degistirmenu->decorationStyle ().setCursor (Cursor::PointingHand);
            degistirmenu->triggered().connect([=] {
                showPopUpMessage ("Degistirmenu");
            });

            auto yetkiliAtaMenu = popmenu->addItem("Yetkili Ata");
            yetkiliAtaMenu->decorationStyle ().setCursor (Cursor::PointingHand);
            yetkiliAtaMenu->triggered().connect([=](){
                this->yetkiliAtaDialog (rContainer->attributeValue (Style::dataoid).toUTF8 ());
            });


            auto yetkiliKisiMenu = popmenu->addItem("Yetkili Kişi");
            yetkiliKisiMenu->decorationStyle ().setCursor (Cursor::PointingHand);
            yetkiliKisiMenu->setAttributeValue (Style::dataoid,item.yetkiliOid ());
            yetkiliKisiMenu->triggered().connect([=] {
                this->yetkiliGoster (yetkiliKisiMenu->attributeValue (Style::dataoid).toUTF8 ());
            });

            auto projelerMenu = popmenu->addItem("**Projeleri");
            projelerMenu->decorationStyle ().setCursor (Cursor::PointingHand);
            projelerMenu->triggered().connect([=] {
                showPopUpMessage ("<p>ProjelerMenu...</p>");
            });

            auto kaydedenMenu = popmenu->addItem("**Kaydeden Personel");
            kaydedenMenu->decorationStyle ().setCursor (Cursor::PointingHand);
            kaydedenMenu->triggered().connect([=] {
                showPopUpMessage ("<p>KaydedenMenu...</p>");
            });

            auto kayitTarihi = popmenu->addItem("**Kayıt Tarihi");
            kayitTarihi->decorationStyle ().setCursor (Cursor::PointingHand);
            kayitTarihi->triggered().connect([=] {
                showPopUpMessage ("<p>KayitTarihi...</p>");
            });

            Wt::WPushButton *button = container->addNew<Wt::WPushButton>();
            button->setMenu(std::move(popmenu));
            button->addStyleClass (Bootstrap::Button::Primary);
        }


    }

    showPopUpMessage ("Liste Güncellendi");

}

void v2::FirmaManagerPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (errorText);
}

void v2::FirmaManagerPage::yeniFirmaKayitDialog()
{
    auto mDialog = createDialog ("Yeni Firma Kayıt Yap");



    auto firmaAdiLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    firmaAdiLineEdit->setPlaceholderText ("Firma Adını Giriniz");

    firmaAdiLineEdit->changed ().connect ([=](){
        // Set options for email address suggestions:



    });


    Wt::WSuggestionPopup::Options contactOptions;
    contactOptions.highlightBeginTag = "<span class=\"highlight\">";
    contactOptions.highlightEndTag = "</span>";
    contactOptions.listSeparator = ',';
    contactOptions.whitespace = " \\n";
    contactOptions.wordSeparators = "-., \"@\\n;";
    contactOptions.appendReplacedText = ", ";

    Wt::WSuggestionPopup *sp = mDialog->contents ()->addNew<Wt::WSuggestionPopup>(
                Wt::WSuggestionPopup::generateMatcherJS(contactOptions),
                Wt::WSuggestionPopup::generateReplacerJS(contactOptions));

    sp->forEdit(firmaAdiLineEdit);

    for( auto item : this->List () )
    {
        sp->addSuggestion(item.name ());
    }

    auto vergiNoLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    vergiNoLineEdit->setPlaceholderText ("Vergi Numarasını Giriniz");

    auto telefonLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    telefonLineEdit->setPlaceholderText ("Telefon Numarasını Giriniz");

    auto mailLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    mailLineEdit->setPlaceholderText ("e-Mail Adresini Giriniz");

    auto adresLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    adresLineEdit->setPlaceholderText ("Adres Giriniz");

    auto svBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
    svBtn->addStyleClass (Bootstrap::Button::Primary);

    svBtn->clicked ().connect ([=](){
        if( !this->existFirmaKey (SerikBLDCore::Firma::Key::firmaAdi,firmaAdiLineEdit->text ().toUTF8 ()) ){
            if( !this->existFirmaKey (SerikBLDCore::Firma::Key::telefon,telefonLineEdit->text ().toUTF8 ()) ){
                if( !this->existFirmaKey (SerikBLDCore::Firma::Key::email,mailLineEdit->text ().toUTF8 ()) ){
                    if( !this->existFirmaKey (SerikBLDCore::Firma::Key::vergiNo,vergiNoLineEdit->text ().toUTF8 ()) ){
                        SerikBLDCore::Firma::FirmaItem newFirma;
                        newFirma.setName (firmaAdiLineEdit->text ().toUTF8 ());
                        newFirma.setMail (mailLineEdit->text ().toUTF8 ());
                        newFirma.setAdres (adresLineEdit->text ().toUTF8 ());
                        newFirma.setTelefon (telefonLineEdit->text ().toUTF8 ());
                        newFirma.setKaydeden (this->mUser->oid ().value (),QDateTime::currentDateTime ().toMSecsSinceEpoch ());
                        newFirma.setVergi (vergiNoLineEdit->text ().toUTF8 ());
                        if( this->InsertItem (newFirma).size () )
                        {
                            wApp->instance()->root()->removeChild(mDialog);
                            this->UpdateList ();
                        }
                    }else{
                        this->showPopUpMessage ("Bu Vergi No Kayıtlı","err");
                    }
                }else{
                    this->showPopUpMessage ("Bu Mail Adresi Kayıtlı","err");
                }
            }else{
                this->showPopUpMessage ("Bu Telefon No Kayıtlı","err");
            }
        }else{
            this->showPopUpMessage ("Bu Firma Adı Kayıtlı","err");
        }
    });
    mDialog->show ();

}

void v2::FirmaManagerPage::yetkiliAtaDialog(const std::string &firmaOid)
{
    auto mDialog = createDialog ("Yeni Kişi Ata");

    mDialog->contents ()->setOverflow (Overflow::Auto);
    mDialog->setHeight (400);

    auto Searchcontainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
    Searchcontainer->addStyleClass (Bootstrap::Grid::row);

    auto tcadsoyadContainer = Searchcontainer->addWidget (cpp14::make_unique<WContainerWidget>());
    tcadsoyadContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+
                                       Bootstrap::Grid::Medium::col_md_8+
                                       Bootstrap::Grid::Small::col_sm_7+
                                       Bootstrap::Grid::ExtraSmall::col_xs_6);
    auto tcadsoyadLineEdit = tcadsoyadContainer->addWidget (cpp14::make_unique<WLineEdit>());
    tcadsoyadLineEdit->setPlaceholderText ("En Az 3 Harf Giriniz");

    auto araContainer = Searchcontainer->addWidget (cpp14::make_unique<WContainerWidget>());
    araContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                 Bootstrap::Grid::Medium::col_md_4+
                                 Bootstrap::Grid::Small::col_sm_5+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);

    auto araBtn = araContainer->addWidget (cpp14::make_unique<WPushButton>("Ara"));
    araBtn->addStyleClass (Bootstrap::Button::Primary);


    auto listContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
    listContainer->addStyleClass (Bootstrap::Grid::row);
    listContainer->setMaximumSize (500,WLength::Auto);



    auto infoContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
    infoContainer->addStyleClass (Bootstrap::Grid::row);

    auto infoText = infoContainer->addWidget (cpp14::make_unique<WText>(""));

    araBtn->clicked ().connect ([=](){

        if( tcadsoyadLineEdit->text ().toUTF8 ().size () < 3 )
        {
            infoText->setText ("<b>En Az 3 Harf Giriniz</b>");
            infoText->setAttributeValue (Style::style,Style::color::color (Style::color::Red::Red));
            return;
        }
        infoText->setText ("");

        SerikBLDCore::TC filter;

        filter.append(SerikBLDCore::TC::KeyAdSoyad,
                      make_document(kvp("$regex",tcadsoyadLineEdit->text ().toUTF8 ()),
                                    kvp("$options","i")));


        auto cursor = mTCManager->UpdateList (filter);

        listContainer->clear ();


        for( auto item : cursor )
        {
            auto container = listContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::row+" boxShadow");
            container->setMargin (10,Side::Top|Side::Bottom);



            auto adSoyadContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
            adSoyadContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                             Bootstrap::Grid::Medium::col_md_6+
                                             Bootstrap::Grid::Small::col_sm_6+
                                             Bootstrap::Grid::ExtraSmall::col_xs_6);
            adSoyadContainer->addWidget (cpp14::make_unique<WText>(item.AdSoyad ().toStdString ()));


            auto telefonContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
            telefonContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                             Bootstrap::Grid::Medium::col_md_5+
                                             Bootstrap::Grid::Small::col_sm_4+
                                             Bootstrap::Grid::ExtraSmall::col_xs_4);
            auto telefonText = item.CepTelefonu ();

            if( telefonText.count () > 8 )
            {
                telefonText.replace (4,3,"***");
            }

            telefonContainer->addWidget (cpp14::make_unique<WText>(telefonText.toStdString ()));

            auto secContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
            secContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                         Bootstrap::Grid::Medium::col_md_1+
                                         Bootstrap::Grid::Small::col_sm_2+
                                         Bootstrap::Grid::ExtraSmall::col_xs_2);

            secContainer->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());

            auto secBtn = secContainer->addWidget (cpp14::make_unique<WPushButton>("Seç"));
            secBtn->addStyleClass (Bootstrap::Button::Primary);
            secBtn->clicked ().connect ([=](){

                SerikBLDCore::Firma::FirmaItem filter;
                filter.setOid (firmaOid);
                filter.setYetkiliOid (bsoncxx::oid{secContainer->attributeValue (Style::dataoid).toUTF8 ()});

                if( this->UpdateItem (filter) ){
                    this->remogeDialog (mDialog);
                }else{
                    infoText->setText ("<b>Atama Yapılamadı. Daha Sonra Tekrar Deneyin</b>");
                    infoText->setAttributeValue (Style::style,Style::color::color (Style::color::Red::Red));
                }

            });
        }



    });


    mDialog->show ();
}

void v2::FirmaManagerPage::yetkiliGoster(const std::string &vatandasOid)
{


    auto tcItem = mTCManager->Load_byOID (vatandasOid);

    if( tcItem )
    {
        auto mDialog = createDialog ("Yetkili Kişi");

        mDialog->contents ()->addWidget (cpp14::make_unique<WText>(tcItem.value ()->AdSoyad ().toStdString ()));
        mDialog->contents ()->addWidget (cpp14::make_unique<WBreak>());

        auto telefonText = tcItem.value ()->CepTelefonu ();
        if( telefonText.count () > 8 )
        {
            telefonText.replace (4,3,"***");
        }
        mDialog->contents ()->addWidget (cpp14::make_unique<WText>(telefonText.toStdString ()));
        mDialog->contents ()->addWidget (cpp14::make_unique<WBreak>());

        mDialog->show ();
    }else{
        this->showPopUpMessage ("Bir Hata Oluştu. Yetkili Kişi Yüklenemiyor");
    }




}
