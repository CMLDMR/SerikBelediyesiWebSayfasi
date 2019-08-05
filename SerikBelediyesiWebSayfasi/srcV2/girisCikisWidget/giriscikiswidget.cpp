#include "giriscikiswidget.h"

GirisCikisWidget::GirisCikisWidget(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db,_user,"Giriş Çıkış Kayıtları")
{

    setPadding(20,Side::Bottom);
    if( this->User().view()["Statü"].get_utf8().value.to_string() == "Personel" )
    {
        this->initPersonelMenu();
    }else if ( this->User().view()["Statü"].get_utf8().value.to_string() == "Müdür" ) {
        this->initPersonelMenu();
    }else if ( this->User().view()["Statü"].get_utf8().value.to_string() == "Başkan Yardımcısı" || this->User().view()["Statü"].get_utf8().value.to_string() == "Başkan" ) {
        this->initBaskanMenu();
    }




}

void GirisCikisWidget::initGirisCikis()
{
    mContentContainer->clear();

    auto year = QString::fromStdString(mYearSelect->currentText().toUTF8()).toInt();
    auto mounth = mMounthSelect->currentIndex()+1;


    QDate date;
    date.setDate(year,mounth,1);

    for( int i = 1 ; i <= date.daysInMonth() ; i++ )
    {
        this->addRow(QDate(year,mounth,i));
    }

}

void GirisCikisWidget::initPersonelMenu()
{

    auto rContainer = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    rContainer->setMargin(15,Side::Top|Side::Bottom);
    rContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,255),this->getRandom(200,255),this->getRandom(200,255)));
    rContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
        mYearSelect = container->addWidget(cpp14::make_unique<WComboBox>());
        mYearSelect->addItem("2019");
        mYearSelect->addItem("2020");
        mYearSelect->addItem("2021");
        mYearSelect->addItem("2022");
        mYearSelect->addItem("2023");
        mYearSelect->addItem("2024");
    }


    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
        mMounthSelect = container->addWidget(cpp14::make_unique<WComboBox>());
        mMounthSelect->addItem("Ocak");
        mMounthSelect->addItem("Şubat");
        mMounthSelect->addItem("Mart");
        mMounthSelect->addItem("Nisan");
        mMounthSelect->addItem("Mayıs");
        mMounthSelect->addItem("Haziran");
        mMounthSelect->addItem("Temmuz");
        mMounthSelect->addItem("Ağustos");
        mMounthSelect->addItem("Eylül");
        mMounthSelect->addItem("Ekim");
        mMounthSelect->addItem("Kasım");
        mMounthSelect->addItem("Aralık");
    }


    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
        mGosterBtn = container->addWidget(cpp14::make_unique<WPushButton>("Göster"));

    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        mContentContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    }

    mGosterBtn->clicked().connect(this,&GirisCikisWidget::initGirisCikis);
}

void GirisCikisWidget::initBaskanMenu()
{

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top|Side::Bottom);

        mCurrentBirim = container->addWidget(cpp14::make_unique<WComboBox>());
        mCurrentBirim->addItem("Tüm Birim");

        try {

            auto cursor = this->db()->collection("Müdürlükler").find(document{}.view());

            for( auto doc : cursor )
            {
                try {
                    mCurrentBirim->addItem(doc["Birim"].get_utf8().value.to_string());
                } catch (bsoncxx::exception &e) {

                }
            }

        } catch (mongocxx::exception &e) {

        }


    }

    //    {
    //        auto container = this->getRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
    //        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
    //        mBugunBtn = container->addWidget(cpp14::make_unique<WPushButton>("Bugün"));
    //        mBugunBtn->clicked().connect(this,&GirisCikisWidget::initBugun);
    //    }


    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setContentAlignment(AlignmentFlag::Center);
        container->setMargin(10,Side::Top|Side::Bottom);
        auto _container = container->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->setMaximumSize(300,WLength::Auto);
        _container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

        mCalender = _container->addWidget(Wt::cpp14::make_unique<Wt::WCalendar>());
        mCalender->setMargin(WLength::Auto,Side::Left|Side::Right);
        mCalender->select(WDate::currentDate());
        mCalender->clicked().connect([=](WDate date){
            this->initSelectedDay(date.toJulianDay());
        });

    }

    {
        auto container = this->getContentRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        mContentContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    }


}

