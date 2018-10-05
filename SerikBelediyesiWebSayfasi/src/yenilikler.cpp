#include "yenilikler.h"



Giris::Personel::Yenilikler::Yenilikler()
{

        auto fContainer = addWidget(cpp14::make_unique<WContainerWidget>());

        fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        auto rContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());

        rContainer->addStyleClass(Bootstrap::Grid::row);


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::col_full_12);
            auto text = container->addWidget(cpp14::make_unique<WText>("<h3>Repository</h3>"));
            container->setMargin(10,Side::Bottom);
        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(25,Side::Bottom);

            // Create an anchor that links to a URL through clickable text.
            Wt::WLink link = Wt::WLink("https://github.com/CMLDMR?tab=repositories/");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "Serik Belediyesi Kod Repository");

            container->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● Kod Kütüphanesi</b>"));



            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            sContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            auto row = sContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);



            {
                auto listContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                listContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                listContainer->setList(true);
                auto text = listContainer->addWidget(cpp14::make_unique<WText>("▸ Kaynak Kodlar"));
            }
        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(25,Side::Bottom);

            // Create an anchor that links to a URL through clickable text.
            Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/SerikBelediyesiWebSayfasi/");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "Serik Belediyesi");

            container->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● Resmi Web Sayfası</b>"));



            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            sContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            auto row = sContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);



            {
                auto listContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                listContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                listContainer->setList(true);
                auto text = listContainer->addWidget(cpp14::make_unique<WText>("▸ Kaynak Kodlar"));
            }
        }


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(25,Side::Bottom);

            // Create an anchor that links to a URL through clickable text.
            Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/EArsiv/releases");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "EArşiv v0.1");

            container->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● EArşiv v0.1</b>"));



            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            sContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            auto row = sContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);



            {
                auto listContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                listContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                listContainer->setList(true);
                auto text = listContainer->addWidget(cpp14::make_unique<WText>("▸ Setup ve Kaynak Kodlar"));
            }
        }


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(25,Side::Bottom);

            // Create an anchor that links to a URL through clickable text.
            Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/SerikBelediyesiCrossApplication/releases");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "Cross Application v0.7");

            container->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● Cross Application v0.7</b>"));



            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            sContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            auto row = sContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);



            {
                auto listContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                listContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                listContainer->setList(true);
                auto text = listContainer->addWidget(cpp14::make_unique<WText>("▸ Setup ve Kaynak Kodlar"));
            }
        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(25,Side::Bottom);

            // Create an anchor that links to a URL through clickable text.
            Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/QMongoDB/releases");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "QMongoDB v0.5");

            container->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● Database Driver v0.5</b>"));



            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            sContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            auto row = sContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);



            {
                auto listContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                listContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                listContainer->setList(true);
                auto text = listContainer->addWidget(cpp14::make_unique<WText>("▸ Shared Libraries ve Kaynak Kodlar"));
            }
        }


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(25,Side::Bottom);

            // Create an anchor that links to a URL through clickable text.
            Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/SINKAR");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "Sinkar Stok Kasa ");

            container->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● Sinkar Stok Kasa</b>"));



            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            sContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            auto row = sContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);



            {
                auto listContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                listContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                listContainer->setList(true);
                auto text = listContainer->addWidget(cpp14::make_unique<WText>("▸ Kaynak Kodlar"));
            }
        }


        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(25,Side::Bottom);

            // Create an anchor that links to a URL through clickable text.
            Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/SerikBelediyeSporWeb");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "Serik Belediyesi Spor Web Sayfası");

            container->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● www.serikbelediyespor.com</b>"));



            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            sContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            auto row = sContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);



            {
                auto listContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                listContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                listContainer->setList(true);
                auto text = listContainer->addWidget(cpp14::make_unique<WText>("▸ Kaynak Kodlar"));
            }
        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(25,Side::Bottom);

            // Create an anchor that links to a URL through clickable text.
            Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/serikgencfikir.com");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "Serik Genç Fikir");

            container->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● www.serikgencfikir.com</b>"));



            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            sContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            auto row = sContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);



            {
                auto listContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                listContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                listContainer->setList(true);
                auto text = listContainer->addWidget(cpp14::make_unique<WText>("▸ Kaynak Kodlar"));
            }
        }

        {
            auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+Bootstrap::Grid::Medium::col_md_4+Bootstrap::Grid::Small::col_sm_6+Bootstrap::Grid::ExtraSmall::col_xs_12);
            container->setMargin(25,Side::Bottom);

            // Create an anchor that links to a URL through clickable text.
            Wt::WLink link = Wt::WLink("https://github.com/CMLDMR/MOGAT");
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "MOGAT");

            container->addWidget(cpp14::make_unique<WAnchor>(link,"<b>● MOGAT</b>"));



            auto sContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
            sContainer->addStyleClass(Bootstrap::Grid::container_fluid);
            auto row = sContainer->addWidget(cpp14::make_unique<WContainerWidget>());
            row->addStyleClass(Bootstrap::Grid::row);



            {
                auto listContainer = row->addWidget(cpp14::make_unique<WContainerWidget>());
                listContainer->addStyleClass(Bootstrap::Grid::col_full_12);
                listContainer->setList(true);
                auto text = listContainer->addWidget(cpp14::make_unique<WText>("▸ Kaynak Kodlar"));
            }
        }

    }
