
#include "subitem.h"
#include "stok/stokv2manager.h"
#include "taskitem.h"

namespace TodoList {

SubItem::SubItem(const Type &type)
    :SerikBLDCore::Item("")
{
    this->append(Key::AKIS::uuid,QUuid::createUuid().toString(QUuid::WithoutBraces).toStdString());
    this->append(Key::AKIS::epcohTime,QDateTime::currentMSecsSinceEpoch());
    this->append(Key::AKIS::julianDay,QDate::currentDate().toJulianDay());
    this->append(Key::AKIS::month,QDate::currentDate().month());
    this->append(Key::AKIS::year,QDate::currentDate().year());
    this->append(Key::AKIS::mudurOnay,bsoncxx::types::b_int32{static_cast<std::int32_t>(Onay::Beklemede)});
    this->append(Key::AKIS::baskanYrdOnay,bsoncxx::types::b_int32{static_cast<std::int32_t>(Onay::Beklemede)});

    switch (type) {
    case Type::ACIKLAMA:
        this->append(Key::AKIS::type,static_cast<std::int32_t>(SubItem::Type::ACIKLAMA));
        break;
    case Type::MALZEME:
        this->append(Key::AKIS::type,static_cast<std::int32_t>(SubItem::Type::MALZEME));
        break;
    case Type::RESIM:
        this->append(Key::AKIS::type,static_cast<std::int32_t>(SubItem::Type::RESIM));
        break;
    default:
        break;
    }

    this->initWidget();
}

SubItem::SubItem(const SubItem &other)
    :SerikBLDCore::Item("")
{
    this->setUser(other.user());
    this->setDocumentView(other.view());
    this->initWidget();
}

SubItem::SubItem(SubItem &&other)
    :SerikBLDCore::Item("")
{
    this->setUser(other.user());
    this->setDocumentView(other.view());
    this->initWidget();
}

SubItem &SubItem::setPersonel(const std::string &personelOid, const std::string &personelName)
{
    this->append(Key::AKIS::personelOid,bsoncxx::oid{personelOid});
    this->append(Key::AKIS::personelName,personelName);
    return *this;
}

SubItem &SubItem::setAciklama(const std::string &aciklamaText)
{
    this->append(Key::AKIS::aciklama,aciklamaText);
    return *this;
}

SubItem &SubItem::setResimOid(const std::string &resimOid)
{
    this->append(Key::AKIS::resim,bsoncxx::oid{resimOid});
    return *this;
}

SubItem &SubItem::setMudurOnay(const Onay &onay)
{
    this->append(Key::AKIS::mudurOnay,bsoncxx::types::b_int32{static_cast<std::int32_t>(onay)});
    return *this;
}

SubItem &SubItem::setBaskanYrdOnay(const Onay &onay)
{
    this->append(Key::AKIS::baskanYrdOnay,bsoncxx::types::b_int32{static_cast<std::int32_t>(onay)});
    return *this;
}

SubItem &SubItem::addMalzeme(const std::string &malzemeAdi, const double &miktar, const std::string &metric)
{
    MalzemeItem item(malzemeAdi,miktar,metric);
    this->pushArray(Key::AKIS::malzemeList,item.view());
    return *this;
}

std::vector<MalzemeItem> SubItem::getMalzemeList() const
{
    std::vector<MalzemeItem> list;
    auto val = this->element(Key::AKIS::malzemeList);
    if( val ){
        auto arr = val.value().view().get_array().value;
        for( const auto &itemview : arr ){
            MalzemeItem item(itemview.get_document().view());
            list.push_back(item);
        }
    }
    return list;
}

SubItem::Type SubItem::getType() const
{

    auto val = this->element(Key::AKIS::type);
    if( val ){
        return static_cast<Type>(val.value().view().get_int32().value);
    }
    return Type::UNKNOWN;


}

std::string SubItem::uuidString() const
{
    auto val = this->element(Key::AKIS::uuid);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::string SubItem::getAciklama() const
{
    auto val = this->element(Key::AKIS::aciklama);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::string SubItem::getPersoneName() const
{
    auto val = this->element(Key::AKIS::personelName);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::string SubItem::getDateString() const
{
    auto val = this->element(Key::AKIS::julianDay);
    if( val ){
        return QDate::fromJulianDay(val.value().view().get_int64().value).toString("dd/MM/yyyy").toStdString();
    }
    return "0000";
}

std::string SubItem::getTimeString() const
{
    auto val = this->element(Key::AKIS::epcohTime);
    if( val ){
        return QDateTime::fromMSecsSinceEpoch(val.value().view().get_int64().value).time().toString("hh:mm").toStdString();
    }
    return "0000";
}

std::string SubItem::getMudurOnayString() const
{
    switch (getMudurOnay()) {
    case Onay::Beklemede:
        return "Beklemede";
        break;
    case Onay::Onayli:
        return "Onaylı";
        break;
    case Onay::Red:
        return "Red";
        break;
    case Onay::YetersizAciklama:
        return "YetersizAçıklama";
        break;
    default:
        return "unKnownType";
        break;
    }
}

SubItem::Onay SubItem::getMudurOnay() const
{
    auto val = this->element(Key::AKIS::mudurOnay);
    if( val ){
        Onay _onay = static_cast<Onay>(val.value().view().get_int32().value);
        return _onay;
    }
    return Onay::Bilinmeyen;
}

std::string SubItem::getBaskanYrdOnayString() const
{
    switch (getBaskanYrdOnay()) {
    case Onay::Beklemede:
        return "Beklemede";
        break;
    case Onay::Onayli:
        return "Onaylı";
            break;
    case Onay::Red:
        return "Red";
        break;
    case Onay::YetersizAciklama:
        return "YetersizAçıklama";
        break;
    default:
        return "unKnownType";
        break;
    }
}

SubItem::Onay SubItem::getBaskanYrdOnay() const
{
    auto val = this->element(Key::AKIS::baskanYrdOnay);
    if( val ){
        Onay _onay = static_cast<Onay>(val.value().view().get_int32().value);
        return _onay;
    }
    return Onay::Bilinmeyen;
}

std::string SubItem::getResimOid() const
{
    auto val = element(Key::AKIS::resim);
    if( val ){
        return val.value().view().get_oid().value.to_string();
    }
    return "";
}

void SubItem::editContent()
{


    QList<MalzemeItem>* mList = new QList<MalzemeItem>;

    for( const auto &item : this->getMalzemeList() ){
        mList->push_back(item);
    }

    auto mDialog = createFlatDialog("Malzeme Ata",false);


    auto mMalzemeListContainer = mDialog->Content()->addWidget(cpp14::make_unique<WContainerWidget>());
    mMalzemeListContainer->addStyleClass(Bootstrap::Grid::col_full_12);




    auto malzemeAddContainer = mDialog->Content()->addNew<WContainerWidget>();
    malzemeAddContainer->addStyleClass(Bootstrap::Grid::col_full_12);

    auto hMalzemeLayout = malzemeAddContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

    auto MalzemeComboBox = hMalzemeLayout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Left);
    MalzemeComboBox->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
    std::shared_ptr<WStandardItemModel> mModel = std::make_shared<WStandardItemModel>();
    MalzemeComboBox->setModel(mModel);

    if( mUser ){
        SerikBLDCore::Stokv2::Stokv2Manager* mManager = new SerikBLDCore::Stokv2::Stokv2Manager(this->mUser->getDB());
        SerikBLDCore::Stokv2::Kategori filter;
        filter.setBirim(this->mUser->Birimi());
        auto list = mManager->SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Kategori>::List(filter);

        for( const auto &item : list ){
            std::unique_ptr<WStandardItem> newItem = std::make_unique<WStandardItem>(item.getKategoriAdi());
            newItem->setData(item.getMetric(),ItemDataRole::User+1);
            mModel->insertRow(0,std::move(newItem));
        }
    }



    auto MalzemeDoubleSpinBox = hMalzemeLayout->addWidget(cpp14::make_unique<WDoubleSpinBox>(),0,AlignmentFlag::Center);


    auto MalzemeAddBtn = hMalzemeLayout->addWidget(cpp14::make_unique<WPushButton>("Ekle+ "),0,AlignmentFlag::Justify);




    MalzemeAddBtn->clicked().connect([=](){
        if( MalzemeDoubleSpinBox->value() <= 0 ){
            this->showPopUpMessage("Lütfen Geçerli Miktar Giriniz","warn");
            return;
        }
        mList->push_back(MalzemeItem(MalzemeComboBox->currentText().toUTF8(),MalzemeDoubleSpinBox->value(),linb::any_cast<std::string>(mModel->item(MalzemeComboBox->currentIndex())->data(ItemDataRole::User+1))));
        reListMalzeme(mMalzemeListContainer,mList);
    });
    reListMalzeme(mMalzemeListContainer,mList);



    auto aciklamaTextBox = mDialog->Content()->addWidget(cpp14::make_unique<WTextArea>());
    aciklamaTextBox->addStyleClass(Bootstrap::Grid::col_full_12);
    aciklamaTextBox->setHeight(150);
    aciklamaTextBox->setPlaceholderText("Malzemelerin nerede ne için kullanılacağı içeren bilgi giriniz!");
    aciklamaTextBox->setText(this->getAciklama());

    mDialog->Accepted().connect([=](){

        SubItem subItem(SubItem::Type::MALZEME);
        subItem.setAciklama(aciklamaTextBox->text().toUTF8());
        subItem.setPersonel(this->mUser->oid().value().to_string(),this->mUser->AdSoyad());
        for( const auto &malzemeItem : *mList ){
            subItem.addMalzeme(malzemeItem.getMalzemeAdi(),malzemeItem.getMiktar(),malzemeItem.getMetric());
        }



        TaskItem filter;
        filter.setOid(this->mTaskItemOid);

        SerikBLDCore::Item elematch("");
        elematch.append("$elemMatch",make_document(kvp(Key::AKIS::uuid,this->uuidString())));
        filter.append(Key::akis,elematch);

        TaskItem setObj;

        setObj.append("$set",make_document(kvp(Key::akis+".$."+Key::AKIS::aciklama,aciklamaTextBox->text().toUTF8()),
                                            kvp(Key::akis+".$."+Key::AKIS::malzemeList,subItem.element(Key::AKIS::malzemeList).value().view().get_array().value)));



        auto upt = this->mUser->getDB()->db()->collection(filter.getCollection()).update_one(filter.view(),setObj.view());
        if( upt ){
            if( upt->modified_count() ){
                this->showPopUpMessage("Güncellendi: " + std::to_string(upt.value().modified_count()));
                    _mReloadClicked.emit(NoClass());
                delete mList;
                this->removeDialog(mDialog);
            }else{
                this->showPopUpMessage("Güncellendi: " + std::to_string(upt.value().modified_count()),"warn");
            }


        }else{
            this->showPopUpMessage("Hata: Task Güncellenemedi");
        }

    });


    mDialog->Rejected().connect([=](){
        delete mList;
        this->removeDialog(mDialog);
    });


    mDialog->show();
}

void SubItem::reListMalzeme(WContainerWidget *mMalzemeListContainer, QList<MalzemeItem> *mList)
{

    mMalzemeListContainer->clear();
    std::string attribute1 = Style::background::color::color(Style::color::White::White)+Style::color::color(Style::color::Grey::Black);
    std::string attribute2 = Style::background::color::color(Style::color::Grey::DimGray)+Style::color::color(Style::color::White::White);

    int i = 0 ;
    for( const auto &item : *mList ){
        if( i %2 == 0 ){
            auto __malzemeAdiText = mMalzemeListContainer->addWidget(cpp14::make_unique<WText>(item.getMalzemeAdi()));
            __malzemeAdiText->setAttributeValue(Style::style,attribute1);
            __malzemeAdiText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);

            auto __malzemeMiktarText = mMalzemeListContainer->addWidget(cpp14::make_unique<WText>(doubleToString(item.getMiktar())));
            __malzemeMiktarText->setAttributeValue(Style::style,attribute1);
            __malzemeMiktarText->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

            auto __malzemeMectric = mMalzemeListContainer->addWidget(cpp14::make_unique<WText>(item.getMetric()));
            __malzemeMectric->setAttributeValue(Style::style,attribute1);
            __malzemeMectric->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

            auto __silText = mMalzemeListContainer->addWidget(cpp14::make_unique<WText>("SİL"));
                             __silText->setAttributeValue(Style::style,attribute1);
            __silText->setAttributeValue(Style::customData,std::to_string(i));
            __silText->decorationStyle().setCursor(Cursor::PointingHand);
            __silText->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
            __silText->clicked().connect([=](){
                mList->removeAt(QString::fromStdString(__silText->attributeValue(Style::customData).toUTF8()).toInt());
                reListMalzeme(mMalzemeListContainer,mList);
            });

        }else{
            auto __malzemeAdiText = mMalzemeListContainer->addWidget(cpp14::make_unique<WText>(item.getMalzemeAdi()));
            __malzemeAdiText->setAttributeValue(Style::style,attribute2);
            __malzemeAdiText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);

            auto __malzemeMiktarText = mMalzemeListContainer->addWidget(cpp14::make_unique<WText>(doubleToString(item.getMiktar())));
            __malzemeMiktarText->setAttributeValue(Style::style,attribute2);
            __malzemeMiktarText->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

            auto __malzemeMectric = mMalzemeListContainer->addWidget(cpp14::make_unique<WText>(item.getMetric()));
            __malzemeMectric->setAttributeValue(Style::style,attribute2);
            __malzemeMectric->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

            auto __silText = mMalzemeListContainer->addWidget(cpp14::make_unique<WText>("SİL"));
                             __silText->setAttributeValue(Style::style,attribute2);
            __silText->setAttributeValue(Style::customData,std::to_string(i));
            __silText->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_2+Bootstrap::Grid::ExtraSmall::col_xs_2);
            __silText->decorationStyle().setCursor(Cursor::PointingHand);
            __silText->clicked().connect([=](){
                mList->removeAt(QString::fromStdString(__silText->attributeValue(Style::customData).toUTF8()).toInt());
                reListMalzeme(mMalzemeListContainer,mList);
            });
        }
        i++;
    }


}

