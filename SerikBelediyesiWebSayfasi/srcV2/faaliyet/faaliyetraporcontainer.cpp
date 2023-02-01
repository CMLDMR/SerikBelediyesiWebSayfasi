#include "faaliyetraporcontainer.h"

v2::Faaliyet::FaaliyetRaporContainer::FaaliyetRaporContainer( SerikBLDCore::User *_mUser )
    :SerikBLDCore::Faaliyet::Manager ( _mUser->getDB () ),mUser(_mUser),mPersonelManager(new SerikBLDCore::PersonelManager(_mUser->getDB ()))
{
    mGirisBilgiManager = new SerikBLDCore::Faaliyet::GirisBilgileri::GirisBilgileriManager(this->getDB());
    this->initMenu(MenuPage::Raporlar);
}

void v2::Faaliyet::FaaliyetRaporContainer::initMenu(const MenuPage &currentPage)
{

    this->Header()->clear();


    auto RaporlarMenuBtn = this->Header()->addNew<WContainerWidget>();
    RaporlarMenuBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                   Bootstrap::Grid::Medium::col_md_6+
                                   Bootstrap::Grid::Small::col_sm_6+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
    RaporlarMenuBtn->addNew<WText>("<h4>Raporlar</h4>",TextFormat::UnsafeXHTML);
    RaporlarMenuBtn->addStyleClass(Bootstrap::ContextualBackGround::bg_info);
    RaporlarMenuBtn->setPadding(5,Side::Top|Side::Bottom);
    RaporlarMenuBtn->decorationStyle ().setCursor (Cursor::PointingHand);

    RaporlarMenuBtn->setMargin(15,Side::Bottom);
    RaporlarMenuBtn->clicked().connect([=](){
        this->initMenu(MenuPage::Raporlar);
    });

    auto GirisMenuBtn = this->Header()->addNew<WContainerWidget>();
    GirisMenuBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                Bootstrap::Grid::Medium::col_md_6+
                                Bootstrap::Grid::Small::col_sm_6+
                                Bootstrap::Grid::ExtraSmall::col_xs_6);
    GirisMenuBtn->addNew<WText>("<h4>Giriş Bilgileri</h4>",TextFormat::UnsafeXHTML);
    GirisMenuBtn->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);
    GirisMenuBtn->setPadding(5,Side::Top|Side::Bottom);
    GirisMenuBtn->setMargin(15,Side::Bottom);
    GirisMenuBtn->decorationStyle ().setCursor (Cursor::PointingHand);

    GirisMenuBtn->clicked().connect([=](){
        //TODO: Giriş Bilgileri Eklenecek
        this->showPopUpMessage("Yapım Aşamasında","warn");
//        this->initMenu(MenuPage::GirisBilgileri);
    });

    if( currentPage == MenuPage::Raporlar ){

        if( this->mUser->Statu () == SerikBLDCore::IK::Statu::Baskan
                || this->mUser->Statu () == SerikBLDCore::IK::Statu::BaskanYardimcisi
                || this->mUser->Birimi () == "Yazı İşleri Müdürlüğü"){

            auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::col_full_12);
            container->addStyleClass (CSSStyle::Gradient::grayGradient90);

            auto hLayout = container->setLayout (cpp14::make_unique<WHBoxLayout>());
            auto text = hLayout->addWidget(cpp14::make_unique<WText>("Yıl Seç"));
            text->setWidth (WLength("50%"));
            text->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black));
            text->setMargin (6,Side::Top|Side::Bottom);

            mYillar = hLayout->addWidget (cpp14::make_unique<WComboBox>());
            mYillar->setWidth(WLength("50%"));

            mYillar->addItem("2020");
            mYillar->addItem("2021");
            mYillar->addItem("2022");
            mYillar->addItem("2023");
            mYillar->addItem("2024");
            mYillar->addItem("2025");
            mYillar->addItem("2026");
            mYillar->addItem("2027");
            mYillar->addItem("2028");
            mYillar->addItem("2029");
            mYillar->addItem("2030");
            mYillar->addItem("2031");
            mYillar->addItem("2032");

            mYillar->activated ().connect ([=](const int &value ){
                this->initFaaliyetRaporlari ();
            });

            auto mBtn2020 = hLayout->addWidget(cpp14::make_unique<WPushButton>("Listeye Dön"));
            mBtn2020->addStyleClass(Bootstrap::Button::Primary);
            mBtn2020->clicked().connect([=](){
                this->initFaaliyetRaporlari ();
            });
        }
        this->initFaaliyetRaporlari ();
    }


    if( currentPage == MenuPage::GirisBilgileri ){
        if( this->mUser->Statu () == SerikBLDCore::IK::Statu::Baskan
                || this->mUser->Statu () == SerikBLDCore::IK::Statu::BaskanYardimcisi
                || this->mUser->Birimi () == "Yazı İşleri Müdürlüğü"){
            auto container = Header()->addWidget (cpp14::make_unique<WContainerWidget>());


            container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_primary);

            container->addWidget (cpp14::make_unique<WText>("Yeni Faaliyet Girişi Ekle<b>+</b>"));
            container->addStyleClass (CSSStyle::Shadows::shadow8px);
            container->setPadding (5,Side::Top|Side::Bottom);
            container->setMargin (50,Side::Top|Side::Bottom);
            container->decorationStyle ().setCursor (Cursor::PointingHand);

            container->clicked().connect(this,&v2::Faaliyet::FaaliyetRaporContainer::initNewFaaliyetGiris);

            initFaaliyetGirisBilgileri();


        }
    }
}

void v2::Faaliyet::FaaliyetRaporContainer::initFaaliyetRaporlari(const int64_t currentYear)
{

    Content ()->setMargin (25,Side::Top|Side::Bottom);

    this->Content ()->clear ();

    // Başkan ve Başkan Yardımcısı Olmayanlar Sadece Görüntüleme Yapabilirler
    if( this->mUser->Statu () != SerikBLDCore::IK::Statu::Baskan && this->mUser->Statu () != SerikBLDCore::IK::Statu::BaskanYardimcisi ){
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ContextualBackGround::bg_primary);

        container->addWidget (cpp14::make_unique<WText>("Yeni Faaliyet Ekle<b>+</b>"));
        container->addStyleClass (CSSStyle::Shadows::shadow8px);
        container->setPadding (5,Side::Top|Side::Bottom);
        container->setMargin (50,Side::Top|Side::Bottom);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect ([=](){

            auto mDialog = createDialog ("Yeni Faaliyet Ekle");
            mDialog->contents ()->setContentAlignment (AlignmentFlag::Center);
            auto birimComboBox = mDialog->contents()->addNew<WComboBox>();

            auto birimList = this->getBirimler();
            for( const auto &item : birimList ){
                birimComboBox->addItem(item.toStdString());
                if( mUser->Birimi() == item.toStdString() ){
                    birimComboBox->setCurrentIndex(birimComboBox->count());
                }
            }

            if( mUser->Birimi() != "Yazı İşleri Müdürlüğü" ){
                birimComboBox->setEnabled(false);
            }

            mDialog->contents ()->addWidget (cpp14::make_unique<WBreak>());

            mDialog->contents ()->addWidget (cpp14::make_unique<WText>("Yıl Seçiniz"));

            mDialog->contents ()->addWidget (cpp14::make_unique<WBreak>());
            auto yilSpinBoxContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WSpinBox>());
            yilSpinBoxContainer->setMinimum (2000);
            yilSpinBoxContainer->setMaximum (2030);
            yilSpinBoxContainer->setValue (WDate::currentDate ().year ()-1);

            auto savebtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet") );

            savebtn->clicked ().connect ([=](){

                SerikBLDCore::Faaliyet::FaaliyetItem item;
                item.setBirim (birimComboBox->currentText().toUTF8());
                item.setYil (yilSpinBoxContainer->value ());
                item.setEnableViewMode(false);

                if( this->insertFaaliyetItem (item) ){
                    this->initFaaliyetRaporlari ();
                }

                removeDialog (mDialog);

            });

        });
    }


    if( this->mUser->Statu () == SerikBLDCore::IK::Statu::Baskan || this->mUser->Statu () == SerikBLDCore::IK::Statu::BaskanYardimcisi || this->mUser->Birimi () == "Yazı İşleri Müdürlüğü"){

        auto list = this->ListFaaliyetItem (std::stoi (mYillar->currentText ().toUTF8 ()));

        auto birimList = this->mPersonelManager->birimListResmi ();

        QVector<QString> existList ;


        for( const auto &item : list ){
            auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_9+
                                      Bootstrap::Grid::Medium::col_md_9+
                                      Bootstrap::Grid::Small::col_sm_9+
                                      Bootstrap::Grid::ExtraSmall::col_xs_9);

            container->addStyleClass (CSSStyle::Shadows::shadow8px);
            container->setPadding (5,Side::Top|Side::Bottom);
            container->setMargin (5,Side::Top|Side::Bottom);
            container->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
            container->addStyleClass (CSSStyle::Button::blueButton);


            auto hLayout = container->addWidget (cpp14::make_unique<WContainerWidget>());
            hLayout->addStyleClass (Bootstrap::Grid::row);

            auto text = hLayout->addWidget (cpp14::make_unique<WText>(item.getBirim () + " - " + std::to_string (item.getYil ()) + " : Faaliyet Raporu" ));

            text->addStyleClass (Bootstrap::Grid::col_full_12);

            container->decorationStyle ().setCursor (Cursor::PointingHand);


            container->clicked ().connect ([=](){
                this->itemList.clearItems();

                this->testPage (text->text ().toUTF8 (),container->attributeValue (Style::dataoid).toUTF8());
            });

            existList.append (item.getBirim ().c_str ());

            auto setEnableDisableBtnContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            setEnableDisableBtnContainer->addStyleClass (Bootstrap::Grid::col_full_12);
            setEnableDisableBtnContainer->setPadding (5,Side::Top|Side::Bottom);
            setEnableDisableBtnContainer->setMargin (5,Side::Top|Side::Bottom);
            setEnableDisableBtnContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                                         Bootstrap::Grid::Medium::col_md_3+
                                                         Bootstrap::Grid::Small::col_sm_3+
                                                         Bootstrap::Grid::ExtraSmall::col_xs_3);
            setEnableDisableBtnContainer->addStyleClass (CSSStyle::Button::grayButton);


            SerikBLDCore::Faaliyet::FaaliyetItem filter;
            filter.setOid(container->attributeValue (Style::dataoid).toUTF8());

            if( item.ViewModeisEnabled() ){
                setEnableDisableBtnContainer->addWidget(cpp14::make_unique<WText>("Okuma Modu"));
                setEnableDisableBtnContainer->addStyleClass (CSSStyle::Button::grayButton);
                filter.setEnableViewMode(false);

            }else{
                setEnableDisableBtnContainer->addWidget(cpp14::make_unique<WText>("Okuma/Yazma Modu"));
                setEnableDisableBtnContainer->addStyleClass (CSSStyle::Button::Red::DarkRedButton);
                filter.setEnableViewMode(false);
            }

            if( item.getYil() < QDate::currentDate().year()-1 ){
                if( !item.ViewModeisEnabled() ){
                    SerikBLDCore::Faaliyet::FaaliyetItem filter;
                    filter.setOid(container->attributeValue (Style::dataoid).toUTF8());
                    filter.setEnableViewMode(true);
                    auto upt = this->updateItem(filter);
                    if( upt ){
                        if( upt.value().modified_count() ){
                            this->initFaaliyetRaporlari();
                            return;
                        }
                    }
                }
                setEnableDisableBtnContainer->setDisabled(true);
                setEnableDisableBtnContainer->clear();
                setEnableDisableBtnContainer->addWidget(cpp14::make_unique<WText>("Kilitli"));

            }else{
                setEnableDisableBtnContainer->clicked().connect([=](){
                    SerikBLDCore::Faaliyet::FaaliyetItem filter;
                    filter.setOid(container->attributeValue (Style::dataoid).toUTF8());
                    if( item.ViewModeisEnabled() ){
                        filter.setEnableViewMode(false);
                    }else{
                        filter.setEnableViewMode(true);
                    }
                    auto upt = this->updateItem(filter);
                    if( upt ){
                        if( upt.value().modified_count() ){
                            this->initFaaliyetRaporlari();
                            return;
                        }
                    }
                });
            }
        }

        {
            auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::col_full_12);
            container->setMargin(50,Side::Top);
            container->setContentAlignment (AlignmentFlag::Center);
            container->addWidget (cpp14::make_unique<WText>("Sisteme Girmeyen Birimler"));
        }

        QVector<QString> newExistList;

        for( const auto &birimItem : birimList ){

            if( birimItem == "Gelir Şefliği" || birimItem == "Başkanlık" || birimItem == "Antalya Valiliği" || birimItem == "Muhtarlık" || birimItem == "Antalya Büyükşehir Belediyesi" || birimItem == "ASAT" || birimItem == "Test" ) {

            }else{
                newExistList.append(birimItem);
            }
        }




        for( const auto &birimItem : newExistList ){
            if( !existList.contains (birimItem) ){
                auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->addStyleClass (Bootstrap::Grid::col_full_12);

                auto hLayout = container->setLayout (cpp14::make_unique<WHBoxLayout>());

                hLayout->addWidget (cpp14::make_unique<WText>(birimItem.toStdString() + " Faaliyet Raporu Yok" ));

                container->addStyleClass (CSSStyle::Shadows::shadow8px);
                container->setPadding (5,Side::Top|Side::Bottom);
                container->setMargin (5,Side::Top|Side::Bottom);
                container->addStyleClass (CSSStyle::Gradient::redGradient);
            }
        }

    }else{
        //        LOG << "ListFaaliyetItem : " << this->mUser->Birimi() << "\n";

        auto list = this->ListFaaliyetItem (this->mUser->Birimi ());

        for( const auto &item : list ){

            LOG << item.oid().value().to_string()<<"\n";

            auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::col_full_12);

            auto hLayout = container->setLayout (cpp14::make_unique<WHBoxLayout>());

            auto text = hLayout->addWidget (cpp14::make_unique<WText>(std::to_string (item.getYil ()) + " : Faaliyet Raporu" ));
            //            text->addStyleClass (Bootstrap::Button::Primary);

            container->addStyleClass (CSSStyle::Shadows::shadow8px);
            container->setPadding (5,Side::Top|Side::Bottom);
            container->setMargin (5,Side::Top|Side::Bottom);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
            container->addStyleClass (CSSStyle::Button::blueButton);

            text->clicked ().connect ([=](){
                this->itemList.clearItems();
                this->testPage (text->text ().toUTF8 (),container->attributeValue (Style::dataoid).toUTF8());
            });
        }
    }





}

void v2::Faaliyet::FaaliyetRaporContainer::initFaaliyetGirisBilgileri()
{
    this->Content()->clear();
}

void v2::Faaliyet::FaaliyetRaporContainer::initNewFaaliyetGiris()
{

    this->Content()->clear();

    auto createContainer = [=](const std::string &title){
        auto container = cpp14::make_unique<WContainerWidget>();
        container->setPadding(10,AllSides);
        container->addStyleClass(Bootstrap::Grid::col_full_6);
        container->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(175,225),
                                                                                this->getRandom(225,255),
                                                                                this->getRandom(225,255)));
        container->addNew<WText>("<b>"+title+"</b>",TextFormat::UnsafeXHTML);
        container->addNew<WBreak>();
        container->setMargin(15,Side::Top|Side::Bottom);
        return container;
    };

    auto yilContainer = createContainer("Yıl Seçimi");
    auto yilSpinBox = yilContainer->addNew<WSpinBox>();
    yilSpinBox->setMaximum(2050);
    yilSpinBox->setMinimum(WDate::currentDate().year()-1);
    yilSpinBox->setValue(WDate::currentDate().year());
    Content()->addWidget(std::move(yilContainer));



    auto logoPathContainer = createContainer("Belediye Logo Seçimi");
    auto logoImgContainer = logoPathContainer->addNew<WImage>();
    logoImgContainer->setWidth(WLength("100%"));
    auto logoContainer = logoPathContainer->addNew<FileUploaderWidget>("Resim Yükle");
    logoContainer->Uploaded().connect([=](){
        logoImgContainer->setImageLink(WLink(logoContainer->doocRootLocation().toStdString()));
        logoImgContainer->setHeight(350);
    });
    Content()->addWidget(std::move(logoPathContainer));


    auto AtaturkPathContainer = createContainer("Ataturk Giriş Resmi Seçimi");

    Content()->addWidget(std::move(AtaturkPathContainer));


    auto CumhurBaskaniPathContainer = createContainer("Cumhurbaşkanı Giriş Resmi Seçimi");

    Content()->addWidget(std::move(CumhurBaskaniPathContainer));


    auto BelediyeBaskaniPathContainer = createContainer("Belediye Başkanı Giriş Resmi Seçimi");

    Content()->addWidget(std::move(BelediyeBaskaniPathContainer));



    auto ustYoneticiBaskanSunumContainer = createContainer("Üst Yönetici Sunumu");
    auto ustYoneticiBaskanSunumTextEdit = ustYoneticiBaskanSunumContainer->addNew<WTextArea>();
    ustYoneticiBaskanSunumTextEdit->setHeight(300);
    Content()->addWidget(std::move(ustYoneticiBaskanSunumContainer));




    auto ustYoneticiIcKontrolGuvenceContainer = createContainer("İç Kontrol Güvence Beyanı");
    auto ustYoneticiKontrolBeyaniTextEdit = ustYoneticiIcKontrolGuvenceContainer->addNew<WTextArea>();
    ustYoneticiKontrolBeyaniTextEdit->setHeight(300);
    Content()->addWidget(std::move(ustYoneticiIcKontrolGuvenceContainer));


}

