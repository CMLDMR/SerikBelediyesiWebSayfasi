#include "taskitem.h"

#include <filesystem>
#include <fstream>
#include <algorithm>

#include <Wt/WApplication.h>
#include <Wt/WLink.h>

#include "personelmanager.h"

namespace TodoList {


TaskItem::TaskItem()
    :SerikBLDCore::Item(Key::Collection.data())
{

}

TaskItem::TaskItem(SerikBLDCore::User *_mUser)
    :SerikBLDCore::Item(Key::Collection.data()),mUser(_mUser)
{

}

TaskItem &TaskItem::setBirim(const std::string &birim)
{
    this->append(Key::birim,birim);
    return *this;
}

TaskItem &TaskItem::setImageItem(const std::string &oid)
{
    this->append(Key::imageOid,bsoncxx::oid{oid});
    return *this;
}

TaskItem &TaskItem::setBaskanYardimcisi(const std::string &baskanYrdOid, const std::string &baskanYrdAdSoyad)
{
    GorevliItem item;
    item.setPersonel(baskanYrdOid,baskanYrdAdSoyad);
    this->append(Key::baskanYardimcisi,item.view());
    return *this;
}

TaskItem &TaskItem::addGorevli(const std::string &gorevliOid, const std::string &adSoyad)
{
    GorevliItem item;
    item.setPersonel(gorevliOid,adSoyad);

    auto list = getGorevliList();

    bool exist = std::any_of(list.begin(),list.end(),[&gorevliOid](const TodoList::GorevliItem &per){
        return per.personelOid() == gorevliOid;
    });

    if( !exist ){
        this->pushArray(Key::gorevli,item.view());
    }
    return *this;
}

TaskItem &TaskItem::addAkis(const SubItem &subItem)
{
    this->pushArray(Key::akis,subItem.view());
    return *this;
}


std::string TaskItem::getDate(const std::string &format) const
{
    auto val = this->element(Key::julianDay);
    if( val ){
        return QDate::fromJulianDay(val.value().view().get_int64().value).toString(format.c_str()).toStdString();
    }
    return "noDate";
}

std::string TaskItem::getTime(const std::string &format) const
{
    auto val = this->element(Key::epochTime);
    if( val ){
        return QDateTime::fromMSecsSinceEpoch(val.value().view().get_int64().value).time().toString(format.c_str()).toStdString();
    }
    return "noTime";
}

std::string TaskItem::getIsAdi() const
{
    auto val = this->element(Key::isAdi);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::string TaskItem::getAciklama() const
{
    auto val = this->element(Key::aciklama);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

std::string TaskItem::getDurum() const
{
        return isTamamlandi() ? "Bitti" : "Devam";
}

std::string TaskItem::getImageOid() const
{
    auto val = this->element(Key::imageOid);
    if( val ){
    return val.value().view().get_oid().value.to_string();
    }
    return "";
}

bool TaskItem::isTamamlandi() const
{
    auto val = this->element(Key::tamamlandi);
    if( val ){
        return val.value().view().get_bool().value;
    }
    return false;
}

std::list<GorevliItem> TaskItem::getGorevliList() const
{
    std::list<GorevliItem> list;
    auto val = this->element(Key::gorevli);
    if( val ){
        auto arr = val.value().view().get_array().value;
        for( const auto &item : arr ){
            GorevliItem item_;
            item_.setDocumentView(item.get_document().view());
            list.push_back(item_);
        }
    }
    return list;
}

bool TaskItem::isGorevli(const std::string &gorevliOid) const
{
    bool exist = false;
    auto list = getGorevliList();
    for( const auto &personel : list ){
        if( gorevliOid == personel.personelOid() ){
            exist = true;
            break;
        }
    }
    return exist;
}

std::list<SubItem> TaskItem::getAkisList() const
{
    std::list<SubItem> list;
    auto val = this->element(Key::akis);
    if( val ){
        auto arr = val.value().view().get_array().value;
        for( const auto &akisItem : arr ){
            SubItem item(SubItem::Type::ACIKLAMA);
            item.setDocumentView(akisItem.get_document().view());
            list.push_back(item);
        }
    }
    return list;
}

GorevliItem TaskItem::getBaskanYardimcisi() const
{
    GorevliItem item_;
    auto val = this->element(Key::baskanYardimcisi);
    if( val ){
        item_.setDocumentView(val.value().view().get_document().value);
    }
    return item_;
}

TaskManager::TaskManager(SerikBLDCore::User *_mUser)
    :SerikBLDCore::ListItem<TaskItem>(_mUser->getDB()),
      mUser(_mUser)
{

//    this->initCSS();
    this->Content()->setMargin(15,Side::Top);
    this->initHeader();
    this->UpdateList(TaskItem(mUser).setBirim(mUser->Birimi()));
}

void TaskManager::onList(const QVector<TaskItem> *mlist)
{

    this->Content()->clear();

    bool initColor = false;
    for( const auto &item : *mlist ){
        auto container = this->Content()->addNew<TaskListItem>(item);
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        if( initColor ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::LightGray)+
                                         Style::color::color(Style::color::White::AliceBlue));
        }else{
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::White)+
                                         Style::color::color(Style::color::Grey::DimGray));
        }
        initColor = !initColor;

