#include "ik.h"

IK::IK(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db,_user,u8"İnsan Kaynakları")
{

    this->getHeaderRowContainer()->setMargin(15,Side::Top);

    mCurrentSkip = 0;

    {

        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
        mPersonelContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        mPersonelContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,255),this->getRandom(200,255),this->getRandom(200,255)));
        mPersonelContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        mPersonelContainer->setWidth(WLength("100%"));

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

            try {
                auto cursor = this->db()->collection(u8"Müdürlükler").find(document{}.view());
                mBirimFilterComboBox->addItem(u8"Tümü");

                for( auto doc : cursor )
                {
                    mBirimFilterComboBox->addItem(doc["Birim"].get_utf8().value.to_string());
                }
            } catch (mongocxx::exception &e) {
            }

            mBuroPersonelFilterComboBox = layout->addWidget(cpp14::make_unique<WComboBox>());
            mBuroPersonelFilterComboBox->addItem(u8"Tümü");
            mBuroPersonelFilterComboBox->addItem(u8"Büro Personeli");
            mBuroPersonelFilterComboBox->addItem(u8"Saha Personeli");


            auto mFilterBtn = layout->addWidget(cpp14::make_unique<WPushButton>(u8"Listele"),0,AlignmentFlag::Center);
            mFilterBtn->addStyleClass(Bootstrap::Button::Primary);
            mFilterBtn->clicked().connect([=](){
                mCurrentSkip = 0;
                this->loadPersonelList();
            });


            mTotalPersonelCount = layout->addWidget(cpp14::make_unique<WText>(u8"Total Personel"),0,AlignmentFlag::Center);
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

            mListIndexText = layout->addWidget(cpp14::make_unique<WText>(u8""),0,AlignmentFlag::Center);
            mListIndexText->addStyleClass(Bootstrap::Label::info);

            auto mNextBtn = layout->addWidget(cpp14::make_unique<WPushButton>(u8"İleri"),0,AlignmentFlag::Right);
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
    mFotoContainer->setMinimumSize(WLength::Auto,190);

    auto container = this->mPersonelContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->setWidth(WLength("100%"));
    container->setContentAlignment(AlignmentFlag::Center);
    auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());


    mPersonelOidText = layout->addWidget(cpp14::make_unique<WText>(" "),0,AlignmentFlag::Center);

    mStatuComboBox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center);
    mStatuComboBox->addItem(u8"Başkan");
    mStatuComboBox->addItem(u8"Başkan Yardımcısı");
    mStatuComboBox->addItem(u8"Müdür");
    mStatuComboBox->addItem(u8"Personel");

    misimLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);
    misimLineEdit->setPlaceholderText(u8"İsim Soyisim Giriniz");

    mtcnoLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);
    mtcnoLineEdit->setPlaceholderText(u8"TCNO Giriniz");


    mtelnoLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);
    mtelnoLineEdit->setPlaceholderText(u8"Cep Telefonu Giriniz. 0 Kullanarak");

    mPasswordLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);
    mPasswordLineEdit->setPlaceholderText(u8"Bir Şifre Belirleyiniz");

    mBirimComboBox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center);

    mBuroPersonelComboBox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center);
    mBuroPersonelComboBox->addItem(u8"Büro Personeli");
    mBuroPersonelComboBox->addItem(u8"Saha Personeli");

    try {
        auto cursor = this->db()->collection(u8"Müdürlükler").find(document{}.view());

        for( auto doc : cursor )
        {

            try {
                mBirimComboBox->addItem(doc["Birim"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {

            }
        }

    } catch (mongocxx::exception &e) {

    }

    auto mSaveBtn = layout->addWidget(cpp14::make_unique<WPushButton>("Kaydet"));
    mSaveBtn->addStyleClass(Bootstrap::Button::Primary);
    mSaveBtn->clicked().connect(this,&IK::_savePersonel);

    auto mNewBtn = layout->addWidget(cpp14::make_unique<WPushButton>(u8"Alanı Temizle"));
    mNewBtn->addStyleClass(Bootstrap::Button::info);
    mNewBtn->clicked().connect([=](){
        this->misimLineEdit->setText("");
        this->mtelnoLineEdit->setText("");
        this->mtcnoLineEdit->setText("");
        this->mPersonelOidText->setText("");
        this->mPasswordLineEdit->setText("");
    });








     mUpdateinformation = layout->addWidget(cpp14::make_unique<WText>(u8"*"));
     mUpdateinformation->addStyleClass(Bootstrap::Label::info);



     auto mMovePersonel = layout->addWidget(cpp14::make_unique<WPushButton>(u8"Bu Personeli Dondur"));
     mMovePersonel->addStyleClass(Bootstrap::Button::Warning);
     mMovePersonel->clicked().connect([=](){
         auto filter = document{};
         try {
             filter.append(kvp(this->KEYOid,bsoncxx::oid{this->mPersonelOidText->text().toUTF8()}));
         } catch (bsoncxx::exception &e) {
             mUpdateinformation->setText(u8"Taşıma Başarısız E:"+WString::fromUTF8(e.what()));
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
                 mUpdateinformation->setText(u8"Taşıma Başarısız");
                 return;
             }

         } catch (mongocxx::exception &e) {
             mUpdateinformation->setText(u8"Taşıma Başarısız E:"+WString::fromUTF8(e.what()));
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
                     mUpdateinformation->setText(u8"Taşıma Başarısız");
                     return;
                 }
             }
         } catch (mongocxx::exception &e) {
             mUpdateinformation->setText(u8"Taşıma Başarısız E:"+WString::fromUTF8(e.what()));
             std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
             return;
         }


         try {
             auto ins = this->db()->collection(CollectionHold).insert_one(val.view());
             if( ins )
             {
                 if( ins.value().result().inserted_count() )
                 {
                     mUpdateinformation->setText(u8"Taşıma Başarılı");
                     this->misimLineEdit->setText("");
                     this->mtelnoLineEdit->setText("");
                     this->mtcnoLineEdit->setText("");
                     this->mPersonelOidText->setText("");
                     this->mPasswordLineEdit->setText("");
                     this->mFotoContainer->setAttributeValue(Style::style,Style::background::url(ErrorImgPath)+
                                                             Style::background::repeat::norepeat+
                                                             Style::background::size::contain+
                                                             Style::background::position::center_center);
                     this->mFotoContainer->setHeight(160);
                     this->mFotoContainer->setWidth(120);
                     this->mFotoContainer->setMinimumSize(120,160);

                 }else{
                     mUpdateinformation->setText(u8"Taşıma Başarısız");
                     return;
                 }
             }
         } catch (mongocxx::exception &e) {
             mUpdateinformation->setText(u8"Taşıma Başarısız E:"+WString::fromUTF8(e.what()));
             std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
             return;
         }

     });
}

