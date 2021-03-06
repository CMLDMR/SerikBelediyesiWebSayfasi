#include "dilekceyonetim.h"
#include "dilekce/dilekceview.h"
#include "dilekce/dilekcelistwidgetitem.h"

using namespace SerikBLDCore;


DilekceYonetim::DilekceYonetim(mongocxx::database *_db, bsoncxx::document::value _user)
    : ContainerWidget ("Dilekçe Yönetim") , DilekceManager (_db) , mUser(new User(_db,_user))
{
    this->initControlPanel ();
    this->_clickDilekceItem.connect ( this , &DilekceYonetim::initDilekce );
    this->_clickDilekceBilgiItem.connect ( this , &DilekceYonetim::initBilgiDilekce );
}

void DilekceYonetim::initControlPanel()
{

    this->Header ()->clear ();


    if( this->mUser->Statu () == User::Baskan || this->mUser->Statu () == User::BaskanYardimcisi )
    {
        this->Header ()->addWidget (cpp14::make_unique<WText>("Başkan ve Başkan Yardımcısı Menü"));
    }else if( this->mUser->Statu () == User::Mudur )
    {
        this->Header ()->addWidget (cpp14::make_unique<WText>("Müdür Menüsü"));
        this->initMudurPanel ();
    }else{
        this->Header ()->addWidget (cpp14::make_unique<WText>("Personel/Şef Menüsü Menüsü"));
        this->initPersonelPanel ();
    }

    this->Content ()->setMargin (15,Side::Top|Side::Bottom);

}


// Personel : 05065644692
// Müdür    : 05063022484

void DilekceYonetim::initMudurPanel()
{
    this->Header ()->clear ();

    {
        auto btn = createButton ("Gelen Dilekçeler",Style::color::Green::DarkCyan,Style::color::White::Snow);
        btn->clicked ().connect ([&](){
            Dilekce filter;
            filter.SetBirim (this->mUser->Birimi ().c_str ());
            filter.SetDurum (DilekceDurum::Acik.c_str ());
            this->listDilekce (filter);
        });
        this->Header ()->addWidget (std::move(btn));
    }

    {
        auto btn = createButton ("Bilgilendirmeler",Style::color::Green::SeaGreen,Style::color::White::Snow);
        btn->clicked ().connect ([&](){
            Dilekce filter;
            auto doc = make_document(kvp(Dilekce::KeyBilgiBirimler,make_document(kvp("$elemMatch",make_document(kvp("$eq",this->mUser->Birimi ()))))));
            filter.setDocumentView (doc.view ());
            this->listBilgiDilekce (filter);
        });
        this->Header ()->addWidget (std::move(btn));
    }


    {
        auto btn = createButton ("Cevaplananlar",Style::color::Green::Lime,Style::color::Grey::Black);
        btn->clicked ().connect ([&](){
            Dilekce filter;
            filter.SetBirim (this->mUser->Birimi ().c_str ());
            //TODO: Bu Kod Açılacak
            filter.SetDurum (DilekceDurum::Cevaplandi.c_str ());
            this->listDilekce (filter);
        });
        this->Header ()->addWidget (std::move(btn));
    }

    {
        auto btn = createButton ("İstatistik",Style::color::Green::LawnGreen,Style::color::Grey::DarkSlateGray);
        btn->clicked ().connect ([&](){
            this->showMessage ("Yapım Aşamasında","Dilekçe ile İlgili İstatistik Bilgileri Gösterilecek");
        });
        this->Header ()->addWidget (std::move(btn));
    }


}

