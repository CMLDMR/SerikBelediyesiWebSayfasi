#ifndef CONTAINERWIGET_H
#define CONTAINERWIGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/wtheaders.h"
#include <random>
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"

#include <QString>

class ContainerWidget : public WContainerWidget
{
public:
    enum ContentType{
        Horizontal,
        Vertical
    };

    ContainerWidget(std::string title = "" , ContentType _contentType = Vertical );

    WContainerWidget* Header();
    WContainerWidget* Content();
    WContainerWidget* Footer();

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


    ///
    /// \brief removeTitleBar: Eğer Title Mevcutsa Container da Bulunan Başlığı Siler.
    ///
    void removeTitleBar();


private:
    WContainerWidget* mHeaderContainer;
    WContainerWidget* mContentContainer;
    WContainerWidget* mFootContainer;

    WContainerWidget* mTitleBar;
    std::string mTitle;
};



class FileUploaderWidget : public ContainerWidget , public DBClass
{
public:
    explicit FileUploaderWidget( mongocxx::database* _db , const std::string &title = "PDF Cevap Yükle" );


    bool isUploaded() const;

    enum FilterType{
        Pdf,
        Image
    };

    void setType( FilterType type );



    ///
    /// \brief fileLocation Location is fullPath
    /// \return
    ///
    QString fileLocation() const;

    ///
    /// \brief doocRootLocation Location is without docroot
    /// \return
    ///
    QString doocRootLocation() const;

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
