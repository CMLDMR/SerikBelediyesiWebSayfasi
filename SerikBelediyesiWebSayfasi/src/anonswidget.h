#ifndef ANONSWIDGET_H
#define ANONSWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/basewidget.h"

#include <Wt/WDialog.h>



class AnonsWidget : public BaseClass::ContainerWidget
{
public:
    AnonsWidget(mongocxx::database* _db , bsoncxx::document::value _user );



    void initDevices();

    void DeviceProperties( std::string deviceOid );


    void addAciklama( std::string deviveOid );

    void showFoto( const std::string &deviceOid );



private:

    WContainerWidget* mMap;

    WContainerWidget* mDeviceListContainer;

    WContainerWidget* mDevicePropertiesContainer;

    std::string text;

    int counter ;
};



class Device : public WContainerWidget
{
public:
    Device(const std::string &oid);


    Signal<std::string> &ClickOid();


    double longtitute() const;
    void setLongtitute(double longtitute);

    double latitute() const;
    void setLatitute(double latitute);

    std::string deviceName() const;
    void setDeviceName(const std::string &deviceName);

    std::string oid() const;
    void setOid(const std::string &oid);

    std::string mahalle() const;
    void setMahalle(const std::string &mahalle);

    JSignal<std::string> &PopupClick();

    std::string durum() const;
    void setDurum(const std::string &durum);

private:
    double mLongtitute;
    double mLatitute;

    std::string mDeviceName;

    std::string mOid;

    std::string mMahalle;

    Signal<std::string> _mClickOid;


    WText* mDeviceText;

    WText* mMahalleText;

    JSignal<std::string> _mPopupClick;

    std::string mDurum;

};

#endif // ANONSWIDGET_H
