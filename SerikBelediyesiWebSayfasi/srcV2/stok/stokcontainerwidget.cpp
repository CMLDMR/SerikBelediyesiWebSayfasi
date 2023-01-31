#include "stokcontainerwidget.h"



v2::StokWidget::StokContainerWidget::StokContainerWidget(SerikBLDCore::User *_mUser)
    :ContainerWidget ("Stok Yönetimi / "+_mUser->Birimi ()),
      SerikBLDCore::Stokv2::Stokv2Manager (_mUser->getDB ()),
      mUser(_mUser),
      mPersonelManager(new SerikBLDCore::PersonelManager(_mUser->getDB ()))
//      mYerManager(new SerikBLDCore::Stokv2::Stokv2YerManager(_mUser->getDB ()))
{
    this->initMenuBar ();
}

void v2::StokWidget::StokContainerWidget::initMenuBar()
{

    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addWidget (cpp14::make_unique<WText>("<b>Son Durum</b>"));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_3+
                                  CSSStyle::Button::blueButton);
        container->setPadding (10,Side::Top|Side::Bottom);
        container->clicked ().connect ([=](){
            mCurrentPage = CurrentPage::MalzemeCikisPage;
            this->initlastState ();
        });
    }



    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addWidget (cpp14::make_unique<WText>("Malzeme Girişi"));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_3+
                                  CSSStyle::Button::blueButton);
        container->setPadding (10,Side::Top|Side::Bottom);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect ([=](){
            mCurrentPage = CurrentPage::MalzemeGirisPage;
            SerikBLDCore::Stokv2::Giris filter;
            filter.setMudurluk (mUser->Birimi ());
            this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::UpdateList (filter);

            this->Footer ()->clear ();
            this->Footer ()->setMargin (15,Side::Top|Side::Bottom);

            auto backContainer = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
            backContainer->addStyleClass (Bootstrap::Button::Primary);
            backContainer->addWidget (cpp14::make_unique<WText>("Geri"));
            backContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                          Bootstrap::Grid::Medium::col_md_5+
                                          Bootstrap::Grid::Small::col_sm_5+
                                          Bootstrap::Grid::ExtraSmall::col_xs_5);
            backContainer->setPadding(5,Side::Top|Side::Bottom);
            backContainer->decorationStyle ().setCursor (Cursor::PointingHand);



            auto pageContainer = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
            pageContainer->addStyleClass (Bootstrap::Button::info);

            auto currentPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::currentPage (filter);
            auto totalPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::totalPage (filter);


            auto pageText = pageContainer->addWidget (cpp14::make_unique<WText>(WString("{1}/{2}").arg (currentPage+1).arg(totalPage+1)));
            pageContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                          Bootstrap::Grid::Medium::col_md_2+
                                          Bootstrap::Grid::Small::col_sm_2+
                                          Bootstrap::Grid::ExtraSmall::col_xs_2);
            pageContainer->setPadding(5,Side::Top|Side::Bottom);


            auto nextContainer = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
            nextContainer->addStyleClass (Bootstrap::Button::Primary);
            nextContainer->addWidget (cpp14::make_unique<WText>("İleri"));
            nextContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                          Bootstrap::Grid::Medium::col_md_5+
                                          Bootstrap::Grid::Small::col_sm_5+
                                          Bootstrap::Grid::ExtraSmall::col_xs_5);
            nextContainer->setPadding(5,Side::Top|Side::Bottom);
            nextContainer->decorationStyle ().setCursor (Cursor::PointingHand);




            backContainer->clicked ().connect ([=](){
                SerikBLDCore::Stokv2::Giris filter;
                filter.setMudurluk (mUser->Birimi ());
                this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::back (filter);

                auto currentPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::currentPage (filter);
                auto totalPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::totalPage (filter);

                pageText->setText (WString("{1}/{2}").arg (currentPage+1).arg(totalPage+1));
            });



            nextContainer->clicked ().connect ([=](){
                SerikBLDCore::Stokv2::Giris filter;
                filter.setMudurluk (mUser->Birimi ());
                this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::next (filter);

                auto currentPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::currentPage (filter);
                auto totalPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::totalPage (filter);

                pageText->setText (WString("{1}/{2}").arg (currentPage+1).arg(totalPage+1));
            });

        });
    }



    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addWidget (cpp14::make_unique<WText>("Malzeme Çıkışı"));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_3+
                                  CSSStyle::Button::blueButton);
        container->setPadding (10,Side::Top|Side::Bottom);
        container->clicked ().connect ([=](){
            mCurrentPage = CurrentPage::MalzemeCikisPage;
            SerikBLDCore::Stokv2::Cikis filter;
            filter.setMudurluk (mUser->Birimi ());
            this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::UpdateList (filter);


            this->Footer ()->clear ();
            this->Footer ()->setMargin (15,Side::Top|Side::Bottom);

            auto backContainer = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
            backContainer->addStyleClass (Bootstrap::Button::Primary);
            backContainer->addWidget (cpp14::make_unique<WText>("Geri"));
            backContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                          Bootstrap::Grid::Medium::col_md_5+
                                          Bootstrap::Grid::Small::col_sm_5+
                                          Bootstrap::Grid::ExtraSmall::col_xs_5);
            backContainer->setPadding(5,Side::Top|Side::Bottom);
            backContainer->decorationStyle ().setCursor (Cursor::PointingHand);




            auto pageContainer = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
            pageContainer->addStyleClass (Bootstrap::Button::info);

            auto currentPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::currentPage (filter);
            auto totalPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::totalPage (filter);


            auto pageText = pageContainer->addWidget (cpp14::make_unique<WText>(WString("{1}/{2}").arg (currentPage+1).arg(totalPage+1)));
            pageContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                          Bootstrap::Grid::Medium::col_md_2+
                                          Bootstrap::Grid::Small::col_sm_2+
                                          Bootstrap::Grid::ExtraSmall::col_xs_2);
            pageContainer->setPadding(5,Side::Top|Side::Bottom);




            auto nextContainer = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
            nextContainer->addStyleClass (Bootstrap::Button::Primary);
            nextContainer->addWidget (cpp14::make_unique<WText>("İleri"));
            nextContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                          Bootstrap::Grid::Medium::col_md_5+
                                          Bootstrap::Grid::Small::col_sm_5+
                                          Bootstrap::Grid::ExtraSmall::col_xs_5);
            nextContainer->setPadding(5,Side::Top|Side::Bottom);
            nextContainer->decorationStyle ().setCursor (Cursor::PointingHand);




            backContainer->clicked ().connect ([=](){
                SerikBLDCore::Stokv2::Cikis filter;
                filter.setMudurluk (mUser->Birimi ());
                this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::back (filter);

                auto currentPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::currentPage (filter);
                auto totalPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::totalPage (filter);

                pageText->setText (WString("{1}/{2}").arg (currentPage+1).arg(totalPage+1));
            });



            nextContainer->clicked ().connect ([=](){
                SerikBLDCore::Stokv2::Cikis filter;
                filter.setMudurluk (mUser->Birimi ());
                this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::next (filter);

                auto currentPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::currentPage (filter);
                auto totalPage = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::totalPage (filter);

                pageText->setText (WString("{1}/{2}").arg (currentPage+1).arg(totalPage+1));
            });


        });
    }

        {
            auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addWidget (cpp14::make_unique<WText>("İstatistik"));
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_3+
                                      Bootstrap::Grid::ExtraSmall::col_xs_3+
                                      CSSStyle::Button::blueButton);
            container->setPadding (10,Side::Top|Side::Bottom);
            container->clicked().connect([=](){
                this->showPopUpMessage("Modul Daha Sonra Eklenecek","warn");
            });
        }



    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addWidget (cpp14::make_unique<WText>("Ayarlar"));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_3+
                                  CSSStyle::Button::blueButton);
        container->setPadding (10,Side::Top|Side::Bottom);
        container->clicked ().connect (this,&v2::StokWidget::StokContainerWidget::initAyarlar);
    }

    if( /*this->mUser->Birimi () == "Park ve Bahçeler Müdürlüğü"*/ true ){
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addWidget (cpp14::make_unique<WText>("Yerler"));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_3+
                                  CSSStyle::Button::blueButton);
        container->setPadding (10,Side::Top|Side::Bottom);
        container->clicked ().connect (this,&v2::StokWidget::StokContainerWidget::initYerler);
    }



    mSubMenuBarContainer = this->Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
    mSubMenuBarContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    mSubMenuBarContainer->setMargin (15,Side::Top|Side::Bottom);
    mSubMenuBarContainer->setPadding (10,Side::Bottom|Side::Top);

}

