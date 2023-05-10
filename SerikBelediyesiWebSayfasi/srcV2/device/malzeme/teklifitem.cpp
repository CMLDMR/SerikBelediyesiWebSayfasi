
#include "teklifitem.h"

namespace TodoList {

TeklifItem::TeklifItem()
    :BaseItem::BaseItem(BaseItem::Type::TEKLIF)
{

}

TeklifItem::TeklifItem(const TeklifItem &other)
    :BaseItem::BaseItem(other)

{

}

TeklifItem::TeklifItem(TeklifItem &&other)
    :BaseItem::BaseItem(other)

{

}

TeklifItem::TeklifItem(const BaseItem &other)
    :BaseItem::BaseItem(other)

{

}

void TeklifItem::errorOccured(const std::string &errorText)
{

}

void TeklifItem::initWidget()
{
    this->addStyleClass(CSSStyle::Radius::radius3px);
    auto baslikContainer = this->Content()->addNew<WContainerWidget>();
    baslikContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    baslikContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::GhostWhite));
    baslikContainer->addWidget(cpp14::make_unique<WText>("<h4>Teklifler</h4>"));


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

