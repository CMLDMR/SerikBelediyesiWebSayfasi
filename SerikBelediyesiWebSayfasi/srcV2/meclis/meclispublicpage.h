#ifndef MECLISPUBLICPAGE_H
#define MECLISPUBLICPAGE_H

#include "meclis/meclismanager.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

class MeclisPublicPage : public ContainerWidget , public SerikBLDCore::Meclis::MeclisManager
{
public:
    MeclisPublicPage(SerikBLDCore::DB* db);
};

#endif // MECLISPUBLICPAGE_H