void GirisCikisWidget::initBugun()
{
    auto filter = document{};

    try {
        filter.append(kvp("day",bsoncxx::types::b_int64{QDate::currentDate().toJulianDay()}));
    } catch (bsoncxx::exception &e) {

    }

    if( mCurrentBirim->currentText().toUTF8() != "Tüm Birim" )
    {
        try {
            filter.append(kvp("Birimi",mCurrentBirim->currentText().toUTF8()));
        } catch (bsoncxx::exception &e) {

        }
    }


    QStringList poillist;

    try {

        auto cursor = this->db()->collection("INOUT").find(filter.view());

        for( auto doc : cursor )
        {

            try {
                poillist.push_back(doc["personeloid"].get_oid().value.to_string().c_str());
            } catch (bsoncxx::exception &e) {

            }

        }

        poillist.removeDuplicates();

    } catch (mongocxx::exception &e) {

    }
    mContentContainer->clear();
    for( auto oid : poillist )
    {
        this->addRow( oid , QDate::currentDate().toJulianDay() );
    }
}

void GirisCikisWidget::initSelectedDay(qint64 julianDay)
{
    auto filter = document{};

    try {
        filter.append(kvp("day",bsoncxx::types::b_int64{julianDay}));
    } catch (bsoncxx::exception &e) {

    }

    if( mCurrentBirim->currentText().toUTF8() != "Tüm Birim" )
    {
        try {
            filter.append(kvp("Birimi",mCurrentBirim->currentText().toUTF8()));
        } catch (bsoncxx::exception &e) {

        }
    }


    QStringList poillist;

    try {

        auto cursor = this->db()->collection("INOUT").find(filter.view());

        for( auto doc : cursor )
        {

            try {
                poillist.push_back(doc["personeloid"].get_oid().value.to_string().c_str());
            } catch (bsoncxx::exception &e) {

            }

        }

        poillist.removeDuplicates();

    } catch (mongocxx::exception &e) {

    }
    mContentContainer->clear();
    {
        auto container = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(10,Side::Top|Side::Bottom);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        container->setHeight(20);
        container->setAttributeValue(Style::style,Style::background::color::rgb(125,175,125));
    }
    for( auto oid : poillist )
    {
        this->addRow( oid , julianDay );
    }


    filter.clear();

    if( mCurrentBirim->currentText().toUTF8() != "Tüm Birim" )
    {
        try {
            filter.append(kvp("Birimi",mCurrentBirim->currentText().toUTF8()));
        } catch (bsoncxx::exception &e) {

        }
    }


    auto oidlist = array{};

    for( auto oid : poillist )
    {
        try {
            oidlist.append(bsoncxx::oid{oid.toStdString()});
        } catch (bsoncxx::exception &e) {

        }
    }


        try {
            filter.append(kvp("_id",make_document(kvp("$nin",oidlist))));
        } catch (bsoncxx::exception &e) {

        }


    try {
        filter.append(kvp("Büro Personeli",true));
    } catch (bsoncxx::exception &e) {

    }

    {
        auto container = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(10,Side::Top|Side::Bottom);
        container->setHeight(20);
        container->setAttributeValue(Style::style,Style::background::color::rgb(175,125,125));
    }

    try {
        auto cursor = this->db()->collection("Personel").find(filter.view());
        for( auto doc : cursor )
        {
            QString pername;
            QString perbirim;


            try {
                perbirim = QString(doc["Birimi"].get_utf8().value.to_string().c_str());
            } catch (bsoncxx::exception &e) {
            }

            try {
                pername = QString(doc["ad soyad"].get_utf8().value.to_string().c_str());
            } catch (bsoncxx::exception &e) {
            }

            this->addNinRow( pername , perbirim );

        }
    } catch (mongocxx::exception &e) {

    }



}

void GirisCikisWidget::addRow(QDate date)
{

    auto container = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::col_full_12);
