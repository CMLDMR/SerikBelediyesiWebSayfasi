#include "body.h"
#include "giriswidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/talepler/talepwidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/bilgiedinmebasvuruwidget.h"

Body::Body::Body(mongocxx::database *_db, mongocxx::gridfs::bucket *_bucket)
    :WContainerWidget(),
      mDB(new SerikBLDCore::DB(_db)),
      db(_db),
      Bucket(_bucket)
{
    addStyleClass(Bootstrap::Grid::container_fluid);
    setContentAlignment(AlignmentFlag::Center);
    setPadding(0,AllSides);
    setMargin(0,AllSides);


    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());

    mMainContainer->setMaximumSize(1250,WLength::Auto);

    mMainContainer->addStyleClass(Bootstrap::Grid::row);

    SliderCollection = db->collection(SBLDKeys::Slider::collection);
    HaberlerCollection = db->collection(SBLDKeys::Haberler::collection);
    VideolarCollection = db->collection(SBLDKeys::Video::collection);

    this->initBody();

    this->_clickBack.connect(this,&Body::initBody);

}

void Body::Body::initBody()
{
    mMainContainer->clear();


//    auto iletisim = mMainContainer->addWidget(cpp14::make_unique<Iletisim>());
//    iletisim->addStyleClass(Bootstrap::Grid::col_full_12);

}


void Body::Body::initProje()
{

    mMainContainer->clear();
    auto proje = mMainContainer->addWidget(cpp14::make_unique<Proje>(db));
    proje->mGetBack().connect(this,&Body::initBody);

}

void Body::Body::initHaberler()
{


    mMainContainer->clear();
    mMainContainer->setContentAlignment(AlignmentFlag::Center);

    auto haber = mMainContainer->addWidget(cpp14::make_unique<Haber>(db));
    haber->mGetBack().connect(this,&Body::initBody);
    haber->mGetHaber().connect(this,&Body::setNewsDetail);

}

void Body::Body::initEtkinlikler()
{

    mMainContainer->clear();
    auto etkinlik = mMainContainer->addWidget(cpp14::make_unique<Etkinlik>(db));
    etkinlik->mGetBack().connect(this,&Body::initBody);
    etkinlik->mGetEtkinlik().connect(this,&Body::setEventDetail);

}

void Body::Body::initBilgiEdin()
{
    mMainContainer->clear();
    mMainContainer->addWidget(cpp14::make_unique<BilgiEdin::BilgiEdin>(db));
}

void Body::Body::initiletisim()
{

    mMainContainer->clear();

    mMainContainer->setContentAlignment(AlignmentFlag::Center);
    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);
    container->addStyleClass(Bootstrap::Grid::row);

    {
        auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
        img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        std::string path = "img/map1.jpg";

        img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        img->setHeight(160);
        img->setPadding(0,AllSides);

        auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
        gradientContainer->setHeight(160);
        gradientContainer->addStyleClass("SliderDetailTextBackground");
        auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        layout->addStretch(1);
        auto title = layout->addWidget(cpp14::make_unique<WText>("İletişim"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
        title->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue)+Style::font::family::dosis);
    }

    {
        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));

    }

    {
        auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
        img->addStyleClass(Bootstrap::Grid::container_fluid);
        img->setAttributeValue(Style::style,Style::background::url("img/map.jpg")+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        img->setHeight(600);
        img->setPadding(0,AllSides);


        auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        textContainer->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                         Style::background::color::color(Style::color::White::Snow));

        auto layout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());


        Wt::WLink link = Wt::WLink("https://www.google.co.uk/maps/place/T.C.+Serik+Belediyesi/@36.9165609,31.1040117,16z/data=!4m5!3m4!1s0x14c36295881be76f:0xe1e49781ec99e7eb!8m2!3d36.9161357!4d31.1040598");
        link.setTarget(Wt::LinkTarget::NewWindow);

        std::unique_ptr<Wt::WAnchor> anchor =
                Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                    "<h4>Map Link</h4>");

        layout->addWidget(std::move(anchor),0,AlignmentFlag::Top);

        layout->addWidget(cpp14::make_unique<WText>("<h2>Adres: Orta Mahalle, Kızderesi Cad No:25, 07500 Serik/Antalya</h2>",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
        layout->addWidget(cpp14::make_unique<WText>("İletişim: 444 9 722",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);

        layout->addWidget(cpp14::make_unique<WText>("Fax: +90 242 722 19 68",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
        layout->addWidget(cpp14::make_unique<WText>("mail: serik@serik.bel.tr",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);

    }


}

void Body::Body::initTalep()
{
    mMainContainer->clear();

    mMainContainer->setContentAlignment(AlignmentFlag::Center);
    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);
    container->addStyleClass(Bootstrap::Grid::row);


//    // Yeni Talep Sayfası
    {
        auto talep = container->addWidget (cpp14::make_unique<TalepWidget::TalepWidget>(this->db));
        talep->addStyleClass (Bootstrap::Grid::col_full_12);
    }

}

void Body::Body::initGiris()
{
    mMainContainer->clear();

    mMainContainer->setContentAlignment(AlignmentFlag::Center);
    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);
    container->addStyleClass(Bootstrap::Grid::row);

    // Giris Sayfası
    {
        auto talep = container->addWidget(cpp14::make_unique<Giris::GirisWidget>(db));
        talep->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    }
}

void Body::Body::initHakkinda()
{

    mMainContainer->clear();

    mMainContainer->setContentAlignment(AlignmentFlag::Center);
    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);
    container->addStyleClass(Bootstrap::Grid::row);

    // Giris Sayfası
    {
        auto talep = container->addWidget(cpp14::make_unique<Serik::Hakkinda>(db));
        talep->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    }

}

void Body::Body::initCalismalar()
{
    mMainContainer->clear();

    mMainContainer->setContentAlignment(AlignmentFlag::Center);
    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);
    container->addStyleClass(Bootstrap::Grid::row);

    // Giris Sayfası
    {
        auto talep = container->addWidget(cpp14::make_unique<Calisma>(db));
        talep->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        talep->_ClickBack.connect(this,&Body::initBody);
    }
    wApp->setInternalPath("/initCalismalar",false);
}



void Body::Body::setNewsDetail(std::string oid)
{


    mMainContainer->clear();
    mMainContainer->setContentAlignment(AlignmentFlag::Center);

    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->setPadding(0,AllSides);

    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Haberler::Yayinda,SBLDKeys::Haberler::Yayin::Yayinda));
        filter.append(kvp(SBLDKeys::Haberler::oid,bsoncxx::oid{oid}));

    } catch (bsoncxx::exception &e) {
        container->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }

    auto update = document{};
    bool updateOperatorError;
    try {
        update.append(kvp("$inc",make_document(kvp(SBLDKeys::Haberler::OkunmaSayisi,bsoncxx::types::b_double{1}))));
        updateOperatorError=true;
    } catch (bsoncxx::exception &e) {
        updateOperatorError = true;
    }

    auto collection = db->collection(SBLDKeys::Haberler::collection);
    auto bucket = db->gridfs_bucket();

    try {
        mongocxx::stdx::optional<bsoncxx::document::value> val = collection.find_one_and_update(filter.view(),update.view());
        if( !val )
        {
            container->addWidget(cpp14::make_unique<WText>(WString("Empty Document")));
            return;
        }else{

            auto view = val.value().view();

            {
                auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
                img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                std::string path = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::Haberler::icon].get_oid().value.to_string());

                img->setHeight(150);
                img->setPadding(0,AllSides);

                auto backimg = img->addWidget(cpp14::make_unique<WContainerWidget>());
                backimg->setPositionScheme(PositionScheme::Absolute);
                backimg->setHeight(WLength("100%"));
                backimg->setWidth(WLength("100%"));

                backimg->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center+"z-index:  -99;");
                backimg->addStyleClass("CommanFilter");

                auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
                gradientContainer->setHeight(150);
                gradientContainer->addStyleClass("img/SliderDetailTextBackground");
                auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addStretch(1);
                auto title = layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Haberler::Baslik].get_string().value.data()),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
                title->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
                title->addStyleClass("NewsMainTitle");
                wApp->setInternalPath("/"+title->text().toUTF8(),false);
            }

            {
                auto array = view[SBLDKeys::Haberler::fileOidList].get_array().value;
                for( auto doc : array )
                {
                    std::string path = SBLDKeys::downloadifNotExist(&bucket,doc.get_oid().value.to_string(),true);
                }
            }


            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
                auto back = _layout->addWidget(cpp14::make_unique<WPushButton>("Ana Sayfa"),0,AlignmentFlag::Left);
                back->clicked().connect([=](){
                    this->_clickBack.emit(NoClass());
                });
                back->addStyleClass(Bootstrap::Button::Primary);
                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }

            {
                auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                textContainer->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                                 Style::background::color::color(Style::color::White::Snow));


                auto layout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Haberler::html].get_string().value.data(),TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
            }

            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
                _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
                    this->_clickBack.emit(NoClass());
                });
                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }

        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }

}

void Body::Body::setVideoDetail(std::string oid)
{

    mMainContainer->clear();
    mMainContainer->setContentAlignment(AlignmentFlag::Center);

    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Video::oid,bsoncxx::oid{oid}));

    } catch (bsoncxx::exception &e) {
        container->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }

    try {
        mongocxx::stdx::optional<bsoncxx::document::value> val = VideolarCollection.find_one(filter.view());
        if( !val )
        {
            container->addWidget(cpp14::make_unique<WText>(WString("Empty Document")));
            return;
        }else{

            auto view = val.value().view();

            std::string path = SBLDKeys::downloadifNotExist(Bucket,view[SBLDKeys::Video::VideoLabel].get_oid().value.to_string(),true);

            {
                auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
                img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

                img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
                img->setHeight(150);
                img->setPadding(0,AllSides);

                auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
                gradientContainer->setHeight(150);
                gradientContainer->addStyleClass("SliderDetailTextBackground");
                auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addStretch(1);
                auto title = layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Video::title].get_string().value.data()),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
                title->setAttributeValue(Style::style,Style::font::size::s48px+Style::color::color(Style::color::White::AliceBlue));
                wApp->setInternalPath("/"+title->text().toUTF8(),false);
            }


            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
                _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
                    this->_clickBack.emit(NoClass());
                });
                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }


            {
                std::string mp4Video = SBLDKeys::downloadifNotExist(Bucket,view[SBLDKeys::Video::VideoFile].get_oid().value.to_string(),true);
                std::string poster = path;

                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

                _container->setHtmlTagName("video");
                _container->setAttributeValue("controls","");
                _container->setAttributeValue("autoplay","");

                auto videoContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());

                videoContainer->setHtmlTagName("source");
                videoContainer->setAttributeValue("src",mp4Video);
                videoContainer->setAttributeValue("type","video/mp4");

            }

            {
                auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                textContainer->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                                 Style::background::color::color(Style::color::White::Snow));

                auto layout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Video::VideoExplain].get_string().value.data(),TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
            }

            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
                _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
                    this->_clickBack.emit(NoClass());
                });
                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }

        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }


}

void Body::Body::setBaskanDetail(std::string oid)
{


    mMainContainer->clear();
    mMainContainer->setContentAlignment(AlignmentFlag::Center);

    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Yonetim::Baskan::oid,bsoncxx::oid{oid}));

    } catch (bsoncxx::exception &e) {
        container->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }

    auto collection = db->collection(SBLDKeys::Yonetim::collection);
    try {
        mongocxx::stdx::optional<bsoncxx::document::value> val = collection.find_one(filter.view());
        if( !val )
        {
            container->addWidget(cpp14::make_unique<WText>(WString("Empty Document")));
            return;
        }else{

            auto view = val.value().view();

            std::string path = "img/baskanBackGround.JPG";

            {
                auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
                img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

                img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
                img->setHeight(150);
                img->setPadding(0,AllSides);

                auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
                gradientContainer->setHeight(150);
                gradientContainer->addStyleClass("SliderDetailTextBackground");
                auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addStretch(1);
                auto serik = layout->addWidget(cpp14::make_unique<WText>("Serik Belediye Başkanı"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
                serik->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
                auto title = layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Yonetim::Baskan::baskan].get_string().value.data()),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
                title->setAttributeValue(Style::style,Style::font::size::s48px+Style::color::color(Style::color::White::AliceBlue));
                wApp->setInternalPath("/"+title->text().toUTF8(),false);
            }


            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
                _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
                    this->_clickBack.emit(NoClass());
                });
                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }

            {
                auto array = view[SBLDKeys::Yonetim::Baskan::fileList].get_array().value;
                for( auto doc : array )
                {
                    SBLDKeys::downloadifNotExist<mongocxx::gridfs::bucket,
                            QFileInfo,
                            QString,
                            QFile,
                            QByteArray,
                            QIODevice,
                            bsoncxx::types::value>(Bucket,bsoncxx::types::bson_value::value(doc.get_oid()),true);
                }
            }

            {
                auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                textContainer->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                                 Style::background::color::color(Style::color::White::Snow));


                auto layout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Yonetim::Baskan::html].get_string().value.data(),TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
            }


            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
                _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
                    this->_clickBack.emit(NoClass());
                });
                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }


        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }

    auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
    _container->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = _container->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto titleContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        titleContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto text = titleContainer->addWidget(cpp14::make_unique<BaskanaMesajWidget>(this->db));
        text->setMargin(WLength::Auto,Side::Left|Side::Right);
    }
}

void Body::Body::setEventDetail(std::string oid)
{

    mMainContainer->clear();
    mMainContainer->setContentAlignment(AlignmentFlag::Center);

    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Etkinlik::oid,bsoncxx::oid{oid}));

    } catch (bsoncxx::exception &e) {
        container->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }

    auto collection = db->collection(SBLDKeys::Etkinlik::collection);
    auto bucket = db->gridfs_bucket();
    try {
        mongocxx::stdx::optional<bsoncxx::document::value> val = collection.find_one(filter.view());
        if( !val )
        {
            container->addWidget(cpp14::make_unique<WText>(WString("Empty Document")));
            return;
        }else{

            auto view = val.value().view();

            std::string path = "img/baskanBackGround.JPG";

            {
                auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
                img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);


                img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
                img->setHeight(150);
                img->setPadding(0,AllSides);

                auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
                gradientContainer->setHeight(150);
                gradientContainer->addStyleClass("SliderDetailTextBackground");
                auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addStretch(1);
                auto serik = layout->addWidget(cpp14::make_unique<WText>("Etkinlikler"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
                serik->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
                auto title = layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Etkinlik::title].get_string().value.data()),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
                title->setAttributeValue(Style::style,Style::font::size::s48px+Style::color::color(Style::color::White::AliceBlue));
                wApp->setInternalPath("/"+title->text().toUTF8(),false);
            }


            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
                _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
                    this->_clickBack.emit(NoClass());
                });
                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }


            {
                auto array = view[SBLDKeys::Etkinlik::fileList].get_array().value;
                for( auto doc : array )
                {
                    SBLDKeys::downloadifNotExist(&bucket,doc.get_oid().value.to_string(),true);
                }
            }

            {
                auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                textContainer->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                                 Style::background::color::color(Style::color::White::Snow));

                auto layout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Etkinlik::html].get_string().value.data(),TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
            }


            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
                _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
                    this->_clickBack.emit(NoClass());
                });
                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }


        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }
}

void Body::Body::setProjeDetail(std::string oid)
{
    mMainContainer->clear();
    mMainContainer->setOverflow(Overflow::Hidden);

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
        return;
    }

    bsoncxx::document::value val(document{}.view());

    try {
        auto val_ = this->db->collection(SBLDKeys::Projeler::collection).find_one(filter.view());
        if( val_ )
        {
            if( !val_.value().view().empty() )
            {
                val = val_.value();
            }else{
                mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("Document Empty")));
            }
        }else{
            mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("No Document Returned")));
        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
    }

    auto view = val.view();

    auto TitleContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto TitleContainerColor = TitleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    TitleContainerColor->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(oid.c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(view[SBLDKeys::Projeler::title].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Başlama Tarihi {1}").arg(view[SBLDKeys::Projeler::julianDate].get_int64().value)),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Durum {1}").arg(view[SBLDKeys::Projeler::durum].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Birim {1}").arg(view[SBLDKeys::Projeler::birim].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto text = TitleContainerColor->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(view[SBLDKeys::Projeler::mahalle].get_string().value.data())));
        text->addStyleClass(Bootstrap::Label::Warning+Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto bucket = this->db->gridfs_bucket();
        std::string iconPaht = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::Projeler::icon].get_oid().value.to_string());
        TitleContainer->setAttributeValue(Style::style,Style::background::url(iconPaht)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setHeight(50);
    }


    {
        auto asamaContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        auto bucket = this->db->gridfs_bucket();
        try {
            auto asamaList = view[SBLDKeys::Projeler::slide].get_array().value;

            for( auto _asama : asamaList )
            {
                auto doc = _asama.get_document().value;
                auto container = asamaContainer->addWidget(cpp14::make_unique<WContainerWidget>());

                auto slideCOntainer = cpp14::make_unique<WContainerWidget>();
                slideCOntainer->setHeight(350);

                std::string imgPath = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Projeler::slideItem::img].get_oid().value.to_string());

                auto frontimgContainer = slideCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                frontimgContainer->setHeight(350);
                frontimgContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::contain+Style::background::position::center_center+Style::background::repeat::norepeat);

                container->addWidget(std::move(slideCOntainer));

                auto TextCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                TextCOntainer->setPadding(15,Side::Bottom|Side::Top);
                TextCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.75));
                auto aciklamaText = TextCOntainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Projeler::slideItem::text].get_string().value.data())));
                aciklamaText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold+Style::font::size::s16px);
            }
        } catch (bsoncxx::exception &e) {

        }

    }

}

void Body::Body::setCalismaDetail(std::string oid)
{

    mMainContainer->clear();

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
        return;
    }

    bsoncxx::document::value val(document{}.view());

    try {
        auto val_ = this->db->collection(SBLDKeys::Calismalar::collection).find_one(filter.view());
        if( val_ )
        {
            if( !val_.value().view().empty() )
            {
                val = val_.value();
            }else{
                mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("Document Empty")));
            }
        }else{
            mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("No Document Returned")));
        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
    }

    auto view = val.view();

    auto TitleContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto TitleContainerColor = TitleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    TitleContainerColor->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(oid.c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(view[SBLDKeys::Calismalar::baslik].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Başlama Tarihi {1}").arg(WDate::fromJulianDay(view[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value).toString("dd/MM/yyyy"))),0,AlignmentFlag::Middle);
        text->addStyleClass(Bootstrap::Label::Primary);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Durum {1}").arg(view[SBLDKeys::Calismalar::durum].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Kategori {1}").arg(view[SBLDKeys::Calismalar::kategori].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Birim {1}").arg(view[SBLDKeys::Calismalar::birim].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tanım: <u>{1}</u>").arg(view[SBLDKeys::Calismalar::tanim].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }



    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        container->setPadding(10,AllSides);
        for( auto mah : view[SBLDKeys::Calismalar::mahalleler].get_array().value )
        {
            auto text = container->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(mah.get_string().value.data())));
            text->addStyleClass(Bootstrap::Label::Warning+Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
            //            text->setMargin(5,AllSides);
        }
    }


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto bucket = this->db->gridfs_bucket();
        std::string iconPaht = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::Calismalar::icon].get_oid().value.to_string());
        TitleContainer->setAttributeValue(Style::style,Style::background::url(iconPaht)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setHeight(50);
    }
    {


        auto asamaContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        auto bucket = this->db->gridfs_bucket();
        try {
            auto asamaList = view[SBLDKeys::Calismalar::asamalar].get_array().value;

            for( auto _asama : asamaList )
            {
                auto doc = _asama.get_document().value;
                auto container = asamaContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                //                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
                auto StackWidget = container->addWidget(cpp14::make_unique<WStackedWidget>());
                StackWidget->setTransitionAnimation(WAnimation(AnimationEffect::SlideInFromRight,TimingFunction::EaseInOut,250),true);
                std::vector<std::string> imgPathList;
                for( auto id : doc[SBLDKeys::Calismalar::Asama::Gorseller].get_array().value )
                {
                    std::string imgPath = SBLDKeys::downloadifNotExist(&bucket,id.get_oid().value.to_string());
                    imgPathList.push_back(imgPath);
                }

                for( auto str : imgPathList )
                {
                    auto slideCOntainer = cpp14::make_unique<WContainerWidget>();
                    slideCOntainer->setHeight(350);



                    auto frontimgContainer = slideCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    frontimgContainer->setHeight(350);
                    frontimgContainer->setAttributeValue(Style::style,Style::background::url(str)+Style::background::size::contain+Style::background::position::center_center+Style::background::repeat::norepeat);

                    auto imgContainer = cpp14::make_unique<WContainerWidget>();
                    imgContainer->setHeight(350);
                    imgContainer->setWidth(WLength("100%"));
                    imgContainer->setAttributeValue(Style::style,Style::background::url(str)+Style::background::size::contain+Style::background::position::center_center+"z-index:-1; ");
                    imgContainer->addStyleClass("CommanFilter");
                    imgContainer->setPositionScheme(PositionScheme::Absolute);
                    imgContainer->setOffsets(0,Side::Top);
                    slideCOntainer->addWidget(std::move(imgContainer));

                    StackWidget->addWidget(std::move(slideCOntainer));

                }


                auto btnContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                btnContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));


                auto hLayout = btnContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

                auto backBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("Geri"));
                backBtn->clicked().connect([=](){
                    int indexCount = StackWidget->count();
                    int CurrentIndex = StackWidget->currentIndex();

                    if( CurrentIndex > 0 )
                    {
                        StackWidget->setCurrentIndex(--CurrentIndex );
                    }else{
                        StackWidget->setCurrentIndex( indexCount-1 );
                    }
                });

                hLayout->addStretch(1);
                int imgCounter = 0;

                for( auto str : imgPathList )
                {
                    imgCounter++;
                }

                {
                    auto text = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Resim").arg(imgCounter)),0,AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s24px);
                }
                hLayout->addStretch(1);



                auto nextBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("İleri"));

                nextBtn->clicked().connect([=](){
                    int indexCount = StackWidget->count();
                    int CurrentIndex = StackWidget->currentIndex();

                    if( CurrentIndex < indexCount-1 )
                    {
                        StackWidget->setCurrentIndex(++CurrentIndex );
                    }else{
                        StackWidget->setCurrentIndex( 0 );
                    }
                });

                auto TextCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                TextCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
                auto aciklamaText = TextCOntainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Calismalar::Asama::Aciklama].get_string().value.data())));
                aciklamaText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
                TextCOntainer->setPadding(10,Side::Top|Side::Bottom);
            }
        } catch (bsoncxx::exception &e) {
            std::cout << __LINE__ << " " << __FUNCTION__ << "\n";
        }
    }
}

std::string Body::Body::downloadifNotExist(bsoncxx::types::bson_value::value oid, bool forceFilename)
{

    auto downloader = Bucket->open_download_stream(oid);
    auto file_length = downloader.file_length();
    auto bytes_counter = 0;

    QFileInfo info( downloader.files_document()["filename"].get_string().value.data() );

    QString fullFilename;

    if( forceFilename )
    {
        fullFilename = QString("tempfile/%1").arg(downloader.files_document()["filename"].get_string().value.data());
    }else{
        fullFilename = QString("tempfile/%2.%1").arg(info.suffix())
                .arg(downloader.files_document()["md5"].get_string().value.data());
    }


    if( QFile::exists("docroot/"+fullFilename) )
    {
        return fullFilename.toStdString();
    }

    auto buffer_size = std::min(file_length, static_cast<std::int64_t>(downloader.chunk_size()));
    auto buffer = bsoncxx::stdx::make_unique<std::uint8_t[]>(static_cast<std::size_t>(buffer_size));
    QByteArray mainArray;
    while ( auto length_read = downloader.read(buffer.get(), static_cast<std::size_t>(buffer_size)) ) {
        bytes_counter += static_cast<std::int32_t>( length_read );
        QByteArray ar((const char*)buffer.get(),bytes_counter);
        mainArray+= ar;
    }

    QFile file( "docroot/"+fullFilename );
    if( file.open(QIODevice::WriteOnly) )
    {
        file.write( mainArray );
        file.close();
    }else{
        std::cout << "Error Can Not Open File: " << file.fileName().toStdString() << std::endl;
    }
    return fullFilename.toStdString();
}

std::string Body::Body::downloadifNotExist(std::string oid, bool forceFilename)
{
    auto doc = bsoncxx::builder::basic::document{};

    try {
        doc.append(bsoncxx::builder::basic::kvp("key",bsoncxx::oid{oid}.to_string()));
    } catch (bsoncxx::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return "NULL";
    }
    auto downloader = Bucket->open_download_stream(bsoncxx::types::value(doc.view()["key"].get_oid()));

    auto file_length = downloader.file_length();
    auto bytes_counter = 0;

    QFileInfo info( downloader.files_document()["filename"].get_string().value.data() );

    QString fullFilename;

    if( forceFilename )
    {
        fullFilename = QString("tempfile/%1").arg(downloader.files_document()["filename"].get_string().value.data());
    }else{
        fullFilename = QString("tempfile/%2.%1").arg(info.suffix())
                .arg(downloader.files_document()["md5"].get_string().value.data());
    }

    if( QFile::exists("docroot/"+fullFilename) )
    {
        return fullFilename.toStdString();
    }

    auto buffer_size = std::min(file_length, static_cast<std::int64_t>(downloader.chunk_size()));
    auto buffer = bsoncxx::stdx::make_unique<std::uint8_t[]>(static_cast<std::size_t>(buffer_size));
    QByteArray mainArray;
    while ( auto length_read = downloader.read(buffer.get(), static_cast<std::size_t>(buffer_size)) ) {
        bytes_counter += static_cast<std::int32_t>( length_read );
        QByteArray ar((const char*)buffer.get(),bytes_counter);
        mainArray+= ar;
    }

    QFile file( "docroot/"+fullFilename );
    if( file.open(QIODevice::WriteOnly) )
    {
        file.write( mainArray );
        file.close();
    }else{
        std::cout << "Error Can Not Open File: " << file.fileName().toStdString() << std::endl;
    }
    return fullFilename.toStdString();
}

