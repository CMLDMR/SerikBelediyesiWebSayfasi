#ifndef V2_KVKK_KVKKCONTAINER_H
#define V2_KVKK_KVKKCONTAINER_H

#include "SerikBelediyesiWebSayfasi/BaseClass/page.h"


namespace v2 {
namespace KVKK {

class KVKKContainer : public Comman::Page
{
public:
    KVKKContainer();

private:
    void addLink( const std::string &title , const std::string &url );

};

} // namespace KVKK
} // namespace v2

#endif // V2_KVKK_KVKKCONTAINER_H
