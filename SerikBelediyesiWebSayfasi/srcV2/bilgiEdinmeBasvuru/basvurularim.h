#ifndef BASVURULARIM_H
#define BASVURULARIM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "bilgiEdinme/bilgiedinmemanager.h"
#include "tc.h"
#include "basvrulistitemwidget.h"

namespace V2 {
class Basvurularim : public ContainerWidget , public SerikBLDCore::BilgiEdinme::BilgiEdinmeManager
{
public:
    Basvurularim(DB* _db , const SerikBLDCore::TC& _tcUser);
};
}


#endif // BASVURULARIM_H
