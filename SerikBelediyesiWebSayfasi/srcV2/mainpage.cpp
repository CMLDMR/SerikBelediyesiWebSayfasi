#include "mainpage.h"
#include "slider.h"
#include "mainpagecontroller.h"
#include "SerikBelediyesiWebSayfasi/src/body.h"
#include "SerikBelediyesiWebSayfasi/src/giriswidget.h"

MainPage::MainPage(mongocxx::database *_db)
    :DataBaseWidget (_db)
{
    auto header = addWidget(cpp14::make_unique<HeaderPage>(this->getDB()));
    header->setZIndex(12);
    header->addStyleClass("header");

    mContentWidget = addWidget(cpp14::make_unique<WContainerWidget>());
    this->init();

    header->ClickAnaSayfa().connect([=](){
       this->init();
    });

    header->ClickHaber().connect(this,&MainPage::initHaberler);
    header->ClickCalisma().connect(this,&MainPage::initCalismalar);
    header->ClickProjeler().connect(this,&MainPage::initProjeler);
    header->ClickEtkinlikler().connect(this,&MainPage::initEtkinlikler);
    header->ClickBilgiEdinme().connect(this,&MainPage::initBilgiEdinme);
    header->ClickGiris().connect(this,&MainPage::initGiris);
    header->ClickMeclis().connect(this,&MainPage::initMeclis);
    header->ClickHakkinda().connect(this,&MainPage::initHakkinda);
    header->ClickIletisim().connect(this,&MainPage::initIletisim);
}

void MainPage::init()
{

    mContentWidget->clear();

    auto slider = mContentWidget->addWidget(cpp14::make_unique<Slider>(this->getDB()));
    slider->setPositionScheme(PositionScheme::Absolute);
    slider->setWidth(WLength("100%"));
    slider->setAttributeValue(Style::style,"z-index:-1;");
    slider->setId("sliderid");
    slider->setHeight(768);

    {
        auto container = mContentWidget->addWidget(cpp14::make_unique<WContainerWidget>());
        container->setHeight(800);
        container->setContentAlignment(AlignmentFlag::Center);
//        container->decorationStyle().setBorder(WBorder(BorderStyle::Solid,BorderWidth::Medium,WColor(0,255,0)));


        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setPositionScheme(PositionScheme::Relative);
        _container->setHeight(768);
        _container->setWidth(WLength::Auto);
        _container->setMaximumSize(1270,WLength::Auto);
//        _container->decorationStyle().setBorder(WBorder(BorderStyle::Solid,BorderWidth::Medium,WColor(255,0,0)));

        auto content = _container->addWidget(cpp14::make_unique<WContainerWidget>());
        content->setPositionScheme(PositionScheme::Absolute);

        content->setWidth(550);
        content->setHeight(250);
        content->setOffsets(0,Side::Bottom);
        content->setOffsets(0,Side::Right);

        auto rContainer = content->addWidget(cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass(Bootstrap::Grid::row);

        {
            auto fotoContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            fotoContainer->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_4);
            fotoContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_8);
            fotoContainer->setHeight(250);

            auto foto = fotoContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            foto->setHeight(250);

            foto->setAttributeValue(Style::style,Style::background::url("v2/baskan/baskan1.png")+
                                             Style::background::repeat::norepeat+
                                             Style::background::size::contain+
                                             Style::background::position::center_center);
        }
    }

    {
        auto controller = mContentWidget->addWidget(cpp14::make_unique<MainPageController>(this->getDB()));
        controller->ClickHaber().connect(this,&MainPage::initHaberler);
        controller->ClickCalisma().connect(this,&MainPage::initCalismalar);
        controller->ClickProjeler().connect(this,&MainPage::initProjeler);
        controller->ClickEtkinlikler().connect(this,&MainPage::initEtkinlikler);
        controller->ClickBilgiEdinme().connect(this,&MainPage::initBilgiEdinme);
        controller->ClickGiris().connect(this,&MainPage::initGiris);
        controller->ClickMeclis().connect(this,&MainPage::initMeclis);
        controller->ClickAnounce().connect(this,&MainPage::initAnounceDetail);
    }
}

void MainPage::initHaberler()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Haber>(this->getDB()));
}

void MainPage::initCalismalar()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Calisma>(this->getDB()));
}

void MainPage::initProjeler()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Proje>(this->getDB()));
}

void MainPage::initEtkinlikler()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Etkinlik>(this->getDB()));
}

void MainPage::initBilgiEdinme()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::BilgiEdin::BilgiEdin>(this->getDB()));
}

void MainPage::initGiris()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Giris::GirisWidget>(this->getDB()));
}

void MainPage::initMeclis()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Meclis>(this->getDB()));
}

void MainPage::initHakkinda()
{
    mContentWidget->clear();
    auto widget = mContentWidget->addWidget(cpp14::make_unique<Body::Serik::Hakkinda>(this->getDB()));
}