void v2::Faaliyet::FaaliyetRaporContainer::testPage( const std::string &faaliyetTitle , const std::string &faaliyetOid )
{

    Content ()->clear ();

    auto baslikContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    baslikContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_9+
                                    Bootstrap::Grid::Medium::col_md_9+
                                    Bootstrap::Grid::Small::col_sm_9+
                                    Bootstrap::Grid::ExtraSmall::col_xs_9);

    auto onayContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    onayContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_3);


    auto baslikText = baslikContainer->addWidget (cpp14::make_unique<WText>(""));
    baslikContainer->addWidget (cpp14::make_unique<WBreak>());
    baslikContainer->addWidget (cpp14::make_unique<WText>(faaliyetOid));

    baslikContainer->setMargin (25,Side::Bottom);
    baslikContainer->setAttributeValue (Style::style,Style::Border::bottom::border ("1px solid gray"));

    bool mViewMode = false;

    if( itemList.count () == 0 ){
        SerikBLDCore::Faaliyet::FaaliyetItem filter;
        filter.setOid (faaliyetOid);

        LOG << bsoncxx::to_json(filter.view()) << "\n";


        auto val = this->findOneItem (filter);

        if( val ){

            if( val.value ().view ().empty () ){

                this->showPopUpMessage ("Empty view:","err");

            }else{
                try{
                    auto arrayView = val->view ()["faaliyet"].get_array ().value;

                    for( const auto &item : arrayView ){

                        SerikBLDCore::Faaliyet::RaporItem* _item = new SerikBLDCore::Faaliyet::RaporItem();

                        _item->setDocumentView (item.get_document ().value);

                        this->itemList.push_back (_item);

                    }

                    this->showPopUpMessage ("Faaliyet Yüklendi");
                }catch(bsoncxx::exception &e){
                    this->showPopUpMessage (__FUNCTION__+ std::string(" ") +std::to_string (__LINE__) + std::string(" ")+e.what (),"err");
                }

                try{
                    std::string birim = val->view ()["birimi"].get_string().value.data();
                    baslikText->setText ("<h3>"+birim+" "+faaliyetTitle+"</h3>");
                }catch(bsoncxx::exception &e){
                    baslikText->setText ("<h3>"+faaliyetTitle+"</h3>");

                }

                try{
                    mViewMode = val->view ()["viewMode"].get_bool().value;
                }catch(bsoncxx::exception &e){

                }
            }

        }else{
            this->showPopUpMessage ("Faaliyet Yüklenemedi","err");
        }
    }


    {
        this->addKaydetButon (faaliyetOid,false);
    }

    if( itemList.count () == 0 ){

        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setPadding (10,AllSides);
        container->addWidget (cpp14::make_unique<WText>(" İçerik Başlıklarını Oluştur "))->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->setMargin (35,Side::Top|Side::Bottom);
        container->clicked ().connect ([=](){

            {
                SerikBLDCore::Faaliyet::BaslikItem *baslik = new SerikBLDCore::Faaliyet::BaslikItem();
                baslik->setText("GENEL BİLGİLER");
                this->itemList.addBaslikBack (1,"root","GENEL BİLGİLER");

                {
                    SerikBLDCore::Faaliyet::ParagrafItem *paragraf = new SerikBLDCore::Faaliyet::ParagrafItem();
                    paragraf->setText("Bu Alanı Silip, Müdürlüğünüz Hakkında Kısa Bilgi Giriniz.");
                    this->itemList.push_back(paragraf);
                }
            }

            {
                SerikBLDCore::Faaliyet::BaslikItem *baslik = new SerikBLDCore::Faaliyet::BaslikItem();
                baslik->setText("GÖREV, YETKİ VE SORUMLULUKLAR");
                this->itemList.push_back(baslik);

            }

            {
                SerikBLDCore::Faaliyet::AltBaslikItem *altBaslikGorevlik = new SerikBLDCore::Faaliyet::AltBaslikItem();
                altBaslikGorevlik->setText("Görevler");
                this->itemList.push_back(altBaslikGorevlik);
                {
                    SerikBLDCore::Faaliyet::ParagrafItem *paragraf = new SerikBLDCore::Faaliyet::ParagrafItem();
                    paragraf->setText("Bu Alanı Silip, Görevler Hakkında Kısa Bilgi Giriniz.");
                    this->itemList.push_back(paragraf);
                }

                SerikBLDCore::Faaliyet::AltBaslikItem *altBaslikSorumluluk = new SerikBLDCore::Faaliyet::AltBaslikItem();
                altBaslikSorumluluk->setText("Sorumluluklar");
                this->itemList.push_back(altBaslikSorumluluk);
                {
                    SerikBLDCore::Faaliyet::ParagrafItem *paragraf = new SerikBLDCore::Faaliyet::ParagrafItem();
                    paragraf->setText("Bu Alanı Silip, Sorumluluklar Hakkında Bilgi Giriniz.");
                    this->itemList.push_back(paragraf);
                }

                SerikBLDCore::Faaliyet::AltBaslikItem *altBaslikYetki = new SerikBLDCore::Faaliyet::AltBaslikItem();
                altBaslikYetki->setText("Yetkiler");
                this->itemList.push_back(altBaslikYetki);
                {
                    SerikBLDCore::Faaliyet::ParagrafItem *paragraf = new SerikBLDCore::Faaliyet::ParagrafItem();
                    paragraf->setText("Bu Alanı Silip, Yetkiler Hakkında Bilgi Giriniz.");
                    this->itemList.push_back(paragraf);
                }
            }


            {
                SerikBLDCore::Faaliyet::BaslikItem *baslik = new SerikBLDCore::Faaliyet::BaslikItem();
                baslik->setText("İDAREYE İLİŞKİN BİLGİLER");
                this->itemList.push_back(baslik);
            }

            {
                //                SerikBLDCore::Faaliyet::AltBaslikItem altBaslikGorevlik;
                //                altBaslikGorevlik.setText("İnsan Kaynakları");
                //                this->itemList.push_back(&altBaslikGorevlik);

                //                SerikBLDCore::Faaliyet::AltBaslikItem altBaslikSorumluluk;
                //                altBaslikSorumluluk.setText("Teşkilat Yapısı");
                //                this->itemList.push_back(&altBaslikSorumluluk);

                //                SerikBLDCore::Faaliyet::AltBaslikItem altBaslikYetki;
                //                altBaslikYetki.setText("Fiziksel Yapı");
                //                this->itemList.push_back(&altBaslikYetki);
                SerikBLDCore::Faaliyet::AltBaslikItem *altBaslikGorevlik = new SerikBLDCore::Faaliyet::AltBaslikItem();
                altBaslikGorevlik->setText("İnsan Kaynakları");
                this->itemList.push_back(altBaslikGorevlik);
                {
                    SerikBLDCore::Faaliyet::ParagrafItem *paragraf = new SerikBLDCore::Faaliyet::ParagrafItem();
                    paragraf->setText("Bu Alanı Silip, Persenol Hakkında Bilgi Giriniz.");
                    this->itemList.push_back(paragraf);
                }


                SerikBLDCore::Faaliyet::AltBaslikItem *altBaslikSorumluluk = new SerikBLDCore::Faaliyet::AltBaslikItem();
                altBaslikSorumluluk->setText("Teşkilat Yapısı");
                this->itemList.push_back(altBaslikSorumluluk);
                {
                    SerikBLDCore::Faaliyet::ParagrafItem *paragraf = new SerikBLDCore::Faaliyet::ParagrafItem();
                    paragraf->setText("Bu Alanı Silip, Müdürlüğünüz Altında ki Servisler Hakkında Bilgi Giriniz. Örneğin Zabıta Müdürlüğünün Altındaki Trafik Birimi Gibi");
                    this->itemList.push_back(paragraf);
                }

                SerikBLDCore::Faaliyet::AltBaslikItem *altBaslikYetki = new SerikBLDCore::Faaliyet::AltBaslikItem();
                altBaslikYetki->setText("Fiziksel Yapı");
                this->itemList.push_back(altBaslikYetki);
                {
                    SerikBLDCore::Faaliyet::ParagrafItem *paragraf = new SerikBLDCore::Faaliyet::ParagrafItem();
                    paragraf->setText("Bu Alanı Silip, Müdürlüğünüzün Fiziksel Yapısı Hakkında Bilgi Giriniz.");
                    this->itemList.push_back(paragraf);
                }
            }

            {
                SerikBLDCore::Faaliyet::BaslikItem *baslik = new SerikBLDCore::Faaliyet::BaslikItem();
                baslik->setText("SUNULAN HİZMETLER");
                this->itemList.push_back(baslik);
                {
                    SerikBLDCore::Faaliyet::ParagrafItem *paragraf = new SerikBLDCore::Faaliyet::ParagrafItem();
                    paragraf->setText("Bu Alanı Silip, Yapılan Çalışmalar Hakkında Bilgi Giriniz.");
                    this->itemList.push_back(paragraf);
                }
            }

            {
                SerikBLDCore::Faaliyet::BaslikItem *baslik = new SerikBLDCore::Faaliyet::BaslikItem();
                baslik->setText("MALİ BİLGİLER");
                this->itemList.push_back(baslik);
                {
                    SerikBLDCore::Faaliyet::ParagrafItem *paragraf = new SerikBLDCore::Faaliyet::ParagrafItem();
                    paragraf->setText("Bu Alanı Silip, Mali Bütçesi Hakkında Bilgi Giriniz.");
                    this->itemList.push_back(paragraf);
                }
            }

            {
                SerikBLDCore::Faaliyet::FaaliyetItem item;
                item.setOid (faaliyetOid);

                auto arrayElements = bsoncxx::builder::basic::array{};
                for( const auto &_item : itemList.list () ){
                    try{
                        arrayElements.append (_item->view ());
                    }catch(bsoncxx::exception &e){
                        LOG << e.what () << "\n";
                        return;
                    }
                }

                item.setFaaliyet (arrayElements.view ());
                auto upt = this->updateItem (item);

                if( upt ){
                    //                    if( upt.value().inserted() ){
                    this->showPopUpMessage ("Kayıt Edildi");
                    //                    }else{
                    //                        this->showPopUpMessage ("Kayıt Edilemedi","err");
                    //                    }
                }else{
                    this->showPopUpMessage ("Kayıt Edilemedi","err");
                }
            }


            testPage (faaliyetTitle,faaliyetOid);



            //            auto mDialog = createDialog ("Başlık Ekle");
            //            auto baslikLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
            //            baslikLineEdit->setPlaceholderText ("Başlık Adını Giriniz");
            //            auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Ekle+"));
            //            saveBtn->addStyleClass (Bootstrap::Button::Primary);
            //            saveBtn->clicked ().connect ([=](){
            //                this->itemList.addBaslikBack (1,"root",baslikLineEdit->text ().toUTF8 ());
            //                removeDialog (mDialog);
            //            });
        });
    }else {
    }



    //    {
    //        auto viewModeContainer = Content ()->addWidget(cpp14::make_unique<WContainerWidget>());
    //        viewModeContainer->addStyleClass (Bootstrap::Grid::col_full_12);

    //        if( mViewMode ) {
    //            viewModeContainer->addWidget(cpp14::make_unique<WText>("Düzenleme Moduna Geç"));
    //            viewModeContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_info);
    //        }else{
    //            viewModeContainer->addWidget(cpp14::make_unique<WText>("Görünteleme Moduna Geç"));
    //            viewModeContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);
    //        }

    //        viewModeContainer->setPadding(5,Side::Top|Side::Bottom);
    //        viewModeContainer->decorationStyle().setCursor(Cursor::PointingHand);
    //        viewModeContainer->clicked().connect([=](){
    //            mViewMode = !mViewMode;
    //            this->testPage (faaliyetTitle,faaliyetOid);
    //        });
    //    }


    for( int i = 0 ; i < itemList.count () ; i++ ){

        auto item = itemList.at (i);



        auto baslikItemContainer = Content ()->addWidget (cpp14::make_unique<ItemContainer<SerikBLDCore::Faaliyet::RaporItem>> (item,mUser->getDB (),mViewMode));
        baslikItemContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        baslikContainer->setPadding (15,Side::Top);
        baslikItemContainer->mAddBeforeSignal.connect ([=]( const int &type , const std::string &uuid , const std::string &baslik ){
            this->itemList.addBaslikBack (type,uuid,baslik);
            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);
        });

        baslikItemContainer->mAddAftereSignal.connect ([=]( const int &type , const std::string &uuid , const std::string &baslik ){
            this->itemList.addBaslikNext (type,uuid,baslik);
            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);

        });

        baslikItemContainer->mAddImgBeforeSignal.connect ([=](  const std::string &uuid , const std::string &url , const std::string &title ){
            this->itemList.addImgBack (uuid,url,title);
            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);

        });

        baslikItemContainer->mAddImgAftereSignal.connect ([=]( const std::string &uuid , const std::string &url , const std::string &title ){
            this->itemList.addImgNext (uuid,url,title);
            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);

        });


        baslikItemContainer->mAddTableAftereSignal.connect ([=](    const std::string &title ,
                                                            const std::vector<std::string> &headers,
                                                            const std::vector<TableCell> &cells ){

            itemList.addTableNext (baslikItemContainer->uuid(),title,headers,cells);
            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);

        });


        baslikItemContainer->mAddTableBeforeSignal.connect ([=](    const std::string &title ,
                                                            const std::vector<std::string> &headers,
                                                            const std::vector<TableCell> &cells ){

            itemList.addTableBack (baslikItemContainer->uuid(),title,headers,cells);
            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);

        });


        baslikItemContainer->mDeleteItemSignal.connect ([=]( const std::string &uuid ){
            this->itemList.deleteItem (uuid);
            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);
            //            baslikItemContainer->refreshWidget();

        });

        baslikItemContainer->mChangeText.connect ([=]( const std::string &uuid , const std::string &text){
            this->itemList.changeItem (uuid,text);
            //            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);
            baslikItemContainer->refreshWidget();
        });

        baslikItemContainer->mChangeImg.connect ([=](const std::string &uuid , const std::string &title , const std::string &imgOid){
            this->itemList.changeItem (uuid,title,imgOid,true);
            //            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);
            baslikItemContainer->refreshWidget();
        });

        baslikItemContainer->mChangeImgTitle.connect ([=](const std::string &uuid , const std::string &title , const std::string &imgOid){
            this->itemList.changeItem (uuid,title,imgOid,false);
            //            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);
            baslikItemContainer->refreshWidget();

        });

        baslikItemContainer->mChangeTable.connect ([=](const std::string &title ,
                                                   const std::vector<std::string> &headers,
                                                   const std::vector<TableCell> &cells){

            this->itemList.changeItem (baslikItemContainer->uuid(),title,headers,cells);
            //            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);
            baslikItemContainer->refreshWidget();

        });


        baslikItemContainer->mAddPageBreakBeforeSignal.connect ([=](){
            this->itemList.addPageBreakBack (baslikItemContainer->uuid());
            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);
        });

        baslikItemContainer->mAddPageBreakAftereSignal.connect ([=](){
            this->itemList.addPageBreakNext (baslikItemContainer->uuid());
            this->testPage (faaliyetTitle,faaliyetOid);
            this->addKaydetButon (faaliyetOid,true);
        });

    }
}

