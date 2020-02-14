#ifndef MECLISPUBLICPAGE_H
#define MECLISPUBLICPAGE_H

#include "meclis/meclismanager.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

namespace v2{


namespace MeclisLinks {

static const std::string mainPage{"/meclis"};

static const std::string meclisSelect( const std::string& meclis)
{
    return mainPage+"&select="+meclis;
}

}

class MeclisPublicPage : public ContainerWidget , public SerikBLDCore::Meclis::MeclisManager
{
public:
    explicit MeclisPublicPage(SerikBLDCore::DB* db);

    void onList(const QVector<SerikBLDCore::Meclis::MeclisItem> *mlist) override;



};

}


#endif // MECLISPUBLICPAGE_H
