#ifndef TALEPYONETIM_H
#define TALEPYONETIM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include <vector>
#include <memory>
#include "db.h"
#include "user.h"
#include "Talep/talepmanager.h"

class TalepYonetim : public ContainerWidget , public TalepManager
{
public:
    TalepYonetim(mongocxx::database* _db , bsoncxx::document::value _user);


    Signal<std::string> &clickOid();

private:
    User* mUser;

    void initControlPanel();

    void listTalepler( const Talep& filter = Talep() );

    void initTalep( const std::string &oid );

    std::unique_ptr<WPushButton> createButton( const char* classname , const WString& name );

    Signal<std::string> _clickOid;

};

#endif // TALEPYONETIM_H