void MainPage::initIletisim()
{
    mContentWidget->clear();


//    mMainContainer->clear();
    wApp->setInternalPath("/initiletisim",false);

    //    auto detailContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    //    detailContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    //    detailContainer->setPadding(0,AllSides);

    //    auto layout = detailContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    //    layout->setContentsMargins(0,0,0,0);
    //    auto container = layout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Center);

    mContentWidget->setContentAlignment(AlignmentFlag::Center);
    auto container = mContentWidget->addWidget(cpp14::make_unique<WContainerWidget>());

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
        title->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));
    }


    {
        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        //        _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
        //            this->_clickBack.emit(NoClass());
        //        });
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));


    }


    {
        std::string path = "img/map.jpg";
        auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
        img->addStyleClass(Bootstrap::Grid::container_fluid);
        img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
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


        auto text1 = layout->addWidget(cpp14::make_unique<WText>("<h2>Adres: Orta Mahalle, Kızderesi Cad No:25, 07500 Serik/Antalya</h2>",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
        auto text2 = layout->addWidget(cpp14::make_unique<WText>("İletişim: 444 9 722",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);

        auto text3 = layout->addWidget(cpp14::make_unique<WText>("Fax: +90 242 722 19 68",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);

        auto text4 = layout->addWidget(cpp14::make_unique<WText>("mail: serik@serik.bel.tr",TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);

    }


    {
        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        //        _layout->addWidget(cpp14::make_unique<WPushButton>("Geri"),0,AlignmentFlag::Left)->clicked().connect([=](){
        //            this->_clickBack.emit(NoClass());
        //        });
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                      Style::background::color::color(Style::color::Grey::Gainsboro));
    }




    // Talep Sayfası
    {
        auto talep = container->addWidget(cpp14::make_unique<Body::Talep>(this->getDB()));
        talep->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
    }
}

void MainPage::initAnounceDetail( std::string mOid )
{
    mContentWidget->clear();

    auto mMainContainer = mContentWidget->addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->setMargin( 90 , Side::Top );
    mMainContainer->setContentAlignment(AlignmentFlag::Center);


    auto row = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);
    row->setMaximumSize(1270,WLength::Auto);

    WText* announceTitle = nullptr;;
    WText* AnnounceContent = nullptr;
    WText* LastDate = nullptr;
    WText* Department = nullptr;
    {
        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setPadding(0,AllSides);
        auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
        img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        img->setAttributeValue(Style::style,Style::background::url("img/duyuru1.JPG")+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
        img->setHeight(150);
        img->setPadding(0,AllSides);

        auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
        gradientContainer->setHeight(150);
        gradientContainer->addStyleClass("SliderDetailTextBackground");
        auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
        layout->addStretch(1);
        announceTitle = layout->addWidget(cpp14::make_unique<WText>(""),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
        announceTitle->setAttributeValue(Style::style,Style::font::size::s24px+Style::color::color(Style::color::White::WhiteSmoke));
        wApp->setInternalPath("/"+announceTitle->text().toUTF8(),false);
    }

    auto list = row->addWidget(cpp14::make_unique<Body::NewsAnnounceContent::AnnouncePanel::AnnounceList>(this->getDB()));
    list->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_12);
    list->mGetOid().connect([=](std::string _mOid){
       this->initAnounceDetail(_mOid);
    });



    auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::Large::col_lg_9+Bootstrap::Grid::Medium::col_md_9+Bootstrap::Grid::Small::col_sm_10+Bootstrap::Grid::ExtraSmall::col_xs_12);

    container->setPadding(0,AllSides);


    auto filter = document{};


    try {
        filter.append(kvp(SBLDKeys::Duyurular::oid,bsoncxx::oid{mOid}));
    } catch (bsoncxx::exception &e) {
        container->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }


    try {
        mongocxx::stdx::optional<bsoncxx::document::value> val = this->collection("Duyurular").find_one(filter.view());
        if( !val )
        {
            container->addWidget(cpp14::make_unique<WText>(WString("Empty Document")));
            return;
        }else{

            auto view = val.value().view();

            announceTitle->setText(view[SBLDKeys::Duyurular::title].get_utf8().value.to_string().c_str());

            {
                try {
                    auto array = view[SBLDKeys::Duyurular::fileList].get_array().value;
                    for( auto doc : array )
                    {
                        downloadifNotExist(doc.get_oid().value.to_string(),true);
                    }
                } catch (bsoncxx::exception &e) {
                    std::cout << "Error: No Array in Duyuru Item: " << e.what() << std::endl;
                }

            }


            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
//                auto back = _layout->addWidget(cpp14::make_unique<WPushButton>("Ana Sayfa"),0,AlignmentFlag::Left);
//                back->clicked().connect([=](){
//                    this->_clickBack.emit(NoClass());
//                });
//                back->addStyleClass(Bootstrap::Button::Primary);

                auto textContainer = _layout->addWidget(cpp14::make_unique<WContainerWidget>());

                auto _Llayout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

                Department = _Llayout->addWidget(cpp14::make_unique<WText>("<b>"+view[SBLDKeys::Duyurular::department].get_utf8().value.to_string()+"</b>"));


                LastDate = _Llayout->addWidget(cpp14::make_unique<WText>("Son Yayınlanma  Tarihi:"+QDate::fromString(QString::number((int)view[SBLDKeys::Duyurular::endDate].get_double().value),"yyyyMMdd").toString("dddd dd/MM/yyyy").toStdString()));

                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }


            {
                auto textContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
                textContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                textContainer->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                                 Style::background::color::color(Style::color::White::Snow));

                auto layout = textContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
                AnnounceContent = layout->addWidget(cpp14::make_unique<WText>(view[SBLDKeys::Duyurular::html].get_utf8().value.to_string().c_str(),TextFormat::UnsafeXHTML),0,AlignmentFlag::Top);
            }


            {
                auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
                _container->setHeight(20);
                _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                              Style::background::color::color(Style::color::Grey::Gainsboro));
            }


        }
    } catch (mongocxx::exception &e) {
        mMainContainer->addWidget(cpp14::make_unique<WText>(WString("Error: {1}").arg(e.what())));
        return;
    }






