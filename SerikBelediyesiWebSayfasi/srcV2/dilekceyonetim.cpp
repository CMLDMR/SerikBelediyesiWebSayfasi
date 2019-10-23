#include "dilekceyonetim.h"

DilekceYonetim::DilekceYonetim(mongocxx::database *_db, bsoncxx::document::value _user)
    : ContainerWidget ("Dilekçe Yönetim") , DilekceManager (_db) , User(_db,_user)
{
    this->initControlPanel ();

    this->_clickDilekceItem.connect ( this , &DilekceYonetim::initDilekce );
}

void DilekceYonetim::initControlPanel()
{

    this->Header ()->clear ();


    if( this->Statu () == User::Baskan || this->Statu () == User::BaskanYardimcisi )
    {
        this->Header ()->addWidget (cpp14::make_unique<WText>("Başkan ve Başkan Yardımcısı Menü"));
    }else if( this->Statu () == User::Mudur )
    {
        this->Header ()->addWidget (cpp14::make_unique<WText>("Müdür Menüsü"));
        this->initMudurPanel ();
    }else{
        this->Header ()->addWidget (cpp14::make_unique<WText>("Personel/Şef Menüsü Menüsü"));
    }

    this->Content ()->setMargin (15,Side::Top|Side::Bottom);

}



void DilekceYonetim::initMudurPanel()
{
    this->Header ()->clear ();

    {
        auto btn = createButton ("Gelen Dilekçeler",Style::color::Green::DarkCyan,Style::color::White::Snow);
        btn->clicked ().connect ([&](){
            Dilekce filter;
            filter.SetBirim (this->Birimi ().c_str ());
            this->listDilekce (filter);
        });
        this->Header ()->addWidget (std::move(btn));
    }

    {
        auto btn = createButton ("Bilgilendirmeler",Style::color::Green::SeaGreen,Style::color::White::Snow);
        this->Header ()->addWidget (std::move(btn));
    }


    {
        auto btn = createButton ("Cevaplananlar",Style::color::Green::Lime,Style::color::Grey::Black);
        this->Header ()->addWidget (std::move(btn));
    }

    {
        auto btn = createButton ("İstatistik",Style::color::Green::LawnGreen,Style::color::Grey::DarkSlateGray);
        this->Header ()->addWidget (std::move(btn));
    }


}



std::unique_ptr<ContainerWidget> DilekceYonetim::createButton(const std::string &buttonName , const std::string &backGroundColor , const std::string &textColor )
{

        auto bilgiContainer = (cpp14::make_unique<ContainerWidget>());
        bilgiContainer->setAttributeValue (Style::style,Style::background::color::color (backGroundColor));
        auto textbilgi = bilgiContainer->addWidget (cpp14::make_unique<WText>(buttonName));
        textbilgi->setAttributeValue (Style::style,Style::color::color (textColor));
        bilgiContainer->setContentAlignment (AlignmentFlag::Center|AlignmentFlag::Middle);
        bilgiContainer->setHeight (30);
        bilgiContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                       +Bootstrap::Grid::Medium::col_md_3
                                       +Bootstrap::Grid::Small::col_sm_3
                                       +Bootstrap::Grid::ExtraSmall::col_xs_6
                                       +Bootstrap::ImageShape::img_thumbnail);
        bilgiContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        return bilgiContainer;
}

void DilekceYonetim::listDilekce(Dilekce &filterDilekce)
{

    this->Content ()->clear ();

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);

        auto rContainer = container->addWidget (cpp14::make_unique<ContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);
        rContainer->setWidth (WLength("100%"));
        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>#</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_1
                                       +Bootstrap::Grid::Medium::col_md_1
                                       +Bootstrap::Grid::Small::col_sm_1
                                       +Bootstrap::Grid::ExtraSmall::col_xs_1);
        }

        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>Konu</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_5
                                       +Bootstrap::Grid::Medium::col_md_5
                                       +Bootstrap::Grid::Small::col_sm_5
                                       +Bootstrap::Grid::ExtraSmall::col_xs_5);
        }

        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>Tarih</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                       +Bootstrap::Grid::Medium::col_md_3
                                       +Bootstrap::Grid::Small::col_sm_3
                                       +Bootstrap::Grid::ExtraSmall::col_xs_3);
        }

        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>Sayı</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                       +Bootstrap::Grid::Medium::col_md_3
                                       +Bootstrap::Grid::Small::col_sm_3
                                       +Bootstrap::Grid::ExtraSmall::col_xs_3);
        }
    }

    auto cursor = this->findDilekce (filterDilekce);

    int count = 1;
    for( auto item : cursor )
    {

        auto POid = item.oid ();
        if( POid )
        {

            auto container = this->Content ()->addWidget (cpp14::make_unique<ContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::col_full_12);

            auto rContainer = container->addWidget (cpp14::make_unique<ContainerWidget>());
            rContainer->addStyleClass (Bootstrap::Grid::row);
            rContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            rContainer->setWidth (WLength("100%"));
            rContainer->decorationStyle ().setCursor (Cursor::PointingHand);
            rContainer->setAttributeValue (Style::dataoid,POid->to_string ());

            rContainer->clicked ().connect ([=](){
                _clickDilekceItem.emit (rContainer->attributeValue (Style::dataoid).toUTF8 ());
            });

            {
                auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                _container->addWidget (cpp14::make_unique<WText>("<b>#"+std::to_string (count++)+"</b>"));
                _container->addStyleClass (Bootstrap::Grid::Large::col_lg_1
                                           +Bootstrap::Grid::Medium::col_md_1
                                           +Bootstrap::Grid::Small::col_sm_1
                                           +Bootstrap::Grid::ExtraSmall::col_xs_1);
            }

            {
                auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                _container->addWidget (cpp14::make_unique<WText>(item.konu ().toStdString ()));
                _container->addStyleClass (Bootstrap::Grid::Large::col_lg_5
                                           +Bootstrap::Grid::Medium::col_md_5
                                           +Bootstrap::Grid::Small::col_sm_5
                                           +Bootstrap::Grid::ExtraSmall::col_xs_5);
            }

            {
                auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                _container->addWidget (cpp14::make_unique<WText>(item.tarihText ().toStdString ()));
                _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                           +Bootstrap::Grid::Medium::col_md_3
                                           +Bootstrap::Grid::Small::col_sm_3
                                           +Bootstrap::Grid::ExtraSmall::col_xs_3);
            }

            {
                auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                _container->addWidget (cpp14::make_unique<WText>(std::to_string (item.sayi ())));
                _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                           +Bootstrap::Grid::Medium::col_md_3
                                           +Bootstrap::Grid::Small::col_sm_3
                                           +Bootstrap::Grid::ExtraSmall::col_xs_3);
            }


        }




    }


}

void DilekceYonetim::initDilekce(const std::string &dilekceOid)
{

    this->Content ()->clear ();

}











