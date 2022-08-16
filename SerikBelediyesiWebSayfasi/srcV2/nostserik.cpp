#include "nostserik.h"
#include "inlinestyle.h"
#include "bootstrap.h"

#include "db.h"


v2::NostSerik::NostSerik()
{

    this->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::DarkCyan));

    this->setPositionScheme(PositionScheme::Fixed);
    this->setOffsets(0,AllSides);


//    this->clicked().connect([=](){
//       this->parent()->removeWidget(this);
//    });


}


v2::NostSerikManager::NostSerikManager(SerikBLDCore::DB *mDB)
    :SerikBLDCore::ListItem<NostItem>(mDB)
{

    this->UpdateList();

}

void v2::NostSerikManager::onList(const QVector<NostItem> *mList)
{

    this->Content()->addWidget(cpp14::make_unique<WText>("fdsgsdfg"));

}

v2::NostItem::NostItem()
    :SerikBLDCore::Item("sd")
{

}