void v2::StokWidget::StokContainerWidget::onList(const QVector<SerikBLDCore::Stokv2::Stok> *mlist)
{
    this->Content ()->clear ();
    mSubMenuBarContainer->clear ();



    switch (mCurrentPage) {
    case CurrentPage::MalzemeGirisPage:
        this->initimportMenu ();
        break;

    case CurrentPage::MalzemeCikisPage:
        this->initexportMenu ();
        break;

    case CurrentPage::SonDurumPage:
        this->initlastState ();
        break;

    case CurrentPage::Ayarlar:
        break;

    case CurrentPage::Yerler:
        break;
    default:
        break;
    }


    for( auto item : *mlist ){
        auto container = this->Content ()->addWidget (cpp14::make_unique<MalzemeGirisWidget>(item));
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->asListItem ();
    }




}

void v2::StokWidget::StokContainerWidget::onList(const QVector<SerikBLDCore::Stokv2::Kategori> *mlist)
{

    this->Content ()->clear ();

    auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
    auto titleText = container->addWidget (cpp14::make_unique<WText>("<b>Kategoriler</b>"));
    container->setPadding (5,Side::Top|Side::Bottom);
    container->setMargin (15,Side::Top|Side::Bottom);
    container->addStyleClass (Bootstrap::Grid::col_full_12+
                              CSSStyle::Gradient::blueGradient+
                              CSSStyle::Shadows::shadow8px);


    for( auto item : *mlist ){

        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto text = container->addWidget (cpp14::make_unique<WText>(item.getKategoriAdi ()));
        container->setPadding (5,Side::Top|Side::Bottom);
        container->setMargin (5,Side::Top|Side::Bottom);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                  Bootstrap::Grid::Medium::col_md_6+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6+
                                  CSSStyle::Gradient::blueGradient+
                                  CSSStyle::Shadows::shadow8px);

        auto metriccontainer = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto metrictext = metriccontainer->addWidget (cpp14::make_unique<WText>(item.getMetric ()));
        metriccontainer->setPadding (5,Side::Top|Side::Bottom);
        metriccontainer->setMargin (5,Side::Top|Side::Bottom);

        metriccontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_5+
                                        Bootstrap::Grid::Medium::col_md_5+
                                        Bootstrap::Grid::Small::col_sm_5+
                                        Bootstrap::Grid::ExtraSmall::col_xs_5+
                                        CSSStyle::Gradient::blueGradient+
                                        CSSStyle::Shadows::shadow8px);

        auto delcontainer = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto deltext = delcontainer->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
        delcontainer->setPadding (5,Side::Top|Side::Bottom);
        delcontainer->setMargin (5,Side::Top|Side::Bottom);
        delcontainer->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());

        delcontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                     Bootstrap::Grid::Medium::col_md_1+
                                     Bootstrap::Grid::Small::col_sm_1+
                                     Bootstrap::Grid::ExtraSmall::col_xs_1+
                                     CSSStyle::Gradient::redGradient+
                                     CSSStyle::Shadows::shadow8px);
        delcontainer->decorationStyle ().setCursor (Cursor::PointingHand);

        delcontainer->clicked ().connect ([=](){

            auto askDialog = this->askConfirm ("Silmek İstediğinize Eminmisiniz?");

            askDialog->clicked ().connect ([=](){

                SerikBLDCore::Stokv2::Stok stokfilter;
                stokfilter.setKategoriOid (delcontainer->attributeValue (Style::dataoid).toUTF8 ());

                auto counter = this->SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::countItem (stokfilter);

                if( counter ){
                    this->showPopUpMessage ("Bu Kategoride Malzeme Girişi/Çıkışı Bulunmaktadır.","err");
                    return;
                }

                SerikBLDCore::Stokv2::Kategori filter;
                filter.setOid (delcontainer->attributeValue (Style::dataoid).toUTF8 ());

                auto del = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Kategori>::DeleteItem (filter);

                if( !del ){
                    this->warnDialog ("Silinemedi");
                }else{
                    this->informDialog ("Silindi");
                    filter.clear ();
                    filter.setBirim (mUser->Birimi ());
                    this->updateStokKategoriList (filter,SerikBLDCore::FindOptions().setLimit(99999));
                }

            });


        });


    }
}

void v2::StokWidget::StokContainerWidget::errorOccured(const std::string &errorText)
{

}

