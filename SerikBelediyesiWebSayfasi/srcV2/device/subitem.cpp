
#include "subitem.h"
#include "stok/stokv2manager.h"
#include "taskitem.h"

namespace TodoList {

MalzemeItem::MalzemeItem(SerikBLDCore::User *_mUser)
    :BaseItem::BaseItem(Type::MALZEME,_mUser)
{
    this->append(Key::AKIS::mudurOnay,bsoncxx::types::b_int32{static_cast<std::int32_t>(Onay::Beklemede)});
    this->append(Key::AKIS::baskanYrdOnay,bsoncxx::types::b_int32{static_cast<std::int32_t>(Onay::Beklemede)});
}

MalzemeItem::MalzemeItem(const MalzemeItem &other)
    :BaseItem::BaseItem(other)
{

}

MalzemeItem::MalzemeItem(MalzemeItem &&other)
    :BaseItem::BaseItem(other)
{

}

MalzemeItem::MalzemeItem(const BaseItem &other)
    :BaseItem::BaseItem(other)
{

}





MalzemeItem &MalzemeItem::setMudurOnay(const Onay &onay)
{
    this->append(Key::AKIS::mudurOnay,bsoncxx::types::b_int32{static_cast<std::int32_t>(onay)});
    return *this;
}

MalzemeItem &MalzemeItem::setBaskanYrdOnay(const Onay &onay)
{
    this->append(Key::AKIS::baskanYrdOnay,bsoncxx::types::b_int32{static_cast<std::int32_t>(onay)});
    return *this;
}

MalzemeItem &MalzemeItem::addMalzeme(const std::string &malzemeAdi, const double &miktar, const std::string &metric)
{
    MalzemeListItem item(malzemeAdi,miktar,metric);
    this->pushArray(Key::AKIS::malzemeList,item.view());
    return *this;
}

std::vector<MalzemeListItem> MalzemeItem::getMalzemeList() const
{
    std::vector<MalzemeListItem> list;
    auto val = this->element(Key::AKIS::malzemeList);
    if( val ){
        auto arr = val.value().view().get_array().value;
        for( const auto &itemview : arr ){
            MalzemeListItem item(itemview.get_document().view());
            list.push_back(item);
        }
    }
    return list;
}




std::string MalzemeItem::getMudurOnayString() const
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

MalzemeItem::Onay MalzemeItem::getMudurOnay() const
{
    auto val = this->element(Key::AKIS::mudurOnay);
    if( val ){
        Onay _onay = static_cast<Onay>(val.value().view().get_int32().value);
        return _onay;
    }
    return Onay::Bilinmeyen;
}

std::string MalzemeItem::getBaskanYrdOnayString() const
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

MalzemeItem::Onay MalzemeItem::getBaskanYrdOnay() const
{
    auto val = this->element(Key::AKIS::baskanYrdOnay);
    if( val ){
        Onay _onay = static_cast<Onay>(val.value().view().get_int32().value);
        return _onay;
    }
    return Onay::Bilinmeyen;
}


void MalzemeItem::editContent()
{

    QList<MalzemeListItem>* mList = new QList<MalzemeListItem>;

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
        mList->push_back(MalzemeListItem(MalzemeComboBox->currentText().toUTF8(),MalzemeDoubleSpinBox->value(),linb::any_cast<std::string>(mModel->item(MalzemeComboBox->currentIndex())->data(ItemDataRole::User+1))));
        reListMalzeme(mMalzemeListContainer,mList);
    });
    reListMalzeme(mMalzemeListContainer,mList);

    auto aciklamaTextBox = mDialog->Content()->addWidget(cpp14::make_unique<WTextArea>());
    aciklamaTextBox->addStyleClass(Bootstrap::Grid::col_full_12);
    aciklamaTextBox->setHeight(150);
    aciklamaTextBox->setPlaceholderText("Malzemelerin nerede ne için kullanılacağı içeren bilgi giriniz!");
    aciklamaTextBox->setText(this->getAciklama());

    mDialog->Accepted().connect([=](){

        MalzemeItem subItem;
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

void MalzemeItem::reListMalzeme(WContainerWidget *mMalzemeListContainer, QList<MalzemeListItem> *mList)
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

void MalzemeItem::initWidget()
{
    this->addStyleClass(CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    this->setMargin(25,Side::Bottom|Side::Top);
    this->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::GhostWhite));
    this->initMalzemeList();
}

void MalzemeItem::initMalzemeList()
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
        this->setOnay(Onay::Red);
    });
    hLayout->addWidget(std::move(redBtn),1);

    auto acceptBtn = createBtn("Kabul Et",Style::background::color::rgb (this->getRandom (0,50),
                                                                         this->getRandom (175,255),
                                                                         this->getRandom ()));
    acceptBtn->clicked().connect([=](){
        this->setOnay(Onay::Onayli);
    });
    hLayout->addWidget(std::move(acceptBtn),1);

    auto requiredExplainBtn = createBtn("Yetersiz Açıklama",Style::background::color::rgb (this->getRandom (0,50),
                                                                         this->getRandom (100,155),
                                                                         this->getRandom (175,255)));
    requiredExplainBtn->clicked().connect([=](){
        this->setOnay(Onay::YetersizAciklama);
    });
    hLayout->addWidget(std::move(requiredExplainBtn),1);

    //TODO: Onaylandıktan sonra PDF Olarak Yazdır
    auto yazdirBtn = createBtn("Yazdır",Style::background::color::rgb (this->getRandom (100,155),
                                                                       this->getRandom (100,155),
                                                                       this->getRandom (100,155)));
    hLayout->addWidget(std::move(yazdirBtn),1);

    auto duzenleBtn = createBtn("Düzenle",Style::background::color::rgb (this->getRandom (100,155),
                                                                        this->getRandom (100,155),
                                                                        this->getRandom (100,155)));
    duzenleBtn->clicked().connect(this,&MalzemeItem::editContent);
    hLayout->addWidget(std::move(duzenleBtn),1);

    auto silBtn = createBtn("Sil",Style::background::color::rgb (this->getRandom (200,255),
                                                                         this->getRandom (50,100),
                                                                         this->getRandom (50,100)));
    silBtn->clicked().connect([=](){
        auto okBtn = askConfirm("Silmek İstediğinize Emin misniz?");
        okBtn->clicked().connect([=](){
            TaskItem filter;
            filter.setOid(this->mTaskItemOid);
            TaskItem setObj;
            setObj.append("$pull",make_document(kvp(Key::akis,this->view())));
            auto upt = this->mUser->getDB()->db()->collection(filter.getCollection()).update_one(filter.view(),setObj.view());
            if( upt ){
                if( upt->modified_count() ){
                    this->showPopUpMessage("Güncellendi: " + std::to_string(upt.value().modified_count()));
                        _mReloadClicked.emit(NoClass());
                }else{
                    this->showPopUpMessage("Güncellendi: " + std::to_string(upt.value().modified_count()),"warn");
                }
            }else{
                this->showPopUpMessage("Hata: Task Güncellenemedi");
            }
        });
    });

    hLayout->addWidget(std::move(silBtn),1);

}