        container->clicked().connect([=](){
            this->loadTask(container->getOid());
        });
    }

}

void TaskManager::initCSS()
{





    std::string cssPath = "css/taskmanager-20230330.css";

    if( std::filesystem::exists("docroot/"+cssPath) ){
        std::cout << "\n" << "file Does Exist\n";
    }else{

        std::ofstream out;
        out.open("docroot/"+cssPath,std::ios::out);

        if( out.is_open() ){

            std::string cssContent;
            cssContent += ".taskBtn{"
                          "background-color:\"red\""
                          "}";




            out.write(cssContent.c_str(),cssContent.size());
            out.close();

            wApp->useStyleSheet(WLink(cssPath));
//            Wt::WApplication::useStyleSheet(WLink("css/taskmanager-20230330.css"));

        }

    }





}

void TaskManager::initHeader()
{

    this->Header()->clear();

    auto infoContainer = this->Header()->addNew<WContainerWidget>();
    infoContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_10+
                                 Bootstrap::Grid::Medium::col_md_10+
                                 Bootstrap::Grid::Small::col_sm_9+
                                 Bootstrap::Grid::ExtraSmall::col_xs_8);
    infoContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_info);
    infoContainer->setPadding(5,AllSides);
    infoContainer->addNew<WText>(this->mUser->Birimi());


    auto addContainer = this->Header()->addNew<WContainerWidget>();
    addContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                Bootstrap::Grid::Medium::col_md_2+
                                Bootstrap::Grid::Small::col_sm_3+
                                Bootstrap::Grid::ExtraSmall::col_xs_4);
    addContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);
    addContainer->setPadding(5,AllSides);
    addContainer->addNew<WText>("<b>Yeni Ekle+</b>");
    addContainer->decorationStyle().setCursor(Cursor::PointingHand);
    addContainer->clicked().connect(this,&TaskManager::addNewTask);
}

