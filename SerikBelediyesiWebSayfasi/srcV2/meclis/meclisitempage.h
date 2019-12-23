#ifndef MECLISITEMPAGE_H
#define MECLISITEMPAGE_H


#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "meclis/meclismanager.h"


namespace v2{

class MeclisItemPage : public ContainerWidget , public SerikBLDCore::Meclis::MeclisItem , public SerikBLDCore::Meclis::KararManager , public SerikBLDCore::Meclis::YouTubeManager
{
public:
    explicit MeclisItemPage(DB *_db , const MeclisItem &item);


    void onList(const QVector<SerikBLDCore::Meclis::KararItem> *mlist) override;

    void onList(const QVector<SerikBLDCore::Meclis::YouTubeLink> *mlist) override;

    void initMeclisBilgileri();
    void initKararContoller();

    void addKarar();

    void addYoutubeLink();

private:
    WSpinBox* mYil;
    WLineEdit* mAyLineEdit;
    WCheckBox* mYayinda;
    WTextEdit* mGundemEdit;

    WContainerWidget* mKararContainer;
    WContainerWidget* mLinkContainer;
};



}


#endif // MECLISITEMPAGE_H
