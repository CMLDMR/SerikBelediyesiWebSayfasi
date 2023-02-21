#ifndef FIREWALLCONTAINER_H
#define FIREWALLCONTAINER_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include <QDir>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QJsonObject>
#include <QFile>
#include <QVector>
#include <QPair>

#include <map>
#include <list>
#include <QDebug>


namespace FireWall{


class LogFile
{
public:
    LogFile( const std::string &logFile);



private:
    std::string mFilePath;
};


class FireWallContainer : public ContainerWidget
{
public:
    FireWallContainer();


    void AnalysysDay( const QString &dayLogFile );


    struct SortStruct{
        QString name;
        int count;

        friend bool operator<( const SortStruct &a, const SortStruct &b ){
            return a.count > b.count;
        }

        friend bool operator==( const SortStruct &a, const SortStruct &b ){
            return a.name == b.name;
        }
    };



    QVector<QJsonObject> mArray;
    QMap<QString,int> mMacList;
    QMap<QString,int> mUrlList;
    QMap<QString,int> mIpList;
    QMap<QString,QString> mMactoIP;

    enum class PageState{
        ListMostMac = 0,
        ListMostIP,
        ListMostUrl,
        ListMostMacByUrl,
        ListMostUrlByMac,
        ListMostUrlByIP,
        ListMostUrlBySaat,
        ListMostMacBySaat

    };

    PageState mCurrentPage;


    void ListItem( const QMap<QString,int> &mList );



    void ListItemClicked(const QString &mUrl);

    void ListBySaat();
    void ListMacBySaat();


    QList<QDate> mDateList;

    struct MonthItem{
        std::string mName;
        std::int64_t mCompareValue;
        std::int64_t julianDate;
        std::int64_t month;
        std::int64_t year;

        bool operator<( const MonthItem &item) const{
            return item.mCompareValue > this->mCompareValue;
        }

        friend QDebug operator<<( QDebug debug , const MonthItem &item ){
            debug << item.mName.c_str() << item.mCompareValue <<item.julianDate ;
            return debug;
        }

        const bool operator==( const MonthItem &item ){
            return item.mName == mName;
        }
    };

    WContainerWidget* mMonthContainer;
    WContainerWidget* mDayContainer;




};


}



#endif // FIREWALLCONTAINER_H
