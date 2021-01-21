#include "stokcontainerwidget.h"



v2::StokWidget::StokContainerWidget::StokContainerWidget(SerikBLDCore::User *_mUser)
    :ContainerWidget ("Stok Yönetimi / "+_mUser->Birimi ()),
      SerikBLDCore::Stokv2::Stokv2Manager (_mUser->getDB ()),
      mUser(_mUser)
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
    }

//    {
//        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
//        container->addWidget (cpp14::make_unique<WText>("İstatistik"));
//        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
//                                  Bootstrap::Grid::Medium::col_md_2+
//                                  Bootstrap::Grid::Small::col_sm_3+
//                                  Bootstrap::Grid::ExtraSmall::col_xs_3+
//                                  CSSStyle::Button::blueButton);
//        container->setPadding (10,Side::Top|Side::Bottom);
//    }

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

    case CurrentPage::Ayarlar:
        break;
    default:
        break;
    }


    for( auto item : *mlist ){
        auto container = this->Content ()->addWidget (cpp14::make_unique<MalzemeGirisWidget>(item));
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->asListItem ();
        container->ClickedWidget ().connect ([=](const bsoncxx::oid& itemOid ){
            this->initViewMalzeme (itemOid);
        });
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

                SerikBLDCore::Stokv2::Kategori filter;
                filter.setOid (delcontainer->attributeValue (Style::dataoid).toUTF8 ());

                auto del = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Kategori>::DeleteItem (filter);

                if( !del ){
                    this->warnDialog ("Silinemedi");
                }else{
                    this->informDialog ("Silindi");
                    filter.clear ();
                    filter.setBirim (mUser->Birimi ());
                    this->updateStokKategoriList (filter);
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
            this->updateStokKategoriList (item);

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
                   this->updateStokKategoriList (item);
               }

               this->removeDialog (mDialog);
           });
        });
    }

}

void v2::StokWidget::StokContainerWidget::importMazeleme()
{

    this->Content ()->clear ();

    auto mDialog = createDialog ("Yeni Malzeme Girişi");
    mDialog->setWidth (WLength(500));

    auto _kategoriList = this->::SerikBLDCore::ListItem<SerikBLDCore::Stokv2::Kategori>::List (SerikBLDCore::Stokv2::Kategori().setBirim (mUser->Birimi ()),SerikBLDCore::FindOptions ().setLimit (100));

    auto girisObj = mDialog->contents ()->addWidget (cpp14::make_unique<MalzemeGirisDialog>(this->mUser,_kategoriList));
    girisObj->addStyleClass (Bootstrap::Grid::container_fluid);

    auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Girişi Yap"));
    saveBtn->clicked ().connect ([=](){

        if( girisObj->getCurrentGirenMiktar () <= 0 ){
            warnDialog ("Giren Malzeme Miktarı <b>0</b> Altında Olamaz");
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
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_9+
                                  Bootstrap::Grid::Medium::col_md_9+
                                  Bootstrap::Grid::Small::col_sm_8+
                                  Bootstrap::Grid::ExtraSmall::col_xs_7+
                                  CSSStyle::Gradient::blueGradient+
                                  CSSStyle::Shadows::shadow8px);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto titleText = container->addWidget (cpp14::make_unique<WText>("<b>Yeni Çıkış</b>"));
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

v2::StokWidget::MalzemeGirisDialog::MalzemeGirisDialog(SerikBLDCore::User *_mUser, const QVector<SerikBLDCore::Stokv2::Kategori> &_mKategoriList)
    :SerikBLDCore::PersonelManager (_mUser->getDB ()),mKategoriList(_mKategoriList),mUser(_mUser)
{
    this->initWidget ();
}

void v2::StokWidget::MalzemeGirisDialog::initWidget()
{


    {
        mKalemComboBox = this->Content ()->addWidget (cpp14::make_unique<WComboBox>());
        mKalemComboBox->addStyleClass (Bootstrap::Grid::col_full_12);
        for( const auto &item : mKategoriList ){
            mKalemComboBox->addItem (item.getKategoriAdi ());
        }

    }

    {
        mGirenMiktarSpinBoxValue = this->Content ()->addWidget (cpp14::make_unique<WDoubleSpinBox>());
        mGirenMiktarSpinBoxValue->addStyleClass (Bootstrap::Grid::col_full_12);
        mGirenMiktarSpinBoxValue->setMaximum (9999999);
    }

    {
        mDateEditWidget = this->Content ()->addWidget (cpp14::make_unique<WDateEdit>());
        mDateEditWidget->addStyleClass (Bootstrap::Grid::col_full_12);

    }


    {
        auto textContainer = this->Content ()->addWidget (cpp14::make_unique<WText>("Giren Personel"));
        textContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                      Bootstrap::Grid::Medium::col_md_6+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
        mCurrentGirenOid = this->Content ()->addWidget (cpp14::make_unique<WText>(mUser->oid ().value ().to_string ()));
        mCurrentGirenOid->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                      Bootstrap::Grid::Medium::col_md_6+
                                      Bootstrap::Grid::Small::col_sm_6+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
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
    this->Content ()->decorationStyle ().setCursor (Cursor::PointingHand);
    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto text = container->addWidget (cpp14::make_unique<WText>(this->getMalzemeAdi ()));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto text = container->addWidget (cpp14::make_unique<WText>(std::to_string (this->getMiktar ())+" "+this->getMetric ()));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto text = container->addWidget (cpp14::make_unique<WText>(WDate::fromJulianDay (this->getTeslimAlimJulianDay ()).toString ("dd/MM/yyyy")));
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);
    }

    this->clicked ().connect ([=](){
       this->_clickWidget.emit (this->oid ().value ());
    });
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