void v2::StokWidget::StokContainerWidget::initAyarlar()
{
    mCurrentPage = CurrentPage::MalzemeGirisPage;

    this->Content ()->clear ();
    mSubMenuBarContainer->clear ();
    this->Footer ()->clear ();



    {
        auto container = mSubMenuBarContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addWidget (cpp14::make_unique<WText>("Listele"));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_3+
                                  CSSStyle::Button::blueButton);
        container->setPadding (10,Side::Top|Side::Bottom);
        container->clicked ().connect ([=](){
            SerikBLDCore::Stokv2::Kategori item;
            item.clear ();
            item.setBirim (mUser->Birimi ());
            this->updateStokKategoriList (item,SerikBLDCore::FindOptions().setLimit (9999));
            this->Footer ()->clear ();


        });
    }

    {
        auto container = mSubMenuBarContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addWidget (cpp14::make_unique<WText>("Yeni Kategori"));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_3+
                                  Bootstrap::Grid::ExtraSmall::col_xs_3+
                                  CSSStyle::Button::Red::IndianRedButton);
        container->setPadding (10,Side::Top|Side::Bottom);
        container->clicked ().connect ([=](){
            auto mDialog = this->createDialog ("Yeni Kategori/Kalem Oluştur");


            auto hLayoutContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
            hLayoutContainer->addStyleClass (Bootstrap::Grid::container_fluid);


            auto hLayout = hLayoutContainer->setLayout (cpp14::make_unique<WHBoxLayout>());
            auto adiLineEdit = hLayout->addWidget (cpp14::make_unique<WLineEdit>());
            adiLineEdit->setPlaceholderText ("Yeni Kategori/Kalem Adı");

            auto metricLineEdit = hLayout->addWidget (cpp14::make_unique<WLineEdit>());
            metricLineEdit->setPlaceholderText ("Birimi ( Adet, KG, TON...)");

            auto warnContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
            warnContainer->addStyleClass (Bootstrap::Grid::container_fluid);
            auto warnText = warnContainer->addWidget (cpp14::make_unique<WText>(""));
            warnText->setAttributeValue (Style::style,Style::color::color (Style::color::Red::DarkRed));

            auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
            saveBtn->setDefault (true);

            saveBtn->clicked ().connect ([=](){

                if( adiLineEdit->text ().toUTF8 ().empty () ){
                    warnText->setText ("Kategori Adı Boş Olamaz");
                    return; }
                if( metricLineEdit->text ().toUTF8 ().empty () ){
                    warnText->setText ("Kategori Birimi Boş Olamaz");
                    return;
                }

                SerikBLDCore::Stokv2::Kategori item;
                item.setBirim (mUser->Birimi ());
                item.setKategoriAdi (adiLineEdit->text ().toUTF8 ());

                auto count = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Kategori>::countItem (item);

                if( count ){
                    warnText->setText ("Bu Kategori Zaten Var! Farklı Bir Kategori Adı Deneyiniz");
                    return;
                }




                item.setMetric (metricLineEdit->text ().toUTF8 ());


                auto ins = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Kategori>::InsertItem (item);

                if( ins.size () ){
                    item.clear ();
                    item.setBirim (mUser->Birimi ());
                    this->updateStokKategoriList (item,SerikBLDCore::FindOptions().setLimit (9999));
                    this->Footer ()->clear ();

                }

                this->removeDialog (mDialog);
            });
        });
    }

}

void v2::StokWidget::StokContainerWidget::initYerler()
{
    mCurrentPage = CurrentPage::Yerler;

    this->Content ()->clear ();
    mSubMenuBarContainer->clear ();
    this->Footer ()->clear ();


    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto titleText = container->addWidget (cpp14::make_unique<WText>("<b>Yerler</b>"));
        container->setPadding (5,Side::Top|Side::Bottom);
        container->setMargin (15,Side::Top|Side::Bottom);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
                                  Bootstrap::Grid::Medium::col_md_11+
                                  Bootstrap::Grid::Small::col_sm_11+
                                  Bootstrap::Grid::ExtraSmall::col_xs_10+
                                  CSSStyle::Gradient::blueGradient+
                                  CSSStyle::Shadows::shadow8px);
    }



    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto titleText = container->addWidget (cpp14::make_unique<WText>("<b>+</b>"));
        container->setPadding (5,Side::Top|Side::Bottom);
        container->setMargin (15,Side::Top|Side::Bottom);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
                                  Bootstrap::Grid::Medium::col_md_1+
                                  Bootstrap::Grid::Small::col_sm_1+
                                  Bootstrap::Grid::ExtraSmall::col_xs_2+
                                  CSSStyle::Button::redButton+
                                  CSSStyle::Shadows::shadow8px);

        container->decorationStyle ().setCursor (Cursor::PointingHand);

        container->clicked ().connect ([=](){

            auto mDialog = this->createFlatDialog("Yeni Yer Adı Ekle");

            auto yeniLineEdit = mDialog->Content()->addWidget (cpp14::make_unique<WLineEdit>());
            yeniLineEdit->setPlaceholderText ("Yeni Yer Adını Giriniz!");


//            auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
//            saveBtn->addStyleClass(Bootstrap::Button::Primary);

            mDialog->Accepted().connect([=](){

                if( yeniLineEdit->text ().toUTF8 ().size () < 3 ){
                    this->showPopUpMessage ("Yer Adı Yeterli Değil!");
                    return;
                }

//                SerikBLDCore::Stokv2::Yer yerItem = SerikBLDCore::Stokv2::Yer().setBirim (this->mUser->Birimi()).setYerAdi (yeniLineEdit->text ().toUTF8 ());

//                auto count = mYerManager->countItem (yerItem);

//                if( count ){
//                    warnDialog ("Bu Yer Adı Kayıtlı");
//                }else{
////                    auto ins = mYerManager->InsertItem (yerItem);

//                    if( ins.size () ){
//                        showPopUpMessage ("Kayıt Edildi");
//                        this->initYerler ();
//                        this->removeDialog (mDialog);
//                    }else{
//                        criticDialog ("Kayıt Edilemedi");
//                    }
//                }

            });

        });
    }



//    auto mlist = this->mYerManager->List (SerikBLDCore::Stokv2::Yer().setBirim (mUser->Birimi ()),SerikBLDCore::FindOptions().setLimit (9999));




//    for( auto item : mlist ){

//        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
//        auto text = container->addWidget (cpp14::make_unique<WText>(item.getYerAdi ()));
//        container->setPadding (5,Side::Top|Side::Bottom);
//        container->setMargin (5,Side::Top|Side::Bottom);
//        container->addStyleClass (Bootstrap::Grid::Large::col_lg_11+
//                                  Bootstrap::Grid::Medium::col_md_11+
//                                  Bootstrap::Grid::Small::col_sm_11+
//                                  Bootstrap::Grid::ExtraSmall::col_xs_11+
//                                  CSSStyle::Gradient::blueGradient+
//                                  CSSStyle::Shadows::shadow8px);


//        auto delcontainer = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
//        delcontainer->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());
//        auto deltext = delcontainer->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
//        delcontainer->setPadding (5,Side::Top|Side::Bottom);
//        delcontainer->setMargin (5,Side::Top|Side::Bottom);
//        delcontainer->setAttributeValue (Style::dataoid,item.oid ().value ().to_string ());

//        delcontainer->addStyleClass (Bootstrap::Grid::Large::col_lg_1+
//                                     Bootstrap::Grid::Medium::col_md_1+
//                                     Bootstrap::Grid::Small::col_sm_1+
//                                     Bootstrap::Grid::ExtraSmall::col_xs_1+
//                                     CSSStyle::Gradient::redGradient+
//                                     CSSStyle::Shadows::shadow8px);
//        delcontainer->decorationStyle ().setCursor (Cursor::PointingHand);

//        delcontainer->clicked ().connect ([=](){