void MalzemeItem::setOnay(const Onay &onay)
{


    auto updateTaskItem = [=]( const Onay &onay, const bool mudur = true){
        TaskItem filter;
        filter.setOid(this->mTaskItemOid);
        SerikBLDCore::Item elematch("");
        elematch.append("$elemMatch",make_document(kvp(Key::AKIS::uuid,this->uuidString())));
        filter.append(Key::akis,elematch);
        TaskItem setObj;
        if( mudur ){
            setObj.append("$set",make_document(kvp(Key::akis+".$."+Key::AKIS::mudurOnay,
                                                    bsoncxx::types::b_int32{static_cast<std::int32_t>(onay)})));
        }else{
            setObj.append("$set",make_document(kvp(Key::akis+".$."+Key::AKIS::baskanYrdOnay,
                                                    bsoncxx::types::b_int32{static_cast<std::int32_t>(onay)})));
        }

        auto upt = this->user()->getDB()->db()->collection(filter.getCollection()).update_one(filter.view(),setObj.view());
        if( upt ){
            this->showPopUpMessage("Güncellendi");
                return true;
        }else{
            this->showPopUpMessage("Hata: Task Güncellenemedi");
                    return false;
        }
    };

    auto infoShow = [=](const Onay &onay ){
        switch (onay) {
        case Onay::Onayli:
            this->showPopUpMessage("Zaten Onaylı Olarak İşaretli");
            break;
        case Onay::Red:
            this->showPopUpMessage("Zaten Red Olarak İşaretli");
            break;
        case Onay::YetersizAciklama:
            this->showPopUpMessage("Zaten Yetersiz Açıklama Olarak İşaretli");
            break;
        case Onay::Beklemede:
            this->showPopUpMessage("Zaten Beklemede  Olarak İşaretli");
            break;

        default:

            break;
        }
    };


    if( this->user() ){
        if( mUser->Statu() == SerikBLDCore::IK::Statu::Mudur ){
            if( this->getMudurOnay() != onay ){
                if( updateTaskItem(onay,true) ){
                    this->setMudurOnay(onay);
                    this->initMalzemeList();
                }
            }else{
                infoShow(onay);
            }
        }else if( mUser->Statu() == SerikBLDCore::IK::Statu::BaskanYardimcisi ){
            if( this->getBaskanYrdOnay() != onay ){
                if( updateTaskItem(onay,false) ){
                    this->setBaskanYrdOnay(onay);
                    this->initMalzemeList();
                }
            }else{
                infoShow(onay);
            }
        }
    }
}






Signal<MalzemeItem::Onay> &MalzemeItem::mudurOnayClicked()
{
    return _mMudurOnayClicked;
}

Signal<MalzemeItem::Onay> &MalzemeItem::baskanYrdOnayClicked()
{
    return _mBaskanYrdOnayClicked;
}

Signal<NoClass> &MalzemeItem::reloadClicked()
{
    return _mReloadClicked;
}



std::string MalzemeListItem::getMalzemeAdi() const
{
    auto val = this->element(Key::AKIS::MALZEMELIST::malzemeAdi);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";

}

double MalzemeListItem::getMiktar() const
{
    auto val = this->element(Key::AKIS::MALZEMELIST::miktari);
    if( val ){
        return val.value().view().get_double().value;
    }
    return 0;
}

std::string MalzemeListItem::getMetric() const
{
    auto val = this->element(Key::AKIS::MALZEMELIST::metric);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}


} // namespace TodoList

