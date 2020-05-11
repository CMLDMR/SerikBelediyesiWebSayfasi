#include "firmamanagerpage.h"


v2::FirmaManagerPage::FirmaManagerPage(SerikBLDCore::User *_user)
    :SerikBLDCore::Firma::FirmaManager (_user->getDB ()),
      ContainerWidget ("Firma Yönetimi"),
      mUser(_user)
{

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
            showPopUpMessage ("Arama Eklenecek, " + firmaAraLineEdit->text ().toUTF8 ());
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
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto firmaText = container->addWidget (cpp14::make_unique<WText>("<b>#Telefon</b>"));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Hidden::hidden_sm+
                                      Bootstrap::Grid::Hidden::hidden_xs);
            auto firmaText = container->addWidget (cpp14::make_unique<WText>("<b>#eMail</b>"));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Hidden::hidden_sm+
                                      Bootstrap::Grid::Hidden::hidden_xs);
            auto firmaText = container->addWidget (cpp14::make_unique<WText>("<b>#VergiNo</b>"));
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

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto firmaText = container->addWidget (cpp14::make_unique<WText>(item.name ()));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto firmaText = container->addWidget (cpp14::make_unique<WText>(item.telefon ()));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Hidden::hidden_sm+
                                      Bootstrap::Grid::Hidden::hidden_xs);
            auto firmaText = container->addWidget (cpp14::make_unique<WText>(item.mail ()));
        }

        {
            auto container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Hidden::hidden_sm+
                                      Bootstrap::Grid::Hidden::hidden_xs);
            auto firmaText = container->addWidget (cpp14::make_unique<WText>(item.vergiNo ()));
        }


    }

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
