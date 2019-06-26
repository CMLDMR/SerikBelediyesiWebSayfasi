#ifndef IK_H
#define IK_H

#include "SerikBelediyesiWebSayfasi/BaseClass/basewidget.h"


using namespace Wt;


class BirimWidget;



class IK : public BaseClass::ContainerWidget
{
public:
    IK(mongocxx::database* _db, bsoncxx::document::value _user);




private:

    BirimWidget* mBirimContainer;

    WContainerWidget* mPersonelListFilterContainer;
    WComboBox* mBirimFilterComboBox;
    WComboBox* mBuroPersonelFilterComboBox;
    WText* mTotalPersonelCount;


    WContainerWidget* mPersonelListContainer;
    WContainerWidget* mPersonelListPreNextBtnContainer;
    WText* mListIndexText;

    WContainerWidget* mPersonelContainer;
    WContainerWidget* mFotoContainer;
    WFileUpload* mFileUploader;
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
    const int mCurrentLimit = 20;

    void _loadPersonel( const document &filter );

    void _savePersonel();

    void _holdPersonel();

    void _loadBirimFilterList();

    void _loadPersonelWidgetBirimList();


    QString rootnewFileName;


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

    void setFotoContainerWidget( const std::string& fotoPath );
};



class BirimWidget : public BaseClass::ContainerWidget
{
public:
    BirimWidget(mongocxx::database* _db , bsoncxx::document::value _user) ;



    Signal<NoClass> &BirimListChanged();

private:
    void LoadBirim();

    std::string deleteBirim( const std::string& birimadi);

private:
    const std::string Collection{"Müdürlükler"};
    const std::string CollectionPersonel{"Personel"};

    const std::string KEYBirim{"Birim"};


    Signal<NoClass> _BirimListChanged;

};


#endif // IK_H
