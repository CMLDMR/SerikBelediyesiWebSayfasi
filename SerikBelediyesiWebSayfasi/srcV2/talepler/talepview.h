#ifndef TALEPVIEW_H
#define TALEPVIEW_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "db.h"
#include "Talep/talep.h"
#include "Talep/talepsubitem.h"
#include "Talep/talepmanager.h"
#include "user.h"
#include "tcmanager.h"
#include "personelmanager.h"

class TalepView : public ContainerWidget , public SerikBLDCore::Talep , public SerikBLDCore::TalepManager
{
public:
    TalepView(const Talep &talepItem ,
              mongocxx::database* _db ,
              SerikBLDCore::User* _mUser,
              bool _mPublicLink = true );

    ///
    /// \brief TalepView : Bu Yapıcı Fonksiyon Sadece Vatandaş Login Olduktan Sonra Çağırılacaktır.
    /// \param talepItem
    /// \param _db
    /// \param _mTC
    ///
    TalepView(const Talep &talepItem ,
              mongocxx::database* _db ,
              SerikBLDCore::TC *_mTC );


    Signal<NoClass> &DurumChanged();

private:

    Signal<NoClass> _durumChanged;

    SerikBLDCore::User* mUser;
    SerikBLDCore::TC* mTC;
    bool mPublicLink;

    SerikBLDCore::TCManager* mTCManager;
    SerikBLDCore::PersonelManager* mPersonelManager;

    void initTCView();

    ///
    /// \brief initTalepView
    /// Public Link ve Personel için Talep Bilgilerini Görüntüler
    void initTalepView();


    ///
    /// \brief initTalepVatandasView
    /// Vatandaş Giriş Yaptığında Talep Bilgileri Görüntüler
    void initTalepVatandasView();

    ///
    /// \brief initTalepCevap
    /// Sadece Personel Arayüzü için
    void initTalepCevap();


    ///
    /// \brief initTalepVatandasCevap
    /// Vatandaş Açıklama , Resim ve PDF Ekleme Kısmı
    void initTalepVatandasCevap();


    ContainerWidget* mGorevliPersonelContainer;
    void updateGorevliPersonelWidget();
    void gorevliEkle();

    void addEventItem(SerikBLDCore::TalepSubItem::ItemType type_  , const std::string &islemGerekce = "");

    void addEventVatandasItem( SerikBLDCore::TalepSubItem::ItemType type_ );

    void addLogEventItem( const std::string& yeniDurum , const std::string &islemGerekcesi );

};

#endif // TALEPVIEW_H
