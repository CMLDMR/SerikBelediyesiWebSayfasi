#include "footer.h"

Footer::Footer::Footer()
{

    addStyleClass(Bootstrap::Grid::container_fluid);
    setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::Black));
    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());

    mMainContainer->addStyleClass(Bootstrap::Grid::row);
    mMainContainer->setContentAlignment(AlignmentFlag::Center);
    auto SubMainContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    SubMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);
    SubMainContainer->setMaximumSize(1600,WLength::Auto);
    SubMainContainer->setHeight(150);
    SubMainContainer->setMargin(50,Side::Top);

    auto row = SubMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    row->addStyleClass(Bootstrap::Grid::row);

//    {
//        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
//        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
//        container->setAttributeValue(Style::style,Style::color::rgb("255,255,255"));
//        auto layoutContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        auto layout = layoutContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//        auto title = layout->addWidget(cpp14::make_unique<WText>("Yönetim"),0,AlignmentFlag::Left);
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Azure)+Style::font::size::s20px);

//        {
//            std::unique_ptr<Wt::WAnchor> anchor = Wt::cpp14::make_unique<Wt::WAnchor>();
//            anchor->setText("Başkan");
//            anchor->decorationStyle().setCursor(Cursor::PointingHand);
//            layout->addWidget(std::move(anchor),0,AlignmentFlag::Left);
//        }
//        {
//            std::unique_ptr<Wt::WAnchor> anchor = Wt::cpp14::make_unique<Wt::WAnchor>();
//            anchor->setText("Başkan Yardımcıları");
//            anchor->decorationStyle().setCursor(Cursor::PointingHand);
//            layout->addWidget(std::move(anchor),0,AlignmentFlag::Left);
//        }
//        {
//            std::unique_ptr<Wt::WAnchor> anchor = Wt::cpp14::make_unique<Wt::WAnchor>();
//            anchor->setText("Meclis Üyeleri");
//            anchor->decorationStyle().setCursor(Cursor::PointingHand);
//            layout->addWidget(std::move(anchor),0,AlignmentFlag::Left);
//        }
//        {
//            std::unique_ptr<Wt::WAnchor> anchor = Wt::cpp14::make_unique<Wt::WAnchor>();
//            anchor->setText("Komisyonlar");
//            anchor->decorationStyle().setCursor(Cursor::PointingHand);
//            layout->addWidget(std::move(anchor),0,AlignmentFlag::Left);
//        }
//        {
//            std::unique_ptr<Wt::WAnchor> anchor = Wt::cpp14::make_unique<Wt::WAnchor>();
//            anchor->setText("Encümen Üyeleri");
//            anchor->decorationStyle().setCursor(Cursor::PointingHand);
//            layout->addWidget(std::move(anchor),0,AlignmentFlag::Left);
//        }
//        {
//            std::unique_ptr<Wt::WAnchor> anchor = Wt::cpp14::make_unique<Wt::WAnchor>();
//            anchor->setText("Birimler");
//            anchor->decorationStyle().setCursor(Cursor::PointingHand);
//            layout->addWidget(std::move(anchor),0,AlignmentFlag::Left);
//        }

////        layout->addWidget(cpp14::make_unique<WText>("Başkan Yardımcıları"),0,AlignmentFlag::Left);
////        layout->addWidget(cpp14::make_unique<WText>("Meclis Üyeleri"),0,AlignmentFlag::Left);
////        layout->addWidget(cpp14::make_unique<WText>("Komisyonlar"),0,AlignmentFlag::Left);
////        layout->addWidget(cpp14::make_unique<WText>("Encümen Üyeleri"),0,AlignmentFlag::Left);
////        layout->addWidget(cpp14::make_unique<WText>("Birimler"),0,AlignmentFlag::Left);
//    }

//    {
//        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
//        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
//        container->setAttributeValue(Style::style,Style::color::rgb("255,255,255"));
//        auto layoutContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        auto layout = layoutContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//        auto title = layout->addWidget(cpp14::make_unique<WText>("Gündem"),0,AlignmentFlag::Left);
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Azure)+Style::font::size::s20px);

//        layout->addWidget(cpp14::make_unique<WText>("Haberler"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Duyurular"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Meclis Kararları"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Encümen Kararları"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Komisyon Kararları"),0,AlignmentFlag::Left);
//    }

//    {
//        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
//        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
//        container->setAttributeValue(Style::style,Style::color::rgb("255,255,255"));
//        auto layoutContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        auto layout = layoutContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//        auto title = layout->addWidget(cpp14::make_unique<WText>("Sosyal"),0,AlignmentFlag::Left);
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Azure)+Style::font::size::s20px);

