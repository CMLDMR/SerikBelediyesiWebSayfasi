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
    explicit TalepWidget(SerikBLDCore::DB* db);
    explicit TalepWidget( mongocxx::database* _db );

    void init();

    TCWidget* tcWidget;
    TalepItemWidget* talepWidget;

};




class TCWidget : public ContainerWidget , public SerikBLDCore::TCManager
{
public:
    explicit TCWidget(mongocxx::database* db);

    void TCChanged();
    void setDefault();
    std::optional<SerikBLDCore::TC> TCItem();

    std::optional<SerikBLDCore::TC> saveTCItem();


    std::string lastError() const;
    void setError( const std::string &errorStr );
    void setTCItem( SerikBLDCore::TC &tcItem );

    enum errorCode{
        tcnoHatali,
        adsoyadHatali,
        mahalleHatali,
        telefonHatali,
        tcoidHatali
    };

    void setErrorCode(const TCWidget::errorCode &errorCode);
    errorCode lasterrorCode() const;
private:

    std::string mErrorString;
    errorCode mErrorCode;

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
    explicit TalepItemWidget(const SerikBLDCore::DB& db);

    std::string mahalleString() const;
    std::string talepString() const;
    std::string adresString() const;
    std::string fotoOid() const;
    bool bilgilerimGizli() const;

    void setDefault();

private:
    WComboBox* mTalepMahalle;
    WTextEdit* mEdit;
    WLineEdit* mAdres;
    WCheckBox* mBilgilerimiGizle;

    SerikBLDCore::DB mDB;

    FileUploaderWidget* mFileUploadManager;
};
}


#endif // TALEPWIDGET_H
