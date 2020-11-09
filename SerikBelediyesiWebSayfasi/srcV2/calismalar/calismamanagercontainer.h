#ifndef CALISMAMANAGERCONTAINER_H
#define CALISMAMANAGERCONTAINER_H

#include "user.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "calisma/calismamanager.h"

namespace v2{


class CalismaManagerContainer : public ContainerWidget , public SerikBLDCore::CalismaManager
{
public:
    explicit CalismaManagerContainer(SerikBLDCore::User *_user );


private:
    void initHeader();

    void initYeniCalismaEkle();
    WLineEdit* mCalismaAdiLineEdit;
    WComboBox* mMahalleComboBox;
    WComboBox* mBirimComboBox;
    WDoubleSpinBox* mMiktar;

    WDateEdit* mDateEdit;
    WTextEdit* mAciklamaTextEdit;
    WContainerWidget* mResimAlaniContainerWidget;
    std::vector<std::string> mUploadedPictureList;



    SerikBLDCore::User* mUser;
};

}



#endif // CALISMAMANAGERCONTAINER_H
