#include "firewallcontainer.h"

#include <set>
#include <filesystem>

FireWall::FireWallContainer::FireWallContainer()
    :ContainerWidget("FireWallLog")
{


    this->Header()->setMargin(5,Side::Top|Side::Bottom);

    QDir dir;
    if( !dir.cd("firewalllog/log") ){
        this->showMessage("Uyarı","Log Klasörü Yok");
        return;
    }


    mDateList.clear();

    mMonthContainer = this->Header()->addNew<WContainerWidget>();
    mMonthContainer->addStyleClass(Bootstrap::Grid::col_full_12);

    mDayContainer = this->Header()->addNew<WContainerWidget>();
    mDayContainer->addStyleClass(Bootstrap::Grid::col_full_12);



    auto pathList = std::filesystem::path("firewalllog/log");



    for( const auto &item : dir.entryInfoList(QDir::Filter::Files) ){
        mDateList.push_back(QDate::fromJulianDay(item.fileName().toInt()));
    }

    std::set<MonthItem> mMonthList;

    for( const auto &date : mDateList ){
        mMonthList.insert(MonthItem{date.toString("MMMM yyyy").toStdString(),
                                    date.toString("yyyyMM").toInt(),
                                    date.toJulianDay(),
                                    date.month(),
                                    date.year()});
    }

    for( const auto &date : mMonthList ){
        qDebug() << date;
    }

    for( const auto &month : mMonthList ){
        auto btnContainer = mMonthContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        btnContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                                    Bootstrap::Grid::Medium::col_md_3+
                                    Bootstrap::Grid::Small::col_sm_4+
                                    Bootstrap::Grid::ExtraSmall::col_xs_6);
        btnContainer->addStyleClass(CSSStyle::Button::blueButton);
        btnContainer->setAttributeValue(Style::dataoid,std::to_string(month.julianDate));
        btnContainer->setPadding(5,Side::Top|Side::Bottom);

        btnContainer->addNew<WText>(month.mName.c_str());

        btnContainer->clicked().connect([=](){
            mDayContainer->clear();
            mDayContainer->setMargin(5,Side::Top|Side::Bottom);
            for( const auto &item : dir.entryInfoList(QDir::Filter::Files) ){

                qDebug() << item.fileName() << month;

                QDate date;
                date.setDate(month.year,month.month,1);
                int days = date.daysInMonth();

                QDate dateBaslangic;
                dateBaslangic.setDate(month.year,month.month,days);

                if( item.fileName().toInt() >= date.toJulianDay() && item.fileName().toInt() <= dateBaslangic.toJulianDay() ){
                    auto btnContainer_ = mDayContainer->addWidget(cpp14::make_unique<WContainerWidget>());
                    btnContainer_->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                                Bootstrap::Grid::Medium::col_md_2+
                                                Bootstrap::Grid::Small::col_sm_3+
                                                Bootstrap::Grid::ExtraSmall::col_xs_4);
                    btnContainer_->addStyleClass(CSSStyle::Button::grayButton);
                    btnContainer_->setAttributeValue(Style::dataoid,item.fileName().toStdString());
                    btnContainer_->setPadding(5,Side::Top|Side::Bottom);

                    btnContainer_->addNew<WText>(WDate::fromJulianDay(item.fileName().toInt()).toString("dd/MMM/yy"));

                    btnContainer_->clicked().connect([=](){
                        this->AnalysysDay(item.absoluteFilePath());
                    });
                }
            }
        });




    }







}

