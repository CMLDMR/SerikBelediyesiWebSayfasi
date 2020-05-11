#ifndef FIRMAMANAGERPAGE_H
#define FIRMAMANAGERPAGE_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "firma/firmamanager.h"
#include "user.h"

namespace v2{

class FirmaManagerPage : public SerikBLDCore::Firma::FirmaManager, public ContainerWidget
{
public:
    explicit FirmaManagerPage(SerikBLDCore::User *_user );

    void onList(const QVector<SerikBLDCore::Firma::FirmaItem> *mlist) override;

    void errorOccured(const std::string &errorText) override;

    void yeniFirmaKayitDialog();

private:
    SerikBLDCore::User* mUser;
};

}



#endif // FIRMAMANAGERPAGE_H
