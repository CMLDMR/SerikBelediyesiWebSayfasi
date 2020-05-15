#ifndef MIMARIPROJEMANAGERPAGE_H
#define MIMARIPROJEMANAGERPAGE_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "imar/mimariprojemanager.h"
#include "imar/mimariproje.h"

namespace v2{

class MimariProjeManagerPage : public ContainerWidget , public SerikBLDCore::Imar::MimariProjeManager
{
public:
    explicit MimariProjeManagerPage(SerikBLDCore::DB *_db);


    void onList(const QVector<SerikBLDCore::Imar::MimariProje::MimariProje> *mlist) override;

    void errorOccured(const std::string &errorText) override;

    void setFirmaOid(const std::string &firmaOid);

private:
    std::string mFirmaOid;
};

}



#endif // MIMARIPROJEMANAGERPAGE_H