void v2::Faaliyet::FaaliyetRaporContainer::printDialog(const std::string &faaliyetTitle, const std::string &faaliyetOid)
{
    Content ()->clear ();


    auto container = Content()->addWidget (cpp14::make_unique<ContainerWidget>());
    container->setPositionScheme (PositionScheme::Fixed);
    container->setOverflow (Overflow::Auto);
    container->setOffsets (0,AllSides);
    container->setZIndex (99999);
    container->setWidth (WLength("100%"));
    container->setHeight (WLength("100%"));
    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::Gainsboro));


    container->clicked ().connect([=](){
        Content ()->removeWidget (container);
    });


}





void v2::Faaliyet::FaaliyetRaporContainer::errorOccured(const std::string &errorText)
{

    this->showPopUpMessage (errorText,"err");

}

void v2::Faaliyet::FaaliyetRaporContainer::addKaydetButon(const std::string &titleOid , const bool &unsaved)
{

    Footer ()->clear ();
    Footer ()->setMargin (20,Side::Top|Side::Bottom);

    auto fContainer = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    fContainer->setPositionScheme (PositionScheme::Fixed);
    fContainer->setWidth (WLength("100%"));
    fContainer->setHeight (40);
    fContainer->setContentAlignment (AlignmentFlag::Center);
    fContainer->setOffsets (0,Side::Left|Side::Right);
    fContainer->setOffsets (40,Side::Bottom);

    fContainer->setZIndex (1000);

    auto hLayout = fContainer->setLayout (cpp14::make_unique<WHBoxLayout>());
    hLayout->addStretch ();



    auto container = hLayout->addWidget (cpp14::make_unique<WContainerWidget>());
    //    container->addStyleClass (Bootstrap::Grid::col_full_12);
    container->setHeight (40);
    container->setWidth (50);

    container->setPadding (10,Side::Bottom|Side::Top);
    hLayout->addStretch ();

    //    container->setWidth (WLength("25%"));


    if( unsaved ){
        container->addStyleClass (CSSStyle::Button::redButton);
        container->addWidget (cpp14::make_unique<WText>("<b>Kaydet</b>"));
        container->clicked ().connect ([=](){

            SerikBLDCore::Faaliyet::FaaliyetItem item;
            item.setOid (titleOid);

            auto arrayElements = array{};
            for( const auto &_item : itemList.list () ){
                try{
                    arrayElements.append (_item->view ());
                }catch(bsoncxx::exception &e){
                    LOG << e.what ();
                    return;
                }
            }

            item.setFaaliyet (arrayElements.view ());
            auto upt = this->updateItem (item);

            if( upt ){
                if( upt.value ().modified_count () ){
                    this->showPopUpMessage ("Kayıt Edildi");
                    this->addKaydetButon (titleOid,false);

                    for( const auto &imgOid : this->itemList.willDeleteImgOidList ){
                        if( !this->getDB ()->deleteGridFS (imgOid.c_str ()) ){
                            this->showPopUpMessage ("Bazı Dosyalar Silinemedi","err");
                        }
                    }
                    this->itemList.willDeleteImgOidList.clear ();
                }else{
                    this->showPopUpMessage ("Kayıt Edilemedi","err");
                }
            }else{
                this->showPopUpMessage ("Kayıt Edilemedi","err");
            }
        });
    }else{
        container->addStyleClass (CSSStyle::Gradient::greenGradient);
        container->addWidget (cpp14::make_unique<WText>("Kaydedildi"));
        return;

    }




}



void v2::Faaliyet::SquencedRaporItem::addPageBreakNext(const std::string &currentUuid)
{

    SerikBLDCore::Faaliyet::PageBreak *item = new SerikBLDCore::Faaliyet::PageBreak;

    int currentBaslikindex = 0;

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( currentUuid == this->mList.at (i)->uuid () ){
            currentBaslikindex = i;
            break;
        }
    }

    this->mList.insert (currentBaslikindex+1,item);
}

void v2::Faaliyet::SquencedRaporItem::addPageBreakBack( const std::string &currentUuid)
{
    SerikBLDCore::Faaliyet::PageBreak *item = new SerikBLDCore::Faaliyet::PageBreak;

    int currentBaslikindex = 0;

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( currentUuid == this->mList.at (i)->uuid () ){
            currentBaslikindex = i;
            break;
        }
    }

    this->mList.insert (currentBaslikindex,item);
}

void v2::Faaliyet::SquencedRaporItem::addBaslikNext(const int& type , const std::string &currentUuid , const std::string &baslik)
{



    int currentBaslikindex = 0;
    int lastBaslikIndex = 0;

    std::string newBaslik = QString::fromStdString (baslik).toUpper ().toStdString ();

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( currentUuid == this->mList.at (i)->uuid () ){
            currentBaslikindex = i;
            break;
        }
    }

    int nextBaslikindex = currentBaslikindex;


    for( int i = currentBaslikindex+1 ; i < this->mList.count () ; i++ ){
        if( mList.at (i)->isBaslik () ){
            nextBaslikindex = i;
            break;
        }
    }


    for( int i = 0 ; i < this->mList.count () ; i++ ){
        if( mList.at (i)->isBaslik () ){
            lastBaslikIndex = i;
        }
    }


    SerikBLDCore::Faaliyet::RaporItem *item;

    switch (type) {
    case 1:
        item = new SerikBLDCore::Faaliyet::BaslikItem;
        item->setText (newBaslik);
        break;

    case 2:
        item = new SerikBLDCore::Faaliyet::AltBaslikItem;
        item->setText (baslik);
        break;

    case 3:
        item = new SerikBLDCore::Faaliyet::ParagrafItem;
        item->setText (baslik);
        break;

    case 4:
        item = new SerikBLDCore::Faaliyet::ImgItem;
        item->setText (baslik);
        break;

    case 5:
        item = new SerikBLDCore::Faaliyet::TableItem;
        item->setText (baslik);
        break;
    default:
        break;
    }

    this->mList.insert (currentBaslikindex+1,item);
}

void v2::Faaliyet::SquencedRaporItem::addBaslikBack(const int& type , const std::string &currentUuid, const std::string &baslik)
{

    SerikBLDCore::Faaliyet::RaporItem *item;

    std::string newBaslik = QString::fromStdString (baslik).toUpper ().toStdString ();


    switch (type) {
    case 1:
        item = new SerikBLDCore::Faaliyet::BaslikItem;
        item->setText (newBaslik);
        break;

    case 2:
        item = new SerikBLDCore::Faaliyet::AltBaslikItem;
        item->setText (baslik);
        break;

    case 3:
        item = new SerikBLDCore::Faaliyet::ParagrafItem;
        item->setText (baslik);
        break;

    case 4:
        item = new SerikBLDCore::Faaliyet::ImgItem;
        item->setText (baslik);
        break;

    case 5:
        item = new SerikBLDCore::Faaliyet::TableItem;
        item->setText (baslik);
        break;
    default:
        break;
    }



    if( this->mList.count () <= 0 ){
        this->mList.append (item);
        return;
    }





    int currentBaslikindex = 0;

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( currentUuid == this->mList.at (i)->uuid () ){
            currentBaslikindex = i;
            break;
        }
    }

    int backBaslikindex = currentBaslikindex;


    for( int i = currentBaslikindex ; i >= 0 ; i-- ){
        if( mList.at (i)->isBaslik () ){
            backBaslikindex = i;
            break;
        }
    }

    this->mList.insert (currentBaslikindex,item);

}

void v2::Faaliyet::SquencedRaporItem::addImgNext(const std::string &currentUuid, const std::string &url, const std::string &title)
{
    SerikBLDCore::Faaliyet::ImgItem *item = new SerikBLDCore::Faaliyet::ImgItem;
    item->setText (title);
    item->setImgOid (url);

    int currentBaslikindex = 0;

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( currentUuid == this->mList.at (i)->uuid () ){
            currentBaslikindex = i;
            break;
        }
    }

    this->mList.insert (currentBaslikindex+1,item);
}

void v2::Faaliyet::SquencedRaporItem::addImgBack(const std::string &currentUuid, const std::string &url, const std::string &title)
{
    SerikBLDCore::Faaliyet::ImgItem *item = new SerikBLDCore::Faaliyet::ImgItem;
    item->setText (title);
    item->setImgOid (url);

    int currentBaslikindex = 0;

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( currentUuid == this->mList.at (i)->uuid () ){
            currentBaslikindex = i;
            break;
        }
    }

    this->mList.insert (currentBaslikindex,item);
}

void v2::Faaliyet::SquencedRaporItem::addTableNext(const std::string &currentUuid, const std::string &title, const std::vector<std::string> &headers, const std::vector<v2::Faaliyet::TableCell> &cells)
{
    SerikBLDCore::Faaliyet::TableItem *item = new SerikBLDCore::Faaliyet::TableItem;
    item->setText (title);
    item->setHeaders (headers);

    for( const auto &cell : cells ){
        item->setCell (cell.row,cell.col,cell.value);
    }



    int currentBaslikindex = 0;

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( currentUuid == this->mList.at (i)->uuid () ){
            currentBaslikindex = i;
            break;
        }
    }

    this->mList.insert (currentBaslikindex+1,item);
}

void v2::Faaliyet::SquencedRaporItem::addTableBack(const std::string &currentUuid, const std::string &title, const std::vector<std::string> &headers, const std::vector<v2::Faaliyet::TableCell> &cells)
{
    SerikBLDCore::Faaliyet::TableItem *item = new SerikBLDCore::Faaliyet::TableItem;
    item->setText (title);
    item->setHeaders (headers);

    for( const auto &cell : cells ){
        item->setCell (cell.row,cell.col,cell.value);
    }



    int currentBaslikindex = 0;

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( currentUuid == this->mList.at (i)->uuid () ){
            currentBaslikindex = i;
            break;
        }
    }

    this->mList.insert (currentBaslikindex,item);
}

void v2::Faaliyet::SquencedRaporItem::deleteItem(const std::string &uuid)
{
    int currentBaslikindex = 0;

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( uuid == this->mList.at (i)->uuid () ){
            if( this->mList.at (i)->isImg () ){
                SerikBLDCore::Faaliyet::ImgItem *item = new SerikBLDCore::Faaliyet::ImgItem;
                item->setDocumentView (this->mList[i]->view ());
                willDeleteImgOidList.push_back (item->getImgOid ());
            }
            currentBaslikindex = i;
            break;
        }
    }

    this->mList.remove (currentBaslikindex);
}

void v2::Faaliyet::SquencedRaporItem::changeItem(const std::string &uuid, const std::string &text)
{
    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( uuid == this->mList.at (i)->uuid () ){
            if( this->mList[i]->isBaslik () ){
                this->mList[i]->setText (QString::fromStdString (text).toUpper ().toStdString ());
            }else if (this->mList[i]->isAltBaslik () ) {
                this->mList[i]->setText (text);
            }else{
                this->mList[i]->setText (text);
            }
            break;
        }
    }
}

void v2::Faaliyet::SquencedRaporItem::changeItem(const std::string &uuid, const std::string &title, const std::string &imgOid, const bool &deleteImgOid)
{
    SerikBLDCore::Faaliyet::ImgItem *item = new SerikBLDCore::Faaliyet::ImgItem;

    //    LOGN << "Change imgOid: "<<std::boolalpha << deleteImgOid << "\n";

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( uuid == this->mList.at (i)->uuid () ){
            item->setDocumentView (this->mList[i]->view ());

            if( deleteImgOid ){
                willDeleteImgOidList.push_back (item->getImgOid ());
            }

            item->setImgOid (imgOid);
            item->setText (title);
            this->mList.replace (i,item);

            break;
        }
    }
}

void v2::Faaliyet::SquencedRaporItem::changeItem(const std::string &currentUuid, const std::string &title, const std::vector<std::string> &headers, const std::vector<v2::Faaliyet::TableCell> &cells)
{
    SerikBLDCore::Faaliyet::TableItem *item = new SerikBLDCore::Faaliyet::TableItem;

    for( int i = 0 ; i < this->mList.count (); i++ ){
        if( currentUuid == this->mList.at (i)->uuid () ){
            item->setDocumentView (mList[i]->view ());
            item->setText (title);
            item->setHeaders (headers);
            item->resetCells ();
            for( const auto &cell : cells ){
                item->setCell (cell.row,cell.col,cell.value);
            }
            this->mList.replace (i,std::move(item));
            break;
        }
    }

}


template<typename T>
v2::Faaliyet::ItemContainer<T>::ItemContainer(const SerikBLDCore::Faaliyet::RaporItem *item, SerikBLDCore::DB* _mDB, const bool &_mViewMode)
    :mViewMode(_mViewMode),mDB(_mDB)
{
    this->setDocumentView (item->view ());
    this->refreshWidget ();
}

template<typename T>
WContainerWidget *v2::Faaliyet::ItemContainer<T>::addNewButton(const std::string &name, const bool &toTop, const int &rightoffSet, const std::string &className)
{
    auto tempContainter = addWidget (cpp14::make_unique<WContainerWidget>());
    tempContainter->setPositionScheme (PositionScheme::Absolute);
    tempContainter->setOffsets (rightoffSet,Side::Right);
    if( toTop ){
        tempContainter->setOffsets (0,Side::Top);
    }else{
        tempContainter->setOffsets (0,Side::Bottom);
    }
    tempContainter->addStyleClass (className);
    tempContainter->addWidget(cpp14::make_unique<WText>(name));
    tempContainter->decorationStyle().setCursor(Cursor::PointingHand);
    tempContainter->setPadding(10,Side::Left|Side::Right);
    tempContainter->addStyleClass("gra");

    //    return std::move(tempContainter);
    return tempContainter;

}

template<typename T>
void v2::Faaliyet::ItemContainer<T>::addBaslikPre(const bool &addPre)
{
    auto mDialog = createDialog ("Başlık Ekle");

    auto baslikLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    baslikLineEdit->setPlaceholderText ("Başlık Adını Giriniz");
    baslikLineEdit->setText ("Başlık");

    auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Ekle+"));
    saveBtn->addStyleClass (Bootstrap::Button::Primary);

    saveBtn->clicked ().connect ([=](){
        if( !addPre ){
            mAddAftereSignal(1,this->uuid(),baslikLineEdit->text().toUTF8());
        }else{
            mAddBeforeSignal(1,this->uuid(),baslikLineEdit->text().toUTF8());
        }
        removeDialog (mDialog);

    });

    mDialog->show ();
}


template<typename T>
void v2::Faaliyet::ItemContainer<T>::addAltBaslikPre(const bool &addPre)
{
    auto mDialog = createDialog ("Öncesine Alt Başlık Ekle");

    auto baslikLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    baslikLineEdit->setPlaceholderText ("Alt Başlık Adını Giriniz");
    baslikLineEdit->setText ("Alt bAşlık");

    auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Ekle+"));
    saveBtn->addStyleClass (Bootstrap::Button::Primary);

    saveBtn->clicked ().connect ([=](){
        if( !addPre ){
            mAddAftereSignal(2,this->uuid(),baslikLineEdit->text().toUTF8());
        }else{
            mAddBeforeSignal(2,this->uuid(),baslikLineEdit->text().toUTF8());
        }
        removeDialog (mDialog);

    });

    mDialog->show ();
}


template<typename T>
void v2::Faaliyet::ItemContainer<T>::addParagrafPre(const bool &addPre)
{
    auto mDialog = createDialog ("Parağraf Ekle");

    auto baslikLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WTextEdit>());
    baslikLineEdit->setExtraPlugins("lists");
    baslikLineEdit->setToolBar(0,"numlist,bullist");
    baslikLineEdit->setConfigurationSetting("language","tr");

    baslikLineEdit->setHeight (450);

    auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Ekle+"));
    saveBtn->addStyleClass (Bootstrap::Button::Primary);

    saveBtn->clicked ().connect ([=](){
        if( !addPre ){
            mAddAftereSignal(3,this->uuid(),baslikLineEdit->text().toUTF8());
        }else{
            mAddBeforeSignal(3,this->uuid(),baslikLineEdit->text().toUTF8());
        }

        removeDialog (mDialog);

    });

    mDialog->setHeight(600);
    mDialog->setWidth(1000);


    mDialog->show ();
}


