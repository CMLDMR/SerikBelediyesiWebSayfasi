#ifndef PERSONELMANAGERPAGE_H
#define PERSONELMANAGERPAGE_H



#include "personelmanager.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"


class IKManagerPage : public ContainerWidget
{
public:
    IKManagerPage(SerikBLDCore::DB* _db );

private:
    SerikBLDCore::DB* mDB;

    void initPersonelManager();

    void initBirimManager();
};

class PersonelManagerPage : public SerikBLDCore::PersonelManager , public ContainerWidget
{
public:
    PersonelManagerPage(SerikBLDCore::DB* _db );


    virtual void onList(const QVector<SerikBLDCore::IK::Personel> *mlist) override;



private:

};

#endif // PERSONELMANAGERPAGE_H
