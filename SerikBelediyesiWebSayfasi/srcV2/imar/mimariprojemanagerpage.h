#ifndef MIMARIPROJEMANAGERPAGE_H
#define MIMARIPROJEMANAGERPAGE_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "imar/mimariprojemanager.h"
#include "imar/mimariproje.h"
#include "user.h"
#include "imar/mimarilog.h"
#include "tc.h"

namespace v2{

class MimariProjeManagerPage : public ContainerWidget,
        public SerikBLDCore::Imar::MainProjeManager
{
public:
    explicit MimariProjeManagerPage(SerikBLDCore::DB *_db , SerikBLDCore::TC* _tcUser);


    void onList(const QVector<SerikBLDCore::Imar::MimariProje::MainProje> *mlist) override;
    void errorOccured(const std::string &errorText) override;

    void setFirmaOid(const std::string &firmaOid);


    virtual void loadProject( const std::string &projectOid );

    SerikBLDCore::TC* User();

private:
    SerikBLDCore::TC* mFirmaYetkilisi;
    std::string mFirmaOid;


};

class KurumsalMimariProjeManagerPage : public MimariProjeManagerPage
{
public:
    explicit KurumsalMimariProjeManagerPage(SerikBLDCore::User* _user);

    void loadProject(const std::string &projectOid) override;

private:
    SerikBLDCore::User* mUser;
};


class BireyselMimariProjeManagerPage : public MimariProjeManagerPage
{
public:
    explicit BireyselMimariProjeManagerPage(SerikBLDCore::DB *_db , SerikBLDCore::TC* _tcUser);

    void loadList();

    void loadYapiList();

    void loadProject(const std::string &projectOid) override;



private:

    const int limit = 20;
    int skip = 0;
    int mTotalItemCount;
};


}



#endif // MIMARIPROJEMANAGERPAGE_H
