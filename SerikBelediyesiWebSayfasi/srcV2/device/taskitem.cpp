#include "taskitem.h"


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

TaskItem &TaskItem::setGorevli(const std::string &gorevliOid, const std::string &adSoyad)
{
    this->append(Key::gorevli,bsoncxx::oid{gorevliOid});
    this->append(Key::gorevliAdSoyad,adSoyad);
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
        return isTamamlandi() ? "Tamamlandı" : "Devam Ediyor";
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

TaskManager::TaskManager(SerikBLDCore::User *_mUser)
    :SerikBLDCore::ListItem<TaskItem>(_mUser->getDB()),
      mUser(_mUser)
{
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






    }


}

TaskListItem::TaskListItem(const TaskItem &item)
{
    setPadding(3,Side::Top|Side::Bottom);
    setMargin(3,Side::Top|Side::Bottom);

    this->mOid = item.oid().value().to_string();
    auto rContainer = this->addNew<WContainerWidget>();
    rContainer->addStyleClass(Bootstrap::Grid::row);

    rContainer->addWidget(cpp14::make_unique<WText>(item.element(Key::isAdi).value().view().get_string().value.data()))
            ->addStyleClass(Bootstrap::Grid::Large::col_lg_8+Bootstrap::Grid::Medium::col_md_8+Bootstrap::Grid::Small::col_sm_1+Bootstrap::Grid::ExtraSmall::col_xs_1);

    rContainer->addWidget(cpp14::make_unique<WText>(WString{"{1}"}.arg(WDate::fromJulianDay(item.element(Key::julianDay).value().view().get_int64().value).toString("dd/MM/yyyy"))))
                          ->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
    rContainer->addWidget(cpp14::make_unique<WText>(WString{"{1}"}.arg(QDateTime::fromMSecsSinceEpoch(item.element(Key::epochTime).value().view().get_int64().value).time().toString("hh:mm:ss").toStdString())))
                          ->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);

    auto durumContainer = rContainer->addWidget(cpp14::make_unique<WText>(item.getDurum()));
    durumContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_4);
//    durumContainer->addStyleClass(item.isTamamlandi() ? Bootstrap::ContextualBackGround::bg_success : Bootstrap::ContextualBackGround::bg_danger);
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




} // namespace TodoList