void TaskManager::addNewTask()
{
    auto mDialog = createFlatDialog("Yeni İş Tanımla");

    auto isAdiLineEdit = mDialog->Content()->addNew<WLineEdit>();
    mDialog->Content()->addNew<WBreak>();

    auto textArea = mDialog->Content()->addNew<WTextArea>();
    mDialog->Content()->addNew<WBreak>();

    auto imageText = mDialog->Content()->addNew<WText>("Varsa Resim Ekleyiniz");
    auto imageContainer = mDialog->Content()->addNew<WContainerWidget>();
    imageContainer->setWidth(WLength("100%"));
    imageContainer->setHeight(250);

    auto uploaderWidget = mDialog->Content()->addNew<FileUploaderWidget>("Resim Yükle");
    uploaderWidget->setType(FileUploaderWidget::Image);

    uploaderWidget->Uploaded().connect([=](){
        imageText->setText(uploaderWidget->fileLocation().toStdString());
        imageContainer->setAttributeValue(Style::style,"height:250px; "+Style::background::url(uploaderWidget->doocRootLocation().toStdString())+
                                                            Style::background::size::contain+
                                                            Style::background::repeat::norepeat+
                                                            Style::background::position::center_center);
        imageContainer->setHeight(250);
    });

    auto isTipiComboBox = mDialog->Content()->addNew<WComboBox>();

    isTipiComboBox->addItem("Kamera Kurulum");
    isTipiComboBox->addItem("Kamera Bakım");
    isTipiComboBox->addItem("Bilgisayar Kurulum");
    isTipiComboBox->addItem("Kamera Bakım");
    isTipiComboBox->addItem("Hoparler Bakım");
    isTipiComboBox->addItem("Hoparker Kurulum");
    isTipiComboBox->addItem("Bilgisayar Bakım");
    isTipiComboBox->addItem("Malzeme Alımı");

    mDialog->Accepted().connect([=](){

        TaskItem taskItem(mUser);

        if( uploaderWidget->isUploaded() ){
            auto uploadedOid = this->uploadfile(uploaderWidget->fileLocation());
            taskItem.setImageItem(uploadedOid.view().get_oid().value.to_string());
        }




        taskItem.append(Key::aciklama,textArea->text().toUTF8());
        taskItem.setBirim(mUser->Birimi());
        taskItem.append(Key::epochTime,QDateTime::currentMSecsSinceEpoch());
        taskItem.append(Key::julianDay,QDate::currentDate().toJulianDay());
        taskItem.append(Key::tamamlandi,false);
        taskItem.append(Key::isAdi,isAdiLineEdit->text().toUTF8());


        auto ins = this->InsertItem(taskItem);
        if( ins.empty() ){
            this->showMessage("Uyarı","İş Kayıt Edilemedi");
        }else{
            TaskItem filterItem(mUser);
            filterItem.append(Key::birim,mUser->Birimi());
            this->UpdateList(filterItem);
            this->removeDialog(mDialog);
        }
    });

    mDialog->show();

}

void TaskManager::loadTask(const std::string &taskoid)
{
    TaskItem filter;
    filter.setOid(taskoid);

    auto val = this->FindOneItem(filter);

    if( !val.view().empty() ){
        this->Content()->clear();
        auto container = this->Content()->addWidget(cpp14::make_unique<TaskItemWidget>(val,this->mUser));
        container->setMargin(25,Side::Top);

        this->Footer()->clear();
        auto hLayout = this->Footer()->setLayout(cpp14::make_unique<WHBoxLayout>());


        auto baskanYardimciAta = createSmallButton("Başkan Yardımcısı Ata+");
        baskanYardimciAta->clicked().connect([=](){
            this->assignBaskanYardimcisi(taskoid);
        });
        hLayout->addWidget(std::move(baskanYardimciAta));


        auto gorevliBtn = createSmallButton("Personel Ata+");
        gorevliBtn->clicked().connect([=](){
            this->assignPersonel(taskoid);
        });
        hLayout->addWidget(std::move(gorevliBtn));

        auto malzemeEkleBtn = createSmallButton("Malzeme Ekle+");
        malzemeEkleBtn->clicked().connect([=](){
            this->assignMalzeme(taskoid);
        });
        hLayout->addWidget(std::move(malzemeEkleBtn));

        auto aciklamaBtn = createSmallButton("Açıklama Ekle+");
        hLayout->addWidget(std::move(aciklamaBtn));

        auto resimEkleBtn = createSmallButton("Resim Ekle+");
        hLayout->addWidget(std::move(resimEkleBtn));

        auto tamamlaBtn = createSmallButton("Tamamla");
        hLayout->addWidget(std::move(tamamlaBtn));

        auto silBtn = createSmallButton("SİL!");
        silBtn->clicked().connect([=](){
            auto obkBtn = askConfirm("Silmek İstediğinize Eminmisiniz?");
            obkBtn->clicked().connect([=](){
                this->deleteTask(taskoid);
            });
        });
        hLayout->addWidget(std::move(silBtn));

        hLayout->addStretch(1);
    }


}