void SubItem::initWidget()
{
    this->addStyleClass(CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    this->setMargin(25,Side::Bottom|Side::Top);

    switch (getType()) {
    case Type::ACIKLAMA:

    break;

    case Type::MALZEME:
        this->initMalzemeList();
    break;

    case Type::RESIM:

    break;

    case Type::UNKNOWN:

        break;
default:
    break;
}
}

void SubItem::initMalzemeList()
{

    this->Content()->clear();
    this->Header()->clear();
    this->Footer()->clear();
    auto mList = this->getMalzemeList();


    auto personelContainer = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
    personelContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    auto pLayout = personelContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    pLayout->addStretch(1);
    auto personelName = pLayout->addWidget(cpp14::make_unique<WText>(this->getPersoneName()));
    personelName->addStyleClass(Bootstrap::Label::Primary);
    auto saatText = pLayout->addWidget(cpp14::make_unique<WText>(this->getTimeString()));
    saatText->addStyleClass(Bootstrap::Label::info);
    auto tarihText = pLayout->addWidget(cpp14::make_unique<WText>(this->getDateString()));
    tarihText->addStyleClass(Bootstrap::Label::info);

    auto mudurOnayText = pLayout->addWidget(cpp14::make_unique<WText>("Müdür ONAY:"+this->getMudurOnayString()));
    if( this->getMudurOnay() == Onay::Onayli ){
        mudurOnayText->addStyleClass(Bootstrap::Label::Success);
    }else if( this->getMudurOnay() == Onay::Beklemede ){
        mudurOnayText->addStyleClass(Bootstrap::Label::Warning);
    }else if( this->getMudurOnay() == Onay::Red ){
        mudurOnayText->addStyleClass(Bootstrap::Label::Danger);
    }else if( this->getMudurOnay() == Onay::YetersizAciklama ){
        mudurOnayText->addStyleClass(Bootstrap::Label::Primary);
    }else{
        mudurOnayText->addStyleClass(Bootstrap::Label::info);
    }

    auto baskanYrdOnayText = pLayout->addWidget(cpp14::make_unique<WText>("Başkan Yardımcısı ONAY:"+this->getBaskanYrdOnayString()));
        if( this->getBaskanYrdOnay() == Onay::Onayli ){
            baskanYrdOnayText->addStyleClass(Bootstrap::Label::Success);
        }else if( this->getBaskanYrdOnay() == Onay::Beklemede ){
            baskanYrdOnayText->addStyleClass(Bootstrap::Label::Warning);
        }else if( this->getBaskanYrdOnay() == Onay::Red ){
            baskanYrdOnayText->addStyleClass(Bootstrap::Label::Danger);
        }else if( this->getBaskanYrdOnay() == Onay::YetersizAciklama ){
            baskanYrdOnayText->addStyleClass(Bootstrap::Label::Primary);
        }else{
            baskanYrdOnayText->addStyleClass(Bootstrap::Label::info);
        }

    auto baslikContainer = this->Content()->addNew<WContainerWidget>();
    baslikContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    baslikContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::GhostWhite));
    baslikContainer->addWidget(cpp14::make_unique<WText>("<h4>Malzeme Talep Listesi</h4>"));

    std::string attribute1 = Style::background::color::color(Style::color::White::White)+Style::color::color(Style::color::Grey::Black);
    std::string attribute2 = Style::background::color::color(Style::color::Grey::DimGray)+Style::color::color(Style::color::White::White);

    int i = 0;
    for( const auto &item : mList ){
        if( i %2 == 0 ){
                auto __malzemeAdiText = this->Content()->addWidget(cpp14::make_unique<WText>(item.getMalzemeAdi()));
                __malzemeAdiText->setAttributeValue(Style::style,attribute1);
                __malzemeAdiText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);

                auto __malzemeMiktarText = this->Content()->addWidget(cpp14::make_unique<WText>(doubleToString(item.getMiktar())));
                __malzemeMiktarText->setAttributeValue(Style::style,attribute1);
                __malzemeMiktarText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

                auto __malzemeMectric = this->Content()->addWidget(cpp14::make_unique<WText>(item.getMetric()));
                __malzemeMectric->setAttributeValue(Style::style,attribute1);
                __malzemeMectric->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        }else{
                auto __malzemeAdiText = this->Content()->addWidget(cpp14::make_unique<WText>(item.getMalzemeAdi()));
                __malzemeAdiText->setAttributeValue(Style::style,attribute2);
                __malzemeAdiText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);

                auto __malzemeMiktarText = this->Content()->addWidget(cpp14::make_unique<WText>(doubleToString(item.getMiktar())));
                __malzemeMiktarText->setAttributeValue(Style::style,attribute2);
                __malzemeMiktarText->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

                auto __malzemeMectric = this->Content()->addWidget(cpp14::make_unique<WText>(item.getMetric()));
                __malzemeMectric->setAttributeValue(Style::style,attribute2);
                __malzemeMectric->addStyleClass(Bootstrap::Grid::Large::col_lg_4+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        }
        i++;
    }

    auto containerAciklama = this->Content()->addWidget(cpp14::make_unique<WContainerWidget>());
    containerAciklama->addStyleClass(Bootstrap::Grid::col_full_12);
    containerAciklama->setMargin(15,Side::Top);
    containerAciklama->addStyleClass(CSSStyle::Shadows::shadow8px);
    containerAciklama->setContentAlignment(AlignmentFlag::Left);
    containerAciklama->setAttributeValue(Style::style,Style::color::color(Style::color::Grey::Black));
    containerAciklama->addWidget(cpp14::make_unique<WText>(this->getAciklama()));

    this->Footer()->setPositionScheme(PositionScheme::Relative);

    auto btnContainer = this->Footer()->addNew<WContainerWidget>();
    btnContainer->addStyleClass(Bootstrap::Grid::col_full_12);


    auto hLayout = btnContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

    auto redBtn = createBtn("Red",Style::background::color::rgb (this->getRandom (175,255),
                                                                 this->getRandom (0,50),
                                                                 this->getRandom ()));
    redBtn->clicked().connect([=](){
        if( mUser ){
            if( this->getMudurOnay() != Onay::Red ){
                if( mUser->Statu() == SerikBLDCore::IK::Statu::Mudur ){
                    this->setMudurOnay(Onay::Red);
                    this->initMalzemeList();
                    _mMudurOnayClicked.emit(Onay::Red);
                }
            }else{
                this->showPopUpMessage("Zaten Red Olarak İşaretli");
            }

            if( this->getBaskanYrdOnay() != Onay::Red ){
                if( mUser->Statu() == SerikBLDCore::IK::Statu::BaskanYardimcisi ){
                    this->setBaskanYrdOnay(Onay::Red);
                    this->initWidget();
                    _mBaskanYrdOnayClicked.emit(Onay::Red);
                }
            }else{
                this->showPopUpMessage("Zaten Red Olarak İşaretli");
            }

        }
    });
    hLayout->addWidget(std::move(redBtn),1);


    auto acceptBtn = createBtn("Kabul Et",Style::background::color::rgb (this->getRandom (0,50),
                                                                         this->getRandom (175,255),
                                                                         this->getRandom ()));
    acceptBtn->clicked().connect([=](){

        if( mUser ){
            if( this->getMudurOnay() != Onay::Onayli ){
                if( mUser->Statu() == SerikBLDCore::IK::Statu::Mudur ){
                    this->setMudurOnay(Onay::Onayli);
                    this->initMalzemeList();
                    _mMudurOnayClicked.emit(Onay::Onayli);
                }
            }else{
                this->showPopUpMessage("Zaten Onaylı Olarak İşaretli");
            }

            if( this->getBaskanYrdOnay() != Onay::Onayli ){
                if( mUser->Statu() == SerikBLDCore::IK::Statu::BaskanYardimcisi ){
                    this->setBaskanYrdOnay(Onay::Onayli);
                    this->initWidget();
                    _mBaskanYrdOnayClicked.emit(Onay::Onayli);
                }
            }else{
                this->showPopUpMessage("Zaten Onaylı Olarak İşaretli");
            }

        }
    });
    hLayout->addWidget(std::move(acceptBtn),1);

    auto requiredExplainBtn = createBtn("Yetersiz Açıklama",Style::background::color::rgb (this->getRandom (0,50),
                                                                         this->getRandom (100,155),
                                                                         this->getRandom (175,255)));
    requiredExplainBtn->clicked().connect([=](){
        if( mUser ){
            if( this->getMudurOnay() != Onay::YetersizAciklama ){
                if( mUser->Statu() == SerikBLDCore::IK::Statu::Mudur ){
                    this->setMudurOnay(Onay::YetersizAciklama);
                    this->initMalzemeList();
                    _mMudurOnayClicked.emit(Onay::YetersizAciklama);
                }
            }else{
                this->showPopUpMessage("Zaten Yetersiz Açıklama Olarak İşaretli");
            }


            if( this->getBaskanYrdOnay() != Onay::YetersizAciklama ){
                if( mUser->Statu() == SerikBLDCore::IK::Statu::BaskanYardimcisi ){
                    this->setBaskanYrdOnay(Onay::YetersizAciklama);
                    this->initWidget();
                    _mBaskanYrdOnayClicked.emit(Onay::YetersizAciklama);
                }
            }else{
                this->showPopUpMessage("Zaten Yetersiz Açıklama Olarak İşaretli");
            }

        }
    });
    hLayout->addWidget(std::move(requiredExplainBtn),1);

    //TODO: Onaylandıktan sonra PDF Olarak Yazdır
    auto yazdirBtn = createBtn("Yazdır",Style::background::color::rgb (this->getRandom (100,155),
                                                                       this->getRandom (100,155),
                                                                       this->getRandom (100,155)));
    hLayout->addWidget(std::move(yazdirBtn),1);

    //TODO: Düzenle
    auto duzenleBtn = createBtn("Düzenle",Style::background::color::rgb (this->getRandom (100,155),
                                                                        this->getRandom (100,155),
                                                                        this->getRandom (100,155)));
    duzenleBtn->clicked().connect(this,&SubItem::editContent);
    hLayout->addWidget(std::move(duzenleBtn),1);

    //TODO: Sil
    auto silBtn = createBtn("Sil",Style::background::color::rgb (this->getRandom (200,255),
                                                                         this->getRandom (50,100),
                                                                         this->getRandom (50,100)));
    hLayout->addWidget(std::move(silBtn),1);

}

