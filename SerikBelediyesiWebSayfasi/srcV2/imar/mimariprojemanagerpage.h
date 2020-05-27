#ifndef MIMARIPROJEMANAGERPAGE_H
#define MIMARIPROJEMANAGERPAGE_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "imar/mimariprojemanager.h"
#include "imar/mimariproje.h"
#include "SerikBelediyesiWebSayfasi/srcV2/imar/mimarprojeview.h"
#include "user.h"
#include "imar/mimarilog.h"
#include "tc.h"

namespace v2{

class MimariProjeManagerPage : public ContainerWidget,
        public SerikBLDCore::Imar::MimariProjeManager,
        public SerikBLDCore::Imar::MimariLogManager
{
public:
    explicit MimariProjeManagerPage(SerikBLDCore::DB *_db , SerikBLDCore::TC* _tcUser);


    void onList(const QVector<SerikBLDCore::Imar::MimariProje::MimariProje> *mlist) override;
    void errorOccured(const std::string &errorText) override;

    void onList(const QVector<SerikBLDCore::Imar::MimariLog> *mlist) override;


    void setFirmaOid(const std::string &firmaOid);

    void loadProject( const std::string& projeOid );



private:
    SerikBLDCore::TC* mFirmaYetkilisi;
    std::string mFirmaOid;

    int mCurrentLogFilterIndex;

    WContainerWidget* mProjectListContainer;

    WContainerWidget* mProjectInformationContainer;
    WContainerWidget* mProjectHeaderContainer;
    WContainerWidget* mProjectDetailContainer;

    WContainerWidget* mMimariProjectContainer;
    WText* mMimariOnayText = nullptr;
    WContainerWidget* mMimariOnayTextContainer = nullptr;


    WContainerWidget* mStatikProjectContainer;
    WText* mStatikOnayText = nullptr;
    WContainerWidget* mStatikOnayTextContainer = nullptr;
};

}

namespace Kurumsal {

namespace v2 {

class MimariProjeManagerPage : public ContainerWidget,
        public SerikBLDCore::Imar::MimariProjeManager,
        public SerikBLDCore::Imar::MimariLogManager
{
public:
    explicit MimariProjeManagerPage( SerikBLDCore::User* _user );

    void onList(const QVector<SerikBLDCore::Imar::MimariProje::MimariProje> *mlist) override;
    void errorOccured(const std::string &errorText) override;

    void onList(const QVector<SerikBLDCore::Imar::MimariLog> *mlist) override;


    void loadProject( const std::string& projeOid );

    void loadMimariProjectBtn(Comman::MimarProjeView* projectView);



private:
    SerikBLDCore::User* mUser;

    int mCurrentLogFilterIndex;

    WContainerWidget* mProjectListContainer;

    WContainerWidget* mProjectInformationContainer;
    WContainerWidget* mProjectHeaderContainer;
    WContainerWidget* mProjectDetailContainer;

    WContainerWidget* mMimariProjectContainer;
    WText* mMimariOnayText = nullptr;
    WContainerWidget* mMimariOnayTextContainer = nullptr;


    WContainerWidget* mStatikProjectContainer;
    WText* mStatikOnayText = nullptr;
    WContainerWidget* mStatikOnayTextContainer = nullptr;

};

}

}

#endif // MIMARIPROJEMANAGERPAGE_H