//            auto askDialog = this->askConfirm ("Silmek İstediğinize Eminmisiniz?");

//            askDialog->clicked ().connect ([=](){
//                SerikBLDCore::Stokv2::Yer filter;
//                filter.setOid (delcontainer->attributeValue (Style::dataoid).toUTF8 ());

//                if( mYerManager->DeleteItem (filter) ){
//                    this->showPopUpMessage ("Silindi");
//                    this->initYerler ();
//                }else{
//                    warnDialog ("Silinemedi: " + mYerManager->getLastError ().toStdString ());
//                }
//            });
//        });
//    }

}

void v2::StokWidget::StokContainerWidget::importMazeleme()
{

    this->Content ()->clear ();

    auto mDialog = createFlatDialog("Yeni Malzeme Girişi");

    auto _kategoriList = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Kategori>::List (SerikBLDCore::Stokv2::Kategori().setBirim (mUser->Birimi ()),
                                                                                               SerikBLDCore::FindOptions ().setLimit (9999));

    auto girisObj = mDialog->Content()->addWidget (cpp14::make_unique<MalzemeGirisDialog>(this->mUser,_kategoriList));
    girisObj->addStyleClass (Bootstrap::Grid::container_fluid);

//    auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Girişi Yap"));
    mDialog->Accepted().connect ([=](){

        if( girisObj->getCurrentGirenMiktar () <= 0 ){
            criticDialog ("Giren Malzeme Miktarı <b>0</b> veya  <b>Altında</b> Olamaz");
            return;
        }

        if( std::abs (girisObj->getJulianDay () - WDate::currentDate ().toJulianDay ()) > 999 ){
            criticDialog ("Teslim Alım Tarihi Çok Eski. En Fazla 30 Gün");
            return;
        }

        if( girisObj->getCurrentGirenFiyat () <= 0 ){
            criticDialog ("Giren Malzeme Fiyatı <b>0</b> veya <b>Altında</b> Olamaz");
            return;
        }

        SerikBLDCore::Stokv2::Giris girisItem;
        girisItem.setKategoriOid (girisObj->getCurrentKategoriOid ());
        girisItem.setMalzemeAdi (girisObj->getCurrentMalzemeAdi ());
        girisItem.setMetric (girisObj->getCurruntMalzemeMetric ());
        girisItem.setMiktar (girisObj->getCurrentGirenMiktar ());
        girisItem.setTeslimOid (girisObj->getCurrentMalzemeGirenOid ());
        girisItem.setMudurluk (mUser->Birimi ());
        girisItem.setTeslimAlimJulianDay (girisObj->getJulianDay ());
//        girisItem.setBirimFiyat (girisObj->getCurrentGirenFiyat ());


        auto ins = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::InsertItem (girisItem);

        if( ins.size () ){
            removeDialog (mDialog);
            informDialog ("Malzeme Girişi Yapıldı");
        }else{
            removeDialog (mDialog);
            criticDialog ("Malzeme Girişi Yapılamadı");
        }


    });



}

void v2::StokWidget::StokContainerWidget::exportMalzeme(const double &maxMiktar, const PipeLineStokItem &item)
{

    auto mDialog = createDialog ("Yeni Malzeme Çıkışı");
    mDialog->setWidth (WLength(700));

    auto _kategoriList = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Kategori>::List (SerikBLDCore::Stokv2::Kategori().setBirim (mUser->Birimi ()),SerikBLDCore::FindOptions ().setLimit (99999));

    auto mContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
    mContainer->addStyleClass (Bootstrap::Grid::col_full_12);

    auto rContainer = mContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass (Bootstrap::Grid::row);

    auto nameContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    nameContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    nameContainer->setPadding (5,AllSides);
    auto nameText = nameContainer->addWidget (cpp14::make_unique<WText>(item.name + "Maximum Çıkış Miktari:"+std::to_string (item.miktarGiris)));

    auto oidContianer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    oidContianer->addStyleClass (Bootstrap::Grid::col_full_12);
    oidContianer->setPadding (5,AllSides);
    auto oidText = oidContianer->addWidget (cpp14::make_unique<WText>(item.kategoriOid));

    auto valueContianer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    oidContianer->addStyleClass (Bootstrap::Grid::col_full_12);
    oidContianer->setPadding (5,AllSides);
    auto valueSpinBox = valueContianer->addWidget (cpp14::make_unique<WDoubleSpinBox>());
    valueSpinBox->setMaximum (9999999);


    auto dateContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    dateContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    dateContainer->setPadding (5,AllSides);
    auto dateEdit = dateContainer->addWidget (cpp14::make_unique<WDateEdit>());

    auto yerContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    yerContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    yerContainer->setPadding (5,AllSides);
    auto yerComboBox = yerContainer->addWidget (cpp14::make_unique<WComboBox>());

//    auto __yerlist = mYerManager->List (SerikBLDCore::Stokv2::Yer().setBirim (mUser->Birimi ()));

//    for( const auto &item : __yerlist ){
//        yerComboBox->addItem (item.getYerAdi ());
//    }



    auto personelContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    personelContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    personelContainer->setPadding (5,AllSides);
    personelContainer->setHeight (200);
    personelContainer->setOverflow (Overflow::Scroll);
    auto personelRContainer = personelContainer->addWidget (cpp14::make_unique<WContainerWidget>());

    auto selectedPersonelNameContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    selectedPersonelNameContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    auto personelNameText = selectedPersonelNameContainer->addWidget (cpp14::make_unique<WText>(""));

    auto selectedPersonelOidContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    selectedPersonelOidContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    auto personelOid = selectedPersonelOidContainer->addWidget (cpp14::make_unique<WText>(""));


    {
        auto list = mPersonelManager->List (SerikBLDCore::IK::Personel().setBirim (mUser->Birimi ().c_str ()),SerikBLDCore::FindOptions().setLimit (1000));
        for( const auto &personelItem : list ){

            auto container = personelRContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                      Bootstrap::Grid::Medium::col_md_4+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_4+CSSStyle::Shadows::shadow8px);
            container->setMargin (5,Side::Top|Side::Bottom);
            container->decorationStyle ().setCursor (Cursor::PointingHand);
            auto text = container->addWidget (cpp14::make_unique<WText>(personelItem.AdSoyad ().toStdString ()));
            container->setPadding (3,AllSides);
            if( personelItem.statu ().toStdString ()== SerikBLDCore::IK::Statu::Mudur ){
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::DimGray)+Style::color::color (Style::color::White::AliceBlue));
            }else{
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Linen));
            }

            container->clicked ().connect ([=]() {
               personelNameText->setText (personelItem.AdSoyad ().toStdString ());
               personelOid->setText (personelItem.oid ().value ().to_string ());
            });

        }
    }


    auto aciklamaContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    aciklamaContainer->addStyleClass (Bootstrap::Grid::col_full_12);
    aciklamaContainer->setPadding (5,AllSides);
    aciklamaContainer->setHeight (150);
    auto aciklamaTextEdit = aciklamaContainer->addWidget(cpp14::make_unique<WTextArea>());
    aciklamaTextEdit->setHeight (150);





    auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet & Çık"));
    saveBtn->clicked ().connect ([=](){


        if( valueSpinBox->value () > maxMiktar ) {
            this->showPopUpMessage ("Kullanılacak Miktar Stoktakinden Fazla Olamaz","err");
            return;
        }

        if( valueSpinBox->value () <= 0 ){
            criticDialog ("Çıkan Malzeme Miktarı <b>0</b> Altında Olamaz");
            return;
        }

        if( aciklamaTextEdit->text ().toUTF8 ().size () <= 25 ){
            criticDialog ("Açıklama Yeterli Değil");
            return;
        }

        if( personelOid->text ().toUTF8 ().empty () ){
            criticDialog ("Personel Seçmediniz");
            return;
        }

        if( std::abs (dateEdit->date ().toJulianDay () - WDate::currentDate ().toJulianDay ()) > 999 ){
            criticDialog ("Teslim Edilen Tarih Çok Eski. En Fazla 30 Gün");
            return;
        }


        SerikBLDCore::Stokv2::Cikis cikisItem;
        cikisItem.setKategoriOid (item.kategoriOid);
        cikisItem.setMalzemeAdi (item.name);
        cikisItem.setMetric (item.metric);
        cikisItem.setMiktar (valueSpinBox->value ());
        cikisItem.setTeslimAlanPersonel (personelOid->text ().toUTF8 (),personelNameText->text ().toUTF8 ());
        cikisItem.setMudurluk (mUser->Birimi ());
        cikisItem.setTeslimEtJulianDay (dateEdit->date ().toJulianDay ());
        cikisItem.setAciklama (aciklamaTextEdit->text ().toUTF8 ());
//        cikisItem.setCikisYeri (yerComboBox->currentText ().toUTF8 ());


        auto ins = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::InsertItem (cikisItem);

        if( ins.size () ){
            removeDialog (mDialog);
            informDialog ("Malzeme Çıkışı Yapıldı");
            this->initlastState ();
        }else{
            removeDialog (mDialog);
            criticDialog ("Malzeme Çıkışı Yapılamadı");
        }


//        mDialog->setWidth (700);
    });

}

