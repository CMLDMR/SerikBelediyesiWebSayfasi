#ifndef VATANDASWIDGET_H
#define VATANDASWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "tc.h"
#include "tcmanager.h"

class VatandasWidget : public ContainerWidget , public SerikBLDCore::TCManager
{
public:
    VatandasWidget( SerikBLDCore::DB* db , SerikBLDCore::TC* tc );

    void initTCView(SerikBLDCore::TC* _mTC);

private:
    SerikBLDCore::TC* mTC;

    void initChangeTC();

    void initTC();

    bool mEnableEdit;

};



//TODO: Çalışmalarla ilgili SMS Gidecek mi?
class VatandasYeniKayitWidget : public ContainerWidget , public SerikBLDCore::TCManager
{
public:
    explicit VatandasYeniKayitWidget( SerikBLDCore::DB* db );



    Signal<std::string> &onExisted();

private:
    SerikBLDCore::TC* mTC;

    bool __tcKayitli = true;
    bool __telefonKayitli = true;
    bool __kayitYapilabilir = false;

    WLineEdit* adreslineEdit,*telefonLineEdit,*adsoyadLineEdit,*tcLineEdit;
    WComboBox* mahalleComboBox;
    WLineEdit* tcoidLineEdit;


    Signal<std::string> _onExisted;


};

#endif // VATANDASWIDGET_H
