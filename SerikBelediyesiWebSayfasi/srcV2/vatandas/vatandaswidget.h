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



//TODO: Çalışmalarla ilgili SMS Gidecek mi?
class VatandasYeniKayitWidget : public ContainerWidget , public SerikBLDCore::TCManager
{
public:
    explicit VatandasYeniKayitWidget( SerikBLDCore::DB* db );




private:
    SerikBLDCore::TC* mTC;

    bool __tcKayitli = true;
    bool __telefonKayitli = true;
    bool __kayitYapilabilir = false;

    WLineEdit* adreslineEdit,*telefonLineEdit,*adsoyadLineEdit,*tcLineEdit;
    WComboBox* mahalleComboBox;
    WLineEdit* tcoidLineEdit;


};

#endif // VATANDASWIDGET_H
