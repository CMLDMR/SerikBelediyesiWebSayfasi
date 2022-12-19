#include "duyuruyonetim.h"



v2::Duyuru::DuyuruYonetim::DuyuruYonetim(DB *_mdb)
    :ContainerWidget("Duyurular"),SerikBLDCore::ListItem<v2::Duyuru::DuyuruItem>(_mdb)
{
    this->UpdateList(Duyuru::DuyuruItem());

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

    }

}

void v2::Duyuru::DuyuruYonetim::errorOccured(const std::string &errorText)
{

    this->showMessage("HATA",errorText);

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
    auto val = this->element(Key::endDate);
    if( val ){
        auto str = QString::number(static_cast<int>(val->get_double().value));
        auto yilStr = str.mid(0,4);
        auto ayStr = str.mid(4,2);
        auto dayStr = str.mid(6,2);
        return dayStr.toStdString()+"/"+ayStr.toStdString()+"/"+yilStr.toStdString();
    }
    return "";
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
        container->addNew<WText>((this->Yayinda() ? "Yayında" : "Değil"));
        if( this->Yayinda() ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::LightGreen));
        }else{
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Red::LightCoral));
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