template<typename T>
void v2::Faaliyet::ItemContainer<T>::addImgPre(const bool &addPre)
{
    auto mDialog = createDialog ("Sonrasına Resim Ekle");



    auto baslikLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    baslikLineEdit->setPlaceholderText ("Resim Bilgisi Ekleyiniz");

    mDialog->contents ()->addWidget(cpp14::make_unique<WBreak>());


    auto imgContainer = mDialog->contents ()->addWidget(cpp14::make_unique<WContainerWidget>());
    imgContainer->setWidth(WLength("100%"));
    imgContainer->setHeight(150);
    imgContainer->setMinimumSize(WLength::Auto,150);

    auto fileUploader = mDialog->contents ()->addWidget(cpp14::make_unique<FileUploaderWidget>("Resim Yükle"));
    fileUploader->setType(FileUploaderWidget::FilterType::Image);

    fileUploader->Uploaded().connect([=](){


        QImage qimg;
        if( qimg.load(fileUploader->fileLocation()) ){
            if( qimg.width () != 1000 ){
                qimg = qimg.scaledToWidth (1000,Qt::FastTransformation);
            }

            if( qimg.height () > 600 ){
                qimg = qimg.copy (0,qimg.height ()/2-300,1000,600);
            }

            if( qimg.save(fileUploader->fileLocation()) ){
                LOG << "file Can not Save..........\n";
            }
        }else{
            LOG << "FİLE CAN NOT OPEN\n";
        }

        imgContainer->setAttributeValue(Style::style,Style::background::url (fileUploader->doocRootLocation().toStdString())+Style::background::repeat::norepeat+
                                        Style::background::size::contain+Style::background::position::center_center);
        imgContainer->setHeight(150);
        imgContainer->setMinimumSize(WLength::Auto,150);
    });

    auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Ekle+"));
    saveBtn->addStyleClass (Bootstrap::Button::Primary);

    saveBtn->clicked ().connect ([=](){
        if( fileUploader->isUploaded() ){
            if( baslikLineEdit->text().toUTF8().size() ){
                auto val = mDB->uploadfile (fileUploader->fileLocation());
                if( !addPre ){

                    mAddImgAftereSignal(this->uuid(),val.view().get_oid ().value.to_string (),baslikLineEdit->text().toUTF8());
                }else{
                    mAddImgBeforeSignal(this->uuid(),val.view().get_oid ().value.to_string (),baslikLineEdit->text().toUTF8());
                }
                removeDialog (mDialog);
            }else{
                this->showPopUpMessage ("Hata: Resim Bilgisi Girmediniz!","err");
            }
        }else{
            this->showPopUpMessage ("Hata: Resim Yüklemediniz!","err");
        }


    });
    mDialog->show ();
}


template<typename T>
void v2::Faaliyet::ItemContainer<T>::addTablePre(const bool &addPre)
{
    auto mDialog = createDialog ("Tablo Ekle");

    auto baslikLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
    baslikLineEdit->setPlaceholderText ("Tablo Bilgisi. (Örn: Gider İstatistikleri)");

    mDialog->contents ()->addWidget(cpp14::make_unique<WBreak>());

    auto tableContainer = mDialog->contents ()->addWidget(cpp14::make_unique<WContainerWidget>());
    tableContainer->setWidth(WLength("100%"));
    tableContainer->setMinimumSize(WLength::Auto,250);

    auto table = tableContainer->addWidget(Wt::cpp14::make_unique<Wt::WTableView>());

    auto infoText = mDialog->contents()->addWidget(cpp14::make_unique<WText>(""));
    infoText->setAttributeValue(Style::style,Style::color::color (Style::color::Red::Crimson));


    table->setColumnResizeEnabled(true);
    table->setColumnAlignment(0, AlignmentFlag::Center);
    table->setHeaderAlignment(0, AlignmentFlag::Center);
    table->setAlternatingRowColors(true);
    table->setRowHeight(28);
    table->setHeaderHeight(28);
    table->setSelectionMode(SelectionMode::Single);
    table->setEditTriggers(EditTrigger::SelectedClicked);
    table->setSelectionBehavior(SelectionBehavior::Items);
    table->setSortingEnabled(false);



    table->setOverflow(Overflow::Scroll);
    table->setMaximumSize (WLength::Auto,WLength(400));


    auto model = std::make_shared<WStandardItemModel>(0,0);
    table->setModel(model);



    auto rowEkleContainer = mDialog->contents()->addWidget(cpp14::make_unique<WContainerWidget>());
    rowEkleContainer->addStyleClass(Bootstrap::Grid::row);

    auto rowEkleBtn = rowEkleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowEkleBtn->setContentAlignment(AlignmentFlag::Center);
    rowEkleBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                              Bootstrap::Grid::Medium::col_md_2+
                              Bootstrap::Grid::Small::col_sm_2+
                              Bootstrap::Grid::ExtraSmall::col_xs_2);
    rowEkleBtn->addStyleClass(CSSStyle::Button::blueButton);
    rowEkleBtn->addWidget(cpp14::make_unique<WText>("Satır Ekle+"));
    rowEkleBtn->setPadding(7,Side::Top|Side::Bottom);



    auto colBaslik = rowEkleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    colBaslik->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                             Bootstrap::Grid::Medium::col_md_3+
                             Bootstrap::Grid::Small::col_sm_3+
                             Bootstrap::Grid::ExtraSmall::col_xs_3);
    colBaslik->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_5+
                             Bootstrap::Grid::Offset::Medium::col_md_5+
                             Bootstrap::Grid::Offset::Small::col_sm_5+
                             Bootstrap::Grid::Offset::ExtraSmall::col_xs_5);

    auto sutunBaslikLineEdit = colBaslik->addWidget(cpp14::make_unique<WLineEdit>());
    sutunBaslikLineEdit->setPlaceholderText("Sütun Başlığını Girniz");


    auto colEkleBtn = rowEkleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    colEkleBtn->setContentAlignment(AlignmentFlag::Center);
    colEkleBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                              Bootstrap::Grid::Medium::col_md_2+
                              Bootstrap::Grid::Small::col_sm_2+
                              Bootstrap::Grid::ExtraSmall::col_xs_2);
    colEkleBtn->addStyleClass(CSSStyle::Button::blueButton);
    colEkleBtn->addWidget(cpp14::make_unique<WText>("Sütun Ekle+"));
    colEkleBtn->setPadding(7,Side::Top|Side::Bottom);



    rowEkleBtn->clicked().connect([=](){




        Wt::WStandardItem *root = model->invisibleRootItem();

        if( root->columnCount () <= 0 ){
            infoText->setText("<b>ilk Önce Sütun Ekleyiniz</b>");
            return;
        }

        infoText->setText("");


        std::vector<std::unique_ptr<WStandardItem>> list;

        for (int row = 0; row < root->columnCount (); ++row) {
            auto topLevel = std::make_unique<Wt::WStandardItem>();
            topLevel->setText("");
            topLevel->setEditable (true);
            list.push_back (std::move(topLevel));

        }
        root->insertRow (root->rowCount (),std::move(list));

        for( int i = 0 ; i < root->rowCount () ; i++ ){
            for( int j = 0 ; j < root->columnCount () ; j++ ){
                if( !model->index (i,j).data ().has_value () ){
                    auto item = std::make_unique<WStandardItem>("");
                    item->setEditable (true);
                    model->setItem (i,j,std::move(item));
                }
            }
        }

    });



    colEkleBtn->clicked().connect([=](){

        infoText->setText("");

        Wt::WStandardItem *root = model->invisibleRootItem();

        auto topLevel = std::make_unique<Wt::WStandardItem>();
        topLevel->setText("");
        topLevel->setEditable (true);
        std::vector<std::unique_ptr<WStandardItem>> list;
        list.push_back (std::move(topLevel));
        root->insertColumn (root->columnCount (),std::move(list));

        model->setHeaderData (root->columnCount ()-1,Orientation::Horizontal,sutunBaslikLineEdit->text().toUTF8(),ItemDataRole::Display);

        for( int i = 0 ; i < root->rowCount () ; i++ ){
            for( int j = 0 ; j < root->columnCount () ; j++ ){
                if( !model->index (i,j).data ().has_value () ){
                    auto item = std::make_unique<WStandardItem>("");
                    item->setEditable (true);
                    model->setItem (i,j,std::move(item));
                }
            }
        }


    });


    auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Ekle+"));
    saveBtn->addStyleClass (Bootstrap::Button::Primary);



    saveBtn->clicked ().connect ([=](){

        if( baslikLineEdit->text().toUTF8().size() == 0 ){
            infoText->setText("Başlık Girmediniz!");
            return;
        }

        std::vector<TableCell> cells;

        std::vector<std::string> headers;
        for( int i = 0 ; i < model->columnCount () ; i++ ){
            auto val = cpp17::any_cast<std::string>(model->headerData (i));
            headers.push_back (val);
        }


        for( int i = 0 ; i < model->rowCount (); i++ ){
            for( int j = 0 ; j < model->columnCount (); j++ ){
                TableCell cell{i,j,model->item (i,j)->text ().toUTF8 ()};
                cells.push_back (cell);
            }
        }

        if( !addPre ){
            mAddTableAftereSignal(baslikLineEdit->text().toUTF8(),headers,cells);
        }else{
            mAddTableBeforeSignal(baslikLineEdit->text().toUTF8(),headers,cells);
        }

        removeDialog (mDialog);
    });

    mDialog->setWidth(1100);
    mDialog->setMaximumSize (WLength::Auto,WLength(640));

    mDialog->show ();
}

template<typename T>
void v2::Faaliyet::ItemContainer<T>::addPageBreakPre(const bool &addPre)
{
    if( addPre ){
        mAddPageBreakBeforeSignal(NoClass());
    }else{
        mAddPageBreakAftereSignal(NoClass());
    }
}

template<typename T>
void v2::Faaliyet::ItemContainer<T>::initWidgetType()
{

    if( !mViewMode ){
        this->setAttributeValue (Style::style,Style::background::color::rgba (255,255,255,0)+Style::Border::bottom::border ("1px Dotted lightgray"));
    }
    this->setPositionScheme (PositionScheme::Relative);

    auto lineContainer = this->addWidget (cpp14::make_unique<WContainerWidget>());
    lineContainer->setPositionScheme(PositionScheme::Absolute);
    auto typeText = lineContainer->addWidget(cpp14::make_unique<WText>(""));
    typeText->setAttributeValue(Style::style,Style::font::size::s11px+Style::color::color (Style::color::White::AliceBlue));
    lineContainer->setOffsets(0,Side::Bottom|Side::Right);
    lineContainer->setPadding(2,Side::Left|Side::Right);

    if( this->isPageBreak() && !mViewMode ){
        //        this->setPadding (5,Side::Top);
        this->setContentAlignment (AlignmentFlag::Left);
        addWidget (cpp14::make_unique<WText>("Sayfa Sonu - Bu Element Çıktı Alınırken Kullanılacaktır. Lütfen Dokunmayınız."));
        setPadding (15,Side::Left);
        lineContainer->addStyleClass(CSSStyle::Button::Red::FireBrickButton);
        typeText->setText("SS");
        lineContainer->setPadding(0,Side::Top|Side::Bottom);
        this->setAttributeValue (Style::style,Style::background::color::rgb (255,150,200)+Style::color::color(Style::color::White::AliceBlue)+Style::Border::border ("1px Dotted red"));
    }

    if( this->isBaslik() ){
        this->setPadding (20,Side::Top);
        this->setContentAlignment (AlignmentFlag::Left);
        this->setText (this->getText ());
        addWidget (cpp14::make_unique<WText>("<h3>"+this->getText ()+"</h3>"));
        setPadding (15,Side::Left);
        lineContainer->addStyleClass(CSSStyle::Button::Red::CrimsonButton);
        if( !mViewMode ){
            typeText->setText("B");
        }else{
            this->removeWidget(lineContainer);
        }

    }


    if( this->isAltBaslik() ){

        //        this->setPositionScheme (PositionScheme::Relative);
        this->setContentAlignment (AlignmentFlag::Left);
        QString str = QString::fromStdString (this->getText());
        QStringList strList = str.split(" ");
        str.clear ();
        for( auto _str : strList ){
            QString lowerCase = _str.at (0);
            _str.remove(0,1);
            str += lowerCase.toUpper () + _str.toLower ()+" ";
        }
        this->setText (str.toStdString ());
        auto text = addWidget (cpp14::make_unique<WText>(this->getText ()));
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
        //        setPadding (6,Side::Top|Side::Bottom);
        setPadding (40,Side::Left);
        lineContainer->addStyleClass(CSSStyle::Button::Red::IndianRedButton);
        if( !mViewMode ){
            typeText->setText("A");
        }else{
            this->removeWidget(lineContainer);
        }
    }


    if( this->isParagraf() ){
        //        this->setPositionScheme (PositionScheme::Relative);
        this->setContentAlignment (AlignmentFlag::Justify);
        this->setText (this->getText ());
        auto text = addWidget (cpp14::make_unique<WText>(this->getText (),TextFormat::UnsafeXHTML));
        text->setTextAlignment(AlignmentFlag::Left);
        setPadding (65,Side::Left);
        lineContainer->addStyleClass(CSSStyle::Button::Red::LightCoralButton);
        if( !mViewMode ){
            typeText->setText("P");
        }else{
            this->removeWidget(lineContainer);
        }
    }

    if( this->isImg() ){

        //        this->setPositionScheme (PositionScheme::Relative);
        this->setContentAlignment (AlignmentFlag::Left);
        this->setText (this->getText ());



        SerikBLDCore::Faaliyet::ImgItem imgItem;
        imgItem.setDocumentView (this->view());

        auto imgPath = mDB->downloadFileWeb (imgItem.getImgOid ().c_str ());


        QImage img;
        if( img.load (QString("docroot/")+imgPath.c_str() ) ){
            if( img.height () > 600 || img.width () != 1000 ){

                auto warnContainer = this->addWidget (cpp14::make_unique<WContainerWidget>());
                warnContainer->setPositionScheme (PositionScheme::Absolute);
                warnContainer->setOffsets(0,Side::Top|Side::Left|Side::Right);
                warnContainer->addStyleClass(CSSStyle::Gradient::redGradient);
                warnContainer->setPadding(10,Side::Top|Side::Bottom);
                warnContainer->setContentAlignment(AlignmentFlag::Center);
                warnContainer->addWidget(cpp14::make_unique<WText>("<h4>Bu Resim Uygun Boyutlarda Değil!</h4>"));

                warnContainer->addWidget(cpp14::make_unique<WBreak>());
                warnContainer->addWidget(cpp14::make_unique<WText>(WString("<h5>Resim {1}px * {2}px {3}</h5>").arg (img.width ()).arg(img.height()).arg (imgPath.c_str ())));


                auto duzeltBtn = warnContainer->addWidget(cpp14::make_unique<WPushButton>("Düzelt"));
                duzeltBtn->addStyleClass(Bootstrap::Button::Danger);
                duzeltBtn->clicked().connect([=](){

                    QImage qimg;
                    if( qimg.load (QString("docroot/")+imgPath.c_str() ) ){
                        qimg = qimg.scaledToWidth (1000,Qt::FastTransformation);

                        if( qimg.height () < 600 ){
                            qimg = qimg.scaledToHeight (600,Qt::FastTransformation);

                            if( qimg.width () > 1000 ){
                                qimg = qimg.copy (qimg.width ()/2-500,0,1000,qimg.height ());
                            }
                        }

                        if( qimg.height () > 600 ){
                            qimg = qimg.copy (0,qimg.height ()/2-300,1000,600);
                        }





                        if( qimg.save(QString("docroot/")+imgPath.c_str()) ){
                            LOG << "file Can not Save..........\n";
                        }


                        auto val = mDB->uploadfile (QString("docroot/")+imgPath.c_str());

                        SerikBLDCore::Faaliyet::ImgItem _imgItem;
                        _imgItem.setDocumentView (this->view());
                        _imgItem.setImgOid (val.view().get_oid ().value.to_string ());
                        this->setDocumentView(_imgItem.view ());

                        mChangeImg(this->uuid(),_imgItem.getText (),val.view().get_oid ().value.to_string ());
                    }





                });
            }

        }




        auto imgContainer= addWidget (cpp14::make_unique<WContainerWidget>());
        imgContainer->setAttributeValue (Style::style,Style::background::url (imgPath)+Style::background::repeat::norepeat+
                                         Style::background::size::contain+
                                         Style::background::position::center_center);
        imgContainer->setHeight (250);
        imgContainer->setWidth (WLength("100%"));

        auto textContainer = addWidget (cpp14::make_unique<WContainerWidget>());
        textContainer->setContentAlignment(AlignmentFlag::Center);
        textContainer->addWidget (cpp14::make_unique<WText>(this->getText()));


        setPadding (65,Side::Left);
        lineContainer->addStyleClass(CSSStyle::Button::Red::LightSalmonButton);
        if( !mViewMode ){
            typeText->setText("R");
        }else{
            this->removeWidget(lineContainer);
        }

    }

    if( this->isTable() ){

        //        this->setPositionScheme (PositionScheme::Relative);
        this->setContentAlignment (AlignmentFlag::Center);
        this->setText (this->getText ());
        //        this->setMargin (25,Side::Top);
        this->setPadding (65,Side::Left);

        addWidget (cpp14::make_unique<WText>("<b>"+this->getText()+"</b>"));
        addWidget (cpp14::make_unique<WBreak>());

        //CUSTOM TABLE
        {
            SerikBLDCore::Faaliyet::TableItem tableItem;
            tableItem.setDocumentView (this->view());

            auto divContainer = addWidget (cpp14::make_unique<WContainerWidget>());
            divContainer->setWidth(WLength("100%"));

            auto tableContainer = divContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            tableContainer->setHtmlTagName ("table");
            tableContainer->addStyleClass ("blueTable");
            {
                auto trContainer = tableContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                trContainer->setHtmlTagName ("thead");
                auto thContainer = trContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                thContainer->setHtmlTagName ("tr");


                for( std::size_t i = 0 ; i < tableItem.headers ().size () ; i++ ){
                    auto text = thContainer->addWidget (cpp14::make_unique<WText>(tableItem.headers ().at (i)));
                    text->setHtmlTagName ("th");

                }
            }

            for( auto i = 0 ; i < tableItem.row () ; i++ ){
                auto trContainer = tableContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                trContainer->setHtmlTagName ("tr");
                for( auto j = 0 ; j < tableItem.column () ; j++ ){
                    auto thContainer = trContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                    thContainer->setHtmlTagName ("td");
                    thContainer->addWidget (cpp14::make_unique<WText>(tableItem.cell (i,j)));
                }
            }
        }

        lineContainer->addStyleClass(CSSStyle::Button::Red::SalmonButton);
        if( !mViewMode ){
            typeText->setText("T");
        }else{
            this->removeWidget(lineContainer);
        }

    }


    //    if( mViewMode ){
    //        this->setMargin (0,Side::Bottom);
    //        this->setPadding (10,Side::Top);

    //    }else{
    //        this->setPadding (25,Side::Top);
    //        this->setMargin (10,Side::Bottom);
    //    }

    //    this->decorationStyle ().setBorder (WBorder(BorderStyle::Solid,BorderWidth::Thin,WColor(225,225,225)),Side::Top);

}

