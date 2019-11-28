#ifndef VATANDASWIDGET_H
#define VATANDASWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "tc.h"
#include "tcmanager.h"

class VatandasWidget : public ContainerWidget , public SerikBLDCore::TCManager
{
public:
    VatandasWidget( SerikBLDCore::DB* db , SerikBLDCore::TC* tc );


private:
    SerikBLDCore::TC* mTC;

    void initChangeTC();

    void initTC();
};

#endif // VATANDASWIDGET_H
