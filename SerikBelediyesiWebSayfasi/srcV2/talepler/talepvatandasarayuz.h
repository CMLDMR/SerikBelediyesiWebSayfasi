#ifndef TALEPVATANDASARAYUZ_H
#define TALEPVATANDASARAYUZ_H

#include "Talep/talepmanager.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "tc.h"


class TalepVatandasArayuz : public ContainerWidget , public SerikBLDCore::TalepManager
{
public:
    TalepVatandasArayuz(mongocxx::database* _db , bsoncxx::document::value* _userValue );



    void initList();

    void initTalep( const std::string &oid );


private:
    SerikBLDCore::TC mTCUser;
};

#endif // TALEPVATANDASARAYUZ_H