void Body::Body::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });
    messageBox->show();
}





//Body::ProjectPanel::ProjectPanel(mongocxx::database *_db)
//    :WContainerWidget(),
//      db(_db)
//{
//    setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::Black));
//    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
//    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
//    this->refreshList();
//    CurrentIndex = 2;
//    this->setCurrentProject();
//}

//void Body::ProjectPanel::setCurrentProject()
//{

//    mMainContainer->clear();

//    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//    row->addStyleClass(Bootstrap::Grid::row);
//    row->setMargin(15,Side::Top|Side::Bottom);

//    auto collection = db->collection(SBLDKeys::Projeler::collection);
//    auto bucket = db->gridfs_bucket();

//    auto Currentview = document{}.view();

//    std::vector<CurrentItemKey> pathList;

//    auto filter = document{};

//    try {
//        filter.append(kvp(SBLDKeys::Projeler::oid,bsoncxx::oid{list.at(CurrentIndex).oid}));
//    } catch (bsoncxx::exception &e) {
//        setstatus(e);
//        return;
//    }

//    try {
//        auto val = collection.find_one(filter.view());
//        if( !val )
//        {
//            setstatus("Empty Document");
//        }else{
//            auto Currentview = val.value().view();
//            try {
//                auto arlist = Currentview[SBLDKeys::Projeler::slide].get_array().value;

//                int currentIndex = 0;
//                for( auto element : arlist )
//                {
//                    std::string path = SBLDKeys::downloadifNotExist(&bucket,element[SBLDKeys::Projeler::slideItem::img].get_oid().value.to_string().c_str());
//                    CurrentItemKey itemKey;
//                    itemKey.imgPath = path;
//                    itemKey.text = element[SBLDKeys::Projeler::slideItem::text].get_string().value.data();
//                    itemKey.index = currentIndex++;
//                    pathList.push_back(itemKey);
//                }

//            } catch (bsoncxx::exception &e) {
//                setstatus(e);
//                return;
//            }
//        }
//    } catch (mongocxx::exception &e) {
//        setstatus(e);
//        return;
//    }

//    // Proje Başlığı
//    {

//        auto titleContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
//        auto title = titleContainer->addWidget(cpp14::make_unique<WText>(list.at(CurrentIndex).title));
//        title->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
//        title->setAttributeValue(Style::style,Style::font::size::s24px+Style::color::color(Style::color::White::FloralWhite));

//    }


//    {
//        auto sliderContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
//        sliderContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

//        auto container = sliderContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//        container->addStyleClass(Bootstrap::Grid::container_fluid);

//        auto _row = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        _row->addStyleClass(Bootstrap::Grid::row);

//        {
//            auto blackLine = _row->addWidget(cpp14::make_unique<WContainerWidget>());
//            blackLine->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
//            blackLine->setAttributeValue(Style::style,Style::Border::border("0px solid "+Style::color::Green::DarkOliveGreen)+
//                                         Style::background::color::color(Style::color::Grey::LightGray));
//            //            blackLine->setHeight(550);
//            blackLine->addStyleClass("blackLine");

//            auto layout = blackLine->setLayout(cpp14::make_unique<WVBoxLayout>());

//            auto imgContainer = layout->addWidget(cpp14::make_unique<WContainerWidget>());
//            //            imgContainer->setAttributeValue(Style::style,Style::background::url("img/2.jpg"));
//            imgContainer->addStyleClass("ProjectSliderButtonBackground");
//            auto _layout = imgContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

//            auto text = _layout->addWidget(cpp14::make_unique<WText>("<"),0,AlignmentFlag::Middle);
//            text->addStyleClass("ProjectSliderButton");
//            text->clicked().connect([=](){
//                CurrentIndex--;
//                int count = list.size();
//                if( CurrentIndex < 0 )
//                {
//                    CurrentIndex = count-1;
//                }
//                this->setCurrentProject();
//            });


//            auto title = _layout->addWidget(cpp14::make_unique<WText>("Yayla Suyu Projesi"));
//            title->setAttributeValue(Style::style,Style::color::color(Style::color::Grey::Black)
//                                     +Style::font::weight::bold+Style::font::size::s24px);

//            int count = list.size();
//            if( CurrentIndex == 0 )
//            {
//                title->setText(list.at(count-1).title);
//                imgContainer->setAttributeValue(Style::style,Style::background::url(list.at(count-1).img)+Style::background::size::cover);
//            }else{
//                title->setText(list.at(CurrentIndex-1).title);
//                imgContainer->setAttributeValue(Style::style,Style::background::url(list.at(CurrentIndex-1).img)+Style::background::size::cover);
//            }
//        }

//        {
//            SubCurrentIndex = 0;
//            auto sliderArena = _row->addWidget(cpp14::make_unique<WContainerWidget>());
//            sliderArena->addStyleClass(Bootstrap::Grid::Large::col_lg_8 +Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
//            sliderArena->setPadding(0,AllSides);

//            auto imgContainer = sliderArena->addWidget(cpp14::make_unique<WContainerWidget>());
//            imgContainer->addStyleClass(Bootstrap::Grid::container_fluid);
//            imgContainer->setHeight(550);
//            imgContainer->setPadding(0,AllSides);
//            imgContainer->setMargin(0,AllSides);

//            {
//                auto imgRow = imgContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//                imgRow->addStyleClass("ProjectIMGRow");

//                auto sliderStackedWidget = imgRow->addWidget(cpp14::make_unique<WStackedWidget>());
//                {
//                    int row = 0;
//                    for( auto doc : pathList )
//                    {
//                        auto imgContainerWidget = cpp14::make_unique<WContainerWidget>();
//                        imgContainerWidget->setAttributeValue(Style::style,Style::background::url(doc.imgPath)+Style::background::size::cover+Style::background::origin::border_box);
//                        imgContainerWidget->setHeight(550);
//                        sliderStackedWidget->insertWidget(row++,std::move(imgContainerWidget));
//                    }
//                    sliderStackedWidget->setCurrentIndex(0);
//                }

//                auto backBtn = imgRow->addWidget(cpp14::make_unique<WContainerWidget>());
//                backBtn->addStyleClass("ProjectIMGSlideBack");
//                backBtn->addWidget(cpp14::make_unique<WText>("<"));

//                auto NextBtn = imgRow->addWidget(cpp14::make_unique<WContainerWidget>());
//                NextBtn->addStyleClass("ProjectIMGSlideNext");
//                NextBtn->addWidget(cpp14::make_unique<WText>(">"));

//                NextBtn->clicked().connect([=](){

//                    SubCurrentIndex++;
//                    if( SubCurrentIndex >= pathList.size() )
//                    {
//                        SubCurrentIndex = 0;
//                    }
//                    sliderStackedWidget->setCurrentIndex(SubCurrentIndex,WAnimation(AnimationEffect::Fade,TimingFunction::EaseInOut,250));

//                });

//                backBtn->clicked().connect([=](){
//                    SubCurrentIndex--;
//                    if( SubCurrentIndex < 0 )
//                    {
//                        SubCurrentIndex = pathList.size()-1;
//                    }
//                    sliderStackedWidget->setCurrentIndex(SubCurrentIndex,WAnimation(AnimationEffect::Fade,TimingFunction::EaseInOut,250));

//                    // Proje Slide Alt Yazı Tanım
//                    //                    textTitle->setText(WString("{1}/{2} - ").arg(SubCurrentIndex).arg(pathList.size()-1).toUTF8()+pathList.at(SubCurrentIndex).text);
//                });



//            }
//        }

//        {
//            auto blackLine = _row->addWidget(cpp14::make_unique<WContainerWidget>());
//            blackLine->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
//            blackLine->setAttributeValue(Style::style,Style::Border::border("0px solid "+Style::color::Green::DarkOliveGreen)+
//                                         Style::background::color::color(Style::color::Grey::LightGray));
//            //            blackLine->setHeight(550);
//            blackLine->addStyleClass("blackLine");
//            auto layout = blackLine->setLayout(cpp14::make_unique<WVBoxLayout>());

//            auto imgContainer = layout->addWidget(cpp14::make_unique<WContainerWidget>());
//            imgContainer->setAttributeValue(Style::style,Style::background::url("img/4.jpg")+Style::background::size::cover);
//            imgContainer->addStyleClass("ProjectSliderButtonBackground");
//            auto _layout = imgContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

//            auto text = _layout->addWidget(cpp14::make_unique<WText>(">"),0,AlignmentFlag::Middle);
//            text->addStyleClass("ProjectSliderButton");

//            text->clicked().connect([=](){
//                CurrentIndex++;
//                int count = list.size();
//                if( CurrentIndex >= count )
//                {
//                    CurrentIndex = 0;
//                }
//                this->setCurrentProject();
//            });


//            auto title = _layout->addWidget(cpp14::make_unique<WText>("**"));
//            title->setAttributeValue(Style::style,Style::color::color(Style::color::Grey::Black)
//                                     +Style::font::weight::bold+Style::font::size::s24px);
//            int count = list.size();

//            if( CurrentIndex == count-1 )
//            {
//                title->setText(list.at(0).title);
//                imgContainer->setAttributeValue(Style::style,Style::background::url(list.at(0).img)+Style::background::size::cover);
//            }else{
//                title->setText(list.at(CurrentIndex+1).title);
//                imgContainer->setAttributeValue(Style::style,Style::background::url(list.at(CurrentIndex+1).img)+Style::background::size::cover);
//            }
//        }
//    }


//}

//void Body::ProjectPanel::refreshList()
//{

//    auto collection = db->collection(SBLDKeys::Projeler::collection);
//    auto bucket = db->gridfs_bucket();

//    list.clear();
//    try {
//        auto cursor = collection.find(document{}.view());

//        for( auto doc : cursor )
//        {

//            try {

//                std::string path;

//                auto array = doc[SBLDKeys::Projeler::slide].get_array().value;

//                int itemCount = std::distance(array.begin(),array.end());

//                if( itemCount )
//                {
//                    for( auto element : array )
//                    {
//                        path = SBLDKeys::downloadifNotExist(&bucket,element[SBLDKeys::Projeler::slideItem::img].get_oid().value.to_string());
//                        break;
//                    }
//                }

//                ProjectHeader header;
//                header.oid = doc[SBLDKeys::Projeler::oid].get_oid().value.to_string().c_str();
//                header.title = doc[SBLDKeys::Projeler::title].get_string().value.data();
//                header.img = path;

//                //                std::cout << "IMGPATH: " << path;

//                list.push_back(header);

//            } catch (bsoncxx::exception &e) {
//                setstatus(e);
//            }
//        }
//    } catch (mongocxx::exception &e) {
//        setstatus(e);
//    }

//}

//void Body::ProjectPanel::setstatus(bsoncxx::exception &e)
//{
//    mMainContainer->clear();
//    auto error = mMainContainer->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(e.what())));
//    error->setAttributeValue(Style::style,Style::font::size::s18px);
//}

//void Body::ProjectPanel::setstatus(mongocxx::exception &e)
//{
//    mMainContainer->clear();
//    auto error = mMainContainer->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(e.what())));
//    error->setAttributeValue(Style::style,Style::font::size::s18px);
//}

//void Body::ProjectPanel::setstatus(WString e)
//{
//    mMainContainer->clear();
//    auto error = mMainContainer->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(e.toUTF8().c_str())));
//    error->setAttributeValue(Style::style,Style::font::size::s18px);
//}


Body::Iletisim::Iletisim()
{
    setAttributeValue(Style::style,Style::background::color::color(Style::color::Orange::DarkOrange));
    setPadding(25,AllSides);

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    mMainContainer->setMargin(90,Side::Top);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto img = layout->addWidget(cpp14::make_unique<WContainerWidget>());
        img->setAttributeValue(Style::style,Style::background::url("img/phone-center.png")+Style::background::repeat::norepeat+Style::background::position::center_center);
        img->setHeight(100);


        auto textContainer = layout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto textLayout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text1 = textLayout->addWidget(cpp14::make_unique<WText>("Çağrı Merkezi"),0,AlignmentFlag::Middle);
            text1->setAttributeValue(Style::style,Style::color::color(Style::color::White::White)+
                                     Style::font::size::s18px+Style::font::weight::bold);
        }
        {
            auto text1 = textLayout->addWidget(cpp14::make_unique<WText>("444 9 722"),0,AlignmentFlag::Middle);
            text1->setAttributeValue(Style::style,Style::color::color(Style::color::White::White)+
                                     Style::Border::border("2px solid white")+
                                     Style::font::size::s18px+
                                     Style::font::weight::bold);
        }
    }

    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto img = layout->addWidget(cpp14::make_unique<WContainerWidget>());
        img->setAttributeValue(Style::style,Style::background::url("img/fax.png")+Style::background::repeat::norepeat+Style::background::position::center_center);

        img->setHeight(100);


        auto textContainer = layout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto textLayout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text1 = textLayout->addWidget(cpp14::make_unique<WText>("Mail/Fax"),0,AlignmentFlag::Middle);
            text1->setAttributeValue(Style::style,Style::color::color(Style::color::White::White)+
                                     Style::font::size::s18px+Style::font::weight::bold);
        }
        {
            auto text1 = textLayout->addWidget(cpp14::make_unique<WText>("serik@serik.bel.tr"),0,AlignmentFlag::Middle);
            text1->setAttributeValue(Style::style,Style::color::color(Style::color::White::White)+
                                     Style::Border::border("2px solid white")+
                                     Style::font::size::s16px+
                                     Style::font::weight::bold);
        }
        {
            auto text1 = textLayout->addWidget(cpp14::make_unique<WText>("+90 242 722 19 68"),0,AlignmentFlag::Middle);
            text1->setAttributeValue(Style::style,Style::color::color(Style::color::White::White)+
                                     Style::Border::border("2px solid white")+
                                     Style::font::size::s16px+
                                     Style::font::weight::bold);
        }
    }

    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto img = layout->addWidget(cpp14::make_unique<WContainerWidget>());
        img->setAttributeValue(Style::style,Style::background::url("img/order.png")+Style::background::repeat::norepeat+Style::background::position::center_center);

        img->setHeight(100);

        auto textContainer = layout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto textLayout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            auto text1 = textLayout->addWidget(cpp14::make_unique<WText>("İstek/Şikayet 'de"),0,AlignmentFlag::Middle);
            text1->setAttributeValue(Style::style,Style::color::color(Style::color::White::White)+
                                     Style::font::size::s18px+Style::font::weight::bold);
        }
        {
            auto text1 = textLayout->addWidget(cpp14::make_unique<WText>("Bulun"),0,AlignmentFlag::Middle);
            text1->setAttributeValue(Style::style,Style::color::color(Style::color::White::White)+
                                     Style::Border::border("2px solid white")+
                                     Style::font::size::s18px+
                                     Style::font::weight::bold);
            text1->decorationStyle().setCursor(Cursor::PointingHand);
        }

        container->clicked().connect([=](){
            this->_mClickTalep.emit(NoClass());
        });
    }

    if( 0 ){ //  Disabled Facebook and Twitter
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);


        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        layout->addStretch(1);
        {
            auto text1 = layout->addWidget(cpp14::make_unique<WText>("Bizi Takip Edin"));
            text1->setAttributeValue(Style::style,Style::color::color(Style::color::White::White)+
                                     Style::font::size::s18px+Style::font::weight::bold);
        }

        auto textContainer = layout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto textLayout = textContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

        // FACEBOOK
        {
            Wt::WLink link = Wt::WLink("https://www.facebook.com/serikbelediye/");
            link.setTarget(Wt::LinkTarget::NewWindow);

            auto text1 = textLayout->addWidget(cpp14::make_unique<WAnchor>(link,"F"),0,AlignmentFlag::Center);
            text1->setAttributeValue(Style::style,Style::Border::border("2px solid AntiqueWhite")+
                                     Style::font::size::s24px+Style::font::weight::bold+
                                     Style::color::color(Style::color::White::AntiqueWhite));
            text1->setMaximumSize(40,40);
            text1->setMinimumSize(40,40);
            text1->decorationStyle().setCursor(Cursor::PointingHand);


        }

        // TWEETER
        {
            Wt::WLink link = Wt::WLink("https://twitter.com/serikbelediyesi");
            link.setTarget(Wt::LinkTarget::NewWindow);

            auto text1 = textLayout->addWidget(cpp14::make_unique<WAnchor>(link,"T"),0,AlignmentFlag::Center);
            text1->setAttributeValue(Style::style,Style::Border::border("2px solid AntiqueWhite")+
                                     Style::font::size::s24px+Style::font::weight::bold+
                                     Style::color::color(Style::color::White::AntiqueWhite));
            text1->setMaximumSize(40,40);
            text1->setMinimumSize(40,40);
            text1->decorationStyle().setCursor(Cursor::PointingHand);
        }

        //        // YOUTUBE
        //        {
        //            auto text1 = textLayout->addWidget(cpp14::make_unique<WText>("Y"),0,AlignmentFlag::Center);
        //            text1->setAttributeValue(Style::style,Style::Border::border("2px solid AntiqueWhite")+
        //                                     Style::font::size::s24px+Style::font::weight::bold+
        //                                     Style::color::color(Style::color::White::AntiqueWhite));
        //            text1->setMaximumSize(40,40);
        //            text1->setMinimumSize(40,40);
        //            text1->decorationStyle().setCursor(Cursor::PointingHand);
        //        }
    }

}

Signal<NoClass> &Body::Iletisim::mClickTalep()
{
    return this->_mClickTalep;
}

void Body::Iletisim::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

//Body::IstatistikAnket::IstatistikAnket()
//{
//    setAttributeValue(Style::style,Style::background::color::color(Style::color::White::AliceBlue));
//    setHeight(350);
//    addWidget(cpp14::make_unique<WContainerWidget>());
//}


Body::Proje::Proje(mongocxx::database *_db)
    :SerikBLDCore::DB(_db)
{
    this->clear();

    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    mMainContainer->setPadding(90,Side::Top);

    auto detailContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    detailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    detailContainer->setPadding(0,AllSides);
    detailContainer->setContentAlignment(AlignmentFlag::Center);

    container = detailContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);
    container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::Snow));

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
        auto serik = layout->addWidget(cpp14::make_unique<WText>("Projeler"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
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
        auto projeListbtn = _layout->addWidget(cpp14::make_unique<WPushButton>("Projeler"),0,AlignmentFlag::Left);
        projeListbtn->clicked().connect([=](){
            this->initPage();
        });
        projeListbtn->addStyleClass(Bootstrap::Button::info);

        _layout->addStretch(1);
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }


    {
        auto projeblock = container->addWidget(cpp14::make_unique<Job::ProjectBlock>(this->getDB()->db(),true));
        projeblock->ClickLastProject().connect(this,&Proje::setselectedProject);

    }

    {
        contentContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        this->initPage();
    }

    {
        mStatisticContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
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
        auto projeListbtn = _layout->addWidget(cpp14::make_unique<WPushButton>("Projeler"),0,AlignmentFlag::Left);
        projeListbtn->clicked().connect([=](){
            this->initPage();
        });
        projeListbtn->addStyleClass(Bootstrap::Button::info);

        auto istatisticbtn = _layout->addWidget(cpp14::make_unique<WPushButton>("İstatistik"),0,AlignmentFlag::Left);
        istatisticbtn->clicked().connect([=](){
            this->initIstatistic();
        });
        istatisticbtn->addStyleClass(Bootstrap::Button::Warning);

        _layout->addStretch(1);
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }
}

Signal<NoClass> &Body::Proje::mGetBack()
{
    return _ClickBack;
}

void Body::Proje::initIstatistic()
{
    contentContainer->clear();
    mStatisticContainer->clear();

    {
        auto fluidContainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto row  = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        row->addStyleClass(Bootstrap::Grid::row);

        // Mahallelere Gore
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3);

            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.85));

            container->setPadding(5,AllSides);
            container->decorationStyle().setCursor(Cursor::PointingHand);

            auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
            _container->setHeight(150);

            auto vlayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = vlayout->addWidget(cpp14::make_unique<WText>("Mahallelere Göre Dağılım"),0,AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s16px+Style::font::weight::bold);

            container->clicked().connect([=](){
                this->initMahallelereGoreDagilim();
            });
        }

        // Birimlere Gore
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3);

            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.85));

            container->setPadding(5,AllSides);
            container->decorationStyle().setCursor(Cursor::PointingHand);

            auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
            _container->setHeight(150);

            auto vlayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = vlayout->addWidget(cpp14::make_unique<WText>("Birimlere Göre Dağılım"),0,AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s16px+Style::font::weight::bold);

            container->clicked().connect([=](){
                this->initBirimlereGoreDagilim();
            });
        }

        // Yıllara Gore
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3);

            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.85));

            container->setPadding(5,AllSides);
            container->decorationStyle().setCursor(Cursor::PointingHand);

            auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
            _container->setHeight(150);

            auto vlayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = vlayout->addWidget(cpp14::make_unique<WText>("Yıllara Göre Dağılım"),0,AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s16px+Style::font::weight::bold);

            container->clicked().connect([=](){
                this->initYillaraGoreDagilim();
            });
        }

        // Duruma Gore
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3);

            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.85));

            container->setPadding(5,AllSides);
            container->decorationStyle().setCursor(Cursor::PointingHand);

            auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
            _container->setHeight(150);

            auto vlayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = vlayout->addWidget(cpp14::make_unique<WText>("Duruma Göre Dağılım"),0,AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s16px+Style::font::weight::bold);

            container->clicked().connect([=](){
                this->initDurumaGoreDagilim();
            });
        }

    }
}

void Body::Proje::initMahallelereGoreDagilim()
{

    mStatisticContainer->clear();

    auto fluidContainer = mStatisticContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rowCOntainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowCOntainer->addStyleClass(Bootstrap::Grid::row);

    {
        auto titleCOntainer = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        titleCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto title = rowCOntainer->addWidget(cpp14::make_unique<WText>("<h2>Projelerin Mahallelere Dağılımı</h2>"));
    }

    auto container = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    auto model = std::make_shared<WStandardItemModel>();
    model->setItemPrototype(cpp14::make_unique<NumericItem>());

    auto filter = document{};

    auto view = filter.view();

    // Configure the header.
    model->insertColumns(model->columnCount(), 2);
    model->setHeaderData(0, WString("Mahalleler"));
    model->setHeaderData(1, WString("Proje Sayısı"));

    int projeCount = this->db()->collection(SBLDKeys::Mahalle::collection).count_documents(view);

    std::vector<std::string> mahalleList;

    try {
        auto cursor = this->db()->collection(SBLDKeys::Mahalle::collection).find(filter.view());
        for( auto doc : cursor )
        {
            if( doc[SBLDKeys::Mahalle::mahalle].get_string().value.data() != "NULL" )
            {
                mahalleList.push_back(doc[SBLDKeys::Mahalle::mahalle].get_string().value.data());
            }
        }
    } catch (mongocxx::exception &e) {
        std::cout << "Errror: " << e.what() << std::endl;
    }

    model->insertRows(model->rowCount(), mahalleList.size());
    int row = 0;

    for( auto mahalle : mahalleList )
    {

        auto _filter = document{};

        try {
            _filter.append(kvp(SBLDKeys::Projeler::online,SBLDKeys::Projeler::onlineSet::online));
        } catch (bsoncxx::exception &e) {

        }

        try {
            _filter.append(kvp(SBLDKeys::Projeler::mahalle,mahalle));
        } catch (bsoncxx::exception &e) {

        }

        auto count = this->db()->collection(SBLDKeys::Projeler::collection).count_documents(view);

        model->setData(  row, 0, WString(mahalle));
        model->setData(  row, 1, count);
        row++;
    }


    auto tableCOntainer = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    tableCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    auto hLayout = tableCOntainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    WTableView* table = hLayout->addWidget(cpp14::make_unique<WTableView>(),0,AlignmentFlag::Justify);


    table->setMargin(10, Side::Top | Side::Bottom);
    table->setMargin(WLength::Auto, Side::Left | Side::Right);
    table->setSortingEnabled(true);
    table->setModel(model);
    table->setRowHeight(28);
    table->setHeaderHeight(28);
    table->setWidth(WLength::Auto);

    if (WApplication::instance()->environment().ajax())
        table->setEditTriggers(EditTrigger::SingleClicked);
    else
        table->setEditTriggers(EditTrigger::None);

    /*
     * Create the pie chart.
     */
    Chart::WPieChart *chart =  container->addWidget(cpp14::make_unique<Chart::WPieChart>());
    chart->setModel(model);       // Set the model.
    chart->setLabelsColumn(0);    // Set the column that holds the labels.
    chart->setDataColumn(1);      // Set the column that holds the data.

    // Configure location and type of labels.
    chart->setDisplayLabels(Chart::LabelOption::Outside |
                            Chart::LabelOption::TextLabel |
                            Chart::LabelOption::TextPercentage);

    // Enable a 3D and shadow effect.
    chart->setPerspectiveEnabled(true, 0.20);
    chart->setShadowEnabled(true);

    chart->setExplode(0, 0.3);  // Explode the first item.
    chart->resize(1200 , 400);    // WPaintedWidget must be given an explicit size.
    chart->setMargin(10, Side::Top | Side::Bottom); // Add margin vertically.
    chart->setMargin(WLength::Auto, Side::Left | Side::Right); // Center horizontally.

}