void v2::StokWidget::StokContainerWidget::initlastState()
{


   auto girisItemList = this->getGirisList ();
   auto cikisItemList = this->getCikisList ();


    auto kategoriList = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Kategori>::List (SerikBLDCore::Stokv2::Kategori().setBirim (this->mUser->Birimi ()),SerikBLDCore::FindOptions().setLimit (99999));


    std::for_each(girisItemList.begin (),girisItemList.end (),[&girisItemList,&kategoriList](PipeLineStokItem item){
//        LOG << item.kategoriOid <<  " : " << item.miktarGiris <<  " : " << item.name << " : " << item.metric << std::endl;
        for( auto kategoriItem : kategoriList ){
            if( item.kategoriOid == kategoriItem.oid ().value ().to_string () ){
                PipeLineStokItem newItem;
                newItem.setValues (item);
                newItem.metric = kategoriItem.getMetric ();
                newItem.name = kategoriItem.getKategoriAdi ();
                std::replace(girisItemList.begin (),girisItemList.end (),item,newItem);
                break;
            }
        }
    });


    std::for_each(cikisItemList.begin (),cikisItemList.end (),[&cikisItemList,&kategoriList](PipeLineStokItem item){
//        LOG << item.kategoriOid <<  " : " << item.miktarGiris <<  " : " << item.name << " : " << item.metric << std::endl;
        for( auto kategoriItem : kategoriList ){
            if( item.kategoriOid == kategoriItem.oid ().value ().to_string () ){
                PipeLineStokItem newItem;
                newItem.setValues (item);
                newItem.metric = kategoriItem.getMetric ();
                newItem.name = kategoriItem.getKategoriAdi ();
                std::replace(cikisItemList.begin (),cikisItemList.end (),item,newItem);
                break;
            }
        }
    });


    std::vector<PipeLineStokItem> lastStateList;


    std::transform(girisItemList.begin (),girisItemList.end (),std::back_inserter(lastStateList),[&cikisItemList](PipeLineStokItem item){

        PipeLineStokItem newItem;
        newItem.setValues (item);
        for( const auto &cikisItem : cikisItemList ){
            if( item.kategoriOid == cikisItem.kategoriOid ) {
                newItem.miktarGiris = newItem.miktarGiris - cikisItem.miktarCikis;
            }
        }
        return newItem;
    });

//    std::for_each(girisItemList.begin (),girisItemList.end (),[&cikisItemList,&lastStateList](PipeLineStokItem item){
//        for( const auto &cikisItem : cikisItemList ){
//            if( item.kategoriOid == cikisItem.kategoriOid ) {
//                PipeLineStokItem newItem;
//                newItem.setValues (item);
//                newItem.miktarGiris = newItem.miktarGiris - cikisItem.miktarCikis;
//                lastStateList.push_back (newItem);
//            }
//        }
//    });


//    std::for_each(girisItemList.begin (),girisItemList.end (),[&girisItemList,&cikisItemList](PipeLineStokItem item){
//        for( auto cikisItem : cikisItemList ){
//            if( item.kategoriOid == cikisItem.kategoriOid ) {
//                PipeLineStokItem newItem;
//                newItem.setValues (item);
//                newItem.miktarGiris = newItem.miktarGiris - cikisItem.miktarCikis;
//                std::replace(girisItemList.begin (),girisItemList.end (),item,newItem);
//                break;
//            }
//        }
//    });




//    std::for_each(girisItemList.begin (),girisItemList.end (),[](PipeLineStokItem item){
//        LOG << "Giris: "<<item.kategoriOid <<  " : " << item.miktarGiris <<  " : " << item.name << " : " << item.metric << std::endl;
//    });

//    std::for_each(cikisItemList.begin (),cikisItemList.end (),[](PipeLineStokItem item){
//        LOG << "Cikis: "<<item.kategoriOid <<  " : " << item.miktarGiris <<  " : " << item.name << " : " << item.metric << std::endl;
//    });

//    std::for_each(lastStateList.begin (),lastStateList.end (),[](PipeLineStokItem item){
//        LOG << "Last: "<<item.kategoriOid <<  " : " << item.miktarGiris <<  " : " << item.name << " : " << item.metric << std::endl;
//    });

    this->Content ()->clear ();


    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setPadding (10,AllSides);

        auto titleText = container->addWidget (cpp14::make_unique<WText>("Malzeme Son Durum"));
        titleText->setMargin (10,Side::Top|Side::Bottom);
        container->addStyleClass (CSSStyle::Gradient::blueGradient);
    }


    for(const auto &item : lastStateList ){

        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setPositionScheme (PositionScheme::Relative);
        container->setHeight (25);
        container->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (150,180),this->getRandom (150,180),this->getRandom (150,180))+
                                      Style::color::color (Style::color::White::AliceBlue));
        container->setMargin (10,Side::Top);
        container->addStyleClass (CSSStyle::Shadows::shadow8px);

