#include "sikayetitemwidget.h"

SikayetItemWidget::SikayetItemWidget(mongocxx::database *_db, UserClass &userValue, const bsoncxx::oid &_oid)
    :DBClass (_db),UserClass (userValue),mCurrentSikayet(Sikayet::SikayetItem::Load_Sikayet(_db,_oid).value())
{
    this->initHeader();
    this->initContent();
}

void SikayetItemWidget::initHeader()
{
    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Right);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::adSoyad)->get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Label::Primary);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
    }

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Left);

        auto text = container->addWidget(cpp14::make_unique<WText>("05335018051 TCNOITEM class Yazılacak"));
        text->addStyleClass(Bootstrap::Label::Success);
        text->setAttributeValue(Style::style,Style::font::size::s16px+Style::font::weight::bold);
    }



    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Right);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::tarih)->get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Label::Default);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }
    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Left);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::saat)->get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Label::Default);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }




    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Center);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::mahalle)->get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Label::Default);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Center);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::tamAdress)->get_utf8().value.to_string()));
        text->addStyleClass(Bootstrap::Label::Default);
        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }

    {
        auto container = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

        container->setContentAlignment(AlignmentFlag::Justify);
        container->addWidget(cpp14::make_unique<WText>("<h4><b>Konu:</b></h4>"));
        container->addWidget(cpp14::make_unique<WBreak>());

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::konu)->get_utf8().value.to_string()));
    }
}

void SikayetItemWidget::initContent()
{

    this->Content()->clear();

    auto array = mCurrentSikayet->Element(Sikayet::KEY::asama).value().get_array().value;

    for( auto _item : array )
    {
        auto item = this->Content()->addWidget(cpp14::make_unique<AsamaItemWidget>(_item.get_document().value));
    }


}

AsamaItemWidget::AsamaItemWidget(bsoncxx::document::view &&view)
    :Sikayet::ASAMA (view)
{

    addStyleClass(Bootstrap::Grid::col_full_12);
    addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    setMargin(10,Side::Top);
    setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(150,170),this->getRandom(170,190),this->getRandom(190,210)));

    addWidget(cpp14::make_unique<WText>(tarih()));
    addWidget(cpp14::make_unique<WBreak>());
    addWidget(cpp14::make_unique<WText>(saat()));

    if( tip() == Sikayet::KEY::ASAMAKEY::TIP::aciklama )
    {
        addWidget(cpp14::make_unique<WBreak>());
        addWidget(cpp14::make_unique<WText>(aciklama()));
    }else{
        addWidget(cpp14::make_unique<WBreak>());
        addWidget(cpp14::make_unique<WText>(degisiklik()));
    }

    {
        addWidget(cpp14::make_unique<WBreak>());
        auto birimText = addWidget(cpp14::make_unique<WText>(birim()));
        birimText->addStyleClass(Bootstrap::Label::Primary);
        birimText->setAttributeValue(Style::style,Style::font::size::s12px);
    }

    {
        addWidget(cpp14::make_unique<WBreak>());
        auto birimText = addWidget(cpp14::make_unique<WText>(personelAdi()));
        birimText->addStyleClass(Bootstrap::Label::Success);
        birimText->setAttributeValue(Style::style,Style::font::size::s10px);
    }

}
