#ifndef FAALIYETRAPORCONTAINER_H
#define FAALIYETRAPORCONTAINER_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "faaliyetrapor/faaliyetrapor.h"
#include "personelmanager.h"
#include "user.h"
#include <any>
#include <algorithm>
#include <QImage>

namespace v2{

namespace Faaliyet {


struct TableCell{
    int row;
    int col;
    std::string value;
};


class SquencedRaporItem{
public:
    SquencedRaporItem(){}

    void addPageBreakNext(const std::string &currentUuid );
    void addPageBreakBack(const std::string &currentUuid );

    void addBaslikNext(const int& type , const std::string &currentUuid, const std::string &baslik );
    void addBaslikBack(const int& type , const std::string &currentUuid, const std::string &baslik );

    void addImgNext( const std::string &currentUuid, const std::string &url , const std::string &title );
    void addImgBack( const std::string &currentUuid, const std::string &url , const std::string &title );

    void addTableNext( const std::string &currentUuid, const std::string &title , const std::vector<std::string> &headers, const std::vector<TableCell> &cells );
    void addTableBack( const std::string &currentUuid, const std::string &title , const std::vector<std::string> &headers, const std::vector<TableCell> &cells );

    void deleteItem( const std::string &uuid );

    void changeItem( const std::string &uuid , const std::string &text );
    void changeItem( const std::string &uuid , const std::string &title , const std::string &imgOid , const bool &deleteImgOid = true );

    void changeItem( const std::string &currentUuid, const std::string &title , const std::vector<std::string> &headers, const std::vector<TableCell> &cells );


    int count(){ return mList.count ();}

    void clearItems(){
        mList.clear();
    }

    SerikBLDCore::Faaliyet::RaporItem* operator[](const int &index ){
        return mList.at (index);
    }

    SerikBLDCore::Faaliyet::RaporItem* at(const int &index ){
        return mList.at (index);
    }

    void push_back( SerikBLDCore::Faaliyet::RaporItem* item ){
        this->mList.append (item);
    }


    QVector<SerikBLDCore::Faaliyet::RaporItem *> list() const;



    std::vector<std::string> willDeleteImgOidList;      // Kayıttan Sonra Silinecek IMG Oid Listesi


private:
    QVector<SerikBLDCore::Faaliyet::RaporItem*> mList;

};







class FaaliyetRaporContainer : public ContainerWidget, public SerikBLDCore::Faaliyet::Manager
{
public:
    explicit FaaliyetRaporContainer( SerikBLDCore::User* _mUser );

    enum class MenuPage{
        Raporlar,
        GirisBilgileri
    };

    void initMenu( const MenuPage &currentPage = MenuPage::Raporlar );

    void initFaaliyetRaporlari( const std::int64_t currentYear = 2020 );

    void initFaaliyetGirisBilgileri();

    void initNewFaaliyetGiris();

    void testPage(const std::string &faaliyetTitle , const std::string &faaliyetOid);


    void printDialog(const std::string &faaliyetTitle , const std::string &faaliyetOid );


    SquencedRaporItem itemList;


    virtual void errorOccured(const std::string &errorText) override;


    void addKaydetButon(const std::string &titleOid, const bool &unsaved = true );




private:
    SerikBLDCore::User* mUser;

    bool mViewMode = true;

    WComboBox* mYillar = nullptr;

    SerikBLDCore::PersonelManager* mPersonelManager;

    SerikBLDCore::Faaliyet::GirisBilgileri::GirisBilgileriManager* mGirisBilgiManager;
};




template< typename T >
class ItemContainer : public ContainerWidget , public T {
public:
    explicit ItemContainer( const SerikBLDCore::Faaliyet::RaporItem* item , SerikBLDCore::DB* _mDB , const bool &_mViewMode = false );



    // type int
    // 1 = BASLIK
    // 2 = ALTBASLIK
    // 3 = PARAGRAF
    // 4 = RESIM
    // 5 = TABLE

    Signal<bool,int,std::string,std::string> mBaslikSignal;

    Signal<int,std::string,std::string> mAddBeforeSignal;
    Signal<int,std::string,std::string> mAddAftereSignal;

    // Parameter Arrange: uui - imgurl - title
    Signal<std::string,std::string,std::string> mAddImgBeforeSignal;
    Signal<std::string,std::string,std::string> mAddImgAftereSignal;

    // Parameter Arrange: title - headers - cells
    Signal<std::string,std::vector<std::string>,std::vector<TableCell>> mAddTableBeforeSignal;
    Signal<std::string,std::vector<std::string>,std::vector<TableCell>> mAddTableAftereSignal;

    // Parameter: uuid
    Signal<std::string> mDeleteItemSignal;

    // Parameter: uuid - text /Baslik,Alt Baslik,Paragraf
    Signal<std::string,std::string> mChangeText;

    // Parameter: uuid - title - imgOid
    Signal<std::string,std::string,std::string> mChangeImg;
    Signal<std::string,std::string,std::string> mChangeImgTitle;


    // Parameter Arrange: uui
    Signal<NoClass> mAddPageBreakBeforeSignal;
    Signal<NoClass> mAddPageBreakAftereSignal;



    Signal<std::string,std::vector<std::string>,std::vector<TableCell>> mChangeTable;



    virtual void errorOccured(const std::string &errorText) override{
        this->showPopUpMessage ("Hata: "+errorText,"err");
    }

    WContainerWidget* addNewButton( const std::string &name,
                                    const bool &toTop,
                                    const int &rightoffSet,
                                    const std::string &className);

    void addBaslikPre( const bool &addPre = true );

    void addAltBaslikPre( const bool &addPre = true );

    void addParagrafPre( const bool &addPre = true );

    void addImgPre( const bool &addPre = true );

    void addTablePre( const bool &addPre = true );


    void addPageBreakPre( const bool &addPre = true );




    void refreshWidget();

private:
    void initWidgetType();
    void editWidgetType();

    bool mViewMode = false;


    SerikBLDCore::DB* mDB;


};












}





}



#endif // FAALIYETRAPORCONTAINER_H
