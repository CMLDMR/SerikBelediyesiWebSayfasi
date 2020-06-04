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

// TODO: Sadece Bir Proje Görecek Şekilde Gösterilecek Bir Sınıf Daha Gösterilecek.
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

    virtual void onList(const QVector<SerikBLDCore::Imar::MimariLog> *mlist) override;

    void errorOccured(const std::string &errorText) override;

    std::string selectedProjectOid() const;

    int currentLogFilterIndex() const;

    virtual void addIslemLog( const std::string& log , const bsoncxx::oid& projeOid );

    void setSelectedProjectOid(const std::string &selectedProjectOid);

private:
    int mCurrentLogFilterIndex;
    std::string mSelectedProjectOid;

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

    void onList(const QVector<SerikBLDCore::Imar::MimariLog> *mlist) override;

    virtual void addIslemLog(const std::string &log , const bsoncxx::oid& projeOid ) override;


private:
    SerikBLDCore::User* mUser;
};

}



#endif // MAINPROJEVIEW_H
