#ifndef SMSMANAGER_H
#define SMSMANAGER_H

#include "sms/smsabstrackmanager.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"


class SMSManager : public SerikBLDCore::SMSAbstractManager , public ContainerWidget
{
    bool mSMSGonderiliyor;
public:
    explicit SMSManager( SerikBLDCore::DB* _db );
    ~SMSManager() override;


    bool insertAndSendSMS(const SerikBLDCore::SMS::SMSItem &item) override;

    bool checkRapor(const SerikBLDCore::SMS::SMSItem &item) override;

    Signal<std::string> &smsSended();
    Signal<std::string> &ErrorOccured();
    Signal<std::string> &messageOccured();
private:
    Http::Client* mHttpClient;

    Signal<std::string> _smsSended;
    Signal<std::string> _errorOccured;
    Signal<std::string> _messageOccured;

    QString mLastCheckItemOid;


};

#endif // SMSMANAGER_H