void Body::Proje::initBirimlereGoreDagilim()
{
    mStatisticContainer->clear();

    auto fluidContainer = mStatisticContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rowCOntainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowCOntainer->addStyleClass(Bootstrap::Grid::row);

    {
        auto titleCOntainer = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        titleCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        rowCOntainer->addWidget(cpp14::make_unique<WText>("<h2>Projelerin Birimlere Dağılımı</h2>"));
    }

    auto container = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    auto model = std::make_shared<WStandardItemModel>();
    model->setItemPrototype(cpp14::make_unique<NumericItem>());

    // Configure the header.
    model->insertColumns(model->columnCount(), 2);
    model->setHeaderData(0, WString("Birimler"));
    model->setHeaderData(1, WString("Proje Sayısı"));

    auto filter = document{};

    std::vector<std::string> birimList;

    try {
        filter.append(kvp(SBLDKeys::Birimler::haberlesmekodu,make_document(kvp("$ne","0"))));
    } catch (bsoncxx::exception &e) {

    }

    try {
        auto cursor = this->db()->collection(SBLDKeys::Birimler::collection).find(filter.view());
        for( auto doc : cursor )
        {
            if( doc[SBLDKeys::Birimler::birim].get_string().value.data() != "NULL" )
            {
                birimList.push_back(doc[SBLDKeys::Birimler::birim].get_string().value.data());
            }
        }
    } catch (mongocxx::exception &e) {
        std::cout << "Errror: " << e.what() << std::endl;
    }

    model->insertRows(model->rowCount(), birimList.size());
    int row = 0;




    for( auto birim : birimList )
    {

        auto _filter = document{};

        try {
            _filter.append(kvp(SBLDKeys::Projeler::online,SBLDKeys::Projeler::onlineSet::online));
        } catch (bsoncxx::exception &e) {

        }

        try {
            _filter.append(kvp(SBLDKeys::Projeler::birim,birim));
        } catch (bsoncxx::exception &e) {

        }

        auto _view = _filter.view();

        auto count = this->db()->collection(SBLDKeys::Projeler::collection).count_documents(_view);

        model->setData(  row, 0, WString(birim));
        model->setData(  row, 1, count);
        row++;
    }

    auto tableCOntainer = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    tableCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    auto hLayout = tableCOntainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    WTableView* table = hLayout->addWidget(cpp14::make_unique<WTableView>(),0,AlignmentFlag::Center);


    table->setMargin(10, Side::Top | Side::Bottom);
    table->setMargin(WLength::Auto, Side::Left | Side::Right);
    table->setSortingEnabled(true);
    table->setModel(model);
    //    table->setColumnWidth(1, 100);
    table->setRowHeight(28);
    table->setHeaderHeight(28);
    //    table->setWidth(150 + 100 + 14 + 2);



    /*
     * Create the pie chart.
     */
    Chart::WPieChart *chart =
            container->addWidget(cpp14::make_unique<Chart::WPieChart>());
    chart->setModel(model);       // Set the model.
    chart->setLabelsColumn(0);    // Set the column that holds the labels.
    chart->setDataColumn(1);      // Set the column that holds the data.

    // Configure location and type of labels.
    chart->setDisplayLabels(Chart::LabelOption::Outside |
                            Chart::LabelOption::TextLabel |
                            Chart::LabelOption::TextPercentage);

    // Enable a 3D and shadow effect.
    chart->setPerspectiveEnabled(true, 0.2);
    chart->setShadowEnabled(true);

    chart->setExplode(0, 0.3);  // Explode the first item.
    chart->resize(1200 , 400);    // WPaintedWidget must be given an explicit size.
    chart->setMargin(10, Side::Top | Side::Bottom); // Add margin vertically.
    chart->setMargin(WLength::Auto, Side::Left | Side::Right); // Center horizontally.
}

void Body::Proje::initYillaraGoreDagilim()
{
    mStatisticContainer->clear();

    auto fluidContainer = mStatisticContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rowCOntainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowCOntainer->addStyleClass(Bootstrap::Grid::row);

    {
        auto titleCOntainer = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        titleCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto title = rowCOntainer->addWidget(cpp14::make_unique<WText>("<h2>Projelerin Yıllara Dağılımı</h2>"));
    }

    auto container = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    auto model = std::make_shared<WStandardItemModel>();
    model->setItemPrototype(cpp14::make_unique<NumericItem>());

    // Configure the header.
    model->insertColumns(model->columnCount(), 2);
    model->setHeaderData(0, WString("Yıl"));
    model->setHeaderData(1, WString("Proje Sayısı"));

    auto filter = document{};
    auto view = filter.view();

    std::vector<std::int32_t> yilList;

    yilList.push_back(2014);
    yilList.push_back(2015);
    yilList.push_back(2016);
    yilList.push_back(2017);
    yilList.push_back(2018);
    yilList.push_back(2019);


    model->insertRows(model->rowCount(), yilList.size());
    int row = 0;


    for( auto yil : yilList )
    {

        auto _filter = document{};

        try {
            _filter.append(kvp(SBLDKeys::Projeler::online,SBLDKeys::Projeler::onlineSet::online));
        } catch (bsoncxx::exception &e) {

        }

        try {
            _filter.append(kvp(SBLDKeys::Projeler::year,yil));
        } catch (bsoncxx::exception &e) {

        }

        auto _view = _filter.view();

        auto count = this->db()->collection(SBLDKeys::Projeler::collection).count_documents(_view);

        model->setData(  row, 0, WString("{1}").arg(yil));
        model->setData(  row, 1, count);
        row++;
    }

    auto tableCOntainer = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    tableCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    auto hLayout = tableCOntainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    WTableView* table = hLayout->addWidget(cpp14::make_unique<WTableView>(),0,AlignmentFlag::Center);


    table->setMargin(10, Side::Top | Side::Bottom);
    table->setMargin(WLength::Auto, Side::Left | Side::Right);
    table->setSortingEnabled(true);
    table->setModel(model);
    table->setRowHeight(28);
    table->setHeaderHeight(28);


    /*
     * Create the pie chart.
     */
    Chart::WPieChart *chart =
            container->addWidget(cpp14::make_unique<Chart::WPieChart>());
    chart->setModel(model);       // Set the model.
    chart->setLabelsColumn(0);    // Set the column that holds the labels.
    chart->setDataColumn(1);      // Set the column that holds the data.

    // Configure location and type of labels.
    chart->setDisplayLabels(Chart::LabelOption::Outside |
                            Chart::LabelOption::TextLabel |
                            Chart::LabelOption::TextPercentage);

    // Enable a 3D and shadow effect.
    chart->setPerspectiveEnabled(true, 0.2);
    chart->setShadowEnabled(true);

    chart->setExplode(0, 0.3);  // Explode the first item.
    chart->resize(1200 , 400);    // WPaintedWidget must be given an explicit size.
    chart->setMargin(10, Side::Top | Side::Bottom); // Add margin vertically.
    chart->setMargin(WLength::Auto, Side::Left | Side::Right); // Center horizontally.
}

void Body::Proje::initDurumaGoreDagilim()
{
    mStatisticContainer->clear();

    auto fluidContainer = mStatisticContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rowCOntainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowCOntainer->addStyleClass(Bootstrap::Grid::row);

    {
        auto titleCOntainer = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        titleCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto title = rowCOntainer->addWidget(cpp14::make_unique<WText>("<h2>Projelerin Duruma Göre Dağılımı</h2>"));
    }

    auto container = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    auto model = std::make_shared<WStandardItemModel>();
    model->setItemPrototype(cpp14::make_unique<NumericItem>());

    // Configure the header.
    model->insertColumns(model->columnCount(), 2);
    model->setHeaderData(0, WString("Durum"));
    model->setHeaderData(1, WString("Proje Sayısı"));


    auto filter = document{};
    auto view = filter.view();

    std::vector<std::string> durumList;

    durumList.push_back(SBLDKeys::Projeler::DURUM::tamamlandi);
    durumList.push_back(SBLDKeys::Projeler::DURUM::yapilacak);
    durumList.push_back(SBLDKeys::Projeler::DURUM::devamediyor);



    model->insertRows(model->rowCount(), durumList.size());
    int row = 0;




    for( auto durum : durumList )
    {

        auto _filter = document{};

        try {
            _filter.append(kvp(SBLDKeys::Projeler::online,SBLDKeys::Projeler::onlineSet::online));
        } catch (bsoncxx::exception &e) {

        }

        try {
            _filter.append(kvp(SBLDKeys::Projeler::durum,durum));
        } catch (bsoncxx::exception &e) {

        }

        auto _view = _filter.view();

        auto count = this->db()->collection(SBLDKeys::Projeler::collection).count_documents(_view);

        model->setData(  row, 0, WString("{1}").arg(durum));
        model->setData(  row, 1, count);
        row++;
    }

    auto tableCOntainer = rowCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    tableCOntainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    auto hLayout = tableCOntainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    WTableView* table = hLayout->addWidget(cpp14::make_unique<WTableView>(),0,AlignmentFlag::Center);


    table->setMargin(10, Side::Top | Side::Bottom);
    table->setMargin(WLength::Auto, Side::Left | Side::Right);
    table->setSortingEnabled(true);
    table->setModel(model);
    table->setRowHeight(28);
    table->setHeaderHeight(28);



    /*
     * Create the pie chart.
     */
    Chart::WPieChart *chart =
            container->addWidget(cpp14::make_unique<Chart::WPieChart>());
    chart->setModel(model);       // Set the model.
    chart->setLabelsColumn(0);    // Set the column that holds the labels.
    chart->setDataColumn(1);      // Set the column that holds the data.

    // Configure location and type of labels.
    chart->setDisplayLabels(Chart::LabelOption::Outside |
                            Chart::LabelOption::TextLabel |
                            Chart::LabelOption::TextPercentage);

    // Enable a 3D and shadow effect.
    chart->setPerspectiveEnabled(true, 0.2);
    chart->setShadowEnabled(true);

    chart->setExplode(0, 0.3);  // Explode the first item.
    chart->resize(1200 , 400);    // WPaintedWidget must be given an explicit size.
    chart->setMargin(10, Side::Top | Side::Bottom); // Add margin vertically.
    chart->setMargin(WLength::Auto, Side::Left | Side::Right); // Center horizontally.
}



void Body::Proje::initPage()
{

    contentContainer->clear();


    auto rowContainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowContainer->addStyleClass(Bootstrap::Grid::row);
    //    rowContainer->setMargin(5,AllSides);

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Projeler::online,SBLDKeys::Projeler::onlineSet::online));
    } catch (bsoncxx::exception &e) {
        setstatus(e);
        return;
    }


//    auto collection = this->getDB()->collection(SBLDKeys::Projeler::collection);
//    auto bucket = this->bucket();

    try {

        auto cursor = this->db()->collection(SBLDKeys::Projeler::collection).find(filter.view());

        list.clear();

        for( auto doc : cursor )
        {
            item _item;
            _item.oid = doc[SBLDKeys::Projeler::oid].get_oid().value.to_string();
            _item.title = doc[SBLDKeys::Projeler::title].get_string().value.data();
//            _item.iconPath = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Projeler::icon].get_oid().value.to_string());
            _item.iconPath = this->downloadFileWeb(doc[SBLDKeys::Projeler::icon].get_oid().value.to_string().c_str());
            list.push_back(_item);
        }


        for( item _item : list )
        {
            addItem(rowContainer,_item);
        }


    } catch (mongocxx::exception &e) {
        setstatus(e);
        return;
    }

    initListinProje = true;

}

void Body::Proje::setstatus(bsoncxx::exception &e)
{
    setstatus(e.what());
}

void Body::Proje::setstatus(mongocxx::exception &e)
{
    setstatus(e.what());
}

void Body::Proje::setstatus(std::string e)
{
    contentContainer->clear();
    auto text = contentContainer->addWidget(cpp14::make_unique<WText>(e.c_str()));
    text->setAttributeValue(Style::style,Style::font::size::s16px);
}

void Body::Proje::addItem(WContainerWidget *widget, item _item , bool OneColoum )
{

    if( OneColoum )
    {
        auto __container = widget->addWidget(cpp14::make_unique<WContainerWidget>());
        __container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);


        auto container = __container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::url(_item.iconPath)+Style::background::repeat::norepeat+Style::background::size::cover);
        container->setMargin(5,AllSides);
        container->setPadding(0,AllSides);

        auto grayContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        grayContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,120),this->getRandom(0,120),this->getRandom(0,120),0.75));
        grayContainer->setMargin(0,AllSides);
        grayContainer->setId("GRAYID");
        grayContainer->setPadding(15,AllSides);

        auto layoutContainer = grayContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        layoutContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        layoutContainer->setHeight(75);
        auto layout = layoutContainer->setLayout(cpp14::make_unique<WVBoxLayout>());



        auto text = layout->addWidget(cpp14::make_unique<WText>(_item.title),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        container->decorationStyle().setCursor(Cursor::PointingHand);

        container->clicked().connect([=](){
            setselectedProject(_item.oid);
        });
    }else{
        auto __container = widget->addWidget(cpp14::make_unique<WContainerWidget>());
        __container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);


        auto container = __container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::url(_item.iconPath)+Style::background::repeat::norepeat+Style::background::size::cover);
        container->setMargin(5,AllSides);
        container->setPadding(0,AllSides);

        auto grayContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        grayContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,120),this->getRandom(0,120),this->getRandom(0,120),0.75));
        grayContainer->setMargin(0,AllSides);
        grayContainer->setId("GRAYID");
        grayContainer->setPadding(15,AllSides);

        auto layoutContainer = grayContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        layoutContainer->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        layoutContainer->setHeight(150);
        auto layout = layoutContainer->setLayout(cpp14::make_unique<WVBoxLayout>());



        auto text = layout->addWidget(cpp14::make_unique<WText>(_item.title),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        container->decorationStyle().setCursor(Cursor::PointingHand);

        container->clicked().connect([=](){
            setselectedProject(_item.oid);
        });
    }


}

void Body::Proje::setselectedProject(std::string oid)
{

    if( initListinProje )
    {
        contentContainer->clear();

        auto _fluidCOntainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _fluidCOntainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto _rowContainer = _fluidCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _rowContainer->addStyleClass(Bootstrap::Grid::row);

        {
            auto _container = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_3);

            auto fluidContainer_ = _container->addWidget(cpp14::make_unique<WContainerWidget>());
            fluidContainer_->addStyleClass(Bootstrap::Grid::container_fluid);

            auto rowCOntainer_ = fluidContainer_->addWidget(cpp14::make_unique<WContainerWidget>());
            rowCOntainer_->addStyleClass(Bootstrap::Grid::row);

            {
                auto text = rowCOntainer_->addWidget(cpp14::make_unique<WText>("<h4>Projeler</h4>"));
            }
            //            rowCOntainer_->setMaximumSize(WLength::Auto,950);
            //            rowCOntainer_->setOverflow(Overflow::Scroll);

            for( item _item : list )
            {
                addItem(rowCOntainer_,_item , true);
            }
        }

        ProjectDetailContainer = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        ProjectDetailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_9);

        initListinProje = false;
    }







    ProjectDetailContainer->clear();



    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        ProjectDetailContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
        return;
    }

    bsoncxx::document::value val(document{}.view());

    try {
        auto val_ = this->getDB()->db()->collection(SBLDKeys::Projeler::collection).find_one(filter.view());
        if( val_ )
        {
            if( !val_.value().view().empty() )
            {
                val = val_.value();
            }else{
                ProjectDetailContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("Document Empty")));
            }
        }else{
            ProjectDetailContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("No Document Returned")));
        }
    } catch (mongocxx::exception &e) {
        ProjectDetailContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
    }

    auto view = val.view();

    auto TitleContainer = ProjectDetailContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto TitleContainerColor = TitleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    TitleContainerColor->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(oid.c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(view[SBLDKeys::Projeler::title].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Başlama Tarihi {1}").arg(WDate::fromJulianDay(view[SBLDKeys::Projeler::julianDate].get_int64().value).toString("yyyy").toUTF8().c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Durum {1}").arg(view[SBLDKeys::Projeler::durum].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Birim {1}").arg(view[SBLDKeys::Projeler::birim].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto text = TitleContainerColor->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(view[SBLDKeys::Projeler::mahalle].get_string().value.data())));
        text->addStyleClass(Bootstrap::Label::Warning+Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        std::string iconPaht = this->downloadFileWeb(view[SBLDKeys::Projeler::icon].get_oid().value.to_string().c_str());
        TitleContainer->setAttributeValue(Style::style,Style::background::url(iconPaht)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setHeight(50);
    }


    {

        //        mMainContainer->setPadding(5,Side::Top);
        auto asamaContainer = ProjectDetailContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//        auto bucket = this->getDB()->gridfs_bucket();
        try {
            auto asamaList = view[SBLDKeys::Projeler::slide].get_array().value;

            int topIndex = 1;
            for( auto _asama : asamaList )
            {
                auto doc = _asama.get_document().value;
                auto container = asamaContainer->addWidget(cpp14::make_unique<WContainerWidget>());


                auto slideCOntainer = cpp14::make_unique<WContainerWidget>();
                slideCOntainer->setHeight(350);


                std::string imgPath = this->downloadFileWeb(doc[SBLDKeys::Projeler::slideItem::img].get_oid().value.to_string().c_str());

                auto frontimgContainer = slideCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                frontimgContainer->setHeight(350);
                frontimgContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::contain+Style::background::position::center_center+Style::background::repeat::norepeat);

                auto imgContainer = cpp14::make_unique<WContainerWidget>();
                imgContainer->setHeight(350);
                imgContainer->setWidth(WLength("100%"));
                imgContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::contain+Style::background::position::center_center+"z-index:-1; ");
                imgContainer->addStyleClass("CommanFilter");
                imgContainer->setPositionScheme(PositionScheme::Absolute);
                //                imgContainer->setMargin(0,Side::Top|Side::Left);
                imgContainer->setOffsets(350*topIndex,Side::Top); topIndex++;
                slideCOntainer->addWidget(std::move(imgContainer));

                container->addWidget(std::move(slideCOntainer));

                auto TextCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                TextCOntainer->setPadding(15,Side::Bottom|Side::Top);
                TextCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.75));
                auto aciklamaText = TextCOntainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Projeler::slideItem::text].get_string().value.data())));
                aciklamaText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold+Style::font::size::s16px);
            }
        } catch (bsoncxx::exception &e) {

        }

    }

}

Body::Haber::Haber(mongocxx::database *_db)
    :WContainerWidget(),
      db(_db)
{

    skip = 0;
    count = 0;

    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

//    mMainContainer->setMargin(90,Side::Top);
    mMainContainer->setPadding(90,Side::Top);

    auto detailContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    detailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    detailContainer->setPadding(0,AllSides);
    detailContainer->setContentAlignment(AlignmentFlag::Center);

    container = detailContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->setContentAlignment(AlignmentFlag::Center);
    container->setMaximumSize(1024,WLength::Auto);
    container->setPadding(0,AllSides);
    container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::Snow));

    {
        std::string path = "img/baskanBackGround.JPG";

        auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
        img->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        img->setHeight(100);
        img->setPadding(0,AllSides);

        auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
        gradientContainer->setHeight(100);
        gradientContainer->addStyleClass("SliderDetailTextBackground");
        auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        layout->addStretch(1);
        auto serik = layout->addWidget(cpp14::make_unique<WText>("Haberler"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
        serik->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
    }


    {
        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }

    {
        contentContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        contentContainer->addStyleClass(Bootstrap::Grid::container_fluid);
        this->initPage();
    }

    {
        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
            if( skip > limit )
            {
                skip -= limit;
                this->initPage();
            }

        });

        auto collection = db->collection(SBLDKeys::Haberler::collection);

        try {
            count = collection.count_documents(document{}.view());
        } catch (mongocxx::exception &e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

        for( std::int64_t i = 0 ; i <= count/limit ; i++ )
        {
            auto btn = _layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(i)),0,AlignmentFlag::Left);
            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->setMargin(10,Side::Left|Side::Right);
            btn->clicked().connect([=](){
                if( i == count/limit - 1 )
                {
                    skip = i * limit;
                }else{
                    skip = i * limit;
                }
                this->initPage();
            });
        }
        _layout->addWidget(cpp14::make_unique<WPushButton>("İleri"),0,AlignmentFlag::Left)->clicked().connect([=](){
            if( skip < count - limit )
            {
                skip += limit;
                this->initPage();
            }
        });

        _layout->addStretch(1);

        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }
}

Signal<NoClass> &Body::Haber::mGetBack()
{
    return this->_ClickBack;
}

Signal<std::string> &Body::Haber::mGetHaber()
{
    return _ClickHaber;
}

void Body::Haber::initPage()
{

    contentContainer->clear();


    auto rowContainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowContainer->addStyleClass(Bootstrap::Grid::row);

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Haberler::Yayinda,SBLDKeys::Haberler::Yayin::Yayinda));
    } catch (bsoncxx::exception &e) {
        setstatus(e);
        return;
    }

    mongocxx::options::find findOption;

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp(SBLDKeys::Haberler::oid,-1));
    } catch (bsoncxx::exception &e) {
        return;
    }

    findOption.sort(sortDoc.view());
    findOption.limit(limit);
    findOption.skip(skip);

    auto collection = db->collection(SBLDKeys::Haberler::collection);
    auto bucket = db->gridfs_bucket();

    try {
        auto cursor = collection.find(filter.view(),findOption);
        list.clear();
        for( auto doc : cursor )
        {
            item _item;
            _item.oid = doc[SBLDKeys::Haberler::oid].get_oid().value.to_string();
            _item.title = doc[SBLDKeys::Haberler::Baslik].get_string().value.data();
            _item.iconPath = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Haberler::icon].get_oid().value.to_string());
            list.push_back(_item);
        }

        for( item _item : list )
        {
            addItem(rowContainer,_item);
        }

    } catch (mongocxx::exception &e) {
        setstatus(e);
        return;
    }

}

void Body::Haber::setstatus(bsoncxx::exception &e)
{
    setstatus(e.what());
}

void Body::Haber::setstatus(mongocxx::exception &e)
{
    setstatus(e.what());
}

void Body::Haber::setstatus(std::string e)
{
    contentContainer->clear();
    auto text = contentContainer->addWidget(cpp14::make_unique<WText>(e.c_str()));
    text->setAttributeValue(Style::style,Style::font::size::s16px);
}

void Body::Haber::addItem(WContainerWidget *widget, item _item)
{
    auto _container = widget->addWidget(cpp14::make_unique<WContainerWidget>());
    _container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    _container->setContentAlignment(AlignmentFlag::Center);
    _container->setMargin(25,Side::Bottom);

    auto thumpcontainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());
    thumpcontainer->setHeight(250);
    thumpcontainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);


    auto layout = thumpcontainer->setLayout(cpp14::make_unique<WVBoxLayout>());
    auto img = layout->addWidget(cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
    img->setAttributeValue(Style::style,Style::background::url(_item.iconPath)+Style::background::repeat::norepeat+Style::background::size::cover);
    img->addStyleClass(Bootstrap::ImageShape::img_circle);
    img->setHeight(150);


    auto text = layout->addWidget(cpp14::make_unique<WText>(_item.title),0,AlignmentFlag::Bottom);
    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
    thumpcontainer->decorationStyle().setCursor(Cursor::PointingHand);

    thumpcontainer->clicked().connect([=](){
        this->setNews(_item.oid);
    });
}

void Body::Haber::setNews(std::string oid)
{
    mMainContainer->clear();
    mMainContainer->setContentAlignment(AlignmentFlag::Center);
    auto container = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->setPadding(0,AllSides);

    container->setMaximumSize(1024,WLength::Auto);
    container->setPadding(0,AllSides);

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Haberler::Yayinda,SBLDKeys::Haberler::Yayin::Yayinda));
        filter.append(kvp(SBLDKeys::Haberler::oid,bsoncxx::oid{oid}));

    } catch (bsoncxx::exception &e) {
        container->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }


    auto update = document{};
    try {
        update.append(kvp("$inc",make_document(kvp(SBLDKeys::Haberler::OkunmaSayisi,bsoncxx::types::b_double{1}))));
    } catch (bsoncxx::exception &e) {
        std::cout << __LINE__ << " " << __FUNCTION__ << "\n";
    }



    auto collection = db->collection(SBLDKeys::Haberler::collection);
    auto bucket = db->gridfs_bucket();

    try {
        mongocxx::stdx::optional<bsoncxx::document::value> val = collection.find_one_and_update(filter.view(),update.view());
        if( !val )
        {
            container->addWidget(cpp14::make_unique<WText>(WString("Empty Document")));
            return;
        }else{

            auto view = val.value().view();

            {
                auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
                img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                std::string path = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::Haberler::icon].get_oid().value.to_string());
                img->setHeight(150);
                img->setPadding(0,AllSides);

                auto backimg = img->addWidget(cpp14::make_unique<WContainerWidget>());
                backimg->setPositionScheme(PositionScheme::Absolute);
                backimg->setHeight(WLength("100%"));
                backimg->setWidth(WLength("100%"));

                backimg->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center+"z-index:  -99;");
                backimg->addStyleClass("CommanFilter");

                auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
                gradientContainer->setHeight(150);
                gradientContainer->addStyleClass("SliderDetailTextBackground");
                auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addStretch(1);
                auto title = layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Haberler::Baslik].get_string().value.data()),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
                title->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
                title->addStyleClass("NewsMainTitle");
            }

            {
                auto array = view[SBLDKeys::Haberler::fileOidList].get_array().value;
                for( auto doc : array )
                {
                    std::string path = SBLDKeys::downloadifNotExist(&bucket,doc.get_oid().value.to_string(),true);
                }
            }

            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                _container->setLayout(cpp14::make_unique<WHBoxLayout>());
                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }

            {
                auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                textContainer->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                                 Style::background::color::color(Style::color::White::Snow));

                auto layout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Haberler::html].get_string().value.data(),TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
            }

            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                _container->setLayout(cpp14::make_unique<WHBoxLayout>());

                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }

        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }

}