void TaskManager::assignPersonel(const std::string &taskOid)
{

    mPersonelSelectWidget.clear();
    auto mDialog = createFlatDialog("Personel Ata");

    SerikBLDCore::PersonelManager* mPersonelManager = new SerikBLDCore::PersonelManager(this->getDB());

    auto list = mPersonelManager->PersonelList(this->mUser->Birimi(),500);

    mDialog->Content()->setOverflow(Overflow::Scroll);
    auto personelContainer = mDialog->Content()->addWidget(cpp14::make_unique<WContainerWidget>());
    personelContainer->addStyleClass(Bootstrap::Grid::col_full_12);

    auto gLayout = personelContainer->setLayout(cpp14::make_unique<WGridLayout>());

    TaskItem selectedpersonelBefore;
    selectedpersonelBefore.setOid(taskOid);
    selectedpersonelBefore = this->FindOneItem(selectedpersonelBefore);
    auto selectedPerList = selectedpersonelBefore.getGorevliList();


    int i = 0; int j = 0;
    for( const auto &personel : list ){
        auto personelItemContainer = gLayout->addWidget(cpp14::make_unique<PersonelSelectWidget>(personel),i,j,AlignmentFlag::Justify);
        if( selectedpersonelBefore.isGorevli(personel.oid()->to_string() ) ){
            personelItemContainer->setSelect();
            mPersonelSelectWidget.push_back(personelItemContainer);
        }
        personelItemContainer->clicked().connect([=](){
            if( personelItemContainer->selected() ){
                mPersonelSelectWidget.push_back(personelItemContainer);
            }else{
                for( const auto &pItem : mPersonelSelectWidget ){
                    if( personelItemContainer->oid().value().to_string() == pItem->oid().value().to_string() ){
                        mPersonelSelectWidget.remove(pItem);
                        break;
                    }
                }
            }
        });
        j++;
        if( j > 3 ) { i++; j = 0 ; }
    }

    mDialog->Content()->setHeight(400);
    mDialog->show();


    mDialog->Accepted().connect([=](){


        TaskItem taskItem;
        taskItem.setOid(taskOid);

        for( const auto &pItem : mPersonelSelectWidget ){
            taskItem.addGorevli(pItem->oid()->to_string(),pItem->AdSoyad().toStdString());
        }

        auto upt = this->UpdateItem(taskItem);
        if( upt ){
            this->loadTask(taskOid);

            delete mPersonelManager;

            this->removeDialog(mDialog);
        }else{
            this->showPopUpMessage("Personel Eklenemedi","warn");
        }

    });

}

