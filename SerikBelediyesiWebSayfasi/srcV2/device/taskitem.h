#ifndef TODOLIST_TASKITEM_H
#define TODOLIST_TASKITEM_H


#include "item.h"
#include "listitem.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "user.h"

namespace TodoList {


namespace Key {
inline const std::string Collection{"TodoList"};
inline const std::string birim{"birim"};
inline const std::string julianDay{"julianDay"};
inline const std::string epochTime{"epochTime"};
inline const std::string isAdi{"isAdi"};
inline const std::string aciklama{"aciklama"};
inline const std::string akis{"akis"};
inline const std::string resim{"resim"};
inline const std::string tamamlandi{"tamamlandi"};
inline const std::string gorevli{"gorevli"};
namespace AKIS {
inline const std::string aciklama{"aciklama"};
inline const std::string resim{"resim"};
inline const std::string julianDay{"julianDay"};
inline const std::string epcohTime{"epochTime"};
inline const std::string uuid{"uuid"};
}
}



class TaskItem : public SerikBLDCore::Item
{
public:
    TaskItem();
    TaskItem( const TaskItem &other ):SerikBLDCore::Item(Key::Collection.data()){
        this->setDocumentView(other.view());
    }

    TaskItem( TaskItem &&other ):SerikBLDCore::Item(Key::Collection.data()){
        this->setDocumentView(other.view());
    }

    TaskItem &operator=( const TaskItem &other ){
        this->setDocumentView(other.view());
        return *this;
    }

    TaskItem &operator=( TaskItem &&other ){
        this->setDocumentView(other.view());
        return *this;
    }


    TaskItem &setBirim(const std::string &birim );

};

class TaskListItem : public WContainerWidget
{
public:
    TaskListItem(const TaskItem &item);
};


class TaskManager : public SerikBLDCore::ListItem<TaskItem>, public ContainerWidget
{
public:
    explicit TaskManager(SerikBLDCore::User* _mUser);

    virtual void onList( const QVector<TodoList::TaskItem> *mlist ) override;

    void initHeader();

    void addNewTask();

private:
    SerikBLDCore::User* mUser;
};

} // namespace TodoList

#endif // TODOLIST_TASKITEM_H
