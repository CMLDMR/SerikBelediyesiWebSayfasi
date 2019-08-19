#include "dbclass.h"

#include <QString>
#include <QFile>
#include <QByteArray>
#include <QFileInfo>
#include <fstream>

#include <iostream>

DBClass::DBClass( mongocxx::database* _db )
    :__db(_db)
{

}

mongocxx::database *DBClass::db() const
{
    return __db;
}

std::vector<std::string> DBClass::BirimList() const
{

    auto filter = document{};

    try {
        filter.append(kvp("Haberleşme Kodu",make_document(kvp("$ne","0"))));
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }


    std::vector<std::string> list;


    try {
        auto cursor = this->db()->collection("Müdürlükler").find(filter.view());

        for( auto doc : cursor )
        {
            try {
                list.push_back(doc["Birim"].get_utf8().value.to_string());
            } catch (bsoncxx::exception &e) {
                std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                std::cout << str << std::endl;
            }
        }

    } catch (mongocxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }



    return list;

}

const std::string DBClass::downloadFile(const std::string &oid, const bool &forceFilename)
{

    if( QFile::exists( QString("docroot/tempfile/%1.indexed").arg(oid.c_str()) ) )
    {
        QFile file(QString("docroot/tempfile/%1.indexed").arg(oid.c_str()));
        if( file.open(QIODevice::ReadOnly) )
        {
            QString fileName = QString::fromUtf8(file.readAll());
            file.close();
            return fileName.toStdString();
        }
    }else{
        std::cout << "FILE NOT FOUND: " << QString("docroot/tempfile/%1.indexed").arg(oid.c_str()).toStdString() << std::endl;
    }




    auto doc = bsoncxx::builder::basic::document{};

    try {
        doc.append(bsoncxx::builder::basic::kvp("key",bsoncxx::oid{oid}));
    } catch (bsoncxx::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return "img/404-header.png";
    }

    mongocxx::gridfs::downloader downloader;

    try {
        downloader = this->db()->gridfs_bucket().open_download_stream(bsoncxx::types::value(bsoncxx::types::b_oid{bsoncxx::oid{oid}}));
    } catch (mongocxx::gridfs_exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FILE__ << " " << e.what() << std::endl;
        return "img/error.png";
    }


    auto file_length = downloader.file_length();
//    auto bytes_counter = 0;

    QFileInfo info( downloader.files_document()["filename"].get_utf8().value.to_string().c_str() );

    QString fullFilename;

    if( forceFilename )
    {
        fullFilename = QString("tempfile/%1").arg(downloader.files_document()["filename"].get_utf8().value.to_string().c_str());
    }else{
        fullFilename = QString("tempfile/%2.%1").arg(info.suffix())
                    .arg(downloader.files_document()["_id"].get_oid().value.to_string().c_str());
    }


    auto buffer_size = std::min(file_length, static_cast<std::int64_t>(downloader.chunk_size()));
    auto buffer = bsoncxx::stdx::make_unique<std::uint8_t[]>(static_cast<std::size_t>(buffer_size));

    std::ofstream out;

    out.open("docroot/"+fullFilename.toStdString(),std::ios::out | std::ios::app | std::ios::binary);


    if( out.is_open() )
    {
        while ( auto length_read = downloader.read(buffer.get(), static_cast<std::size_t>(buffer_size)) ) {
            auto bufferPtr = buffer.get();
            out.write( reinterpret_cast<char*>( bufferPtr ) , length_read );
//            bytes_counter += static_cast<std::int32_t>( length_read );
        }
        out.close();
    }

    else{
        std::cout << "Error Can Not Open File: " <<"docroot/"+fullFilename.toStdString() << std::endl;
        return "img/error.png";
    }


    QFile file(QString("docroot/tempfile/%1.indexed").arg(oid.c_str()));
    if( file.open(QIODevice::ReadWrite) )
    {
        file.write(fullFilename.toUtf8());
        file.close();
    }else{
        std::cout << "FILE CAN NOT CREATED: " << file.fileName().toStdString() << fullFilename.toStdString() << std::endl;
    }

    std::cout << "FILE FORCED : " << forceFilename <<" FILE FILL: " << fullFilename.toStdString() <<" TOTHIS FILE: " << file.fileName().toStdString() << std::endl;

    return fullFilename.toStdString();
}

const bsoncxx::types::value DBClass::uploadfile(QString filepath)
{
    QFile file( filepath );
    if( file.open( QIODevice::ReadOnly ) )
    {
        QFileInfo info(filepath);
        auto uploader = this->db()->gridfs_bucket().open_upload_stream(info.fileName().toStdString().c_str());
        QByteArray ar = file.readAll();
        uploader.write((std::uint8_t*)ar.data(),ar.size());
        auto res = uploader.close();
        file.close();
        return res.id();
    }else{
        throw("Dosya Açılamadı");
    }
}

UserClass::UserClass(const bsoncxx::document::value &_userValue)
    :mUserValue(_userValue)
{

}

UserClass::UserClass(const UserClass &_user)
    :mUserValue(_user.UserValue())
{

}

bsoncxx::document::value UserClass::UserValue() const
{
    return mUserValue;
}

UserClass UserClass::User() const
{
    return *this;
}

boost::optional<std::string> UserClass::birim()
{
    try {
        return this->mUserValue.view()["Birimi"].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
        return boost::none;
    }
}

boost::optional<std::string> UserClass::adSoyad()
{
    try {
        return this->mUserValue.view()["ad soyad"].get_utf8().value.to_string();
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
        return boost::none;
    }
}

bool UserClass::yetki(const std::string &yetkiAdi)
{
    bool exist = false;
    try {
        auto array = this->mUserValue.view()["Yetkiler"].get_array().value;
        for( auto item : array )
        {
            if( yetkiAdi == item.get_utf8().value.to_string() )
            {
                exist = true;
                break;
            }
        }
    } catch (bsoncxx::exception &e) {
        std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
        std::cout << str << std::endl;
    }
    return exist;
}
