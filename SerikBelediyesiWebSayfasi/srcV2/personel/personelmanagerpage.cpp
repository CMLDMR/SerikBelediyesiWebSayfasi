#include "personelmanagerpage.h"
#include <QDate>
#include <QTime>


using namespace SerikBLDCore;

IKManagerPage::IKManagerPage(SerikBLDCore::DB *_db)
    :ContainerWidget ("İnsan Kaynakları Yönetimi"),mDB(_db)
{

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5+
                                  "boxShadow");
//        container->setHeight (100);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::DeepPink));
        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Personel Yönetimi"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::weight::bold);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect (this,&IKManagerPage::initPersonelManager );
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());

        container->addStyleClass (Bootstrap::Grid::Offset::Large::col_lg_4+
                                  Bootstrap::Grid::Offset::Medium::col_md_4+
                                  Bootstrap::Grid::Offset::Small::col_sm_2+
                                  Bootstrap::Grid::Offset::ExtraSmall::col_xs_2+
                                  "boxShadow");

        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5+
                                  "boxShadow");
//        container->setHeight (100);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::MediumVioletRed));
        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Birim Yönetimi"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::weight::bold);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect ( this , &IKManagerPage::initBirimManager );

    }

    Content ()->setMargin (15,Side::Top);
}

void IKManagerPage::initPersonelManager()
{
    Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<PersonelManagerPage>(mDB));

}

void IKManagerPage::initBirimManager()
{
    Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<BirimManagerPage>(mDB));
}


PersonelManagerPage::PersonelManagerPage(SerikBLDCore::DB *_db)
    :SerikBLDCore::PersonelManager (_db),
      ContainerWidget ("Personel Yönetimi"),
      mBirimManager( new SerikBLDCore::BirimManager(_db))
{
    this->setLimit (1000);

    initBirimList ();
    Content ()->setMargin (15,Side::Top|Side::Bottom);
    Content ()->addStyleClass ("boxShadow");
}

void PersonelManagerPage::onList(const QVector<SerikBLDCore::IK::Personel> *mlist)
{

    Content ()->clear ();


    auto titleContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    titleContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    titleContainer->setMargin (15,Side::Bottom);
    auto titleText = titleContainer->addWidget (cpp14::make_unique<WText>(WString("<b>{1}</b> - {2} Adet Personel").arg(0).arg (mlist->count ()),TextFormat::UnsafeXHTML));
    titleContainer->addStyleClass (Bootstrap::ContextualBackGround::bg_warning);

    if( mlist->count () == 0 )
    {
        this->showPopUpMessage ("Bu Birim de Hiç Personel Yok.","msg");
        return;
    }

    int mudurExist = 0;

    std::string titleString{};

    for( const auto &item : *mlist )
    {
        titleString = item.Birim().toStdString();
        auto container = Content ()->addWidget (cpp14::make_unique<PersonelThumpPage>(item,this->getDB ()));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
        container->setMargin (2,Side::Bottom);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->setContentAlignment (AlignmentFlag::Center);
        container->setHeight(200);

        container->clicked ().connect ([=](){
            Content ()->clear ();
            auto pPage = Content ()->addWidget (cpp14::make_unique<PersonelPage>(item,this->getDB ()));
            pPage->ClickedBack ().connect ([=](){
                SerikBLDCore::IK::Personel filter;
                filter.setBirim (birimComboBoxFilter->currentText ().toUTF8 ().c_str ());
                this->UpdateList (filter);
            });
        });

        if( item.statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur && item.Birim () != "Başkanlık" )
        {
            mudurExist++;
        }
    }

    titleText->setText(WString("<b>{1}</b> - {2} Adet Personel").arg(titleString).arg(mlist->count()));

    if( !mListFromSearchText ){
        if( mudurExist == 0 )
        {

            auto birimOid = this->mBirimManager->FindOneItem(SerikBLDCore::IK::BirimItem().setBirimAdi(titleString.c_str()));

            SerikBLDCore::IK::Personel personelFilter;
            personelFilter.addMudurluk(birimOid.oid().value().to_string().c_str());

            auto mAltMudur = this->FindOneItem(personelFilter);

            if( mAltMudur.AdSoyad().isEmpty() ){
                this->showPopUpMessage ("!Dikkat! Bu Birime <u>Müdür</u> Atanmamış","hata");

            }else{
                auto container = Content ()->addWidget (cpp14::make_unique<PersonelThumpPage>(std::move(mAltMudur),this->getDB ()));
                container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                          Bootstrap::Grid::Medium::col_md_2+
                                          Bootstrap::Grid::Small::col_sm_3+
                                          Bootstrap::Grid::ExtraSmall::col_xs_4);
                container->setMargin (5,Side::Bottom);
                container->decorationStyle ().setCursor (Cursor::PointingHand);
                container->setContentAlignment (AlignmentFlag::Center);
                container->setHeight(200);

                container->clicked ().connect ([=](){
                    Content ()->clear ();
                    auto pPage = Content ()->addWidget (cpp14::make_unique<PersonelPage>(std::move(mAltMudur),this->getDB ()));
                    pPage->ClickedBack ().connect ([=](){
                        SerikBLDCore::IK::Personel filter;
                        filter.setBirim (birimComboBoxFilter->currentText ().toUTF8 ().c_str ());
                        this->UpdateList (filter);
                    });
                });
            }




        }

        if( mudurExist > 1 )
        {
            this->showPopUpMessage ("!Dikkat! Bu Birim de Birden Fazla <u>Müdür</u> Atanmış","hata");
        }
    }





}

