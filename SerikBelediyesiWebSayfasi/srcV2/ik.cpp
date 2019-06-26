#include "ik.h"

IK::IK(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db,_user,"İnsan Kaynakları")
{

    this->getHeaderRowContainer()->setMargin(15,Side::Top);

    mCurrentSkip = 0;

    {

        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        mBirimContainer = container->addWidget(cpp14::make_unique<BirimWidget>(this->db(),this->User()));
        mBirimContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(220,255),this->getRandom(220,255),this->getRandom(220,255)));
        mBirimContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        mBirimContainer->setWidth(WLength("100%"));
        mBirimContainer->BirimListChanged().connect(this,&IK::_loadBirimFilterList);
        mBirimContainer->BirimListChanged().connect(this,&IK::_loadPersonelWidgetBirimList);
    }

    {

        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
        mPersonelContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        mPersonelContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,255),this->getRandom(200,255),this->getRandom(200,255)));
        mPersonelContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        mPersonelContainer->setWidth(WLength("100%"));


//        auto mBirimContainer = container->addWidget(cpp14::make_unique<BirimWidget>(this->db(),this->User()));
//        mBirimContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(220,255),this->getRandom(220,255),this->getRandom(220,255)));
//        mBirimContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
//        mBirimContainer->setWidth(WLength("100%"));
    }



    {

        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);

        {

            mPersonelListFilterContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            mPersonelListFilterContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,255),this->getRandom(200,255),this->getRandom(200,255)));
            mPersonelListFilterContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            mPersonelListFilterContainer->setWidth(WLength("100%"));
            mPersonelListFilterContainer->addStyleClass(Bootstrap::Grid::row);
            mPersonelListFilterContainer->setMargin(10,Side::Bottom);

            auto container = this->mPersonelListFilterContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,220),this->getRandom(200,220),this->getRandom(200,220)));
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            container->setWidth(WLength("100%"));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

            mBirimFilterComboBox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center);


            this->_loadBirimFilterList();
//            try {
//                auto cursor = this->db()->collection("Müdürlükler").find(document{}.view());
//                mBirimFilterComboBox->addItem("Tümü");

//                for( auto doc : cursor )
//                {
//                    mBirimFilterComboBox->addItem(doc["Birim"].get_utf8().value.to_string());
//                }
//            } catch (mongocxx::exception &e) {

//            }

            mBuroPersonelFilterComboBox = layout->addWidget(cpp14::make_unique<WComboBox>());
            mBuroPersonelFilterComboBox->addItem("Tümü");
            mBuroPersonelFilterComboBox->addItem("Büro Personeli");
            mBuroPersonelFilterComboBox->addItem("Saha Personeli");


            auto mFilterBtn = layout->addWidget(cpp14::make_unique<WPushButton>("Listele"),0,AlignmentFlag::Center);
            mFilterBtn->addStyleClass(Bootstrap::Button::Primary);
            mFilterBtn->clicked().connect([=](){
                mCurrentSkip = 0;
                this->loadPersonelList();
            });


            mTotalPersonelCount = layout->addWidget(cpp14::make_unique<WText>("Total Personel"),0,AlignmentFlag::Center);
            mTotalPersonelCount->addStyleClass(Bootstrap::Label::Primary);



        }


        {
            mPersonelListContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            mPersonelListContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,255),this->getRandom(200,255),this->getRandom(200,255)));
            mPersonelListContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            mPersonelListContainer->setWidth(WLength("100%"));
            mPersonelListContainer->addStyleClass(Bootstrap::Grid::row);
        }


        {
            mPersonelListPreNextBtnContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            mPersonelListPreNextBtnContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,255),this->getRandom(200,255),this->getRandom(200,255)));
            mPersonelListPreNextBtnContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            mPersonelListPreNextBtnContainer->setWidth(WLength("100%"));
            mPersonelListPreNextBtnContainer->addStyleClass(Bootstrap::Grid::row);
            mPersonelListPreNextBtnContainer->setMargin(10,Side::Top);
            auto container = this->mPersonelListPreNextBtnContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,220),this->getRandom(200,220),this->getRandom(200,220)));
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            container->setWidth(WLength("100%"));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());

            auto mBackBtn = layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left);
            mBackBtn->addStyleClass(Bootstrap::Button::Primary);
            mBackBtn->clicked().connect([=](){
                mCurrentSkip -= mCurrentLimit;
                this->loadPersonelList();
            });

            mListIndexText = layout->addWidget(cpp14::make_unique<WText>(""),0,AlignmentFlag::Center);
            mListIndexText->addStyleClass(Bootstrap::Label::info);

            auto mNextBtn = layout->addWidget(cpp14::make_unique<WPushButton>("İleri"),0,AlignmentFlag::Right);
            mNextBtn->addStyleClass(Bootstrap::Button::Primary);
            mNextBtn->clicked().connect([=](){
                mCurrentSkip += mCurrentLimit;
                this->loadPersonelList();
            });


        }

    }


    this->LoadPersonelWidgets();

    this->loadPersonelList(document{},mCurrentSkip);


}