std::unique_ptr<WContainerWidget> SubItem::createBtn(const std::string &btnName, const std::string &backColor )
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

void SubItem::setTaskItemOid(const std::string &newTaskItemOid)
{
    mTaskItemOid = newTaskItemOid;
}

SerikBLDCore::User *SubItem::user() const
{
    return mUser;
}

Signal<SubItem::Onay> &SubItem::mudurOnayClicked()
{
    return _mMudurOnayClicked;
}

Signal<SubItem::Onay> &SubItem::baskanYrdOnayClicked()
{
    return _mBaskanYrdOnayClicked;
}

Signal<NoClass> &SubItem::reloadClicked()
{
    return _mReloadClicked;
}

void SubItem::setUser(SerikBLDCore::User *newUser)
{
    mUser = newUser;
}

std::string MalzemeItem::getMalzemeAdi() const
{
    auto val = this->element(Key::AKIS::MALZEMELIST::malzemeAdi);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";

}

double MalzemeItem::getMiktar() const
{
    auto val = this->element(Key::AKIS::MALZEMELIST::miktari);
    if( val ){
        return val.value().view().get_double().value;
    }
    return 0;
}

std::string MalzemeItem::getMetric() const
{
    auto val = this->element(Key::AKIS::MALZEMELIST::metric);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}


} // namespace TodoList

