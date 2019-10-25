#ifndef CONTAINERWIGET_H
#define CONTAINERWIGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/wtheaders.h"
#include <random>
#include "SerikBelediyesiWebSayfasi/BaseClass/dbclass.h"

#include <QString>

class ContainerWidget : public WContainerWidget
{
public:
    ContainerWidget(std::string title = "" );


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





    void showMessage( std::string title , std::string msg , std::string btnText = "Tamam");



private:
    WContainerWidget* mHeaderContainer;
    WContainerWidget* mContentContainer;
    WContainerWidget* mFootContainer;


};



class FileUploaderWidget : public ContainerWidget , public DBClass
{
public:
    explicit FileUploaderWidget( mongocxx::database* _db , const std::string &title = "PDF Cevap Yükle" );


    bool isUploaded() const;



    QString fileLocation() const;

    QString doocRootLocation() const;

    Signal<NoClass> &Uploaded();

private:
    WFileUpload* mFileUploader;

    QString mFileLocation;
    QString mDoocRootLocation;

    bool mIsUploaded;

    Signal<NoClass> _Uploaded;
};


#endif // CONTAINERWIGET_H
