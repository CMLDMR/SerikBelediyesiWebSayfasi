#include "smsmanager.h"
#include "../url.h"
#include <QDate>
#include <QTime>


SMSManager::SMSManager(DB *_db)
    :SerikBLDCore::SMSAbstractManager (_db)
{
    mHttpClient = wApp->root ()->addChild (std::make_unique<Http::Client>());
    mHttpClient->setSslCertificateVerificationEnabled(false);
    mSMSGonderiliyor = false;
}

SMSManager::~SMSManager()
{
    wApp->root()->removeChild (mHttpClient);
}

bool SMSManager::insertAndSendSMS(const SerikBLDCore::SMS::SMSItem &item)
{

    if( !confirmTelefonNumarasi ( item.numaraText () ) )
    {
        _errorOccured.emit (QString("Telefon Numarasında Hata Var").toStdString ());
        return false;
    }


    int kalanSure = 0;
    if( !canSend ( item.numaraText () , kalanSure ) )
    {
        _errorOccured.emit (QString("Bu Numaraya 1 DK içinde 2 SMS Atamazsınız. Kalan Süre:%1").arg (QTime::fromMSecsSinceStartOfDay (60000-kalanSure).toString ("mm:ss")).toStdString ());
        return false;
    }

    if( mSMSGonderiliyor )
    {
        _errorOccured.emit ("Şuanda Başka Bir SMS Gönderiliyor");
        return false;
    }
    mSMSGonderiliyor = true;
    QByteArray ar;
    ar.clear();
    QXmlStreamWriter stream(&ar);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
    stream.writeStartElement("MainmsgBody");
    stream.writeTextElement("Command", SMSCONFIG::Command.c_str());
    stream.writeTextElement("PlatformID", SMSCONFIG::PlatformID.c_str());
    stream.writeTextElement("ChannelCode", SMSCONFIG::ChannelCode.c_str ());
    stream.writeTextElement("UserName", SMSCONFIG::UserName.c_str ());
    stream.writeTextElement("PassWord", SMSCONFIG::Password.c_str ());
    stream.writeTextElement("Mesgbody", item.smsText ());
    stream.writeTextElement("Numbers", item.numaraText ());
    stream.writeTextElement("Type", SMSCONFIG::Type.c_str ());
    stream.writeTextElement("Concat", SMSCONFIG::Concat.c_str());
    stream.writeTextElement("Originator", SMSCONFIG::Originator.c_str ());
    stream.writeEndElement();
    stream.writeEndDocument();

    auto _mHttpClient = wApp->root ()->addChild (std::make_unique<Http::Client>());

    Http::Message msg;
    msg.setHeader("Content-Type","application/json");
    msg.addBodyText(ar.toStdString());
    _mHttpClient->done().connect([=](AsioWrapper::error_code code, Http::Message message){

        SerikBLDCore::SMS::SMSItem _item;
        _item.setRaporID (6);
        _item.setID (message.body().c_str());
        _item.setSMS (item.smsText ());
        _item.setNumara (item.numaraText ());
        _item.setJulianDay (QDate::currentDate ().toJulianDay ());
        _item.setmSecStartOfDay (QTime::currentTime ().msecsSinceStartOfDay ());

        auto val = this->insertItem (_item );
        if( val )
        {
            _messageOccured.emit ("SMS ID Alındı ve Kayıt Edildi");
            mSMSGonderiliyor = false;
        }else{
            _errorOccured.emit ("SMS Gönderildi Ancak Kayıt Edilemedi");
            mSMSGonderiliyor = false;
        }
        wApp->root()->removeChild (_mHttpClient);

    });

    _mHttpClient->post("http://processor.smsorigin.com/xml/process.aspx",msg);


}

