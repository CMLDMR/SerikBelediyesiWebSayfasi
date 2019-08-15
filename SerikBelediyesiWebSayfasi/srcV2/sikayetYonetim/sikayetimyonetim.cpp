#include "sikayetimyonetim.h"
#include "bilgiedinmeyonetim.h"
#include "sikayetyonetimwidget.h"

SikayetimYonetim::SikayetimYonetim(mongocxx::database *_db, bsoncxx::document::value _user)
    :BaseClass::ContainerWidget (_db,_user,"Talep/Bilgi Edinme Yönetimi")
{

    auto rContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    rContainer->setMargin(15,Side::Top|Side::Bottom);


    auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    container->addStyleClass(Bootstrap::Grid::col_full_12);

    auto mContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
    mContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto _rContainer = mContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    _rContainer->addStyleClass(Bootstrap::Grid::row);

    {
        auto BilgiEdinmeContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        BilgiEdinmeContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6
                                            +Bootstrap::Grid::Medium::col_md_6
                                            +Bootstrap::Grid::Small::col_sm_6
                                            +Bootstrap::Grid::ExtraSmall::col_xs_6);
        BilgiEdinmeContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

        BilgiEdinmeContainer->setHeight(100);
        BilgiEdinmeContainer->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(),this->getRandom(),this->getRandom()));

        auto vLayout = BilgiEdinmeContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Bilgi Edinme"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px
                                +Style::color::color(Style::color::White::Snow));

        BilgiEdinmeContainer->decorationStyle().setCursor(Cursor::PointingHand);

        BilgiEdinmeContainer->clicked().connect(this,&SikayetimYonetim::initBilgiEdinme);

    }

    {
        auto SikayetContainer = _rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        SikayetContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6
                                            +Bootstrap::Grid::Medium::col_md_6
                                            +Bootstrap::Grid::Small::col_sm_6
                                            +Bootstrap::Grid::ExtraSmall::col_xs_6);
        SikayetContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

        SikayetContainer->setHeight(100);
        SikayetContainer->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(),this->getRandom(),this->getRandom()));

        auto vLayout = SikayetContainer->setLayout(cpp14::make_unique<WVBoxLayout>());

        auto text = vLayout->addWidget(cpp14::make_unique<WText>("Şikayet Yönetimi\n(Yapım Aşamasında)"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->setAttributeValue(Style::style,Style::font::size::s16px
                                +Style::color::color(Style::color::White::Snow));

        SikayetContainer->decorationStyle().setCursor(Cursor::PointingHand);
        SikayetContainer->clicked().connect(this,&SikayetimYonetim::initSikayetler);

    }



    mContentContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    mContentContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    mContentContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    mContentContainer->setMargin(25,Side::Top);


}

void SikayetimYonetim::initBilgiEdinme()
{
    mContentContainer->clear();
    mContentContainer->addWidget(cpp14::make_unique<BilgiEdinmeYonetim>(this->db(),this->User()));
}



void SikayetimYonetim::initSikayetler()
{
    mContentContainer->clear();
    mContentContainer->addWidget(cpp14::make_unique<SikayetYonetimWidget>(this->db(),this->User()));
}
