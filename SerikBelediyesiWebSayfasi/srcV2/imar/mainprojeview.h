#ifndef MAINPROJEVIEW_H
#define MAINPROJEVIEW_H

#include "imar/mimarilog.h"
#include "imar/mimariproje.h"
#include "imar/mimariprojemanager.h"
#include "tc.h"
#include "user.h"

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include <map>



namespace v2 {

class MainProjeView :
        public ContainerWidget,
        public SerikBLDCore::Imar::MimariProje::MainProje,
        public SerikBLDCore::Imar::BaseProjeManager,
        public SerikBLDCore::Imar::MimariLogManager
{
public:
    explicit MainProjeView( SerikBLDCore::Imar::MimariProje::MainProje& mainProje,
                   SerikBLDCore::DB* _db,
                   SerikBLDCore::TC* _mtcUser);

    virtual void initHeader();

    void addNewProje( const int& projeType );

    virtual void loadProject(const bsoncxx::oid& projectOid );








    void onList(const QVector<SerikBLDCore::Imar::MimariProje::BaseProject> *mlist) override;

    void onList(const QVector<SerikBLDCore::Imar::MimariLog> *mlist) override;

    void errorOccured(const std::string &errorText) override;

private:
    int mCurrentLogFilterIndex;

    std::map<std::string,std::string> mfilePathTitleMap;

    SerikBLDCore::TC* mTCUser;
};


class KurumsalProjeView : public MainProjeView
{
public:
    explicit KurumsalProjeView( SerikBLDCore::Imar::MimariProje::MainProje& mainProje,
                                SerikBLDCore::User* _user );

    virtual void initHeader() override;

    virtual void loadProject(const bsoncxx::oid &projectOid) override;


private:
    SerikBLDCore::User* mUser;
};

}



#endif // MAINPROJEVIEW_H