void TaskManager::assignBaskanYardimcisi(const std::string &taskOid)
{


    mPersonelSelectWidget.clear();
    auto mDialog = createFlatDialog("Personel Ata");

    SerikBLDCore::PersonelManager* mPersonelManager = new SerikBLDCore::PersonelManager(this->getDB());

    SerikBLDCore::IK::Personel filter;
    filter.setStatu(SerikBLDCore::IK::Statu::BaskanYardimcisi.c_str());
    auto list = mPersonelManager->List(filter);

    mDialog->Content()->setOverflow(Overflow::Scroll);
    auto personelContainer = mDialog->Content()->addWidget(cpp14::make_unique<WContainerWidget>());
    personelContainer->addStyleClass(Bootstrap::Grid::col_full_12);

    auto gLayout = personelContainer->setLayout(cpp14::make_unique<WGridLayout>());

    TaskItem selectedpersonelBefore;
    selectedpersonelBefore.setOid(taskOid);
    selectedpersonelBefore = this->FindOneItem(selectedpersonelBefore);


    int i = 0; int j = 0;
    for( const auto &personel : list ){
        auto personelItemContainer = gLayout->addWidget(cpp14::make_unique<PersonelSelectWidget>(personel),i,j,AlignmentFlag::Justify);
        if( selectedpersonelBefore.getBaskanYardimcisi().personelOid() == personelItemContainer->oid()->to_string() ){
            personelItemContainer->setSelect();
            mPersonelSelectWidget.push_back(personelItemContainer);
        }
        personelItemContainer->clicked().connect([=](){
            if( personelItemContainer->selected() ){
                mPersonelSelectWidget.push_back(personelItemContainer);
            }else{
                for( const auto &pItem : mPersonelSelectWidget ){
                    if( personelItemContainer->oid().value().to_string() == pItem->oid().value().to_string() ){
                        mPersonelSelectWidget.remove(pItem);
                        break;
                    }
                }
            }
        });
        j++;
        if( j > 3 ) { i++; j = 0 ; }
    }

    mDialog->Content()->setHeight(150);
    mDialog->show();


    mDialog->Accepted().connect([=](){


        TaskItem taskItem;
        taskItem.setOid(taskOid);

        if( mPersonelSelectWidget.size() > 1 ){
            this->showPopUpMessage("Birden Fazla Başkan Yardımcısı Seçemezsiniz","warn");
            return;
        }

        if( !mPersonelSelectWidget.size() ){
            this->showPopUpMessage("Başkan Yardımcısı Seçmediniz","warn");
            return;
        }
        //TODO: Başkan Yardımcısı Set Edilecek
        for( const auto &pItem : mPersonelSelectWidget ){
            taskItem.setBaskanYardimcisi(pItem->oid()->to_string(),pItem->AdSoyad().toStdString());
        }

        auto upt = this->UpdateItem(taskItem);
        if( upt ){
            this->loadTask(taskOid);

            delete mPersonelManager;

            this->removeDialog(mDialog);

            this->showPopUpMessage("Başkan Yardımcısı Eklendi");
        }else{
            this->showPopUpMessage("Personel Eklenemedi","warn");
        }

    });

}

void TaskManager::assignMalzeme(const std::string &taskOid)
{

    QList<MalzemeItem>* mList = new QList<MalzemeItem>;
    auto mDialog = createFlatDialog("Malzeme Ata",false);


    auto mMalzemeListContainer = mDialog->Content()->addWidget(cpp14::make_unique<WContainerWidget>());
    mMalzemeListContainer->addStyleClass(Bootstrap::Grid::col_full_12);




    auto malzemeAddContainer = mDialog->Content()->addNew<WContainerWidget>();
    malzemeAddContainer->addStyleClass(Bootstrap::Grid::col_full_12);

    auto hMalzemeLayout = malzemeAddContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

    auto MalzemeComboBox = hMalzemeLayout->addWidget(cpp14::make_unique<WComboBox>(),0,AlignmentFlag::Left);
    MalzemeComboBox->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
    SerikBLDCore::Stokv2::Stokv2Manager* mManager = new SerikBLDCore::Stokv2::Stokv2Manager(this->getDB());
    SerikBLDCore::Stokv2::Kategori filter;
    filter.setBirim(this->mUser->Birimi());
    auto list = mManager->SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Kategori>::List(filter);


    std::shared_ptr<WStandardItemModel> mModel = std::make_shared<WStandardItemModel>();

    for( const auto &item : list ){
        std::unique_ptr<WStandardItem> newItem = std::make_unique<WStandardItem>(item.getKategoriAdi());
        newItem->setData(item.getMetric(),ItemDataRole::User+1);
        mModel->insertRow(0,std::move(newItem));
    }

    MalzemeComboBox->setModel(mModel);

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



    auto aciklamaTextBox = mDialog->Content()->addWidget(cpp14::make_unique<WTextArea>());
    aciklamaTextBox->addStyleClass(Bootstrap::Grid::col_full_12);
    aciklamaTextBox->setHeight(150);
    aciklamaTextBox->setPlaceholderText("Malzemelerin nerede ne için kullanılacağı içeren bilgi giriniz!");

    mDialog->Accepted().connect([=](){

        SubItem subItem(SubItem::Type::MALZEME);
        subItem.setAciklama(aciklamaTextBox->text().toUTF8());
        subItem.setPersonel(this->mUser->oid().value().to_string(),this->mUser->AdSoyad());
        for( const auto &malzemeItem : *mList ){
            subItem.addMalzeme(malzemeItem.getMalzemeAdi(),malzemeItem.getMiktar(),malzemeItem.getMetric());
        }

        TaskItem taskItem;
        taskItem.setOid(taskOid);

        auto upt = this->pushValue(taskItem,Key::akis,subItem.view());
        if( upt ){
            this->loadTask(taskOid);
            delete mList;
            this->removeDialog(mDialog);
        }else{
            this->showPopUpMessage(this->getLastError().toStdString(),"warn");
        }
    });


    mDialog->Rejected().connect([=](){
        delete mList;
        this->removeDialog(mDialog);
    });


    mDialog->show();
}