void DilekceYonetim::initPersonelPanel()
{
    this->Header ()->clear ();

    {
        auto btn = createButton ("Gelen Dilekçeler",Style::color::Green::DarkCyan,Style::color::White::Snow);
        btn->clicked ().connect ([&](){
            Dilekce filter;
            filter.append(Dilekce::KeyGorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ()),
                                                                                                   kvp("ad soyad",this->mUser->AdSoyad ()),
                                                                                                   kvp("fotooid",this->mUser->element ("fotooid").value ())))));
            filter.SetBirim (this->mUser->Birimi ().c_str ());
            //TODO: Bu Kod Açılacak
            filter.SetDurum (DilekceDurum::Acik.c_str ());



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
        btn->clicked ().connect ([&](){
            Dilekce filter;
            filter.append(Dilekce::KeyGorevliPersonel,make_document(kvp("$elemMatch",make_document(kvp("_id",this->mUser->oid ().value ()),
                                                                                                   kvp("ad soyad",this->mUser->AdSoyad ()),
                                                                                                   kvp("fotooid",this->mUser->element ("fotooid").value ())))));
            filter.SetBirim (this->mUser->Birimi ().c_str ());
            //TODO: Bu Kod Açılacak
            filter.SetDurum (DilekceDurum::Cevaplandi.c_str ());
            this->listDilekce (filter);
        });

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
            _container->addWidget (cpp14::make_unique<WText>("<b>Konu</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                       +Bootstrap::Grid::Medium::col_md_3
                                       +Bootstrap::Grid::Small::col_sm_6
                                       +Bootstrap::Grid::ExtraSmall::col_xs_6);
        }

        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>Tarih</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                       +Bootstrap::Grid::Medium::col_md_3
                                       +Bootstrap::Grid::Small::col_sm_6
                                       +Bootstrap::Grid::ExtraSmall::col_xs_6);
        }

        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>Sayı</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                       +Bootstrap::Grid::Medium::col_md_3
                                       +Bootstrap::Grid::Small::col_sm_6
                                       +Bootstrap::Grid::ExtraSmall::col_xs_6);
        }

        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>Durum</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                       +Bootstrap::Grid::Medium::col_md_3
                                       +Bootstrap::Grid::Small::col_sm_6
                                       +Bootstrap::Grid::ExtraSmall::col_xs_6);
        }
    }


    auto cursor = this->findDilekce (filterDilekce);

    for( auto item : cursor )
    {

        auto dItem = this->Content ()->addWidget (cpp14::make_unique<DilekceListWidgetItem>(item));
        dItem->addStyleClass (Bootstrap::Grid::col_full_12);
        dItem->ClickDilekceWidget ().connect ([=](const std::string& oid){
            _clickDilekceItem.emit (oid);
        });
    }


}

void DilekceYonetim::listBilgiDilekce(const Dilekce &filterDilekce)
{

    this->Content ()->clear ();


    /// Header SATIR
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

    for( auto item : cursor )
    {

        auto dItem = this->Content ()->addWidget (cpp14::make_unique<DilekceListWidgetItem>(item));
        dItem->addStyleClass (Bootstrap::Grid::col_full_12);
        dItem->ClickDilekceWidget ().connect ([=](const std::string& oid){
            _clickDilekceBilgiItem.emit (oid);
        });

    }
}

void DilekceYonetim::initDilekce(const std::string &dilekceOid)
{
    auto dilekce = this->LoadDilekce (dilekceOid);
    if( dilekce )
    {
        this->Content ()->clear ();
        auto dilekceView = this->Content ()->addWidget (cpp14::make_unique<DilekceView>(dilekce.get ()
                                                                                        ,this->db ()
                                                                                        ,this->mUser
                                                                                        ,false
                                                                                        ,false));

        dilekceView->ClickKapatildi ().connect (this,&DilekceYonetim::initControlPanel );


    }else{
        this->showMessage ("Hata","Bu Dilekçe Yüklenemedi");
    }
}

void DilekceYonetim::initBilgiDilekce(const std::string &dilekceOid)
{
    auto dilekce = this->LoadDilekce (dilekceOid);
    if( dilekce )
    {
        this->Content ()->clear ();
        std::cout << "CALL initBilgiDilekce" << std::endl;
        auto dilekceView = this->Content ()->addWidget (cpp14::make_unique<DilekceView>(dilekce.get ()
                                                                                        ,this->db ()
                                                                                        ,this->mUser
                                                                                        ,false
                                                                                        ,true));
    }else{
        this->showMessage ("Hata","Bu Dilekçe Yüklenemedi");
    }
}











