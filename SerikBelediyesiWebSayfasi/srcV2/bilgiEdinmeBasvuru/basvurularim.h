#ifndef BASVURULARIM_H
#define BASVURULARIM_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "bilgiEdinme/bilgiedinmemanager.h"
#include "tc.h"
#include "basvrulistitemwidget.h"
#include "bilgiEdinme/bilgiedinmeitem.h"
#include "user.h"




namespace V2 {

class Basvurularim : public ContainerWidget , public SerikBLDCore::BilgiEdinmeManager
{
public:
    Basvurularim(DB* _db , const SerikBLDCore::TC& _tcUser);
    virtual ~Basvurularim();


    void initControlWidget();

    virtual void onList( const QVector<SerikBLDCore::BilgiEdinmeItem> *mlist ) final;

    std::unique_ptr<SerikBLDCore::BilgiEdinmeItem> mCurrentFilter;

    void initItem( const std::string& itemOid );

};



class GelenBasvurular : public ContainerWidget , public SerikBLDCore::BilgiEdinmeManager
{
public:
    GelenBasvurular(DB* _db , bsoncxx::document::value _user );
    virtual ~GelenBasvurular();


    void initControlWidget();

    virtual void onList( const QVector<SerikBLDCore::BilgiEdinmeItem> *mlist ) final;

    std::unique_ptr<SerikBLDCore::BilgiEdinmeItem> mCurrentFilter;

    void initItem( const std::string& itemOid );

    SerikBLDCore::User* mUser;

};

}


#endif // BASVURULARIM_H
