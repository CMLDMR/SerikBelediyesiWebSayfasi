#include "stokkategoripage.h"

v2::StokKategoriPage::StokKategoriPage(DB *_db, SerikBLDCore::User *_mUser)
    :ContainerWidget ("Stok Kategori"),
      SerikBLDCore::Stok::StokKategoriManager (_db),
      mUser(_mUser)
{

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_10+
                                  Bootstrap::Grid::Medium::col_md_10+
                                  Bootstrap::Grid::Small::col_sm_10+
                                  Bootstrap::Grid::ExtraSmall::col_xs_10);
        mKategoriAdiLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mKategoriAdiLineEdit->setPlaceholderText ("Yeni Kategori Adını Giriniz");
        mKategoriAdiLineEdit->setWidth (WLength("100%"));
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
        container->setContentAlignment (AlignmentFlag::Center);
        auto svBtn = container->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
        svBtn->addStyleClass (Bootstrap::Button::Primary);

        svBtn->clicked ().connect ([=](){

            if( !mKategoriAdiLineEdit->text ().toUTF8 ().size () )
            {
                this->errorOccured ("Kategori Adını Girmediniz");
                return;
            }


            SerikBLDCore::Stok::StokKategori kategori(mUser->Birimi ());
            kategori.setKategoriName (mKategoriAdiLineEdit->text ().toUTF8 ());

            auto countItem = this->countItem (kategori);

            if( countItem != 0 )
            {
                this->errorOccured ("Bu Kategoriden Mevcut");
            }else{
                if( this->InsertItem (kategori).size () )
                {
                    this->UpdateList (SerikBLDCore::Stok::StokKategori(mUser->Birimi ()));
                    this->showPopUpMessage ("Kategori Eklendi");
                }
            }


        });

    }

    this->UpdateList (SerikBLDCore::Stok::StokKategori(mUser->Birimi ()));

}

void v2::StokKategoriPage::onList( const QVector<SerikBLDCore::Stok::StokKategori> *mlist )
{
        Content ()->clear ();
        for( auto item : *mlist )
        {
            auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                                      Bootstrap::Grid::Medium::col_md_11+
                                      Bootstrap::Grid::Small::col_sm_11+
                                      Bootstrap::Grid::ExtraSmall::col_xs_11);
            container->addWidget (cpp14::make_unique<WText>(item.kategoriName ()));
            container->addStyleClass ("boxRadius");
            container->setMargin (3,Side::Top|Side::Bottom);
            container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+Style::color::color (Style::color::Grey::Black));

            auto delContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            delContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                         Bootstrap::Grid::Medium::col_md_1+
                                         Bootstrap::Grid::Small::col_sm_1+
                                         Bootstrap::Grid::ExtraSmall::col_xs_1);
            delContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Crimson)+Style::color::color (Style::color::White::Snow));
            delContainer->addStyleClass ("boxRadius");
            delContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>",TextFormat::UnsafeXHTML));
            delContainer->decorationStyle ().setCursor (Cursor::PointingHand);
            delContainer->setMargin (3,Side::Top|Side::Bottom);
            delContainer->clicked ().connect ([=](){
                auto delBtn = this->askConfirm ("Silmek İstediğinize Emin misiniz?");
                delBtn->clicked ().connect ([=](){
                    SerikBLDCore::Stok::StokKategori filter(mUser->Birimi ());
                    filter.setOid (item.oid ().value ().to_string ());
                    if( !this->DeleteItem (filter) )
                    {

                    }else{
                        this->UpdateList (SerikBLDCore::Stok::StokKategori(mUser->Birimi ()));
                    }
                });
            });
        }
}

void v2::StokKategoriPage::errorOccured( const std::string &errorText )
{
    this->showPopUpMessage (errorText,"hata");
}