void PersonelManagerPage::initBirimList()
{
    Header ()->clear ();
    Header ()->setMargin (25,Side::Top|Side::Bottom);
    Header ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
    Header ()->addStyleClass ("boxShadow");

    mListFromSearchText = false;

    {
        auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setContentAlignment (AlignmentFlag::Center);
        container->addWidget (cpp14::make_unique<WText>("<b>Yeni Personel Tanımla</b>"));
        container->addStyleClass (Bootstrap::Button::Primary);
        container->setMargin(5,Side::Top|Side::Bottom);
        container->setPadding(5,Side::Top|Side::Bottom);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked ().connect ( this , &PersonelManagerPage::initNewPersonelWidget );
    }

    {
        auto container = Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setContentAlignment (AlignmentFlag::Center);
        container->addWidget (cpp14::make_unique<WText>("<b>Print to A4</b>"));
        container->addStyleClass (Bootstrap::Button::info);
        container->setMargin(5,Side::Top|Side::Bottom);
        container->setPadding(5,Side::Top|Side::Bottom);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked ().connect ([=](){

            auto list = this->List();
            std::string json{"["};

            for( const auto &personelItem : list ){

                std::string jsonObj{"{"};

                jsonObj += "\"adsoyad\":\""+personelItem.AdSoyad().toStdString()+"\",";
                jsonObj += "\"birim\":\""+personelItem.Birim().toStdString()+"\",";

                auto url = this->downloadFileWeb(personelItem.FotoOid());
                jsonObj += "\"fotourl\":\"" + url+"\"";
                jsonObj += "}";

                json += jsonObj+",";
            }
            json += "]";
            container->doJavaScript("printPersonelListToA4("+json+");");
        });
    }




    auto birimFiltercontainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    birimFiltercontainer->addStyleClass (Bootstrap::Grid::col_full_12);
    birimFiltercontainer->setContentAlignment (AlignmentFlag::Center);

    mBirimManager->setLimit (500);
    mBirimManager->UpdateList();
    for( const auto& item : mBirimManager->List () )
    {

        {
            auto container_ = birimFiltercontainer->addNew<WContainerWidget>();
            container_->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_3+
                                      Bootstrap::Grid::ExtraSmall::col_xs_4);

            auto container = container_->addNew<WContainerWidget>();
            container->addStyleClass(CSSStyle::Button::blueButton);
            container->addNew<WText>(item.birimAdi().toStdString().size() > 15 ? item.birimAdi().mid(0,12).append("...").toStdString() : item.birimAdi().toStdString() );
            container->setMargin(5,Side::Top|Side::Bottom);
            container->setPadding(5,Side::Top|Side::Bottom);

            container->clicked().connect([=](){
                mListFromSearchText = false;
                this->UpdateList (SerikBLDCore::IK::Personel().setBirim (item.birimAdi().toStdString().c_str()));
            });

        }
    }



    //        auto statuFiltercontainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    //        statuFiltercontainer->addStyleClass (Bootstrap::Grid::col_full_12);
    //        statuFiltercontainer->setContentAlignment (AlignmentFlag::Center);

    //        auto statuComboBox = statuFiltercontainer->addWidget (cpp14::make_unique<WComboBox>());

    //        statuComboBox->addItem (SerikBLDCore::IK::Statu::Personel);
    //        statuComboBox->addItem (SerikBLDCore::IK::Statu::Sef);
    //        statuComboBox->addItem (SerikBLDCore::IK::Statu::Mudur);
    //        statuComboBox->addItem (SerikBLDCore::IK::Statu::BaskanYardimcisi);
    //        statuComboBox->addItem (SerikBLDCore::IK::Statu::Baskan);

    //        statuComboBox->changed ().connect ([=](){
    //                mListFromSearchText = true;
    //                this->UpdateList (SerikBLDCore::IK::Personel().setStatu (statuComboBox->currentText ().toUTF8 ().c_str ()));
    //        });







    auto nameFiltercontainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    nameFiltercontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+
                                        Bootstrap::Grid::Medium::col_md_10+
                                        Bootstrap::Grid::Small::col_sm_10+
                                        Bootstrap::Grid::ExtraSmall::col_xs_8);
    nameFiltercontainer->setContentAlignment (AlignmentFlag::Center);
    auto searchLineEdit = nameFiltercontainer->addWidget (cpp14::make_unique<WLineEdit>());
    searchLineEdit->setPlaceholderText ("En Az 3 Harf Giriniz");
    //        searchLineEdit->changed ().connect ([=](){
    //            if( searchLineEdit->text ().toUTF8 ().size () >= 3 )
    //            {
    //                mListFromSearchText = true;
    //                SerikBLDCore::IK::Personel item;
    //                item.append("$regex",searchLineEdit->text ().toUTF8 ()).append("$options","i");
    //                SerikBLDCore::IK::Personel filter;
    //                filter.append(SerikBLDCore::IK::Personel::KeyAdSoyad,item.view ());
    //                this->UpdateList (filter);
    //            }
    //        });

    auto nameFilterSearchcontainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    nameFilterSearchcontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                              Bootstrap::Grid::Medium::col_md_2+
                                              Bootstrap::Grid::Small::col_sm_2+
                                              Bootstrap::Grid::ExtraSmall::col_xs_4);
    nameFilterSearchcontainer->addNew<WText>("Ara");
    nameFilterSearchcontainer->addStyleClass(CSSStyle::Button::blueButton);
    nameFilterSearchcontainer->setPadding(3,Side::Bottom|Side::Top);
    nameFilterSearchcontainer->clicked().connect([=](){

        if( searchLineEdit->text ().toUTF8 ().size () >= 3 )
        {
            mListFromSearchText = true;
            SerikBLDCore::IK::Personel item;
            item.append("$regex",searchLineEdit->text ().toUTF8 ()).append("$options","i");
            SerikBLDCore::IK::Personel filter;
            filter.append(SerikBLDCore::IK::Personel::KeyAdSoyad,item.view ());
            this->UpdateList (filter);
        }else{
            this->showPopUpMessage("En Az 3 Karakter Giriniz");
        }


    });






}

void PersonelManagerPage::initNewPersonelWidget()
{
    Content ()->clear ();
    auto yeniPersonel = Content ()->addWidget (cpp14::make_unique<YeniPersonelWidget>());
    yeniPersonel->addStyleClass (Bootstrap::Grid::col_full_12);
    yeniPersonel->PersonelSaved ().connect ([=]( ){
        auto val = this->uploadfile (yeniPersonel->photoFilePath ());



        yeniPersonel->setFotoOid (val.view().get_oid ().value.to_string ().c_str ());

        auto insertedID = this->InsertItem (*yeniPersonel);
        if( insertedID.size () )
        {
            this->showPopUpMessage ("Personel Kaydı Başarılı","msg");
            SerikBLDCore::IK::Personel filter;
            filter.setOid (insertedID);
            this->UpdateList (filter);
        }
    });
}

void PersonelManagerPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (errorText,"hata");
}

BirimManagerPage::BirimManagerPage(SerikBLDCore::DB *_db)
    :SerikBLDCore::BirimManager (_db),
      ContainerWidget ("Birim Yönetimi"),
      mPersonelManager( new SerikBLDCore::PersonelManager(_db))
{
    this->Footer()->removeStyleClass(CSSStyle::Shadows::shadow8px);
    this->Footer()->setMargin(25,Side::Top|Side::Bottom);

    Succes.connect ([=](const std::string &msg){
        this->showPopUpMessage (msg);
    });

    Error.connect ([=](const std::string &msg){
        this->showPopUpMessage (msg,"hata");
    });


    this->setLimit (100);
    this->UpdateList ();


    Header ()->setMargin (25,Side::Top|Side::Bottom);
    Header ()->addStyleClass ("boxShadow");
    Header ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);

    auto newBirimContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    newBirimContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+
                                      Bootstrap::Grid::Medium::col_md_7+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
    auto newBirimLineEdit = newBirimContainer->addWidget (cpp14::make_unique<WLineEdit>());
    newBirimLineEdit->setPlaceholderText ("Yeni Birim Adını Giriniz");


    auto newBirimSiraContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    newBirimSiraContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_3+
                                      Bootstrap::Grid::ExtraSmall::col_xs_3);
    auto newBirimSiraNoSpinBox = newBirimSiraContainer->addWidget (cpp14::make_unique<WSpinBox>());
    newBirimSiraNoSpinBox->setMaximum(100);
    newBirimSiraNoSpinBox->setMinimum(-1);



    auto newBirimSaveContainer = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    newBirimSaveContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                          Bootstrap::Grid::Medium::col_md_3+
                                          Bootstrap::Grid::Small::col_sm_3+
                                          Bootstrap::Grid::ExtraSmall::col_xs_3);
    auto newBirimKaydet = newBirimSaveContainer->addWidget (cpp14::make_unique<WPushButton>("Yeni Ekle+"));
    newBirimKaydet->addStyleClass (Bootstrap::Button::Primary);
    newBirimKaydet->clicked ().connect ([=](){

        auto count = this->countItem (SerikBLDCore::IK::BirimItem().setBirimAdi (newBirimLineEdit->text ().toUTF8 ().c_str ())
                                      .setSiraNo(newBirimSiraNoSpinBox->value()));

        if( count )
        {
            this->showPopUpMessage ("Bu Birim Zaten Var","hata");
            return;
        }

        if( this->InsertItem (SerikBLDCore::IK::BirimItem().setBirimAdi (newBirimLineEdit->text ().toUTF8 ().c_str ())
                              .setSiraNo(newBirimSiraNoSpinBox->value())).size () ){
            this->showPopUpMessage ("Yeni Birim Başarılı Bir Şekilde Kayıt Edildi");
            this->UpdateList ();
        }
    });

}