bool SMSManager::checkRapor(const SerikBLDCore::SMS::SMSItem &item)
{
    auto app = wApp->instance ();
    WApplication::UpdateLock lock(app);
    if( app )
    {
        app->enableUpdates (true);
        if( mSMSGonderiliyor )
        {
            _errorOccured.emit ("Rapor Sogulanıyor. Daha Sonra Tekrar Deneyin");
            return false;
        }

        mSMSGonderiliyor = true;

        mLastCheckItemOid = QString::fromStdString (item.oid ().value ().to_string ());

        //ID:645408335
        QByteArray ar;
        ar.clear();
        QXmlStreamWriter stream(&ar);
        stream.setAutoFormatting(true);
        stream.writeStartDocument();
        stream.writeStartElement("MainReportRoot");
        stream.writeTextElement("Command", QString::number (15));
        stream.writeTextElement("PlatformID", SMSCONFIG::PlatformID.c_str());
        stream.writeTextElement("ChannelCode", SMSCONFIG::ChannelCode.c_str ());
        stream.writeTextElement("UserName", SMSCONFIG::UserName.c_str ());
        stream.writeTextElement("PassWord", SMSCONFIG::Password.c_str ());
        stream.writeTextElement("MsgID", item.idText ().mid (3,9));
        stream.writeTextElement("Status", "");
        stream.writeTextElement("AddDeliveryTime", "1");
        stream.writeEndElement();
        stream.writeEndDocument();

        auto _mHttpClient = wApp->root ()->addChild (std::make_unique<Http::Client>());

        Http::Message msg;
        msg.setHeader("Content-Type","application/json");
        msg.addBodyText(ar.toStdString());

        _mHttpClient->done().connect([=](AsioWrapper::error_code code, Http::Message message){

            std::cout << code.message () << " :\n" << message.body () << std::endl;
            SerikBLDCore::SMS::SMSItem _item;
            _item.setOid (mLastCheckItemOid.toStdString ());
            QString rapor = QString::fromStdString (message.body ());
            auto raporlist = rapor.split (" ");
            QString raporText;
            if( raporlist.size () >= 3 )
            {
                QString _numara = raporlist.first ();
                QString _sonuc = raporlist.at (1);
                _item.setRaporID (_sonuc.toInt ());
                QString _zaman = raporlist.at (2);

                switch (_sonuc.toUInt ()) {
                case 3:
                    _sonuc = "Başarılı (Alıcıya başarılı olarak iletilmiş)";
                    break;
                case 5:
                    _sonuc = "Başarısız (GSM numarası hatalı ya da gönderim iptal edilmiş)";
                    break;
                case 6:
                    _sonuc = "Beklemede (İşlemde veya gönderilmeye çalışılıyor)";
                    break;
                case 9:
                    _sonuc = "Zaman Aşımı (Geçerlilik süresi bitene kadar gönderilmeye çalışılmış fakat başarısız olmuş)";
                    break;
                default:
                    break;
                }

                raporText = QDate::fromString (_zaman.mid (0,8),"yyyyMMdd").toString ("dd/MM/yyyy dddd")+
                        QTime::fromString (_zaman.mid (8,6),"hhmmss").toString ("hh:mm:ss") + " Tarihinde : " + _sonuc;

                _item.setRapor (raporText);

            }


            if( raporText.count () )
            {
                auto val = this->updateItem (_item);
                if( val )
                {
                    if( val.value ().modified_count () )
                    {
                        _messageOccured.emit ("Rapor Alındı ve Kayıt Edildi");
                    }else{
//                        _errorOccured.emit ("Bilgi: Rapor Değişikliği Yok. "+this->getLastError ().toStdString ());
                    }
                }else{
                    _errorOccured.emit (this->getLastError ().toStdString ());
                }
            }else{
                _errorOccured.emit ("Alınan Rapor da Bir Hata Oluştu: " + rapor.toStdString () + " - " + std::to_string (rapor.split (" ").count ()));
            }
            mSMSGonderiliyor = false;
            wApp->root()->removeChild (_mHttpClient);
            wApp->instance ()->triggerUpdate ();
            wApp->instance ()->enableUpdates (false);
        });
        _mHttpClient->post("http://processor.smsorigin.com/xml/process.aspx",msg);
    }



}

Signal<std::string> &SMSManager::smsSended()
{
    return _smsSended;
}

Signal<std::string> &SMSManager::ErrorOccured()
{
    return _errorOccured;
}

Signal<std::string> &SMSManager::messageOccured()
{
    return _messageOccured;
}
