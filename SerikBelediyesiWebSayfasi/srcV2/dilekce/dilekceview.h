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

class DilekceView : public ContainerWidget , public Dilekce , public DilekceManager
{
public:
    DilekceView(Dilekce* _dilekce , mongocxx::database *_db, User* _user , bool _mPublicLink = false , bool _mBilgi = false );

private:
    TCManager* mTCManager;
    PersonelManager* mPersonelManager;

    void initTCView();

    void initDilekceView();

    void initCevapView();

private:
    User* mUser;

    ContainerWidget* mAciklamaContainer;
    ContainerWidget* mGorevliPersonelContainer;

    ContainerWidget* mCevapContainer;
    ContainerWidget* mCevapFileContainer;
    FileUploaderWidget* mCevapUploader;

    ContainerWidget* mCevapEklerContainer;
    FileUploaderWidget* mCevapEkUploader;
    QStringList mUploadedFilePathList;

    void addAciklama(const DilekceAciklama &aciklama);

    void gorevliEkle();
    void updateGorevliPersonelWidget();

    bool mPublicLink;
    bool mBilgi;
};

#endif // DILEKCEVIEW_H
