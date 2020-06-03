#ifndef YENIMIMARIPROJE_H
#define YENIMIMARIPROJE_H


#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "imar/mimariproje.h"
#include "tc.h"
#include "firma/firmaitem.h"

namespace v2 {


class YeniMimariProje : public ContainerWidget, public SerikBLDCore::Imar::MimariProje::MainProje , public SerikBLDCore::DB
{
public:
    YeniMimariProje(SerikBLDCore::DB* _db ,const SerikBLDCore::TC* mTCUser , const SerikBLDCore::Firma::FirmaItem* mFirma);


    Signal<NoClass> &BackList();

private:
    Signal<NoClass> _backList;
};

}


#endif // YENIMIMARIPROJE_H