void IK::LoadPersonelWidgets()
{

    this->mPersonelContainer->clear();
    this->mPersonelContainer->setContentAlignment(AlignmentFlag::Center);
    mFotoContainer = mPersonelContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mFotoContainer->decorationStyle().setBorder(WBorder(BorderStyle::Solid,1,WColor(50,50,50,125)),AllSides);
    mFotoContainer->setHeight(WLength("100%"));
    mFotoContainer->setWidth(120);
    mFotoContainer->setHeight(160);
    mFotoContainer->setMinimumSize(WLength::Auto,160);


    auto fileUploaderContainer = this->mPersonelContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fileUploaderContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(150,170),this->getRandom(170,190),this->getRandom(200,220)));
    fileUploaderContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    fileUploaderContainer->setMargin(10,Side::Top|Side::Bottom);

    mFileUploader = fileUploaderContainer->addWidget(cpp14::make_unique<WFileUpload>());
    auto progresBar = fileUploaderContainer->addWidget(cpp14::make_unique<WProgressBar>());
    mFileUploader->setFileTextSize(6000); // Set the maximum file size to 2048 kB.
    mFileUploader->setMargin(10, Wt::Side::Right);
    mFileUploader->setFilters("image/*");
    mFileUploader->setMultiple(false);
    mFileUploader->setProgressBar(progresBar);

    mFileUploader->fileTooLarge().connect([=] {
        std::cout << mFileUploader->fileTextSize() << std::endl;
        this->showMessage("Hata","Dosya Boyutu Fazla Büyük. Max: 6MB Olmalı");
    });

    Wt::WPushButton *uploadButton = fileUploaderContainer->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>("Resmi Yükle"));
    uploadButton->setMargin(10, Wt::Side::Left | Wt::Side::Right);

    uploadButton->clicked().connect([=] {
        if( this->mPersonelOidText->text().toUTF8().size() == 24 )
        {
            mFileUploader->progressBar()->setValue(0);

            mFileUploader->upload();
            mFileUploader->enable();
            mFileUploader->setHidden(false);
        }else{
            this->showMessage("Uyarı","Personel Seçmediniz. Lütfen Personel Seçiniz");
        }
    });

    mFileUploader->uploaded().connect([=] {
        auto list = mFileUploader->uploadedFiles();

        for( auto item : list )
        {
            QString _fileName = QString("%1%2").arg(QUuid::createUuid().toString()).arg(item.clientFileName().c_str());
            rootnewFileName = QString("docroot/tempfile/")+_fileName;
            QString newFileName = QString("tempfile/")+ _fileName;
            if( QFile::copy(item.spoolFileName().c_str(),rootnewFileName) )
            {
                QImage img;
                bool _mSuccess = false;
                if( img.load(rootnewFileName) )
                {
                    img = img.scaled(120,160,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
                    if( img.save(rootnewFileName,"JPG",75) )
                    {
                        _mSuccess = true;
                    }else{
                        this->showMessage("Hata","Dosya Kayıt Edilemedi");
                    }
                }else{
                    this->showMessage("Hata","Dosya Açılamadı");
                }

                if( _mSuccess )
                {

                    auto filter = document{};

                    try {
                        filter.append(kvp(this->KEYOid,bsoncxx::oid{this->mPersonelOidText->text().toUTF8()}));
                    } catch (bsoncxx::exception &e) {
                        this->setFotoContainerWidget(ErrorImgPath);
                        this->mUpdateinformation->setText("Resim Yüklenemedi E: " + WString("{1}").arg(e.what()) );
                        return;
                    }

                    auto val = this->uploadfile(rootnewFileName);

                    auto setDoc = document{};

                    try {
                        setDoc.append(kvp("$set",make_document(kvp(this->KEYFotooid,val.get_oid().value))));
                    } catch (bsoncxx::exception &e) {
                        this->setFotoContainerWidget(ErrorImgPath);
                        this->mUpdateinformation->setText("Resim Yüklenemedi E: " + WString("{1}").arg(e.what()));
                        return;
                    }

                    //                    std::cout << bsoncxx::to_json(filter.view()) << std::endl;
                    //                    std::cout << bsoncxx::to_json(setDoc.view()) << std::endl;

                    try {

                        auto upt = this->db()->collection(Collection).update_one(filter.view(),setDoc.view());

                        if( upt )
                        {
                            if( upt.value().result().modified_count() )
                            {
                                this->setFotoContainerWidget(newFileName.toStdString());
                                this->mUpdateinformation->setText("Resim Başarılı Şekilde Yüklendi");
                            }else{
                                this->setFotoContainerWidget(ErrorImgPath);
                                this->mUpdateinformation->setText("Resim Yüklenemedi E: " + WString("{1}").arg(upt.value().result().modified_count()));
                            }
                        }else{
                            this->setFotoContainerWidget(ErrorImgPath);
                            this->mUpdateinformation->setText("Resim Yüklenemedi E: " + WString("{1}").arg("No PTR"));
                        }

                    } catch (mongocxx::exception &e) {
                        this->setFotoContainerWidget(ErrorImgPath);
                        this->mUpdateinformation->setText("Resim Yüklenemedi E: " + WString("{1}").arg(e.what()));
                    }

                }else{
                    this->setFotoContainerWidget(ErrorImgPath);
                    this->mUpdateinformation->setText("Resim Yüklenemedi");
                }

            }else{
                this->setFotoContainerWidget(ErrorImgPath);
                this->mUpdateinformation->setText("Resim Yüklenemedi");
            }
        }

    });



    auto container = this->mPersonelContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->setWidth(WLength("100%"));
    container->setContentAlignment(AlignmentFlag::Center);
    auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());


    mPersonelOidText = layout->addWidget(cpp14::make_unique<WText>(" "),0,AlignmentFlag::Center);

    mStatuComboBox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center);
    mStatuComboBox->addItem("Başkan");
    mStatuComboBox->addItem("Başkan Yardımcısı");
    mStatuComboBox->addItem("Müdür");
    mStatuComboBox->addItem("Personel");

    misimLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);
    misimLineEdit->setPlaceholderText("İsim Soyisim Giriniz");

    mtcnoLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);
    mtcnoLineEdit->setPlaceholderText("TCNO Giriniz");


    mtelnoLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);
    mtelnoLineEdit->setPlaceholderText("Cep Telefonu Giriniz. 0 Kullanarak");

    mPasswordLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);
    mPasswordLineEdit->setPlaceholderText("Bir Şifre Belirleyiniz");

    mBirimComboBox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center);

    mBuroPersonelComboBox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center);
    mBuroPersonelComboBox->addItem("Büro Personeli");
    mBuroPersonelComboBox->addItem("Saha Personeli");

    this->_loadPersonelWidgetBirimList();
