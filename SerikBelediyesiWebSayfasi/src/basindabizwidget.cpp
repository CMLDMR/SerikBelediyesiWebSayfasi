#include "basindabizwidget.h"



BasindaBizWidget::BasindaBizWidget(mongocxx::database *_db, bool initForBody)
    :DataBaseWidget (_db) , mInitForBody( initForBody )
{

    if( mInitForBody )
    {
        this->initForBody();
    }else{
        this->initForBasin();
    }


//    setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(225),this->getRandom(235),this->getRandom(240)));


}

void BasindaBizWidget::initForBody()
{
    setContentAlignment(AlignmentFlag::Center|AlignmentFlag::Middle);
    setMargin(50,Side::Bottom|Side::Top);

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);


    // Header Arena For Widget
    {
        auto mHeaderContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto mHeaderFluidContainer = mHeaderContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        mHeaderRowContainer = mHeaderFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        this->headerRowContainer()->addStyleClass(Bootstrap::Grid::row);

        auto container = this->headerRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());

        auto text = container->addWidget(cpp14::make_unique<WText>(WString::fromUTF8("Basında Biz")));

        text->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);

        text->setPadding(25,Side::Left|Side::Right);
    }




    // MainContainer Areana For Widget
    {
        auto mContentContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mContentContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto mContentFluidContainer = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mContentFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        mRowContainer = mContentFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        this->rowContainer()->addStyleClass(Bootstrap::Grid::row);


        skip = 0;
        this->loadListMore(6);

    }



    // Footer Arena For Widget
    {
        auto mHeaderContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto mHeaderFluidContainer = mHeaderContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        mFooterRowContainer = mHeaderFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        this->footerRowContainer()->addStyleClass(Bootstrap::Grid::row);

        auto container = this->footerRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb(0,0,0));

        auto text = container->addWidget(cpp14::make_unique<WText>(WString::fromUTF8("Tüm Basılı Yayınlar")));
        text->setAttributeValue(Style::style,Style::color::rgb("255,255,255"));

//        text->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);

        text->setPadding(25,Side::Left|Side::Right);

        container->decorationStyle().setCursor(Cursor::PointingHand);

        container->clicked().connect([=](){
            m_TumBasiliYayinlar.emit(NoClass());
        });

    }





}

void BasindaBizWidget::initForBasin()
{


    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);


    auto rContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    auto detailContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    detailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    detailContainer->setPadding(0,AllSides);
    detailContainer->setContentAlignment(AlignmentFlag::Center);


    auto container = detailContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);


    skip = 0;

    itemCount = this->collection("basin").count(document{}.view());



    {

        std::string path = "img/baskanBackGround.JPG";

        auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
        img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

        img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        img->setHeight(100);
        img->setPadding(0,AllSides);

        auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
        gradientContainer->setHeight(100);
        gradientContainer->addStyleClass("SliderDetailTextBackground");
        auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        layout->addStretch(1);
        auto serik = layout->addWidget(cpp14::make_unique<WText>("Basında Biz"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
        serik->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));

    }


    {

        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto anaSayfaBtn = _layout->addWidget(cpp14::make_unique<WPushButton>("Ana Sayfa"),0,AlignmentFlag::Left);
        anaSayfaBtn->clicked().connect([=](){
                 this->_ClickBack.emit(NoClass());
         });
        anaSayfaBtn->addStyleClass(Bootstrap::Button::Primary);

        _layout->addStretch(1);
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                     Style::background::color::color(Style::color::Grey::Gainsboro));

    }

    // MainContainer Areana For Widget
    {
        auto mContentContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mContentContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto mContentFluidContainer = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mContentFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        mRowContainer = mContentFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        this->rowContainer()->addStyleClass(Bootstrap::Grid::row);

        this->loadListMore();

    }

    // Footer Arena For Widget
    {
        auto mHeaderContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto mHeaderFluidContainer = mHeaderContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        mFooterRowContainer = mHeaderFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        this->footerRowContainer()->addStyleClass(Bootstrap::Grid::row);

        auto container = this->footerRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb(0,0,0));
        container->setHeight(40);

        auto text = container->addWidget(cpp14::make_unique<WText>(WString::fromUTF8("Daha Fazla Yükle+")));
        text->setAttributeValue(Style::style,Style::color::rgb("255,255,255"));

//        text->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);

        text->setPadding(25,Side::Left|Side::Right);

        container->decorationStyle().setCursor(Cursor::PointingHand);

        container->clicked().connect([=](){
            if( itemCount > skip + 12 )
            {
                skip += 12;
                this->loadListMore();
            }
        });

    }


}