Body::Etkinlik::Etkinlik(mongocxx::database *_db)
    :WContainerWidget(),
      db(_db)
{

    skip = 0;
    count = 0;

    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    mMainContainer->setMargin(90,Side::Top);


    auto detailContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    detailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    detailContainer->setPadding(0,AllSides);
    detailContainer->setContentAlignment(AlignmentFlag::Center);


    container = detailContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->setContentAlignment(AlignmentFlag::Center);
    container->setMaximumSize(1250,WLength::Auto);
    container->setPadding(0,AllSides);
    container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::Snow));



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
        auto serik = layout->addWidget(cpp14::make_unique<WText>("Etkinlikler"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
        serik->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
    }


    {
        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setId("_containerID");
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
                _layout->addWidget(cpp14::make_unique<WPushButton>("Etkinlik Anasayfa"),0,AlignmentFlag::Left)->clicked().connect([=](){
//                        this->_ClickBack.emit(NoClass());
                    this->initPage();
                });
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }




    {
        contentContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        contentContainer->addStyleClass(Bootstrap::Grid::container_fluid);
        this->initPage();
    }






    {
        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
            if( skip > limit )
            {
                skip -= limit;
                this->initPage();
            }

        });

        auto collection = db->collection(SBLDKeys::Etkinlik::collection);

        try {
            count = collection.count_documents(document{}.view());
        } catch (mongocxx::exception &e) {
            std::cout << "Error: " << e.what() << std::endl;
        }

        //        _layout->addWidget(cpp14::make_unique<WText>("Sayfalar"),0,AlignmentFlag::Left);

        for( std::int64_t i = 0 ; i <= count/limit ; i++ )
        {
            auto btn = _layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(i)),0,AlignmentFlag::Left);
            btn->decorationStyle().setCursor(Cursor::PointingHand);
            btn->setMargin(10,Side::Left|Side::Right);
            btn->clicked().connect([=](){
                if( i == count/limit - 1 )
                {
                    skip = i * limit;
                }else{
                    skip = i * limit;
                }
                this->initPage();
            });
        }
        _layout->addWidget(cpp14::make_unique<WPushButton>("İleri"),0,AlignmentFlag::Left)->clicked().connect([=](){
            if( skip < count - limit )
            {
                skip += limit;
                this->initPage();
            }

        });

        _layout->addStretch(1);

        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }
}

Signal<NoClass> &Body::Etkinlik::mGetBack()
{
    return this->_ClickBack;
}

Signal<std::string> &Body::Etkinlik::mGetEtkinlik()
{
    return _ClickEtkinlik;
}

void Body::Etkinlik::initPage()
{

    contentContainer->clear();


    auto rowContainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowContainer->addStyleClass(Bootstrap::Grid::row);
    //    rowContainer->setContentAlignment(AlignmentFlag::Center);
    //    rowContainer->setAttributeValue(Style::style,Style::Border::border("1px solid red"));


    auto filter = document{};

    try {
        //        filter.append(kvp(SBLDKeys::Haberler::Yayinda,SBLDKeys::Haberler::Yayin::Yayinda));
    } catch (bsoncxx::exception &e) {
        setstatus(e);
        return;
    }



    mongocxx::options::find findOption;

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp(SBLDKeys::Etkinlik::oid,-1));
    } catch (bsoncxx::exception &e) {
        return;
    }

    findOption.sort(sortDoc.view());
    findOption.limit(limit);
    findOption.skip(skip);


    auto collection = db->collection(SBLDKeys::Etkinlik::collection);
    auto bucket = db->gridfs_bucket();

    try {

        auto cursor = collection.find(filter.view(),findOption);

        list.clear();

        for( auto doc : cursor )
        {
            item _item;
            _item.oid = doc[SBLDKeys::Etkinlik::oid].get_oid().value.to_string();
            _item.title = doc[SBLDKeys::Etkinlik::title].get_string().value.data();
            _item.iconPath = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Etkinlik::iconFile].get_oid().value.to_string());
            list.push_back(_item);
        }


        for( item _item : list )
        {
            addItem(rowContainer,_item);
        }


    } catch (mongocxx::exception &e) {
        setstatus(e);
        return;
    }
}

void Body::Etkinlik::setstatus(bsoncxx::exception &e)
{
    setstatus(e.what());
}

void Body::Etkinlik::setstatus(mongocxx::exception &e)
{
    setstatus(e.what());
}

void Body::Etkinlik::setstatus(std::string e)
{
    contentContainer->clear();
    auto text = contentContainer->addWidget(cpp14::make_unique<WText>(e.c_str()));
    text->setAttributeValue(Style::style,Style::font::size::s16px);
}

void Body::Etkinlik::addItem(WContainerWidget *widget, item _item)
{
    auto container = widget->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3);
    container->setHeight(250);

    auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
    _container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

    auto img = layout->addWidget(cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
    img->setAttributeValue(Style::style,Style::background::url(_item.iconPath)+Style::background::repeat::norepeat+Style::background::size::cover);
    img->addStyleClass(Bootstrap::ImageShape::img_rounded);

    auto text = layout->addWidget(cpp14::make_unique<WText>(_item.title),0,AlignmentFlag::Bottom);
    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
    container->decorationStyle().setCursor(Cursor::PointingHand);

    container->clicked().connect([=](){
        this->setEtkinlik(_item.oid);
    });
}

void Body::Etkinlik::setEtkinlik(std::string oid)
{

    contentContainer->clear();

    auto detailContainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    detailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    detailContainer->setPadding(0,AllSides);

    auto layout = detailContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    layout->setContentsMargins(0,0,0,0);
    auto container = layout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Center);
    container->setMaximumSize(1024,WLength::Auto);
    container->setPadding(0,AllSides);

    auto filter = document{};


    try {
        //        filter.append(kvp(SBLDKeys::Haberler::Yayinda,SBLDKeys::Haberler::Yayin::Yayinda));
        filter.append(kvp(SBLDKeys::Haberler::oid,bsoncxx::oid{oid}));

    } catch (bsoncxx::exception &e) {
        container->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }

    auto collection = db->collection(SBLDKeys::Etkinlik::collection);
    auto bucket = db->gridfs_bucket();

    try {
        mongocxx::stdx::optional<bsoncxx::document::value> val = collection.find_one(filter.view());
        if( !val )
        {
            container->addWidget(cpp14::make_unique<WText>(WString("Empty Document")));
            return;
        }else{

            auto view = val.value().view();

            {
                auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
                img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                std::string path = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::Etkinlik::iconFile].get_oid().value.to_string());

                //                std::cout << "OUT PATH: " << path << std::endl;
                img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
                img->setHeight(300);
                img->setPadding(0,AllSides);

                auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
                gradientContainer->setHeight(300);
                gradientContainer->addStyleClass("SliderDetailTextBackground");
                auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                layout->addStretch(1);
                auto title = layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Etkinlik::title].get_string().value.data()),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
                title->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
            }

            {
                try {
                    auto array = view[SBLDKeys::Etkinlik::fileList].get_array().value;
                    for( auto doc : array )
                    {
                        SBLDKeys::downloadifNotExist(&bucket,doc.get_oid().value.to_string());
                    }
                } catch (bsoncxx::exception &e) {
                    std::cout << "ERROR: " << e.what() << std::endl;
                }

            }

            {
                auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                textContainer->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                                 Style::background::color::color(Style::color::White::Snow));


                auto layout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                auto text = layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Etkinlik::html].get_string().value.data(),TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
            }

        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }
}

//Body::Video::Video(mongocxx::database *_db)
//    :WContainerWidget(),
//      db(_db)
//{

//    skip = 0;
//    count = 0;

//    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
//    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);



//    auto detailContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//    detailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
//    detailContainer->setPadding(0,AllSides);
//    detailContainer->setContentAlignment(AlignmentFlag::Center);


//    container = detailContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//    container->setContentAlignment(AlignmentFlag::Center);
//    container->setMaximumSize(1250,WLength::Auto);
//    container->setPadding(0,AllSides);



//    {
//        std::string path = "img/baskanBackGround.JPG";

//        auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

//        img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
//        img->setHeight(100);
//        img->setPadding(0,AllSides);

//        auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
//        gradientContainer->setHeight(100);
//        gradientContainer->addStyleClass("SliderDetailTextBackground");
//        auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//        layout->addStretch(1);
//        auto serik = layout->addWidget(cpp14::make_unique<WText>("Video Galeri"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
//        serik->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
//    }


//    {
//        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
//        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
//        //        _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
//        //                this->_ClickBack.emit(NoClass());
//        ////            this->initPage();
//        //        });
//        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
//                                      Style::background::color::color(Style::color::Grey::Gainsboro));
//    }




//    {
//        contentContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        contentContainer->addStyleClass(Bootstrap::Grid::container_fluid);
//        this->initPage();
//    }






//    {
//        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
//        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
//        _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
//            if( skip > limit )
//            {
//                skip -= limit;
//                this->initPage();
//            }

//        });

//        auto collection = db->collection(SBLDKeys::Video::collection);

//        try {
//            count = collection.count_documents(document{}.view());
//        } catch (mongocxx::exception &e) {
//            std::cout << "Error: " << e.what() << std::endl;
//        }

//        //        _layout->addWidget(cpp14::make_unique<WText>("Sayfalar"),0,AlignmentFlag::Left);

//        for( std::int64_t i = 0 ; i <= count/limit ; i++ )
//        {
//            auto btn = _layout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(i)),0,AlignmentFlag::Left);
//            btn->decorationStyle().setCursor(Cursor::PointingHand);
//            btn->setMargin(10,Side::Left|Side::Right);
//            btn->clicked().connect([=](){
//                if( i == count/limit - 1 )
//                {
//                    skip = i * limit;
//                }else{
//                    skip = i * limit;
//                }
//                this->initPage();
//            });
//        }
//        _layout->addWidget(cpp14::make_unique<WPushButton>("İleri"),0,AlignmentFlag::Left)->clicked().connect([=](){
//            if( skip < count - limit )
//            {
//                skip += limit;
//                this->initPage();
//            }

//        });

//        _layout->addStretch(1);

//        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
//                                      Style::background::color::color(Style::color::Grey::Gainsboro));
//    }
//}

//Signal<NoClass> &Body::Video::mGetBack()
//{
//    return this->_ClickBack;
//}

//Signal<std::string> &Body::Video::mGetVideo()
//{
//    return _ClickVideo;
//}

//void Body::Video::initPage()
//{

//    contentContainer->clear();


//    auto rowContainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//    rowContainer->addStyleClass(Bootstrap::Grid::row);
//    //    rowContainer->setContentAlignment(AlignmentFlag::Center);
//    //    rowContainer->setAttributeValue(Style::style,Style::Border::border("1px solid red"));


//    auto filter = document{};

//    try {
//        //        filter.append(kvp(SBLDKeys::Haberler::Yayinda,SBLDKeys::Haberler::Yayin::Yayinda));
//    } catch (bsoncxx::exception &e) {
//        setstatus(e);
//        return;
//    }



//    mongocxx::options::find findOption;

//    auto sortDoc = document{};

//    try {
//        sortDoc.append(kvp(SBLDKeys::Video::oid,-1));
//    } catch (bsoncxx::exception &e) {
//        return;
//    }

//    findOption.sort(sortDoc.view());
//    findOption.limit(limit);
//    findOption.skip(skip);


//    auto collection = db->collection(SBLDKeys::Video::collection);
//    auto bucket = db->gridfs_bucket();

//    try {

//        auto cursor = collection.find(filter.view(),findOption);

//        list.clear();

//        for( auto doc : cursor )
//        {
//            item _item;
//            _item.oid = doc[SBLDKeys::Video::oid].get_oid().value.to_string();
//            _item.title = doc[SBLDKeys::Video::title].get_string().value.data();
//            _item.iconPath = SBLDKeys::downloadifNotExist(&bucket,doc[SBLDKeys::Video::VideoLabel].get_oid().value.to_string());
//            list.push_back(_item);
//        }


//        for( item _item : list )
//        {
//            addItem(rowContainer,_item);
//        }


//    } catch (mongocxx::exception &e) {
//        setstatus(e);
//        return;
//    }
//}

//void Body::Video::setstatus(bsoncxx::exception &e)
//{
//    setstatus(e.what());
//}

//void Body::Video::setstatus(mongocxx::exception &e)
//{
//    setstatus(e.what());
//}

//void Body::Video::setstatus(std::string e)
//{
//    contentContainer->clear();
//    auto text = contentContainer->addWidget(cpp14::make_unique<WText>(e.c_str()));
//    text->setAttributeValue(Style::style,Style::font::size::s16px);
//}

//void Body::Video::addItem(WContainerWidget *widget, item _item)
//{
//    auto container = widget->addWidget(cpp14::make_unique<WContainerWidget>());
//    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3);
//    container->setHeight(250);
//    container->setAttributeValue(Style::style,Style::Border::border("1px solid gray"));
//    //    container->setMargin(5,AllSides);

//    auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

//    auto img = layout->addWidget(cpp14::make_unique<WContainerWidget>(),1,AlignmentFlag::Justify);
//    img->setAttributeValue(Style::style,Style::background::url(_item.iconPath)+Style::background::repeat::norepeat+Style::background::size::cover);


//    auto text = layout->addWidget(cpp14::make_unique<WText>(_item.title),0,AlignmentFlag::Bottom);
//    text->setAttributeValue(Style::style,Style::font::size::s12px+Style::font::weight::bold);
//    container->decorationStyle().setCursor(Cursor::PointingHand);

//    container->clicked().connect([=](){
//        this->setVideo(_item.oid);
//    });
//}

//void Body::Video::setVideo(std::string oid)
//{

//    contentContainer->clear();

//    auto detailContainer = contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//    detailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
//    detailContainer->setPadding(0,AllSides);

//    auto layout = detailContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
//    layout->setContentsMargins(0,0,0,0);
//    auto container = layout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Center);
//    container->setMaximumSize(1250,WLength::Auto);
//    container->setPadding(0,AllSides);

//    auto filter = document{};



//    try {
//        filter.append(kvp(SBLDKeys::Video::oid,bsoncxx::oid{oid}));

//    } catch (bsoncxx::exception &e) {
//        container->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
//        return;
//    }
//    //    std::cout << "Video Detail" << std::endl;


//    auto collection = db->collection(SBLDKeys::Video::collection);
//    auto bucket = db->gridfs_bucket();
//    try {
//        mongocxx::stdx::optional<bsoncxx::document::value> val = collection.find_one(filter.view());
//        if( !val )
//        {
//            container->addWidget(cpp14::make_unique<WText>(WString("Empty Document")));
//            return;
//        }else{

//            auto view = val.value().view();

//            std::string path = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::Video::VideoLabel].get_oid().value.to_string(),true);


//            {
//                auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
//                img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);


//                img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
//                img->setHeight(300);
//                img->setPadding(0,AllSides);

//                auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
//                gradientContainer->setHeight(300);
//                gradientContainer->addStyleClass("SliderDetailTextBackground");
//                auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//                layout->addStretch(1);
//                auto title = layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Video::title].get_string().value.data().c_str()),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
//                title->setAttributeValue(Style::style,Style::font::size::s48px+Style::color::color(Style::color::White::AliceBlue));
//            }


//            {
//                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
//                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
//                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
//                _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
//                    this->_ClickBack.emit(NoClass());
//                });
//                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
//                                              Style::background::color::color(Style::color::Grey::Gainsboro));
//            }


//            {
//                std::string mp4Video = SBLDKeys::downloadifNotExist(&bucket,view[SBLDKeys::Video::VideoFile].get_oid().value.to_string(),true);
//                std::string poster = path;

//                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
//                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

//                _container->setHtmlTagName("video");
//                _container->setAttributeValue("controls","");
//                _container->setAttributeValue("autoplay","");

//                auto videoContainer = _container->addWidget(cpp14::make_unique<WContainerWidget>());

//                videoContainer->setHtmlTagName("source");
//                videoContainer->setAttributeValue("src",mp4Video);
//                videoContainer->setAttributeValue("type","video/mp4");

//            }




//            {
//                auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//                textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
//                textContainer->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
//                                                 Style::background::color::color(Style::color::White::Snow));

//                auto layout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//                auto text = layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Video::VideoExplain].get_string().value.data().c_str(),TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
//            }






//            //            {
//            //                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
//            //                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
//            //                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
//            //                _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
//            //                    this->_ClickBack.emit(NoClass());
//            //                });
//            //                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
//            //                                             Style::background::color::color(Style::color::Grey::Gainsboro));
//            //            }


//        }
//    } catch (mongocxx::exception &e) {
//        container->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
//        return;
//    }
//}




Body::Talep::Talep(mongocxx::database *_db)
    :WContainerWidget(),
      db(_db)
{


    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::Snow));
    mMainContainer->setMargin(25,Side::Top|Side::Bottom);
    mMainContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    {
        auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        row->addStyleClass(Bootstrap::Grid::row);

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("Talepte/Şikayette Bulun"));
            text->setAttributeValue(Style::style,Style::font::size::s20px);
        }

    }


    {
        auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        row->addStyleClass(Bootstrap::Grid::row);

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("Kişisel Bilgileriniz"));
        }

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_6);
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            auto layoutContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());

            auto layout = layoutContainer->setLayout(cpp14::make_unique<WGridLayout>());

            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("TC NO"),0,0,AlignmentFlag::Middle);
                mTcNO = layout->addWidget(cpp14::make_unique<WLineEdit>(),0,1);
                mTcNO->setPlaceholderText("TCNO Kayıtlı ise Bilgileriniz Otomatik Dolacaktır");
                mTcNO->textInput().connect(this,&Talep::TCChanged);
                mTcNO->setInputMask("99999999999");
            }

            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("Ad Soyad"),1,0);
                mAdSoyad = layout->addWidget(cpp14::make_unique<WLineEdit>(),1,1);
                mAdSoyad->setPlaceholderText("Adınızı ve Soyadınızı Giriniz");
            }

            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("Mahalle"),2,0,AlignmentFlag::Middle);
                mMahalle = layout->addWidget(cpp14::make_unique<WComboBox>(),2,1);


                auto collection = db->collection(SBLDKeys::Mahalle::collection);

                try {
                    auto cursor = collection.find(document{}.view());

                    for( auto doc : cursor )
                    {
                        mMahalle->addItem(doc[SBLDKeys::Mahalle::mahalle].get_string().value.data());
                    }

                } catch (mongocxx::exception &e) {
                    mMahalle->addItem(WString("Error: {1}").arg(e.what()));
                }

            }
            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("Telefon"),3,0,AlignmentFlag::Middle);
                mTelefon = layout->addWidget(cpp14::make_unique<WLineEdit>(),3,1);
                mTelefon->setPlaceholderText("Cep Telefonu Numaranızı Giriniz(05321234567)");
                //                mTelefon->setInputMask("99999999999");
            }

        }






        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_6);
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            auto layoutContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());

            auto layout = layoutContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("Mahalle"),0,AlignmentFlag::Middle);
                mTalepMahalle = layout->addWidget(cpp14::make_unique<WComboBox>(),0);


                auto collection = db->collection(SBLDKeys::Mahalle::collection);

                try {
                    auto cursor = collection.find(document{}.view());

                    for( auto doc : cursor )
                    {
                        mTalepMahalle->addItem(doc[SBLDKeys::Mahalle::mahalle].get_string().value.data());
                    }

                } catch (mongocxx::exception &e) {
                    mTalepMahalle->addItem(WString("Error: {1}").arg(e.what()));
                }

            }


            {
                auto labelAdSoyad = layout->addWidget(cpp14::make_unique<WText>("Talebiniz"));
                mEdit = layout->addWidget(cpp14::make_unique<WTextEdit>());


                mEdit->setHeight(250);
                auto labelCount = layout->addWidget(cpp14::make_unique<WText>(WString("{1} - Min(61) Karakter").arg(mEdit->text().toUTF8().size())));
                mEdit->changed().connect([=](){
                    labelCount->setText(WString("{1} - Min(61) Karakter").arg(mEdit->text().toUTF8().size()));
                });
                mEdit->setText("Şikayetinizi/Talebinizi Detaylı Olarak Buraya Yazın");
            }

            {
                auto text = layout->addWidget(cpp14::make_unique<WText>("Adres"),1,AlignmentFlag::Left);
                mAdres = layout->addWidget(cpp14::make_unique<WLineEdit>(),1);
                mAdres->setPlaceholderText("Şikayet/Talep Adresi Giriniz");
            }



        }

    }


    {
        auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        row->addStyleClass(Bootstrap::Grid::row);
        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
            [[maybe_unused]] auto text = layout->addWidget(cpp14::make_unique<WText>("<h4>Talebinizin Tam Olarak Yerine Getirilebilmesi için "
                                                                    "Lütfen Bilgilerinizi Eksiksiz Doldurunuz</h4>"));
            mSave = layout->addWidget(cpp14::make_unique<WPushButton>("Kaydet"));
            mSave->addStyleClass(Bootstrap::Button::Primary);
            mSave->clicked().connect(this,&Talep::Save);
        }

        {
            auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            container->setAttributeValue(Style::style,Style::background::color::rgba(125,175,225));
            auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("<h5><p><b>Sinek ve İlaçlama İçin Lütfen</b></p>"
                                                                    "<p>Antalya Büyükşehir Belediyesi</p>"
                                                                    "<p>Çevre Koruma ve Kontrol Daire Başkanlığı Çevre Sağlığı Şube Müdürlüğünü (Vektörel Mücadele) Arayınız</p>"
                                                                    "<p>İletişim: 0 242 871 74 45</p>"
                                                                    "<p>Arama Saatleri 09:00 - 16:00</p></h5>"));
        }



    }
}

void Body::Talep::showMessage(std::string title, std::string msg)
{

    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();

}

void Body::Talep::Save()
{

    if( mTcNO->text().toUTF8().size() != 11 )
    {
        this->showMessage("UYARI","TCNO Hatalı");
        return;
    }

    if( mAdSoyad->text().empty() )
    {
        this->showMessage("UYARI","Ad Soyad Kısmını Doldurmadınız");
        return;
    }

    if( mMahalle->currentText().toUTF8() == "NULL" )
    {
        this->showMessage("UYARI","Mahalle Seçmediniz");
        return;
    }

    if( mTalepMahalle->currentText().toUTF8() == "NULL" )
    {
        this->showMessage("UYARI","Talep/Şikayet Mahallesi Seçmediniz");
        return;
    }


    if( mTelefon->text().toUTF8().size() != 11 )
    {
        this->showMessage("UYARI","Telefon Numarası Hatalı");
        return;
    }

    if( mAdres->text().empty() )
    {
        this->showMessage("UYARI","Adres Bilgisi Girmediniz");
        return;
    }

    if( mEdit->text().toUTF8().size() <= 61 )
    {
        this->showMessage("UYARI","Talep Kısmına Yeterli Bilgi Girmediniz");
        return;
    }



    if( !mQueried )
    {
        this->saveTC();
    }


    auto collection = db->collection(SBLDKeys::SikayetKey::collection);

    auto doc = document{};

    try {
        if( mQueried )
        {
            doc.append(kvp(SBLDKeys::SikayetKey::mainKey::vatandasadSoyad,oldadsoyad.c_str()));
            doc.append(kvp(SBLDKeys::SikayetKey::mainKey::vatandas,oldtcno.c_str()));
        }else{
            doc.append(kvp(SBLDKeys::SikayetKey::mainKey::vatandasadSoyad,mAdSoyad->text().toUTF8().c_str()));
            doc.append(kvp(SBLDKeys::SikayetKey::mainKey::vatandas,mTcNO->text().toUTF8().c_str()));
        }

        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::konu,mEdit->text().toUTF8().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::mahalle,mTalepMahalle->currentText().toUTF8().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::tamadres,mAdres->text().toUTF8().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::durum,SBLDKeys::SikayetKey::durumType::devamediyor));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::tarih,QDate::currentDate().toString("dddd - dd/MM/yyyy").toStdString()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::saat,QTime::currentTime().toString("hh:mm").toStdString().c_str()));
        doc.append(kvp(SBLDKeys::SikayetKey::mainKey::julianDay,QDate::currentDate().toJulianDay()));

    } catch (bsoncxx::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return;
    }


    try {
        auto ins = collection.insert_one(doc.view());

        if( !ins )
        {
            showMessage("UYARI",WString("Talebiniz Alınamadı. Daha Sonra Tekrar Deneyiniz").toUTF8().c_str());
            return;
        }

        if( ins.value().result().inserted_count() )
        {
            showMessage("Bilgi",WString("Talep Kaydınız Başarılı Bir Şekilde Alındı. En Kısa Zaman Tarafınıza Geri Dönüş Yapılacaktır.\nAyrıca Telefon Numaranız ile birlikte Giriş Yaparak Şikayetinizin/Talebinizin Son Durumu Hakkında Bilgi Alabilirsiniz.Teşekkürler").toUTF8().c_str());
            mTcNO->setText("");
            mEdit->setText("Şikayetinizi/Talebinizi Detaylı Olarak Buraya Yazın");
            mAdSoyad->setText("");
            mTelefon->setText("");
            mMahalle->setCurrentIndex(0);
            mTalepMahalle->setCurrentIndex(0);
            mAdres->setText("");

        }else{
            showMessage("UYARI",WString("Talebiniz Alınamadı. Daha Sonra Tekrar Deneyiniz").toUTF8().c_str());
        }

    } catch (mongocxx::exception &e) {
        showMessage("UYARI",WString("Talebiniz Alınamadı: %1").arg(e.what()).toUTF8().c_str());
        return;
    }



}

