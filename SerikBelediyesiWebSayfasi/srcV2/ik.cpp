#include "ik.h"

IK::IK(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db,_user,"İnsan Kaynakları")
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
                auto cursor = this->db()->collection("Müdürlükler").find(document{}.view());
                mBirimFilterComboBox->addItem("Tümü");

                for( auto doc : cursor )
                {
                    mBirimFilterComboBox->addItem(doc["Birim"].get_utf8().value.to_string());
                }
            } catch (mongocxx::exception &e) {
            }

            mBuroPersonelComboBox = layout->addWidget(cpp14::make_unique<WComboBox>());
            mBuroPersonelComboBox->addItem("Tümü");
            mBuroPersonelComboBox->addItem("Büro Personeli");
            mBuroPersonelComboBox->addItem("Saha Personeli");


            auto mFilterBtn = layout->addWidget(cpp14::make_unique<WPushButton>("Listele"),0,AlignmentFlag::Center);
            mFilterBtn->addStyleClass(Bootstrap::Button::Primary);
            mFilterBtn->clicked().connect([=](){
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

            auto mNextBtn = layout->addWidget(cpp14::make_unique<WPushButton>("İleri"),0,AlignmentFlag::Right);
            mNextBtn->addStyleClass(Bootstrap::Button::Primary);
            mNextBtn->clicked().connect([=](){
                mCurrentSkip += mCurrentLimit;
                this->loadPersonelList();
            });


        }

    }


    {
        this->mPersonelContainer->clear();
        auto container = this->mPersonelContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setWidth(WLength("100%"));
        container->setContentAlignment(AlignmentFlag::Center);
        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto fotoContainer = layout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Center);
        fotoContainer->decorationStyle().setBorder(WBorder(BorderStyle::Solid,1,WColor(50,50,50,125)),AllSides);
        fotoContainer->setHeight(WLength("100%"));
        fotoContainer->setWidth(120);
        fotoContainer->setHeight(160);

        mPersonelOidText = layout->addWidget(cpp14::make_unique<WText>(" "),0,AlignmentFlag::Center);

        mStatuComboBox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center);
        mStatuComboBox->addItem("Başkan");
        mStatuComboBox->addItem("Başkan Yardımcısı");
        mStatuComboBox->addItem("Müdür");
        mStatuComboBox->addItem("Personel");

        misimLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);

        mtcnoLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);

        mtelnoLineEdit = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Center);

        mBirimComboBox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center);

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

    this->loadPersonelList(document{},mCurrentSkip);


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

    if( mBuroPersonelComboBox->currentText().toUTF8() != "Tümü" )
    {
        if( mBuroPersonelComboBox->currentText().toUTF8() == "Büro Personeli" )
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




//    mMahalleComboBox = layout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Center);


}
