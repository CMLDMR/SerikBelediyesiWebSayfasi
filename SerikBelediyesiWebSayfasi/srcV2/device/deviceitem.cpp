#include "deviceitem.h"

DeviceServices::DeviceServicesItem::DeviceServicesItem()
    :SerikBLDCore::Item(DeviceServices::Key::Collection)
{

}

DeviceServices::DeviceServicesItem &DeviceServices::DeviceServicesItem::setType(const std::string &typeName)
{
    this->append(Key::type,typeName);
    return *this;
}

DeviceServices::DeviceServicesItem &DeviceServices::DeviceServicesItem::setUrl(const std::string &mUrl)
{
    this->append(Key::url,mUrl);
    return *this;
}

DeviceServices::DeviceServicesItem &DeviceServices::DeviceServicesItem::setIpAddress(const std::string &ipAddress)
{
    this->append(Key::ipAdress,ipAddress);
    return *this;
}

DeviceServices::DeviceServicesItem &DeviceServices::DeviceServicesItem::setName(const std::string &mName)
{
    this->append(Key::Name,mName);
    return *this;
}

DeviceServices::DeviceServicesItem &DeviceServices::DeviceServicesItem::setStatus(const std::string &mStatus)
{
    this->append(Key::Status,mStatus);
    return *this;
}

DeviceServices::DeviceServicesItem &DeviceServices::DeviceServicesItem::setCheckTime(const int64_t &timeStamp)
{
    this->append(Key::lastUpdateTime,timeStamp);
    return *this;
}

DeviceServices::DeviceServicesItem &DeviceServices::DeviceServicesItem::setLastEventTime(const int64_t &timeStamp)
{
    this->append(Key::lastEventTime,timeStamp);
    return *this;
}

std::string DeviceServices::DeviceServicesItem::getType() const
{
    auto val = this->element(Key::type);
    if( val ){
        return val->get_utf8().value.to_string();
    }
    return "UnKnown";
}

std::string DeviceServices::DeviceServicesItem::getUrl() const
{
    auto val = this->element(Key::url);
    if( val ){
        return val->get_utf8().value.to_string();
    }
    return "UnKnown";
}

std::string DeviceServices::DeviceServicesItem::getIpAddress() const
{
    auto val = this->element(Key::ipAdress);
    if( val ){
        return val->get_utf8().value.to_string();
    }
    return "UnKnown";
}

std::string DeviceServices::DeviceServicesItem::getName() const
{
    auto val = this->element(Key::Name);
    if( val ){
        return val->get_utf8().value.to_string();
    }
    return "UnKnown";
}

std::string DeviceServices::DeviceServicesItem::getStatus() const
{
    auto val = this->element(Key::Status);
    if( val ){
        return val->get_utf8().value.to_string();
    }
    return "UnKnown";
}

int64_t DeviceServices::DeviceServicesItem::getLastChecked() const
{
    auto val = this->element(Key::lastUpdateTime);
    if( val ){
        return val->get_int64().value;
    }
    return 0;
}

int64_t DeviceServices::DeviceServicesItem::getLastEventTime() const
{
    auto val = this->element(Key::lastEventTime);
    if( val ){
        return val->get_int64().value;
    }
    return 0;
}

DeviceServices::DeviceServicesManager::DeviceServicesManager(SerikBLDCore::DB *_mDB)
    :SerikBLDCore::ListItem<DeviceServicesItem>(_mDB)
{

}

void DeviceServices::DeviceServicesManager::onList(const QVector<DeviceServicesItem> *mlist)
{

}

void DeviceServices::DeviceServicesManager::errorOccured(const std::string &errorText)
{

}
