#ifndef CALISMAMANAGERCONTAINER_H
#define CALISMAMANAGERCONTAINER_H

#include "user.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "calisma/calismamanager.h"
#include "calismacontainerwidget.h"

namespace v2{


class CalismaManagerContainer : public ContainerWidget , public SerikBLDCore::CalismaManager
{
public:
    explicit CalismaManagerContainer(SerikBLDCore::User *_user );

    void onList( const QVector<SerikBLDCore::Calisma> *mlist ) override;


private:
    void initHeader();
    void initFooter();
    void initIstatistik();

    void initYeniCalismaEkle();
    WLineEdit* mCalismaAdiLineEdit;
    WComboBox* mMahalleComboBox;
    WComboBox* mBirimComboBox;
    WComboBox* mTipComboBox;

    WDoubleSpinBox* mMiktar;

    WDateEdit* mDateEdit;
    WTextEdit* mAciklamaTextEdit;
    WContainerWidget* mResimAlaniContainerWidget;
    std::vector<std::string> mUploadedPictureList;



    SerikBLDCore::User* mUser;


    void loadCalisma( const std::string &calismaOid );
};

}



#endif // CALISMAMANAGERCONTAINER_H