void IK::loadPersonel(const std::string &mtcno)
{


    auto filter = document{};


    this->_loadPersonel(filter);


}

void IK::loadPersonelList(bsoncxx::builder::basic::document filter, const int &skip)
{


    if( mBirimFilterComboBox->currentText().toUTF8() != u8"Tümü" )
    {
        try {
            filter.append(kvp("Birimi",mBirimFilterComboBox->currentText().toUTF8()));
        } catch (bsoncxx::exception &e) {

        }
    }

    if( mBuroPersonelFilterComboBox->currentText().toUTF8() != u8"Tümü" )
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

    mListIndexText->setText(WString(u8"{1} - {2} / {3}").arg(mCurrentSkip).arg(mCurrentSkip+mCurrentLimit).arg(count));


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
        this->mFotoContainer->setAttributeValue(Style::style,Style::background::url(filePath)+
                                                Style::background::repeat::norepeat+
                                                Style::background::size::contain+
                                                Style::background::position::center_center);
        this->mFotoContainer->setHeight(160);
        this->mFotoContainer->setWidth(120);
        this->mFotoContainer->setMinimumSize(120,160);

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
        this->showMessage(u8"UYARI",u8"Şifre En Az 5 Haneli Olması Gerekir");
        return;
    }

    if( mtcnoLineEdit->text().toUTF8().size() != 11 )
    {
        this->showMessage(u8"UYARI",u8"TCNO Hanesi Hatalı");
        return;
    }

    if( mtelnoLineEdit->text().toUTF8().size() != 11 )
    {
        this->showMessage(u8"UYARI",u8"Cep Telefonu Hanesi Hatalı");
        return;
    }

    if( misimLineEdit->text().toUTF8().size() < 6 )
    {
        this->showMessage(u8"UYARI",u8"İsim Soyisim Yetersiz");
        return;
    }



    auto doc = document{};

    try {
        doc.append(kvp(this->KEYadsoyad,this->misimLineEdit->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        doc.append(kvp(this->KEYBirimi,this->mBirimComboBox->currentText().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        doc.append(kvp(this->KEYStatu,this->mStatuComboBox->currentText().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
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
        mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        doc.append(kvp(this->KEYTel,this->mtelnoLineEdit->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

    try {
        doc.append(kvp(this->KEYTcno,this->mtcnoLineEdit->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }

//    try {
//        doc.append(kvp(this->KEYPasword,this->mtcnoLineEdit->text().toUTF8()));
//    } catch (bsoncxx::exception &E) {

//    }

    try {
        doc.append(kvp(this->KEYPasword,this->mPasswordLineEdit->text().toUTF8()));
    } catch (bsoncxx::exception &e) {
        mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return;
    }



    if( this->mPersonelOidText->text().toUTF8().size() == 24 )
    {
        auto filter = document{};

        try {
            filter.append(kvp(this->KEYOid,bsoncxx::oid{this->mPersonelOidText->text().toUTF8()}));
        } catch (bsoncxx::exception &e) {
            mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
            std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
            return;
        }

        auto setDoc = document{};

        try {
            setDoc.append(kvp("$set",doc));
        } catch (bsoncxx::exception &e) {
            mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
            std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
            return;
        }


        try {
            auto upt = this->db()->collection(Collection).update_one(filter.view(),setDoc.view());
            if( upt )
            {
                if( upt.value().result().modified_count() )
                {
                    mUpdateinformation->setText(u8"Kayıt Başarılı");
                    this->_loadPersonel(filter);
                }else{
                    mUpdateinformation->setText(u8"Kayıt Başarısız");
                    std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " NO DOCUMENT UPDATED" <<  std::endl;
                }
            }
        } catch (mongocxx::exception &e) {
            mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
            std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        }
    }else{
        try {
            auto ins = this->db()->collection(Collection).insert_one(doc.view());
            if( ins )
            {
                if( ins.value().result().inserted_count() )
                {
                    mUpdateinformation->setText(u8"Kayıt Başarılı");

                    auto filter = document{};

                    try {
                        filter.append(kvp(this->KEYOid,ins.value().inserted_id().get_oid()));
                    } catch (bsoncxx::exception &e) {
                        mUpdateinformation->setText(u8"Yeni Kayıt Yüklenemedi");
                        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " NO DOCUMENT UPDATED" <<  std::endl;
                        return;
                    }

                    this->_loadPersonel(filter);
                    mUpdateinformation->setText(u8"Yeni Kayıt Yüklendi");
                }else{
                    mUpdateinformation->setText(u8"Kayıt Başarısız");
                    std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " NO DOCUMENT UPDATED" <<  std::endl;
                }
            }
        } catch (mongocxx::exception &e) {
            mUpdateinformation->setText(u8"Kayıt Başarısız E:"+WString::fromUTF8(e.what()));
            std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        }
    }


}
