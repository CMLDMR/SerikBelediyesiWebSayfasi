
#include "resimitem.h"
#include <db.h>

namespace TodoList {

ResimItem::ResimItem()
    :BaseItem::BaseItem(BaseItem::Type::RESIM)
{

}

ResimItem::ResimItem(const ResimItem &other)
    :BaseItem::BaseItem(other)
{

}

ResimItem::ResimItem(ResimItem &&other)
    :TodoList::BaseItem::BaseItem(other)
{

}

ResimItem::ResimItem(const BaseItem &other)
    :TodoList::BaseItem::BaseItem(other)
{

}

ResimItem &ResimItem::setResimOid(const std::string &resimOid)
{
    this->append(Key::AKIS::resim,bsoncxx::oid{resimOid});
    return *this;
}

std::string ResimItem::getResimOid() const
{
    auto val = element(Key::AKIS::resim);
    if( val ){
        return val.value().view().get_oid().value.to_string();
    }
    return "";
}

} // namespace TodoList



void TodoList::ResimItem::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage(errorText,"warn");
}

void TodoList::ResimItem::initWidget()
{

    this->setMargin(30,Side::Top);
    this->addStyleClass(CSSStyle::Radius::radius3px);


    auto perInformationContainer = this->Content()->addWidget(cpp14::make_unique<WContainerWidget>());
    perInformationContainer->addStyleClass(Bootstrap::Grid::col_full_12);

    auto hLayout = perInformationContainer->setLayout(cpp14::make_unique<WHBoxLayout>());
    hLayout->addStretch(1);

    auto personelNameText = hLayout->addWidget(cpp14::make_unique<WText>(this->getPersoneName()));
    personelNameText->addStyleClass(Bootstrap::Label::Primary);
    auto timeText = hLayout->addWidget(cpp14::make_unique<WText>(this->getTimeString()));
    timeText->addStyleClass(Bootstrap::Label::info);
    auto dateText = hLayout->addWidget(cpp14::make_unique<WText>(this->getDateString()));
    dateText->addStyleClass(Bootstrap::Label::info);



    auto imgContainer = this->Content()->addNew<WContainerWidget>();
    imgContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    imgContainer->setHeight(350);

    auto imgPath = SerikBLDCore::DB::instance()->downloadFileWeb(this->getResimOid().c_str());

    imgContainer->setAttributeValue(Style::style,Style::background::url(imgPath)+
                                                      Style::background::size::contain+
                                                      Style::background::position::center_center+
                                                      Style::background::repeat::norepeat);


    auto aciklamaText = this->Content()->addWidget(cpp14::make_unique<WText>(this->getAciklama()));
    aciklamaText->addStyleClass(Bootstrap::Grid::col_full_12);
    aciklamaText->setMargin(10,Side::Top|Side::Bottom);

}