void TaskManager::reListMalzeme(WContainerWidget *mMalzemeListContainer, QList<MalzemeItem> *mList)
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

std::unique_ptr<WContainerWidget> TaskManager::createSmallButton(const std::string &name)
{
    auto container = std::make_unique<WContainerWidget>();
    container->addNew<WText>(name);
    container->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(180,200),this->getRandom(180,200),this->getRandom(180,200))+Style::color::color(Style::color::Grey::Black));
    container->addStyleClass(CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    container->decorationStyle().setCursor(Cursor::PointingHand);
    container->setPadding(2,Side::Left|Side::Right);
    return container;
}

void TaskManager::updateTaskList()
{
    TaskItem filterItem;
    filterItem.append(Key::birim,mUser->Birimi());
    this->UpdateList(filterItem);
}

void TaskManager::deleteTask(const std::string &taskOid)
{

    TaskItem filter;

    auto taskItem = this->FindOneItem(filter);

    for( const auto &akis : taskItem.getAkisList() ){
        if( !akis.getResimOid().empty() ){
            this->deleteGridFS(akis.getResimOid().c_str());
        }
    }
    if( !taskItem.getImageOid().empty() ){

    }
    if( deleteGridFS(taskItem.getImageOid().c_str()) ){
        this->deleteGridFS(taskItem.getImageOid().c_str());
        this->DeleteItem(filter);
    }
    this->updateTaskList();
}

TaskListItem::TaskListItem(const TaskItem &item)
{
    setPadding(3,Side::Top|Side::Bottom);
    setMargin(3,Side::Top|Side::Bottom);

    this->mOid = item.oid().value().to_string();
    auto rContainer = this->addNew<WContainerWidget>();
    rContainer->addStyleClass(Bootstrap::Grid::row);

    rContainer->addWidget(cpp14::make_unique<WText>(item.element(Key::isAdi).value().view().get_string().value.data()))
            ->addStyleClass(Bootstrap::Grid::Large::col_lg_6+Bootstrap::Grid::Medium::col_md_6+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_1);

    auto dateTimeStr = WDate::fromJulianDay(item.element(Key::julianDay).value().view().get_int64().value).toString("dd/MM/yy") + " " + QDateTime::fromMSecsSinceEpoch(item.element(Key::epochTime).value().view().get_int64().value).time().toString("hh:mm").toStdString();
    rContainer->addWidget(cpp14::make_unique<WText>(WString{"{1}"}.arg(dateTimeStr)))
                          ->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);

    auto pList = item.getGorevliList();
    if( pList.size() ){
        auto __gContainer = rContainer->addWidget(cpp14::make_unique<WText>(WString{"{1} +{2}"}.arg(pList.front().adSoyad()).arg(pList.size()-1)));
            __gContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
        __gContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::ForestGreen)+Style::color::color(Style::color::White::AliceBlue));
    }else{
        auto __gContainer = rContainer->addWidget(cpp14::make_unique<WText>(WString{"{1}"}.arg("Personel Yok")));
            __gContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_3+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
        __gContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::LightGray)+Style::color::color(Style::color::White::AliceBlue));
    }




    auto durumContainer = rContainer->addWidget(cpp14::make_unique<WText>(item.getDurum()));
    durumContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);

    if( item.isTamamlandi() ){
        durumContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::Chartreuse)+Style::color::color(Style::color::White::AliceBlue));
    }else{
        durumContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Red::DarkRed)+Style::color::color(Style::color::White::AliceBlue));
    }
    decorationStyle().setCursor(Cursor::PointingHand);
}

