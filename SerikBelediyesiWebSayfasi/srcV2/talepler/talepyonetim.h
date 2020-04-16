#ifndef TALEPYONETIM_H
#define TALEPYONETIM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"

#include <vector>
#include <memory>
#include "db.h"
#include "user.h"
#include "Talep/talepmanager.h"
#include "Talep/talepkategorimanager.h"

class TalepYonetim : public ContainerWidget , public SerikBLDCore::TalepManager
{
    enum class SiralamaType{
        NormalSiralama,
        Raporlama
    };


public:
    TalepYonetim(mongocxx::database* _db , const bsoncxx::document::value _user);

    Signal<std::string> &clickOid();

private:
    SerikBLDCore::User* mUser;
    SerikBLDCore::TalepKategoriManager* mTalepKategoriManager;

    void initControlPanel();

    void initPersonelPanel();

    void listTalepler( const SerikBLDCore::Talep& filter = SerikBLDCore::Talep() ,
                       const SerikBLDCore::FindOptions& options = SerikBLDCore::FindOptions() ,
                       SiralamaType siralamaType = SiralamaType::NormalSiralama );

    void initKategoriDagilimi();

    void initTalep( const std::string &oid );

    std::unique_ptr<WPushButton> createButton( const char* classname , const WString& name );

    Signal<std::string> _clickOid;



};

#endif // TALEPYONETIM_H