//        layout->addWidget(cpp14::make_unique<WText>("Facebook"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Twitter"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("İnstagram"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Youtube"),0,AlignmentFlag::Left);
//    }

//    {
//        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
//        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
//        container->setAttributeValue(Style::style,Style::color::rgb("255,255,255"));
//        auto layoutContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        auto layout = layoutContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//        auto title = layout->addWidget(cpp14::make_unique<WText>("Bağlantılar"),0,AlignmentFlag::Left);
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Azure)+Style::font::size::s20px);

//        layout->addWidget(cpp14::make_unique<WText>("Serik BelediyeSpor"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Serik Genç Fikir"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Mogat"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Expo2016"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Ombudsmanlık"),0,AlignmentFlag::Left);
//    }

//    {
//        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
//        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
//        container->setAttributeValue(Style::style,Style::color::rgb("255,255,255"));
//        auto layoutContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        auto layout = layoutContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//        auto title = layout->addWidget(cpp14::make_unique<WText>("Projeler"),0,AlignmentFlag::Left);
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Azure)+Style::font::size::s20px);

//        layout->addWidget(cpp14::make_unique<WText>("Projeler"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Çalışmalar"),0,AlignmentFlag::Left);
//    }

//    {
//        auto container = row->addWidget(cpp14::make_unique<WContainerWidget>());
//        container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_4+Bootstrap::Grid::ExtraSmall::col_xs_12);
//        container->setAttributeValue(Style::style,Style::color::rgb("255,255,255"));
//        auto layoutContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
//        auto layout = layoutContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
//        auto title = layout->addWidget(cpp14::make_unique<WText>("İletişim"),0,AlignmentFlag::Left);
//        title->setAttributeValue(Style::style,Style::color::color(Style::color::White::Azure)+Style::font::size::s20px);

//        layout->addWidget(cpp14::make_unique<WText>("Adres"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Google Map"),0,AlignmentFlag::Left);
//        layout->addWidget(cpp14::make_unique<WText>("Çağrı Merkezi"),0,AlignmentFlag::Left);
//    }

    {
        auto mMainContainerWidget = SubMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        {
            auto container = mMainContainerWidget->addWidget(cpp14::make_unique<WContainerWidget>());
            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            layout->addWidget(cpp14::make_unique<WText>("Serik Belediye Başkanlığı"))->addStyleClass("footerText");
        }


        {
            auto container = mMainContainerWidget->addWidget(cpp14::make_unique<WContainerWidget>());
            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            auto text = layout->addWidget(cpp14::make_unique<WText>("Bu Web Sayfası Serik Belediyesi Bilgi İşlem Müdürlüğü Tarafından Açık Kaynak Kodlu Olarak Geliştirilmektedir."));
            text->addStyleClass("footerText");
        }

        {
            auto container = mMainContainerWidget->addWidget(cpp14::make_unique<WContainerWidget>());
            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

            layout->addStretch(1);
            layout->addWidget(cpp14::make_unique<WText>("© Copyright 2018  "))->addStyleClass("footerText");
            {
                Wt::WLink link = Wt::WLink("http://www.webtoolkit.eu/");
                link.setTarget(Wt::LinkTarget::NewWindow);

                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,"Wt");

                layout->addWidget(std::move(anchor));
            }
            layout->addWidget(cpp14::make_unique<WText>(","))->addStyleClass("footerText");
            {
                Wt::WLink link = Wt::WLink("http://www.qt.io/");
                link.setTarget(Wt::LinkTarget::NewWindow);

                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,"Qt");

                layout->addWidget(std::move(anchor));
            }
            layout->addWidget(cpp14::make_unique<WText>(WString::fromUTF8(" & "),TextFormat::Plain))->addStyleClass("footerText");
            {
                Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/SerikBelediyesiWebSayfasi");
                link.setTarget(Wt::LinkTarget::NewWindow);

                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,"gitHub");

                layout->addWidget(std::move(anchor));
            }
            layout->addStretch(1);

        }

        {
            auto container = mMainContainerWidget->addWidget(cpp14::make_unique<WContainerWidget>());
            auto layout = container->setLayout(cpp14::make_unique<WHBoxLayout>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);
            {
                Wt::WLink link = Wt::WLink("http://www.serik.bel.tr:7070");
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,"Kurum içi Portal");
                layout->addWidget(std::move(anchor),0,AlignmentFlag::Center);
            }
        }
    }
}

void Footer::Footer::showMessage(std::string title, std::string msg)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, Wt::StandardButton::Ok));

        messageBox->buttonClicked().connect([=] {
            this->removeChild(messageBox);
        });

        messageBox->show();
}