Signal<NoClass> &BasindaBizWidget::getTumilanlar()
{

    return m_TumBasiliYayinlar;

}

Signal<NoClass> &BasindaBizWidget::ClickAnayfa()
{
    return _ClickBack;
}

WContainerWidget *BasindaBizWidget::rowContainer() const
{
    return mRowContainer;
}

WContainerWidget *BasindaBizWidget::headerRowContainer() const
{
    return mHeaderRowContainer;
}

void BasindaBizWidget::loadListMore(const int &limit)
{
    try {

        auto sortDoc = document{};

        sortDoc.append(kvp("julianDate",-1));
        mongocxx::options::find findOptions;
        findOptions.sort(sortDoc.view());
        findOptions.limit(limit);
        findOptions.skip(skip);

        auto cursor = this->getDB()->collection("basin").find(document{}.view(),findOptions);



        for( auto doc : cursor )
        {
            this->rowContainer()->addWidget(cpp14::make_unique<BasinItem>(this->getDB() , doc))->addStyleClass(Bootstrap::Grid::Large::col_lg_2
                                                                                                     +Bootstrap::Grid::Medium::col_md_3
                                                                                                     +Bootstrap::Grid::Small::col_sm_4
                                                                                                     +Bootstrap::Grid::ExtraSmall::col_xs_6);
        }

    } catch (mongocxx::exception &e) {
        std::cout << "Line: " << __LINE__ << "  ->" <<e.what() << std::endl;
    }
}

WContainerWidget *BasindaBizWidget::footerRowContainer() const
{
    return mFooterRowContainer;
}

BasinItem::BasinItem(mongocxx::database *_db, const bsoncxx::document::view &view)
    :DataBaseWidget (_db)
{

    setHeight(250);

    decorationStyle().setCursor(Cursor::PointingHand);

    addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    auto vlayout = setLayout(cpp14::make_unique<WVBoxLayout>());

    // Gazete Adı
    {
        auto container = vlayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);

        auto hLayout = container->setLayout(cpp14::make_unique<WHBoxLayout>());

        auto logo_ = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
        logo_->setWidth(35);
        auto logo = logo_->addWidget(cpp14::make_unique<WContainerWidget>());
        logo->setWidth(30);
        logo->setHeight(30);

        auto val = this->collection("basinGazete").find_one(make_document(kvp("gazeteAdi",view["gazete"].get_utf8().value.to_string())));

        if( val )
        {
            auto bucket = this->bucket();
            logo->setAttributeValue(Style::style,Style::background::size::contain+
                                    Style::background::repeat::norepeat+
                                    Style::background::position::center_center+
                                    Style::background::url(SBLDKeys::downloadifNotExist(&bucket , val.value().view()["iconOid"].get_oid().value.to_string())));

//            logo->setAttributeValue(Style::style,Style::background::size::contain+
//                                    Style::background::repeat::norepeat+
//                                    Style::background::position::center_center+
//                                    Style::background::url("img/milliyet.jpg"));

            //milliyet.jpg
        }else{

        }

//        auto text_ = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
//        auto text = logo_->addWidget(cpp14::make_unique<WText>(WString::fromUTF8(view["gazete"].get_utf8().value.to_string().c_str())));

        auto text = hLayout->addWidget(cpp14::make_unique<WText>(WString::fromUTF8(view["gazete"].get_utf8().value.to_string().c_str())),0,
                AlignmentFlag::Center|AlignmentFlag::Left|AlignmentFlag::Middle);
    }


    // Resim
    {
        auto imgContainer = vlayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);
        auto bucket = this->getDB()->gridfs_bucket();
        std::string url_ = SBLDKeys::downloadifNotExist(&bucket,view["baski"].get_oid().value.to_string());