//    try {
//        auto cursor = this->db()->collection("Müdürlükler").find(document{}.view());

//        for( auto doc : cursor )
//        {

//            try {
//                mBirimComboBox->addItem(doc["Birim"].get_utf8().value.to_string());
//            } catch (bsoncxx::exception &e) {

//            }
//        }

//    } catch (mongocxx::exception &e) {

//    }

    auto mSaveBtn = layout->addWidget(cpp14::make_unique<WPushButton>("Kaydet"));
    mSaveBtn->addStyleClass(Bootstrap::Button::Primary);
    mSaveBtn->clicked().connect(this,&IK::_savePersonel);

    auto mNewBtn = layout->addWidget(cpp14::make_unique<WPushButton>("Alanı Temizle"));
    mNewBtn->addStyleClass(Bootstrap::Button::info);
    mNewBtn->clicked().connect([=](){
        this->misimLineEdit->setText("");
        this->mtelnoLineEdit->setText("");
        this->mtcnoLineEdit->setText("");
        this->mPersonelOidText->setText("");
        this->mPasswordLineEdit->setText("");
    });








    mUpdateinformation = layout->addWidget(cpp14::make_unique<WText>("*"));
    mUpdateinformation->addStyleClass(Bootstrap::Label::info);



    auto mMovePersonel = layout->addWidget(cpp14::make_unique<WPushButton>("Bu Personeli Dondur"));
    mMovePersonel->addStyleClass(Bootstrap::Button::Warning);
    mMovePersonel->clicked().connect(this,&IK::_holdPersonel);
}

