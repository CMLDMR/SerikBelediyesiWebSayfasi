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
inline constexpr std::string endDate{"Bitiş Tarihi"};
inline constexpr std::string icerik{"İçerik"};
inline constexpr std::string julianBaslangic{"julianBaslangic"};
inline constexpr std::string julianBitis{"julianBitis"};


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

};



namespace Widget {

    class ListItem : public ContainerWidget, public DuyuruItem
    {
    public:
        ListItem(const DuyuruItem &item);
    };

    class DuyuruView : public ContainerWidget , public DuyuruItem
    {
    public:
        DuyuruView( const DuyuruItem &item );
    };

}


}




}



#endif // DUYURUYONETIM_H
