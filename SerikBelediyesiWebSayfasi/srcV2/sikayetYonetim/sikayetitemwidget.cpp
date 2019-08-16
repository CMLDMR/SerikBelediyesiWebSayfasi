#include "sikayetitemwidget.h"

SikayetItemWidget::SikayetItemWidget(mongocxx::database *_db, UserClass &userValue, const bsoncxx::oid &_oid)
    :DBClass (_db),UserClass (userValue),mCurrentSikayet(Sikayet::SikayetItem::Load_Sikayet(_db,_oid).value())
{
    this->initHeader();
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

        auto text = container->addWidget(cpp14::make_unique<WText>("05335018051"));
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
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                                 Bootstrap::Grid::Medium::col_md_6+
                                 Bootstrap::Grid::Small::col_sm_6+
                                 Bootstrap::Grid::ExtraSmall::col_xs_6);
        container->setMargin(15,Side::Top);

        container->setContentAlignment(AlignmentFlag::Right);

        auto text = container->addWidget(cpp14::make_unique<WText>(this->mCurrentSikayet->Element(Sikayet::KEY::mahalle)->get_utf8().value.to_string()));
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
//        text->addStyleClass(Bootstrap::Label::Default);
//        text->setAttributeValue(Style::style,Style::font::size::s14px+Style::font::weight::normal);
    }
}