//        auto percentBar = container->addWidget (cpp14::make_unique<WContainerWidget>());
//        percentBar->setHeight (25);
//        percentBar->setOffsets (0,Side::Top|Side::Left);
//        percentBar->setWidth (WLength(WString("{1}%").arg(item.miktarGiris).toUTF8 ()));
//        percentBar->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (),this->getRandom (),this->getRandom ()));

        auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->setPositionScheme (PositionScheme::Absolute);
        rContainer->setWidth (WLength("100%"));
        rContainer->setHeight (WLength("100%"));
        rContainer->setOffsets (0,Side::Top|Side::Left);
        rContainer->addStyleClass (Bootstrap::Grid::row);


        {
            auto nameContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            nameContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                          Bootstrap::Grid::Medium::col_md_3+
                                          Bootstrap::Grid::Small::col_sm_3+
                                          Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto nametext = nameContainer->addWidget (cpp14::make_unique<WText>(item.name));
            nameContainer->setHeight (25);
        }


        {
            auto nameContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            nameContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                          Bootstrap::Grid::Medium::col_md_3+
                                          Bootstrap::Grid::Small::col_sm_3+
                                          Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto nametext = nameContainer->addWidget (cpp14::make_unique<WText>("Kalan Miktar: "+std::to_string (item.miktarGiris)+" "+item.metric));
            nameContainer->setHeight (25);
        }


        {
            auto nameContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            nameContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                          Bootstrap::Grid::Medium::col_md_3+
                                          Bootstrap::Grid::Small::col_sm_3+
                                          Bootstrap::Grid::ExtraSmall::col_xs_3+
                                          Bootstrap::Grid::Offset::Large::col_lg_3+
                                          Bootstrap::Grid::Offset::Medium::col_md_3+
                                          Bootstrap::Grid::Offset::Small::col_sm_3+
                                          Bootstrap::Grid::Offset::ExtraSmall::col_xs_3);
            auto nametext = nameContainer->addWidget (cpp14::make_unique<WText>("Malzeme Çıkışı Yap"));
            nameContainer->addStyleClass (CSSStyle::Button::blueButton);
            nameContainer->setHeight (25);
            nameContainer->clicked ().connect ([=](){
                this->exportMalzeme (item.miktarGiris,item);
            } );
        }



    }




    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setMargin (50,Side::Top);
        container->setPadding (10,AllSides);

        auto titleText = container->addWidget (cpp14::make_unique<WText>("Malzeme Giriş Miktarı"));
        titleText->setMargin (10,Side::Top|Side::Bottom);
        container->addStyleClass (CSSStyle::Gradient::greenGradient);
    }


    for(const auto &item : girisItemList ){

        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setPositionScheme (PositionScheme::Relative);
        container->setHeight (25);
        container->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (150,180),this->getRandom (150,180),this->getRandom (150,180))+
                                      Style::color::color (Style::color::White::AliceBlue));
        container->setMargin (10,Side::Top);
        container->addStyleClass (CSSStyle::Shadows::shadow8px);

//        auto percentBar = container->addWidget (cpp14::make_unique<WContainerWidget>());
//        percentBar->setHeight (25);
//        percentBar->setOffsets (0,Side::Top|Side::Left);
//        percentBar->setWidth (WLength(WString("{1}%").arg(item.miktarGiris).toUTF8 ()));
//        percentBar->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (),this->getRandom (),this->getRandom ()));

        auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->setPositionScheme (PositionScheme::Absolute);
        rContainer->setWidth (WLength("100%"));
        rContainer->setHeight (WLength("100%"));
        rContainer->setOffsets (0,Side::Top|Side::Left);
        rContainer->addStyleClass (Bootstrap::Grid::row);


        {
            auto nameContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            nameContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                          Bootstrap::Grid::Medium::col_md_3+
                                          Bootstrap::Grid::Small::col_sm_3+
                                          Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto nametext = nameContainer->addWidget (cpp14::make_unique<WText>(item.name));
            nameContainer->setHeight (25);
        }


        {
            auto nameContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            nameContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                          Bootstrap::Grid::Medium::col_md_3+
                                          Bootstrap::Grid::Small::col_sm_3+
                                          Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto nametext = nameContainer->addWidget (cpp14::make_unique<WText>("Giren Miktar: "+std::to_string (item.miktarGiris)+" "+item.metric));
            nameContainer->setHeight (25);
        }






    }




    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setMargin (25,Side::Top);
        container->setPadding (10,AllSides);

        auto titleText = container->addWidget (cpp14::make_unique<WText>("Malzeme Çıkış Miktarı"));
        titleText->setMargin (10,Side::Top|Side::Bottom);
        container->addStyleClass (CSSStyle::Gradient::redGradient);
    }


    for(const auto &item : cikisItemList ){

        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->setPositionScheme (PositionScheme::Relative);
        container->setHeight (25);
        container->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (150,180),this->getRandom (150,180),this->getRandom (150,180))+
                                      Style::color::color (Style::color::White::AliceBlue));
        container->setMargin (10,Side::Top);
        container->addStyleClass (CSSStyle::Shadows::shadow8px);

//        auto percentBar = container->addWidget (cpp14::make_unique<WContainerWidget>());
//        percentBar->setHeight (25);
//        percentBar->setOffsets (0,Side::Top|Side::Left);
//        percentBar->setWidth (WLength(WString("{1}%").arg(item.miktarGiris).toUTF8 ()));
//        percentBar->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (),this->getRandom (),this->getRandom ()));

        auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->setPositionScheme (PositionScheme::Absolute);
        rContainer->setWidth (WLength("100%"));
        rContainer->setHeight (WLength("100%"));
        rContainer->setOffsets (0,Side::Top|Side::Left);
        rContainer->addStyleClass (Bootstrap::Grid::row);


        {
            auto nameContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            nameContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                          Bootstrap::Grid::Medium::col_md_3+
                                          Bootstrap::Grid::Small::col_sm_3+
                                          Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto nametext = nameContainer->addWidget (cpp14::make_unique<WText>(item.name));
            nameContainer->setHeight (25);
        }


        {
            auto nameContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            nameContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                          Bootstrap::Grid::Medium::col_md_3+
                                          Bootstrap::Grid::Small::col_sm_3+
                                          Bootstrap::Grid::ExtraSmall::col_xs_3);
            auto nametext = nameContainer->addWidget (cpp14::make_unique<WText>("Çıkan Miktar: "+std::to_string (item.miktarCikis)+" "+item.metric));
            nameContainer->setHeight (25);
        }






    }



}