void Body::Talep::TCChanged()
{


    if( mTcNO->text().toUTF8().size() !=  11 )
    {

        mQueried = false;
        mTelefon->setEnabled(!mQueried);
        mMahalle->setEnabled(!mQueried);
        mAdSoyad->setEnabled(!mQueried);
        mTelefon->setText("");
        mAdSoyad->setText("");
        mMahalle->setCurrentIndex(0);

        return;
    }else{

    }

    auto collection = db->collection(SBLDKeys::TC::collection);

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::TC::tcno,mTcNO->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
        return;
    }


    try {

        auto val = collection.find_one(filter.view());

        if( val )
        {
            mQueried = true;
            mTelefon->setEnabled(!mQueried);
            mMahalle->setEnabled(!mQueried);
            mAdSoyad->setEnabled(!mQueried);


            auto view = val.value().view();


            oldadsoyad = view[SBLDKeys::TC::isimsoyisim].get_string().value.data();
            oldmahalle = view[SBLDKeys::TC::mahalle].get_string().value.data();
            oldtcno = view[SBLDKeys::TC::tcno].get_string().value.data();
            oldtelefon = view[SBLDKeys::TC::cepTel].get_string().value.data();


            std::string isim = view[SBLDKeys::TC::isimsoyisim].get_string().value.data();

            for( decltype (isim.size()) i = 1 ; i < isim.size()-1 ; i++ )
            {
                if( isim.at(i) == ' ' )
                {
                    i += 2;
                }
                if( i < isim.size() )
                {
                    isim.replace(i,1,"*");
                }
            }

            mAdSoyad->setText(isim.c_str());
            for( int i = 0 ; i < mMahalle->count() ; i++ ){
                if( mMahalle->itemText(i).toUTF8() == view[SBLDKeys::TC::mahalle].get_string().value.data() )
                {
                    mMahalle->setCurrentIndex(i);
                    break;
                }
            }


            std::string tel = view[SBLDKeys::TC::cepTel].get_string().value.data();

            for( decltype (tel.size()) i = 3 ; i < tel.size()-3 ; i++ )
            {
                if( i == tel.size()/2-1 )
                {
                    i += 1;
                }
                if( i < tel.size() )
                {
                    tel.replace(i,1,"*");
                }
            }

            mTelefon->setText(tel.c_str());
        }else{
            mQueried = false;
            mTelefon->setEnabled(!mQueried);
            mMahalle->setEnabled(!mQueried);
            mAdSoyad->setEnabled(!mQueried);
        }


    } catch (mongocxx::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

}

void Body::Talep::saveTC()
{


    auto doc = document{};

    try {
        doc.append(kvp(SBLDKeys::TC::cepTel,mTelefon->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        std::cout << "Tel Error: " << e.what() << std::endl;
    }

    try {
        doc.append(kvp(SBLDKeys::TC::isimsoyisim,mAdSoyad->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        std::cout << "mAdSoyad Error: " << e.what() << std::endl;
    }

    try {
        doc.append(kvp(SBLDKeys::TC::mahalle,mMahalle->currentText().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        std::cout << "mMahalle Error: " << e.what() << std::endl;
    }

    try {
        doc.append(kvp(SBLDKeys::TC::tcno,mTcNO->text().toUTF8().c_str()));
    } catch (bsoncxx::exception &e) {
        std::cout << "mTcNO Error: " << e.what() << std::endl;
    }


    auto collection = db->collection(SBLDKeys::TC::collection);

    try {
        auto ins = collection.insert_one(doc.view());
        if( ins )
        {
            if( ins.value().result().inserted_count() )
            {

            }else{
                showMessage("UYARI","Bilgileriniz Kayıt Edilemedi");
            }
        }
    } catch (mongocxx::exception &e) {
        showMessage("UYARI","Bilgileriniz Kayıt Edilemedi");
        return;
    }


}





Body::BilgiEdin::BilgiEdin::BilgiEdin(mongocxx::database *_db)
    :WContainerWidget(),
      db(_db)
{
    setAttributeValue(Style::style,Style::background::color::color(Style::color::White::Snow));
    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
//    mMainContainer->setMaximumSize(1250,WLength::Auto);
    mMainContainer->setContentAlignment(AlignmentFlag::Center);
    mMainContainer->setMargin(90,Side::Top);
    mMainContainer->setId("bilgiEdinmeid");



    mHeaderContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mHeaderContainer->setContentAlignment(AlignmentFlag::Center);
//    mHeaderContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    mHeaderContainer->setPadding(0,AllSides);
    mHeaderContainer->setMaximumSize(1250,WLength::Auto);

    this->initHeader();


//    mMainContainer->addWidget (cpp14::make_unique<WText>("Bilgi Edinme"));

    auto stackContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    stackContainer->setMaximumSize(1250,WLength::Auto);
    stackContainer->setWidth (WLength("100%"));

//    stackContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    mContentContainer = stackContainer->addWidget(cpp14::make_unique<WStackedWidget>());
    mContentContainer->setWidth (WLength("100%"));

    auto controller = std::make_unique<Controller>();
    controller->clickedbasvuru().connect(this,&BilgiEdin::setsayfa);
    mContentContainer->insertWidget(ControlSayfa,std::move(controller));

    auto bilgiEdin = std::make_unique<BilgiEdinmeBasvuruWidget>(new SerikBLDCore::DB(db));

    mContentContainer->insertWidget(BilgiEdinSayfa,std::move(bilgiEdin));

//    menu->addItem("Yeni BaşvuruV2")->clicked ().connect ([=](){
//        this->Content ()->clear();
//        this->Content ()->addWidget (Wt::cpp14::make_unique<BilgiEdinmeBasvuruWidget>(new SerikBLDCore::DB(db),&mTCUser));
//    });

    auto stratejikPlan = std::make_unique<StratejikPlan>(db);
    mContentContainer->insertWidget(StratejikPlanSayfa,std::move(stratejikPlan));

    auto performans = std::make_unique<Performans>();
    mContentContainer->insertWidget(PerformansSayfa,std::move(performans));

    auto faaliyet = std::make_unique<faaliyetRapor>();
    mContentContainer->insertWidget(FaaliyetSayfa,std::move(faaliyet));

    auto denetim = std::make_unique<DenetimRapor>();
    mContentContainer->insertWidget(DenetimRaporSayfa,std::move(denetim));

    auto malidurum = std::make_unique<MaliRapor>();
    mContentContainer->insertWidget(MaliDurumBeklentiSayfa,std::move(malidurum));


    auto kamuHizmet = std::make_unique<KamuHizmetRapor>();
    mContentContainer->insertWidget(KamuHizmet,std::move(kamuHizmet));


}

void Body::BilgiEdin::BilgiEdin::initHeader()
{
    std::string path = "img/baskanBackGround.JPG";

    auto img = mHeaderContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
    img->setHeight(100);
    img->setPadding(0,AllSides);

    auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
    gradientContainer->setHeight(100);
    gradientContainer->addStyleClass("SliderDetailTextBackground");
    auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
    layout->addStretch(1);
    auto serik = layout->addWidget(cpp14::make_unique<WText>("Bilgi Edinme & Raporlar"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
    serik->setAttributeValue(Style::style,Style::font::size::s24px+Style::color::color(Style::color::White::AliceBlue));
}

void Body::BilgiEdin::BilgiEdin::setsayfa(Sayfa sayfa)
{


    switch (sayfa) {
    //    case Sayfa::MaliDurumBeklentiSayfa:
    //        this->showMessage("Bilgi","Mali Durum ve Beklentiler Raporu Kullanılabilir Değil");
    //        return;
    case Sayfa::BagisSayfa:
        this->showMessage("Bilgi","Yardım ve Bağışlar Raporu Kullanılabilir Değil");
        return;
        //    case Sayfa::KamuHizmet:
        //        this->showMessage("Bilgi","Kamu Hizmetleri Standartları Sayfası Hazırlanma Aşamasında");
        //        return;
    case Sayfa::SunulanHizmet:
        this->showMessage("Bilgi","Sunulan Hizmetler Tablosu Sayfası Hazırlanma Aşamasında");
        return;
    }


    mContentContainer->setCurrentIndex(sayfa);
}

void Body::BilgiEdin::BilgiEdin::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}

Body::BilgiEdin::Controller::Controller()
{
    setMargin(10,Side::Bottom|Side::Top);

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    mMainContainer->setMaximumSize(950,WLength::Auto);

    row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    addButton("Bilgi Edinme Başvurusu",BilgiEdin::BilgiEdinSayfa);
    addButton("Stratejik Plan",BilgiEdin::StratejikPlanSayfa);
    addButton("Performans Programı",BilgiEdin::PerformansSayfa);
    addButton("Faaliyet Raporları",BilgiEdin::FaaliyetSayfa);
    addButton("Denetim Raporları",BilgiEdin::DenetimRaporSayfa);
    addButton("Mali Durum ve Beklentiler Raporu",BilgiEdin::MaliDurumBeklentiSayfa);
    addButton("Yapılan Bağış ve Yardımlar",BilgiEdin::BagisSayfa);
    addButton("Kamu Hizmetleri Standartları",BilgiEdin::KamuHizmet);
    addButton("Sunulan Hizmet Standartları",BilgiEdin::SunulanHizmet);


}

Signal<Body::BilgiEdin::BilgiEdin::Sayfa> &Body::BilgiEdin::Controller::clickedbasvuru()
{
    return this->_clickbasvuru_;
}



void Body::BilgiEdin::Controller::addButton(std::string title, BilgiEdin::Sayfa sayfa)
{

    auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);

    auto backContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    backContainer->setHeight(150);
    backContainer->decorationStyle().setCursor(Cursor::PointingHand);
    backContainer->setMargin(10,Side::Top|Side::Bottom);
    backContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    auto layout = backContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

    layout->addStretch(1);
    auto text = layout->addWidget(cpp14::make_unique<WText>(title));
    text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold+Style::font::size::s18px);
    layout->addStretch(1);

    backContainer->setAttributeValue(Style::style,Style::background::color::rgba(25,50,75));
    backContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    container->clicked().connect([=](){
        _clickbasvuru_.emit(sayfa);
    });

}

void Body::BilgiEdin::Controller::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->show();
}








Body::BilgiEdin::StratejikPlan::StratejikPlan(mongocxx::database *_db)
    :WContainerWidget()
{


    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    mMainContainer->setHeight(1200);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2015-2019_stratejik_plan.pdf");
    link.setTarget(Wt::LinkTarget::NewWindow);

    auto anchor = cpp14::make_unique<WAnchor>(WLink(link),"<h3>2015 - 2019 Strateji Planı</h3>");

    row->addWidget (std::move(anchor));

}

Body::BilgiEdin::Performans::Performans()
{



    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto titleContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        auto text = titleContainer->addWidget(cpp14::make_unique<WText>("Serik Belediyesi Performans Programları"));
        text->setAttributeValue(Style::style,Style::font::size::s20px+Style::font::weight::bold);
        titleContainer->setMargin(25,Side::Bottom|Side::Top);
    }


    auto controlWidget = row->addWidget(cpp14::make_unique<WContainerWidget>());
    controlWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    {
        auto layout = controlWidget->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/PERFORMANS_PROGRAMI_2018.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2018 Performans Programı");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/PERFORMANS_PROGRAMI_2017.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2017 Performans Programı");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/PERFORMANS_PROGRAMI_2016.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2016 Performans Programı");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/PERFORMANS_PROGRAMI_2015.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2015 Performans Programı");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
    }



}

Body::BilgiEdin::faaliyetRapor::faaliyetRapor()
    :WContainerWidget()
{

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto titleContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        auto text = titleContainer->addWidget(cpp14::make_unique<WText>("Faaliyet Raporları"));
        text->setAttributeValue(Style::style,Style::font::size::s20px+Style::font::weight::bold);
        titleContainer->setMargin(25,Side::Bottom|Side::Top);
    }


    auto controlWidget = row->addWidget(cpp14::make_unique<WContainerWidget>());
    controlWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    {
        auto layout = controlWidget->setLayout(cpp14::make_unique<WVBoxLayout>());

        //2020_FAALIYET_RAPORU

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2022_FAALIYET_RAPORU.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                    "2022 Faaliyet Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }


        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2021_FAALIYET_RAPORU.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2021 Faaliyet Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2020_FAALIYET_RAPORU.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2020 Faaliyet Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2019_FAALIYET_RAPORU.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2019 Faaliyet Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2018faaliyetraporu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2018 Faaliyet Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2017_tam_sayfa_faaliyet_raporu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2017 Faaliyet Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2016_FAALIYET_RAPORU.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2016 Faaliyet Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2015_FAALIYET_RAPORU.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2015 Faaliyet Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2014_FAALIYET_RAPORU.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2014 Faaliyet Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

    }
}

Body::BilgiEdin::DenetimRapor::DenetimRapor()
{

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto titleContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        auto text = titleContainer->addWidget(cpp14::make_unique<WText>("Denetim Raporları"));
        text->setAttributeValue(Style::style,Style::font::size::s20px+Style::font::weight::bold);
        titleContainer->setMargin(25,Side::Bottom|Side::Top);
    }


    auto controlWidget = row->addWidget(cpp14::make_unique<WContainerWidget>());
    controlWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    {
        auto layout = controlWidget->setLayout(cpp14::make_unique<WVBoxLayout>());


        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2021_mahalliidare_denetim.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2021 Mahalli İdareler Denetim Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2020denetimkomisyonuraporu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2020 Denetim Komisyonu Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2019denetimkomisyonraporu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2019 Denetim Komisyonu Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2018denetimkomisyonuraporu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2018 Denetim Komisyonu Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2017denetimkomisyonuraporu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2017 Denetim Komisyonu Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2016_denetimkomisyonu_raporu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2016 Denetim Komisyonu Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2015_sayistay_raporu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2015 Sayıştay raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/20140915mhidarelerdenetim.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2014 Mahalli İdareler Denetim Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

    }

}

Body::BilgiEdin::MaliRapor::MaliRapor()
{
    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto titleContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        auto text = titleContainer->addWidget(cpp14::make_unique<WText>("Mali Durum ve Beklentiler Raporları"));
        text->setAttributeValue(Style::style,Style::font::size::s20px+Style::font::weight::bold);
        titleContainer->setMargin(25,Side::Bottom|Side::Top);
    }


    auto controlWidget = row->addWidget(cpp14::make_unique<WContainerWidget>());
    controlWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    {
        auto layout = controlWidget->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/2017_mali_durum_ve_beklentiler_raporu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "2017 Mali Durum ve Beklentiler Raporu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

    }
}

void Body::Serik::Hakkinda::initPage()
{


    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    mMainContainer->setContentAlignment(AlignmentFlag::Center);
    mMainContainer->setMargin(90,Side::Top);
    mMainContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Snow));



    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);
    row->setMaximumSize(1270,WLength::Auto);


    {
        std::string path = "img/baskanBackGround.JPG";

        auto img = row->addWidget(cpp14::make_unique<WContainerWidget>());
        img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        img->setMargin(25,Side::Bottom);

        img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        img->setHeight(100);
        img->setPadding(0,AllSides);

        auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
        gradientContainer->setHeight(100);
        gradientContainer->addStyleClass("SliderDetailTextBackground");
        auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        layout->addStretch(1);
        auto serik = layout->addWidget(cpp14::make_unique<WText>("Serik"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
        serik->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
    }

    auto menuContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    menuContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);

    auto contentContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    contentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_9+Bootstrap::Grid::Medium::col_md_9+Bootstrap::Grid::Small::col_sm_8+Bootstrap::Grid::ExtraSmall::col_xs_12);

    // Create a stack where the contents will be located.
    auto contents = Wt::cpp14::make_unique<Wt::WStackedWidget>();
    contents->setTransitionAnimation(WAnimation(WAnimation::Effect::SlideInFromTop,TimingFunction::EaseInOut,500));

    Wt::WMenu *menu =
            menuContainer->addWidget(Wt::cpp14::make_unique<Wt::WMenu>(contents.get()));
    menu->setStyleClass("nav nav-pills nav-stacked");
    //    menu->setWidth(150);


    {
        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Hakkinda::Tip,SBLDKeys::Hakkinda::TipType::Kategori));
        } catch (bsoncxx::exception &e) {
            std::cout << "Error SBLDKeys::Hakkinda::Tip " << e.what() << std::endl;
        }

        try {
            auto cursor = db->collection(SBLDKeys::Hakkinda::collection).find(filter.view());

            for( auto doc : cursor )
            {

                std::string catName;
                try {
                    catName = doc[SBLDKeys::Hakkinda::Kategori::KategoriName].get_string().value.data();
                } catch (bsoncxx::exception &e) {
                    catName = std::string{"Error: "} + e.what() ;
                }

                std::string oid;
                try {
                    oid = doc[SBLDKeys::oid].get_oid().value.to_string();
                } catch (bsoncxx::exception &e) {
                    oid = std::string{"Error: "} + e.what() ;
                }
                menu->addItem(catName,cpp14::make_unique<Serik::Content>(db,oid));
            }

        } catch (mongocxx::exception &e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }

    contentContainer->addWidget(std::move(contents));

}



void Body::Serik::Content::initContent()
{

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    auto btnContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    btnContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    btnContainer->setZIndex(10);


    ContentContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    ContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Hakkinda::Item::kategoriCODE,bsoncxx::oid{this->mKategoriOid}));
    } catch (bsoncxx::exception &e) {
        ContentContainer->clear();
        ContentContainer->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }

    QHash<QString,QString> mapList;

    try {
        auto cursor = db->collection(SBLDKeys::Hakkinda::collection).find(filter.view());

        for( auto doc : cursor )
        {

            std::string itemName;
            std::string oid;

            try {
                itemName = doc[SBLDKeys::Hakkinda::Item::itemName].get_string().value.data();
            } catch (bsoncxx::exception &e) {
                itemName = e.what();
            }

            try {
                oid = doc[SBLDKeys::oid].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                oid = "null";
            }

            mapList.insert(itemName.c_str(),oid.c_str());
        }

    } catch (mongocxx::exception &e) {
        std::cout << "DB Error: " << e.what() << std::endl;
    }


    for( int i = 0 ; i < mapList.keys().count() ; i++ )
    {
        QString key = mapList.keys().at(i);
        QString value = mapList[key];

        auto btn = cpp14::make_unique<WContainerWidget>();
        btn->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
        btn->setHeight(40);
        btn->addNew<WText>(key.toStdString().c_str());
        btn->clicked().connect([=](){
            if( value != "null" )
            {
//                this->selectedContent(value.toStdString());
            }
        });
        btn->addStyleClass(CSSStyle::Button::blueButton);
        btn->setMargin(10,Side::Bottom);

        btnContainer->addWidget(std::move(btn));
    }

    return;
}

void Body::Serik::Content::selectedContent(std::string oid)
{

    this->ContentContainer->clear();
    this->ContentContainer->setMargin(25,Side::Top|Side::Bottom);
    this->ContentContainer->setZIndex(-100);
    //    this->ContentContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::FloralWhite)+Style::Border::border("1px solid gray"));

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        ContentContainer->clear();
        ContentContainer->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }


    bsoncxx::document::value value(document{}.view());

    try {
        auto val = db->collection(SBLDKeys::Hakkinda::collection).find_one(filter.view());
        if( val )
        {
            if( !val.value().view().empty() )
            {
                value = val.value();
            }
        }
    } catch (mongocxx::exception &e) {
        ContentContainer->clear();
        ContentContainer->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }


    if( !value.view().empty() )
    {
        auto view = value.view();

        try {
            auto arList = view[SBLDKeys::Hakkinda::Item::fileOidList].get_array().value;
            auto bucket = db->gridfs_bucket();
            for( auto oid : arList )
            {
                SBLDKeys::downloadifNotExist(&bucket,oid.get_oid().value.to_string(),true);
            }
        } catch (bsoncxx::exception &e) {
            auto text = cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what()),TextFormat::UnsafeXHTML);
            ContentContainer->addWidget(std::move(text));
        }

        try {
            auto text = cpp14::make_unique<WText>(view[SBLDKeys::Hakkinda::Item::itemName].get_string().value.data(),TextFormat::UnsafeXHTML);
            text->setAttributeValue(Style::style,Style::font::size::s24px+Style::font::weight::bold);
            ContentContainer->addWidget(std::move(text));
        } catch (bsoncxx::exception &e) {
            auto text = cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what()),TextFormat::UnsafeXHTML);
            ContentContainer->addWidget(std::move(text));
        }


        try {
            auto text = cpp14::make_unique<WText>(view[SBLDKeys::Hakkinda::Item::html].get_string().value.data(),TextFormat::UnsafeXHTML);
            ContentContainer->addWidget(std::move(text));
        } catch (bsoncxx::exception &e) {
            auto text = cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what()),TextFormat::UnsafeXHTML);
            ContentContainer->addWidget(std::move(text));
        }
    }


}


Body::Job::ProjectBlock::ProjectBlock(mongocxx::database *_db, bool _subPage)
    :SerikBLDCore::DB (_db),subPage(_subPage)
{
    this->clear();
    if( !subPage )
    {
        //        addWidget(cpp14::make_unique<WText>("<h3>Projeler</h3>"))->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);
    }
    //    setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.12));
    setPadding(25,Side::Bottom);


    //    addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    mContainerFluid = addWidget(cpp14::make_unique<WContainerWidget>());
    mContainerFluid->addStyleClass(Bootstrap::Grid::container_fluid);


    auto row = mContainerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);
    //Proje Yılları
    {
        auto yilContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        yilContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);

        auto containerFluid = yilContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        auto _row = containerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
        _row->addStyleClass(Bootstrap::Grid::row);

        const int margin = 0;
        const int padding = 10;

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(125,175,150));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("Tüm Yıllar"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
            container->clicked().connect([=](){
                this->displayinYear();
            });
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,150,225));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2019"));
            //            title->setText(Bootstrap::Badges::badget("2019" , "7"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
            container->clicked().connect([=](){
                this->displayinYear(2019);
            });
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,150,215));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2018"));
            //            title->setText(Bootstrap::Badges::badget("2018" , "98"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
            container->clicked().connect([=](){
                this->displayinYear(2018);
            });
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,150,205));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2017"));
            //            title->setText(Bootstrap::Badges::badget("2017" , "114"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
            container->clicked().connect([=](){
                this->displayinYear(2017);
            });
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,150,195));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2016"));
            //            title->setText(Bootstrap::Badges::badget("2016" , "224"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
            container->clicked().connect([=](){
                this->displayinYear(2016);
            });
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,150,185));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2015"));
            //            title->setText(Bootstrap::Badges::badget("2015" , "152"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
            container->clicked().connect([=](){
                this->displayinYear(2015);
            });
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,150,175));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2014"));
            //            title->setText(Bootstrap::Badges::badget("2014" , "252"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
            container->clicked().connect([=](){
                this->displayinYear(2014);
            });
        }

    }


    // Proje Tamamlanan , Devam Eden , Yapılacak olan , Yıl Yıl
    {
        mDurumDetailContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        mDurumDetailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_5+Bootstrap::Grid::Medium::col_md_5+Bootstrap::Grid::Small::col_sm_10+Bootstrap::Grid::ExtraSmall::col_xs_10);

        this->displayinYear();

    }



    // Son Proje
    {
        auto detaycontainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        detaycontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        //        detaycontainer->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(),this->getRandom(),this->getRandom()));


        auto containerFluid = detaycontainer->addWidget(cpp14::make_unique<WContainerWidget>());
        containerFluid->addStyleClass(Bootstrap::Grid::container_fluid);

        auto _row = containerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
        _row->addStyleClass(Bootstrap::Grid::row);

        const int margin = 5 ;
        const int padding = 0 ;

        {
            auto _container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            _container->setPadding(padding,AllSides);
            _container->setAttributeValue(Style::style,Style::background::url("img/2.jpg")+Style::background::size::cover);


            auto container  = _container->addWidget(cpp14::make_unique<WContainerWidget>());
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.5));

            container->setPadding(15,AllSides);

            auto container_ = container->addWidget(cpp14::make_unique<WContainerWidget>());
            container_->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
            container_->setHeight(235);

            auto vLayout = container_->setLayout(cpp14::make_unique<WVBoxLayout>());

            //            vLayout->addStretch(1);
            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Son Proje"),1,AlignmentFlag::Middle);


            {
                auto filter = document{};
                try {
                    filter.append(kvp(SBLDKeys::Projeler::online,SBLDKeys::Projeler::onlineSet::online));
                } catch (bsoncxx::exception &e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }


                auto sortDoc = document{};

                try {
                    sortDoc.append(kvp(SBLDKeys::Projeler::julianDate,-1));
                } catch (bsoncxx::exception &e) {
                    std::cout << "Error: " << e.what() << std::endl;
                }

                mongocxx::options::find findOptions;

                findOptions.limit(1);

                findOptions.sort(sortDoc.view());

                try {
                    auto val = this->getDB()->db()->collection(SBLDKeys::Projeler::collection).find_one(filter.view(),findOptions);
                    if( val )
                    {
                        if( !val.value().view().empty() )
                        {
                            try {
                                text->setText(std::string("Son Başlanan: ")+val.value().view()[SBLDKeys::Projeler::title].get_string().value.data());
                                _container->setAttributeValue(Style::dataoid,val.value().view()[SBLDKeys::oid].get_oid().value.to_string().c_str());
                            } catch (bsoncxx::exception &e) {
                                text->setText(e.what());
                            }
                        }
                    }
                } catch (mongocxx::exception &e) {
                    text->setText(e.what());
                }

            }


            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(margin,AllSides);
            //            vLayout->addStretch(1);

            _container->clicked().connect([=](){
                _clickProject.emit(_container->attributeValue(Style::dataoid).toUTF8().c_str());
            });

            _container->decorationStyle().setCursor(Cursor::PointingHand);

            //            if( !subPage )
            //            {
            //                auto btn = _row->addWidget(cpp14::make_unique<WPushButton>("İncele"));
            //                btn->addStyleClass(Bootstrap::Button::info);
            //                btn->setMargin(margin+5,Side::Top);

            //                btn->clicked().connect([=](){
            //                    _clickProject.emit(_container->attributeValue(Style::dataoid).toUTF8().c_str());
            //                });

            //            }

        }


    }


}

