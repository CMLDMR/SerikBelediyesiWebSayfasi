//#ifndef DEVICEITEM_H
//#define DEVICEITEM_H

//#include "item.h"
//#include "listitem.h"


//namespace DeviceServices{

//namespace Key{

//static const std::string Collection{"DeviceServices"};
//static const std::string type{"Type"};
//static const std::string url{"url"};
//static const std::string ipAdress{"IpAddress"};
//static const std::string Name{"Name"};
//static const std::string Status{"Status"};
//static const std::string lastUpdateTime{"lastUpdateTime"};
//static const std::string lastEventTime{"lastEventTime"};

//}


//class DeviceServicesItem : public SerikBLDCore::Item
//{
//public:
//    DeviceServicesItem();

//    DeviceServicesItem& setType( const std::string &typeName);
//    DeviceServicesItem& setUrl( const std::string &mUrl);
//    DeviceServicesItem& setIpAddress( const std::string &ipAddress);
//    DeviceServicesItem& setName( const std::string &mName);
//    DeviceServicesItem& setStatus( const std::string &mStatus);
//    DeviceServicesItem& setCheckTime( const std::int64_t &timeStamp );
//    DeviceServicesItem& setLastEventTime( const std::int64_t &timeStamp );


//    std::string getType() const;
//    std::string getUrl() const;
//    std::string getIpAddress() const;
//    std::string getName() const;
//    std::string getStatus() const;
//    std::int64_t getLastChecked() const;
//    std::int64_t getLastEventTime() const;
//};



//class DeviceServicesManager : public SerikBLDCore::ListItem<DeviceServicesItem>
//{
//public:
//    DeviceServicesManager(SerikBLDCore::DB* _mDB);

//    virtual void onList(const QVector<DeviceServicesItem> *mlist) override;

//    virtual void errorOccured(const std::string &errorText) override;
//};


//}




//#endif // DEVICEITEM_H
