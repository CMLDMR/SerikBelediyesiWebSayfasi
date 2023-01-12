#ifndef CONTAINERWIGET_H
#define CONTAINERWIGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/wtheaders.h"
#include <random>
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"

#include <QString>


constexpr int MAJOR = 3;
constexpr int MINOR = 5;
constexpr int REV = 34;
constexpr int COM = 542;


#define LOG std::cout <<"\n"<< __LINE__ << " " << __FUNCTION__ << " -> "
#define LOGN std::cout << "\n"<<__LINE__ << " " << __FUNCTION__ << "\n"

class DialogContainerWidget;


class ControllerWidget : public WContainerWidget
{

    WContainerWidget* mBackContainer;
    WContainerWidget* mInfoContainer;
    WContainerWidget* mNextContainer;
    WText* mText;

    Signal<NoClass> _backClicked;
    Signal<NoClass> _nextClicked;



public:
    ControllerWidget(){

        mBackContainer = this->addNew<WContainerWidget>();
        mBackContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);
        mBackContainer->addStyleClass(Bootstrap::Button::Primary);
        mBackContainer->addNew<WText>("Geri");
        mBackContainer->clicked().connect([=](){ _backClicked.emit(NoClass());});
        mBackContainer->setPadding(5,Side::Top|Side::Bottom);
        mBackContainer->decorationStyle().setCursor(Cursor::PointingHand);

        mInfoContainer = this->addNew<WContainerWidget>();
        mInfoContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_8+
                                      Bootstrap::Grid::Medium::col_md_8+
                                      Bootstrap::Grid::Small::col_sm_8+
                                      Bootstrap::Grid::ExtraSmall::col_xs_8);
        mInfoContainer->addStyleClass(Bootstrap::Label::info);
        mText = mInfoContainer->addNew<WText>("Info");
        mInfoContainer->setPadding(5,Side::Top|Side::Bottom);

        mNextContainer = this->addNew<WContainerWidget>();
        mNextContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);
        mNextContainer->addStyleClass(Bootstrap::Button::Primary);
        mNextContainer->addNew<WText>("İleri");
        mNextContainer->clicked().connect([=](){ _nextClicked.emit(NoClass());});
        mNextContainer->setPadding(5,Side::Top|Side::Bottom);
        mNextContainer->decorationStyle().setCursor(Cursor::PointingHand);
    }

    void setText( const std::string &text ){
        this->mText->setText(text);
    }

    Signal<NoClass> &BackClicked(){
        return _backClicked;
    }

    Signal<NoClass> &NextClicked(){
        return _nextClicked;
    }


};


class ContainerWidget : public WContainerWidget
{
public:
    enum ContentType{
        Horizontal,
        Vertical
    };

    explicit ContainerWidget(const std::string &title = "" , ContentType _contentType = Vertical );

    WContainerWidget* Header();
    WContainerWidget* Content();
    WContainerWidget* Footer();

    void setTitleBarBackColor( const std::string& color = Style::color::Purple::MidnightBlue );

    void initWidget();



    WPushButton *askConfirm( const std::string& question );

    void warnDialog( const std::string &warnMessage );
    void criticDialog( const std::string &criticMessage );
    void informDialog( const std::string &informMessage );



    ContainerWidget& setDataOid( const std::string &oid ){
        this->setAttributeValue (Style::dataoid,oid);
        return *this;
    }




    std::unique_ptr<WContainerWidget> createButton( const std::string &menuName , const Cursor cursor = Cursor::Auto);


    /**
     * @brief createDialog:  WDialog Oluşturur ve Geri ptr Geri Döndürür.
     * @return
     */
    WDialog* createDialog( const std::string& title = "" );

    void removeDialog( WDialog* mDialog );


    DialogContainerWidget *createFlatDialog(const std::string &title = "");
    void removeDialog(DialogContainerWidget *removedContainer );

    enum ContainerStyleType
    {
        ROW = 0,
        CONTAINERFLUID
    };


    void setContainerStyle(ContainerStyleType type);

    inline int getRandom(int begin = 0 , int end = 127 ) const
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist(begin,end);
        return dist(mt);
    }

    inline std::string rgbaRandom(){

        return WString("{1},{2},{3}").arg (this->getRandom (150,200))
                .arg (this->getRandom (170,220))
                .arg (this->getRandom (180,230)).toUTF8 ();
    }

    void showMessage( std::string title , std::string msg , std::string btnText = "Tamam");


    /**
     * @brief showPopUpMessage : Mesaj Gönder infoType info,msg ve ""
     * @param msg
     * @param infoType info,msg ve boşluk
     */
    void showPopUpMessage(const std::string & msg , const std::string &infoType = "info" );///*msg*/


    ///
    /// \brief removeTitleBar: Eğer Title Mevcutsa Container da Bulunan Başlığı Siler.
    ///
    void removeTitleBar();


    WContainerWidget *titleBar() const;

    ///
    /// \brief getContoller
    /// \return
    ///
    std::optional<ControllerWidget*> getContoller() {
        if( !mController ){
            mController = addNew<ControllerWidget>();
            mController->addStyleClass(Bootstrap::Grid::col_full_12);
            mController->setMargin(20,Side::Bottom);
            mController->setMargin(10,Side::Top);
        }

        return mController;
    }

private:
    WContainerWidget* mHeaderContainer;
    WContainerWidget* mContentContainer;
    WContainerWidget* mFootContainer;

    WContainerWidget* mTitleBar;
    std::string mTitle;

    bool initController = false;
    ContainerWidget::ContentType mContainerStyle;
    ControllerWidget* mController = nullptr;

};



class DialogContainerWidget : public WContainerWidget
{
public:
    explicit DialogContainerWidget(const std::string &title);

    WContainerWidget* Content();



    Signal<NoClass> &Accepted();
    Signal<NoClass> &Rejected();
private:
    WContainerWidget* mContent;

    Signal<NoClass> _Accepted;
    Signal<NoClass> _Rejected;
};



class FileUploaderWidget : public ContainerWidget /*, public DBClass*/
{
public:
    explicit FileUploaderWidget( const std::string &title = "PDF Cevap Yükle" );





    /**
     * @brief isUploaded: Check file is Uploaded
     * @return
     */
    bool isUploaded() const;





    /**
     * @brief The FilterType enum: Selectable File Type
     */
    enum FilterType{
        Pdf,
        Image,
        DWG
    };





    /**
     * @brief setType: File Type
     * @param type: Pdf , Image
     */
    void setType( FilterType type );






    /**
     * @brief fileLocation: Location is FULL PATH
     * @return
     */
    QString fileLocation() const;





    /**
     * @brief doocRootLocation: Location is Without docroot folder
     * @return
     */
    QString doocRootLocation() const;




    /**
     * @brief Uploaded: Signal Triggered When File Uploaded
     * @return
     */
    Signal<NoClass> &Uploaded();

private:
    WFileUpload* mFileUploader;

    QString mFileLocation;
    QString mDoocRootLocation;

    bool mIsUploaded;

    Signal<NoClass> _Uploaded;

    FilterType mType;
};



#endif // CONTAINERWIGET_H