void Body::Job::ProjectBlock::displayinYear(int year)
{


    auto filter = document{};

    if( year != 0 )
    {
        try {
            filter.append(kvp(SBLDKeys::Projeler::year,year));
        } catch (bsoncxx::exception &e) {

        }
    }

    if( !mDurumDetailContainer ) return;
    mDurumDetailContainer->clear();

    auto containerFluid = mDurumDetailContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    containerFluid->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    auto _row = containerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
    _row->addStyleClass(Bootstrap::Grid::row);

    const int margin = 5 ;
    const int padding = 0 ;


    {
        auto _container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        _container->setPadding(padding,AllSides);
        _container->setAttributeValue(Style::style,Style::background::url("img/1.jpeg")+Style::background::size::cover);

        auto container  = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.5));

        container->setPadding(15,AllSides);

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);



        auto textContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto hLayout = textContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto text = hLayout->addWidget(cpp14::make_unique<WText>("Tüm Projeler"));
        auto view = filter.view();
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        //        text->setMargin(margin,AllSides);
        auto ProjeAdetText = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->db()->collection(SBLDKeys::Projeler::collection).count_documents(view))));
        ProjeAdetText->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        //        ProjeAdetText->setText(Bootstrap::Badges::badget(WString("{1}").arg(this->count(SBLDKeys::Projeler::collection,view)).toUTF8()));



        vLayout->addStretch(1);

        {
            auto spaceWidget = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            spaceWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            spaceWidget->setHeight(10);
        }
    }


    filter.clear();
    if( year != 0 )
    {
        try {
            filter.append(kvp(SBLDKeys::Projeler::year,year));
        } catch (bsoncxx::exception &e) {

        }
    }

    try {
        filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::devamediyor));
    } catch (bsoncxx::exception &e) {

    }


    {
        auto _container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        _container->setPadding(padding,AllSides);
        _container->setAttributeValue(Style::style,Style::background::url("img/1.jpeg")+Style::background::size::cover);

        auto container  = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.5));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        auto textContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto hLayout = textContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto text = hLayout->addWidget(cpp14::make_unique<WText>("Devam Eden"));
        auto view = filter.view();
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        //        text->setMargin(margin,AllSides);
        auto ProjeAdetText = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->db()->collection(SBLDKeys::Projeler::collection).count_documents(view))));
        //        ProjeAdetText->addStyleClass(Bootstrap::Button::Primary);
        ProjeAdetText->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

        vLayout->addStretch(1);

        {
            auto spaceWidget = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            spaceWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            spaceWidget->setHeight(10);
        }
    }

    filter.clear();
    if( year != 0 )
    {
        try {
            filter.append(kvp(SBLDKeys::Projeler::year,year));
        } catch (bsoncxx::exception &e) {

        }
    }

    try {
        filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::tamamlandi));
    } catch (bsoncxx::exception &e) {

    }

    {
        auto _container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        _container->setPadding(padding,AllSides);
        _container->setAttributeValue(Style::style,Style::background::url("img/1.jpeg")+Style::background::size::cover);

        auto container  = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.5));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);
        auto textContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto hLayout = textContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto text = hLayout->addWidget(cpp14::make_unique<WText>("Tamamlanan"));
        auto view = filter.view();
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        //        text->setMargin(margin,AllSides);
        auto ProjeAdetText = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->db()->collection(SBLDKeys::Projeler::collection).count_documents(view))));
        //        ProjeAdetText->addStyleClass(Bootstrap::Button::Success);
        ProjeAdetText->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        vLayout->addStretch(1);

        {
            auto spaceWidget = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            spaceWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            spaceWidget->setHeight(10);
        }
    }

    filter.clear();
    if( year != 0 )
    {
        try {
            filter.append(kvp(SBLDKeys::Projeler::year,year));
        } catch (bsoncxx::exception &e) {

        }
    }

    try {
        filter.append(kvp(SBLDKeys::Projeler::durum,SBLDKeys::Projeler::DURUM::yapilacak));
    } catch (bsoncxx::exception &e) {

    }

    {
        auto _container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        _container->setPadding(padding,AllSides);
        _container->setAttributeValue(Style::style,Style::background::url("img/1.jpeg")+Style::background::size::cover);

        auto container  = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.5));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);
        auto textContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto hLayout = textContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto text = hLayout->addWidget(cpp14::make_unique<WText>("Yapılacak"));
        auto view = filter.view();
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        //        text->setMargin(margin,AllSides);
        auto ProjeAdetText = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->db()->collection(SBLDKeys::Projeler::collection).count_documents(view))));
        //        ProjeAdetText->addStyleClass(Bootstrap::Button::info);
        ProjeAdetText->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        vLayout->addStretch(1);

        {
            auto spaceWidget = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            spaceWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            spaceWidget->setHeight(10);
        }
    }




    //    if( !subPage )
    //    {
    //        auto btn = _row->addWidget(cpp14::make_unique<WPushButton>("Detaylı İncele"));
    //        btn->addStyleClass(Bootstrap::Button::Primary);
    //        btn->clicked().connect([=](){
    //            _clickProjects.emit(NoClass());
    //        });
    //    }





}

Body::Job::TaskBlock::TaskBlock(mongocxx::database *_db)
    :SerikBLDCore::DB(_db)
{
    this->clear();
    //    addWidget(cpp14::make_unique<WText>("<h3>Çalışmalar</h3>"));
    //    setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.12));

    setPadding(25,Side::Bottom);


    //    addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    mContainerFluid = addWidget(cpp14::make_unique<WContainerWidget>());
    mContainerFluid->addStyleClass(Bootstrap::Grid::container_fluid);


    auto row = mContainerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    //Proje Yılları
    {
        auto yilContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        yilContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);

        auto containerFluid = yilContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        auto _row = containerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
        _row->addStyleClass(Bootstrap::Grid::row);

        const int margin = 0;
        const int padding = 10;

        {

            auto filter = document{};

            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(125,175,150));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("Tüm Yıllar"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
        }

        {

            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,150,225));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2019"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,140,215));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2018"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,130,205));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2017"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,120,195));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2016"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,110,185));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2015"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
        }

        {
            auto container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            container->setMargin(margin,Side::Top|Side::Bottom);
            container->setPadding(padding,Side::Top|Side::Bottom);
            container->setAttributeValue(Style::style,Style::background::color::rgb(175,100,175));
            container->decorationStyle().setCursor(Cursor::PointingHand);
            auto title = container->addWidget(cpp14::make_unique<WText>("2014"));
            //            title->setText(Bootstrap::Badges::badget("2014" , "252"));
            title->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
        }

    }


    // Proje Tamamlanan , Devam Eden , Yapılacak olan , Yıl Yıl
    {
        mDurumDetailContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        mDurumDetailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_5+Bootstrap::Grid::Medium::col_md_5+Bootstrap::Grid::Small::col_sm_10+Bootstrap::Grid::ExtraSmall::col_xs_10);

        this->displayinYear();

    }


    // Proje Tamamlanan , Devam Eden , Yapılacak olan , Yıl Yıl
    {
        auto detaycontainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        detaycontainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        //        detaycontainer->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(),this->getRandom(),this->getRandom()));


        auto containerFluid = detaycontainer->addWidget(cpp14::make_unique<WContainerWidget>());
        containerFluid->addStyleClass(Bootstrap::Grid::container_fluid);

        auto _row = containerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
        _row->addStyleClass(Bootstrap::Grid::row);

        const int margin = 5 ;
        const int padding = 0 ;

        {

            auto sortDoc = document{};

            try {
                sortDoc.append(kvp(SBLDKeys::Calismalar::updateDate,-1));
            } catch (bsoncxx::exception &e) {
                std::cout << "Sort DOc Error: " << e.what() << std::endl;
            }

            mongocxx::options::find findOptions;

            findOptions.limit(1);
            findOptions.sort(sortDoc.view());

            auto projectView = document{};

            try {
                projectView.append(kvp(SBLDKeys::Calismalar::baslik,true));
            } catch (bsoncxx::exception &e) {

            }


            auto _container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            _container->setPadding(padding,AllSides);
            std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
            _container->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover);


            auto container  = _container->addWidget(cpp14::make_unique<WContainerWidget>());
            container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.5));

            container->setPadding(15,AllSides);

            auto container_ = container->addWidget(cpp14::make_unique<WContainerWidget>());
            container_->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
            container_->setHeight(235);

            auto vLayout = container_->setLayout(cpp14::make_unique<WVBoxLayout>());


            auto text = vLayout->addWidget(cpp14::make_unique<WText>("Son Çalışma"),1,AlignmentFlag::Middle);


            auto filter = document{};

            try {
                filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            } catch (bsoncxx::exception &e) {

            }


            try {
                auto val = this->getDB()->db()->collection(SBLDKeys::Calismalar::collection).find_one(filter.view(),findOptions);

                if( val )
                {
                    if( !val.value().view().empty() )
                    {
                        try {
                            text->setText(std::string("Son Başlanan: ") + val.value().view()[SBLDKeys::Calismalar::baslik].get_string().value.data());
                            container_->setAttributeValue(Style::dataoid,val.value().view()[SBLDKeys::oid].get_oid().value.to_string());
                            container_->clicked().connect([=](){
                                _clickTask.emit(container_->attributeValue(Style::dataoid).toUTF8());
                            });
                            container_->decorationStyle().setCursor(Cursor::PointingHand);
                        } catch (bsoncxx::exception &e) {
                            std::cout << "Eroror: " << e.what() << std::endl;
                        }
                    }
                }
            } catch (mongocxx::exception &e) {

            }

            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            text->setMargin(margin,AllSides);
            //            vLayout->addStretch(1);

        }

        //        {
        //            auto btn = _row->addWidget(cpp14::make_unique<WPushButton>("İncele"));
        //            btn->addStyleClass(Bootstrap::Button::info);
        //            btn->setMargin(margin+5,Side::Top);
        //        }

    }
}

void Body::Job::TaskBlock::displayinYear(int year)
{


    auto filter = document{};



    if( !mDurumDetailContainer ) return;

    mDurumDetailContainer->clear();


    auto containerFluid = mDurumDetailContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    containerFluid->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    auto _row = containerFluid->addWidget(cpp14::make_unique<WContainerWidget>());
    _row->addStyleClass(Bootstrap::Grid::row);

    const int margin = 5 ;
    const int padding = 0 ;


    {
        auto _container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        //            _container->setMargin(margin,AllSides);
        _container->setPadding(padding,AllSides);
        _container->setAttributeValue(Style::style,Style::background::url("img/1.jpeg")+Style::background::size::cover);

        auto container  = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.5));

        container->setPadding(15,AllSides);

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        filter.clear();



        auto textContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto hLayout = textContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto text = hLayout->addWidget(cpp14::make_unique<WText>("Tüm Çalışmalar"));

        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        auto ProjeAdetText = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))));
        ProjeAdetText->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

        vLayout->addStretch(1);

        {
            auto spaceWidget = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            spaceWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            spaceWidget->setHeight(10);
        }
    }



    {
        auto _container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        _container->setPadding(padding,AllSides);
        _container->setAttributeValue(Style::style,Style::background::url("img/1.jpeg")+Style::background::size::cover);

        auto container  = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.5));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        filter.clear();
        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
        } catch (bsoncxx::exception &e) {

        }
        auto textContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto hLayout = textContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto text = hLayout->addWidget(cpp14::make_unique<WText>("Devam Eden"));

        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        auto ProjeAdetText = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))));
        ProjeAdetText->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        vLayout->addStretch(1);

        {
            auto spaceWidget = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            spaceWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            spaceWidget->setHeight(10);
        }
    }

    {
        auto _container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        _container->setPadding(padding,AllSides);
        _container->setAttributeValue(Style::style,Style::background::url("img/1.jpeg")+Style::background::size::cover);

        auto container  = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.5));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);

        filter.clear();
        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
        } catch (bsoncxx::exception &e) {

        }

        auto textContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto hLayout = textContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto text = hLayout->addWidget(cpp14::make_unique<WText>("Tamamlanan"));
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        auto ProjeAdetText = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))));
        ProjeAdetText->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        vLayout->addStretch(1);

        {
            auto spaceWidget = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            spaceWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            spaceWidget->setHeight(10);
        }

    }

    {
        auto _container = _row->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        _container->setPadding(padding,AllSides);
        _container->setAttributeValue(Style::style,Style::background::url("img/aspendos.jpg")+Style::background::size::cover);

        auto container  = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),.5));

        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());

        vLayout->addStretch(1);
        filter.clear();
        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::yapilacak));
        } catch (bsoncxx::exception &e) {

        }
        auto textContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
        auto hLayout = textContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto text = hLayout->addWidget(cpp14::make_unique<WText>("Yapılacaklar"));
        text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        auto ProjeAdetText = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))));
        ProjeAdetText->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
        vLayout->addStretch(1);

        {
            auto spaceWidget = _row->addWidget(cpp14::make_unique<WContainerWidget>());
            spaceWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            spaceWidget->setHeight(10);
        }

    }

}



Body::Job::Block::Block(mongocxx::database *_db)
    :SerikBLDCore::DB(_db)
{

    //    setPadding(0,AllSides);
    //    setMargin(50,Side::Bottom|Side::Top);

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());

    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setContentAlignment(AlignmentFlag::Center);

        auto text = container->addWidget(cpp14::make_unique<WText>("Projeler & Çalışmalar"));
        text->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);
        text->setPadding(25,Side::Left|Side::Right);
    }


    mProjectBlock = row->addWidget(cpp14::make_unique<ProjectBlock>(this->getDB()->db()));
    mProjectBlock->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    mTaskBlock = row->addWidget(cpp14::make_unique<TaskBlock>(this->getDB()->db()));
    mTaskBlock->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

}



Body::Calisma::Calisma(mongocxx::database *_db)
    :SerikBLDCore::DB(_db),TableFilter(document{}.view())
{
    this->clear();
    mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    mMainContainer->setPadding(90,Side::Top);

    auto detailContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    detailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    detailContainer->setPadding(0,AllSides);
    detailContainer->setContentAlignment(AlignmentFlag::Center);


    container = detailContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    container->setMaximumSize(1024,WLength::Auto);
    container->setPadding(0,AllSides);
    container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::Snow));


    skip = 0;


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
        auto serik = layout->addWidget(cpp14::make_unique<WText>("Çalışmalar"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
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

        auto projeListbtn = _layout->addWidget(cpp14::make_unique<WPushButton>("Zaman Tüneli"),0,AlignmentFlag::Left);
        projeListbtn->clicked().connect([=](){

            //             auto filter = document{};

            //             try {
            //                 filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            //             } catch (bsoncxx::exception &e) {

            //             }

            //             this->initTable(filter.view());
            this->zamanTuneli();

        });
        projeListbtn->addStyleClass(Bootstrap::Button::info);

        //         auto istatisticbtn = _layout->addWidget(cpp14::make_unique<WPushButton>("İstatistik"),0,AlignmentFlag::Left);
        //         istatisticbtn->clicked().connect([=](){
        //                 this->initIstatistic();
        //         });
        //         istatisticbtn->addStyleClass(Bootstrap::Button::Warning);

        _layout->addStretch(1);
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }



    this->initMenu();

    this->initPage();






}

void Body::Calisma::zamanTuneli()
{
    mTableContainer->clear();

    auto _fluidCOntainer = mTableContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _fluidCOntainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto _rowContainer = _fluidCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _rowContainer->addStyleClass(Bootstrap::Grid::row);
    _rowContainer->setMargin(25,Side::Bottom|Side::Top);


    {
        auto container = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setMargin(WLength::Auto,Side::Left|Side::Right);
        container->setAttributeValue(Style::style,Style::Border::border("1px solid black"));
        auto text = container->addWidget(cpp14::make_unique<WText>("Bugün: " + QDate::currentDate().toString("dddd dd/MMMM/yyyy").toStdString()));
        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold+Style::font::size::s20px+Style::background::color::rgb(0,77,153));
        text->setPadding(15,AllSides);
        text->setMargin(0,Side::Top);
        container->setPadding(0,Side::Top);
        container->setMargin(0,Side::Top);
        container->setZIndex(11);
        container->setAttributeValue(Style::style,Style::background::color::rgb(0,77,153));

    }


    auto _LeftContainer = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _LeftContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    _LeftContainer->setPadding(3,Side::Left);
    auto LeftFluidContainer = _LeftContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    LeftFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto LeftRowContainer = LeftFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    LeftRowContainer->addStyleClass(Bootstrap::Grid::row);
    LeftRowContainer->setAttributeValue(Style::customData,"left");

    // Time Line
    {
        auto TimeLineContainer = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        TimeLineContainer->setAttributeValue(Style::style,Style::Border::border("1px solid #004d99"));
        TimeLineContainer->setMargin(0,Side::Top|Side::Bottom);
        TimeLineContainer->setPositionScheme(PositionScheme::Absolute);
        TimeLineContainer->setOffsets(25,Side::Top|Side::Bottom);
        TimeLineContainer->setOffsets(WLength("50%"),Side::Left);
        TimeLineContainer->setZIndex(10);
    }


    // Right Side Wall

    auto _RightContainer = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _RightContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_6);
    _RightContainer->setPadding(3,Side::Right);
    auto RightFluidContainer = _RightContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    RightFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto RightRowContainer = RightFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    RightRowContainer->addStyleClass(Bootstrap::Grid::row);
    RightRowContainer->setAttributeValue(Style::customData,"right");

    skip = 0;

    this->addZamanTuneli(_rowContainer,LeftRowContainer,RightRowContainer);
}

void Body::Calisma::addZamanTuneli(WContainerWidget *_rowContainer, WContainerWidget *LeftRowContainer, WContainerWidget *RightRowContainer)
{

    mongocxx::options::find findOptions;

    findOptions.skip(skip);

    findOptions.limit(10);

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp(SBLDKeys::Calismalar::updateDate,-1));
    } catch (bsoncxx::exception &e) {

    }



    findOptions.sort(sortDoc.view());



    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
    } catch (bsoncxx::exception &e) {

    }

    auto birimlist = array{};

    try {
        birimlist.append(make_document(kvp(SBLDKeys::Calismalar::birim,"Fen İşleri Müdürlüğü")));
    } catch (bsoncxx::exception &e) {

    }

    try {
        birimlist.append(make_document(kvp(SBLDKeys::Calismalar::birim,"İmar ve Şehircilik Müdürlüğü")));
    } catch (bsoncxx::exception &e) {

    }

    try {
        birimlist.append(make_document(kvp(SBLDKeys::Calismalar::birim,"Temizlik İşleri Müdürlüğü")));
    } catch (bsoncxx::exception &e) {

    }

    try {
        birimlist.append(make_document(kvp(SBLDKeys::Calismalar::birim,"Park ve Bahçeler Müdürlüğü")));
    } catch (bsoncxx::exception &e) {

    }

    try {
        birimlist.append(make_document(kvp(SBLDKeys::Calismalar::birim,"Kültür ve Sosyal İşler Müdürlüğü")));
    } catch (bsoncxx::exception &e) {

    }

    try {
        filter.append(kvp("$or",birimlist));
    } catch (bsoncxx::exception &e) {

    }




    LeftSide = true;

    try {
        auto cursor = this->getDB()->db()->collection(SBLDKeys::Calismalar::collection).find(filter.view(),findOptions);


        for( auto doc : cursor )
        {
            if( LeftSide )
            {
                this->addCalismaItemToZamanTuneli(LeftRowContainer,doc);
            }else{
                this->addCalismaItemToZamanTuneli(RightRowContainer,doc);
            }
            LeftSide = !LeftSide;
        }


    } catch (mongocxx::exception &e) {

    }


    {
        auto container = _rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setZIndex(11);

        auto btn = container->addWidget(cpp14::make_unique<WPushButton>("Daha Fazla Yükle"));
        btn->addStyleClass(Bootstrap::Button::Primary);
        btn->setMargin(WLength::Auto,Side::Left|Side::Right);
        btn->clicked().connect([=](){
            skip += 10;
            _rowContainer->removeWidget(container);
            this->addZamanTuneli(_rowContainer ,LeftRowContainer,RightRowContainer);
        });
    }
}