void FireWall::FireWallContainer::AnalysysDay(const QString &dayLogFile)
{



    QFile file(dayLogFile);

    QByteArray ar;

    if( file.open(QIODevice::ReadOnly) ){
        ar = file.readAll();
        file.close();
    }else{
        this->showMessage("UYARI","Log Dosyası Açılamadı: " + dayLogFile.toStdString());
        return;
    }

    if( ar.isEmpty() ){
        this->showMessage("UYARI","Log Dosyası Boş: " + dayLogFile.toStdString());
        return;
    }

    this->Footer()->clear();
    this->Content()->clear();




    QString str;
    mArray.clear();
    mMacList.clear();
    mUrlList.clear();
    mIpList.clear();
    mMactoIP.clear();

    for( int i = 0 ; i < ar.size() ; i++ ){

        if( QChar(ar[i]).unicode() == 10 ){
            auto obj = QJsonDocument::fromJson(str.toUtf8()).object();
            mArray.push_back(obj);

            if( !mMacList.contains(obj["MAC"].toString()) ){
                mMacList.insert(obj["MAC"].toString(),1);
            }else{
                mMacList[obj["MAC"].toString()]++;
            }

            if( !mUrlList.contains(obj["url"].toString()) ){
                mUrlList.insert(obj["url"].toString(),1);
            }else{
                mUrlList[obj["url"].toString()]++;
            }

            if( !mIpList.contains(obj["ip"].toString()) ){
                mIpList.insert(obj["ip"].toString(),1);
            }else{
                mIpList[obj["ip"].toString()]++;
            }

            if( !mMactoIP.contains(obj["MAC"].toString()) ){
                mMactoIP.insert(obj["MAC"].toString(),obj["ip"].toString());
            }

            str.clear();
        }else{
            str.append(QChar(ar[i]));
        }
    }


    auto listByMacBtn = this->Content()->addNew<WPushButton>("MAC List");
    auto listByIpBtn = this->Content()->addNew<WPushButton>("IP List");
    auto listByUrlBtn = this->Content()->addNew<WPushButton>("URL List");
    auto listURLBySaatBtn = this->Content()->addNew<WPushButton>("URL Saatlik");
    auto listMACBySaatBtn = this->Content()->addNew<WPushButton>("MAC Saatlik");



    listByMacBtn->clicked().connect([=](){
        mCurrentPage = PageState::ListMostMac;
        this->ListItem(mMacList);
    });

    listByIpBtn->clicked().connect([=](){
        mCurrentPage = PageState::ListMostIP;
        this->ListItem(mIpList);
    });


    listByUrlBtn->clicked().connect([=](){
        mCurrentPage = PageState::ListMostUrl;
        this->ListItem(mUrlList);
    });

    listURLBySaatBtn->clicked().connect([=](){
        mCurrentPage = PageState::ListMostUrlBySaat;
        this->ListBySaat();
    });

    listMACBySaatBtn->clicked().connect([=](){
        mCurrentPage = PageState::ListMostMacBySaat;
        this->ListMacBySaat();
    });

}


void FireWall::FireWallContainer::ListItem(const QMap<QString, int> &mList)
{
    this->Footer()->clear();


    auto mTitleContainer = this->Footer()->addNew<WContainerWidget>();
    mTitleContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    mTitleContainer->setMargin(10,Side::Top|Side::Bottom);
    mTitleContainer->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(),
                                                                                  this->getRandom(),
                                                                                  this->getRandom())+
                                       Style::color::color(Style::color::White::Snow)+Style::font::size::s16px);

    switch (mCurrentPage) {
    case PageState::ListMostUrl:
        mTitleContainer->addNew<WText>("En Çok Gidilen URL Adresleri");
        break;
    case PageState::ListMostIP:
        mTitleContainer->addNew<WText>("En Yoğun IP Adresleri");
        break;
    case PageState::ListMostMac:
        mTitleContainer->addNew<WText>("En Yoğun MAC Adresleri");
        break;
    case PageState::ListMostUrlBySaat:
        mTitleContainer->addNew<WText>("Saat Saat URL Kullanımı");
        break;
    default:
        break;
    }







    int maxCount = 0;

    QList<SortStruct> mMapStructList;

    for( const auto &macItem : mList ){
        if( maxCount < macItem ){
            maxCount = macItem;
        }
        SortStruct m{mList.key(macItem),macItem};
        mMapStructList.push_back(m);
    }

    std::sort(mMapStructList.begin(),mMapStructList.end());

    QStringList strList;

    for( const auto &item : mMapStructList ){
        strList.append(item.name + "--" + QString::number(item.count));
    }
    strList.removeDuplicates();


    int __counter = 0;
    for( const auto &item : strList ){
        auto container = this->Footer()->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setPositionScheme(PositionScheme::Relative);
        container->setContentAlignment(AlignmentFlag::Left);

        container->addNew<WText>(std::to_string(__counter++)+". " +item.toStdString());
        container->setMargin(3,Side::Bottom);

        auto barContainer = container->addNew<WContainerWidget>();
        barContainer->setPositionScheme(PositionScheme::Absolute);
        barContainer->setOffsets(0,Side::Top|Side::Left|Side::Bottom);
        barContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
        barContainer->setWidth(WLength(WString("{1}%").arg(static_cast<double>(item.split("--").last().toDouble())/static_cast<double>(maxCount)*100.0).toUTF8()));

        container->decorationStyle().setCursor(Cursor::PointingHand);
        container->clicked().connect([=](){
            if( mCurrentPage == PageState::ListMostUrl ){
                mCurrentPage = PageState::ListMostMacByUrl;
                this->ListItemClicked(item.split("--").first());
            }

            if( mCurrentPage == PageState::ListMostMac ){
                mCurrentPage = PageState::ListMostUrlByMac;
                this->ListItemClicked(item.split("--").first());
            }

            if( mCurrentPage == PageState::ListMostIP ){
                mCurrentPage = PageState::ListMostUrlByIP;
                this->ListItemClicked(item.split("--").first());
            }

        });

    }


}



