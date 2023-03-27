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

TaskManager::TaskManager(SerikBLDCore::User *_mUser)
    :SerikBLDCore::ListItem<TaskItem>(_mUser->getDB()),
      mUser(_mUser)
{

    this->initHeader();

    this->UpdateList(TaskItem().setBirim(mUser->Birimi()));

}

void TaskManager::onList(const QVector<TaskItem> *mlist)
{

    this->Content()->clear();


    for( const auto &item : *mlist ){
        auto container = this->Content()->addNew<TaskListItem>(item);
        container->addStyleClass(Bootstrap::Grid::col_full_12);
    }


}

void TaskManager::initHeader()
{

    this->Header()->clear();

    auto infoContainer = this->Header()->addNew<WContainerWidget>();
    infoContainer->addStyleClass(Bootstrap::Grid::col_full_6);
    infoContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_info);
    infoContainer->setPadding(5,AllSides);
    infoContainer->addNew<WText>(this->mUser->Birimi());

    auto newTaskContainer = this->Header()->addNew<WContainerWidget>();
    newTaskContainer->addStyleClass(Bootstrap::Grid::col_full_6);
    newTaskContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);
    newTaskContainer->setPadding(5,AllSides);
    newTaskContainer->addNew<WText>("<b>Yeni Ekle+</b>");

    newTaskContainer->decorationStyle().setCursor(Cursor::PointingHand);

    newTaskContainer->clicked().connect(this,&TaskManager::addNewTask);

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
    auto uploaderWidget = mDialog->Content()->addNew<FileUploaderWidget>();
    uploaderWidget->setType(FileUploaderWidget::Image);

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

        taskItem.append(Key::aciklama,textArea->text().toUTF8());
        taskItem.append(Key::birim,mUser->Birimi());
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

TaskListItem::TaskListItem(const TaskItem &item)
{

    addNew<WText>(item.element(Key::isAdi).value().view().get_string().value.data());


}

} // namespace TodoList