void Body::Calisma::addCalismaItemToZamanTuneli(WContainerWidget *widget, bsoncxx::document::view doc)
{
    auto container = widget->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    container->setMargin(25,Side::Bottom|Side::Top);
    container->setPadding(15,Side::Top|Side::Bottom);
    container->decorationStyle().setCursor(Cursor::PointingHand);

    auto fContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    auto imgContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto imgPath = this->downloadFileWeb(doc[SBLDKeys::Calismalar::icon].get_oid().value.to_string().c_str());
    imgContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
    imgContainer->setHeight(this->getRandom(180,250));
    imgContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    if( LeftSide )
    {
        container->setAttributeValue(Style::style,Style::background::color::color("#c2c2d6; ")+Style::Border::borderRardius("15","0","15","15"));
    }else{
        container->setAttributeValue(Style::style,Style::background::color::color("#c2c2d6; ")+Style::Border::borderRardius("0","15","15","15"));
    }
    {
        auto textContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto text = textContainer->addWidget(cpp14::make_unique<WText>(doc[SBLDKeys::Calismalar::baslik].get_string().value.data()));
        text->setAttributeValue(Style::style,Style::font::weight::bold);
        text->setMargin(WLength::Auto,Side::Left|Side::Right);
    }


    {
        auto mahList = doc[SBLDKeys::Calismalar::mahalleler].get_array().value;
        for( auto mah : mahList )
        {
            auto textContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            QString str = mah.get_string().value.data();
            str = str.remove(str.count()-9,9);
            auto text = textContainer->addWidget(cpp14::make_unique<WText>(str.toStdString().c_str()));
            text->addStyleClass(Bootstrap::Label::Warning);
            text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s12px);
            text->setMargin(WLength::Auto,Side::Left|Side::Right);
        }
    }

    container->setAttributeValue(Style::dataoid,doc[SBLDKeys::oid].get_oid().value.to_string());

    container->clicked().connect([=](){
        this->setCalismaDetail(container->attributeValue(Style::dataoid).toUTF8());
    });



    {
        auto dateContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        dateContainer->setPositionScheme(PositionScheme::Absolute);
        dateContainer->setHeight(25);
        dateContainer->setPadding(0,AllSides);

        {
            auto _fluidContainer = dateContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _fluidContainer->setPadding(0,AllSides);
            _fluidContainer->setMargin(0,AllSides);
            auto hLayout = _fluidContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
            hLayout->setContentsMargins(0,0,0,0);


            if( LeftSide )
            {
                auto _colorContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                //                _colorContainer->setAttributeValue(Style::style,Style::background::color::rgba(0,77,153,0.85));
                //                _colorContainer->setHeight(25);
                _colorContainer->setMargin(0,Side::Right);


                auto text = _colorContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(QDate::fromJulianDay(doc[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value).toString("dd/MMMM/yyyy").toStdString())));
                text->addStyleClass(Bootstrap::Label::Primary);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::lighter+Style::font::size::s14px);
                text->addStyleClass("ZamanTuneliFont");


                auto _imgContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                _imgContainer->setAttributeValue(Style::style,Style::background::url("img/leftConnect.png")+Style::background::size::cover+Style::background::position::center_center+Style::background::repeat::norepeat);
                _imgContainer->setHeight(20);
                _imgContainer->setWidth(30);
                _imgContainer->setMargin(0,Side::Left);


            }else{
                auto _imgContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                _imgContainer->setAttributeValue(Style::style,Style::background::url("img/rightConnect.png")+Style::background::size::cover+Style::background::position::center_center+Style::background::repeat::norepeat);
                _imgContainer->setHeight(20);
                _imgContainer->setWidth(30);
                _imgContainer->setMargin(0,Side::Right);

                auto _colorContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
                //                _colorContainer->setAttributeValue(Style::style,Style::background::color::rgba(0,77,153,0.85));
                //                _colorContainer->setHeight(25);
                _colorContainer->setMargin(0,Side::Left);

                auto text = _colorContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(QDate::fromJulianDay(doc[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value).toString("dd/MMMM/yyyy").toStdString())));
                text->addStyleClass(Bootstrap::Label::Primary);
                text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::lighter+Style::font::size::s14px);
                text->addStyleClass("ZamanTuneliFont");
            }

        }

        dateContainer->setOffsets(-10,Side::Top);

        if( LeftSide )
        {
            dateContainer->setOffsets(-5,Side::Right);
        }else{
            dateContainer->setOffsets(-5,Side::Left);
        }
    }

    {
        auto dateContainer = imgContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        dateContainer->setPositionScheme(PositionScheme::Absolute);
        dateContainer->setHeight(25);
        dateContainer->setPadding(0,AllSides);

        {
            auto _fluidContainer = dateContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            _fluidContainer->setPadding(0,AllSides);
            _fluidContainer->setMargin(0,AllSides);
            auto hLayout = _fluidContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
            hLayout->setContentsMargins(0,0,0,0);

            QString str = doc[SBLDKeys::Calismalar::birim].get_string().value.data();
            str = str.mid(0,str.count()-9);
            auto text = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(str.toStdString().c_str())));
            //            text->addStyleClass(Bootstrap::Label::Success);
            text->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::ForestGreen)+Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold+Style::font::size::s14px);
            text->addStyleClass("ZamanTuneliFont");
            text->addStyleClass("CommanBoxShadow");
        }

        dateContainer->setOffsets(WLength("85%"),Side::Top);

        if( LeftSide )
        {
            dateContainer->setOffsets(-15,Side::Left);
        }else{
            dateContainer->setOffsets(-15,Side::Right);
        }
    }

    //    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void Body::Calisma::setCalismaDetail(std::string oid)
{

    mTableContainer->clear();


    //    mTableContainer->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(oid.c_str())));

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
        return;
    }

    bsoncxx::document::value val(document{}.view());

    try {
        auto val_ = this->getDB()->db()->collection(SBLDKeys::Calismalar::collection).find_one(filter.view());
        if( val_ )
        {
            if( !val_.value().view().empty() )
            {
                val = val_.value();
            }else{
                mTableContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("Document Empty")));
            }
        }else{
            mTableContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("No Document Returned")));
        }
    } catch (mongocxx::exception &e) {
        mTableContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
    }

    auto view = val.view();

    auto TitleContainer = mTableContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto TitleContainerColor = TitleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    TitleContainerColor->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(oid.c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(view[SBLDKeys::Calismalar::baslik].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Başlama Tarihi {1}").arg(WDate::fromJulianDay(view[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value).toString("dd/MM/yyyy"))),0,AlignmentFlag::Middle);
        text->addStyleClass(Bootstrap::Label::Primary);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Durum {1}").arg(view[SBLDKeys::Calismalar::durum].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Kategori {1}").arg(view[SBLDKeys::Calismalar::kategori].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Birim {1}").arg(view[SBLDKeys::Calismalar::birim].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tanım: <u>{1}</u>").arg(view[SBLDKeys::Calismalar::tanim].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }



    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        container->setPadding(10,AllSides);
        for( auto mah : view[SBLDKeys::Calismalar::mahalleler].get_array().value )
        {
            auto text = container->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(mah.get_string().value.data())));
            text->addStyleClass(Bootstrap::Label::Warning+Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
            //            text->setMargin(5,AllSides);
        }
    }


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        std::string iconPaht = this->downloadFileWeb(view[SBLDKeys::Calismalar::icon].get_oid().value.to_string().c_str());
        TitleContainer->setAttributeValue(Style::style,Style::background::url(iconPaht)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setHeight(50);
    }
    {


        auto asamaContainer = mTableContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        try {
            auto asamaList = view[SBLDKeys::Calismalar::asamalar].get_array().value;

            for( auto _asama : asamaList )
            {
                auto doc = _asama.get_document().value;
                auto container = asamaContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                //                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
                auto StackWidget = container->addWidget(cpp14::make_unique<WStackedWidget>());
                StackWidget->setTransitionAnimation(WAnimation(AnimationEffect::SlideInFromRight,TimingFunction::EaseInOut,250),true);
                std::vector<std::string> imgPathList;
                for( auto id : doc[SBLDKeys::Calismalar::Asama::Gorseller].get_array().value )
                {
                    std::string imgPath = this->downloadFileWeb(id.get_oid().value.to_string().c_str());
                    imgPathList.push_back(imgPath);
                }

                for( auto str : imgPathList )
                {
                    auto slideCOntainer = cpp14::make_unique<WContainerWidget>();
                    slideCOntainer->setHeight(350);



                    auto frontimgContainer = slideCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    frontimgContainer->setHeight(350);
                    frontimgContainer->setAttributeValue(Style::style,Style::background::url(str)+Style::background::size::contain+Style::background::position::center_center+Style::background::repeat::norepeat);

                    auto imgContainer = cpp14::make_unique<WContainerWidget>();
                    imgContainer->setHeight(350);
                    imgContainer->setWidth(WLength("100%"));
                    imgContainer->setAttributeValue(Style::style,Style::background::url(str)+Style::background::size::contain+Style::background::position::center_center+"z-index:-1; ");
                    imgContainer->addStyleClass("CommanFilter");
                    imgContainer->setPositionScheme(PositionScheme::Absolute);
                    imgContainer->setOffsets(0,Side::Top);
                    slideCOntainer->addWidget(std::move(imgContainer));

                    StackWidget->addWidget(std::move(slideCOntainer));

                }


                auto btnContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                btnContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));


                auto hLayout = btnContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

                auto backBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("Geri"));
                backBtn->clicked().connect([=](){
                    int indexCount = StackWidget->count();
                    int CurrentIndex = StackWidget->currentIndex();

                    if( CurrentIndex > 0 )
                    {
                        StackWidget->setCurrentIndex(--CurrentIndex );
                    }else{
                        StackWidget->setCurrentIndex( indexCount-1 );
                    }
                });

                hLayout->addStretch(1);
                int imgCounter = 0;

                for( auto str : imgPathList )
                {
                    imgCounter++;
                }

                {
                    auto text = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Resim").arg(imgCounter)),0,AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s24px);
                }
                hLayout->addStretch(1);



                auto nextBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("İleri"));

                nextBtn->clicked().connect([=](){
                    int indexCount = StackWidget->count();
                    int CurrentIndex = StackWidget->currentIndex();

                    if( CurrentIndex < indexCount-1 )
                    {
                        StackWidget->setCurrentIndex(++CurrentIndex );
                    }else{
                        StackWidget->setCurrentIndex( 0 );
                    }
                });

                auto TextCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                TextCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
                auto aciklamaText = TextCOntainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Calismalar::Asama::Aciklama].get_string().value.data())));
                aciklamaText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
                TextCOntainer->setPadding(10,Side::Top|Side::Bottom);
            }
        } catch (bsoncxx::exception &e) {

        }


    }

}

void Body::Calisma::initTable(bsoncxx::document::view view)
{

    mTableContainer->clear();
    try {
        CalismaCount = this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(view);
    } catch (bsoncxx::exception &e) {
        CalismaCount = 0;
        return;
    }

    {
        auto container = mTableContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::container_fluid);
        auto hLayout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
        auto back = hLayout->addWidget(cpp14::make_unique<WPushButton>("Geri"));
        back->clicked().connect([=](){
            if( skip > limit )
            {
                skip -= limit;
                this->initTable(TableFilter.view());
            }
        });
        hLayout->addStretch(1);

        auto text = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1} - {2} Arasını Görüntülüyorsunuz").arg(skip).arg(skip+limit)));
        text->setAttributeValue( Style::style , Style::font::size::s16px+Style::font::weight::bold );
        hLayout->addStretch(1);

        auto next = hLayout->addWidget(cpp14::make_unique<WPushButton>("İleri"));
        next->clicked().connect([=](){
            std::cout << CalismaCount << " " << skip << std::endl;
            if( skip < CalismaCount - limit-1 )
            {
                skip += limit;
                this->initTable(TableFilter.view());
            }
        });
    }

    auto table = cpp14::make_unique<WTable>();
    auto table_ = table.get();
    table_->setHeaderCount(1);

    table_->elementAt(0, 0)->addWidget(cpp14::make_unique<WText>("#"));
    table_->elementAt(0, 1)->addWidget(cpp14::make_unique<WText>("Başlık"));
    table_->elementAt(0, 2)->addWidget(cpp14::make_unique<WText>("Tarih"));
    table_->elementAt(0, 3)->addWidget(cpp14::make_unique<WText>("Mahalle"));
    table_->elementAt(0, 4)->addWidget(cpp14::make_unique<WText>("Birim"));
    table_->elementAt(0, 5)->addWidget(cpp14::make_unique<WText>("Durum"));
    table_->elementAt(0, 6)->addWidget(cpp14::make_unique<WText>("incele"));

    table_->addStyleClass("table form-inline table-bordered table-hover table-condensed table-striped");


    mTableContainer->addWidget(std::move(table));


    mongocxx::options::find findOptions;

    findOptions.limit(this->limit);

    auto projectView = document{};

    try {
        projectView.append(kvp(SBLDKeys::Calismalar::baslik,true));
    } catch (bsoncxx::exception &e) {
        std::cout << "ProjectVıew Error: " << e.what() << std::endl;
    }
    try {
        projectView.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,true));
    } catch (bsoncxx::exception &e) {
        std::cout << "ProjectVıew Error: " << e.what() << std::endl;
    }

    try {
        projectView.append(kvp(SBLDKeys::Calismalar::mahalleler,true));
    } catch (bsoncxx::exception &e) {
        std::cout << "ProjectVıew Error: " << e.what() << std::endl;
    }

    try {
        projectView.append(kvp(SBLDKeys::Calismalar::birim,true));
    } catch (bsoncxx::exception &e) {
        std::cout << "ProjectVıew Error: " << e.what() << std::endl;
    }

    try {
        projectView.append(kvp(SBLDKeys::Calismalar::durum,true));
    } catch (bsoncxx::exception &e) {
        std::cout << "ProjectVıew Error: " << e.what() << std::endl;
    }

    findOptions.skip(skip);

    findOptions.projection(projectView.view());


    auto sortDoc = document{};

    try {
        sortDoc.append(kvp(SBLDKeys::Calismalar::updateDate,-1));
    } catch (bsoncxx::exception &e) {

    }

    findOptions.sort(sortDoc.view());


    std::cout << "FIND: Working" << std::endl;


    try {
        auto cursor = this->db()->collection(SBLDKeys::Calismalar::collection).find(view,findOptions);


        CalismaList.clear();

        for( auto doc : cursor )
        {
            CalismaItem item;

            try {
                item.oid = doc[SBLDKeys::oid].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {
                std::cout << "Error: " << e.what() << std::endl;
            }

            try {
                item.birim = doc[SBLDKeys::Calismalar::birim].get_string().value.data();
            } catch (bsoncxx::exception &e) {
                std::cout << "Error: " << e.what() << std::endl;
            }

            try {
                item.durum = doc[SBLDKeys::Calismalar::durum].get_string().value.data();
            } catch (bsoncxx::exception &e) {
                std::cout << "Error: " << e.what() << std::endl;
            }

            try {
                item.baslik = doc[SBLDKeys::Calismalar::baslik].get_string().value.data();
                QString str = QString::fromStdString(item.baslik);
                if( str.count() > 25 )
                {
                    str = str.mid(0,25);
                    str.append("...");
                    item.baslik = str.toStdString();
                }
            } catch (bsoncxx::exception &e) {
                std::cout << "Error: " << e.what() << std::endl;
            }

            try {
                item.tarih = doc[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value;
            } catch (bsoncxx::exception &e) {
                std::cout << "Error: " << e.what() << std::endl;
            }



            try {
                auto arlist = doc[SBLDKeys::Calismalar::mahalleler].get_array().value;
                for( auto mah : arlist )
                {
                    item.mahalleList.push_back(mah.get_string().value.data());
                }
            } catch (bsoncxx::exception &e) {
                std::cout << "Error: " << e.what() << std::endl;
            }

            CalismaList.push_back(item);

        }



    } catch (mongocxx::exception &e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    std::cout << "Place Table" << std::endl;

    int row = 1;
    int red = this->getRandom(50,75);
    int green = this->getRandom(50,75);
    int blue = this->getRandom(50,75);
    for( auto item : CalismaList )
    {
        table_->elementAt(row, 0)->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(row+skip)));
        table_->elementAt(row, 1)->addWidget(cpp14::make_unique<WText>(item.baslik));
        table_->elementAt(row, 2)->addWidget(cpp14::make_unique<WText>(QDate::fromJulianDay(item.tarih).toString("dd/MM/yyyy").toStdString()));

        if( item.mahalleList.size() > 1 )
        {
            QString str = item.mahalleList.front().c_str();
            if( str.count() > 16 ) { str = str.mid(0,13); str.append("..."); }
            table_->elementAt(row, 3)->addWidget(cpp14::make_unique<WText>(WString("{1} +{2}").arg(str.toStdString().c_str()).arg(item.mahalleList.size()-1)));
        }else{
            if( item.mahalleList.size() )
            {
                table_->elementAt(row, 3)->addWidget(cpp14::make_unique<WText>(item.mahalleList.front().c_str()));
            }else{
                table_->elementAt(row, 3)->addWidget(cpp14::make_unique<WText>("Mahalle Yok"))->addStyleClass(Bootstrap::Label::Danger);
            }
        }


        table_->elementAt(row, 4)->addWidget(cpp14::make_unique<WText>(item.birim))->addStyleClass(Bootstrap::Label::Default);

        if( item.durum == SBLDKeys::Calismalar::DURUM::devamediyor )
        {
            auto text = cpp14::make_unique<WText>(item.durum );
            text->addStyleClass(Bootstrap::Label::Warning);
            table_->elementAt(row, 5)->addWidget(std::move(text));
        }else{
            auto text = cpp14::make_unique<WText>(item.durum );
            text->addStyleClass(Bootstrap::Label::Success);
            table_->elementAt(row, 5)->addWidget(std::move(text));
        }

        {
            auto container = cpp14::make_unique<WContainerWidget>();
            container->decorationStyle().setCursor(Cursor::PointingHand);
            container->setAttributeValue(Style::style,Style::background::color::rgb(red += 3,green += 3,blue += 3));
            auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
            auto text = layout->addWidget(cpp14::make_unique<WText>("İncele"),0,AlignmentFlag::Middle);
            text->setAttributeValue(Style::style,Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
            table_->elementAt(row, 6)->setPadding(0,AllSides);
            container->setMargin(0,AllSides);
            container->clicked().connect([=](){
                this->setSelectedCalisma(item.oid);
            });
            table_->elementAt(row, 6)->addWidget(std::move(container));
        }

        row++;
    }



}

void Body::Calisma::setSelectedCalisma(std::string oid)
{
    mTableContainer->clear();


    //    mTableContainer->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(oid.c_str())));

    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::oid,bsoncxx::oid{oid}));
    } catch (bsoncxx::exception &e) {
        mTableContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
        return;
    }

    bsoncxx::document::value val(document{}.view());

    try {
        auto val_ = this->db()->collection(SBLDKeys::Calismalar::collection).find_one(filter.view());
        if( val_ )
        {
            if( !val_.value().view().empty() )
            {
                val = val_.value();
            }else{
                mTableContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("Document Empty")));
            }
        }else{
            mTableContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg("No Document Returned")));
        }
    } catch (mongocxx::exception &e) {
        mTableContainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(e.what())));
    }

    auto view = val.view();

    auto TitleContainer = mTableContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    auto TitleContainerColor = TitleContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    TitleContainerColor->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125),0.75));


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(oid.c_str())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("<h2>{1}</h2>").arg(view[SBLDKeys::Calismalar::baslik].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Başlama Tarihi {1}").arg(view[SBLDKeys::Calismalar::baslamaTarihi].get_int64().value)),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Durum {1}").arg(view[SBLDKeys::Calismalar::durum].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Kategori {1}").arg(view[SBLDKeys::Calismalar::kategori].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Birim {1}").arg(view[SBLDKeys::Calismalar::birim].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));

    }

    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tanım: <u>{1}</u>").arg(view[SBLDKeys::Calismalar::tanim].get_string().value.data())),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold+Style::color::color(Style::color::White::AliceBlue));
    }



    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        container->setPadding(10,AllSides);
        for( auto mah : view[SBLDKeys::Calismalar::mahalleler].get_array().value )
        {
            auto text = container->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(mah.get_string().value.data())));
            text->addStyleClass(Bootstrap::Label::Warning+Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_6);
            text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s16px);
            //            text->setMargin(5,AllSides);
        }
    }


    {
        auto container = TitleContainerColor->addWidget(cpp14::make_unique<WContainerWidget>());
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        std::string iconPaht = this->downloadFileWeb(view[SBLDKeys::Calismalar::icon].get_oid().value.to_string().c_str());
        TitleContainer->setAttributeValue(Style::style,Style::background::url(iconPaht)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        container->setHeight(50);
    }
    {


        auto asamaContainer = mTableContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        try {
            auto asamaList = view[SBLDKeys::Calismalar::asamalar].get_array().value;

            for( auto _asama : asamaList )
            {
                auto doc = _asama.get_document().value;
                auto container = asamaContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                //                container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
                auto StackWidget = container->addWidget(cpp14::make_unique<WStackedWidget>());
                StackWidget->setTransitionAnimation(WAnimation(AnimationEffect::SlideInFromRight,TimingFunction::EaseInOut,250),true);
                std::vector<std::string> imgPathList;
                for( auto id : doc[SBLDKeys::Calismalar::Asama::Gorseller].get_array().value )
                {
                    std::string imgPath = this->downloadFileWeb(id.get_oid().value.to_string().c_str());
                    imgPathList.push_back(imgPath);
                }

                for( auto str : imgPathList )
                {
                    auto slideCOntainer = cpp14::make_unique<WContainerWidget>();
                    slideCOntainer->setHeight(350);



                    auto frontimgContainer = slideCOntainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    frontimgContainer->setHeight(350);
                    frontimgContainer->setAttributeValue(Style::style,Style::background::url(str)+Style::background::size::contain+Style::background::position::center_center+Style::background::repeat::norepeat);

                    auto imgContainer = cpp14::make_unique<WContainerWidget>();
                    imgContainer->setHeight(350);
                    imgContainer->setWidth(WLength("100%"));
                    imgContainer->setAttributeValue(Style::style,Style::background::url(str)+Style::background::size::contain+Style::background::position::center_center+"z-index:-1; ");
                    imgContainer->addStyleClass("CommanFilter");
                    imgContainer->setPositionScheme(PositionScheme::Absolute);
                    imgContainer->setOffsets(0,Side::Top);
                    slideCOntainer->addWidget(std::move(imgContainer));

                    StackWidget->addWidget(std::move(slideCOntainer));

                }


                auto btnContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                btnContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));


                auto hLayout = btnContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

                auto backBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("Geri"));
                backBtn->clicked().connect([=](){
                    int indexCount = StackWidget->count();
                    int CurrentIndex = StackWidget->currentIndex();

                    if( CurrentIndex > 0 )
                    {
                        StackWidget->setCurrentIndex(--CurrentIndex );
                    }else{
                        StackWidget->setCurrentIndex( indexCount-1 );
                    }
                });

                hLayout->addStretch(1);
                int imgCounter = 0;

                for( auto str : imgPathList )
                {
                    imgCounter++;
                }

                {
                    auto text = hLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Resim").arg(imgCounter)),0,AlignmentFlag::Center);
                    text->setAttributeValue(Style::style,Style::font::weight::bold+Style::font::size::s24px);
                }
                hLayout->addStretch(1);



                auto nextBtn = hLayout->addWidget(cpp14::make_unique<WPushButton>("İleri"));

                nextBtn->clicked().connect([=](){
                    int indexCount = StackWidget->count();
                    int CurrentIndex = StackWidget->currentIndex();

                    if( CurrentIndex < indexCount-1 )
                    {
                        StackWidget->setCurrentIndex(++CurrentIndex );
                    }else{
                        StackWidget->setCurrentIndex( 0 );
                    }
                });

                auto TextCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                TextCOntainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
                auto aciklamaText = TextCOntainer->addWidget(cpp14::make_unique<WText>(WString("{1}").arg(doc[SBLDKeys::Calismalar::Asama::Aciklama].get_string().value.data())));
                aciklamaText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::weight::bold);
            }
        } catch (bsoncxx::exception &e) {

        }


    }



}

void Body::Calisma::initPage()
{
    mContentContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    mContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_10);
    mTableContainer = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mTableContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    mTableContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::Snow));



    this->zamanTuneli();

    //    auto filter = document{};

    //    try {
    //        filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
    //    } catch (bsoncxx::exception &e) {

    //    }



    //    TableFilter = filter.extract();

    //    this->initTable(TableFilter.view());
}

void Body::Calisma::initMenu()
{



    mMenuContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    mMenuContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2);
    //    mMenuContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));

    const int height = 50;
    const int margin = 3;
    int red = this->getRandom(0,120);
    int green = this->getRandom(0,120);
    int blue = this->getRandom(0,120);

    {
        auto container = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb( red += 5 , green += 7 , blue += 5 )+Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
        container->setMargin(margin,AllSides);
        container->setHeight(height);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Tüm Çalışmalar: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(document{}.view()))),0,AlignmentFlag::Middle);
        vLayout->addStretch(1);
        container->clicked().connect([=](){
            auto _filter = document{};
            skip = 0;
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            } catch (bsoncxx::exception &e) {

            }
            TableFilter = _filter.extract();
            this->initTable(TableFilter.view());
        });
    }

    {
        auto filter = document{};
        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
        } catch (bsoncxx::exception &e) {

        }
        auto container = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb( red += 5 , green += 7 , blue += 5 )+Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
        container->setMargin(margin,AllSides);
        container->setHeight(height);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        auto text = vLayout->addWidget(cpp14::make_unique<WText>(WString("Devam Eden: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))),0,AlignmentFlag::Middle);
        vLayout->addStretch(1);
        container->clicked().connect([=](){
            skip = 0;
            auto _filter = document{};
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            } catch (bsoncxx::exception &e) {

            }
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
            } catch (bsoncxx::exception &e) {

            }

            TableFilter = _filter.extract();
            this->initTable(TableFilter.view());
        });
    }

    {
        auto filter = document{};
        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
        } catch (bsoncxx::exception &e) {

        }
        auto container = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb( red += 5 , green += 7 , blue += 5 )+Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
        container->setMargin(margin,AllSides);
        container->setHeight(height);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        vLayout->addWidget(cpp14::make_unique<WText>(WString("Tamamlanan: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))),0,AlignmentFlag::Middle);
        vLayout->addStretch(1);
        container->clicked().connect([=](){
            skip = 0;
            auto _filter = document{};
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            } catch (bsoncxx::exception &e) {

            }
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
            } catch (bsoncxx::exception &e) {

            }

            TableFilter = _filter.extract();
            this->initTable(TableFilter.view());
        });
    }


    {
        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-1))));
        } catch (bsoncxx::exception &e) {

        }

        auto container = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb( red += 5 , green += 7 , blue += 5 )+Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
        container->setMargin(margin,AllSides);
        container->setHeight(height);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        vLayout->addWidget(cpp14::make_unique<WText>(WString("Bugün: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))),0,AlignmentFlag::Middle);
        vLayout->addStretch(1);

        container->clicked().connect([=](){
            skip = 0;
            auto _filter = document{};
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            } catch (bsoncxx::exception &e) {

            }
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-1))));
            } catch (bsoncxx::exception &e) {

            }
            TableFilter = _filter.extract();
            this->initTable(TableFilter.view());
        });
    }

    {
        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-7))));
        } catch (bsoncxx::exception &e) {

        }

        auto container = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb( red += 5 , green += 7 , blue += 5 )+Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
        container->setMargin(margin,AllSides);
        container->setHeight(height);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        vLayout->addWidget(cpp14::make_unique<WText>(WString("Son 1 Hafta: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))),0,AlignmentFlag::Middle);
        vLayout->addStretch(1);

        container->clicked().connect([=](){
            skip = 0;
            auto _filter = document{};
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            } catch (bsoncxx::exception &e) {

            }
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-7))));
            } catch (bsoncxx::exception &e) {

            }
            TableFilter = _filter.extract();
            this->initTable(TableFilter.view());
        });
    }

    {
        auto filter = document{};


        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-30))));
        } catch (bsoncxx::exception &e) {

        }

        auto container = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb( red += 5 , green += 7 , blue += 5 )+Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
        container->setMargin(margin,AllSides);
        container->setHeight(height);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        vLayout->addWidget(cpp14::make_unique<WText>(WString("Son 1 Ay: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))),0,AlignmentFlag::Middle);
        vLayout->addStretch(1);
        container->clicked().connect([=](){
            skip = 0;
            auto _filter = document{};
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            } catch (bsoncxx::exception &e) {

            }
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-30))));
            } catch (bsoncxx::exception &e) {

            }
            TableFilter = _filter.extract();
            this->initTable(TableFilter.view());
        });
    }

    {
        auto filter = document{};


        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-90))));
        } catch (bsoncxx::exception &e) {

        }

        auto container = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb( red += 5 , green += 7 , blue += 5 )+Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
        container->setMargin(margin,AllSides);
        container->setHeight(height);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        vLayout->addWidget(cpp14::make_unique<WText>(WString("Son 3 Ay: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))),0,AlignmentFlag::Middle);
        vLayout->addStretch(1);
        container->clicked().connect([=](){
            skip = 0;
            auto _filter = document{};
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            } catch (bsoncxx::exception &e) {

            }
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::currentDate().toJulianDay()-90))));
            } catch (bsoncxx::exception &e) {

            }
            TableFilter = _filter.extract();
            this->initTable(TableFilter.view());
        });
    }



    {
        auto filter = document{};


        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::fromString(QString("01.01.%1").arg(QDate::currentDate().year()),"dd.MM.yyyy").toJulianDay()))));
        } catch (bsoncxx::exception &e) {

        }

        auto container = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb( red += 5 , green += 7 , blue += 5 )+Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
        container->setMargin(margin,AllSides);
        container->setHeight(height);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        vLayout->addWidget(cpp14::make_unique<WText>(WString("Bu Yıl: {1}").arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))),0,AlignmentFlag::Middle);
        vLayout->addStretch(1);
        container->clicked().connect([=](){
            skip = 0;
            auto _filter = document{};
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            } catch (bsoncxx::exception &e) {

            }
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,make_document(kvp("$gte",QDate::fromString(QString("01.01.%1").arg(QDate::currentDate().year()),"dd.MM.yyyy").toJulianDay()))));
            } catch (bsoncxx::exception &e) {

            }
            TableFilter = _filter.extract();
            this->initTable(TableFilter.view());
        });
    }



    for( int yil = QDate::currentDate().year() ; yil > 2014 ; yil-- ){
        auto filter = document{};


        auto gteDoc = document{};

        //        int yil = QDate::currentDate().year()-1;
        try {
            gteDoc.append(kvp("$gte",QDate::fromString(QString("01.01.%1").arg(yil-1),"dd.MM.yyyy").toJulianDay()));
        } catch (bsoncxx::exception &e) {

        }

        try {
            gteDoc.append(kvp("$lte",QDate::fromString(QString("01.01.%1").arg(yil),"dd.MM.yyyy").toJulianDay()));
        } catch (bsoncxx::exception &e) {

        }

        try {
            filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,gteDoc));
        } catch (bsoncxx::exception &e) {

        }

        auto container = mMenuContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue(Style::style,Style::background::color::rgb( red += 5 , green += 7 , blue += 5 )+Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s12px+Style::font::weight::bold);
        container->setMargin(margin,AllSides);
        container->setHeight(height);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto vLayout = container->setLayout(cpp14::make_unique<WVBoxLayout>());
        vLayout->addStretch(1);
        vLayout->addWidget(cpp14::make_unique<WText>(WString("{1} Yılı: {2}").arg(yil-1).arg(this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()))),0,AlignmentFlag::Middle);
        vLayout->addStretch(1);
        container->clicked().connect([=](){
            skip = 0;
            auto _filter = document{};
            try {
                _filter.append(kvp(SBLDKeys::Calismalar::yayin,SBLDKeys::Calismalar::Yayinda::yayinda));
            } catch (bsoncxx::exception &e) {

            }

            auto gteDoc = document{};

            //        int yil = QDate::currentDate().year()-1;
            try {
                gteDoc.append(kvp("$gte",QDate::fromString(QString("01.01.%1").arg(yil-1),"dd.MM.yyyy").toJulianDay()));
            } catch (bsoncxx::exception &e) {

            }

            try {
                gteDoc.append(kvp("$lte",QDate::fromString(QString("01.01.%1").arg(yil),"dd.MM.yyyy").toJulianDay()));
            } catch (bsoncxx::exception &e) {

            }

            try {
                _filter.append(kvp(SBLDKeys::Calismalar::baslamaTarihi,gteDoc));
            } catch (bsoncxx::exception &e) {

            }
            TableFilter = _filter.extract();
            this->initTable(TableFilter.view());

        });
    }


}

