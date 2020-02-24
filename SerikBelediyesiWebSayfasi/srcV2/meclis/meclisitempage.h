#ifndef MECLISITEMPAGE_H
#define MECLISITEMPAGE_H


#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "meclis/meclismanager.h"


namespace v2{

class MeclisItemPage : public ContainerWidget ,
        public SerikBLDCore::Meclis::MeclisItem ,
        public SerikBLDCore::Meclis::KararManager ,
        public SerikBLDCore::Meclis::YouTubeManager ,
        public SerikBLDCore::Meclis::RaporManager,
        public SerikBLDCore::Meclis::TeklifManager,
        public SerikBLDCore::Meclis::GundemManager
{
public:
    explicit MeclisItemPage(DB *_db , const MeclisItem &item );


    void onList(const QVector<SerikBLDCore::Meclis::KararItem> *mlist) override;

    void onList(const QVector<SerikBLDCore::Meclis::YouTubeLink> *mlist) override;

    void onList(const QVector<SerikBLDCore::Meclis::RaporItem> *mlist) override;

    void onList(const QVector<SerikBLDCore::Meclis::TeklifItem> *mlist) override;

    void onList(const QVector<SerikBLDCore::Meclis::GundemItem> *mlist) override;

    void initMeclisBilgileri();

    void initKararContoller();

    void addGundem();

    void addKarar();

    void addYoutubeLink();

    void addRapor();

    void addTeklif();

    Signal<const SerikBLDCore::Meclis::MeclisItem&> &updateRequest();

private:
    WSpinBox* mYil;
    WLineEdit* mAyLineEdit;
    WCheckBox* mYayinda;
//    WTextEdit* mGundemEdit;
    WTimeEdit* mMeclisSaat;
    WDateEdit* mMeclisTarih;

    WContainerWidget* mGundemContainer;
    WContainerWidget* mTeklifContainer;
    WContainerWidget* mRaporContainer;
    WContainerWidget* mKararContainer;
    WContainerWidget* mLinkContainer;

    Signal<const SerikBLDCore::Meclis::MeclisItem&> _updateRequest;

};


class MeclisItemPublicPage : public ContainerWidget ,
        public SerikBLDCore::Meclis::MeclisItem ,
        public SerikBLDCore::Meclis::KararManager ,
        public SerikBLDCore::Meclis::YouTubeManager ,
        public SerikBLDCore::Meclis::RaporManager,
        public SerikBLDCore::Meclis::TeklifManager,
        public SerikBLDCore::Meclis::GundemManager
{
public:
    explicit MeclisItemPublicPage(DB *_db , const MeclisItem &item );


    void onList(const QVector<SerikBLDCore::Meclis::KararItem> *mlist) override;

    void onList(const QVector<SerikBLDCore::Meclis::YouTubeLink> *mlist) override;

    void onList(const QVector<SerikBLDCore::Meclis::RaporItem> *mlist) override;

    void onList(const QVector<SerikBLDCore::Meclis::TeklifItem> *mlist) override;

    void onList(const QVector<SerikBLDCore::Meclis::GundemItem> *mlist) override;

    void initMeclisBilgileri();


private:
    WSpinBox* mYil;
    WLineEdit* mAyLineEdit;
    WCheckBox* mYayinda;
    WTextEdit* mGundemEdit;
    WTimeEdit* mMeclisSaat;
    WDateEdit* mMeclisTarih;

    WContainerWidget* mGundemContainer;
    WContainerWidget* mTeklifContainer;
    WContainerWidget* mRaporContainer;
    WContainerWidget* mKararContainer;
    WContainerWidget* mLinkContainer;

};



}


#endif // MECLISITEMPAGE_H