void FireWall::FireWallContainer::ListItemClicked(const QString &mUrl)
{
    this->Footer()->clear();


    auto mTitleContainer = this->Footer()->addNew<WContainerWidget>();
    mTitleContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    mTitleContainer->setMargin(10,Side::Top|Side::Bottom);
    mTitleContainer->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(),
                                                                                  this->getRandom(),
                                                                                  this->getRandom())+
                                       Style::color::color(Style::color::White::Snow)+Style::font::size::s16px);

    switch (mCurrentPage) {
    case PageState::ListMostMacByUrl:
        mTitleContainer->addNew<WText>("<p><b>"+mUrl.toStdString()+"</b></p><p>En Yoğun Kullanan MAC Adresleri</p>");
        break;
    case PageState::ListMostUrlByMac:
        mTitleContainer->addNew<WText>("<p>"+mUrl.toStdString()+"</p><p>En Yoğun Kullanılan URL Adresleri</p>");
    case PageState::ListMostUrlByIP:
        mTitleContainer->addNew<WText>("<p>"+mUrl.toStdString()+"</p><p>En Yoğun Kullanılan IP Adresleri</p>");
    default:
        break;
    }

    int maxCount = 0;

    QMap<QString,int> mUrlClickedList;

    for( const auto &obj : mArray ){
        if( mCurrentPage == PageState::ListMostMacByUrl ){
            if( mUrl == obj["url"].toString() ){
                if( !mUrlClickedList.contains(obj["MAC"].toString()) ){
                    mUrlClickedList.insert(obj["MAC"].toString(),1);
                }else{
                    mUrlClickedList[obj["MAC"].toString()]++;
                }
            }

        }

        if( mCurrentPage == PageState::ListMostUrlByMac ){
            if( mUrl == obj["MAC"].toString() ){
                if( !mUrlClickedList.contains(obj["url"].toString()) ){
                    mUrlClickedList.insert(obj["url"].toString(),1);
                }else{
                    mUrlClickedList[obj["url"].toString()]++;
                }
            }
        }

    }

    QList<SortStruct> mMapStructList;

    for( const auto &macItem : mUrlClickedList ){
        if( maxCount < macItem ){
            maxCount = macItem;
        }
        SortStruct m{mUrlClickedList.key(macItem),macItem};
        mMapStructList.push_back(m);
    }

    std::sort(mMapStructList.begin(),mMapStructList.end());

    QStringList strList;

    for( const auto &item : mMapStructList ){
        strList.append(item.name + "   --  " + QString::number(item.count));
    }
    strList.removeDuplicates();


    int __counter = 0;
    for( const auto &item : strList ){
        auto container = this->Footer()->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setPositionScheme(PositionScheme::Relative);
        container->setContentAlignment(AlignmentFlag::Left);

        container->addNew<WText>(std::to_string(__counter++)+". " +item.toStdString());
        container->setMargin(3,Side::Bottom);

        auto barContainer = container->addNew<WContainerWidget>();
        barContainer->setPositionScheme(PositionScheme::Absolute);
        barContainer->setOffsets(0,Side::Top|Side::Left|Side::Bottom);
        barContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
        barContainer->setWidth(WLength(WString("{1}%").arg(static_cast<double>(item.split("--").last().toDouble())/static_cast<double>(maxCount)*100.0).toUTF8()));

//        container->decorationStyle().setCursor(Cursor::PointingHand);
//        container->clicked().connect([=](){
//            this->ListUrlClicked(item.split("--").first());
//        });

    }

}