void BirimManagerPage::onList(const QVector<SerikBLDCore::IK::BirimItem> *mlist)
{

    Content ()->clear ();
    for ( const auto &item : *mlist ) {



        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail+
                                  Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6+
                                  Bootstrap::ContextualBackGround::bg_warning);
        container->setContentAlignment (AlignmentFlag::Center);
        container->setMargin (5,Side::Top|Side::Bottom);
        auto text = container->addWidget (cpp14::make_unique<WText>(item.birimAdi ().toStdString ()));
        text->setMargin (5,Side::Top|Side::Bottom);
        container->setPositionScheme (PositionScheme::Relative);
        container->setHeight (55);
        text->decorationStyle ().setCursor (Cursor::PointingHand);
        container->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
        text->clicked ().connect ([=](){
            this->initBirim( item );
        });

        auto SiraContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        SiraContainer->setPositionScheme (PositionScheme::Absolute);
        auto siraText = SiraContainer->addWidget (cpp14::make_unique<WText>("SıraNO: "+std::to_string(item.SiraNo())));
        siraText->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::DimGray)+
                                     Style::font::size::s10px);
        siraText->setPadding (1,Side::Left|Side::Right);
        SiraContainer->setOffsets (0,Side::Bottom|Side::Right);



        auto xContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        xContainer->setPositionScheme (PositionScheme::Absolute);
        xContainer->addStyleClass ("boxShadow");
        xContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Red));
        auto deleteText = xContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
        deleteText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+
                                       Style::font::weight::bold+
                                       Style::font::size::s10px);
        deleteText->setPadding (5,Side::Left|Side::Right);
        xContainer->setOffsets (0,Side::Top|Side::Right);
        xContainer->decorationStyle ().setCursor (Cursor::PointingHand);

        xContainer->clicked ().connect ([=](){

            SerikBLDCore::IK::Personel personelItem;

            personelItem.setBirim ( text->text ().toUTF8 ().c_str () );

            auto __countItem = mPersonelManager->countItem (personelItem);
            if( __countItem ){
                this->showPopUpMessage (WString("Bu Birime Ait {1} Adet Personel Var.Önce Bu Personelleri Taşıyın").arg (__countItem).toUTF8 (),"hata");
            }else{
                auto btn = askConfirm ("Silmek İstediğinize Eminmisiniz?");
                btn->clicked ().connect ([=](){
                    SerikBLDCore::IK::BirimItem item_;
                    item_.setOid (container->attributeValue (Style::dataoid).toUTF8 ().c_str ());

                    if( this->DeleteItem (item_) ){
                        this->showPopUpMessage ("Birimi Silindi");
                        this->UpdateList ();
                    }else{
                        this->showPopUpMessage ("Bir Hata Oluştu: " + this->getLastError ().toStdString (),"hata");
                    }
                });
            }



        });
    }

}

void BirimManagerPage::initAltBirimler( const std::string &birimOid , const std::string &birimAdi )
{
    this->Footer ()->clear ();
    this->Footer ()->addWidget (cpp14::make_unique<AltBirimManagerPage>(this->getDB (),bsoncxx::oid{birimOid},birimAdi));
}

void BirimManagerPage::initBirim(const SerikBLDCore::IK::BirimItem &birimItem)
{
    this->Footer ()->clear ();
    auto birimWidget = this->Footer ()->addNew<BirimWidget>(birimItem);
    birimWidget->addStyleClass(Bootstrap::Grid::col_full_12);


    birimWidget->ChangedClicked().connect([=]( const std::string &oid,
                                          const std::string &birimAdi,
                                          const std::int32_t &siraNo){

        SerikBLDCore::IK::BirimItem __item;
        __item.setBirimAdi(birimAdi.c_str());
        __item.setSiraNo(siraNo);
        __item.setOid(oid);

        if( this->UpdateItem(__item) ){
            this->UpdateList();
            this->Footer()->clear();
        }

    });

    auto __container = this->Footer ()->addWidget (cpp14::make_unique<AltBirimManagerPage>(this->getDB (),birimItem.oid().value(),birimItem.birimAdi().toStdString()));
    __container->setMargin(25,Side::Top|Side::Bottom);
}



PersonelThumpPage::PersonelThumpPage(SerikBLDCore::IK::Personel &personel, SerikBLDCore::DB *_db)
    :SerikBLDCore::IK::Personel (personel),mDB(_db)
{


    auto fotoContinaer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    fotoContinaer->setWidth (90);
    fotoContinaer->setHeight (120);
    fotoContinaer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    auto filePath = mDB->downloadFileWeb (this->FotoOid ());
    fotoContinaer->setAttributeValue (Style::style,Style::background::url (filePath)+
                                      Style::background::size::cover+
                                      Style::background::repeat::norepeat+
                                      Style::background::position::center_center);

    Content ()->addWidget (cpp14::make_unique<WBreak>());
    auto isimText = Content ()->addWidget (cpp14::make_unique<WText>(this->AdSoyad ().toStdString ()));
    isimText->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DarkSlateGray)+Style::color::color(Style::color::White::Snow));
    isimText->setPadding(5,Side::Left|Side::Right);

    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur )
    {
        Content ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
    }

    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::BaskanYardimcisi )
    {
        Content ()->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
    }

    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Baskan )
    {
        Content ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
    }
    Content ()->addWidget (cpp14::make_unique<WBreak>());
    Content ()->addWidget (cpp14::make_unique<WText>("<i>"+this->statu ().toStdString ()+"</i>"));
}

PersonelThumpPage::PersonelThumpPage(const Personel &personel, SerikBLDCore::DB *_db)
    :SerikBLDCore::IK::Personel (personel),mDB(_db)
{
    auto fotoContinaer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    fotoContinaer->setWidth (90);
    fotoContinaer->setHeight (120);
    fotoContinaer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    auto filePath = mDB->downloadFileWeb (this->FotoOid ());
    fotoContinaer->setAttributeValue (Style::style,Style::background::url (filePath)+
                                      Style::background::size::cover+
                                      Style::background::repeat::norepeat+
                                      Style::background::position::center_center);

    Content ()->addWidget (cpp14::make_unique<WBreak>());
    auto isimText = Content ()->addWidget (cpp14::make_unique<WText>(this->AdSoyad ().toStdString ()));
    isimText->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DarkSlateGray)+Style::color::color(Style::color::White::Snow));
    isimText->setPadding(5,Side::Left|Side::Right);

    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur )
    {
        Content ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
    }

    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::BaskanYardimcisi )
    {
        Content ()->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
    }

    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Baskan )
    {
        Content ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
    }
    Content ()->addWidget (cpp14::make_unique<WBreak>());
    Content ()->addWidget (cpp14::make_unique<WText>("<i>"+this->statu ().toStdString ()+"</i>"));
}

void PersonelThumpPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (this->AdSoyad ().toStdString () +": " +errorText,"hata");
}

PersonelPage::PersonelPage(const Personel &personel, SerikBLDCore::DB *_db)
    :SerikBLDCore::IK::Personel (personel),
      SerikBLDCore::PersonelManager (_db),
      mBirimManager( new SerikBLDCore::BirimManager(_db)),
      mPersonelManager( new SerikBLDCore::PersonelManager(_db))
{

    mPersonelManager->setLimit (1000);
    mBirimManager->setLimit (100);
    setMargin(25,Side::Top);
    Footer ()->setMargin (25,Side::Top);
    mSMSManager = Footer ()->addWidget (cpp14::make_unique<SMSManager>(this->getDB ()));
    mSMSManager->addStyleClass (Bootstrap::Grid::col_full_12);

    mSMSManager->smsSended ().connect ([=](const std::string &sms){
        this->showPopUpMessage ("SMS Gönderildi: " + sms);
        this->initSMSLog ();
    });

    mSMSManager->ErrorOccured ().connect ([=](const std::string &errMsg){
        this->showPopUpMessage (errMsg,"hata");
    });



    initHeader ();

    initContent ();

    this->initSMSLog ();

    mSMSManager->messageOccured ().connect ([=](const std::string &errMsg){
        this->showPopUpMessage (errMsg,"msg");
        this->initSMSLog ();
    });

}

