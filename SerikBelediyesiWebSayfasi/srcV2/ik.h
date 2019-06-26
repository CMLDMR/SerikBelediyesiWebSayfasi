#ifndef IK_H
#define IK_H

#include "SerikBelediyesiWebSayfasi/BaseClass/basewidget.h"


using namespace Wt;

class IK : public BaseClass::ContainerWidget
{
public:
    IK(mongocxx::database* _db, bsoncxx::document::value _user);




private:

    WContainerWidget* mPersonelListFilterContainer;
    WComboBox* mBirimFilterComboBox;
    WComboBox* mBuroPersonelFilterComboBox;
    WText* mTotalPersonelCount;


    WContainerWidget* mPersonelListContainer;
    WContainerWidget* mPersonelListPreNextBtnContainer;
    WText* mListIndexText;

    WContainerWidget* mPersonelContainer;
    WContainerWidget* mFotoContainer;
    void LoadPersonelWidgets();

    WText* mPersonelOidText;
    WLineEdit* misimLineEdit;
    WLineEdit* mtcnoLineEdit;
    WLineEdit* mtelnoLineEdit;
    WComboBox* mBirimComboBox;
    WComboBox* mStatuComboBox;
    WComboBox* mBuroPersonelComboBox;
    WText* mUpdateinformation;
    WLineEdit* mPasswordLineEdit;

    void loadPersonel( const std::string& mtcno);

    void loadPersonelList(bsoncxx::builder::basic::document filter = document{} , const int &skip = 0 );

    int mCurrentSkip;
    const int mCurrentLimit = 10;

    void _loadPersonel( const document &filter );

    void _savePersonel();



private:

    const std::string Collection{"Personel"};
    const std::string CollectionHold{"PersonelHold"};
    const std::string KEYadsoyad{"ad soyad"};
    const std::string KEYOid{"_id"};
    const std::string KEYBirimi{"Birimi"};
    const std::string KEYStatu{"Statü"};
    const std::string KEYTel{"telefon"};
    const std::string KEYTcno{"tcno"};
    const std::string KEYPasword{"password"};
    const std::string KEYBuroPer{"Büro Personeli"};
    const std::string KEYFotooid{"fotooid"};


    const std::string ErrorImgPath{"img/feature_5.png"};
};

#endif // IK_H
