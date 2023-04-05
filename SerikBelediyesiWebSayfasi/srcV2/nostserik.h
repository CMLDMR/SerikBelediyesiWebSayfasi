#ifndef NOSTSERIK_H
#define NOSTSERIK_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "item.h"
#include "listitem.h"

namespace SerikBLDCore {
class DB;
}

namespace v2 {




namespace Tip {
const static std::string Resim{"resim"};
const static std::string Video{"video"};
};

namespace NostSerikKey {
const static std::string Aciklama{"aciklama"};
const static std::string Tip{"tip"};
const static std::string fileOid{"fileOid"};
const static std::string thumbNails{"thumbNails"};
const static std::string width{"width"};
const static std::string height{"height"};
};


class NostItem : public SerikBLDCore::Item
{
    const std::string Collection{"Galeri"};
public:
    NostItem();
    NostItem( const NostItem &other);
    NostItem( const NostItem &&other);



    NostItem &operator=( const NostItem &&other);
    NostItem &operator=( const NostItem &other);


    NostItem &setThumbnail( const std::string &fileOid );
    NostItem &setFileOid( const std::string &fileoid, const std::string &tipname = Tip::Resim);
    NostItem &setAciklama( const std::string &aciklama );
    NostItem &setTip( const std::string &tipname );
    NostItem &setWidthHeight( const int &w , const int &h );

    std::string getAciklama() const;
    std::string getTip() const;
    std::string getFileOid() const;
    std::string getThumbNails() const;
    int getWidth() const;
    int getHeight() const;
};


class NostItemThumb : public WContainerWidget
{
public:
    NostItemThumb(const std::string &url , const std::string &aciklama, const std::string &fileOid);

    Signal<std::string> &Clicked();
    Signal<std::string> &DelClicked();
    Signal<std::string> &EditClicked();
private:
    std::string mFileOid;
    Signal<std::string> _clicked;
    Signal<std::string> _delClicked;
    Signal<std::string> _editClicked;
};


class NostSerikManager : public ContainerWidget , public SerikBLDCore::ListItem<NostItem>
{
public:
    explicit NostSerikManager(SerikBLDCore::DB* mDB);


    virtual void onList(const QVector<NostItem> *mlist ) override;

private:
    void initController();

    void addNewFile();
    WText* mCurrentPageInfo;
};


class NostSerik : public ContainerWidget , public SerikBLDCore::ListItem<NostItem>
{



public:
    explicit NostSerik(SerikBLDCore::DB* mDB, const bool &showFullScreen = false );

    virtual void onList(const QVector<NostItem> *mlist ) override;

    WContainerWidget* mContentContainer;
    WContainerWidget* mImgFullContainer = nullptr;


    QVector<NostItem> mFList;

    void showItem();
    void showItemFull();

    int image_index{0};

private:
    bool mShowFullScreen{false};
};


}


#endif // NOSTSERIK_H
