#include "duyuruyonetim.h"



v2::Duyuru::DuyuruYonetim::DuyuruYonetim(DB *_mdb)
    :ContainerWidget("Duyurular"),SerikBLDCore::ListItem<v2::Duyuru::DuyuruItem>(_mdb)
{


    mControllerWidget = this->getContoller();
    if( mControllerWidget ){
        mControllerWidget.value()->BackClicked().connect([=](){
            this->back(Duyuru::DuyuruItem());
        });
        mControllerWidget.value()->NextClicked().connect([=](){
            this->next(Duyuru::DuyuruItem());
        });
        mControllerWidget.value()->setText(std::to_string(this->currentPage())+"/"+std::to_string(this->totalPage()));
    }

    {
        auto ListeleContainerWidget = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        ListeleContainerWidget->setContentAlignment(AlignmentFlag::Center);
        ListeleContainerWidget->setPadding(5,Side::Top|Side::Bottom);
        ListeleContainerWidget->decorationStyle().setCursor(Cursor::PointingHand);
        ListeleContainerWidget->addNew<WText>("Listele");
        ListeleContainerWidget->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);
        ListeleContainerWidget->clicked().connect([=](){ this->UpdateList();});
        ListeleContainerWidget->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_8+
                                              Bootstrap::Grid::Offset::Medium::col_md_8+
                                              Bootstrap::Grid::Offset::Small::col_sm_8+
                                              Bootstrap::Grid::Offset::ExtraSmall::col_xs_8+
                                              Bootstrap::Grid::Large::col_lg_2+
                                              Bootstrap::Grid::Medium::col_md_2+
                                              Bootstrap::Grid::Small::col_sm_2+
                                              Bootstrap::Grid::ExtraSmall::col_xs_2);
        this->Header()->setMargin(25,Side::Top|Side::Bottom);
        this->Header()->setContentAlignment(AlignmentFlag::Right);
    }


    {
        auto ListeleContainerWidget = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        ListeleContainerWidget->setContentAlignment(AlignmentFlag::Center);
        ListeleContainerWidget->decorationStyle().setCursor(Cursor::PointingHand);
        ListeleContainerWidget->setPadding(5,Side::Top|Side::Bottom);
        ListeleContainerWidget->addNew<WText>("Yeni Ekle");
        ListeleContainerWidget->addStyleClass(Bootstrap::ContextualBackGround::bg_danger);
        ListeleContainerWidget->clicked().connect(this,&DuyuruYonetim::yeniDuyuruEkle);
        ListeleContainerWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                              Bootstrap::Grid::Medium::col_md_2+
                                              Bootstrap::Grid::Small::col_sm_2+
                                              Bootstrap::Grid::ExtraSmall::col_xs_2);
        this->Header()->setMargin(25,Side::Top|Side::Bottom);
        this->Header()->setContentAlignment(AlignmentFlag::Right);
    }

    this->UpdateList(Duyuru::DuyuruItem());


}

void v2::Duyuru::DuyuruYonetim::onList(const QVector<DuyuruItem> *mlist)
{

    this->Content()->clear();

    if( mControllerWidget ){
        mControllerWidget.value()->setText(std::to_string(this->currentPage())+"/"+std::to_string(this->totalPage()));
    }


    for( const auto &item : *mlist ){

        auto container = Content()->addNew<Widget::ListItem>(item);
        container->setMargin(5,Side::Bottom);
        container->clicked().connect([=](){
            this->initDuyuruItem(item.oid().value().to_string());
        });

    }

}

void v2::Duyuru::DuyuruYonetim::errorOccured(const std::string &errorText)
{
    this->showMessage("HATA",errorText);
}

void v2::Duyuru::DuyuruYonetim::initDuyuruItem(const std::string &duyuruOid)
{


    this->Content()->clear();
    DuyuruItem filter;
    filter.setOid(duyuruOid);

    auto duyuruItem = this->FindOneItem(filter);
    auto duyuruWidget = this->Content()->addNew<Widget::DuyuruView>(duyuruItem);
    duyuruWidget->setMargin(25,Side::Left|Side::Right);

}

void v2::Duyuru::DuyuruYonetim::yeniDuyuruEkle()
{


    //TODO: Duyuru Ekleme Tamamlanmadı
    this->Content()->clear();



    auto baslikLineEdit = this->Content()->addNew<WLineEdit>();
    baslikLineEdit->setPlaceholderText("Başlık Giriniz");
    baslikLineEdit->addStyleClass(Bootstrap::Grid::col_full_12);
    this->Content()->addNew<WBreak>();

    auto icerikContainer = this->Content()->addNew<WContainerWidget>();
    icerikContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    auto icerikTextEdit = icerikContainer->addNew<WTextEdit>();
    icerikContainer->setMargin(25,Side::Top|Side::Bottom);
    icerikTextEdit->setHeight(350);


    auto startContainer = Content()->addNew<WContainerWidget>();
    startContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                             Bootstrap::Grid::Medium::col_md_6+
                             Bootstrap::Grid::Small::col_sm_6+
                             Bootstrap::Grid::ExtraSmall::col_xs_6);
    auto startTarih = startContainer->addNew<WDateEdit>();
    startTarih->setPlaceholderText("İlk Yayınlanma Tarihi");

    auto lastContainer = Content()->addNew<WContainerWidget>();
    lastContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6+
                             Bootstrap::Grid::Medium::col_md_6+
                             Bootstrap::Grid::Small::col_sm_6+
                             Bootstrap::Grid::ExtraSmall::col_xs_6);
    auto lastTarih = lastContainer->addNew<WDateEdit>();
    lastTarih->setPlaceholderText("Son Yayınlanma Tarihi");


}

