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



};

#endif // FIREWALLCONTAINER_H