PersonelPage::PersonelPage(const SerikBLDCore::IK::Personel &personel, SerikBLDCore::DB *_db, SerikBLDCore::User *mUser)
    :SerikBLDCore::IK::Personel (personel),
      SerikBLDCore::PersonelManager (_db),
      mBirimManager( new SerikBLDCore::BirimManager(_db) ),
      mPersonelManager( new SerikBLDCore::PersonelManager(_db) )
{
    mPersonelManager->setLimit (1000);
    mBirimManager->setLimit (100);
    setMargin(25,Side::Top);
    Footer ()->setMargin (25,Side::Top);
    mSMSManager = Footer ()->addWidget (cpp14::make_unique<SMSManager>(this->getDB ()));
    mSMSManager->addStyleClass (Bootstrap::Grid::col_full_12);

    mSMSManager->smsSended ().connect ([=](const std::string &sms){
        this->showPopUpMessage ("SMS Gönderildi: " + sms);
        this->initSMSLog ();
    });

    mSMSManager->ErrorOccured ().connect ([=](const std::string &errMsg){
        this->showPopUpMessage (errMsg,"hata");
    });



    initHeader ();

    initOwnContent ();

    this->initSMSLog ();


    mSMSManager->messageOccured ().connect ([=](const std::string &errMsg){
        this->showPopUpMessage (errMsg,"msg");
        this->initSMSLog ();
    });
}

void PersonelPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage (this->AdSoyad ().toStdString () +": " +errorText,"hata");
}

Signal<NoClass> &PersonelPage::ClickedBack()
{
    return _clickedBack;
}

void PersonelPage::initHeader()
{
    Header ()->clear ();
    auto container = Header() -> addWidget (cpp14::make_unique<WContainerWidget>());
    auto filePath = this->downloadFileWeb (this->FotoOid ());
    container->setAttributeValue (Style::style,Style::background::url (filePath)+Style::background::size::cover+
                                  Style::background::repeat::norepeat+
                                  Style::background::position::center_center);
    container->setWidth (120);
    container->setHeight (160);
    container->setMinimumSize (120,160);
    container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    Header ()->addWidget (cpp14::make_unique<WBreak>());
    auto fileManagerContainer = Header ()->addWidget (cpp14::make_unique<FileUploaderWidget>("Fotoğraf Yükle") );
    fileManagerContainer->setType (FileUploaderWidget::Image);

    fileManagerContainer->Uploaded ().connect ([=](){

        if( fileManagerContainer->isUploaded () )
        {
            auto val = this->uploadfile (fileManagerContainer->fileLocation ());
            this->setFotoOid (val.view().get_oid ().value.to_string ().c_str ());
            if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyFotoOid,val) ){
                auto filePath = this->downloadFileWeb (this->FotoOid ());
                container->setAttributeValue (Style::style,Style::background::url (filePath)+Style::background::size::contain+Style::background::repeat::norepeat+Style::background::position::center_center);
                container->setWidth (120);
                container->setHeight (160);
                container->setMinimumSize (120,160);

                this->showPopUpMessage ("Fotoğraf Değiştirildi");
            }
        }else{
            this->showPopUpMessage ("Fotoğraf Yüklenmedi","hata");
        }

    });
}