void IK::loadPersonel(const std::string &mtcno)
{


    auto filter = document{};


    this->_loadPersonel(filter);


}

void IK::loadPersonelList(bsoncxx::builder::basic::document filter, const int &skip)
{


    if( mBirimFilterComboBox->currentText().toUTF8() != "Tümü" )
    {
        try {
            filter.append(kvp("Birimi",mBirimFilterComboBox->currentText().toUTF8()));
        } catch (bsoncxx::exception &e) {

        }
    }

    if( mBuroPersonelFilterComboBox->currentText().toUTF8() != "Tümü" )
    {
        if( mBuroPersonelFilterComboBox->currentText().toUTF8() == u8"Büro Personeli" )
        {
            try {
                filter.append(kvp(this->KEYBuroPer,true));
            } catch (bsoncxx::exception &e) {

            }
        }else{
            try {
                filter.append(kvp(this->KEYBuroPer,false));
            } catch (bsoncxx::exception &e) {

            }
        }
    }

    int count = 0 ;

    try {
        count = this->db()->collection(Collection).count(filter.view());
    } catch (mongocxx::exception &e) {

    }


    mTotalPersonelCount->setText(WString("{1} Personel").arg(count));

    if( mCurrentSkip >= count )
    {
        mCurrentSkip = 0;
    }

    if( mCurrentSkip < 0 )
    {
        mCurrentSkip = count - mCurrentLimit;
    }

    mListIndexText->setText(WString("{1} - {2} / {3}").arg(mCurrentSkip).arg(mCurrentSkip+mCurrentLimit).arg(count));


    mongocxx::options::find findOptions;

    findOptions.skip( mCurrentSkip );
    findOptions.limit( mCurrentLimit );

    auto projectView = document{};

    try {
        projectView.append(kvp(KEYadsoyad,true));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << __FILE__ << __FUNCTION__ << std::endl;
    }
    try {
        projectView.append(kvp(KEYOid,true));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << __FILE__ << __FUNCTION__ << std::endl;
    }

    try {
        projectView.append(kvp(KEYBirimi,true));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << __FILE__ << __FUNCTION__ << std::endl;
    }


    try {
        auto cursor = this->db()->collection(Collection).find(filter.view(),findOptions);

        this->mPersonelListContainer->clear();
        for( auto doc : cursor )
        {
            auto container = this->mPersonelListContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,20),this->getRandom(0,20),this->getRandom(0,20)));
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            container->setWidth(WLength("100%"));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

            try {
                auto mOid = doc[KEYOid].get_oid().value.to_string();
                container->setAttributeValue(Style::dataoid,mOid);
            } catch (bsoncxx::exception &e) {
                std::cout << __LINE__ << __FILE__ << __FUNCTION__ << std::endl;
                this->mPersonelListContainer->removeWidget(container);
                return;
            }

            try {
                auto adsoyad = doc[KEYadsoyad].get_utf8().value.to_string();
                auto text = layout->addWidget(cpp14::make_unique<WText>(adsoyad),0,AlignmentFlag::Center);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
            } catch (bsoncxx::exception &e) {
                std::cout << __LINE__ << __FILE__ << __FUNCTION__ << std::endl;
                this->mPersonelListContainer->removeWidget(container);
                return;
            }

            //            try {
            //                auto adsoyad = doc[KEYBirimi].get_utf8().value.to_string();
            //                auto text = layout->addWidget(cpp14::make_unique<WText>(adsoyad),0,AlignmentFlag::Center);
            //                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
            //            } catch (bsoncxx::exception &e) {
            //                std::cout << __LINE__ << __FILE__ << __FUNCTION__ << std::endl;
            //                this->mPersonelListContainer->removeWidget(container);
            //                return;
            //            }

            container->clicked().connect([=](){
                auto filter = document{};

                try {
                    filter.append(kvp(this->KEYOid,bsoncxx::oid{container->attributeValue(Style::dataoid).toUTF8()}));
                } catch (bsoncxx::exception &e) {
                    std::cout << "ERROR: " << __LINE__ << __FILE__ << __FUNCTION__ << std::endl;
                    return;
                }

                this->_loadPersonel(filter);
            });

        }

    } catch (mongocxx::exception &e) {

    }


}

