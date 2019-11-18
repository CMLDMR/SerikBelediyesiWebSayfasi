#ifndef TALEPWIDGET_H
#define TALEPWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "Talep/talepmanager.h"
#include "tcmanager.h"


namespace TalepWidget {
class TCWidget;
class TalepItemWidget;

class TalepWidget : public ContainerWidget , public SerikBLDCore::TalepManager
{
public:
    TalepWidget(SerikBLDCore::DB* db);
    TalepWidget( mongocxx::database* _db );

    void init();

    TCWidget* tcWidget;
    TalepItemWidget* talepWidget;

};




class TCWidget : public ContainerWidget , public SerikBLDCore::TCManager
{
public:
    TCWidget(mongocxx::database* db);

    void TCChanged();
    void setDefault();
    boost::optional<SerikBLDCore::TC> TCItem();

    void setTCItem( SerikBLDCore::TC &tcItem );

private:

    WLineEdit* mTcNO;
    WLineEdit* mAdSoyad;
    WComboBox* mMahalle;
    WLineEdit* mTelefon;

    std::string mTCOid;

    bool mTCLoadFromExternal;

};


class TalepItemWidget : public ContainerWidget , public SerikBLDCore::Talep
{
public:
    TalepItemWidget(SerikBLDCore::DB& db);

    std::string mahalleString() const;
    std::string talepString() const;
    std::string adresString() const;
    std::string fotoOid() const;

    void setDefault();

private:
    WComboBox* mTalepMahalle;
    WTextEdit* mEdit;
    WLineEdit* mAdres;

    SerikBLDCore::DB mDB;

    FileUploaderWidget* mFileUploadManager;
};
}


#endif // TALEPWIDGET_H
