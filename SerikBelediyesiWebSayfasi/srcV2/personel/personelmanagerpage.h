#ifndef PERSONELMANAGERPAGE_H
#define PERSONELMANAGERPAGE_H



#include "personelmanager.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/sms/smsmanager.h"
#include <QRandomGenerator>

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






class BirimManagerPage : public SerikBLDCore::BirimManager , public ContainerWidget
{
public:
    explicit BirimManagerPage( SerikBLDCore::DB* _db );
    void onList(const QVector<SerikBLDCore::IK::BirimItem> *mlist) override;
private:
    Signal<std::string> Error;
    Signal<std::string> Succes;
    SerikBLDCore::PersonelManager* mPersonelManager;
};

#endif // PERSONELMANAGERPAGE_H