void IK::_loadPersonel(const bsoncxx::builder::basic::document &filter)
{
    bsoncxx::document::value val(document{}.view());

    try {
        auto _val = this->db()->collection(this->Collection).find_one(filter.view());
        if( _val )
        {
            val = _val.value();
        }else{
            return;
        }

    } catch (mongocxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << std::endl;
    }

    try {
        auto filePath = this->downloadFile(val.view()[this->KEYFotooid].get_oid().value.to_string());
        this->setFotoContainerWidget(filePath);

    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << std::endl;
    }




    try {
        mPersonelOidText->setText(val.view()[this->KEYOid].get_oid().value.to_string());
    } catch (bsoncxx::exception &e) {
        mPersonelOidText->setText(e.what());
    }


    try {
        auto statustr = val.view()[this->KEYStatu].get_utf8().value.to_string();
        for( int i = 0 ; i < mStatuComboBox->count() ; i++ )
        {
            if( mStatuComboBox->itemText(i).toUTF8() == statustr )
            {
                mStatuComboBox->setCurrentIndex(i);
                break;
            }
        }
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << std::endl;
    }

    try {
        auto statustr = val.view()[this->KEYBirimi].get_utf8().value.to_string();
        for( int i = 0 ; i < mBirimComboBox->count() ; i++ )
        {
            if( mBirimComboBox->itemText(i).toUTF8() == statustr )
            {
                mBirimComboBox->setCurrentIndex(i);
                break;
            }
        }
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << std::endl;
    }


    try {
        if( val.view()[this->KEYBuroPer].get_bool().value )
        {
            this->mBuroPersonelComboBox->setCurrentIndex(0);
        }else{
            this->mBuroPersonelComboBox->setCurrentIndex(1);
        }
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << std::endl;
    }



    try {
        misimLineEdit->setText(val.view()[this->KEYadsoyad].get_utf8().value.to_string());
    } catch (bsoncxx::exception &e) {
        misimLineEdit->setText(e.what());
    }


    try {
        mtcnoLineEdit->setText(val.view()[this->KEYTcno].get_utf8().value.to_string());
    } catch (bsoncxx::exception &e) {
        mtcnoLineEdit->setText(e.what());
    }

    try {
        mtelnoLineEdit->setText(val.view()[this->KEYTel].get_utf8().value.to_string());
    } catch (bsoncxx::exception &e) {
        mtelnoLineEdit->setText(e.what());
    }

    try {
        mPasswordLineEdit->setText(val.view()[this->KEYPasword].get_utf8().value.to_string());
    } catch (bsoncxx::exception &e) {
        mPasswordLineEdit->setText(e.what());
    }

}