//        std::string url_ = "img/"+std::to_string(this->getRandom(0,15))+".jpg";
        imgContainer->setAttributeValue(Style::style,Style::background::url(url_)+Style::background::repeat::norepeat+
                                        Style::background::size::cover);
        imgContainer->addStyleClass(Bootstrap::ImageShape::img_rounded);
        imgContainer->setHeight(200);
    }

    // Tarih
    {
        auto text = vlayout->addWidget(cpp14::make_unique<WText>(QDate::fromJulianDay(view["julianDate"].get_int64().value).toString("dd/MM/yyyy").toStdString().c_str()),0,AlignmentFlag::Center);
//        text->addStyleClass(Bootstrap::ContextualBackGround::bg_info);

    }

    setGazetefileoid(view["baski"].get_oid().value.to_string());
    setGazeteTitle(view["gazete"].get_utf8().value.to_string());
    setTarihTitle(QDate::fromJulianDay(view["julianDate"].get_int64().value).toString("dd/MM/yyyy").toStdString());

    clicked().connect([=](){


        auto imgContainer = wApp->root()->addWidget(cpp14::make_unique<WContainerWidget>());
        imgContainer->setPositionScheme(PositionScheme::Fixed);
        imgContainer->addStyleClass("rootBody");

        imgContainer->setZIndex(10000);
        imgContainer->setFloatSide(Side::Top|Side::Left);

        imgContainer->setWidth(WLength("100%"));
        imgContainer->setHeight(WLength("100%"));

        imgContainer->setAttributeValue(Style::style,Style::background::color::rgba(0,0,0,0.85));

        auto baskiContainer = imgContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        baskiContainer->setWidth(WLength("100%"));
        baskiContainer->setHeight(WLength("100%"));
        auto bucket = this->bucket();
        baskiContainer->setAttributeValue(Style::style,Style::background::url(SBLDKeys::downloadifNotExist(&bucket,this->getGazetefileoid()))
                                          +Style::background::size::contain
                                          +Style::background::position::center_center
                                          +Style::background::repeat::norepeat);

        auto laytout = baskiContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

        {
            auto container = laytout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Top|AlignmentFlag::Center);

            container->setMinimumSize(240,WLength::Auto);
            container->setHeight(30);

            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(50,100),this->getRandom(75,175),0.65));
            auto text = container->addWidget(cpp14::make_unique<WText>((this->getGazeteTitle())));
            text->setAttributeValue(Style::style,Style::font::size::s16px
                                    +Style::color::color(Style::color::White::AntiqueWhite)
                                    +Style::font::weight::bold);
            container->setContentAlignment(AlignmentFlag::Center);
            text->setMargin(WLength::Auto,AllSides);
        }

        laytout->addStretch(1);

//        {
//            auto container = laytout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Middle|AlignmentFlag::Justify);
//            container->setAttributeValue(Style::style,Style::background::color::rgba(255,0,0));

//            auto hLayout = container->setLayout(cpp14::make_unique<WHBoxLayout>());

//            {
//                auto text = hLayout->addWidget(cpp14::make_unique<WText>("<"),0,AlignmentFlag::Left);
//                text->setAttributeValue(Style::style,Style::font::size::s48px
//                                        +Style::font::weight::bold
//                                        +Style::color::color(Style::color::Orange::Coral));

//                text->clicked().connect([=](){

//                    auto bucket = this->bucket();
//                    baskiContainer->setAttributeValue(Style::style,Style::background::url(SBLDKeys::downloadifNotExist(&bucket,this->getGazetefileoid()))
//                                                      +Style::background::size::contain
//                                                      +Style::background::position::center_center
//                                                      +Style::background::repeat::norepeat);

//                });
//            }

//            hLayout->addStretch(1);

//            {
//                auto text = hLayout->addWidget(cpp14::make_unique<WText>(">"),0,AlignmentFlag::Left);
//                text->setAttributeValue(Style::style,Style::font::size::s48px
//                                        +Style::font::weight::bold
//                                        +Style::color::color(Style::color::Orange::Coral));
//            }
//        }


        laytout->addStretch(1);
        {
            auto container = laytout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Bottom|AlignmentFlag::Center);

            container->setMinimumSize(240,WLength::Auto);
            container->setHeight(30);

            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(50,100),this->getRandom(75,175),0.65));
            auto text = container->addWidget(cpp14::make_unique<WText>((this->getTarihTitle())));
            text->setAttributeValue(Style::style,Style::font::size::s14px
                                    +Style::color::color(Style::color::White::AntiqueWhite)
                                    +Style::font::weight::bold);
            container->setContentAlignment(AlignmentFlag::Center);
        }


        baskiContainer->clicked().connect([=](){
            wApp->root()->removeWidget(imgContainer);
        });



    });

}

Signal<std::string> &BasinItem::getClick()
{

    return _Click;

}



std::string BasinItem::getTarihTitle() const
{
    return tarihTitle;
}

std::string BasinItem::getGazeteTitle() const
{
    return gazeteTitle;
}

std::string BasinItem::getGazetefileoid() const
{
    return gazetefileoid;
}

void BasinItem::setTarihTitle(const std::string &value)
{
    tarihTitle = value;
}

void BasinItem::setGazeteTitle(const std::string &value)
{
    gazeteTitle = value;
}

void BasinItem::setGazetefileoid(const std::string &value)
{
    gazetefileoid = value;
}