template<typename T>
void v2::Faaliyet::ItemContainer<T>::editWidgetType()
{

    if( this->isBaslik() ){
        this->::ContainerWidget::clear ();
        this->setAttributeValue (Style::style,Style::background::color::rgb (225,225,225));

        this->setMargin (35,Side::Top);
        this->setPositionScheme (PositionScheme::Relative);
        this->setContentAlignment (AlignmentFlag::Left);
        this->setText (this->getText ());
        auto text = addWidget (cpp14::make_unique<WLineEdit>(this->getText ()));
        text->setWidth(WLength("100%"));
        //        text->setAttributeValue (Style::style,Style::font::size::s18px+Style::font::family::dosis+Style::font::weight::bold);


        auto cBtnContainer = addWidget (cpp14::make_unique<WContainerWidget>());
        cBtnContainer->addStyleClass(Bootstrap::Grid::row);

        auto saveBtnContainer = cBtnContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        saveBtnContainer->addStyleClass (CSSStyle::Button::greenButton+
                                         Bootstrap::Grid::Large::col_lg_2+
                                         Bootstrap::Grid::Medium::col_md_2+
                                         Bootstrap::Grid::Small::col_sm_2+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3+
                                         Bootstrap::Grid::Offset::Large::col_lg_2+
                                         Bootstrap::Grid::Offset::Medium::col_md_2+
                                         Bootstrap::Grid::Offset::Small::col_sm_2+
                                         Bootstrap::Grid::Offset::ExtraSmall::col_xs_3);
        saveBtnContainer->setContentAlignment (AlignmentFlag::Center);
        saveBtnContainer->setPadding(5,Side::Top|Side::Bottom);

        saveBtnContainer->addWidget (cpp14::make_unique<WText>("Kaydet"));
        saveBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        saveBtnContainer->clicked().connect([=](){
            this->setText(text->text().toUTF8());
            mChangeText(this->uuid(),text->text().toUTF8());

        });


        auto cancelBtnContainer = cBtnContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        cancelBtnContainer->addStyleClass (CSSStyle::Button::Red::IndianRedButton+
                                           Bootstrap::Grid::Large::col_lg_2+
                                           Bootstrap::Grid::Medium::col_md_2+
                                           Bootstrap::Grid::Small::col_sm_2+
                                           Bootstrap::Grid::ExtraSmall::col_xs_3+
                                           Bootstrap::Grid::Offset::Large::col_lg_4+
                                           Bootstrap::Grid::Offset::Medium::col_md_4+
                                           Bootstrap::Grid::Offset::Small::col_sm_4+
                                           Bootstrap::Grid::Offset::ExtraSmall::col_xs_3);
        cancelBtnContainer->setContentAlignment (AlignmentFlag::Center);
        cancelBtnContainer->addWidget (cpp14::make_unique<WText>("İptal"));
        cancelBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        cancelBtnContainer->setPadding(5,Side::Top|Side::Bottom);

        cancelBtnContainer->clicked().connect([=](){
            this->refreshWidget ();
        });


        setPadding (15,Side::Top|Side::Bottom);
        setPadding (15,Side::Left);
    }


    if( this->isAltBaslik() ){
        this->::ContainerWidget::clear ();
        this->setAttributeValue (Style::style,Style::background::color::rgb (225,225,225));

        this->setPositionScheme (PositionScheme::Relative);
        this->setContentAlignment (AlignmentFlag::Left);
        this->setText (this->getText ());
        auto text = addWidget (cpp14::make_unique<WLineEdit>(this->getText ()));
        text->setWidth(WLength("100%"));
        //        text->setAttributeValue (Style::style,Style::font::size::s18px+Style::font::family::dosis+Style::font::weight::bold);


        auto cBtnContainer = addWidget (cpp14::make_unique<WContainerWidget>());
        cBtnContainer->addStyleClass(Bootstrap::Grid::row);

        auto saveBtnContainer = cBtnContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        saveBtnContainer->addStyleClass (CSSStyle::Button::greenButton+
                                         Bootstrap::Grid::Large::col_lg_2+
                                         Bootstrap::Grid::Medium::col_md_2+
                                         Bootstrap::Grid::Small::col_sm_2+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3+
                                         Bootstrap::Grid::Offset::Large::col_lg_2+
                                         Bootstrap::Grid::Offset::Medium::col_md_2+
                                         Bootstrap::Grid::Offset::Small::col_sm_2+
                                         Bootstrap::Grid::Offset::ExtraSmall::col_xs_3);
        saveBtnContainer->setContentAlignment (AlignmentFlag::Center);
        saveBtnContainer->setPadding(5,Side::Top|Side::Bottom);

        saveBtnContainer->addWidget (cpp14::make_unique<WText>("Kaydet"));
        saveBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        saveBtnContainer->clicked().connect([=](){
            this->setText(text->text().toUTF8());
            mChangeText(this->uuid(),text->text().toUTF8());

        });


        auto cancelBtnContainer = cBtnContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        cancelBtnContainer->addStyleClass (CSSStyle::Button::Red::IndianRedButton+
                                           Bootstrap::Grid::Large::col_lg_2+
                                           Bootstrap::Grid::Medium::col_md_2+
                                           Bootstrap::Grid::Small::col_sm_2+
                                           Bootstrap::Grid::ExtraSmall::col_xs_3+
                                           Bootstrap::Grid::Offset::Large::col_lg_4+
                                           Bootstrap::Grid::Offset::Medium::col_md_4+
                                           Bootstrap::Grid::Offset::Small::col_sm_4+
                                           Bootstrap::Grid::Offset::ExtraSmall::col_xs_3);
        cancelBtnContainer->setContentAlignment (AlignmentFlag::Center);
        cancelBtnContainer->setPadding(5,Side::Top|Side::Bottom);

        cancelBtnContainer->addWidget (cpp14::make_unique<WText>("İptal"));
        cancelBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);

        cancelBtnContainer->clicked().connect([=](){
            this->refreshWidget ();
        });


        setPadding (15,Side::Left);
    }


    if( this->isParagraf() ){
        this->::ContainerWidget::clear ();
        this->setAttributeValue (Style::style,Style::background::color::rgb (225,225,225));

        this->setMargin (35,Side::Top);
        //        this->setPositionScheme (PositionScheme::Relative);
        //        this->setContentAlignment (AlignmentFlag::Center);
        //        this->setText (this->getText ());

        this->removeStyleClass (Bootstrap::Grid::container_fluid);




        auto text = addWidget (cpp14::make_unique<WTextEdit>(this->getText ()));
        text->addStyleClass (Bootstrap::Grid::container_fluid);
        text->setExtraPlugins("lists");
        text->setToolBar(0,"numlist,bullist,indent");
        text->setConfigurationSetting("language","tr");


        //        text->setWidth(WLength("100%"));
        text->setMargin(30,Side::Right);
        text->setHeight(300);

        auto cBtnContainer = addWidget (cpp14::make_unique<WContainerWidget>());
        cBtnContainer->addStyleClass(Bootstrap::Grid::row);


        auto saveBtnContainer = cBtnContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        saveBtnContainer->addStyleClass (CSSStyle::Button::greenButton+
                                         Bootstrap::Grid::Large::col_lg_2+
                                         Bootstrap::Grid::Medium::col_md_2+
                                         Bootstrap::Grid::Small::col_sm_2+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3+
                                         Bootstrap::Grid::Offset::Large::col_lg_2+
                                         Bootstrap::Grid::Offset::Medium::col_md_2+
                                         Bootstrap::Grid::Offset::Small::col_sm_2+
                                         Bootstrap::Grid::Offset::ExtraSmall::col_xs_3);
        saveBtnContainer->setContentAlignment (AlignmentFlag::Center);
        saveBtnContainer->addWidget (cpp14::make_unique<WText>("Kaydet"));
        saveBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        saveBtnContainer->setPadding(5,Side::Top|Side::Bottom);

        saveBtnContainer->clicked().connect([=](){
            this->setText(text->text().toUTF8());
            mChangeText(this->uuid(),text->text().toUTF8());
        });


        auto cancelBtnContainer = cBtnContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        cancelBtnContainer->addStyleClass (CSSStyle::Button::Red::IndianRedButton+
                                           Bootstrap::Grid::Large::col_lg_2+
                                           Bootstrap::Grid::Medium::col_md_2+
                                           Bootstrap::Grid::Small::col_sm_2+
                                           Bootstrap::Grid::ExtraSmall::col_xs_3+
                                           Bootstrap::Grid::Offset::Large::col_lg_4+
                                           Bootstrap::Grid::Offset::Medium::col_md_4+
                                           Bootstrap::Grid::Offset::Small::col_sm_4+
                                           Bootstrap::Grid::Offset::ExtraSmall::col_xs_3);
        cancelBtnContainer->setContentAlignment (AlignmentFlag::Center);
        cancelBtnContainer->setPadding(5,Side::Top|Side::Bottom);

        cancelBtnContainer->addWidget (cpp14::make_unique<WText>("İptal"));
        cancelBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);

        cancelBtnContainer->clicked().connect([=](){
            this->refreshWidget ();
        });


        setPadding (15,Side::Top|Side::Bottom);
        setPadding (15,Side::Left);
    }


    if( this->isImg() ){

        this->::ContainerWidget::clear ();
        this->setAttributeValue (Style::style,Style::background::color::rgb (225,225,225));

        this->setPositionScheme (PositionScheme::Relative);
        this->setContentAlignment (AlignmentFlag::Left);
        this->setText (this->getText ());


        SerikBLDCore::Faaliyet::ImgItem imgItem;
        imgItem.setDocumentView (this->view());

        auto imgPath = mDB->downloadFileWeb (imgItem.getImgOid ().c_str ());

        auto imgContainer= addWidget (cpp14::make_unique<WContainerWidget>());
        imgContainer->setAttributeValue (Style::style,Style::background::url (imgPath)+Style::background::repeat::norepeat+
                                         Style::background::size::contain+
                                         Style::background::position::center_center);
        imgContainer->setHeight (250);
        imgContainer->setWidth (WLength("100%"));



        auto textContainer = addWidget (cpp14::make_unique<WContainerWidget>());
        textContainer->setContentAlignment(AlignmentFlag::Center);
        auto baslikLineEdit = textContainer->addWidget (cpp14::make_unique<WLineEdit>(this->getText()));



        addWidget (cpp14::make_unique<WBreak>());
        auto fileContainer= addWidget (cpp14::make_unique<FileUploaderWidget>("Resim Yükle"));
        fileContainer->setType(FileUploaderWidget::FilterType::Image);
        fileContainer->Uploaded().connect([=](){

            QImage qimg;
            if( qimg.load(fileContainer->fileLocation()) ){
                if( qimg.width () != 1000 ){
                    qimg = qimg.scaledToWidth (1000,Qt::FastTransformation);
                }

                if( qimg.height () > 600 ){
                    qimg = qimg.copy (0,qimg.height ()/2-300,1000,600);
                }

                if( qimg.save(fileContainer->fileLocation()) ){
                    LOG << "file Can not Save..........\n";
                }
            }else{
                LOG << "FİLE CAN NOT OPEN\n";
            }

            imgContainer->setAttributeValue (Style::style,Style::background::url (fileContainer->doocRootLocation().toStdString())+Style::background::repeat::norepeat+
                                             Style::background::size::contain+
                                             Style::background::position::center_center);
            imgContainer->setHeight (250);
            imgContainer->setMinimumSize(WLength::Auto,WLength(250));
            imgContainer->setWidth (WLength("100%"));

        });



        auto cBtnContainer = addWidget (cpp14::make_unique<WContainerWidget>());
        cBtnContainer->addStyleClass(Bootstrap::Grid::row);

        auto saveBtnContainer = cBtnContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        saveBtnContainer->addStyleClass (CSSStyle::Button::greenButton+
                                         Bootstrap::Grid::Large::col_lg_2+
                                         Bootstrap::Grid::Medium::col_md_2+
                                         Bootstrap::Grid::Small::col_sm_2+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3+
                                         Bootstrap::Grid::Offset::Large::col_lg_2+
                                         Bootstrap::Grid::Offset::Medium::col_md_2+
                                         Bootstrap::Grid::Offset::Small::col_sm_2+
                                         Bootstrap::Grid::Offset::ExtraSmall::col_xs_3);
        saveBtnContainer->setContentAlignment (AlignmentFlag::Center);
        saveBtnContainer->setPadding(5,Side::Top|Side::Bottom);
        saveBtnContainer->addWidget (cpp14::make_unique<WText>("Kaydet"));
        saveBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        saveBtnContainer->clicked().connect([=](){
            if( fileContainer->isUploaded() ){
                auto val = mDB->uploadfile (fileContainer->fileLocation());

                SerikBLDCore::Faaliyet::ImgItem _imgItem;
                _imgItem.setDocumentView (this->view());
                _imgItem.setImgOid (val.view().get_oid ().value.to_string ());
                _imgItem.setText (baslikLineEdit->text().toUTF8());
                this->setDocumentView(_imgItem.view ());

                mChangeImg(this->uuid(),baslikLineEdit->text().toUTF8(),val.view().get_oid ().value.to_string ());

            }else{
                SerikBLDCore::Faaliyet::ImgItem _imgItem;
                _imgItem.setDocumentView (this->view());
                _imgItem.setText (baslikLineEdit->text().toUTF8());
                this->setDocumentView(_imgItem.view ());
                mChangeImgTitle(this->uuid(),baslikLineEdit->text().toUTF8(),imgItem.getImgOid ());
            }
        });


        auto cancelBtnContainer = cBtnContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        cancelBtnContainer->addStyleClass (CSSStyle::Button::Red::IndianRedButton+
                                           Bootstrap::Grid::Large::col_lg_2+
                                           Bootstrap::Grid::Medium::col_md_2+
                                           Bootstrap::Grid::Small::col_sm_2+
                                           Bootstrap::Grid::ExtraSmall::col_xs_3+
                                           Bootstrap::Grid::Offset::Large::col_lg_4+
                                           Bootstrap::Grid::Offset::Medium::col_md_4+
                                           Bootstrap::Grid::Offset::Small::col_sm_4+
                                           Bootstrap::Grid::Offset::ExtraSmall::col_xs_3);
        cancelBtnContainer->setContentAlignment (AlignmentFlag::Center);
        cancelBtnContainer->setPadding(5,Side::Top|Side::Bottom);
        cancelBtnContainer->addWidget (cpp14::make_unique<WText>("İptal"));
        cancelBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);

        cancelBtnContainer->clicked().connect([=](){
            this->refreshWidget ();
        });

        setPadding (5,Side::Top|Side::Bottom);
        setPadding (65,Side::Left);
    }



    if( this->isTable() ){

        ::ContainerWidget::clear ();
        this->setAttributeValue (Style::style,Style::background::color::rgb (225,225,225));
        this->setPositionScheme (PositionScheme::Relative);
        this->setContentAlignment (AlignmentFlag::Center);
        this->setText (this->getText ());
        this->setMargin (25,Side::Top);
        this->setPadding (65,Side::Left);

        auto baslikLineEdit = addWidget (cpp14::make_unique<WLineEdit>(this->getText()));
        addWidget (cpp14::make_unique<WBreak>());





        auto tableView = addWidget (cpp14::make_unique<WTableView>());
        tableView->setColumnResizeEnabled(true);
        tableView->setColumnAlignment(0, AlignmentFlag::Center);
        tableView->setHeaderAlignment(0, AlignmentFlag::Center);
        tableView->setAlternatingRowColors(true);
        tableView->setSelectionMode(SelectionMode::Single);
        tableView->setEditTriggers(EditTrigger::SelectedClicked);
        tableView->setSelectionBehavior(SelectionBehavior::Items);
        tableView->setSortingEnabled(false);

        SerikBLDCore::Faaliyet::TableItem tableItem;
        tableItem.setDocumentView (this->view());

        auto model = std::make_shared<WStandardItemModel>(tableItem.row (),tableItem.column ());

        tableView->setModel(model);

        for( std::size_t i = 0; i < tableItem.headers ().size (); i++ ){
            model->setHeaderData (i,Orientation::Horizontal,tableItem.headers ()[i],ItemDataRole::Display);
        }

        for( int i = 0 ; i < tableItem.row (); i++ ){
            for( int j = 0 ; j < tableItem.column (); j++ ){
                auto item = std::make_unique<WStandardItem>(tableItem.cell (i,j));
                item->setEditable (true);
                model->setItem (i,j,std::move(item));
            }
        }



        auto cBtnContainer = addWidget (cpp14::make_unique<WContainerWidget>());
        cBtnContainer->addStyleClass(Bootstrap::Grid::row);

        auto rowEkleOBtn = cBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rowEkleOBtn->setContentAlignment(AlignmentFlag::Center);
        rowEkleOBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                   Bootstrap::Grid::Medium::col_md_1+
                                   Bootstrap::Grid::Small::col_sm_1+
                                   Bootstrap::Grid::ExtraSmall::col_xs_1);
        rowEkleOBtn->addStyleClass(CSSStyle::Button::Green::GreenYellow);
        rowEkleOBtn->addWidget(cpp14::make_unique<WText>("Satır Ekle<"));
        rowEkleOBtn->setPadding(7,Side::Top|Side::Bottom);
        rowEkleOBtn->clicked().connect([=](){
            Wt::WStandardItem *root = model->invisibleRootItem();

            if( root->columnCount () <= 0 ){
                warnDialog ("Hiç Sütun Yok. Sütun Ekleyiniz");
                return;
            }

            int row = -1;
            for( WModelIndex item : tableView->selectedIndexes () ){
                try {
                    row = item.row ();
                }  catch (const std::exception& e) {
                    LOG << "error: " << e.what () << "\n";
                    warnDialog (std::string("Satır Bulunamadı: ")+e.what ());
                }

                if( row != -1 ){
                    std::vector<std::unique_ptr<WStandardItem>> list;

                    for (int col = 0; col < root->columnCount (); ++col) {
                        auto topLevel = std::make_unique<Wt::WStandardItem>();
                        topLevel->setText("");
                        topLevel->setEditable (true);
                        list.push_back (std::move(topLevel));
                    }
                    root->insertRow (row,std::move(list));

                    this->showPopUpMessage ("Satır Eklendi");
                }else{
                    warnDialog ("Satır Seçmediniz");
                }
            }
        });



        auto rowEkleBtn = cBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rowEkleBtn->setContentAlignment(AlignmentFlag::Center);
        rowEkleBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                  Bootstrap::Grid::Medium::col_md_1+
                                  Bootstrap::Grid::Small::col_sm_1+
                                  Bootstrap::Grid::ExtraSmall::col_xs_1);
        rowEkleBtn->addStyleClass(CSSStyle::Button::blueButton);
        rowEkleBtn->addWidget(cpp14::make_unique<WText>("Satır Ekle+"));
        rowEkleBtn->setPadding(7,Side::Top|Side::Bottom);
        rowEkleBtn->clicked().connect([=](){
            Wt::WStandardItem *root = model->invisibleRootItem();
            if( root->columnCount () <= 0 ){
                return;
            }

            std::vector<std::unique_ptr<WStandardItem>> list;

            for (int row = 0; row < root->columnCount (); ++row) {
                auto topLevel = std::make_unique<Wt::WStandardItem>();
                topLevel->setText("");
                topLevel->setEditable (true);
                list.push_back (std::move(topLevel));
            }
            root->insertRow (root->rowCount (),std::move(list));

            for( int i = 0 ; i < root->rowCount () ; i++ ){
                for( int j = 0 ; j < root->columnCount () ; j++ ){
                    if( !model->index (i,j).data ().has_value () ){
                        auto item = std::make_unique<WStandardItem>("");
                        item->setEditable (true);
                        model->setItem (i,j,std::move(item));
                    }
                }
            }
        });



        auto rowEkleSBtn = cBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rowEkleSBtn->setContentAlignment(AlignmentFlag::Center);
        rowEkleSBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                   Bootstrap::Grid::Medium::col_md_1+
                                   Bootstrap::Grid::Small::col_sm_1+
                                   Bootstrap::Grid::ExtraSmall::col_xs_1);
        rowEkleSBtn->addStyleClass(CSSStyle::Button::Green::GreenYellow);
        rowEkleSBtn->addWidget(cpp14::make_unique<WText>("Satır Ekle>"));
        rowEkleSBtn->setPadding(7,Side::Top|Side::Bottom);
        rowEkleSBtn->clicked().connect([=](){
            Wt::WStandardItem *root = model->invisibleRootItem();

            if( root->columnCount () <= 0 ){
                warnDialog ("Hiç Sütun Yok. Sütun Ekleyiniz");
                return;
            }

            int row = -1;
            for( WModelIndex item : tableView->selectedIndexes () ){
                try {
                    row = item.row ();
                }  catch (const std::exception& e) {
                    warnDialog (std::string("Satır Bulunamadı: ")+e.what ());
                }

                if( row != -1 ){
                    std::vector<std::unique_ptr<WStandardItem>> list;

                    for (int col = 0; col < root->columnCount (); ++col) {
                        auto topLevel = std::make_unique<Wt::WStandardItem>();
                        topLevel->setText("");
                        topLevel->setEditable (true);
                        list.push_back (std::move(topLevel));
                    }
                    root->insertRow (row+1,std::move(list));

                    this->showPopUpMessage ("Satır Eklendi");
                }else{
                    warnDialog ("Satır Seçmediniz");
                }
            }
        });



        auto rowCikarBtn = cBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        rowCikarBtn->setContentAlignment(AlignmentFlag::Center);
        rowCikarBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                   Bootstrap::Grid::Medium::col_md_1+
                                   Bootstrap::Grid::Small::col_sm_1+
                                   Bootstrap::Grid::ExtraSmall::col_xs_1);
        rowCikarBtn->addStyleClass(CSSStyle::Button::redButton);
        rowCikarBtn->addWidget(cpp14::make_unique<WText>("Satır Sil-"));
        rowCikarBtn->setPadding(7,Side::Top|Side::Bottom);
        rowCikarBtn->clicked().connect([=](){

            int row = -1;
            for( WModelIndex item : tableView->selectedIndexes () ){

                try {
                    //                        auto anyData = cpp17::any_cast<WString>(item.data ());
                    row = item.row ();
                }  catch (const std::exception& e) {
                    LOG << "error: " << e.what () << "\n";
                    warnDialog (std::string("Satır Bulunamadı: ")+e.what ());
                }

                if( row != -1 ){
                    auto btn = askConfirm (WString("{1} Satırı Silmek İstediğinize Eminisiniz?").arg(row).toUTF8());
                    btn->clicked().connect([=](){
                        model->removeRow (row);
                    });
                }else{
                    warnDialog ("Satır Seçmediniz");
                }
            }
        });





        auto colEkleOBtn = cBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        colEkleOBtn->setContentAlignment(AlignmentFlag::Center);
        colEkleOBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                   Bootstrap::Grid::Medium::col_md_1+
                                   Bootstrap::Grid::Small::col_sm_1+
                                   Bootstrap::Grid::ExtraSmall::col_xs_1);

        colEkleOBtn->addStyleClass(CSSStyle::Button::blueButton);
        colEkleOBtn->addWidget(cpp14::make_unique<WText>("Sütun Ekle<"));
        colEkleOBtn->setPadding(7,Side::Top|Side::Bottom);





        auto colBaslik = cBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        colBaslik->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                                 Bootstrap::Grid::Medium::col_md_3+
                                 Bootstrap::Grid::Small::col_sm_3+
                                 Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto sutunBaslikLineEdit = colBaslik->addWidget(cpp14::make_unique<WLineEdit>());
        sutunBaslikLineEdit->setPlaceholderText("Sütun Başlığını Girniz");




        colEkleOBtn->clicked().connect([=](){


            Wt::WStandardItem *root = model->invisibleRootItem();

            int col = -1;
            for( WModelIndex item : tableView->selectedIndexes () ){
                try {
                    auto anyData = cpp17::any_cast<WString>(item.data ());
                    col = item.column ();
                }  catch (const std::exception& e) {
                    warnDialog (std::string("Sütun Bulunamadı: ")+e.what ());
                }
            }
            if( col >= 0 ){
                auto topLevel = std::make_unique<Wt::WStandardItem>();
                topLevel->setText("");
                topLevel->setEditable (true);
                std::vector<std::unique_ptr<WStandardItem>> list;
                list.push_back (std::move(topLevel));
                root->insertColumn (col,std::move(list));

                model->setHeaderData (col,Orientation::Horizontal,sutunBaslikLineEdit->text().toUTF8(),ItemDataRole::Display);

                for( int i = 0 ; i < root->rowCount () ; i++ ){
                    for( int j = 0 ; j < root->columnCount () ; j++ ){
                        if( !model->index (i,j).data ().has_value () ){
                            auto item = std::make_unique<WStandardItem>("");
                            item->setEditable (true);
                            model->setItem (i,j,std::move(item));
                        }
                    }
                }
            } else{
                warnDialog ("Sütun Seçmediniz");
            }

        });


        auto colEkleSBtn = cBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        colEkleSBtn->setContentAlignment(AlignmentFlag::Center);
        colEkleSBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                   Bootstrap::Grid::Medium::col_md_1+
                                   Bootstrap::Grid::Small::col_sm_1+
                                   Bootstrap::Grid::ExtraSmall::col_xs_1);
        colEkleSBtn->addStyleClass(CSSStyle::Button::blueButton);
        colEkleSBtn->addWidget(cpp14::make_unique<WText>("Sütun Ekle<b>></b>"));
        colEkleSBtn->setPadding(7,Side::Top|Side::Bottom);

        colEkleSBtn->clicked().connect([=](){


            Wt::WStandardItem *root = model->invisibleRootItem();

            int col = -1;
            for( WModelIndex item : tableView->selectedIndexes () ){
                try {
                    //                        auto anyData = cpp17::any_cast<WString>(item.data ());
                    col = item.column ();
                }  catch (const std::exception& e) {
                    warnDialog (std::string("Sütun Bulunamadı: ")+e.what ());
                }
            }
            if( col >= 0 ){
                auto topLevel = std::make_unique<Wt::WStandardItem>();
                topLevel->setText("");
                topLevel->setEditable (true);
                std::vector<std::unique_ptr<WStandardItem>> list;
                list.push_back (std::move(topLevel));
                root->insertColumn (col+1,std::move(list));

                model->setHeaderData (col+1,Orientation::Horizontal,sutunBaslikLineEdit->text().toUTF8(),ItemDataRole::Display);

                for( int i = 0 ; i < root->rowCount () ; i++ ){
                    for( int j = 0 ; j < root->columnCount () ; j++ ){
                        if( !model->index (i,j).data ().has_value () ){
                            auto item = std::make_unique<WStandardItem>("");
                            item->setEditable (true);
                            model->setItem (i,j,std::move(item));
                        }
                    }
                }
            } else{
                warnDialog ("Sütun Seçmediniz");
            }


        });


        auto colEkleBtn = cBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        colEkleBtn->setContentAlignment(AlignmentFlag::Center);
        colEkleBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                  Bootstrap::Grid::Medium::col_md_1+
                                  Bootstrap::Grid::Small::col_sm_1+
                                  Bootstrap::Grid::ExtraSmall::col_xs_1);
        colEkleBtn->addStyleClass(CSSStyle::Button::blueButton);
        colEkleBtn->addWidget(cpp14::make_unique<WText>("Sütun Ekle+"));
        colEkleBtn->setPadding(7,Side::Top|Side::Bottom);

        colEkleBtn->clicked().connect([=](){


            Wt::WStandardItem *root = model->invisibleRootItem();

            auto topLevel = std::make_unique<Wt::WStandardItem>();
            topLevel->setText("");
            topLevel->setEditable (true);
            std::vector<std::unique_ptr<WStandardItem>> list;
            list.push_back (std::move(topLevel));
            root->insertColumn (root->columnCount (),std::move(list));

            model->setHeaderData (root->columnCount ()-1,Orientation::Horizontal,sutunBaslikLineEdit->text().toUTF8(),ItemDataRole::Display);

            for( int i = 0 ; i < root->rowCount () ; i++ ){
                for( int j = 0 ; j < root->columnCount () ; j++ ){
                    if( !model->index (i,j).data ().has_value () ){
                        auto item = std::make_unique<WStandardItem>("");
                        item->setEditable (true);
                        model->setItem (i,j,std::move(item));
                    }
                }
            }


        });



        auto colreNameBtn = cBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        colreNameBtn->setContentAlignment(AlignmentFlag::Center);
        colreNameBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                    Bootstrap::Grid::Medium::col_md_1+
                                    Bootstrap::Grid::Small::col_sm_1+
                                    Bootstrap::Grid::ExtraSmall::col_xs_1);
        colreNameBtn->addStyleClass(CSSStyle::Button::Red::IndianRedButton);
        colreNameBtn->addWidget(cpp14::make_unique<WText>("S.A.Dğş."));
        colreNameBtn->setPadding(7,Side::Top|Side::Bottom);
        colreNameBtn->clicked().connect([=](){
            //            Wt::WStandardItem *root = model->invisibleRootItem();
            int col = -1;
            for( WModelIndex item : tableView->selectedIndexes () ){
                try {
                    col = item.column ();
                }  catch (const std::exception& e) {
                    warnDialog (std::string("Sütun Bulunamadı: ")+e.what ());
                }
            }
            if( col >= 0 ){
                auto mDialog = createDialog ("Sütunun Adını Değiştir");

                auto newName = mDialog->contents()->addWidget(cpp14::make_unique<WLineEdit>());
                newName->setText(tableItem.headers ()[col]);

                auto kaydetBtn = mDialog->footer()->addWidget(cpp14::make_unique<WPushButton>("Kaydet"));
                kaydetBtn->addStyleClass(Bootstrap::Button::Primary);
                kaydetBtn->clicked().connect([=](){
                    model->setHeaderData (col,newName->text().toUTF8());
                    this->removeDialog (mDialog);
                });


                mDialog->show();
            } else{
                warnDialog ("Sütun Seçmediniz");
            }


        });






        auto colCikarBtn = cBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        colCikarBtn->setContentAlignment(AlignmentFlag::Center);
        colCikarBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                   Bootstrap::Grid::Medium::col_md_1+
                                   Bootstrap::Grid::Small::col_sm_1+
                                   Bootstrap::Grid::ExtraSmall::col_xs_1);
        colCikarBtn->addStyleClass(CSSStyle::Button::redButton);
        colCikarBtn->addWidget(cpp14::make_unique<WText>("Sütun Sil-"));
        colCikarBtn->setPadding(7,Side::Top|Side::Bottom);
        colCikarBtn->clicked().connect([=](){
            //            Wt::WStandardItem *root = model->invisibleRootItem();

            int col = -1;
            for( WModelIndex item : tableView->selectedIndexes () ){

                try {
                    col = item.column ();
                }  catch (const std::exception& e) {
                    LOG << "error: " << e.what () << "\n";
                    warnDialog (std::string("Sütun Bulunamadı: ")+e.what ());
                }

                if( col != -1 ){
                    auto btn = askConfirm (WString("<b>{1}</b> Sütunu Silmek İstediğinize Eminisiniz?").arg(tableItem.headers ()[col]).toUTF8());
                    btn->clicked().connect([=](){
                        model->removeColumn (col);
                    });
                }else{
                    warnDialog ("Sütun Seçmediniz");
                }
            }
        });


        auto saveBtnContainer = cBtnContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        saveBtnContainer->addStyleClass (CSSStyle::Button::greenButton+
                                         Bootstrap::Grid::Large::col_lg_2+
                                         Bootstrap::Grid::Medium::col_md_2+
                                         Bootstrap::Grid::Small::col_sm_2+
                                         Bootstrap::Grid::ExtraSmall::col_xs_3);

        saveBtnContainer->setContentAlignment (AlignmentFlag::Center);
        saveBtnContainer->setPadding(5,Side::Top|Side::Bottom);
        saveBtnContainer->addWidget (cpp14::make_unique<WText>("Kaydet"));
        saveBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        saveBtnContainer->clicked().connect([=](){

            if( baslikLineEdit->text().toUTF8().size() == 0 ){
                warnDialog ("Başlık İsmi Yeterli Değil");
                return;
            }

            std::vector<TableCell> cells;

            std::vector<std::string> headers;
            for( int i = 0 ; i < model->columnCount () ; i++ ){
                auto val = cpp17::any_cast<std::string>(model->headerData (i));
                headers.push_back (val);
            }


            for( int i = 0 ; i < model->rowCount (); i++ ){
                for( int j = 0 ; j < model->columnCount (); j++ ){
                    TableCell cell{i,j,model->item (i,j)->text ().toUTF8 ()};
                    cells.push_back (cell);
                }
            }

            SerikBLDCore::Faaliyet::TableItem __item;
            __item.setDocumentView (this->view());
            __item.setText (baslikLineEdit->text().toUTF8());
            __item.setHeaders (headers);

            for( const auto &cell : cells ){
                __item.setCell (cell.row,cell.col,cell.value);
            }
            this->setDocumentView(__item.view ());
            mChangeTable(baslikLineEdit->text().toUTF8(),headers,cells);

        });


        auto cancelBtnContainer = cBtnContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        cancelBtnContainer->addStyleClass (CSSStyle::Button::Red::IndianRedButton+
                                           Bootstrap::Grid::Large::col_lg_2+
                                           Bootstrap::Grid::Medium::col_md_2+
                                           Bootstrap::Grid::Small::col_sm_2+
                                           Bootstrap::Grid::ExtraSmall::col_xs_3+
                                           Bootstrap::Grid::Offset::Large::col_lg_8+
                                           Bootstrap::Grid::Offset::Medium::col_md_8+
                                           Bootstrap::Grid::Offset::Small::col_sm_8+
                                           Bootstrap::Grid::Offset::ExtraSmall::col_xs_6);
        cancelBtnContainer->setContentAlignment (AlignmentFlag::Center);
        cancelBtnContainer->setPadding(5,Side::Top|Side::Bottom);
        cancelBtnContainer->addWidget (cpp14::make_unique<WText>("İptal"));
        cancelBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);

        cancelBtnContainer->clicked().connect([=](){
            this->refreshWidget ();
        });

        setMargin (15,Side::Bottom);
    }

}

