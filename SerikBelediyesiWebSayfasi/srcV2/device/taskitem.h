#ifndef TODOLIST_TASKITEM_H
#define TODOLIST_TASKITEM_H


#include "listitem.h"
#include "SerikBelediyesiWebSayfasi/srcV2/device/malzeme/malzemeitem.h"
#include "stok/stokv2manager.h"

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
    inline const std::string imageOid{"imageOid"};
    inline const std::string gorevli{"gorevli"};
    inline const std::string baskanYardimcisi{"baskanYardimcisi"};

    namespace GOREVLI {
        inline const std::string gorevliOid{"gorevliOid"};
        inline const std::string gorevliAdSoyad{"gorevliadSoyad"};
    }
    namespace BASKANYARDIMCISI{
        inline const std::string baskanYardimcisiOid{"baskanYardimcisiOid"};
        inline const std::string baskanYardimcisiAdSoyad{"baskanYardimcisiAdSoyad"};
    }
}



class GorevliItem : public SerikBLDCore::Item
{
public:
    GorevliItem() : SerikBLDCore::Item(""){}

    GorevliItem &setPersonel( const std::string &personelOid, const std::string &adSoyad ){
        this->append(Key::GOREVLI::gorevliOid,bsoncxx::oid{personelOid});
        this->append(Key::GOREVLI::gorevliAdSoyad,adSoyad);
        return *this;
    }

    std::string personelOid() const{
        auto val = this->element(Key::GOREVLI::gorevliOid);
        if( val ){
            return val.value().view().get_oid().value.to_string();
        }
        return "";
    }

    std::string adSoyad() const {
        auto val = this->element(Key::GOREVLI::gorevliAdSoyad);
        if( val ){
            return val.value().view().get_string().value.data();
        }
        return "";
    }
};





class TaskItem : public SerikBLDCore::Item
{
public:
    TaskItem();
    TaskItem(SerikBLDCore::User* _mUser);
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
    TaskItem &setImageItem( const std::string &oid );
    TaskItem &setBaskanYardimcisi( const std::string &baskanYrdOid , const std::string &baskanYrdAdSoyad );
    TaskItem &addGorevli( const std::string &gorevliOid , const std::string &adSoyad );
    TaskItem &addAkis( const MalzemeItem &subItem );



    std::string getDate(const std::string &format = "dd/MM/yyyy") const;
    std::string getTime( const std::string &format = "hh:mm:ss") const;
    std::string getIsAdi() const;
    std::string getAciklama() const;
    std::string getDurum() const;
    std::string getImageOid() const;
    bool isTamamlandi() const;
    std::list<GorevliItem> getGorevliList() const;
    bool isGorevli( const std::string &gorevliOid ) const;
    std::list<MalzemeItem> getAkisList() const;
    GorevliItem getBaskanYardimcisi() const;

private:
    SerikBLDCore::User* mUser;
};



class TaskItemWidget : public ContainerWidget, public TaskItem, public SerikBLDCore::DB
{
public:
    explicit TaskItemWidget(const TaskItem &item , SerikBLDCore::User* _mUser);

private:
    void initWidget();
    void loadAkis(const BaseItem &akisItem );
    SerikBLDCore::User* mUser;

    WContainerWidget* mMalzemeListContainer;
    WContainerWidget* mSperatorContainer;
    WContainerWidget* mTeklifItemContainer;

};




class TaskListItem : public WContainerWidget
{
public:
    TaskListItem(const TaskItem &item);


    std::string getOid() const;

private:
    std::string mOid;
};



class PersonelSelectWidget;
class TaskManager : public SerikBLDCore::ListItem<TaskItem>, public ContainerWidget
{
public:
    explicit TaskManager(SerikBLDCore::User* _mUser);

    virtual void onList( const QVector<TodoList::TaskItem> *mlist ) override;

    void initCSS();

    void initHeader();

    void addNewTask();

    void loadTask( const std::string &taskoid );

    void assignPersonel( const std::string &taskOid );
    void assignBaskanYardimcisi( const std::string &taskOid );
    void assignResim( const std::string &taskOid );
    void assignAciklama( const std::string &taskOid );

    void assignMalzeme( const std::string &taskOid );
    void reListMalzeme( WContainerWidget* mMalzemeListContainer , QList<MalzemeListItem>* mList );

    std::unique_ptr<WContainerWidget> createSmallButton( const std::string &name );

    void updateTaskList();

private:
    SerikBLDCore::User* mUser{nullptr};

    std::list<PersonelSelectWidget*> mPersonelSelectWidget;

    void deleteTask( const std::string &taskOid );

};


class PersonelSelectWidget : public SerikBLDCore::IK::Personel, public WContainerWidget
{
public:
    PersonelSelectWidget(const SerikBLDCore::IK::Personel &personel);


    bool selected() const;

    void setSelect();
    void setUnselect();
    void ToogleSelect();

private:
    bool mSelected{false};
};


} // namespace TodoList

#endif // TODOLIST_TASKITEM_H
