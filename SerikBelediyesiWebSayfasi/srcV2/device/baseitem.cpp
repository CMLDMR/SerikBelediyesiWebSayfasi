
#include "baseitem.h"


namespace TodoList {



BaseItem::BaseItem(const Type &type, SerikBLDCore::User *_mUser)
    :SerikBLDCore::Item("")
{
    this->append(Key::AKIS::uuid,QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString());
    this->append(Key::AKIS::epcohTime,QDateTime::currentMSecsSinceEpoch());
    this->append(Key::AKIS::julianDay,QDate::currentDate().toJulianDay());
    this->append(Key::AKIS::month,QDate::currentDate().month());
    this->append(Key::AKIS::year,QDate::currentDate().year());


    switch (type) {
    case Type::ACIKLAMA:
        this->append(Key::AKIS::type,static_cast<std::int32_t>(BaseItem::Type::ACIKLAMA));
        break;
    case Type::MALZEME:
        this->append(Key::AKIS::type,static_cast<std::int32_t>(BaseItem::Type::MALZEME));
        break;
    case Type::RESIM:
        this->append(Key::AKIS::type,static_cast<std::int32_t>(BaseItem::Type::RESIM));
        break;
    case Type::TEKLIF:
        this->append(Key::AKIS::type,static_cast<std::int32_t>(BaseItem::Type::TEKLIF));
        break;
    case Type::TAMAMLANDI:
        this->append(Key::AKIS::type,static_cast<std::int32_t>(BaseItem::Type::TAMAMLANDI));
        break;
    case Type::ONAYLANDI:
        this->append(Key::AKIS::type,static_cast<std::int32_t>(BaseItem::Type::ONAYLANDI));
        break;
    default:
        break;
    }

    this->setUser(_mUser);

}

BaseItem::BaseItem(const BaseItem &other)
    :SerikBLDCore::Item("")
{
    this->setDocumentView(other.view());
    this->setUser(other.mUser);
    this->setTaskItemOid(other.mTaskItemOid);
}

BaseItem::BaseItem(BaseItem &&other)
    :SerikBLDCore::Item("")
{
    this->setDocumentView(other.view());
    this->setUser(other.mUser);
    this->setTaskItemOid(other.mTaskItemOid);
}


BaseItem::Type BaseItem::getType() const
{
    auto val = this->element(Key::AKIS::type);
    if( val ){
        return static_cast<Type>(val.value().view().get_int32().value);
    }
    return Type::UNKNOWN;
}

BaseItem &BaseItem::setPersonel(const std::string &personelOid, const std::string &personelName)
{
    this->append(Key::AKIS::personelOid,bsoncxx::oid{personelOid});
    this->append(Key::AKIS::personelName,personelName);
    return *this;
}

BaseItem &BaseItem::setAciklama(const std::string &aciklamaText)
{
    this->append(Key::AKIS::aciklama,aciklamaText);
    return *this;
}

std::string BaseItem::uuidString() const
{
    auto val = this->element(Key::AKIS::uuid);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::string BaseItem::getAciklama() const
{
    auto val = this->element(Key::AKIS::aciklama);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::string BaseItem::getPersoneName() const
{
    auto val = this->element(Key::AKIS::personelName);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::string BaseItem::getDateString() const
{
    auto val = this->element(Key::AKIS::julianDay);
    if( val ){
        return QDate::fromJulianDay(val.value().view().get_int64().value).toString("dd/MM/yyyy").toStdString();
    }
    return "0000";
}

std::string BaseItem::getTimeString() const
{
    auto val = this->element(Key::AKIS::epcohTime);
    if( val ){
        return QDateTime::fromMSecsSinceEpoch(val.value().view().get_int64().value).time().toString("hh:mm").toStdString();
    }
    return "0000";
}



std::unique_ptr<WContainerWidget> BaseItem::createBtn(const std::string &btnName, const std::string &backColor )
{

    auto container = cpp14::make_unique<WContainerWidget>();
    container->addStyleClass (/*Bootstrap::Grid::Large::col_lg_2+
                             Bootstrap::Grid::Medium::col_md_2+
                             Bootstrap::Grid::Small::col_sm_3+
                             Bootstrap::Grid::ExtraSmall::col_xs_3+*/
                             Bootstrap::ImageShape::img_thumbnail);
    container->setAttributeValue (Style::style,backColor);
    auto btnText = container->addWidget (cpp14::make_unique<WText>(btnName));
    btnText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
    container->decorationStyle ().setCursor (Cursor::PointingHand);
    container->addStyleClass (CSSStyle::Button::blueButton);
    container->setHeight (25);
    container->setPadding(5,Side::Left|Side::Right);
    return container;

}


void BaseItem::setUser(SerikBLDCore::User *newUser)
{
    mUser = newUser;
}

SerikBLDCore::User *BaseItem::user() const
{
    return mUser;
}

void BaseItem::setTaskItemOid(const std::string &newTaskItemOid)
{
    mTaskItemOid = newTaskItemOid;
}


} // namespace TodoList

