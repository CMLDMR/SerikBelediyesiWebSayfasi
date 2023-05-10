#include "cssbuilder.h"

#include <mutex>
#include <iostream>

#include <QFile>
#include <QByteArray>




namespace Widget {

std::once_flag mCSSOnceFlag;
CSSBuilder* CSSBuilder::mBuilder{nullptr};

CSSBuilder::CSSBuilder()
{

    //    :/button/src/button.css

}

CSSBuilder *CSSBuilder::instance()
{

    std::call_once(mCSSOnceFlag,[=](){
        mBuilder = new CSSBuilder();
        mBuilder->initCSS();
    });

    return mBuilder;
}

void CSSBuilder::initCSS()
{
    std::cout << "\ninstall button.css " << installCSS(":/button/src/button.css","button");
}



bool CSSBuilder::installCSS(const std::string &sourcePath, const std::string &cssName )
{


    const std::string dest = "css/resourceCSS/"+cssName+".css";
    const std::string filePath{"docroot/"+dest};


    QFile file( sourcePath.data() );


    QByteArray data;
    if( file.open(QIODevice::ReadOnly) ){
        data = file.readAll();
        file.close();
    }else{
        std::cout << sourcePath << " Can Not Copy" << "\nError: " << file.errorString().toStdString() << "\n";
        file.close();
        return false;
    }

    file.setFileName(filePath.data());
    if( file.exists() ){
        file.remove();
    }

    if( !file.open(QIODevice::ReadWrite) ){
        std::cout << sourcePath << " Can Not Copy" << "\n";
        file.close();
        return false;
    }else{
        file.write(data);
        file.close();
    }

    mFileListPath.push_back(dest);

    return true;
}

std::vector<std::string> CSSBuilder::getCSSPathList() const
{
    return mFileListPath;
}

} // namespace Widget