void v2::StokWidget::StokContainerWidget::initimportMenu()
{

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto titleText = container->addWidget (cpp14::make_unique<WText>("<b>Malzeme Giriş Listesi</b>"));
        container->setPadding (5,Side::Top|Side::Bottom);
        container->setMargin (15,Side::Top|Side::Bottom);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_9+
                                  Bootstrap::Grid::Medium::col_md_9+
                                  Bootstrap::Grid::Small::col_sm_8+
                                  Bootstrap::Grid::ExtraSmall::col_xs_7+
                                  CSSStyle::Gradient::blueGradient+
                                  CSSStyle::Shadows::shadow8px);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto titleText = container->addWidget (cpp14::make_unique<WText>("<b>Yeni Girişi</b>"));
        container->setPadding (5,Side::Top|Side::Bottom);
        container->setMargin (15,Side::Top|Side::Bottom);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5+
                                  CSSStyle::Button::greenButton+
                                  CSSStyle::Shadows::shadow8px);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect (this,&v2::StokWidget::StokContainerWidget::importMazeleme);

    }


}

void v2::StokWidget::StokContainerWidget::initexportMenu()
{
    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto titleText = container->addWidget (cpp14::make_unique<WText>("<b>Malzeme Çıkış Listesi</b>"));
        container->setPadding (5,Side::Top|Side::Bottom);
        container->setMargin (15,Side::Top|Side::Bottom);
        container->addStyleClass (Bootstrap::Grid::col_full_12+
                                  CSSStyle::Gradient::blueGradient+
                                  CSSStyle::Shadows::shadow8px);
    }
}

void v2::StokWidget::StokContainerWidget::initViewMalzeme(const bsoncxx::oid &malzemeOid)
{
    SerikBLDCore::Stokv2::Stok filter;
    filter.setOid (malzemeOid);

    auto val = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::FindOneItem (filter);

    if( val.view ().empty () ){
        this->showPopUpMessage ("Malzeme Yüklenemedi","err");
    }else{
        this->Content ()->clear ();
        this->Content ()->addWidget (cpp14::make_unique<MalzemeGirisWidget>(val))->asFullPage ();
        this->showPopUpMessage ("Malzeme Yüklendi");
    }
}

std::vector<v2::StokWidget::PipeLineStokItem> v2::StokWidget::StokContainerWidget::getGirisList()
{
    std::vector<PipeLineStokItem> itemList;

    mongocxx::pipeline stages;


    auto matchFilter = document{};

    try {
        matchFilter.append (kvp(SerikBLDCore::Stokv2::Key::mudurluk,this->mUser->Birimi ()));
    }  catch (bsoncxx::exception &e) {
        LOG << e.what () << std::endl;
        this->showPopUpMessage (std::string("Hata Oluştu. ") + e.what (),"err");
        return itemList;
    }

    try {
        matchFilter.append (kvp("type","giris"));
    }  catch (bsoncxx::exception &e) {
        LOG << e.what () << std::endl;
        this->showPopUpMessage (std::string("Hata Oluştu. ") + e.what (),"err");
        return itemList;
    }


    auto groupFilter = document{};

    try {
        groupFilter.append (kvp("_id","$kategoriOid"));
    }  catch (bsoncxx::exception &e) {
        LOG << e.what () << std::endl;
        this->showPopUpMessage (std::string("Hata Oluştu. ") + e.what (),"err");
        return itemList;
    }

    try {
        groupFilter.append (kvp("count",make_document(kvp("$sum","$miktar"))));
    }  catch (bsoncxx::exception &e) {
        LOG << e.what () << std::endl;
        this->showPopUpMessage (std::string("Hata Oluştu. ") + e.what (),"err");
        return itemList;
    }

    stages.match (matchFilter.view ()).group (groupFilter.view ());



    auto cursor = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::getDB ()->db ()->collection (SerikBLDCore::Stokv2::Key::Collection).aggregate (stages);




    for (auto&& doc : cursor) {


        PipeLineStokItem item;
        bool error = false;

        try{
            item.kategoriOid = doc["_id"].get_oid ().value.to_string ();
        }catch(bsoncxx::exception &e){
            LOG << e.what ();
            error = true;
        }

        try{
            item.miktarGiris = doc["count"].get_double ().value;
        }catch(bsoncxx::exception &e){
            LOG << e.what ();
            error = true;
        }

        if( !error ){
            itemList.push_back (item);
        }

//        LOG << bsoncxx::to_json (doc) << std::endl;
    }

    return itemList;

}

std::vector<v2::StokWidget::PipeLineStokItem> v2::StokWidget::StokContainerWidget::getCikisList()
{
    std::vector<PipeLineStokItem> itemList;

    mongocxx::pipeline stages;


    auto matchFilter = document{};

    try {
        matchFilter.append (kvp(SerikBLDCore::Stokv2::Key::mudurluk,this->mUser->Birimi ()));
    }  catch (bsoncxx::exception &e) {
        LOG << e.what () << std::endl;
        this->showPopUpMessage (std::string("Hata Oluştu. ") + e.what (),"err");
        return itemList;
    }

    try {
        matchFilter.append (kvp("type","cikis"));
    }  catch (bsoncxx::exception &e) {
        LOG << e.what () << std::endl;
        this->showPopUpMessage (std::string("Hata Oluştu. ") + e.what (),"err");
        return itemList;
    }


    auto groupFilter = document{};

    try {
        groupFilter.append (kvp("_id","$kategoriOid"));
    }  catch (bsoncxx::exception &e) {
        LOG << e.what () << std::endl;
        this->showPopUpMessage (std::string("Hata Oluştu. ") + e.what (),"err");
        return itemList;
    }

    try {
        groupFilter.append (kvp("count",make_document(kvp("$sum","$miktar"))));
    }  catch (bsoncxx::exception &e) {
        LOG << e.what () << std::endl;
        this->showPopUpMessage (std::string("Hata Oluştu. ") + e.what (),"err");
        return itemList;
    }

    stages.match (matchFilter.view ()).group (groupFilter.view ());



    auto cursor = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Stok>::getDB ()->db ()->collection (SerikBLDCore::Stokv2::Key::Collection).aggregate (stages);




    for (auto&& doc : cursor) {


        PipeLineStokItem item;
        bool error = false;

        try{
            item.kategoriOid = doc["_id"].get_oid ().value.to_string ();
        }catch(bsoncxx::exception &e){
            LOG << e.what ();
            error = true;
        }

        try{
            item.miktarCikis = doc["count"].get_double ().value;
        }catch(bsoncxx::exception &e){
            LOG << e.what ();
            error = true;
        }

        if( !error ){
            itemList.push_back (item);
        }

//        LOG << bsoncxx::to_json (doc) << std::endl;
    }

    return itemList;
}

v2::StokWidget::MalzemeGirisDialog::MalzemeGirisDialog(SerikBLDCore::User *_mUser, const QVector<SerikBLDCore::Stokv2::Kategori> &_mKategoriList)
    :SerikBLDCore::PersonelManager (_mUser->getDB ()),mKategoriList(_mKategoriList),mUser(_mUser)
{
    this->initWidget ();
}

