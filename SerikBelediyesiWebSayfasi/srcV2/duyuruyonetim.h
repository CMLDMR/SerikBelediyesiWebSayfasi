#ifndef DUYURUYONETIM_H
#define DUYURUYONETIM_H
#include "item.h"
#include "listitem.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"



namespace v2{


namespace Duyuru {
namespace Key {
inline constexpr std::string Collection{"Duyurular"};
inline constexpr std::string baslik{"Başlık"};
inline constexpr std::string yayinda{"Yayında"};
inline constexpr std::string birim{"Birim"};

[[deprecated("Eski Program da Oluşturulan Tarihler. Yenisi julianBaslangic")]]
inline constexpr std::string endDate{"Bitiş Tarihi"};
inline constexpr std::string icerik{"İçerik"};
inline constexpr std::string julianBaslangic{"julianBaslangic"};
inline constexpr std::string julianBitis{"julianBitis"};

[[deprecated("Eski Program da Oluşturulan fileList. Yenisi fileOidList")]]
inline constexpr std::string fileList{"fileList"};

inline constexpr std::string fileOidList{"fileOidList"};
namespace FILEKEY {
inline constexpr std::string fileTitle{"fileTitle"};
inline constexpr std::string fileOid{"fileOid"};
}


}

class DuyuruItem : public SerikBLDCore::Item
{
public:
    explicit DuyuruItem():SerikBLDCore::Item(Duyuru::Key::Collection){}
    std::string Baslik() const;
    bool Yayinda() const;
    std::string Birim() const;
    std::string SonTarihText() const;
    std::string Icerik() const;
    std::int64_t BaslangicDate() const;
    std::int64_t BitisDate() const;

    [[deprecated("Eski Program da Oluşturulan Dosya Oidleri")]]
    std::vector<std::string> fileList();

    std::map<std::string,std::string> fileOidList() const;

    DuyuruItem& setBaslik( const std::string &baslik);
    DuyuruItem& setYayinda( const bool &yayinda );
    DuyuruItem& setJulianDate( const std::int64_t &start_date , const std::int64_t &end_date );
    DuyuruItem& setIcerik( const std::string &icerik);
    DuyuruItem& addFile( const std::string &fileTitle , const std::string &fileOid);
    DuyuruItem& removeFile(const std::string &fileOid);
    DuyuruItem& setBirim( const std::string &birim );

};

class DuyuruYonetim : public ContainerWidget, public SerikBLDCore::ListItem<DuyuruItem>
{
public:
    explicit DuyuruYonetim(SerikBLDCore::DB* _mdb);
    void onList( const QVector<DuyuruItem> *mlist) override;
    void errorOccured( const std::string &errorText ) override;
    void initDuyuruItem( const std::string &duyuruOid );
    void yeniDuyuruEkle();

    std::optional<ControllerWidget*> mControllerWidget;

private:
            //[ FileOid , File Title ]
    std::map<std::string,std::string> newFileList;
};


namespace Widget {
    class ListItem : public ContainerWidget, public DuyuruItem
    {
    public:
        explicit ListItem(const DuyuruItem &item);
    };

    class DuyuruView : public ContainerWidget , public DuyuruItem
    {
    public:
        explicit DuyuruView( const DuyuruItem &item ,SerikBLDCore::DB* _mDB, const bool &publicView = true);

        void loadPublic();
        void loadEditable();
    private:
        bool mPublicView;
        SerikBLDCore::DB* mDB;
    };



    class PublicListView : public ContainerWidget, public SerikBLDCore::ListItem<DuyuruItem>
    {
    public:
        explicit PublicListView(SerikBLDCore::DB* _mdb);

        void onList( const QVector<DuyuruItem> *mlist) override;

        void initList();

        void initDuyuruItem( const std::string &duyuruOid );

    private:
        WContainerWidget* mListContainerWidget;
        WContainerWidget* mContentContainer;

    };

}
}
}
#endif // DUYURUYONETIM_H
