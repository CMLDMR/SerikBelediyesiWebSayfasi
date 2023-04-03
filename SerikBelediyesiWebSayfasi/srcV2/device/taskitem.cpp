#include "taskitem.h"

#include <filesystem>
#include <fstream>
#include <Wt/WApplication.h>
#include <Wt/WLink.h>

#include "personelmanager.h"

namespace TodoList {


TaskItem::TaskItem()
    :SerikBLDCore::Item(Key::Collection.data())
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

TaskItem &TaskItem::addGorevli(const std::string &gorevliOid, const std::string &adSoyad)
{
    GorevliItem item;
    item.setPersonel(gorevliOid,adSoyad);

    auto list = getGorevliList();

    bool exist = false;
    for( const auto &personel : list ){
        if( personel.personelOid() == gorevliOid ){
            exist = true;
            break;
        }
    }
    if( !exist ){
        this->pushArray(Key::gorevli,item.view());
    }
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

TaskManager::TaskManager(SerikBLDCore::User *_mUser)
    :SerikBLDCore::ListItem<TaskItem>(_mUser->getDB()),
      mUser(_mUser)
{

//    this->initCSS();
    this->Content()->setMargin(15,Side::Top);
    this->initHeader();
    this->UpdateList(TaskItem().setBirim(mUser->Birimi()));
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

        TaskItem taskItem;

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
            TaskItem filterItem;
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
        auto container = this->Content()->addWidget(cpp14::make_unique<TaskItemWidget>(val,this->getDB()));
        container->setMargin(25,Side::Top);



        this->Footer()->clear();
        auto hLayout = this->Footer()->setLayout(cpp14::make_unique<WHBoxLayout>());

        auto gorevliBtn = createSmallButton("Personel Ata+");
        gorevliBtn->clicked().connect([=](){
            this->assignPersonel(taskoid);
        });
        hLayout->addWidget(std::move(gorevliBtn));

        auto malzemeEkleBtn = createSmallButton("Malzeme Ekle+");
        hLayout->addWidget(std::move(malzemeEkleBtn));

        auto aciklamaBtn = createSmallButton("Açıklama Ekle+");
        hLayout->addWidget(std::move(aciklamaBtn));

        auto resimEkleBtn = createSmallButton("Resim Ekle+");
        hLayout->addWidget(std::move(resimEkleBtn));

        auto tamamlaBtn = createSmallButton("Tamamla");
        hLayout->addWidget(std::move(tamamlaBtn));

        auto silBtn = createSmallButton("SİL!");
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

        LOG << bsoncxx::to_json(taskItem.view()).c_str() << "\n";

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

TaskItemWidget::TaskItemWidget(const TaskItem &item, DB *mDB)
    :SerikBLDCore::DB(mDB)
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