std::string v2::Duyuru::DuyuruItem::Baslik() const
{
    auto val = this->element(Key::baslik);
    if( val ){
        return val->get_utf8().value.to_string();
    }
    return "";
}

bool v2::Duyuru::DuyuruItem::Yayinda() const
{
    auto val = this->element(Key::yayinda);
    if( val ){
        return val->get_utf8().value.to_string() == "Yayında" ? true : false;
    }
    return false;
}

std::string v2::Duyuru::DuyuruItem::Birim() const
{
    auto val = this->element(Key::birim);
    if( val ){
        return val->get_utf8().value.to_string();
    }
    return "";
}

std::string v2::Duyuru::DuyuruItem::SonTarihText() const
{
    auto val = this->element(Key::julianBitis);
    if( val ){
        auto str = QDate::fromJulianDay(val.value().get_int64().value).toString("dd/MM/yyyy").toStdString();
        return str;
    }
    return "";
}

std::string v2::Duyuru::DuyuruItem::Icerik() const
{
    auto val = this->element(Key::icerik);
    if( val ){
        return val.value().get_utf8().value.to_string();
    }
    return "";
}

int64_t v2::Duyuru::DuyuruItem::BaslangicDate() const
{
    auto val = this->element(Key::julianBaslangic);
    if( val ){
        return val->get_int64().value;
    }
    return 0;
}

int64_t v2::Duyuru::DuyuruItem::BitisDate() const
{
    auto val = this->element(Key::julianBitis);
    if( val ){
        return val->get_int64().value;
    }
    return 0;
}



v2::Duyuru::Widget::ListItem::ListItem(const DuyuruItem &item)
    :DuyuruItem(item)
{
    this->addStyleClass(CSSStyle::Shadows::shadow8px+CSSStyle::Radius::radius3px);
    this->addStyleClass(Bootstrap::Grid::col_full_12);


    //Birim
    {
        auto container = this->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                                 Bootstrap::Grid::Medium::col_md_3+
                                 Bootstrap::Grid::Small::col_sm_3+
                                 Bootstrap::Grid::ExtraSmall::col_xs_3);
        container->addNew<WText>(this->Birim());
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::AntiqueWhite));
    }


    //Başlık
    {
        auto container = this->addNew<WContainerWidget>();
        container->setContentAlignment(AlignmentFlag::Left);
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_7+
                                 Bootstrap::Grid::Medium::col_md_7+
                                 Bootstrap::Grid::Small::col_sm_7+
                                 Bootstrap::Grid::ExtraSmall::col_xs_7);
        container->addNew<WText>(this->Baslik().size() > 70 ? this->Baslik().substr(0,70)+"..." : this->Baslik() );
    }

    //Yayında
    {
        auto container = this->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                 Bootstrap::Grid::Medium::col_md_1+
                                 Bootstrap::Grid::Small::col_sm_1+
                                 Bootstrap::Grid::ExtraSmall::col_xs_1);
        std::string sonDurum = "";
        if( this->Yayinda() ){

            if( QDate::currentDate().toJulianDay() < this->BaslangicDate() ){
                sonDurum = "Yaklaşıyor";
            }else{
                if( QDate::currentDate().toJulianDay() > this->BitisDate() ){
                    sonDurum = "Geçmiş";
                }else{
                    sonDurum = "Yayında";
                }
            }
        }else{
            sonDurum = "Değil";
        }


        container->addNew<WText>(sonDurum);
        if( sonDurum == "Yayında" ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::LightGreen));
        }else if( sonDurum == "Yaklaşıyor" ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::Green));
        }else if( sonDurum == "Geçmiş" ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::DarkSeaGreen));
        }else if( sonDurum == "Değil" ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Red::Red));
        }


    }

    //Son Tarih
    {
        auto container = this->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                 Bootstrap::Grid::Medium::col_md_1+
                                 Bootstrap::Grid::Small::col_sm_1+
                                 Bootstrap::Grid::ExtraSmall::col_xs_1);
        container->addNew<WText>(this->SonTarihText());
    }

    decorationStyle().setCursor(Cursor::PointingHand);
}

v2::Duyuru::Widget::DuyuruView::DuyuruView(const DuyuruItem &item)
    :DuyuruItem(item)
{

    this->addStyleClass(Bootstrap::ImageShape::img_thumbnail);


    auto baslikText = this->Content()->addNew<WText>("<h4>"+this->Baslik()+"</h4>",TextFormat::UnsafeXHTML);
    baslikText->addStyleClass(Bootstrap::Grid::col_full_12);
    baslikText->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DimGray)+Style::color::color(Style::color::White::AliceBlue));

    auto icerikText = this->Content()->addNew<WText>(item.Icerik(),TextFormat::UnsafeXHTML);
    icerikText->addStyleClass(Bootstrap::Grid::col_full_12);
    icerikText->setMargin(25,Side::Bottom);

    auto tarihText = this->Content()->addNew<WText>("<h5>"+QDate::fromJulianDay(item.BaslangicDate()).toString("dd/MM/yyyy").toStdString()+"  -  " + QDate::fromJulianDay(item.BitisDate()).toString("dd/MM/yyyy").toStdString()+"</h5>",TextFormat::UnsafeXHTML);
    tarihText->addStyleClass(Bootstrap::Grid::col_full_12);
    tarihText->addStyleClass(Bootstrap::ContextualBackGround::bg_danger);


    auto kalanGunText = this->Content()->addNew<WText>("Yayından Kalkmasına " + std::to_string(this->BitisDate() - QDate::currentDate().toJulianDay())+" gün var",TextFormat::UnsafeXHTML);
    kalanGunText->addStyleClass(Bootstrap::Grid::col_full_12);
    tarihText->addStyleClass(Bootstrap::ContextualBackGround::bg_info);

}
