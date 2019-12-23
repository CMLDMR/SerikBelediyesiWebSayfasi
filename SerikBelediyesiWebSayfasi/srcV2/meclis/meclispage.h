#ifndef MECLISPAGE_H
#define MECLISPAGE_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "meclis/meclismanager.h"


namespace v2 {

/**
 * @brief The MeclisPageManager class:  Meclis Kararları Yönetim Paneli
 */
class MeclisPageManager : public ContainerWidget , public SerikBLDCore::Meclis::MeclisManager
{
public:
    explicit MeclisPageManager( DB* db );

    void initController();

    virtual void onList( const QVector<SerikBLDCore::Meclis::MeclisItem>* mlist ) override;

    void yeniEkle();

    void initMeclisPage( const SerikBLDCore::Meclis::MeclisItem &meclisItem );
};

/**
 * @brief The MeclisNewItemPage class
 */
class MeclisNewItemPage : public ContainerWidget , public SerikBLDCore::Meclis::MeclisItem
{
public:
    explicit MeclisNewItemPage();


    Signal<NoClass> &ClickSaved();

//    int yil() const;
//    std::string ay() const;
//    bool yayinda() const;
//    std::string gundem() const;


private:
    WSpinBox* yilSpinBox;
    WLineEdit* ayTextEdit;
    WCheckBox* onlineCheckBox;
    WTextEdit* mGundemEdit;

    Signal<NoClass> _ClickSaved;

};

}




#endif // MECLISPAGE_H
