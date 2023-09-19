#ifndef V2_KARAVAN_KARAVANCONTROLWIDGET_H
#define V2_KARAVAN_KARAVANCONTROLWIDGET_H


#include <SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h>

#include "SerikBelediyesiWebSayfasi/srcV2/karavan/karavanitem.h"
#include "tcmanager.h"
#include "user.h"
#include "karavansorguwidget.h"

namespace v2 {
namespace Karavan {



class MainWidget : public ContainerWidget, public v2::Karavan::KaravanManager
{
public:
    MainWidget(SerikBLDCore::User* _mUser);

    // KaravanManager interface
public:
    virtual void onList(const QVector<KaravanItem> *mlist) override;

private:
    SerikBLDCore::TCManagerV2* mTCManager;

    KaravanSorguWidget* mSorguWidget;

};




class KaravanControlWidget : public ContainerWidget
{
public:
    KaravanControlWidget();

    void init(const std::string &tcno = "");


    void setExistTCNO(SerikBLDCore::DB *db, SerikBLDCore::TC *tcvalue );
    void setNewTCNO(const std::string &tcno , SerikBLDCore::TCManagerV2* mTCManager);


    Signal<NoClass> &saveClicked();

    std::string getTCNO() const;
    std::string getPlakaNo() const;
    std::int64_t getBaslangicDate() const;
    std::int64_t getBitisDate() const;
    std::string getTelefon() const;



    Signal<std::string> &tCNOClicked();

private:
    WDateEdit* mBaslangicDate;
    WDateEdit* mBitisDate;
    WLineEdit* mTCNOLineEdit;
    WLineEdit* mPlakaNoLineEdit;
    WPushButton* mKaydetButton;

    Signal<NoClass> mSaveClicked;
    Signal<std::string> mTCNOClicked;

    std::string mTelefonNo;

};







class ListWidget : public ContainerWidget, public v2::Karavan::KaravanItem
{
public:
    ListWidget(const int &index, const KaravanItem &item);


    Signal<std::string> silClicked() const;
    void setSilClicked(const Signal<std::string> &newSilClicked);

    Signal<std::string> &updateClicked();

    Signal<std::string> &silClicked();

    Signal<std::string, std::string> &detailClicked();

private:
    Signal<std::string> _silClicked;
    Signal<std::string,std::string> _detailClicked;
    Signal<std::string> _updateClicked;
};


} // namespace Karavan
} // namespace v2

#endif // V2_KARAVAN_KARAVANCONTROLWIDGET_H