void Body::Calisma::initIstatistic()
{
    mTableContainer->clear();

    auto gridContainer = mTableContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    gridContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    auto fluidContainer = gridContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    fluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rowContainer = fluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rowContainer->addStyleClass(Bootstrap::Grid::row);

    {
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
        container->setPadding(5,AllSides);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto textCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        textCOntainer->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        auto vLayout = textCOntainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Birimlere Göre"),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s18px+Style::font::weight::bold);
        textCOntainer->setHeight(100);
        container->clicked().connect([=](){
            this->initIstatisticBirimlereGore();
        });
    }

    {
        auto container = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(0,125),this->getRandom(0,125),this->getRandom(0,125)));
        container->setPadding(5,AllSides);
        container->decorationStyle().setCursor(Cursor::PointingHand);
        auto textCOntainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        textCOntainer->setAttributeValue(Style::style,Style::Border::border("1px solid white"));
        auto vLayout = textCOntainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Mahallelere Göre"),0,AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s18px+Style::font::weight::bold);
        textCOntainer->setHeight(100);
        container->clicked().connect([=](){
            this->initIstatisticMahallereGore();
        });
    }

    mChartesianChartWidget = rowContainer->addWidget(cpp14::make_unique<WContainerWidget>());

    mChartesianChartWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    this->initIstatisticBirimlereGore();


}

void Body::Calisma::initIstatisticBirimlereGore()
{

    mChartesianChartWidget->clear();

    auto container = mChartesianChartWidget->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::container_fluid);

    auto model_ = std::make_shared<WStandardItemModel>();
    auto model = model_.get();
    //    model->setItemPrototype(cpp14::make_unique<NumericItem>());


    model->insertColumns(model->columnCount(), 4);
    model->setHeaderData(0, WString("Birim"));
    model->setHeaderData(1, WString("Toplam"));
    model->setHeaderData(2, WString("Tamamlandı"));
    model->setHeaderData(3, WString("Devam Ediyor"));


    std::vector<std::string> mBirimList;

    {
        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Birimler::haberlesmekodu,make_document(kvp("$ne","0"))));
        } catch (bsoncxx::exception &e) {

        }

        try {
            auto cursor = this->db()->collection(SBLDKeys::Birimler::collection).find(filter.view());

            for( auto doc : cursor )
            {
                std::string str = doc[SBLDKeys::Birimler::birim].get_string().value.data();
                mBirimList.push_back(str);
            }

        } catch (bsoncxx::exception &e) {

        }
    }

    model->insertRows(model->rowCount(), mBirimList.size());
    int row = 0;


    for( auto str : mBirimList )
    {
        QString birim = QString::fromStdString(str);
        birim = birim.remove(birim.count()-9,9);


        model->setData(  row, 0, birim.toStdString());

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::birim,str));
        } catch (bsoncxx::exception &e) {

        }

        model->setData(  row, 1, this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()));

        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
        } catch (bsoncxx::exception &e) {

        }

        model->setData(  row, 2, this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()));


        filter.clear();
        try {
            filter.append(kvp(SBLDKeys::Calismalar::birim,str));
        } catch (bsoncxx::exception &e) {

        }
        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
        } catch (bsoncxx::exception &e) {

        }

        model->setData(  row, 3, this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()));
        row++;
    }

    auto table = mChartesianChartWidget->addWidget(cpp14::make_unique<WTableView>());
    table->setModel(model_);
    table->setSortingEnabled(true);
    table->setColumnResizeEnabled(true);
    table->setAlternatingRowColors(true);
    table->setHeaderAlignment(0, AlignmentFlag::Center);
    table->setColumnAlignment(0, AlignmentFlag::Center);
    table->setRowHeight(28);
    table->setHeaderHeight(28);
    table->setMargin(10, Side::Top | Side::Bottom);
    table->setMargin(WLength::Auto, Side::Left | Side::Right);
    table->setWidth(WLength("100%"));
    table->setColumnWidth(0,250);

    for (int i = 1; i < model->columnCount(); ++i)
        table->setColumnWidth(i, 171);

    /*
     * Create the category chart.
     */
    auto chartCOntainer = mChartesianChartWidget->addWidget(cpp14::make_unique<WContainerWidget>());
    chartCOntainer->setOverflow(Overflow::Auto);
    auto chart = chartCOntainer->addWidget(cpp14::make_unique<Chart::WCartesianChart>());
    chart->setModel(model_);
    chart->setXSeriesColumn(0);
    chart->setOrientation(Orientation::Horizontal);
    chart->setLegendEnabled(true);

    /*
     * Provide ample space for the title, the X and Y axis and the legend.
     */
    chart->setPlotAreaPadding(200, Side::Left );
    chart->setPlotAreaPadding(150, Side::Right );
    chart->setPlotAreaPadding(50, Side::Top);
    chart->setPlotAreaPadding(10, Side::Bottom);

    /*
     * Add all (but first) column as bar series.
     */
    for (int column = 1; column < model->columnCount(); ++column) {
        auto series = cpp14::make_unique<Chart::WDataSeries>(column, Chart::SeriesType::Bar);
        series->setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3));
        series->setLabelsEnabled(Chart::Axis::Ordinate,true);
        series->setLegendEnabled(true);
        if( column == 1 )series->setBrush(WBrush(StandardColor::DarkGray));
        if( column == 2 )series->setBrush(WBrush(StandardColor::DarkGreen));
        if( column == 3 )series->setBrush(WBrush(StandardColor::DarkRed));

        chart->addSeries(std::move(series));
    }

    chart->resize(800, 1500);
    chart->setMargin(WLength::Auto, Side::Left | Side::Right);

    //    chart->setAttributeValue(Style::style,Style::Border::border("1px solid red"));

}

void Body::Calisma::initIstatisticMahallereGore()
{

    mChartesianChartWidget->clear();

    auto container = mChartesianChartWidget->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::container_fluid);

    auto model_ = std::make_shared<WStandardItemModel>();
    auto model = model_.get();
    //    model->setItemPrototype(cpp14::make_unique<NumericItem>());


    model->insertColumns(model->columnCount(), 4);
    model->setHeaderData(0, WString("Mahalle"));
    model->setHeaderData(1, WString("Toplam"));
    model->setHeaderData(2, WString("Tamamlandı"));
    model->setHeaderData(3, WString("Devam Ediyor"));


    std::vector<std::string> mMahalleList;

    {
        auto filter = document{};



        try {
            auto cursor = this->db()->collection(SBLDKeys::Mahalle::collection).find(filter.view());

            for( auto doc : cursor )
            {
                std::string str = doc[SBLDKeys::Mahalle::mahalle].get_string().value.data();
                mMahalleList.push_back(str);
            }

        } catch (bsoncxx::exception &e) {

        }
    }

    model->insertRows(model->rowCount(), mMahalleList.size());
    int row = 0;


    for( auto str : mMahalleList )
    {
        QString birim = QString::fromStdString(str);
        birim = birim.remove(birim.count()-9,9);


        model->setData(  row, 0, birim.toStdString());

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::mahalleler,str));
        } catch (bsoncxx::exception &e) {

        }

        model->setData(  row, 1, this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()));

        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
        } catch (bsoncxx::exception &e) {

        }

        model->setData(  row, 2, this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()));


        filter.clear();
        try {
            filter.append(kvp(SBLDKeys::Calismalar::mahalleler,str));
        } catch (bsoncxx::exception &e) {

        }
        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
        } catch (bsoncxx::exception &e) {

        }

        model->setData(  row, 3, this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()));
        row++;
    }

    auto table = mChartesianChartWidget->addWidget(cpp14::make_unique<WTableView>());
    table->setModel(model_);
    table->setSortingEnabled(true);
    table->setColumnResizeEnabled(true);
    table->setAlternatingRowColors(true);
    table->setHeaderAlignment(0, AlignmentFlag::Center);
    table->setColumnAlignment(0, AlignmentFlag::Center);
    table->setRowHeight(28);
    table->setHeaderHeight(28);
    table->setMargin(10, Side::Top | Side::Bottom);
    table->setMargin(WLength::Auto, Side::Left | Side::Right);
    table->setWidth(WLength("100%"));
    table->setColumnWidth(0,130);

    for (int i = 1; i < model->columnCount(); ++i)
        table->setColumnWidth(i, 70);

    /*
     * Create the category chart.
     */
    auto chartCOntainer = mChartesianChartWidget->addWidget(cpp14::make_unique<WContainerWidget>());
    chartCOntainer->setOverflow(Overflow::Auto);
    auto chart = chartCOntainer->addWidget(cpp14::make_unique<Chart::WCartesianChart>());
    chart->setModel(model_);
    chart->setXSeriesColumn(0);
    chart->setOrientation(Orientation::Horizontal);
    chart->setLegendEnabled(true);

    /*
     * Provide ample space for the title, the X and Y axis and the legend.
     */
    chart->setPlotAreaPadding(200, Side::Left );
    chart->setPlotAreaPadding(150, Side::Right );
    chart->setPlotAreaPadding(50, Side::Top);
    chart->setPlotAreaPadding(10, Side::Bottom);

    /*
     * Add all (but first) column as bar series.
     */
    for (int column = 1; column < model->columnCount(); ++column) {
        auto series = cpp14::make_unique<Chart::WDataSeries>(column, Chart::SeriesType::Bar);
        series->setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3));
        series->setLabelsEnabled(Chart::Axis::Ordinate,true);
        series->setLegendEnabled(true);
        if( column == 1 )series->setBrush(WBrush(StandardColor::DarkGray));
        if( column == 2 )series->setBrush(WBrush(StandardColor::DarkGreen));
        if( column == 3 )series->setBrush(WBrush(StandardColor::DarkRed));

        chart->addSeries(std::move(series));
    }

    chart->resize(800, 4000);
    chart->setMargin(WLength::Auto, Side::Left | Side::Right);

}

void Body::Calisma::initIstatisticBirimMahalle()
{

    mChartesianChartWidget->clear();

    auto container = mChartesianChartWidget->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::container_fluid);

    auto model_ = std::make_shared<WStandardItemModel>();
    auto model = model_.get();
    //    model->setItemPrototype(cpp14::make_unique<NumericItem>());


    model->insertColumns(model->columnCount(), 4);
    model->setHeaderData(0, WString("Birim"));
    model->setHeaderData(1, WString("Toplam"));
    model->setHeaderData(2, WString("Tamamlandı"));
    model->setHeaderData(3, WString("Devam Ediyor"));


    std::vector<std::string> mBirimList;

    {
        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Birimler::haberlesmekodu,make_document(kvp("$ne","0"))));
        } catch (bsoncxx::exception &e) {

        }

        try {
            auto cursor = this->db()->collection(SBLDKeys::Birimler::collection).find(filter.view());

            for( auto doc : cursor )
            {
                std::string str = doc[SBLDKeys::Birimler::birim].get_string().value.data();
                mBirimList.push_back(str);
            }

        } catch (bsoncxx::exception &e) {

        }
    }

    model->insertRows(model->rowCount(), mBirimList.size());
    int row = 0;


    for( auto str : mBirimList )
    {
        QString birim = QString::fromStdString(str);
        birim = birim.remove(birim.count()-9,9);


        model->setData(  row, 0, birim.toStdString());

        auto filter = document{};

        try {
            filter.append(kvp(SBLDKeys::Calismalar::birim,str));
        } catch (bsoncxx::exception &e) {

        }

        model->setData(  row, 1, this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()));

        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::tamamlandi));
        } catch (bsoncxx::exception &e) {

        }

        model->setData(  row, 2, this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()));


        filter.clear();
        try {
            filter.append(kvp(SBLDKeys::Calismalar::birim,str));
        } catch (bsoncxx::exception &e) {

        }
        try {
            filter.append(kvp(SBLDKeys::Calismalar::durum,SBLDKeys::Calismalar::DURUM::devamediyor));
        } catch (bsoncxx::exception &e) {

        }

        model->setData(  row, 3, this->db()->collection(SBLDKeys::Calismalar::collection).count_documents(filter.view()));
        row++;
    }

    auto table = mChartesianChartWidget->addWidget(cpp14::make_unique<WTableView>());
    table->setModel(model_);
    table->setSortingEnabled(true);
    table->setColumnResizeEnabled(true);
    table->setAlternatingRowColors(true);
    table->setHeaderAlignment(0, AlignmentFlag::Center);
    table->setColumnAlignment(0, AlignmentFlag::Center);
    table->setRowHeight(28);
    table->setHeaderHeight(28);
    table->setMargin(10, Side::Top | Side::Bottom);
    table->setMargin(WLength::Auto, Side::Left | Side::Right);
    table->setWidth(WLength("100%"));
    table->setColumnWidth(0,250);

    for (int i = 1; i < model->columnCount(); ++i)
        table->setColumnWidth(i, 171);

    /*
     * Create the category chart.
     */
    auto chartCOntainer = mChartesianChartWidget->addWidget(cpp14::make_unique<WContainerWidget>());
    chartCOntainer->setOverflow(Overflow::Auto);
    auto chart = chartCOntainer->addWidget(cpp14::make_unique<Chart::WCartesianChart>());
    chart->setModel(model_);
    chart->setXSeriesColumn(0);
    chart->setOrientation(Orientation::Horizontal);
    chart->setLegendEnabled(true);

    /*
     * Provide ample space for the title, the X and Y axis and the legend.
     */
    chart->setPlotAreaPadding(200, Side::Left );
    chart->setPlotAreaPadding(150, Side::Right );
    chart->setPlotAreaPadding(50, Side::Top);
    chart->setPlotAreaPadding(10, Side::Bottom);

    /*
     * Add all (but first) column as bar series.
     */
    for (int column = 1; column < model->columnCount(); ++column) {
        auto series = cpp14::make_unique<Chart::WDataSeries>(column, Chart::SeriesType::Bar);
        series->setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3));
        series->setLabelsEnabled(Chart::Axis::Ordinate,true);
        series->setLegendEnabled(true);
        if( column == 1 )series->setBrush(WBrush(StandardColor::DarkGray));
        if( column == 2 )series->setBrush(WBrush(StandardColor::DarkGreen));
        if( column == 3 )series->setBrush(WBrush(StandardColor::DarkRed));

        chart->addSeries(std::move(series));
    }

    chart->resize(800, 1500);
    chart->setMargin(WLength::Auto, Side::Left | Side::Right);


}

Body::BaskanaMesajWidget::BaskanaMesajWidget(mongocxx::database *_database)
    :SerikBLDCore::DB(_database)
{

    this->clear();
    this->setMargin(25,Side::Top|Side::Bottom);
    this->setAttributeValue(Style::style,Style::Border::border("1px solid black"));


    auto container = this->addWidget(cpp14::make_unique<WContainerWidget>());

    container->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = container->addWidget(cpp14::make_unique<WContainerWidget>());

    row->addStyleClass(Bootstrap::Grid::row);

    this->initializetion(row);




}

void Body::BaskanaMesajWidget::initializetion(WContainerWidget *row)
{

    row->clear();


    {
        auto TitleContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        TitleContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto text = TitleContainer->addWidget(cpp14::make_unique<WText>("Başkana Mesaj Gönder"));
        text->setMargin(WLength::Auto,AllSides);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);
        text->setMargin(10,Side::Bottom|Side::Top);
    }


    // Ad Soyad

    auto adSoyadContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    adSoyadContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    auto adSoyadLineEdit = adSoyadContainer->addWidget(cpp14::make_unique<WLineEdit>());
    adSoyadLineEdit->setMargin(WLength::Auto,AllSides);
    adSoyadLineEdit->setPlaceholderText("Adınızı/Soyadınızı Yazınız");
    adSoyadLineEdit->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);



    // Telefon Numarası

    auto TelContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    TelContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    auto TelText = TelContainer->addWidget(cpp14::make_unique<WLineEdit>());
    TelText->setMargin(WLength::Auto,AllSides);
    TelText->setPlaceholderText("Telefon Numarası Giriniz");
    TelText->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);




    // Mesaj

    auto MesajContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
    MesajContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    auto MesajText = MesajContainer->addWidget(cpp14::make_unique<WLineEdit>());
    MesajText->setMargin(WLength::Auto,AllSides);
    MesajText->setPlaceholderText("Mesajınızı Yazınız");
    MesajText->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::bold);



    // Gönder
    {
        auto TitleContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        TitleContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto btn = TitleContainer->addWidget(cpp14::make_unique<WPushButton>("Gönder"));
        btn->setMargin(WLength::Auto,AllSides);
        btn->addStyleClass(Bootstrap::Button::Primary);

        btn->clicked().connect([=](){

            if( !adSoyadLineEdit->text().toUTF8().size() )
            {
                this->showMessage("Hata","Adınızı ve Soyadınızı Girmediniz");
                return;
            }

            if( !TelText->text().toUTF8().size() )
            {
                this->showMessage("Hata","Telefon Numaranızı Girmediniz");
                return;
            }

            if( MesajText->text().toUTF8().size() < 50 )
            {
                std::cout << MesajText->text().toUTF8() << std::endl;

                this->showMessage("Hata",WString("Mesajınızı Anlaşılabilir Olması için Minimum 50 Karakter Olarak Giriniz. {1} Girdiniz").arg(MesajText->text().toUTF8().size()).toUTF8());
                return;
            }

            auto doc = document{};

            try {
                doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::julianDate,bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()}));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e.what());
                return;
            }

            try {
                doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::saat,WTime::currentServerTime().toString("hh:mm").toUTF8()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e.what());
                return;
            }

            try {
                doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::ad,adSoyadLineEdit->text().toUTF8()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e.what());
                return;
            }

            try {
                doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::tel,TelText->text().toUTF8()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e.what());
                return;
            }

            try {
                doc.append(kvp(SBLDKeys::Yonetim::Baskan::MESAJ::mesaj,MesajText->text().toUTF8()));
            } catch (bsoncxx::exception &e) {
                this->showMessage("Hata","Error: " , e.what());
                return;
            }


            try {
                auto ins = this->db()->collection(SBLDKeys::Yonetim::Baskan::MESAJ::collection).insert_one(doc.view());
                if( ins )
                {
                    if( ins.value().result().inserted_count() )
                    {
                        this->showMessage("Bilgi","Belirttiğiniz Telefon Numarasından Size Geri Dönüş Yapılacaktır. İlginiz İçin Teşekkürler","Tamam");
                        this->initializetion(row);
                    }else{
                        this->showMessage("Hata","Bilgiler Kayıt Edil","Tamam");
                    }
                }else{
                    this->showMessage("Bilgi","Belirttiğiniz Telefon Numarasından Size Geri Dönüş Yapılacaktır. İlginiz İçin Teşekkürler","Tamam");
                }
            } catch (mongocxx::exception &e) {
                this->showMessage("Hata","Bir Hata Oluştu");
            }




        });
    }

}

Body::BilgiEdin::KamuHizmetRapor::KamuHizmetRapor()
{
    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

    {
        auto titleContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
        auto text = titleContainer->addWidget(cpp14::make_unique<WText>("Kamu Hizmet Standartları"));
        text->setAttributeValue(Style::style,Style::font::size::s20px+Style::font::weight::bold);
        titleContainer->setMargin(25,Side::Bottom|Side::Top);
    }


    auto controlWidget = row->addWidget(cpp14::make_unique<WContainerWidget>());
    controlWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

    {
        auto layout = controlWidget->setLayout(cpp14::make_unique<WVBoxLayout>());
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/yazi_isleri_mudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Yazı İşleri Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        //veteriner_isleri_mudurlugu.pdf
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/veteriner_isleri_mudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Veteriner İşleri Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        //temizlik_isleri_mudurlugu
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/temizlik_isleri_mudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Temizlik İşleri Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/fen_isleri_mudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Fen İşleri Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/insankaynaklariveegitimmudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "İnsan Kaynakları ve Eğitim Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/sporislerimudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Spor İşleri Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        //bilgiislemmudurlugu
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/bilgiislemmudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Bilgi İşlem Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        //parkvebahcelermudurlugu
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/parkvebahcelermudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Park ve Bahçeler Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

//        //cevrekorumakontrolmudurlugu
//        {
//            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/cevrekorumakontrolmudurlugu.pdf");
//            link.setTarget(Wt::LinkTarget::NewWindow);

//            std::unique_ptr<Wt::WAnchor> anchor =
//                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
//                                                        "Çevre Koruma ve Kontrol Müdürlüğü Hizmet Tablosu");
//            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
//            layout->addWidget(std::move(anchor));
//        }

        //zabitaruhsat
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/zabitaruhsat.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Zabıta Müdürlüğü/Ruhsat Kısım Amirliği Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        //kultursosyal
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/kultursosyal.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Kültür ve Sosyal Hizmetler Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        //hukukislerimudurlugu
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/hukukislerimudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Hukuk İşleri Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        //imarvesehircilikmudurlugu
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/imarvesehircilikmudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "İmar ve Şehircilik Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        //destekhizmetleri
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/destekhizmetleri.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Destek Hizmetleri Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }
        //muhtarlikislerimudurlugu
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/muhtarlikislerimudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Muhtarlık İşleri Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        //kadinailehizmetleri
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/kadinailehizmetleri.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Kadın ve Aile Hizmetleri Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        //malihizmetler
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/malihizmetler.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Mali Hizmetler Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

        //ozelkalemmudurlugu
        {
            Wt::WLink link = Wt::WLink("http://www.serik.bel.tr/file/kamu/ozelkalemmudurlugu.pdf");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                        "Özel Kalem Müdürlüğü Hizmet Tablosu");
            anchor->setAttributeValue(Style::style,Style::font::size::s20px);
            layout->addWidget(std::move(anchor));
        }

    }
}