std::string TaskListItem::getOid() const
{
    return mOid;
}

TaskItemWidget::TaskItemWidget(const TaskItem &item, SerikBLDCore::User *_mUser)
    :SerikBLDCore::DB(_mUser->getDB()),mUser(_mUser)
{
    this->setDocumentView(item.view());
    this->initWidget();
}

void TaskItemWidget::initWidget()
{
    this->Header()->clear();

    auto isAdiContainer = this->Header()->addNew<WText>("<h4><b>"+this->getIsAdi()+"</b></h4>");
    isAdiContainer->addStyleClass(Bootstrap::Grid::col_full_12);


    auto infoContainer = this->Header()->addNew<WContainerWidget>();
    infoContainer->addStyleClass(Bootstrap::Grid::col_full_12);

    auto hLayout = infoContainer->setLayout(cpp14::make_unique<WHBoxLayout>());

    auto dateContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    dateContainer->addNew<WText>(this->getDate());
    dateContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::LightGray)+Style::color::color(Style::color::Grey::Black));
    dateContainer->addStyleClass(CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);

    auto hourContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    hourContainer->addNew<WText>(this->getTime());
    hourContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::LightGray)+Style::color::color(Style::color::Grey::Black));
    hourContainer->addStyleClass(CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);

    auto durumContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    durumContainer->addStyleClass(CSSStyle::Gradient::grayGradient90+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);

    if( isTamamlandi() ){
        durumContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::Chartreuse)+Style::color::color(Style::color::White::AliceBlue));
    }else{
        durumContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Red::DarkRed)+Style::color::color(Style::color::White::AliceBlue));
    }
    durumContainer->addNew<WText>(this->getDurum());

    auto pList = this->getGorevliList();
    if( pList.size() ){
        for( const auto &pItem : pList ){
            auto __gContainer = hLayout->addWidget(cpp14::make_unique<WText>(WString{"{1}"}.arg(pItem.adSoyad())));
            __gContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::ForestGreen)+Style::color::color(Style::color::White::AliceBlue));
            __gContainer->addStyleClass(CSSStyle::Gradient::grayGradient90+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
        }
    }else{
        auto __gContainer = hLayout->addWidget(cpp14::make_unique<WText>(WString{"{1}"}.arg("Personel Yok")));
        __gContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::LightGray)+Style::color::color(Style::color::White::AliceBlue));
        __gContainer->addStyleClass(CSSStyle::Gradient::grayGradient90+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    }


    hLayout->addStretch(1);

    auto baskanYardimcisiContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    auto baskanYrdText = baskanYardimcisiContainer->addNew<WText>(this->getBaskanYardimcisi().adSoyad());
    baskanYrdText->setAttributeValue(Style::style,Style::background::color::color(Style::color::Yellow::Moccasin)+Style::color::color(Style::color::Grey::DimGray));
    baskanYrdText->addStyleClass(CSSStyle::Gradient::grayGradient90+CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    baskanYrdText->setInline(true);
    baskanYrdText->setPadding(5,AllSides);

    auto imageContainer = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
    imageContainer->setHeight(250);
    imageContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    auto fileUrl = this->downloadFileWeb(this->getImageOid().c_str());
    imageContainer->setAttributeValue(Style::style,Style::background::url(fileUrl)+
                                                        Style::background::size::contain+
                                                        Style::background::position::center_center+
                                                        Style::background::repeat::norepeat);
    imageContainer->setMargin(5,Side::Top|Side::Bottom);


    auto isAciklamaContainer = this->Header()->addNew<WText>(this->getAciklama());
    isAciklamaContainer->setMargin(10,Side::Top|Side::Bottom);
    isAciklamaContainer->setPadding(10,Side::Top|Side::Bottom);
    isAciklamaContainer->addStyleClass(Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_rounded);

    auto list = this->getAkisList();
    for( const auto &akisItem : list ){
        this->loadAkis(akisItem);
    }
}

