#ifndef MECLISPUBLICPAGE_H
#define MECLISPUBLICPAGE_H

#include "meclis/meclismanager.h"
#include "meclis/meclisuyesi.h"
#include "tcmanager.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

namespace v2{


namespace MeclisLinks {

//static const std::string mainPage{"/meclis"};

//static const std::string meclisSelect( const std::string& meclis)
//{
//    return mainPage+"&select="+meclis;
//}

}

class MeclisPublicPage : public ContainerWidget ,
        public SerikBLDCore::Meclis::MeclisManager,
        public SerikBLDCore::Meclis::UyeManager
{
public:
    explicit MeclisPublicPage(SerikBLDCore::DB* db);
    virtual ~MeclisPublicPage(){
        delete tcManager;
    }

    void onList(const QVector<SerikBLDCore::Meclis::MeclisItem> *mlist) override;

    void onList(const QVector<SerikBLDCore::Meclis::MeclisUyesi> *mlist) override;


    void loadMeclisToplantilari();

    SerikBLDCore::TCManager* tcManager;



};

}


#endif // MECLISPUBLICPAGE_H
