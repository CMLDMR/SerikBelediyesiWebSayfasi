#include "kadinailestock.h"

KadinAileStock::KadinAileStock(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget(_db,_user,"Stok Yardım")
{

    mYardim = new StokYardim(this->db(),this->User());

    this->getHeaderRowContainer()->setMargin(15,Side::Top|Side::Bottom);

    {
        auto container = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Yardım Girişi"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect(this,&KadinAileStock::initStockGiris);

    }

    {
        auto container = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Yardım Çıkışı"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect(this,&KadinAileStock::initStockCikis);
    }

    {
        auto container = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>("TC Kayıt"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect(this,&KadinAileStock::initTCKayit);
    }

    {
        auto container = this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Ayarlar"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect(this,&KadinAileStock::initAyarlar);
    }
}

KadinAileStock::~KadinAileStock()
{
    delete mYardim;
}

void KadinAileStock::initAyarlar()
{
    this->getContentRowContainer()->clear();
    this->getContentRowContainer()->addWidget( cpp14::make_unique<StokKategori>( mYardim , this->db() , this->User() ) )->addStyleClass(Bootstrap::Grid::col_full_12);
}

void KadinAileStock::initTCKayit()
{
    this->getContentRowContainer()->clear();
    this->getContentRowContainer()->addWidget( cpp14::make_unique<TCKayit>( this->db() , this->User() ) )->addStyleClass(Bootstrap::Grid::col_full_12);
}

void KadinAileStock::initStockGiris()
{
    this->getContentRowContainer()->clear();
    this->getContentRowContainer()->addWidget( cpp14::make_unique<StockGiris>( this->db() , this->User() ) )->addStyleClass(Bootstrap::Grid::col_full_12);
}

void KadinAileStock::initStockCikis()
{
    this->getContentRowContainer()->clear();
    this->getContentRowContainer()->addWidget( cpp14::make_unique<StockCikis>( this->db() , this->User() ) )->addStyleClass(Bootstrap::Grid::col_full_12);
}



StokYardim::StokYardim(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::BaseWidget (_db,_user)
{

}

StokYardim::~StokYardim()
{

}

bool StokYardim::addKategoriName(const std::string &kategoriName)
{

    bool returnValue = false;

    auto insDoc = document{};

    try {
        insDoc.append(kvp(mKategoriKEY,kategoriName));
    } catch (bsoncxx::exception &e) {
        returnValue = false;
    }

    std::int64_t count = 0;

    try {
        count = this->db()->collection(mKategoriColl).count(insDoc.view());
    } catch (mongocxx::exception &e) {
        returnValue = false;
    }


    if( count == 0 )
    {
        try {
            auto ins = this->db()->collection(mKategoriColl).insert_one(insDoc.view());

            if( ins.value().result().inserted_count() )
            {
                returnValue = true;
            }else{
                returnValue = false;
            }
        } catch (mongocxx::exception &e) {
            returnValue = false;
        }
    }else{
        returnValue = false;
    }

    return returnValue;
}

bool StokYardim::deleteKategoriName(const std::string &kategoriOid)
{
    bool returnValue = true;

    auto delDoc = document{};

    try {
        delDoc.append(kvp(mOidKEY,bsoncxx::oid{kategoriOid}));
    } catch (bsoncxx::exception &e) {
        returnValue = false;
    }

    std::int64_t count = 0;

    try {
        count = this->db()->collection(mKategoriColl).count(delDoc.view());
    } catch (mongocxx::exception &e) {
        returnValue = false;
    }


    auto mKategoriName = std::string{""};
    try {
        auto val = this->db()->collection(mKategoriColl).find_one(delDoc.view());
        if( val )
        {

            auto view =val.value().view();

            try {
                mKategoriName = view[mMalzemeAdiKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                returnValue = false;
            }


        }
    } catch (mongocxx::exception &e) {
        returnValue = false;
    }

    try {
        auto _count = this->db()->collection(mMalzemeColl).count(make_document(kvp(mMalzemeAdiKEY,mKategoriName)).view());
        if( _count != 0 )
        {
            returnValue = false;
        }
    } catch (mongocxx::exception &e) {
        returnValue = false;
    }



    if( count != 0 && returnValue )
    {
        try {
            auto ins = this->db()->collection(mKategoriColl).delete_one(delDoc.view());

            if( ins.value().result().deleted_count() )
            {
                returnValue = true;
            }else{
                returnValue = false;
            }
        } catch (mongocxx::exception &e) {
            returnValue = false;
        }
    }else{
        returnValue = false;
    }

    return returnValue;
}

std::vector<StokYardim::StokKategoriDoc> StokYardim::getKategoriList()
{

    std::vector<StokKategoriDoc> list;

    try {

        auto cursor = this->db()->collection(this->mKategoriColl).find(document{}.view());

        for( auto doc : cursor )
        {

            StokKategoriDoc _doc;

            try {
                _doc.mKategoriOid = doc[mOidKEY].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {

            }

            try {
                _doc.mKategoriName = doc[mKategoriKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {

            }

            list.push_back(_doc);
        }

    } catch (mongocxx::exception &e) {

    }

    return list;
}

bool StokYardim::addMalzeme(const std::string &kategoriadi, const std::string &malzemeadi, const std::string &tcno)
{
    bool returnedValue = true;

    auto insDoc = document{};

    try {
        insDoc.append(kvp(this->mKategoriKEY,kategoriadi));
    } catch (bsoncxx::exception &e) {
        returnedValue = false;
    }

    try {
        insDoc.append(kvp(this->mStokta,true));
    } catch (bsoncxx::exception &e) {
        returnedValue = false;
    }

    try {
        insDoc.append(kvp(this->mVerenTCNOKEY,tcno));
    } catch (bsoncxx::exception &e) {
        returnedValue = false;
    }

    try {
        insDoc.append(kvp(this->mVerilisTarihiKEY,QDate::currentDate().toJulianDay()));
    } catch (bsoncxx::exception &e) {
        returnedValue = false;
    }

    try {
        insDoc.append(kvp(this->mMalzemeAdiKEY,malzemeadi));
    } catch (bsoncxx::exception &e) {
        returnedValue = false;
    }


    if( returnedValue )
    {
        try {

            auto ins = this->db()->collection(this->mMalzemeColl).insert_one(insDoc.view());

            if( ins )
            {
                if( ins.value().result().inserted_count() )
                {
                    returnedValue = true;
                }else{
                    returnedValue = false;
                }
            }else{
                returnedValue = false;
            }

        } catch (mongocxx::exception &e) {
            returnedValue = false;
        }
    }



    return returnedValue;
}

bool StokYardim::deleteMalzeme(const std::string &malzemeOid)
{
    auto filter = document{};
    try {
        filter.append(kvp("_id",bsoncxx::oid{malzemeOid}));
    } catch (bsoncxx::exception &e) {
        std::cout << "Error: " << malzemeOid << " " << __LINE__ << __FILE__ << std::endl;
        return false;
    }

    bool deleteIT = true;
    try {
        auto val = this->db()->collection(this->mMalzemeColl).find_one(filter.view());

        if( val )
        {
            if( !val.value().view()[this->mStokta].get_bool().value )
            {
                deleteIT = false;
            }
        }else{
            deleteIT = false;
        }

    } catch (mongocxx::exception &e) {
        return false;
    }



    if( deleteIT )
    {
        try {
            auto del = this->db()->collection(this->mMalzemeColl).delete_one(filter.view());
            if( del )
            {
                if( del.value().deleted_count() ){
                    return true;
                }else{
                    return false;
                }
            }else{
                return false;
            }
        } catch (mongocxx::exception &e) {
            return false;
        }
    }else{
        return false;
    }

}

std::vector<StokYardim::StokMalzeme> StokYardim::getMalzemeList(const std::string &tcno)
{

    std::vector<StokMalzeme> list;
    auto filter = document{};

    try {
        filter.append(kvp(this->mVerenTCNOKEY,tcno));
    } catch (bsoncxx::exception &e) {

    }

    try {

        auto cursor = this->db()->collection(this->mMalzemeColl).find(filter.view());

        for( auto doc : cursor )
        {
            StokMalzeme malzeme;
            try {
                malzeme.mMalzemeOid = doc["_id"].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;
            }
            try {
                malzeme.mKategori = doc[this->mKategoriKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;
            }
            try {
                malzeme.mMalzemeAdi = doc[this->mMalzemeAdiKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mVerenTCNO = doc[this->mVerenTCNOKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mVerilisTarihi = doc[this->mVerilisTarihiKEY].get_int64().value;
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mAlanTCNO = doc[this->mAlanTCNOKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }

            try {
                malzeme.mAlanisTarihi = doc[this->mAlinisTarihiKEY].get_int64().value;
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mStokta = doc[this->mStokta].get_bool().value;
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }

            list.push_back(malzeme);
        }

    } catch (mongocxx::exception &e) {

    }

    return list;
}

std::vector<StokYardim::StokMalzeme> StokYardim::getMalzemeAlanList(const std::string &alanTCNO)
{
    std::vector<StokMalzeme> list;
    auto filter = document{};

    try {
        filter.append(kvp(this->mAlanTCNOKEY,alanTCNO));
    } catch (bsoncxx::exception &e) {

    }

    try {

        auto cursor = this->db()->collection(this->mMalzemeColl).find(filter.view());

        for( auto doc : cursor )
        {
            StokMalzeme malzeme;
            try {
                malzeme.mMalzemeOid = doc["_id"].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;
            }
            try {
                malzeme.mKategori = doc[this->mKategoriKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;
            }
            try {
                malzeme.mMalzemeAdi = doc[this->mMalzemeAdiKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mVerenTCNO = doc[this->mVerenTCNOKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mVerilisTarihi = doc[this->mVerilisTarihiKEY].get_int64().value;
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mAlanTCNO = doc[this->mAlanTCNOKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }

            try {
                malzeme.mAlanisTarihi = doc[this->mAlinisTarihiKEY].get_int64().value;
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mStokta = doc[this->mStokta].get_bool().value;
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }

            list.push_back(malzeme);
        }

    } catch (mongocxx::exception &e) {

    }

    return list;
}

std::vector<StokYardim::StokMalzeme> StokYardim::getStoktaList( const std::string &mkategori )
{
    std::vector<StokMalzeme> list;
    auto filter = document{};

    try {
        filter.append(kvp(this->mStokta,true));
    } catch (bsoncxx::exception &e) {

    }
    if( mkategori.size() > 0 )
    {
        try {
            filter.append(kvp(this->mKategoriKEY,mkategori));
        } catch (bsoncxx::exception &e) {

        }
    }


    try {

        auto cursor = this->db()->collection(this->mMalzemeColl).find(filter.view());

        for( auto doc : cursor )
        {
            StokMalzeme malzeme;
            try {
                malzeme.mMalzemeOid = doc["_id"].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;
            }
            try {
                malzeme.mKategori = doc[this->mKategoriKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;
            }
            try {
                malzeme.mMalzemeAdi = doc[this->mMalzemeAdiKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mVerenTCNO = doc[this->mVerenTCNOKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mVerilisTarihi = doc[this->mVerilisTarihiKEY].get_int64().value;
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mAlanTCNO = doc[this->mAlanTCNOKEY].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }

            try {
                malzeme.mAlanisTarihi = doc[this->mAlinisTarihiKEY].get_int64().value;
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }
            try {
                malzeme.mStokta = doc[this->mStokta].get_bool().value;
            } catch (bsoncxx::exception &e) {
                std::cout << "ERROR: " << __LINE__ << __FILE__ << std::endl;

            }

            list.push_back(malzeme);
        }

    } catch (mongocxx::exception &e) {

    }

    return list;
}

bool StokYardim::setMalzemeTeslim(const std::string &aliciTCNO, const int64_t &alisTarihno, const std::string &mOid)
{
    auto filter = document{};

    try {
        filter.append(kvp("_id",bsoncxx::oid{mOid}));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << __FILE__ << std::endl;
        return false;
    }


    auto doc = document{};

    try {
        doc.append(kvp(this->mAlanTCNOKEY,aliciTCNO));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << __FILE__ << std::endl;
        return false;
    }

    try {
        doc.append(kvp(this->mAlinisTarihiKEY,bsoncxx::types::b_int64{alisTarihno}));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << __FILE__ << std::endl;
        return false;
    }
    try {
        doc.append(kvp(this->mStokta,false));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << __FILE__ << std::endl;
        return false;
    }

    auto setDoc = document{};

    try {
        setDoc.append(kvp("$set",doc));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << __FILE__ << std::endl;
        return false;
    }

    std::cout << "bsoncxx:: " << bsoncxx::to_json(setDoc.view()) << std::endl;


    try {
        auto upt = this->db()->collection(mMalzemeColl).update_one(filter.view(),setDoc.view());
        if( upt )
        {
            std::cout << "Update: " << upt.value().result().modified_count() << " - " << upt.value().modified_count() << std::endl;
            if( upt.value().result().modified_count() )
            {
                return true;
            }else{
                std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << __FILE__ << std::endl;
                return false;
            }
        }else{
            std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << __FILE__ << std::endl;
            return false;
        }
    } catch (mongocxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << __FILE__ << std::endl;
        return false;
    }
}

std::string StokYardim::getVerenTCNOKEY() const
{
    return mVerenTCNOKEY;
}




StokKategori::StokKategori(StokYardim *_mStok, mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db,_user),mStok(_mStok)
{
    {
        mKategorilerContainer = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        mKategorilerContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    }
    this->initKategoriler();
    {
        mKategorilerOptions = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        mKategorilerOptions->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    }
    this->initKategoriOptions();
}

void StokKategori::initKategoriler()
{
    mKategorilerContainer->clear();
    auto rContainer = mKategorilerContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    auto list = this->mStok->getKategoriList();

    for (auto doc : list ) {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        auto _rContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _rContainer->addStyleClass(Bootstrap::Grid::row);
        {
            auto katNameContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            katNameContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_10+Bootstrap::Grid::Medium::col_md_10+Bootstrap::Grid::Small::col_sm_10+Bootstrap::Grid::ExtraSmall::col_xs_9);
            katNameContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            auto text = katNameContainer->addWidget(cpp14::make_unique<WText>(doc.mKategoriName));
        }
        {
            auto katNameContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            katNameContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_3);
            katNameContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            katNameContainer->setAttributeValue(Style::style,Style::background::color::rgba(200,150,150));
            katNameContainer->decorationStyle().setCursor(Cursor::PointingHand);
            auto text = katNameContainer->addWidget(cpp14::make_unique<WText>("X"));
            text->setAttributeValue(Style::dataoid,doc.mKategoriOid);
            katNameContainer->clicked().connect([=](){
                if( this->mStok->deleteKategoriName(text->attributeValue(Style::dataoid).toUTF8()) ){
                    this->initKategoriler();
                }
            });
        }
    }

}

void StokKategori::initKategoriOptions()
{

    auto rContainer = mKategorilerOptions->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        inputLine = container->addWidget(cpp14::make_unique<WLineEdit>());
        inputLine->setPlaceholderText("Kategori Adını Giriniz");
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        auto addBtn = container->addWidget(cpp14::make_unique<WPushButton>("Kaydet"));
        addBtn->clicked().connect([=](){
            this->addKategoriName(inputLine->text().toUTF8());
        });
    }

}

void StokKategori::addKategoriName(const std::string &mKategoriName)
{
    if( mStok->addKategoriName(mKategoriName) )
    {
        this->showMessage("Bilgi","Kategori Eklendi","Tamam");
        this->initKategoriler();
    }else{
        this->showMessage("Uyarı","Bu Kategori Eklenemedi","Tamam");
    }
}





TCKayit::TCKayit(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget ( _db , _user )
{

    mTC = new TC ( _db , _user );

    this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WText>("TCNO KAYIT"));
    this->getHeaderRowContainer()->setMargin(15,Side::Bottom);

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        mtcinput = container->addWidget(cpp14::make_unique<WLineEdit>());
        mtcinput->setPlaceholderText("TCNO Giriniz");
    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        misiminput = container->addWidget(cpp14::make_unique<WLineEdit>());
        misiminput->setPlaceholderText("İsim Soyisim Giriniz");
    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        mtelinput = container->addWidget(cpp14::make_unique<WLineEdit>());
        mtelinput->setPlaceholderText("Telefon Numarası");
    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        maddressinput = container->addWidget(cpp14::make_unique<WLineEdit>());
        maddressinput->setPlaceholderText("Adres Giriniz");
    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        mMahalle = container->addWidget(cpp14::make_unique<WComboBox>());

        try {
            auto cursor = this->db()->collection("Mahalleler").find(document{}.view());

            for( auto doc : cursor )
            {
                try {
                    mMahalle->addItem(doc["Mahalle"].get_utf8().value.to_string());
                } catch (bsoncxx::exception &e) {

                }
            }

        } catch (mongocxx::exception &e) {

        }

    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        auto saveBtn = container->addWidget(cpp14::make_unique<WPushButton>("Kaydet"));
        saveBtn->clicked().connect(this,&TCKayit::Save);
    }

}

void TCKayit::Save()
{

    mTC->setTel(mtelinput->text().toUTF8());
    mTC->setTCNO(mtcinput->text().toUTF8());
    mTC->setAddress(maddressinput->text().toUTF8());
    mTC->setIsimSoyisim(misiminput->text().toUTF8());
    mTC->setMahalle(mMahalle->currentText().toUTF8());

    if( mTC->SaveDB() ){
        this->showMessage("Bilgi","Kayıt Başarılı");
    }else{
        this->showMessage("Uyarı","Kayıt Edilemedi!");
    }
}

TC::TC(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::BaseWidget (_db,_user)
{
    mValid = false;
}

std::string TC::tCNO() const
{
    return mTCNO;
}

void TC::setTCNO(const std::string &tCNO)
{
    mTCNO = tCNO;
}

std::string TC::isimSoyisim() const
{
    return mIsimSoyisim;
}

void TC::setIsimSoyisim(const std::string &isimSoyisim)
{
    mIsimSoyisim = isimSoyisim;
}

std::string TC::tel() const
{
    return mTel;
}

void TC::setTel(const std::string &tel)
{
    mTel = tel;
}

std::string TC::address() const
{
    return mAddress;
}

void TC::setAddress(const std::string &address)
{
    mAddress = address;
}

bool TC::LoadTC(std::string mTCno)
{

    bool returnedValue = false;
    auto filter = document{};

    try {
        filter.append(kvp("TCNO",mTCno));
    } catch (bsoncxx::exception &e) {
        returnedValue = false;
        return false;
    }

    try {

        auto val = this->db()->collection("TC").find_one(filter.view());


        if( val )
        {

            returnedValue = true;
            mValid = true;

            try {
                this->setIsimSoyisim(val.value().view()["İsimSoyisim"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }


            try {
                this->setTel(val.value().view()["Cep Telefonu"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }


            try {
                this->setTCNO(val.value().view()["TCNO"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }


            try {
                this->setAddress(val.value().view()["Tam Adres"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }


            try {
                this->setMahalle(val.value().view()["Mahalle"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }


            try {
                mOid = val.value().view()["_id"].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }


            if( !returnedValue )
            {
                this->setIsimSoyisim("");
                this->setTel("");
                this->setTCNO("");
                this->setAddress("");
                this->setMahalle("");
                mValid = false;
            }
        }

    } catch (mongocxx::exception &e) {
        returnedValue = false;
    }


    return returnedValue;
}

bool TC::LoadTel(std::string mTelNo)
{
    bool returnedValue = false;
    auto filter = document{};

    try {
        filter.append(kvp("Cep Telefonu",mTelNo));
    } catch (bsoncxx::exception &e) {

    }


    try {

        auto val = this->db()->collection("TC").find_one(filter.view());

        if( val )
        {
            returnedValue = true;
            mValid = false;

            try {
                this->setIsimSoyisim(val.value().view()["İsimSoyisim"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }

            try {
                this->setTel(val.value().view()["Cep Telefonu"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }

            try {
                this->setTCNO(val.value().view()["TCNO"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }

            try {
                this->setAddress(val.value().view()["Tam Adres"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }

            try {
                mOid = (val.value().view()["_id"].get_oid().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }

            if( !returnedValue )
            {
                this->setIsimSoyisim("");
                this->setTel("");
                this->setTCNO("");
                this->setAddress("");
                mValid = false;
            }
        }

    } catch (mongocxx::exception &e) {

    }


    return returnedValue;
}

bool TC::LoadOid(bsoncxx::oid oid)
{
    bool returnedValue = false;
    auto filter = document{};

    try {
        filter.append(kvp("_id",oid));
    } catch (bsoncxx::exception &e) {

    }


    try {

        auto val = this->db()->collection("TC").find_one(filter.view());

        if( val )
        {
            returnedValue = true;
            mValid = false;

            try {
                this->setIsimSoyisim(val.value().view()["İsimSoyisim"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }

            try {
                this->setTel(val.value().view()["Cep Telefonu"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }

            try {
                this->setTCNO(val.value().view()["TCNO"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }

            try {
                this->setAddress(val.value().view()["Tam Adres"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }

            try {
                mOid = (val.value().view()["_id"].get_oid().value.to_string());
            } catch (bsoncxx::exception &e) {
                returnedValue = false;
            }

            if( !returnedValue )
            {
                this->setIsimSoyisim("");
                this->setTel("");
                this->setTCNO("");
                this->setAddress("");
                mValid = false;
            }
        }

    } catch (mongocxx::exception &e) {

    }


    return returnedValue;
}

bool TC::SaveDB()
{
    auto filter = document{};

    try {
        filter.append(kvp("TCNO",this->tCNO()));
    } catch (bsoncxx::exception &e) {

    }

    if( this->count("TC",filter.extract() ) )
    {
        return false;
    }

    filter.clear();

    try {
        filter.append(kvp("Cep Telefonu",this->tel()));
    } catch (bsoncxx::exception &e) {

    }

    if( this->count("TC",filter.extract() ) )
    {
        return false;
    }


    auto insDoc = document{};

    try {
        insDoc.append(kvp("TCNO",this->tCNO()));
    } catch (bsoncxx::exception &e) {
        return false;
    }

    try {
        insDoc.append(kvp("Cep Telefonu",this->tel()));
    } catch (bsoncxx::exception &e) {
        return false;
    }

    try {
        insDoc.append(kvp("Mahalle",this->mahalle()));
    } catch (bsoncxx::exception &e) {
        return false;
    }

    try {
        insDoc.append(kvp("İsimSoyisim",this->isimSoyisim()));
    } catch (bsoncxx::exception &e) {
        return false;
    }

    try {
        insDoc.append(kvp("Tam Adres",this->address()));
    } catch (bsoncxx::exception &e) {
        return false;
    }

    try {

        auto ins = this->db()->collection("TC").insert_one(insDoc.view());

        if( ins.value().result().inserted_count() )
        {
            return true;
        }else{
            return false;
        }

    } catch (mongocxx::exception &e) {
        return false;
    }


    return false;
}

bool TC::isValid() const
{
    return mValid;
}

std::string TC::mahalle() const
{
    return mMahalle;
}

void TC::setMahalle(const std::string &mahalle)
{
    mMahalle = mahalle;
}

std::string TC::oid() const
{
    return mOid;
}

void TC::setClear()
{
    this->setIsimSoyisim("");
    this->setTel("");
    this->setTCNO("");
    this->setAddress("");
    this->setMahalle("");
    this->mValid = false;
}

StockGiris::StockGiris(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db,_user)
{
    this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WText>("Stock Girişi"));

    mTC = new TC(_db,_user);

    mStok = new StokYardim(_db,_user);


    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        mTCNOLineEdit = container->addWidget(cpp14::make_unique<WLineEdit>());
        mTCNOLineEdit->setPlaceholderText("TCNO Giriniz");
    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        mTCWidget = container->addWidget(cpp14::make_unique<TCWidget>(this->db(),this->User()));
    }

    mTCNOLineEdit->textInput().connect([=](){
        if( mTCNOLineEdit->text().toUTF8().size() == 11 )
        {
            this->mTCWidget->LoadTCNO( mTCNOLineEdit->text().toUTF8() );
        }else{
            this->mTCWidget->getContentRowContainer()->clear();
            mTCWidget->setClear();
        }
    });


    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto mLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = mLayout->addWidget(cpp14::make_unique<WText>("Kategori Seç"),0,AlignmentFlag::Center);
        mKategoriComboBox = mLayout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Justify);
        auto list = mStok->getKategoriList();
        for( auto item : list )
        {
            mKategoriComboBox->addItem(item.mKategoriName);
        }
    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto mLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = mLayout->addWidget(cpp14::make_unique<WText>("Extra Bilgi"),0,AlignmentFlag::Center);
        mExtraBilgiLineEdit = mLayout->addWidget(cpp14::make_unique<WLineEdit>(),0,AlignmentFlag::Justify);
        mExtraBilgiLineEdit->setPlaceholderText("Extra Bilgi Giriniz");
    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        auto mSaveBtn = container->addWidget(cpp14::make_unique<WPushButton>("Kaydet"));
        mSaveBtn->addStyleClass(Bootstrap::Button::Primary);
        mSaveBtn->clicked().connect([=](){
            if( mTCWidget->getTCNO().size() == 11 )
            {
                if( this->mStok->addMalzeme(mKategoriComboBox->currentText().toUTF8(),
                                        mExtraBilgiLineEdit->text().toUTF8(),
                                        mTCWidget->getTCNO()) )
                {

                    this->ListCurrentTCNOList( mTCWidget->getTCNO() );
                }
            }else{
                this->showMessage("Uyarı","Kişi Bilgileri Eksik!\nLütfen TCNO Sorgulayınız");
                mTCWidget->setClear();
            }
        });
    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(5,Side::Top|Side::Bottom);
        auto mListBtn = container->addWidget(cpp14::make_unique<WPushButton>("Listele"));
        mListBtn->addStyleClass(Bootstrap::Button::Success);
        mListBtn->clicked().connect([=](){
            this->ListCurrentTCNOList(mTCWidget->getTCNO());
        });
    }

}

void StockGiris::ListCurrentTCNOList(const std::string &tcno)
{

    this->getFooterRowContainer()->clear();


    {
        auto container_ = this->getFooterRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container_->addStyleClass(Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
        container_->setAttributeValue(Style::style,Style::background::color::rgb(225,235,245));
        container_->setMargin(5,Side::Top);
        auto rContainer = container_->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Kategori"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Extra Bilgi"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Veriliş Tarih"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Alan TCNO"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Alınış Tarihi"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_5);
            auto text = container->addWidget(cpp14::make_unique<WText>("Durum"));
        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_1);
            auto text = container->addWidget(cpp14::make_unique<WText>("Sil"));
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
            container->setAttributeValue(Style::style,Style::background::color::rgb(220,100,100));
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        }

    }

    auto list = mStok->getMalzemeList(tcno);

    for( auto item : list )
    {
        auto container_ = this->getFooterRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container_->addStyleClass(Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
        container_->setAttributeValue(Style::style,Style::background::color::rgb(175,185,195));
        container_->setMargin(5,Side::Top);
        auto rContainer = container_->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        container_->setAttributeValue(Style::dataoid,item.mMalzemeOid);


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>(item.mKategori));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>(item.mMalzemeAdi));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(item.mVerilisTarihi).toString("dd/MM/yyyy")));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            if( !item.mStokta )
            {
                auto text = container->addWidget(cpp14::make_unique<WText>(item.mAlanTCNO));
            }
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            if( !item.mStokta )
            {
                  auto text = container->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(item.mAlanisTarihi).toString("dd/MM/yyyy")));
            }
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_5);
            if( item.mStokta )
            {
                auto text = container->addWidget(cpp14::make_unique<WText>("Stokta"));

            }else{
                auto text = container->addWidget(cpp14::make_unique<WText>("Verilmiş"));
                container_->setAttributeValue(Style::style,Style::background::color::rgb(165,225,185));
            }
        }

        if( item.mStokta ){
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_1);
            auto text = container->addWidget(cpp14::make_unique<WText>("X"));
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
            container->setAttributeValue(Style::style,Style::background::color::rgb(200,50,50));
            if( !item.mStokta )
            {
                  container->setDisabled(true);
            }
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            container->decorationStyle().setCursor(Cursor::PointingHand);

            container->clicked().connect([=](){
                if( this->mStok->deleteMalzeme(container_->attributeValue(Style::dataoid).toUTF8()) )
                {
                    this->ListCurrentTCNOList(tcno);
                }
            });
        }

    }



}

TCWidget::TCWidget(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db,_user),mTC(new TC(_db,_user))
{
    setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(150,255),this->getRandom(150,255),this->getRandom(150,255)));
    addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    setMargin(15,Side::Top|Side::Bottom);
}

TCWidget::~TCWidget()
{
    delete mTC;
}

void TCWidget::LoadTCNO(const std::string &mtcno)
{
    this->getContentRowContainer()->clear();
    if( mTC->LoadTC(mtcno) ){

        {
            auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::col_full_12);
            auto text = container->addWidget(cpp14::make_unique<WText>(this->mTC->isimSoyisim()));
        }
        {
            auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::col_full_12);
            auto text = container->addWidget(cpp14::make_unique<WText>(this->mTC->tel()));
        }
        {
            auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::col_full_12);
            auto text = container->addWidget(cpp14::make_unique<WText>(this->mTC->tCNO()));
        }
        {
            auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::col_full_12);
            auto text = container->addWidget(cpp14::make_unique<WText>(this->mTC->mahalle()));
        }
        {
            auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::col_full_12);
            auto text = container->addWidget(cpp14::make_unique<WText>(this->mTC->address()));
        }

    }else{
        this->getContentRowContainer()->clear();
    }

}

void TCWidget::setClear()
{
    this->mTC->setClear();
}

std::string TCWidget::getOid() const
{
    return mTC->oid();
}

std::string TCWidget::getTCNO() const
{
    return mTC->tCNO();
}

StockCikis::StockCikis( mongocxx::database *_db , bsoncxx::document::value _user )
    :BaseClass::ContainerWidget ( _db , _user )
{

    this->getHeaderRowContainer()->addWidget(cpp14::make_unique<WText>("Stok Çıkış"));

    mTC = new TC(_db,_user);

    mStok = new StokYardim(_db,_user);


    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        mTCNOLineEdit = container->addWidget(cpp14::make_unique<WLineEdit>());
        mTCNOLineEdit->setPlaceholderText("TCNO Giriniz");
    }


    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        mTCWidget = container->addWidget(cpp14::make_unique<TCWidget>(this->db(),this->User()));
    }


    mTCNOLineEdit->textInput().connect([=](){
        if( mTCNOLineEdit->text().toUTF8().size() == 11 )
        {
            this->mTCWidget->LoadTCNO( mTCNOLineEdit->text().toUTF8() );
            this->ListCurrentTCNOList( this->mTCWidget->getTCNO() );
        }else{
            this->mTCWidget->getContentRowContainer()->clear();
            mTCWidget->setClear();
        }
    });



    {
        mTeslimEdilenWidget = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        mTeslimEdilenWidget->addStyleClass(Bootstrap::Grid::col_full_12);
        mTeslimEdilenWidget->setMargin(5,Side::Top|Side::Bottom);
    }




    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
        container->setAttributeValue(Style::style,Style::background::color::rgb(225,235,205));
        auto mLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = mLayout->addWidget(cpp14::make_unique<WText>("Kategori Seç"),0,AlignmentFlag::Center);
        mKategoriComboBox = mLayout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Justify);
        auto list = mStok->getKategoriList();
        for( auto item : list )
        {
            mKategoriComboBox->addItem(item.mKategoriName);
        }
    }




    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(5,Side::Top|Side::Bottom);
        auto mListBtn = container->addWidget(cpp14::make_unique<WPushButton>("Listele"));
        mListBtn->addStyleClass(Bootstrap::Button::Success);
        mListBtn->clicked().connect([=](){
            this->ListStokList();
        });
    }



    {
        mMalzemeListWidget = this->getFooterRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        mMalzemeListWidget->addStyleClass(Bootstrap::Grid::col_full_12);
        mMalzemeListWidget->setMargin(5,Side::Top|Side::Bottom);
    }

}

void StockCikis::ListCurrentTCNOList(const std::string &tcno)
{

    mTeslimEdilenWidget->clear();

    {
        auto container = mTeslimEdilenWidget->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setContentAlignment(AlignmentFlag::Center|AlignmentFlag::Middle);
        auto text = container->addWidget(cpp14::make_unique<WText>("Daha Önce Kişiye Teslim Edilenler"));

    }

    {
        auto container_ = mTeslimEdilenWidget->addWidget(cpp14::make_unique<WContainerWidget>());
        container_->addStyleClass(Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
        container_->setAttributeValue(Style::style,Style::background::color::rgb(225,235,245));
        container_->setMargin(5,Side::Top);
        auto rContainer = container_->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Kategori"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Extra Bilgi"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Veriliş Tarih"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Alan TCNO"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Alınış Tarihi"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Durum"));
        }

//        {
//            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_1);
//            auto text = container->addWidget(cpp14::make_unique<WText>("Sil"));
//            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
//            container->setAttributeValue(Style::style,Style::background::color::rgb(220,100,100));
//            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
//        }

    }

    auto list = mStok->getMalzemeAlanList(tcno);

    for( auto item : list )
    {
        auto container_ = mTeslimEdilenWidget->addWidget(cpp14::make_unique<WContainerWidget>());
        container_->addStyleClass(Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
        container_->setAttributeValue(Style::style,Style::background::color::rgb(175,185,195));
        container_->setMargin(5,Side::Top);
        auto rContainer = container_->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        container_->setAttributeValue(Style::dataoid,item.mMalzemeOid);


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>(item.mKategori));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>(item.mMalzemeAdi));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(item.mVerilisTarihi).toString("dd/MM/yyyy")));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            if( !item.mStokta )
            {
                auto text = container->addWidget(cpp14::make_unique<WText>(item.mAlanTCNO));
            }
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            if( !item.mStokta )
            {
                  auto text = container->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(item.mAlanisTarihi).toString("dd/MM/yyyy")));
            }
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            if( item.mStokta )
            {
                auto text = container->addWidget(cpp14::make_unique<WText>("Stokta"));
            }else{
                auto text = container->addWidget(cpp14::make_unique<WText>("Verilmiş"));
            }
        }

//        {
//            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_1);
//            auto text = container->addWidget(cpp14::make_unique<WText>("X"));
//            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
//            container->setAttributeValue(Style::style,Style::background::color::rgb(200,50,50));
//            if( !item.mStokta )
//            {
//                  container->setDisabled(true);
//            }
//            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
//            container->decorationStyle().setCursor(Cursor::PointingHand);

//            container->clicked().connect([=](){
//                if( this->mStok->deleteMalzeme(container_->attributeValue(Style::dataoid).toUTF8()) )
//                {
//                    this->ListCurrentTCNOList(tcno);
//                }
//            });
//        }

    }





}

void StockCikis::ListStokList()
{

    mMalzemeListWidget->clear();


    {
        auto container = mMalzemeListWidget->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setContentAlignment(AlignmentFlag::Center|AlignmentFlag::Middle);
        auto text = container->addWidget(cpp14::make_unique<WText>("Stoktaki Malzemeler"));

    }

    {
        auto container_ = mMalzemeListWidget->addWidget(cpp14::make_unique<WContainerWidget>());
        container_->addStyleClass(Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
        container_->setAttributeValue(Style::style,Style::background::color::rgb(225,235,245));
        container_->setMargin(5,Side::Top);
        auto rContainer = container_->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Kategori"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Extra Bilgi"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>("Veriliş Tarih"));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
//            auto text = container->addWidget(cpp14::make_unique<WText>("Alan TCNO"));
//            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
//            auto text = container->addWidget(cpp14::make_unique<WText>("Alınış Tarihi"));
//            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }

//        {
//            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_5);
//            auto text = container->addWidget(cpp14::make_unique<WText>("Durum"));
//        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
            auto text = container->addWidget(cpp14::make_unique<WText>("Teslim Et"));
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
            container->setAttributeValue(Style::style,Style::background::color::rgb(120,200,100));
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        }

    }

    auto list = mStok->getStoktaList(mKategoriComboBox->currentText().toUTF8());

    for( auto item : list )
    {
        auto container_ = mMalzemeListWidget->addWidget(cpp14::make_unique<WContainerWidget>());
        container_->addStyleClass(Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
        container_->setAttributeValue(Style::style,Style::background::color::rgb(175,185,195));
        container_->setMargin(5,Side::Top);
        auto rContainer = container_->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        container_->setAttributeValue(Style::dataoid,item.mMalzemeOid);


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>(item.mKategori));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>(item.mMalzemeAdi));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            auto text = container->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(item.mVerilisTarihi).toString("dd/MM/yyyy")));
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
//            if( !item.mStokta )
//            {
//                auto text = container->addWidget(cpp14::make_unique<WText>(item.mAlanTCNO));
//            }
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }
        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
//            if( !item.mStokta )
//            {
//                  auto text = container->addWidget(cpp14::make_unique<WText>(WDate::fromJulianDay(item.mAlanisTarihi).toString("dd/MM/yyyy")));
//            }
            container->setAttributeValue(Style::style,Style::Border::right::border("1px solid white"));
        }

//        {
//            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_5);
//            if( item.mStokta )
//            {
//                auto text = container->addWidget(cpp14::make_unique<WText>("Stokta"));
//            }else{
//                auto text = container->addWidget(cpp14::make_unique<WText>("Verilmiş"));
//            }
//        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
            auto text = container->addWidget(cpp14::make_unique<WText>("Teslim Et"));
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::Snow));
            container->setAttributeValue(Style::style,Style::background::color::rgb(120,200,100));
            if( !item.mStokta )
            {
                  container->setDisabled(true);
            }
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            container->decorationStyle().setCursor(Cursor::PointingHand);

            container->clicked().connect([=](){

                if( mTCWidget->getTCNO().size() == 11 )
                {
                    if( this->mStok->setMalzemeTeslim(mTCWidget->getTCNO(),WDate::currentDate().toJulianDay(),container_->attributeValue(Style::dataoid).toUTF8())){
                        this->ListStokList();
                        this->ListCurrentTCNOList(mTCWidget->getTCNO());
                    }
                }else{
                    this->showMessage("UYARI","TCNO Girmediniz");
                }

            });
        }

    }



}