void FireWall::FireWallContainer::ListBySaat()
{
    this->Footer()->clear();


    auto mTitleContainer = this->Footer()->addNew<WContainerWidget>();
    mTitleContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    mTitleContainer->setMargin(10,Side::Top|Side::Bottom);
    mTitleContainer->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(),
                                                                                  this->getRandom(),
                                                                                  this->getRandom())+
                                       Style::color::color(Style::color::White::Snow)+Style::font::size::s16px);

    mTitleContainer->addNew<WText>("Saat Saat URL Kullanımı");




    for( int i = 0 ; i < 24 ; i++ ){

        auto container_ = this->Footer()->addNew<WContainerWidget>();
        container_->addStyleClass(Bootstrap::Grid::col_full_12);
        container_->addNew<WText>(QString("<h4>%1:00-%2:59</h4>").arg(i).arg(i).toStdString());
        container_->setMargin(10,Side::Top);
        container_->addStyleClass(CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);

        int maxCount = 0;

        QMap<QString,int> mUrlClickedList;

        for( const auto &obj : mArray ){
                if( i == QDateTime::fromMSecsSinceEpoch(obj["time"].toVariant().toLongLong()).time().hour() ){
                    if( !mUrlClickedList.contains(obj["url"].toString()) ){
                        mUrlClickedList.insert(obj["url"].toString(),1);
                    }else{
                        mUrlClickedList[obj["url"].toString()]++;
                    }
                }
        }

        QList<SortStruct> mMapStructList;

        for( const auto &macItem : mUrlClickedList ){
            if( maxCount < macItem ){
                maxCount = macItem;
            }
            SortStruct m{mUrlClickedList.key(macItem),macItem};
            mMapStructList.push_back(m);
        }

        std::sort(mMapStructList.begin(),mMapStructList.end());

        QStringList strList;

        for( const auto &item : mMapStructList ){
            strList.append(item.name + "   --  " + QString::number(item.count));
        }
        strList.removeDuplicates();


        int __counter = 0;
        for( const auto &item : strList ){
            auto container = container_->addNew<WContainerWidget>();
            container->addStyleClass(Bootstrap::Grid::col_full_12);
            container->setPositionScheme(PositionScheme::Relative);
            container->setContentAlignment(AlignmentFlag::Left);

            container->addNew<WText>(std::to_string(__counter++)+". " +item.toStdString());
            container->setMargin(3,Side::Bottom);

            auto barContainer = container->addNew<WContainerWidget>();
            barContainer->setPositionScheme(PositionScheme::Absolute);
            barContainer->setOffsets(0,Side::Top|Side::Left|Side::Bottom);
            barContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
            barContainer->setWidth(WLength(WString("{1}%").arg(static_cast<double>(item.split("--").last().toDouble())/static_cast<double>(maxCount)*100.0).toUTF8()));
            if( __counter > 5 ){
                break;
            }
        }

    }



}

void FireWall::FireWallContainer::ListMacBySaat()
{
    this->Footer()->clear();


    auto mTitleContainer = this->Footer()->addNew<WContainerWidget>();
    mTitleContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    mTitleContainer->setMargin(10,Side::Top|Side::Bottom);
    mTitleContainer->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(),
                                                                                  this->getRandom(),
                                                                                  this->getRandom())+
                                       Style::color::color(Style::color::White::Snow)+Style::font::size::s16px);

    mTitleContainer->addNew<WText>("Saat Saat MAC Kullanımı");




    for( int i = 0 ; i < 24 ; i++ ){

        auto container_ = this->Footer()->addNew<WContainerWidget>();
        container_->addStyleClass(Bootstrap::Grid::col_full_12);
        container_->addNew<WText>(QString("<h4>%1:00-%2:59</h4>").arg(i).arg(i).toStdString());
        container_->setMargin(10,Side::Top);
        container_->addStyleClass(CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);

        int maxCount = 0;

        QMap<QString,int> mUrlClickedList;

        for( const auto &obj : mArray ){
                if( i == QDateTime::fromMSecsSinceEpoch(obj["time"].toVariant().toLongLong()).time().hour() ){
                    if( !mUrlClickedList.contains(obj["MAC"].toString()) ){
                        mUrlClickedList.insert(obj["MAC"].toString(),1);
                    }else{
                        mUrlClickedList[obj["MAC"].toString()]++;
                    }
                }
        }

        QList<SortStruct> mMapStructList;

        for( const auto &macItem : mUrlClickedList ){
            if( maxCount < macItem ){
                maxCount = macItem;
            }
            SortStruct m{mUrlClickedList.key(macItem),macItem};
            mMapStructList.push_back(m);
        }

        std::sort(mMapStructList.begin(),mMapStructList.end());

        QStringList strList;

        for( const auto &item : mMapStructList ){
            strList.append(item.name + "   --  " + QString::number(item.count));
        }
        strList.removeDuplicates();


        int __counter = 0;
        for( const auto &item : strList ){
            auto container = container_->addNew<WContainerWidget>();
            container->addStyleClass(Bootstrap::Grid::col_full_12);
            container->setPositionScheme(PositionScheme::Relative);
            container->setContentAlignment(AlignmentFlag::Left);

            container->addNew<WText>(std::to_string(__counter++)+". " +item.toStdString());
            container->setMargin(3,Side::Bottom);

            auto barContainer = container->addNew<WContainerWidget>();
            barContainer->setPositionScheme(PositionScheme::Absolute);
            barContainer->setOffsets(0,Side::Top|Side::Left|Side::Bottom);
            barContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(),this->getRandom(),this->getRandom()));
            barContainer->setWidth(WLength(WString("{1}%").arg(static_cast<double>(item.split("--").last().toDouble())/static_cast<double>(maxCount)*100.0).toUTF8()));
            if( __counter > 5 ){
                break;
            }
        }

    }


}

FireWall::LogFile::LogFile(const std::string &logFile)
    :mFilePath(logFile)
{

}