void PersonelPage::initContent()
{
    auto vLayout = Content ()->setLayout (cpp14::make_unique<WVBoxLayout>());

    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Ad Soyad</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setPlaceholderText ("Ad Soyad Giriniz");
        lineEdit->setText (this->AdSoyad ().toStdString ());

        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Kaydet</h6",TextFormat::UnsafeXHTML));
        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);

        kaydetText->clicked ().connect ([=](){
            if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyAdSoyad,lineEdit->text ().toUTF8 ().c_str ()) ){
                this->showPopUpMessage ("İsim Değiştirildi","msg");
            }
        });
    }


    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Telefon Numarası</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setPlaceholderText ("Telefon Numarası Giriniz");
        lineEdit->setText (this->telefon ().toStdString ());

        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Kaydet</h6>",TextFormat::UnsafeXHTML));
        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);
        kaydetText->clicked ().connect ([=](){

            if( SerikBLDCore::IK::Personel::checkTelefonFormat (lineEdit->text ().toUTF8 ().c_str ()) )
            {
                if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyTelefon,lineEdit->text ().toUTF8 ().c_str ()) ){
                    this->showPopUpMessage ("Telefon Değiştirildi","msg");
                }
            }else{
                this->showPopUpMessage ("Telefon Numarası Hatalı","hata");
            }
        });
    }


    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Asıl Birimi</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);
        auto birimComboBox = lineEditContainer->addWidget (cpp14::make_unique<WComboBox>());
        auto counter = 0;
        for( const auto &item : this->birimList () )
        {
            birimComboBox->addItem (item.toStdString ());
            if( this->Birim () == item )
            {
                birimComboBox->setCurrentIndex (counter);
            }
            counter++;
        }

        birimComboBox->sactivated ().connect ([=](const WString& str){
            if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyBirimi,str.toUTF8 ().c_str ()) ){
                this->showPopUpMessage ("Birim Değiştirildi","msg");
            }
        });
    }







    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Statu</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);

        auto statuComboBox = lineEditContainer->addWidget (cpp14::make_unique<WComboBox>());
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Personel);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Sef);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Mudur);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::BaskanYardimcisi);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Baskan);

        if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Personel )
        {
            statuComboBox->setCurrentIndex (0);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Sef  ){
            statuComboBox->setCurrentIndex (1);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur  ){
            statuComboBox->setCurrentIndex (2);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::BaskanYardimcisi  ){
            statuComboBox->setCurrentIndex (3);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Baskan  ){
            statuComboBox->setCurrentIndex (4);
        }

        statuComboBox->sactivated ().connect ([=](const WString& str){

            if( str.toUTF8 () == SerikBLDCore::IK::Statu::Baskan )
            {
                if( this->Birim () != "Başkanlık" )
                {
                    this->showPopUpMessage ("Statunun <u><i>Başkan</i></u> Yapılabilmesi için Önce Birimi <u><i>Başkanlık</i></u> Olarak Değiştiriniz!","hata");


                    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Personel )
                    {
                        statuComboBox->setCurrentIndex (0);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Sef  ){
                        statuComboBox->setCurrentIndex (1);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur  ){
                        statuComboBox->setCurrentIndex (2);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::BaskanYardimcisi  ){
                        statuComboBox->setCurrentIndex (3);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Baskan  ){
                        statuComboBox->setCurrentIndex (4);
                    }


                    return;
                }
            }

            if( this->statu ().toStdString () == str.toUTF8 () )
            {
                return;
            }


            if( str.toUTF8 () == SerikBLDCore::IK::Statu::Sef || str.toUTF8 () == SerikBLDCore::IK::Statu::Personel || str.toUTF8 () == SerikBLDCore::IK::Statu::BaskanYardimcisi )
            {
                if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyStatu,str.toUTF8 ().c_str ()) ){
                    this->setStatu (str.toUTF8 ().c_str ());
                    this->showPopUpMessage ("Statu: <i>" + str.toUTF8 () +"</i> Değiştirildi: ","msg");
                }
            }else{
                auto count = this->countItem (SerikBLDCore::IK::Personel().setStatu (str.toUTF8 ().c_str ()).setBirim (this->Birim ()));

                if( count )
                {
                    this->showPopUpMessage ("Bu Birime Ait Bir <u><i>"+ str.toUTF8 () + "</i></u> Bulunmakta. Mevcut <u><i>"+ str.toUTF8 () +"</i></u> Değiştiriniz","hata");

                    if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Personel )
                    {
                        statuComboBox->setCurrentIndex (0);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Sef  ){
                        statuComboBox->setCurrentIndex (1);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur  ){
                        statuComboBox->setCurrentIndex (2);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::BaskanYardimcisi  ){
                        statuComboBox->setCurrentIndex (3);
                    }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Baskan  ){
                        statuComboBox->setCurrentIndex (4);
                    }


                    return;
                }

                if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyStatu,str.toUTF8 ().c_str ()) ){
                    this->setStatu (str.toUTF8 ().c_str ());
                    this->showPopUpMessage ("Statu: <i>" + str.toUTF8 () +"</i> Değiştirildi: ","msg");
                }
            }

        });

    }



    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Büro Personeli</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WCheckBox>());
        lineEdit->setChecked (this->BuroPersoneli ());

        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Kaydet</h6",TextFormat::UnsafeXHTML));
        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);
        kaydetText->clicked ().connect ([=](){
            if( this->setField(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyBuroPersonel,lineEdit->isChecked ()) ){
                this->showPopUpMessage ("Büro Personeli Durumu Değiştirildi","msg");
            }

        });
    }




    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Şifresi</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setText (this->sifre ().toStdString ());
        lineEdit->setEchoMode (EchoMode::Password);
        lineEdit->setEnabled (false);

        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Oluştur & Gönder</h6",TextFormat::UnsafeXHTML));
        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);

        kaydetText->clicked ().connect ([=](){
            this->showPopUpMessage ("SMS ID Alana Kadar Bekleyin","msg");
            auto newSifre = QRandomGenerator::global ()->generate64 ()%90000 + 10000;
            SerikBLDCore::SMS::SMSItem item;
            item.setSMS (QString("Şifreniz: %1").arg (newSifre));
            item.setNumara (this->telefon ());
            if( mSMSManager->insertAndSendSMS (item) ){
                this->showPopUpMessage ("SMS Gönderildi");
                if( this->setField(Personel().setOid (this->oid ().value ().to_string ()),Personel::KeyPassword,std::to_string (newSifre)) ){
                    this->setSifre (std::to_string (newSifre).c_str ());
                    this->initSMSLog ();
                }
            }
        });
    }






    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Diğer Müdürlükleri</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto birimOidText = vContainer->addWidget (cpp14::make_unique<WText>());
        birimOidText->addStyleClass (Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto birimComboBox = lineEditContainer->addWidget (cpp14::make_unique<WComboBox>());
        auto counter = 0;

        mBirimManager->setLimit (100);

        SerikBLDCore::FindOptions findOptions;
        findOptions.setLimit (100);

        auto birimList = mBirimManager->UpdateList (SerikBLDCore::IK::BirimItem(),findOptions);

        for(const auto &item : birimList )
        {
            birimComboBox->addItem (item.birimAdi ().toStdString ());
            if( this->Birim () == item.birimAdi () )
            {
                birimComboBox->setCurrentIndex (counter);
                if( item.birimAdi ().toStdString () == birimComboBox->currentText ().toUTF8 () )
                {
                    birimOidText->setText (item.oid ().value ().to_string ());
                }
            }
            counter++;
        }


        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Ekle</h6>",TextFormat::UnsafeXHTML));
        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);




        auto mMudurluklerContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mMudurluklerContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        auto rMudurlukContainer = mMudurluklerContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        rMudurlukContainer->addStyleClass (Bootstrap::Grid::row);
        rMudurlukContainer->clear ();
        auto mList = this->mudurlukList ();

        for( auto item : mList )
        {
            auto bContainer = rMudurlukContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            bContainer->addStyleClass (Bootstrap::ContextualBackGround::bg_info+" boxRadius boxShadow");
            for( auto bItem : mBirimManager->List () )
            {
                if( bItem.oid ().value ().to_string () == item.to_string () )
                {
                    auto text = bContainer->addWidget (cpp14::make_unique<WText>(bItem.birimAdi ().toStdString ()));
                    text->addStyleClass (Bootstrap::Grid::Large::col_lg_10+
                                         Bootstrap::Grid::Medium::col_md_10+
                                         Bootstrap::Grid::Small::col_sm_10+
                                         Bootstrap::Grid::ExtraSmall::col_xs_10);
                    text->setAttributeValue (Style::dataoid,bItem.oid ().value ().to_string ());

                    auto deltext = bContainer->addWidget (cpp14::make_unique<WText>("<h6>X</h6>",TextFormat::UnsafeXHTML));
                    deltext->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                            Bootstrap::Grid::Medium::col_md_2+
                                            Bootstrap::Grid::Small::col_sm_2+
                                            Bootstrap::Grid::ExtraSmall::col_xs_2);
                    deltext->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
                    deltext->decorationStyle ().setCursor (Cursor::PointingHand);

                    deltext->clicked ().connect ([=](){

                        auto delRes = this->pullValue(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),
                                                      SerikBLDCore::IK::Personel::KeyMudurlukler,
                                                      bsoncxx::oid{text->attributeValue (Style::dataoid).toUTF8 ()});
                        if( delRes )
                        {
                            if( delRes.value ().modified_count () )
                            {
                                this->showPopUpMessage ("Birim Silindi","msg");
                                rMudurlukContainer->removeWidget(bContainer);
                            }
                        }
                    });
                    break;
                }
            }
        }



        kaydetText->clicked ().connect ([=](){

            if( this->statu ().toStdString () != SerikBLDCore::IK::Statu::Mudur )
            {
                this->showPopUpMessage ("İlk Önce Müdür Olarak Atamalısınız","hata");
                return;
            }


            if( this->mudurlukList ().contains (bsoncxx::oid{birimOidText->text ().toUTF8 ()}) )
            {
                this->showPopUpMessage ("Bu Müdürlük Zaten Var","hata");
            }else{

                {
                    std::string errormessage = "";

                    mPersonelManager->UpdateList (SerikBLDCore::IK::Personel().setStatu (SerikBLDCore::IK::Statu::Mudur.c_str ()));
                    auto cursor = this->mPersonelManager->List ();
                    if( !cursor.empty () )
                    {

                        for( auto item : cursor )
                        {
                            if( item.Birim ().toStdString () == birimComboBox->currentText ().toUTF8 () )
                            {
                                errormessage = birimComboBox->currentText ().toUTF8 () + "; Bu Birime Ait Müdür Var-> " + item.AdSoyad ().toStdString ();
                                break;
                            }

                            if( item.mudurlukList ().contains (bsoncxx::oid{birimOidText->text ().toUTF8 ()}) )
                            {
                                errormessage = birimComboBox->currentText ().toUTF8 () + "; Bu Birime Ait Müdür Var-> " + item.AdSoyad ().toStdString ();
                                break;
                            }
                        }



                    }else{
                        this->showPopUpMessage ("Bir Hata Oluştu. Veritabanına ulaşılamıyor","hata");
                        return;
                    }

                    if( errormessage.size () )
                    {
                        this->showPopUpMessage (errormessage,"hata");
                        return;
                    }


                }



                auto res = this->pushValue(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),
                                           SerikBLDCore::IK::Personel::KeyMudurlukler,
                                           bsoncxx::oid{birimOidText->text ().toUTF8 ()});

                if( res )
                {
                    if( res.value ().modified_count () )
                    {
                        this->showPopUpMessage ("Birim Değiştirildi","msg");
                        this->addMudurluk (birimOidText->text ().toUTF8 ().c_str ());
                        rMudurlukContainer->clear ();
                        for( auto item : this->mudurlukList () )
                        {
                            auto bContainer = rMudurlukContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                            bContainer->addStyleClass (Bootstrap::ContextualBackGround::bg_info+" boxRadius boxShadow");
                            for( auto bItem : mBirimManager->List () )
                            {
                                if( bItem.oid ().value ().to_string () == item.to_string () )
                                {


                                    auto text = bContainer->addWidget (cpp14::make_unique<WText>(bItem.birimAdi ().toStdString ()+ " - " + bItem.oid ().value ().to_string ()));
                                    text->addStyleClass (Bootstrap::Grid::Large::col_lg_10+
                                                         Bootstrap::Grid::Medium::col_md_10+
                                                         Bootstrap::Grid::Small::col_sm_10+
                                                         Bootstrap::Grid::ExtraSmall::col_xs_10);
                                    text->setAttributeValue (Style::dataoid,bItem.oid ().value ().to_string ());
                                    auto deltext = bContainer->addWidget (cpp14::make_unique<WText>("<h6>X</h6>",TextFormat::UnsafeXHTML));
                                    deltext->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                                            Bootstrap::Grid::Medium::col_md_2+
                                                            Bootstrap::Grid::Small::col_sm_2+
                                                            Bootstrap::Grid::ExtraSmall::col_xs_2);
                                    deltext->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
                                    deltext->decorationStyle ().setCursor (Cursor::PointingHand);

                                    deltext->clicked ().connect ([=](){
                                        auto delRes = this->pullValue(SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()),
                                                                      SerikBLDCore::IK::Personel::KeyMudurlukler,
                                                                      bsoncxx::oid{text->attributeValue (Style::dataoid).toUTF8 ()});
                                        if( delRes )
                                        {
                                            if( delRes.value ().modified_count () )
                                            {
                                                //                                                std::cout << "Before Deleted: "<<  text->attributeValue (Style::dataoid).toUTF8 () << std::endl;

                                                this->deleteMudurluk (text->attributeValue (Style::dataoid).toUTF8 ().c_str ());

                                                //                                                for( auto _item : this->mudurlukList () )
                                                //                                                {
                                                //                                                    std::cout << "After Deleted: "<<  _item.to_string () << std::endl;
                                                //                                                }


                                                this->showPopUpMessage ("Birim Silindi","msg");
                                                rMudurlukContainer->removeWidget(bContainer);
                                            }
                                        }
                                    });



                                }
                            }
                        }




                    }
                }



            }

            //            mMudurlukList.insert (birimComboBox->currentText ().toUTF8 ().c_str (),birimOidText->text ().toUTF8 ().c_str ());
            //            rMudurlukContainer->clear ();
            //            QMap<QString, QString>::const_iterator i = mMudurlukList.constBegin();
            //            while (i != mMudurlukList.constEnd()) {
            //                auto bContainer = rMudurlukContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            //                bContainer->addStyleClass (Bootstrap::ContextualBackGround::bg_info+" boxRadius boxShadow");

            //                auto text = bContainer->addWidget (cpp14::make_unique<WText>(i.key ().toStdString ()+ " - " + i.value ().toStdString ()));
            //                text->addStyleClass (Bootstrap::Grid::Large::col_lg_10+
            //                                     Bootstrap::Grid::Medium::col_md_10+
            //                                     Bootstrap::Grid::Small::col_sm_10+
            //                                     Bootstrap::Grid::ExtraSmall::col_xs_10);

            //                auto deltext = bContainer->addWidget (cpp14::make_unique<WText>("<h6>X</h6>",TextFormat::UnsafeXHTML));
            //                deltext->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
            //                                     Bootstrap::Grid::Medium::col_md_2+
            //                                     Bootstrap::Grid::Small::col_sm_2+
            //                                     Bootstrap::Grid::ExtraSmall::col_xs_2);
            //                deltext->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
            //                deltext->decorationStyle ().setCursor (Cursor::PointingHand);


            //                ++i;
            //            }
        });





        birimComboBox->sactivated ().connect ([=](const WString& str){

            for( auto item : mBirimManager->List () )
            {
                if( item.birimAdi ().toStdString () == str.toUTF8 () )
                {
                    birimOidText->setText (item.oid ().value ().to_string ());
                    break;
                }
            }
        });
    }







    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Çalıştığı Servisler</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto altBirimComboBox = lineEditContainer->addWidget (cpp14::make_unique<WComboBox>());
        auto list = this->altBirimList (bsoncxx::oid{this->birimOid (this->Birim ().toStdString ())});
        for( auto item : list.keys () )
        {
            altBirimComboBox->addItem ( list[item].toStdString ());
        }

        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Kaydet</h6>",TextFormat::UnsafeXHTML));

        auto altBirimList = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        altBirimList->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9+
                                     Bootstrap::Grid::Offset::Large::col_lg_2+Bootstrap::Grid::Offset::Medium::col_md_2+Bootstrap::Grid::Offset::Small::col_sm_2+Bootstrap::Grid::Offset::ExtraSmall::col_xs_2);
        auto rAltBirimList = altBirimList->addWidget (cpp14::make_unique<WContainerWidget>());
        rAltBirimList->addStyleClass (Bootstrap::Grid::row);

        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);



        {
            auto list_ = this->altBirimList (bsoncxx::oid{this->birimOid (this->Birim ().toStdString ())});

            rAltBirimList->clear ();
            for( auto item : this->altBirimOidList () ){

                auto altBirimAdi = list_[item.to_string ().c_str ()];

                auto container1 = rAltBirimList->addWidget (cpp14::make_unique<WContainerWidget>());
                container1->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_10+Bootstrap::Grid::ExtraSmall::col_xs_10);
                container1->setContentAlignment (AlignmentFlag::Center);
                auto text1 = container1->addWidget (cpp14::make_unique<WText>(altBirimAdi.toStdString ()));
                container1->setMargin (3,Side::Bottom|Side::Top);


                auto container2 = rAltBirimList->addWidget (cpp14::make_unique<WContainerWidget>());
                container2->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                container2->setContentAlignment (AlignmentFlag::Center);
                auto text2 = container2->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
                container2->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkSalmon)+Style::color::color (Style::color::White::Snow));
                container2->setMargin (3,Side::Bottom|Side::Top);
                container2->decorationStyle ().setCursor (Cursor::PointingHand);
                container2->setAttributeValue (Style::dataoid,item.to_string ());
                container2->clicked ().connect ([=](){

                    auto upt = this->pullValue(Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyAltBirim,bsoncxx::oid{container2->attributeValue (Style::dataoid).toUTF8 ()});
                    if( upt )
                    {
                        auto __bOid = bsoncxx::oid{container2->attributeValue (Style::dataoid).toUTF8 ()};
                        auto __valueOid = bsoncxx::types::bson_value::value (bsoncxx::types::b_oid{__bOid});
                        this->pullArray (SerikBLDCore::IK::Personel::KeyAltBirim,bsoncxx::types::bson_value::value(__valueOid));
                        rAltBirimList->removeWidget(container1);
                        rAltBirimList->removeWidget(container2);
                        this->showPopUpMessage ("Servis Kaldırıldı");
                    }
                });


            }

        }


        kaydetText->clicked ().connect ([=](){


            auto list_ = this->altBirimList (bsoncxx::oid{this->birimOid (this->Birim ().toStdString ())});

            this->addAltBirim (list_.key (altBirimComboBox->currentText ().toUTF8 ().c_str ()));

            if( this->UpdateItem (*this) )
            {
                auto list = this->altBirimList (bsoncxx::oid{this->birimOid (this->Birim ().toStdString ())});

                rAltBirimList->clear ();
                for( auto item : this->altBirimOidList () ){

                    auto altBirimAdi = list[item.to_string ().c_str ()];
                    auto container1 = rAltBirimList->addWidget (cpp14::make_unique<WContainerWidget>());
                    container1->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_10+Bootstrap::Grid::ExtraSmall::col_xs_10);
                    container1->setContentAlignment (AlignmentFlag::Center);
                    auto text1 = container1->addWidget (cpp14::make_unique<WText>(altBirimAdi.toStdString ()));
                    container1->setMargin (3,Side::Bottom|Side::Top);


                    auto container2 = rAltBirimList->addWidget (cpp14::make_unique<WContainerWidget>());
                    container2->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
                    container2->setContentAlignment (AlignmentFlag::Center);
                    auto text2 = container2->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
                    container2->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkSalmon)+Style::color::color (Style::color::White::Snow));
                    container2->setMargin (3,Side::Bottom|Side::Top);
                    container2->decorationStyle ().setCursor (Cursor::PointingHand);
                    container2->setAttributeValue (Style::dataoid,item.to_string ());
                    container2->clicked ().connect ([=](){

                        auto upt = this->pullValue(Personel().setOid (this->oid ().value ().to_string ()),SerikBLDCore::IK::Personel::KeyAltBirim,bsoncxx::oid{container2->attributeValue (Style::dataoid).toUTF8 ()});
                        if( upt )
                        {
                            auto __bOid = bsoncxx::oid{container2->attributeValue (Style::dataoid).toUTF8 ()};
                            auto __valueOid = bsoncxx::types::bson_value::value (bsoncxx::types::b_oid{__bOid});
                            this->pullArray (SerikBLDCore::IK::Personel::KeyAltBirim,bsoncxx::types::bson_value::value(__valueOid));
                            rAltBirimList->removeWidget(container1);
                            rAltBirimList->removeWidget(container2);
                            this->showPopUpMessage ("Servis Kaldırıldı");
                        }
                    });

                }
                this->showPopUpMessage ("Çalıştığı Servis Eklendi");
            }else{
                this->showPopUpMessage ("Çalıştığı Servis Eklenemedi","err");
            }





        });
    }


    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Bu Personel Dondur</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::col_full_12);
        text->addStyleClass (Bootstrap::Label::Default+Bootstrap::Label::Danger);
        text->decorationStyle ().setCursor (Cursor::PointingHand);
        text->clicked ().connect([=](){

            Item item(SerikBLDCore::IK::Personel::CollectionHold);

            item.setDocumentView (this->view ());

            auto ins = this->insertItem ( item );

            if( ins )
            {
                if( ins.value ().result ().inserted_count () )
                {

                    auto del = this->deleteItem (SerikBLDCore::IK::Personel().setOid (this->oid ().value ().to_string ()));
                    if( del )
                    {
                        if( del.value ().deleted_count () )
                        {
                            this->showPopUpMessage ("Personel Taşındı","msg");
                            _clickedBack.emit (NoClass()); // Personel Listesi Sayfasına Geri Dön
                        }else{
                            this->showPopUpMessage ("Personel Yedeklendi Ancak Silinemedi","hata");
                        }
                    }else{
                        this->showPopUpMessage ("Personel Yedeklendi Ancak Silinemedi","hata");
                    }
                }else{
                    this->showPopUpMessage ("Personel Yedeklenemedi","hata");
                }
            }else{
                this->showPopUpMessage ("Personel Yedeklenemedi","hata");
            }



        });



    }
}