void IK::_savePersonel()
{


    if( mPasswordLineEdit->text().toUTF8().size() < 5 )
    {
        this->showMessage("UYARI","Şifre En Az 5 Haneli Olması Gerekir");
        return;
    }

    if( mtcnoLineEdit->text().toUTF8().size() != 11 )
    {
        this->showMessage("UYARI","TCNO Hanesi Hatalı");
        return;
    }

    if( mtelnoLineEdit->text().toUTF8().size() != 11 )
    {
        this->showMessage("UYARI","Cep Telefonu Hanesi Hatalı");
        return;
    }

    if( misimLineEdit->text().toUTF8().size() < 6 )
    {
        this->showMessage("UYARI","İsim Soyisim Yetersiz");
        return;
    }



    auto doc = document{};

    try {
        doc.append(kvp(this->KEYadsoyad,this->misimLineEdit->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        doc.append(kvp(this->KEYBirimi,this->mBirimComboBox->currentText().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        doc.append(kvp(this->KEYStatu,this->mStatuComboBox->currentText().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        if( mBuroPersonelComboBox->currentIndex() == 0 )
        {
            doc.append(kvp(this->KEYBuroPer,true));
        }else{
            doc.append(kvp(this->KEYBuroPer,false));
        }
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        doc.append(kvp(this->KEYTel,this->mtelnoLineEdit->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        doc.append(kvp(this->KEYTcno,this->mtcnoLineEdit->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        doc.append(kvp(this->KEYPasword,this->mPasswordLineEdit->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }



    if( this->mPersonelOidText->text().toUTF8().size() == 24 )
    {
        auto filter = document{};

        try {
            filter.append(kvp(this->KEYOid,bsoncxx::oid{this->mPersonelOidText->text().toUTF8()}));
        } catch (bsoncxx::exception &e) {
            mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
            std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
            return;
        }

        auto setDoc = document{};

        try {
            setDoc.append(kvp("$set",doc));
        } catch (bsoncxx::exception &e) {
            mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
            std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
            return;
        }


        try {
            auto upt = this->db()->collection(Collection).update_one(filter.view(),setDoc.view());
            if( upt )
            {
                if( upt.value().result().modified_count() )
                {
                    mUpdateinformation->setText("Kayıt Başarılı");
                    this->_loadPersonel(filter);
                }else{
                    mUpdateinformation->setText("Kayıt Başarısız");
                    std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << "NO DOCUMENT UPDATED" <<  std::endl;
                }
            }
        } catch (mongocxx::exception &e) {
            mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
            std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        }
    }else{
        try {
            auto ins = this->db()->collection(Collection).insert_one(doc.view());
            if( ins )
            {
                if( ins.value().result().inserted_count() )
                {
                    mUpdateinformation->setText("Kayıt Başarılı");

                    auto filter = document{};

                    try {
                        filter.append(kvp(this->KEYOid,ins.value().inserted_id().get_oid()));
                    } catch (bsoncxx::exception &e) {
                        mUpdateinformation->setText("Yeni Kayıt Yüklenemedi");
                        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " NO DOCUMENT UPDATED" <<  std::endl;
                        return;
                    }

                    this->_loadPersonel(filter);
                    mUpdateinformation->setText("Yeni Kayıt Yüklendi");
                }else{
                    mUpdateinformation->setText("Kayıt Başarısız");
                    std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " NO DOCUMENT UPDATED" <<  std::endl;
                }
            }
        } catch (mongocxx::exception &e) {
            mUpdateinformation->setText("Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
            std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        }
    }


}

void IK::_holdPersonel()
{
    auto filter = document{};
    try {
        filter.append(kvp(this->KEYOid,bsoncxx::oid{this->mPersonelOidText->text().toUTF8()}));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText("Taşıma Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    bsoncxx::document::value val(document{}.view());

    try {
        auto _val = this->db()->collection(Collection).find_one(filter.view());

        if( _val )
        {
            val = _val.value();
        }else{
            mUpdateinformation->setText("Taşıma Başarısız");
            return;
        }

    } catch (mongocxx::exception &e) {
        mUpdateinformation->setText("Taşıma Başarısız E: "+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        auto del = this->db()->collection(Collection).delete_one(filter.view());
        if( del )
        {
            if( del.value().deleted_count() )
            {
                //                    mUpdateinformation->setText(u8"Taşıma Başarılı");
            }else{
                mUpdateinformation->setText("Taşıma Başarısız");
                return;
            }
        }
    } catch (mongocxx::exception &e) {
        mUpdateinformation->setText("Taşıma Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }


    try {
        auto ins = this->db()->collection(CollectionHold).insert_one(val.view());
        if( ins )
        {
            if( ins.value().result().inserted_count() )
            {
                mUpdateinformation->setText("Taşıma Başarılı");
                this->misimLineEdit->setText("");
                this->mtelnoLineEdit->setText("");
                this->mtcnoLineEdit->setText("");
                this->mPersonelOidText->setText("");
                this->mPasswordLineEdit->setText("");
                this->setFotoContainerWidget(ErrorImgPath);

            }else{
                mUpdateinformation->setText("Taşıma Başarısız");
                return;
            }
        }
    } catch (mongocxx::exception &e) {
        mUpdateinformation->setText("Taşıma Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

}

void IK::_loadBirimFilterList()
{
    mBirimFilterComboBox->clear();
    try {
        auto cursor = this->db()->collection("Müdürlükler").find(document{}.view());
        mBirimFilterComboBox->addItem("Tümü");

        for( auto doc : cursor )
        {
            mBirimFilterComboBox->addItem(doc["Birim"].get_utf8().value.to_string());
        }
    } catch (mongocxx::exception &e) {

    }
}

void IK::_loadPersonelWidgetBirimList()
{
    mBirimComboBox->clear();
    try {
        auto cursor = this->db()->collection("Müdürlükler").find(document{}.view());

        for( auto doc : cursor )
        {

            try {
                mBirimComboBox->addItem(doc["Birim"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {

            }
        }

    } catch (mongocxx::exception &e) {

    }
}

void IK::setFotoContainerWidget(const std::string &fotoPath)
{
    this->mFotoContainer->setAttributeValue(Style::style,Style::background::url(fotoPath)+
                                            Style::background::repeat::norepeat+
                                            Style::background::size::contain+
                                            Style::background::position::center_center);
    this->mFotoContainer->setHeight(160);
    this->mFotoContainer->setWidth(120);
    this->mFotoContainer->setMinimumSize(120,160);
}

BirimWidget::BirimWidget(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db,_user)
{

    setMargin(20,Side::Top|Side::Bottom);

    {
        auto text = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WText>("Birimler"));
        text->addStyleClass(Bootstrap::Label::Primary);
    }

    this->LoadBirim();


    {
        auto container = this->getFooterRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(75,125),this->getRandom(75,125),this->getRandom(95,155)));
        container->setMargin(15,Side::Top|Side::Bottom);
        auto rContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        auto yeniBirimAdiTitleContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        yeniBirimAdiTitleContainer->addStyleClass(Bootstrap::Grid::col_full_12);
        auto _yeniBirimGirContainerTitle = yeniBirimAdiTitleContainer->addWidget(cpp14::make_unique<WText>("Yeni Birim Gir"));
        _yeniBirimGirContainerTitle->addStyleClass(Bootstrap::Label::Primary);
        _yeniBirimGirContainerTitle->setAttributeValue(Style::style,Style::font::size::s12px);

        auto yeniBirimAdiContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        yeniBirimAdiContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_9+Bootstrap::Grid::Medium::col_md_9+Bootstrap::Grid::Small::col_sm_9+Bootstrap::Grid::ExtraSmall::col_xs_9);
        auto lineEdit = yeniBirimAdiContainer->addWidget(cpp14::make_unique<WLineEdit>());
        lineEdit->setPlaceholderText("Yeni Birim Adını Giriniz");


        auto yeniBirimAdiSaveContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        yeniBirimAdiSaveContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        auto mSaveBtn = yeniBirimAdiSaveContainer->addWidget(cpp14::make_unique<WPushButton>("Kaydet"));
        mSaveBtn->addStyleClass(Bootstrap::Button::Primary);

        mSaveBtn->clicked().connect([=](){

            auto insDoc = document{};

            try {
                insDoc.append(kvp(KEYBirim,lineEdit->text().toUTF8()));
            } catch (bsoncxx::exception &e) {

            }

            try {
                auto ins = this->db()->collection(Collection).insert_one(insDoc.view());
                if( ins )
                {
                    if( ins.value().result().inserted_count() )
                    {
                        _BirimListChanged.emit(NoClass());
                        this->LoadBirim();
                    }
                }
            } catch (mongocxx::exception &e) {

            }

        });
    }


}

Signal<NoClass> &BirimWidget::BirimListChanged()
{
    return _BirimListChanged;
}

void BirimWidget::LoadBirim()
{

    mongocxx::pipeline pipeLine;

    auto totalGroup = document{};

    try {
        totalGroup.append(kvp("_id","$"+KEYBirim+"i"));
    } catch (bsoncxx::exception &e) {

    }

    try {
        totalGroup.append(kvp("total",make_document(kvp("$sum",1))));
    } catch (bsoncxx::exception &e) {

    }
    pipeLine.group(totalGroup.view());



    std::vector<std::string> birimList;

    this->getContentRowContainer()->clear();
    auto container_ = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    container_->addStyleClass(Bootstrap::Grid::row);

    try {
        auto cursor = this->db()->collection(CollectionPersonel).aggregate(pipeLine);

        for( auto doc : cursor )
        {

            auto container = container_->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(150,170),this->getRandom(170,190),this->getRandom(200,220)));

            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());

            std::string _birim;
            std::string countStr{""};

            try {
                _birim = doc["_id"].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {

            }

            birimList.push_back(_birim);

            QString sBirim = QString::fromStdString(_birim);

            if( sBirim.count() > 20 )
            {
                sBirim = sBirim.mid(0,17);
                sBirim.append("...");
                _birim = sBirim.toStdString();
            }

            try {
                countStr = Bootstrap::Badges::badget(_birim,Bootstrap::Badges::Color::info,std::to_string(doc["total"].get_int32().value));
            } catch (bsoncxx::exception &e) {

            }

            try {
                auto birim = layout->addWidget(cpp14::make_unique<WText>(countStr),0,AlignmentFlag::Left);
            } catch (bsoncxx::exception &e) {

            }



            auto str = Bootstrap::Badges::badget("X");
            auto birim = layout->addWidget(cpp14::make_unique<WText>(str),0,AlignmentFlag::Right);
            birim->setAttributeValue(Style::style,Style::background::color::rgb("255,255,255"));
            birim->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            birim->decorationStyle().setCursor(Cursor::PointingHand);

            birim->clicked().connect([=](){
                auto msg = this->deleteBirim(_birim);
                this->showMessage("Bilgi",msg);
                if( msg == "Birim Silindi" )
                {
                    this->LoadBirim();
                }

            });


        }

    } catch (mongocxx::exception &e) {

    }




    auto arlist = array{};

    for( auto str : birimList )
    {
        try {
            arlist.append(str);
        } catch (bsoncxx::exception &e) {

        }
    }

    auto filter = document{};

    try {
        filter.append(kvp(KEYBirim,make_document(kvp("$nin",arlist))));
    } catch (bsoncxx::exception &e) {

    }

    try {
        auto cursor = this->db()->collection(Collection).find(filter.view());



        for( auto doc : cursor )
        {
            auto container = container_->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(150,170),this->getRandom(170,190),this->getRandom(200,220)));

            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());

            std::string _birim;
            std::string countStr{""};

            try {
                _birim = doc[this->KEYBirim].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {

            }

            birimList.push_back(_birim);

            QString sBirim = QString::fromStdString(_birim);

            if( sBirim.count() > 20 )
            {
                sBirim = sBirim.mid(0,17);
                sBirim.append("...");
                _birim = sBirim.toStdString();
            }

            try {
                countStr = Bootstrap::Badges::badget(_birim,Bootstrap::Badges::Color::info,std::to_string(0));
            } catch (bsoncxx::exception &e) {

            }

            try {
                auto birim = layout->addWidget(cpp14::make_unique<WText>(countStr),0,AlignmentFlag::Left);
            } catch (bsoncxx::exception &e) {

            }



            auto str = Bootstrap::Badges::badget("X");
            auto birim = layout->addWidget(cpp14::make_unique<WText>(str),0,AlignmentFlag::Right);
            birim->setAttributeValue(Style::style,Style::background::color::rgb("255,255,255"));
            birim->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            birim->decorationStyle().setCursor(Cursor::PointingHand);

            birim->clicked().connect([=](){
                auto msg = this->deleteBirim(_birim);
                this->showMessage("Bilgi",msg);
                if( msg == "Birim Silindi" )
                {
                    this->LoadBirim();
                }

            });
        }

    } catch (mongocxx::exception &e) {

    }




}

std::string BirimWidget::deleteBirim(const std::string &birimadi)
{


    auto filter = document{};

    try {
        filter.append(kvp("Birimi",birimadi));
    } catch (bsoncxx::exception &e) {
        return std::to_string(__LINE__) + " - " + __FILE__  + e.what();
    }


    auto count = this->count("Personel",filter.extract());

    if( count )
    {
        return "Bu Birime Ait Atanmış Personeller Bulunmakta. Personellerin Birimlerini Değiştirip Silebilirsiniz";
    }else {
        try {
            auto del = this->db()->collection(Collection).delete_one(make_document(kvp(KEYBirim,birimadi)));
            if( del )
            {
                if( del.value().deleted_count() )
                {
                    _BirimListChanged.emit(NoClass());
                    return "Birim Silindi";
                }else{
                    return "Birim Silinmedi";
                }
            }else{
                return "Bir Hata Oluştu";
            }
        } catch (mongocxx::exception &e) {
            return std::to_string(__LINE__) + " - " + __FILE__  + e.what();
        }
    }
}