//    if( !AnnounceList ) return;
//    AnnounceList->mGetOid().connect([=](std::string oid){
//        auto filter = document{};
//        try {
//            filter.append(kvp(SBLDKeys::Duyurular::oid,bsoncxx::oid{oid}));
//        } catch (bsoncxx::exception &e) {
//            return;
//        }
//        try {
//            mongocxx::stdx::optional<bsoncxx::document::value> val = DuyurularCollection.find_one(filter.view());
//            if( !val )
//            {
//                return;
//            }else{
//                auto view = val.value().view();
//                if( announceTitle )
//                {
//                    announceTitle->setText(view[SBLDKeys::Duyurular::title].get_utf8().value.to_string().c_str());
//                }
//                if( AnnounceContent )
//                {
//                    AnnounceContent->setText(view[SBLDKeys::Duyurular::html].get_utf8().value.to_string().c_str());
//                }

//                if( Department )
//                {
//                    Department->setText("<b>"+view[SBLDKeys::Duyurular::department].get_utf8().value.to_string()+"</b>");
//                }

//                if( LastDate )
//                {
//                    LastDate->setText("Son Yayınlanma  Tarihi:"+QDate::fromString(QString::number((int)view[SBLDKeys::Duyurular::endDate].get_double().value),"yyyyMMdd").toString("dddd dd/MM/yyyy").toStdString());
//                }

//            }
//        } catch (mongocxx::exception &e) {
//            return;
//        }
//    });

}

std::string MainPage::downloadifNotExist(bsoncxx::types::value oid, bool forceFilename)
{

    auto downloader = this->bucket().open_download_stream(oid);
    auto file_length = downloader.file_length();
    auto bytes_counter = 0;

    QFileInfo info( downloader.files_document()["filename"].get_utf8().value.to_string().c_str() );

    QString fullFilename;

    if( forceFilename )
    {
        fullFilename = QString("tempfile/%1").arg(downloader.files_document()["filename"].get_utf8().value.to_string().c_str());
    }else{
        fullFilename = QString("tempfile/%2.%1").arg(info.suffix())
                .arg(downloader.files_document()["oid"].get_oid().value.to_string().c_str());
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

    //    std::cout << "Current Dir: " << QDir::currentPath().toStdString() << std::endl;
    //    std::cout << "file Size: " << mainArray.size() << std::endl;
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

std::string MainPage::downloadifNotExist(std::string oid, bool forceFilename)
{
    auto doc = bsoncxx::builder::basic::document{};

    try {
        doc.append(bsoncxx::builder::basic::kvp("key",bsoncxx::oid{oid}.to_string()));
    } catch (bsoncxx::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return "NULL";
    }
    auto downloader = this->getDB()->gridfs_bucket().open_download_stream(bsoncxx::types::value(doc.view()["key"].get_oid()));

    auto file_length = downloader.file_length();
    auto bytes_counter = 0;

    QFileInfo info( downloader.files_document()["filename"].get_utf8().value.to_string().c_str() );

    QString fullFilename;

    if( forceFilename )
    {
        fullFilename = QString("tempfile/%1").arg(downloader.files_document()["filename"].get_utf8().value.to_string().c_str());
    }else{
        fullFilename = QString("tempfile/%2.%1").arg(info.suffix())
                .arg(downloader.files_document()["oid"].get_oid().value.to_string().c_str());
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

    //    std::cout << "Current Dir: " << QDir::currentPath().toStdString() << std::endl;
    //    std::cout << "file Size: " << mainArray.size() << std::endl;
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
