
#include "aciklamaitem.h"

namespace TodoList {

AciklamaItem::AciklamaItem()
    :BaseItem::BaseItem(BaseItem::Type::ACIKLAMA)

{

}

AciklamaItem::AciklamaItem(const AciklamaItem &other)
    :BaseItem::BaseItem(other)
{

}

AciklamaItem::AciklamaItem(AciklamaItem &&other)
    :TodoList::BaseItem::BaseItem(other)
{

}

AciklamaItem::AciklamaItem(const BaseItem &other)
    :TodoList::BaseItem::BaseItem(other)
{

}

void AciklamaItem::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage(errorText,"warn");
}

void AciklamaItem::initWidget()
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

    auto aciklamaText = this->Content()->addWidget(cpp14::make_unique<WText>(this->getAciklama()));
    aciklamaText->addStyleClass(Bootstrap::Grid::col_full_12);
    aciklamaText->setMargin(10,Side::Top|Side::Bottom);
}

} // namespace TodoList