template<typename T>
void v2::Faaliyet::ItemContainer<T>::refreshWidget()
{


    ::WContainerWidget::clear ();


    if( !mViewMode ){
        this->addStyleClass ("faaliyetItem");
    }


    this->initWidgetType ();

    if( mViewMode ) return;

    auto gradientContainer = addWidget (cpp14::make_unique<WContainerWidget>());
    gradientContainer->setPositionScheme (PositionScheme::Absolute);
    gradientContainer->setOffsets (0,Side::Right);
    gradientContainer->setOffsets (0,Side::Top);
    gradientContainer->setHeight(1);
    gradientContainer->setWidth(WLength("100%"));
    gradientContainer->addStyleClass (CSSStyle::Gradient::grayGradient90);
    gradientContainer->addStyleClass("gra");


    if( !this->isPageBreak() ){
        auto editContainer = addWidget (cpp14::make_unique<WContainerWidget>());
        editContainer->setPositionScheme (PositionScheme::Absolute);
        editContainer->setOffsets (25,Side::Right);
        editContainer->setOffsets (0,Side::Top);
        editContainer->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
        editContainer->addWidget(cpp14::make_unique<WText>("Düzenle"));
        editContainer->decorationStyle().setCursor(Cursor::PointingHand);
        editContainer->setPadding(10,Side::Left|Side::Right);
        editContainer->addStyleClass("gra");

        //    this->doubleClicked ().connect([=](){
        //        this->editWidgetType ();
        //    });

        editContainer->clicked().connect([=] {
            this->editWidgetType ();
        });

    }




    auto delContainer = addWidget (cpp14::make_unique<WContainerWidget>());
    delContainer->setPositionScheme (PositionScheme::Absolute);
    delContainer->setOffsets (100,Side::Right);
    delContainer->setOffsets (0,Side::Top);
    delContainer->addStyleClass (CSSStyle::Button::Red::IndianRedButton);
    delContainer->addWidget(cpp14::make_unique<WText>("Sil"));
    delContainer->decorationStyle().setCursor(Cursor::PointingHand);
    delContainer->setPadding(10,Side::Left|Side::Right);
    delContainer->addStyleClass("gra");

    delContainer->clicked().connect([=] {
        auto btn = askConfirm ("Silmek İstediğinize Emin misiniz?");

        btn->clicked().connect([=](){
            mDeleteItemSignal(this->uuid());
        });
    });



    auto typeinfoContainer = addWidget (cpp14::make_unique<WContainerWidget>());
    typeinfoContainer->setPositionScheme (PositionScheme::Absolute);
    typeinfoContainer->setOffsets (160,Side::Right);
    typeinfoContainer->setOffsets (-20,Side::Top);
    typeinfoContainer->addStyleClass (CSSStyle::Button::grayButton);
    if( this->isBaslik() ){
        typeinfoContainer->addWidget(cpp14::make_unique<WText>("Ana Başlık"));
    }else if( this->isAltBaslik() ){
        typeinfoContainer->addWidget(cpp14::make_unique<WText>("Alt Başlık"));
    }else if( this->isTable() ){
        typeinfoContainer->addWidget(cpp14::make_unique<WText>("Tablo"));
    }else if( this->isImg() ){
        typeinfoContainer->addWidget(cpp14::make_unique<WText>("Resim"));
    }else if( this->isParagraf() ){
        typeinfoContainer->addWidget(cpp14::make_unique<WText>("Parağraf"));
    }else if( this->isPageBreak() ){
        typeinfoContainer->addWidget(cpp14::make_unique<WText>("Sayfa Sonu"));
    }
    typeinfoContainer->setPadding(10,Side::Left|Side::Right);
    typeinfoContainer->addStyleClass("gra");


    if( this->isImg() ){
        auto downloadimgContaier = addNewButton ("",false,20,CSSStyle::Button::greenButton);
        SerikBLDCore::Faaliyet::ImgItem imgItem;
        imgItem.setDocumentView (this->view());

        auto imgPath = mDB->downloadFileWeb (imgItem.getImgOid ().c_str ());

        Wt::WLink link = Wt::WLink(LinkType::Url,imgPath);
        link.setTarget(Wt::LinkTarget::NewWindow);

        std::unique_ptr<Wt::WAnchor> anchor =
                Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                    "indir");
        downloadimgContaier->addWidget(std::move(anchor));
    }





    int offright = 400;
    auto addBaslikPreContainer = addNewButton ("B▲",true,offright,CSSStyle::Button::Red::CrimsonButton);
    addBaslikPreContainer->clicked().connect([=](){
        this->addBaslikPre ();
    });

    auto addBaslikNextContainer = addNewButton ("B▼",false,offright,CSSStyle::Button::Red::CrimsonButton);
    addBaslikNextContainer->clicked().connect([=](){
        this->addBaslikPre (false);
    });


    auto addAltBaslikPreContainer = addNewButton ("A▲",true,offright-45,CSSStyle::Button::Red::IndianRedButton);
    addAltBaslikPreContainer->clicked().connect([=](){
        this->addAltBaslikPre ();

    });

    auto addAltBaslikNextContainer = addNewButton ("A▼",false,offright-45,CSSStyle::Button::Red::IndianRedButton);
    addAltBaslikNextContainer->clicked().connect([=](){
        this->addAltBaslikPre (false);

    });


    auto addParagrafPreContainer = addNewButton ("P▲",true,offright-90,CSSStyle::Button::Red::LightCoralButton);
    addParagrafPreContainer->clicked().connect([=](){
        this->addParagrafPre ();
    });

    auto addParagrafNextContainer = addNewButton ("P▼",false,offright-90,CSSStyle::Button::Red::LightCoralButton);
    addParagrafNextContainer->clicked().connect([=](){
        this->addParagrafPre (false);
    });

    auto addImgPreContainer = addNewButton ("R▲",true,offright-135,CSSStyle::Button::Red::LightSalmonButton);
    addImgPreContainer->clicked().connect([=](){
        this->addImgPre ();
    });

    auto addImgNextContainer = addNewButton ("R▼",false,offright-135,CSSStyle::Button::Red::LightSalmonButton);
    addImgNextContainer->clicked().connect([=](){
        this->addImgPre (false);
    });


    auto addTablePreContainer = addNewButton ("T▲",true,offright-180,CSSStyle::Button::Red::SalmonButton);
    addTablePreContainer->clicked().connect([=](){
        this->addTablePre ();
    });

    auto addTableNextContainer = addNewButton ("T▼",false,offright-180,CSSStyle::Button::Red::SalmonButton);
    addTableNextContainer->clicked().connect([=](){
        this->addTablePre (false);
    });

    auto addPageBreakBackContainer = addNewButton ("SS▲",true,offright-240,CSSStyle::Button::Red::FireBrickButton);
    addPageBreakBackContainer->setToolTip("Öncesine Sayfa Sonu Ekle");
    addPageBreakBackContainer->clicked().connect([=](){
        this->addPageBreakPre ();
    });

    auto addPageBreakNextContainer = addNewButton ("SS▼",false,offright-240,CSSStyle::Button::Red::FireBrickButton);
    addPageBreakNextContainer->setToolTip("Sonrasına Sayfa Sonu Ekle");
    addPageBreakNextContainer->clicked().connect([=](){
        this->addPageBreakPre (false);
    });




    auto addContainer = addWidget (cpp14::make_unique<WContainerWidget>());
    addContainer->setPositionScheme (PositionScheme::Absolute);
    addContainer->setOffsets (0,Side::Right);
    addContainer->setOffsets (0,Side::Top);
    addContainer->addStyleClass("gra");

    auto popupPtr = Wt::cpp14::make_unique<Wt::WPopupMenu>();

    popupPtr->addSeparator ();



    auto baslikEkleMenu = Wt::cpp14::make_unique<Wt::WPopupMenu>();
    baslikEkleMenu->addItem("icon/btn-flat.jpg", ("Öncesine Ekle***"))->triggered().connect([=] {
        this->addBaslikPre ();
    });
    baslikEkleMenu->addItem("icon/btn-flat.jpg", ("Sonrasına Ekle"))->triggered().connect([=] {
        this->addBaslikPre (false);
    });

    popupPtr->addMenu( ("Başlık Ekle"), std::move(baslikEkleMenu));





    auto altBaslikEkleMenu = Wt::cpp14::make_unique<Wt::WPopupMenu>();

    altBaslikEkleMenu->addItem( ("Öncesine Alt Başlık Ekle"))->triggered().connect([=] {
        this->addAltBaslikPre ();
    });

    altBaslikEkleMenu->addItem( ("Sonrasına Alt Başlık Ekle"))->triggered().connect([=] {
        this->addAltBaslikPre (false);
    });

    popupPtr->addMenu( ("Alt Başlık Ekle"), std::move(altBaslikEkleMenu));



    auto paragrafEkleMenu = Wt::cpp14::make_unique<Wt::WPopupMenu>();

    paragrafEkleMenu->addItem( ("Öncesine Paragraf Ekle"))->triggered().connect([=] {
        this->addParagrafPre ();
    });
    paragrafEkleMenu->addItem( ("Sonrasına Paragraf Ekle"))->triggered().connect([=] {
        this->addParagrafPre (false);
    });

    popupPtr->addMenu( ("Paragraf Ekle"), std::move(paragrafEkleMenu));



    auto resimEkleMenu = Wt::cpp14::make_unique<Wt::WPopupMenu>();

    resimEkleMenu->addItem( ("Öncesine Resim Ekle"))->triggered().connect([=] {
        this->addImgPre ();
    });


    resimEkleMenu->addItem( ("Sonrasına Resim Ekle"))->triggered().connect([=] {
        this->addImgPre (false);
    });

    popupPtr->addMenu( ("Resim Ekle"), std::move(resimEkleMenu));



    auto tabloEkleMenu = Wt::cpp14::make_unique<Wt::WPopupMenu>();


    tabloEkleMenu->addItem( ("Mali Tablo Ekle"))->triggered().connect([=] {

        auto mDialog = createDialog ("Mali Tablo Ekle");

        auto baslikLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
        baslikLineEdit->setPlaceholderText ("Tablo Bilgisi. (Örn: 2020 Mali Yılı Bütçe Harcama Raporu)");

        mDialog->contents ()->addWidget(cpp14::make_unique<WBreak>());

        auto tableContainer = mDialog->contents ()->addWidget(cpp14::make_unique<WContainerWidget>());
        tableContainer->setWidth(WLength("100%"));
        tableContainer->setMinimumSize(WLength::Auto,250);

        auto table = tableContainer->addWidget(Wt::cpp14::make_unique<Wt::WTableView>());

        auto infoText = mDialog->contents()->addWidget(cpp14::make_unique<WText>("Bilgi: <b>Sadece Gerekli Alanları Doldurunuz.</b>"));
        infoText->setAttributeValue(Style::style,Style::color::color (Style::color::Red::Crimson));


        table->setColumnResizeEnabled(true);
        table->setColumnAlignment(0, AlignmentFlag::Center);
        table->setHeaderAlignment(0, AlignmentFlag::Center);
        table->setAlternatingRowColors(true);
        table->setRowHeight(28);
        table->setHeaderHeight(28);
        table->setSelectionMode(SelectionMode::Single);
        table->setEditTriggers(EditTrigger::SelectedClicked);
        table->setSelectionBehavior(SelectionBehavior::Items);
        table->setSortingEnabled(false);
        auto model = std::make_shared<WStandardItemModel>(0,7);

        table->setModel(model);

        table->setOverflow(Overflow::Scroll);
        table->setMaximumSize (WLength::Auto,WLength(400));

        table->doubleClicked().connect([=](WModelIndex index,WMouseEvent event ){
            if( !index.data (ItemDataRole::Display).has_value () ){
                auto item = std::make_unique<WStandardItem>("0");
                item->setEditable (true);
                model->setItem (index.row (),index.column (),std::move(item));
            }
        });



        model->setHeaderData (0,Orientation::Horizontal,"#",ItemDataRole::Display);
        model->setHeaderData (1,Orientation::Horizontal,"Verilen Ödenek",ItemDataRole::Display);
        model->setHeaderData (2,Orientation::Horizontal,"Eklenen(+)",ItemDataRole::Display);
        model->setHeaderData (3,Orientation::Horizontal,"Düşülen(+)",ItemDataRole::Display);
        model->setHeaderData (4,Orientation::Horizontal,"Net Ödenek",ItemDataRole::Display);
        model->setHeaderData (5,Orientation::Horizontal,"Ödenen",ItemDataRole::Display);
        model->setHeaderData (6,Orientation::Horizontal,"İptal Edilen",ItemDataRole::Display);

        model->insertRow (0,std::make_unique<WStandardItem>("Personel Giderleri"));
        model->insertRow (1,std::make_unique<WStandardItem>("Sosyal Güv. Kur. Dev. Pir. Giderleri"));
        model->insertRow (2,std::make_unique<WStandardItem>("Mal ve Hizmet Alım Giderleri"));
        model->insertRow (3,std::make_unique<WStandardItem>("Faiz Giderleri"));
        model->insertRow (4,std::make_unique<WStandardItem>("Sermaye Giderleri"));
        model->insertRow (5,std::make_unique<WStandardItem>("Yedek Ödenekler"));
        model->insertRow (6,std::make_unique<WStandardItem>("Genel Toplam"));



        for( int i = 0 ; i < model->rowCount () ; i++ ){
            for( int j = 1 ; j < model->columnCount () ; j++ ){
                auto item = std::make_unique<WStandardItem>("0");
                item->setEditable (true);
                model->setItem (i,j,std::move(item));
            }
        }


        auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Ekle+"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);



        saveBtn->clicked ().connect ([=](){

            if( baslikLineEdit->text().toUTF8().size() == 0 ){
                infoText->setText("Başlık Girmediniz!");
                return;
            }

            std::vector<TableCell> cells;

            std::vector<std::string> headers;
            headers.push_back ("#");
            headers.push_back ("Verilen Ödenek");
            headers.push_back ("Eklenen(+)");
            headers.push_back ("Düşülen(+)");
            headers.push_back ("Net Ödenek");
            headers.push_back ("Ödenen");
            headers.push_back ("İptal Edilen");

            for( int i = 0 ; i < model->rowCount (); i++ ){
                for( int j = 0 ; j < model->columnCount (); j++ ){
                    TableCell cell{i,j,model->item (i,j)->text ().toUTF8 ()};
                    cells.push_back (cell);
                }
            }


            mAddTableAftereSignal(baslikLineEdit->text().toUTF8(),headers,cells);

            removeDialog (mDialog);
        });
        mDialog->setWidth(1100);
        mDialog->setMaximumSize (WLength::Auto,WLength(640));

        mDialog->show ();
    });




    tabloEkleMenu->addItem( ("Öncesine Tablo Ekle"))->triggered().connect([=] {

        this->addTablePre ();

        //        auto mDialog = createDialog ("Tablo Ekle");

        //        auto baslikLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
        //        baslikLineEdit->setPlaceholderText ("Tablo Bilgisi. (Örn: Gider İstatistikleri)");

        //        mDialog->contents ()->addWidget(cpp14::make_unique<WBreak>());

        //        auto tableContainer = mDialog->contents ()->addWidget(cpp14::make_unique<WContainerWidget>());
        //        tableContainer->setWidth(WLength("100%"));
        //        tableContainer->setMinimumSize(WLength::Auto,250);

        //        auto table = tableContainer->addWidget(Wt::cpp14::make_unique<Wt::WTableView>());

        //        auto infoText = mDialog->contents()->addWidget(cpp14::make_unique<WText>(""));
        //        infoText->setAttributeValue(Style::style,Style::color::color (Style::color::Red::Crimson));


        //        table->setColumnResizeEnabled(true);
        //        table->setColumnAlignment(0, AlignmentFlag::Center);
        //        table->setHeaderAlignment(0, AlignmentFlag::Center);
        //        table->setAlternatingRowColors(true);
        //        table->setRowHeight(28);
        //        table->setHeaderHeight(28);
        //        table->setSelectionMode(SelectionMode::Single);
        //        table->setEditTriggers(EditTrigger::SelectedClicked);
        //        table->setSelectionBehavior(SelectionBehavior::Items);
        //        table->setSortingEnabled(false);


        //        table->setOverflow(Overflow::Scroll);
        //        table->setMaximumSize (WLength::Auto,WLength(400));


        //        auto model = std::make_shared<WStandardItemModel>(0,0);
        //        table->setModel(model);



        //        auto rowEkleContainer = mDialog->contents()->addWidget(cpp14::make_unique<WContainerWidget>());
        //        rowEkleContainer->addStyleClass(Bootstrap::Grid::row);

        //        auto rowEkleBtn = rowEkleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        //        rowEkleBtn->setContentAlignment(AlignmentFlag::Center);
        //        rowEkleBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
        //                                  Bootstrap::Grid::Medium::col_md_2+
        //                                  Bootstrap::Grid::Small::col_sm_2+
        //                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
        //        rowEkleBtn->addStyleClass(CSSStyle::Button::blueButton);
        //        rowEkleBtn->addWidget(cpp14::make_unique<WText>("Satır Ekle+"));
        //        rowEkleBtn->setPadding(7,Side::Top|Side::Bottom);



        //        auto colBaslik = rowEkleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        //        colBaslik->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
        //                                 Bootstrap::Grid::Medium::col_md_3+
        //                                 Bootstrap::Grid::Small::col_sm_3+
        //                                 Bootstrap::Grid::ExtraSmall::col_xs_3);
        //        colBaslik->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_5+
        //                                 Bootstrap::Grid::Offset::Medium::col_md_5+
        //                                 Bootstrap::Grid::Offset::Small::col_sm_5+
        //                                 Bootstrap::Grid::Offset::ExtraSmall::col_xs_5);

        //        auto sutunBaslikLineEdit = colBaslik->addWidget(cpp14::make_unique<WLineEdit>());
        //        sutunBaslikLineEdit->setPlaceholderText("Sütun Başlığını Girniz");


        //        auto colEkleBtn = rowEkleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        //        colEkleBtn->setContentAlignment(AlignmentFlag::Center);
        //        colEkleBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
        //                                  Bootstrap::Grid::Medium::col_md_2+
        //                                  Bootstrap::Grid::Small::col_sm_2+
        //                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
        //        colEkleBtn->addStyleClass(CSSStyle::Button::blueButton);
        //        colEkleBtn->addWidget(cpp14::make_unique<WText>("Sütun Ekle+"));
        //        colEkleBtn->setPadding(7,Side::Top|Side::Bottom);



        //        rowEkleBtn->clicked().connect([=](){




        //            Wt::WStandardItem *root = model->invisibleRootItem();

        //            if( root->columnCount () <= 0 ){
        //                infoText->setText("<b>ilk Önce Sütun Ekleyiniz</b>");
        //                return;
        //            }

        //            infoText->setText("");


        //            std::vector<std::unique_ptr<WStandardItem>> list;

        //            for (int row = 0; row < root->columnCount (); ++row) {
        //                auto topLevel = std::make_unique<Wt::WStandardItem>();
        //                topLevel->setText("");
        //                topLevel->setEditable (true);
        //                list.push_back (std::move(topLevel));

        //            }
        //            root->insertRow (root->rowCount (),std::move(list));

        //            for( int i = 0 ; i < root->rowCount () ; i++ ){
        //                for( int j = 0 ; j < root->columnCount () ; j++ ){
        //                    if( !model->index (i,j).data ().has_value () ){
        //                        auto item = std::make_unique<WStandardItem>("");
        //                        item->setEditable (true);
        //                        model->setItem (i,j,std::move(item));
        //                    }
        //                }
        //            }

        //        });



        //        colEkleBtn->clicked().connect([=](){

        //            infoText->setText("");

        //            Wt::WStandardItem *root = model->invisibleRootItem();

        //            auto topLevel = std::make_unique<Wt::WStandardItem>();
        //            topLevel->setText("");
        //            topLevel->setEditable (true);
        //            std::vector<std::unique_ptr<WStandardItem>> list;
        //            list.push_back (std::move(topLevel));
        //            root->insertColumn (root->columnCount (),std::move(list));

        //            model->setHeaderData (root->columnCount ()-1,Orientation::Horizontal,sutunBaslikLineEdit->text().toUTF8(),ItemDataRole::Display);

        //            for( int i = 0 ; i < root->rowCount () ; i++ ){
        //                for( int j = 0 ; j < root->columnCount () ; j++ ){
        //                    if( !model->index (i,j).data ().has_value () ){
        //                        auto item = std::make_unique<WStandardItem>("");
        //                        item->setEditable (true);
        //                        model->setItem (i,j,std::move(item));
        //                    }
        //                }
        //            }


        //        });


        //        auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Ekle+"));
        //        saveBtn->addStyleClass (Bootstrap::Button::Primary);



        //        saveBtn->clicked ().connect ([=](){

        //            if( baslikLineEdit->text().toUTF8().size() == 0 ){
        //                infoText->setText("Başlık Girmediniz!");
        //                return;
        //            }

        //            std::vector<TableCell> cells;

        //            std::vector<std::string> headers;
        //            for( int i = 0 ; i < model->columnCount () ; i++ ){
        //                auto val = cpp17::any_cast<std::string>(model->headerData (i));
        //                headers.push_back (val);
        //            }


        //            for( int i = 0 ; i < model->rowCount (); i++ ){
        //                for( int j = 0 ; j < model->columnCount (); j++ ){
        //                    TableCell cell{i,j,model->item (i,j)->text ().toUTF8 ()};
        //                    cells.push_back (cell);
        //                }
        //            }

        //            mAddTableBeforeSignal(baslikLineEdit->text().toUTF8(),headers,cells);
        //            removeDialog (mDialog);
        //        });

        //        mDialog->setWidth(1100);
        //        mDialog->setMaximumSize (WLength::Auto,WLength(640));

        //        mDialog->show ();
    });

    tabloEkleMenu->addItem( ("Sonrasına Tablo Ekle"))->triggered().connect([=] {

        this->addTablePre (false);
        //        auto mDialog = createDialog ("Tablo Ekle");

        //        auto baslikLineEdit = mDialog->contents ()->addWidget (cpp14::make_unique<WLineEdit>());
        //        baslikLineEdit->setPlaceholderText ("Tablo Bilgisi. (Örn: Gider İstatistikleri)");

        //        mDialog->contents ()->addWidget(cpp14::make_unique<WBreak>());

        //        auto tableContainer = mDialog->contents ()->addWidget(cpp14::make_unique<WContainerWidget>());
        //        tableContainer->setWidth(WLength("100%"));
        //        tableContainer->setMinimumSize(WLength::Auto,250);

        //        auto table = tableContainer->addWidget(Wt::cpp14::make_unique<Wt::WTableView>());

        //        auto infoText = mDialog->contents()->addWidget(cpp14::make_unique<WText>(""));
        //        infoText->setAttributeValue(Style::style,Style::color::color (Style::color::Red::Crimson));


        //        table->setColumnResizeEnabled(true);
        //        table->setColumnAlignment(0, AlignmentFlag::Center);
        //        table->setHeaderAlignment(0, AlignmentFlag::Center);
        //        table->setAlternatingRowColors(true);
        //        table->setRowHeight(28);
        //        table->setHeaderHeight(28);
        //        table->setSelectionMode(SelectionMode::Single);
        //        table->setEditTriggers(EditTrigger::SelectedClicked);
        //        table->setSelectionBehavior(SelectionBehavior::Items);
        //        table->setSortingEnabled(false);


        //        table->setOverflow(Overflow::Scroll);
        //        table->setMaximumSize (WLength::Auto,WLength(400));


        //        auto model = std::make_shared<WStandardItemModel>(0,0);
        //        table->setModel(model);



        //        auto rowEkleContainer = mDialog->contents()->addWidget(cpp14::make_unique<WContainerWidget>());
        //        rowEkleContainer->addStyleClass(Bootstrap::Grid::row);

        //        auto rowEkleBtn = rowEkleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        //        rowEkleBtn->setContentAlignment(AlignmentFlag::Center);
        //        rowEkleBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
        //                                  Bootstrap::Grid::Medium::col_md_2+
        //                                  Bootstrap::Grid::Small::col_sm_2+
        //                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
        //        rowEkleBtn->addStyleClass(CSSStyle::Button::blueButton);
        //        rowEkleBtn->addWidget(cpp14::make_unique<WText>("Satır Ekle+"));
        //        rowEkleBtn->setPadding(7,Side::Top|Side::Bottom);



        //        auto colBaslik = rowEkleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        //        colBaslik->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
        //                                 Bootstrap::Grid::Medium::col_md_3+
        //                                 Bootstrap::Grid::Small::col_sm_3+
        //                                 Bootstrap::Grid::ExtraSmall::col_xs_3);
        //        colBaslik->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_5+
        //                                 Bootstrap::Grid::Offset::Medium::col_md_5+
        //                                 Bootstrap::Grid::Offset::Small::col_sm_5+
        //                                 Bootstrap::Grid::Offset::ExtraSmall::col_xs_5);

        //        auto sutunBaslikLineEdit = colBaslik->addWidget(cpp14::make_unique<WLineEdit>());
        //        sutunBaslikLineEdit->setPlaceholderText("Sütun Başlığını Girniz");


        //        auto colEkleBtn = rowEkleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        //        colEkleBtn->setContentAlignment(AlignmentFlag::Center);
        //        colEkleBtn->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
        //                                  Bootstrap::Grid::Medium::col_md_2+
        //                                  Bootstrap::Grid::Small::col_sm_2+
        //                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
        //        colEkleBtn->addStyleClass(CSSStyle::Button::blueButton);
        //        colEkleBtn->addWidget(cpp14::make_unique<WText>("Sütun Ekle+"));
        //        colEkleBtn->setPadding(7,Side::Top|Side::Bottom);


        //        rowEkleBtn->clicked().connect([=](){




        //            Wt::WStandardItem *root = model->invisibleRootItem();

        //            if( root->columnCount () <= 0 ){
        //                infoText->setText("<b>ilk Önce Sütun Ekleyiniz</b>");
        //                return;
        //            }

        //            infoText->setText("");


        //            std::vector<std::unique_ptr<WStandardItem>> list;

        //            for (int row = 0; row < root->columnCount (); ++row) {
        //                auto topLevel = std::make_unique<Wt::WStandardItem>();
        //                topLevel->setText("");
        //                topLevel->setEditable (true);
        //                list.push_back (std::move(topLevel));

        //            }
        //            root->insertRow (root->rowCount (),std::move(list));

        //            for( int i = 0 ; i < root->rowCount () ; i++ ){
        //                for( int j = 0 ; j < root->columnCount () ; j++ ){
        //                    if( !model->index (i,j).data ().has_value () ){
        //                        auto item = std::make_unique<WStandardItem>("");
        //                        item->setEditable (true);
        //                        model->setItem (i,j,std::move(item));
        //                    }
        //                }
        //            }

        //        });


        //        colEkleBtn->clicked().connect([=](){

        //            infoText->setText("");

        //            Wt::WStandardItem *root = model->invisibleRootItem();

        //            auto topLevel = std::make_unique<Wt::WStandardItem>();
        //            topLevel->setText("");
        //            topLevel->setEditable (true);
        //            std::vector<std::unique_ptr<WStandardItem>> list;
        //            list.push_back (std::move(topLevel));
        //            root->insertColumn (root->columnCount (),std::move(list));

        //            model->setHeaderData (root->columnCount ()-1,Orientation::Horizontal,sutunBaslikLineEdit->text().toUTF8(),ItemDataRole::Display);

        //            for( int i = 0 ; i < root->rowCount () ; i++ ){
        //                for( int j = 0 ; j < root->columnCount () ; j++ ){
        //                    if( !model->index (i,j).data ().has_value () ){
        //                        auto item = std::make_unique<WStandardItem>("");
        //                        item->setEditable (true);
        //                        model->setItem (i,j,std::move(item));
        //                    }
        //                }
        //            }


        //        });

        //        auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Ekle+"));
        //        saveBtn->addStyleClass (Bootstrap::Button::Primary);

        //        saveBtn->clicked ().connect ([=](){

        //            if( baslikLineEdit->text().toUTF8().size() == 0 ){
        //                infoText->setText("Başlık Girmediniz!");
        //                return;
        //            }

        //            std::vector<TableCell> cells;

        //            std::vector<std::string> headers;
        //            for( int i = 0 ; i < model->columnCount () ; i++ ){
        //                auto val = cpp17::any_cast<std::string>(model->headerData (i));
        //                headers.push_back (val);
        //            }


        //            for( int i = 0 ; i < model->rowCount (); i++ ){
        //                for( int j = 0 ; j < model->columnCount (); j++ ){
        //                    TableCell cell{i,j,model->item (i,j)->text ().toUTF8 ()};
        //                    cells.push_back (cell);
        //                }
        //            }

        //            mAddTableAftereSignal(baslikLineEdit->text().toUTF8(),headers,cells);
        //            removeDialog (mDialog);
        //        });

        //        mDialog->setWidth(1100);
        //        mDialog->setMaximumSize (WLength::Auto,WLength(640));

        //        mDialog->show ();
    });

    popupPtr->addMenu( ("Tablo Ekle"), std::move(tabloEkleMenu));

    auto addButton = addContainer->addWidget (cpp14::make_unique<WPushButton>(""));
    addButton->addStyleClass (Bootstrap::Label::Primary);
    addButton->setMenu (std::move(popupPtr));

}


namespace v2 {
namespace Faaliyet {
QVector<SerikBLDCore::Faaliyet::RaporItem *> SquencedRaporItem::list() const
{
    return mList;
}

}
}