void v2::StokWidget::MalzemeGirisDialog::initWidget()
{


    auto gridLayoutContainer = this->Content()->addNew<WContainerWidget>();
    gridLayoutContainer->addStyleClass(Bootstrap::Grid::col_full_12);

    auto gridLayout = gridLayoutContainer->setLayout(cpp14::make_unique<WGridLayout>());


    auto createMenuItem = [gridLayout]<typename T>(const std::string &text , std::unique_ptr<T> &&widget){
            auto row = gridLayout->rowCount();
            auto containerText = gridLayout->addWidget(cpp14::make_unique<WContainerWidget>(),row,0);
            containerText->addWidget(cpp14::make_unique<WText>(text));
            containerText->setPadding(5);
            containerText->setContentAlignment(AlignmentFlag::Right);

            auto containerWidget = gridLayout->addWidget(cpp14::make_unique<WContainerWidget>(),row,1);
            auto ptr = widget.get();
            containerWidget->addWidget(std::move(widget));
            return ptr;
        };

    {
        mKalemComboBox = createMenuItem("Kalem/Kategori",std::make_unique<WComboBox>());
        for( const auto &item : mKategoriList ){
            mKalemComboBox->addItem (item.getKategoriAdi ());
        }
    }

    {
        mGirenMiktarSpinBoxValue = createMenuItem("Miktar",std::make_unique<WDoubleSpinBox>());
        mGirenMiktarSpinBoxValue->setMaximum (9999999);
    }


    {
        mDateEditWidget = createMenuItem("Giriş Tarihi",std::make_unique<WDateEdit>());
        mDateEditWidget->setDate(WDate::currentDate());
    }

    {
        mGirenFiyatSpinBoxValue = createMenuItem("Giriş Fiyatı",std::make_unique<WDoubleSpinBox>());
        mGirenFiyatSpinBoxValue->setMaximum (9999999);
    }

    {
        mCurrentGirenOid = createMenuItem("Giren Personel Oid",std::make_unique<WText>());
        mCurrentGirenOid->setText(mUser->oid ().value ().to_string ());
        mCurrentGirenOid->setPadding(5);
    }

}

std::string v2::StokWidget::MalzemeGirisDialog::getCurrentMalzemeGirenOid() const
{
    return mCurrentGirenOid->text ().toUTF8 ();
}

double v2::StokWidget::MalzemeGirisDialog::getCurrentGirenMiktar() const
{
    return mGirenMiktarSpinBoxValue->value ();
}

double v2::StokWidget::MalzemeGirisDialog::getCurrentGirenFiyat() const
{
    return mGirenFiyatSpinBoxValue->value ();
}

std::string v2::StokWidget::MalzemeGirisDialog::getCurrentKategoriOid() const
{
    std::string currentGirisOid;
    for( auto item : mKategoriList ){
        if( item.getKategoriAdi () == mKalemComboBox->currentText ().toUTF8 () ){
            currentGirisOid = item.oid ().value ().to_string ();
            break;
        }
    }
    return currentGirisOid;
}

std::string v2::StokWidget::MalzemeGirisDialog::getCurrentMalzemeAdi() const
{
    std::string currentMalzemeAdi;
    for( auto item : mKategoriList ){
        if( item.getKategoriAdi () == mKalemComboBox->currentText ().toUTF8 () ){
            currentMalzemeAdi = item.getKategoriAdi ();
            break;
        }
    }
    return currentMalzemeAdi;
}

std::string v2::StokWidget::MalzemeGirisDialog::getCurruntMalzemeMetric() const
{
    std::string currentMalzemeMetric;
    for( const auto &item : mKategoriList ){
        if( item.getKategoriAdi () == mKalemComboBox->currentText ().toUTF8 () ){
            currentMalzemeMetric = item.getMetric ();
            break;
        }
    }
    return currentMalzemeMetric;
}




v2::StokWidget::MalzemeGirisWidget::MalzemeGirisWidget(const Stok &girisItem)
    :SerikBLDCore::Stokv2::Giris (girisItem)
{

}

void v2::StokWidget::MalzemeGirisWidget::asListItem()
{
    this->Content ()->addStyleClass (CSSStyle::Shadows::shadow8px);
    this->Content ()->setMargin (5,Side::Top|Side::Bottom);
    auto cikisObj = this->toCikis ();

    if( cikisObj ){
        {
            auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            auto text = container->addWidget (cpp14::make_unique<WText>(cikisObj->getMalzemeAdi ()));
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_3+
                                      Bootstrap::Grid::ExtraSmall::col_xs_3);
        }

        {
            auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            auto text = container->addWidget (cpp14::make_unique<WText>(std::to_string (cikisObj->getMiktar ())+" "+this->getMetric () + " : " + std::to_string (this->getBirimFiyat ()*this->getMiktar ()) + " TL"));
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                      Bootstrap::Grid::Medium::col_md_3+
                                      Bootstrap::Grid::Small::col_sm_3+
                                      Bootstrap::Grid::ExtraSmall::col_xs_3);
        }

        {
            auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            auto text = container->addWidget (cpp14::make_unique<WText>(WDate::fromJulianDay (cikisObj->getTeslimEtJulianDay ()).toString ("dd/MM/yyyy")));
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_2);
        }

        {
            auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->setOverflow (Overflow::Hidden);
            auto text = container->addWidget (cpp14::make_unique<WText>("cikisObj->getCikisYeri ()"));
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                      Bootstrap::Grid::Medium::col_md_4+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_4);
        }

    }else{
        {
            auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            auto text = container->addWidget (cpp14::make_unique<WText>(this->getMalzemeAdi ()));
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                      Bootstrap::Grid::Medium::col_md_4+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_4);
        }

        {
            auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            auto text = container->addWidget (cpp14::make_unique<WText>(std::to_string (this->getMiktar ())+" "+this->getMetric () + " : " + std::to_string (this->getBirimFiyat ()*this->getMiktar ()) + " TL"));
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                      Bootstrap::Grid::Medium::col_md_4+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_4);
        }

        {
            auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            auto text = container->addWidget (cpp14::make_unique<WText>(WDate::fromJulianDay (this->getTeslimAlimJulianDay ()).toString ("dd/MM/yyyy")));
            container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                      Bootstrap::Grid::Medium::col_md_4+
                                      Bootstrap::Grid::Small::col_sm_4+
                                      Bootstrap::Grid::ExtraSmall::col_xs_4);
        }
    }




//    this->clicked ().connect ([=](){
//        this->_clickWidget.emit (this->oid ().value ());
//    });
}

void v2::StokWidget::MalzemeGirisWidget::asFullPage()
{

}

Signal<bsoncxx::oid> &v2::StokWidget::MalzemeGirisWidget::ClickedWidget()
{
    return _clickWidget;
}


namespace v2 {
namespace StokWidget {
std::int64_t MalzemeGirisDialog::getJulianDay() const
{
    return mDateEditWidget->date ().toJulianDay ();
}
}
}