void PersonelPage::initOwnContent()
{
    auto vLayout = Content ()->setLayout (cpp14::make_unique<WVBoxLayout>());

    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Ad Soyad</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setPlaceholderText ("Ad Soyad Giriniz");
        lineEdit->setText (this->AdSoyad ().toStdString ());
        lineEdit->setEnabled (false);

    }


    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Telefon Numarası</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setPlaceholderText ("Telefon Numarası Giriniz");
        lineEdit->setText (this->telefon ().toStdString ());
        lineEdit->setEnabled (false);

    }


    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Birimi</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);
        auto birimComboBox = lineEditContainer->addWidget (cpp14::make_unique<WComboBox>());
        auto counter = 0;
        for( auto item : this->birimList () )
        {
            birimComboBox->addItem (item.toStdString ());
            if( this->Birim () == item )
            {
                birimComboBox->setCurrentIndex (counter);
            }
            counter++;
        }

        birimComboBox->setDisabled (true);
    }

    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Statu</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);

        auto statuComboBox = lineEditContainer->addWidget (cpp14::make_unique<WComboBox>());
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Personel);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Sef);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Mudur);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::BaskanYardimcisi);
        statuComboBox->addItem (SerikBLDCore::IK::Statu::Baskan);

        if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Personel )
        {
            statuComboBox->setCurrentIndex (0);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Sef  ){
            statuComboBox->setCurrentIndex (1);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Mudur  ){
            statuComboBox->setCurrentIndex (2);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::BaskanYardimcisi  ){
            statuComboBox->setCurrentIndex (3);
        }else if( this->statu ().toStdString () == SerikBLDCore::IK::Statu::Baskan  ){
            statuComboBox->setCurrentIndex (4);
        }

        statuComboBox->setDisabled (true);

    }

    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Büro Personeli</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WCheckBox>());
        lineEdit->setChecked (this->BuroPersoneli ());
        lineEdit->setEnabled (false);

    }


    {
        auto rContainer = vLayout->addWidget (cpp14::make_unique<WContainerWidget>());

        auto vContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        vContainer->addStyleClass (Bootstrap::Grid::row);

        auto text = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Şifresi</h6>",TextFormat::UnsafeXHTML));
        text->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        text->addStyleClass (Bootstrap::Label::Default);

        auto lineEditContainer = vContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineEditContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto lineEdit = lineEditContainer->addWidget (cpp14::make_unique<WLineEdit>());
        lineEdit->setText (this->sifre ().toStdString ());
        lineEdit->setEchoMode (EchoMode::Password);

        auto kaydetText = vContainer->addWidget (cpp14::make_unique<WText>("<h6>Kaydet</h6",TextFormat::UnsafeXHTML));
        kaydetText->addStyleClass (Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kaydetText->addStyleClass (Bootstrap::Label::Primary);
        kaydetText->decorationStyle ().setCursor (Cursor::PointingHand);

        kaydetText->clicked ().connect ([=](){

            if( lineEdit->text ().toUTF8 ().size () < 5 )
            {
                this->showPopUpMessage ("Şifreniz En Az 5 Haneli Olmak Zorunda");
            }else{
                if( this->setField(Personel().setOid (this->oid ().value ().to_string ()),Personel::KeyPassword,lineEdit->text ().toUTF8 ()) ){
                    this->setSifre ( lineEdit->text ().toUTF8 ().c_str ());
                    this->showPopUpMessage ("Şifreniz Değiştirildi","msg");
                }
            }


        });
    }



}