v2::StokKodPage::StokKodPage(SerikBLDCore::DB *_db)
    :ContainerWidget("Stok Birim "),
      SerikBLDCore::Stok::StokKodManager (_db)
{

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                  Bootstrap::Grid::Medium::col_md_5+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mKodAdiLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mKodAdiLineEdit->setPlaceholderText ("Yeni Birim Adını Giriniz");
        mKodAdiLineEdit->setWidth (WLength("100%"));
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                  Bootstrap::Grid::Medium::col_md_5+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        mBirimTipiLineEdit = container->addWidget (cpp14::make_unique<WLineEdit>());
        mBirimTipiLineEdit->setPlaceholderText ("Yeni Birim Tipini Giriniz (KG, TON, Adet...)");
        mBirimTipiLineEdit->setWidth (WLength("100%"));
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_12+
                                  Bootstrap::Grid::ExtraSmall::col_xs_12);
        container->setContentAlignment (AlignmentFlag::Center);
        auto svBtn = container->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
        svBtn->addStyleClass (Bootstrap::Button::Primary);

        svBtn->clicked ().connect ([=](){

            if( !mBirimTipiLineEdit->text ().toUTF8 ().size () )
            {
                this->errorOccured ("Birim Tipini Girmediniz");
                return;
            }

            if( !mKodAdiLineEdit->text ().toUTF8 ().size () )
            {
                this->errorOccured ("Birim Adını Girmediniz");
                return;
            }


            SerikBLDCore::Stok::StokKodItem birim;
            birim.setAdi (mBirimTipiLineEdit->text ().toUTF8 ());
            birim.setBirim (mKodAdiLineEdit->text ().toUTF8 ());

            auto countItem = this->countItem (birim);

            if( countItem != 0 )
            {
                this->errorOccured ("Bu Stok Biriminden Mevcut");
            }else{
                if( this->InsertItem (birim).size () )
                {
                    this->UpdateList (SerikBLDCore::Stok::StokKodItem());
                    this->showPopUpMessage ("Birim Eklendi");
                }
            }


        });

    }

    this->UpdateList (SerikBLDCore::Stok::StokKodItem());
}

void v2::StokKodPage::onList(const QVector<SerikBLDCore::Stok::StokKodItem> *mlist)
{
    Content ()->clear ();
    for( auto item : *mlist )
    {
        auto birimcontainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        birimcontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
        birimcontainer->addWidget (cpp14::make_unique<WText>(item.birim ()));
        birimcontainer->addStyleClass ("boxRadius");
        birimcontainer->setMargin (3,Side::Top|Side::Bottom);
        birimcontainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+Style::color::color (Style::color::Grey::Black));



        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                  Bootstrap::Grid::Medium::col_md_5+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5);
        container->addWidget (cpp14::make_unique<WText>(item.adi ()));
        container->addStyleClass ("boxRadius");
        container->setMargin (3,Side::Top|Side::Bottom);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::LightGray)+Style::color::color (Style::color::Grey::Black));



        auto delContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        delContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                     Bootstrap::Grid::Medium::col_md_1+
                                     Bootstrap::Grid::Small::col_sm_1+
                                     Bootstrap::Grid::ExtraSmall::col_xs_1);
        delContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Crimson)+Style::color::color (Style::color::White::Snow));
        delContainer->addStyleClass ("boxRadius");
        delContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>",TextFormat::UnsafeXHTML));
        delContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        delContainer->setMargin (3,Side::Top|Side::Bottom);
        delContainer->clicked ().connect ([=](){
            auto delBtn = this->askConfirm ("Silmek İstediğinize Emin misiniz?");
            delBtn->clicked ().connect ([=](){
                SerikBLDCore::Stok::StokKodItem filter;
                filter.setOid (item.oid ().value ().to_string ());
                if( !this->DeleteItem (filter) )
                {

                }else{
                    this->UpdateList (SerikBLDCore::Stok::StokKodItem());
                }
            });
        });
    }
}

void v2::StokKodPage::errorOccured(const std::string &errorText)
{
this->showPopUpMessage (errorText,"hata");
}
