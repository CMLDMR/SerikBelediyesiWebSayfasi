#ifndef V2_KARAVAN_KARAVANSORGUWIDGET_H
#define V2_KARAVAN_KARAVANSORGUWIDGET_H


#include <SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h>
#include "SerikBelediyesiWebSayfasi/srcV2/karavan/karavanitem.h"


namespace v2 {
namespace Karavan {

class KaravanSorguWidget : public ContainerWidget
{
public:
    KaravanSorguWidget();


    void init();

    std::string getTCNO() const;
    std::string getPlakaNo() const;
    std::int64_t getBaslangicDate() const;
    std::int64_t getBitisDate() const;
    std::string getTelefon() const;



    KaravanItem Filter();



    Signal<NoClass> &sorguClicked();

private:
    WComboBox* mSureComboBox;
    WLineEdit* mTCNOLineEdit;
    WLineEdit* mPlakaNoLineEdit;
    WPushButton* mSorguButton;

    Signal<NoClass> _SorguClicked;

    std::string mTelefonNo;

};

} // namespace Karavan
} // namespace v2

#endif // V2_KARAVAN_KARAVANSORGUWIDGET_H