//    container->setHeight(70);
    container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,255),this->getRandom(200,255),this->getRandom(200,255)));
    container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    auto rContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());


    auto dayContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//    dayContainer->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black")+Style::Border::right::border("1px solid black"));
    dayContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    dayContainer->setAttributeValue(Style::style,Style::background::color::rgba(125,125,125)+Style::color::color(Style::color::White::Snow));
    dayContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    auto layout = dayContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
    auto text = layout->addWidget(cpp14::make_unique<WText>(date.toString("dddd dd/MMMM/yyyy").toStdString()),0,AlignmentFlag::Center|AlignmentFlag::Middle);


    auto filter = document{};

    try {
        filter.append(kvp("personeloid",this->User().view()["_id"].get_oid().value));
    } catch (bsoncxx::exception &e) {

    }

    try {
        filter.append(kvp("day",bsoncxx::types::b_int64{date.toJulianDay()}));
    } catch (bsoncxx::exception &e) {

    }

    mongocxx::options::find findOptions;

    auto sortDoc = document{};

    try {
        sortDoc.append(kvp("time",1));
    } catch (bsoncxx::exception &e) {

    }

    findOptions.sort(sortDoc.view());


    try {
        auto cursor = this->db()->collection("INOUT").find(filter.view(),findOptions);

        for( auto doc : cursor )
        {

            auto dContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//            dContainer->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid black")+Style::Border::right::border("1px solid black"));
            dContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
            dContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(200,255),this->getRandom(200,255),this->getRandom(200,255)));
            dContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

            auto layout = dContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
            try {
                auto text = layout->addWidget(cpp14::make_unique<WText>(QTime::fromMSecsSinceStartOfDay(doc["time"].get_int32().value).toString("hh:mm").toStdString()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            } catch (bsoncxx::exception &e) {
                auto text = layout->addWidget(cpp14::make_unique<WText>(e.what()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            }

        }


    } catch (mongocxx::exception &e) {

    }


}

void GirisCikisWidget::addRow(QString personeloid, qint64 julianDay)
{

    auto filter = document{};

    try {
        filter.append(kvp("personeloid",bsoncxx::oid{personeloid.toStdString()}));
    } catch (bsoncxx::exception &e) {

    }

    QString PersonelName;
    QString BirimName;

    try {
        auto val = this->db()->collection("INOUT").find_one(filter.view());

        try {
            PersonelName = val.value().view()["ad soyad"].get_utf8().value.to_string().c_str();
        } catch (bsoncxx::exception &e) {

        }

        try {
            BirimName = val.value().view()["Birimi"].get_utf8().value.to_string().c_str();
        } catch (bsoncxx::exception &e) {

        }

    } catch (mongocxx::exception &e) {

    }


    auto container = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::col_full_12);
//    container->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(),this->getRandom(),this->getRandom()));
//    container->setHeight(110);
    container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    container->setMargin(2,Side::Top|Side::Bottom);

    auto rContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//    rContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));

    auto dayContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
//    dayContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));
    dayContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    dayContainer->setHeight(80);

    auto layout = dayContainer->setLayout(cpp14::make_unique<WVBoxLayout>());


    layout->addWidget(cpp14::make_unique<WText>(PersonelName.toStdString()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
    layout->addWidget(cpp14::make_unique<WText>(BirimName.toStdString()),0,AlignmentFlag::Center|AlignmentFlag::Middle);


    filter.clear();

    try {
        filter.append(kvp("personeloid",bsoncxx::oid{personeloid.toStdString()}));
    } catch (bsoncxx::exception &e) {

    }

    try {
        filter.append(kvp("day",bsoncxx::types::b_int64{julianDay}));
    } catch (bsoncxx::exception &e) {

    }


    auto sabahinContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    sabahinContainer->setHeight(80);
    sabahinContainer->setAttributeValue(Style::style,Style::Border::right::border("1px solid black")+Style::Border::left::border("1px solid black"));
    sabahinContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
    auto sabahinimgContainer = sabahinContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    sabahinimgContainer->setHeight(65);
    auto sabahintext = sabahinContainer->addWidget(cpp14::make_unique<WText>("-:-"));

    auto sabahoutContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    sabahoutContainer->setHeight(80);
    sabahoutContainer->setAttributeValue(Style::style,Style::Border::right::border("1px solid black"));
    sabahoutContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
    auto sabahoutimgContainer = sabahoutContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    sabahoutimgContainer->setHeight(65);
    auto sabahouttext = sabahoutContainer->addWidget(cpp14::make_unique<WText>("-:-"));


    auto aksaminContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    aksaminContainer->setHeight(80);
    aksaminContainer->setAttributeValue(Style::style,Style::Border::right::border("1px solid black"));
    aksaminContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
    auto aksaminimgContainer = aksaminContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    aksaminimgContainer->setHeight(65);
    auto aksamintext = aksaminContainer->addWidget(cpp14::make_unique<WText>("-:-"));

    auto aksamoutContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    aksamoutContainer->setHeight(80);
//    aksamoutContainer->setAttributeValue(Style::style,Style::Border::right::border("1px solid black"));
    aksamoutContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
    auto aksamoutimgContainer = aksamoutContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    aksamoutimgContainer->setHeight(65);
    auto aksamouttext = aksamoutContainer->addWidget(cpp14::make_unique<WText>("-:-"));

    auto bucket = this->db()->gridfs_bucket();
    try {
        auto cursor = this->db()->collection("INOUT").find(filter.view());

        for( auto doc : cursor )
        {

            int msecValue = 0;

            std::string imgoid;

            try {
                msecValue = doc["time"].get_int32().value;
            } catch (bsoncxx::exception &e) {

            }

            try {
                imgoid = doc["imgoid"].get_oid().value.to_string();
            } catch (bsoncxx::exception &e) {

            }


            auto currentTime = QTime::fromMSecsSinceStartOfDay(msecValue);
            //            std::cout << msecValue << " " << currentTime.msecsSinceStartOfDay() << " " <<currentTime.toString("hh:mm").toStdString() << " " << QTime(17,0).toString("hh:mm").toStdString() << " " << QTime(19,0).toString().toStdString() << std::endl;

            if( currentTime < QTime(9,0) )
            {
                sabahintext->setText(currentTime.toString("hh:mm").toStdString());
                auto fileName = SBLDKeys::downloadifNotExist(&bucket,imgoid);
                sabahinimgContainer->setAttributeValue(Style::style,Style::background::url(fileName)
                                                       +Style::background::size::contain
                                                       +Style::background::repeat::norepeat
                                                       +Style::background::position::center_center);
            }

            if( currentTime >= QTime(12,0) && currentTime < QTime(13,0) )
            {
                sabahouttext->setText(currentTime.toString("hh:mm").toStdString());
                auto fileName = SBLDKeys::downloadifNotExist(&bucket,imgoid);
                sabahoutimgContainer->setAttributeValue(Style::style,Style::background::url(fileName)
                                                        +Style::background::size::contain
                                                        +Style::background::repeat::norepeat
                                                        +Style::background::position::center_center);
            }

            if( currentTime >= QTime(13,0) && currentTime < QTime(14,0) )
            {
                aksamintext->setText(currentTime.toString("hh:mm").toStdString());
                auto fileName = SBLDKeys::downloadifNotExist(&bucket,imgoid);
                aksaminimgContainer->setAttributeValue(Style::style,Style::background::url(fileName)
                                                       +Style::background::size::contain
                                                       +Style::background::repeat::norepeat
                                                       +Style::background::position::center_center);
            }

            if( currentTime >= QTime(17,0) && currentTime < QTime(18,0) )
            {
                aksamouttext->setText(currentTime.toString("hh:mm").toStdString());
                auto fileName = SBLDKeys::downloadifNotExist(&bucket,imgoid);
                aksamoutimgContainer->setAttributeValue(Style::style,Style::background::url(fileName)
                                                        +Style::background::size::contain
                                                        +Style::background::repeat::norepeat
                                                        +Style::background::position::center_center);
            }

        }


    } catch (mongocxx::exception &e) {

    }


}

void GirisCikisWidget::addNinRow( QString PersonelName , QString BirimName )
{


    auto container = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::col_full_12);
    //    container->setHeight(110);
    container->setMargin(10,Side::Top|Side::Bottom);

    auto rContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));

    auto dayContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    dayContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));
    dayContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
    dayContainer->setHeight(80);

    auto layout = dayContainer->setLayout(cpp14::make_unique<WVBoxLayout>());


    layout->addWidget(cpp14::make_unique<WText>(PersonelName.toStdString()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
    layout->addWidget(cpp14::make_unique<WText>(BirimName.toStdString()),0,AlignmentFlag::Center|AlignmentFlag::Middle);




    auto sabahinContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    sabahinContainer->setHeight(80);
    sabahinContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));
    sabahinContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
    auto sabahinimgContainer = sabahinContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    sabahinimgContainer->setHeight(65);
    auto sabahintext = sabahinContainer->addWidget(cpp14::make_unique<WText>("Yok"));

    auto sabahoutContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    sabahoutContainer->setHeight(80);
    sabahoutContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));
    sabahoutContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
    auto sabahoutimgContainer = sabahoutContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    sabahoutimgContainer->setHeight(65);
    auto sabahouttext = sabahoutContainer->addWidget(cpp14::make_unique<WText>("Yok"));


    auto aksaminContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    aksaminContainer->setHeight(80);
    aksaminContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));
    aksaminContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
    auto aksaminimgContainer = aksaminContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    aksaminimgContainer->setHeight(65);
    auto aksamintext = aksaminContainer->addWidget(cpp14::make_unique<WText>("Yok"));

    auto aksamoutContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    aksamoutContainer->setHeight(80);
    aksamoutContainer->setAttributeValue(Style::style,Style::Border::border("1px solid black"));
    aksamoutContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
    auto aksamoutimgContainer = aksamoutContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    aksamoutimgContainer->setHeight(65);
    auto aksamouttext = aksamoutContainer->addWidget(cpp14::make_unique<WText>("Yok"));



}