void TaskItemWidget::loadAkis(const SubItem &akisItem)
{
    auto container = this->Content()->addNew<SubItem>(akisItem);
    container->setUser(mUser);
    container->addStyleClass(Bootstrap::Grid::col_full_12);


    auto updateTaskItem = [=]( const SubItem::Onay &onay, const bool mudur = true){
        TaskItem filter;
        filter.setOid(this->oid().value().to_string());
        SerikBLDCore::Item elematch("");
        elematch.append("$elemMatch",make_document(kvp(Key::AKIS::uuid,container->uuidString())));
        filter.append(Key::akis,elematch);
        TaskItem setObj;
        if( mudur ){
            setObj.append("$set",make_document(kvp(Key::akis+".$."+Key::AKIS::mudurOnay,
                                                    bsoncxx::types::b_int32{static_cast<std::int32_t>(onay)})));
        }else{
            setObj.append("$set",make_document(kvp(Key::akis+".$."+Key::AKIS::baskanYrdOnay,
                                                    bsoncxx::types::b_int32{static_cast<std::int32_t>(onay)})));
        }

        auto upt = this->getDB()->db()->collection(filter.getCollection()).update_one(filter.view(),setObj.view());
        if( upt ){
            this->showPopUpMessage("Güncellendi");
        }else{
            this->showPopUpMessage("Hata: Task Güncellenemedi");
        }
    };


    container->mudurOnayClicked().connect([=](const SubItem::Onay &onay){
        updateTaskItem(onay,true);
    });
    container->baskanYrdOnayClicked().connect([=]( const SubItem::Onay &onay){
        updateTaskItem(onay,false);
    });


}

PersonelSelectWidget::PersonelSelectWidget(const Personel &personel)
{
    this->setDocumentView(personel.view());
    this->addWidget(cpp14::make_unique<WText>(personel.AdSoyad().toStdString()));

    decorationStyle().setCursor(Cursor::PointingHand);
    addStyleClass(CSSStyle::Radius::radius3px);
    setPadding(10,AllSides);
    setMargin(2,AllSides);
    addStyleClass("taskManagerPersonelUnSelected");

    clicked().connect([=](){
        ToogleSelect();
    });
}

bool PersonelSelectWidget::selected() const
{
    return mSelected;
}

void PersonelSelectWidget::setSelect()
{
    if( !mSelected ){
        ToogleSelect();
    }
}

void PersonelSelectWidget::setUnselect()
{
    if( mSelected ){
        ToogleSelect();
    }
}

void PersonelSelectWidget::ToogleSelect()
{
    if( mSelected ){
        removeStyleClass("taskManagerPersonelSelected");
        addStyleClass("taskManagerPersonelUnSelected");
    }else{
        removeStyleClass("taskManagerPersonelUnSelected");
        addStyleClass("taskManagerPersonelSelected");
    }
    mSelected = !mSelected;
}




} // namespace TodoList
