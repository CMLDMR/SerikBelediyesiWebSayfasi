#ifndef NOSTSERIK_H
#define NOSTSERIK_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "item.h"
#include "listitem.h"

namespace SerikBLDCore {
class DB;
}

namespace v2 {


class NostSerik : public ContainerWidget
{
public:
    NostSerik();
};

namespace NostSerikKey {

namespace NostSerikTip{
const static std::string resim{"resim"};
const static std::string video{"video"};
}


const static std::string Collection{"Galeri"};
const static std::string tip{"tip"};
const static std::string aciklama{"aciklama"};


}


class NostItem : public SerikBLDCore::Item
{
public:
    NostItem();
};


class NostSerikManager : public ContainerWidget , public SerikBLDCore::ListItem<NostItem>
{
public:
    NostSerikManager(SerikBLDCore::DB* mDB);


    virtual void onList(const QVector<NostItem> *mList ) override;
};




}


#endif // NOSTSERIK_H
