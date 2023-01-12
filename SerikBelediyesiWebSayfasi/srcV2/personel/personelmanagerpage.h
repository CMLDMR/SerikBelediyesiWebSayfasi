#ifndef PERSONELMANAGERPAGE_H
#define PERSONELMANAGERPAGE_H



#include "personelmanager.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/sms/smsmanager.h"
#include <QRandomGenerator>
#include <QMap>

#include "yenipersonelwidget.h"
#include "user.h"





class IKManagerPage : public ContainerWidget
{
public:
    explicit IKManagerPage(SerikBLDCore::DB* _db );
private:
    SerikBLDCore::DB* mDB;
    void initPersonelManager();
    void initBirimManager();
};






class PersonelThumpPage : public SerikBLDCore::IK::Personel , public ContainerWidget
{
public:
    explicit PersonelThumpPage(Personel &personel , SerikBLDCore::DB* _db );
    explicit PersonelThumpPage(const Personel &personel , SerikBLDCore::DB* _db );

    virtual void errorOccured(const std::string &errorText) override;
private:
    SerikBLDCore::DB* mDB;
};






class PersonelPage : public SerikBLDCore::IK::Personel , public ContainerWidget , public SerikBLDCore::PersonelManager
{
public:
    explicit PersonelPage(const Personel &personel , SerikBLDCore::DB* _db );
    explicit PersonelPage(const Personel &personel , SerikBLDCore::DB* _db , SerikBLDCore::User* mUser );
    virtual void errorOccured(const std::string &errorText) override;
    Signal<NoClass> &ClickedBack();
private:
    void initHeader();
    void initContent();
    void initOwnContent();
    void initSMSLog();
    SMSManager* mSMSManager;
    Signal<NoClass> _clickedBack;
    SerikBLDCore::BirimManager* mBirimManager;
    SerikBLDCore::PersonelManager* mPersonelManager;

    QMap<QString,QString> mMudurlukList;

};






class PersonelManagerPage : public SerikBLDCore::PersonelManager , public ContainerWidget
{
public:
    explicit PersonelManagerPage(SerikBLDCore::DB* _db );
    virtual void onList(const QVector<SerikBLDCore::IK::Personel> *mlist) override;
    void errorOccured(const std::string &errorText) override;
private:
    void initBirimList();
    SerikBLDCore::BirimManager* mBirimManager;
    void initNewPersonelWidget();
    bool mListFromSearchText;
    WComboBox* birimComboBoxFilter;
};



class BirimWidget : public ContainerWidget , public SerikBLDCore::IK::BirimItem
{
public:
    explicit BirimWidget(const SerikBLDCore::IK::BirimItem &other );


    Signal<std::string,std::string,std::int32_t> &ChangedClicked();
private:
    Signal<std::string,std::string,std::int32_t> _ChangedClicked;
};


class BirimManagerPage : public SerikBLDCore::BirimManager , public ContainerWidget
{
public:
    explicit BirimManagerPage( SerikBLDCore::DB* _db );
    void onList(const QVector<SerikBLDCore::IK::BirimItem> *mlist) override;

    [[deprecated("Burası Artık Kullanılmayacak")]]
    void initAltBirimler( const std::string &birimOid , const std::string &birimAdi );

    void initBirim( const SerikBLDCore::IK::BirimItem &birimItem );
private:
    Signal<std::string> Error;
    Signal<std::string> Succes;
    SerikBLDCore::PersonelManager* mPersonelManager;
};


class AltBirimManagerPage : public SerikBLDCore::AltBirimManager , public ContainerWidget
{
public:
    explicit AltBirimManagerPage( SerikBLDCore::DB* _db , const bsoncxx::oid& birimOid , const std::string& birimAdi );
    AltBirimManagerPage( SerikBLDCore::DB* _db , const SerikBLDCore::IK::BirimItem &_birimItem );

    void onList(const QVector<SerikBLDCore::IK::AltBirimItem> *mlist) override;

    void updateAltBirimList();
private:
    bsoncxx::oid mBirimOid;
    std::string mBirimAdi;
    SerikBLDCore::IK::BirimItem mCurrentBirimItem;
};



#endif // PERSONELMANAGERPAGE_H
