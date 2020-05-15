#ifndef FIRMAMANAGERPAGE_H
#define FIRMAMANAGERPAGE_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "firma/firmamanager.h"
#include "user.h"
#include "tcmanager.h"

namespace v2{

class FirmaManagerPage : public SerikBLDCore::Firma::FirmaManager, public ContainerWidget
{
public:
    explicit FirmaManagerPage(SerikBLDCore::User *_user );

    void onList(const QVector<SerikBLDCore::Firma::FirmaItem> *mlist) override;

    void errorOccured(const std::string &errorText) override;

    void yeniFirmaKayitDialog();

    void yetkiliAtaDialog(const std::string& firmaOid );

    void yetkiliGoster( const std::string& vatandasOid );

private:
    SerikBLDCore::User* mUser;

    SerikBLDCore::TCManagerV2* mTCManager;
};

}



#endif // FIRMAMANAGERPAGE_H