void PersonelPage::initSMSLog()
{


    mSMSManager->Content ()->clear ();

    {
        auto container = mSMSManager->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::ContextualBackGround::bg_info+"boxShadow");
        container->setWidth (WLength("100%"));
        container->addWidget (cpp14::make_unique<WText>("Gönderilen SMSler"));
        container->setMargin (5,Side::Top|Side::Bottom);
        container->setPositionScheme (PositionScheme::Relative);
    }
    for( auto item : mSMSManager->listSMS (this->telefon ()) )
    {
        auto container = mSMSManager->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setWidth (WLength("100%"));
        container->setMargin (25,Side::Top|Side::Bottom);
        container->setPositionScheme (PositionScheme::Relative);
        container->setHeight (78);
        container->addStyleClass ("boxShadow");


        {
            auto smsText = container->addWidget (cpp14::make_unique<WText>("<h6>"+item.smsText ().toStdString ()+"</h6>",TextFormat::UnsafeXHTML));
            smsText->setPositionScheme (PositionScheme::Absolute);
            smsText->setOffsets (0,Side::Left);
            smsText->addStyleClass (Bootstrap::Label::Default+"boxShadow");
            smsText->setOffsets (0,Side::Bottom);
        }


        {
            auto iletiRaporID = item.raporID ();
            std::string rapor_text = "";
            if( iletiRaporID != 6 )
            {
                rapor_text = item.rapor ().toStdString ();
            }else{
                rapor_text = "İleti Raporunu Kontrol Et";
            }
            auto smsText = container->addWidget (cpp14::make_unique<WText>("<h6>"+rapor_text+"</h6>"));

            smsText->setPositionScheme (PositionScheme::Absolute);
            smsText->setOffsets (0,Side::Top|Side::Right);

            if( iletiRaporID != 6 )
            {
                rapor_text = item.rapor ().toStdString ();
            }else{
                rapor_text = "İleti Raporunu Kontrol Et";
                smsText->decorationStyle ().setCursor (Cursor::PointingHand);
            }

            smsText->addStyleClass (Bootstrap::Label::Danger);
            smsText->setAttributeValue (Style::userdata1,item.idText ().toStdString ());
            smsText->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
            smsText->clicked ().connect ([=](){


                if( item.raporID () != 6 )
                {
                    this->showPopUpMessage ("Bilgi: "+ item.rapor ().toStdString (),"msg");
                    return;
                }

                this->showPopUpMessage ("Bekleyin","msg");
                SerikBLDCore::SMS::SMSItem smsItem;
                smsItem.setID (smsText->attributeValue (Style::userdata1).toUTF8 ().c_str ());
                smsItem.setOid (smsText->attributeValue (Style::dataoid).toUTF8 ().c_str ());

                if( this->mSMSManager->checkRapor (smsItem) )
                {
                    //                    this->showPopUpMessage ("SMS ")
                }
            });
        }




        {
            auto tarihText = container->addWidget (cpp14::make_unique<WText>("<h6>"+QDate::fromJulianDay (item.julianDay ()).toString ("dd/MM/yyyy dddd").toStdString ()+
                                                                             " - "+QTime::fromMSecsSinceStartOfDay (item.secStartOfDay ()).toString ("hh:mm").toStdString ()+"</h6>",TextFormat::UnsafeXHTML));
            tarihText->setPositionScheme (PositionScheme::Absolute);
            tarihText->setOffsets (0,Side::Top|Side::Left);
            tarihText->addStyleClass (Bootstrap::Label::Primary);
        }

    }


}

