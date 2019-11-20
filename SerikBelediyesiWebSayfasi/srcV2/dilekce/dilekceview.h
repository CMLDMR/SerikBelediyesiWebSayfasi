#ifndef DILEKCEVIEW_H
#define DILEKCEVIEW_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include <vector>
#include <memory>
#include "db.h"
#include "dilekcemanager.h"
#include "user.h"
#include "tcmanager.h"
#include "personelmanager.h"
#include <QStringList>

class DilekceView : public ContainerWidget , public SerikBLDCore::Dilekce , public SerikBLDCore::DilekceManager
{
public:
    DilekceView(SerikBLDCore::Dilekce* _dilekce , mongocxx::database *_db, SerikBLDCore::User* _user , bool _mPublicLink = false , bool _mBilgi = false );
    Signal<NoClass> &ClickKapatildi();

private:
    SerikBLDCore::TCManager* mTCManager;
    SerikBLDCore::PersonelManager* mPersonelManager;

    void initTCView();

    void initDilekceView();

    void initCevapView();


private:
    SerikBLDCore::User* mUser;

    ContainerWidget* mAciklamaContainer;
    ContainerWidget* mGorevliPersonelContainer;

    ContainerWidget* mCevapContainer;
    ContainerWidget* mCevapFileContainer;
    FileUploaderWidget* mCevapUploader;

    ContainerWidget* mCevapEklerContainer;
    FileUploaderWidget* mCevapEkUploader;
    QStringList mUploadedFilePathList;

    void addAciklama(const SerikBLDCore::DilekceAciklama &aciklama);

    void gorevliEkle();
    void updateGorevliPersonelWidget();

    bool mPublicLink;
    bool mBilgi;

    Signal<NoClass> _ClickKapatildi;
};

#endif // DILEKCEVIEW_H