AltBirimManagerPage::AltBirimManagerPage(SerikBLDCore::DB *_db, const bsoncxx::oid &birimOid , const std::string &birimAdi)
    :SerikBLDCore::AltBirimManager (_db),
      mBirimOid(birimOid),
      mBirimAdi(birimAdi)
{

//    this->setMargin (25,Side::Top|Side::Bottom);
//    this->setPadding (25,Side::Top|Side::Bottom);

//    this->addStyleClass ("boxShadow");

    Header ()->addWidget (cpp14::make_unique<WText>("Alt Birim Yönetimi"));
    Header ()->addWidget (cpp14::make_unique<WBreak>());

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::container_fluid);

        auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);

        auto lineContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        lineContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_8+
                                      Bootstrap::Grid::Medium::col_md_8+
                                      Bootstrap::Grid::Small::col_sm_8+
                                      Bootstrap::Grid::ExtraSmall::col_xs_9);

        auto yeniAdLineEdit = lineContainer->addWidget (cpp14::make_unique<WLineEdit>());
        yeniAdLineEdit->setPlaceholderText ("Yeni Alt Birim Adını Giriniz");


        auto btnContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        btnContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                     Bootstrap::Grid::Medium::col_md_4+
                                     Bootstrap::Grid::Small::col_sm_4+
                                     Bootstrap::Grid::ExtraSmall::col_xs_3);


        auto saveBtn = btnContainer->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);

        saveBtn->clicked ().connect ([=](){
            SerikBLDCore::IK::AltBirimItem _item;
            _item.setName (yeniAdLineEdit->text ().toUTF8 ().c_str ());
            _item.setBirimOid (mBirimOid);
            if( this->InsertItem (_item).size () ){
                this->updateAltBirimList ();
                std::cout << "Inserted Birim Oid: " << bsoncxx::to_json (_item.view ()) << std::endl;

            }else{
                std::cout << "Birim Oid: " << bsoncxx::to_json (_item.view ()) << std::endl;
            }
        });
    }

    this->updateAltBirimList ();

    Content ()->setMargin ( 25 , Side::Top|Side::Bottom );


}

AltBirimManagerPage::AltBirimManagerPage(DB *_db, const IK::BirimItem &_birimItem)
    :SerikBLDCore::AltBirimManager (_db),
      mBirimOid(_birimItem.oid().value()),
      mBirimAdi(_birimItem.birimAdi().toStdString())
{
    mCurrentBirimItem.clear();
    mCurrentBirimItem.setDocumentView(_birimItem.view());
}

void AltBirimManagerPage::onList(const QVector<SerikBLDCore::IK::AltBirimItem> *mlist)
{


    Content ()->clear ();

    for( auto item : *mlist ){

        {
            auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                                      Bootstrap::Grid::Medium::col_md_11+
                                      Bootstrap::Grid::Small::col_sm_11+
                                      Bootstrap::Grid::ExtraSmall::col_xs_11);

            container->addWidget (cpp14::make_unique<WText>(item.name ().toStdString ()));
            container->setMargin (15,Side::Bottom);
            container->setAttributeValue (Style::style,Style::Border::bottom::border_width ("1px solid gray"));
        }

        {
            auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                      Bootstrap::Grid::Medium::col_md_1+
                                      Bootstrap::Grid::Small::col_sm_1+
                                      Bootstrap::Grid::ExtraSmall::col_xs_1);

            container->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::FireBrick)+Style::color::color (Style::color::White::Snow));
            container->setMargin (15,Side::Bottom);
            container->clicked ().connect ([=](){
                auto btn = askConfirm ("Silmek İstediğinize Emin misiniz?");
                btn->clicked ().connect ([=](){
                    SerikBLDCore::IK::AltBirimItem _item;
                    _item.setOid (item.oid ().value ().to_string ());
                    if( this->DeleteItem (_item) )
                    {
                        this->updateAltBirimList ();
                    }
                });
            });
        }

    }

}

void AltBirimManagerPage::updateAltBirimList()
{
    SerikBLDCore::IK::AltBirimItem __item;
    __item.setBirimOid (mBirimOid);
    this->UpdateList (__item);
}

BirimWidget::BirimWidget(const BirimItem &other)
{
    this->::SerikBLDCore::IK::BirimItem::clear();
    this->setDocumentView(other.view());

    auto birimAdiText = this->Content()->addNew<WText>("<b>"+this->birimAdi().toStdString()+"</b>");
    birimAdiText->addStyleClass(Bootstrap::Grid::col_full_12);

    auto birimSiraText = this->Content()->addNew<WText>("SıraNO: "+std::to_string(this->SiraNo()));
    birimSiraText->addStyleClass(Bootstrap::Grid::col_full_12);

    auto degisContainer = this->Content()->addNew<WContainerWidget>();
    degisContainer->addStyleClass(CSSStyle::Button::blueButton);
    degisContainer->addNew<WText>("Değiştir");
    degisContainer->setPadding(5,Side::Top|Side::Bottom);

    degisContainer->clicked().connect([=](){
        auto mDialog = this->createFlatDialog("Birim Bilgilerini Değiştir");

        auto text = mDialog->Content()->addNew<WText>();
        text->addStyleClass(Bootstrap::Grid::col_full_12);
        text->setText("Birim Adını Değiştir");

        auto birimAdiLineEdit = mDialog->Content()->addNew<WLineEdit>();
        birimAdiLineEdit->addStyleClass(Bootstrap::Grid::col_full_12);
        birimAdiLineEdit->setPlaceholderText("Birim Bilgilerini Değiştir");
        birimAdiLineEdit->setText(this->birimAdi().toStdString());

        auto textSiraNo = mDialog->Content()->addNew<WText>("Sıra NO");
        textSiraNo->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        textSiraNo->setPadding(6,Side::Top|Side::Bottom);

        auto siraNoContainer = mDialog->Content()->addNew<WContainerWidget>();
        siraNoContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto siraNoSpinBox = siraNoContainer->addNew<WSpinBox>();
        siraNoSpinBox->setMaximum(100);
        siraNoSpinBox->setMinimum(-1);
        siraNoSpinBox->setValue(this->SiraNo());

        mDialog->Content()->setPadding(25,Side::Top|Side::Bottom);

        mDialog->Accepted().connect([=](){
            _ChangedClicked.emit(this->oid().value().to_string(),
                                 birimAdiLineEdit->text().toUTF8(),
                                 siraNoSpinBox->value());
            this->removeDialog(mDialog);
        });

    });

}

Signal<std::string, std::string, int32_t> &BirimWidget::ChangedClicked()
{
    return _ChangedClicked;
}
