#include "basvuruitemviewwidget.h"

BasvuruItemViewWidget::BasvuruItemViewWidget(const SerikBLDCore::BilgiEdinmeItem &item)
    : mItem(item)
{
    setAttributeValue (Style::style,Style::background::color::color (Style::color::White::OldLace));
    this->Header ()->addWidget (cpp14::make_unique<WText>(WString("Konu: {1}").arg (mItem.konu ().toStdString ())));


    this->Content ()->setMargin (25,Side::Top|Side::Bottom);
    this->Content ()->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    this->Content ()->setWidth (WLength("100%"));
    this->Content ()->addWidget (cpp14::make_unique<WText>(WString("Mesaj: {1}").arg (mItem.mesaj ().toStdString ())));

    if( this->mItem.cevaplandi () )
    {
        this->Content ()->addWidget (cpp14::make_unique<WBreak>());
        auto bckBtn = this->Content ()->addWidget (cpp14::make_unique<WPushButton>("Cevap Dosyası"));
        bckBtn->setMargin (15,Side::Top|Side::Bottom);
        bckBtn->addStyleClass (Bootstrap::Button::Primary);
        bckBtn->clicked ().connect ([&](){
            _ClickedCevapFile.emit (this->mItem.cevapOid ().toStdString ());
        });
    }else{
        this->Footer ()->setMargin (25,Side::Top|Side::Bottom);
        this->Footer ()->setContentAlignment (AlignmentFlag::Center);
        this->Footer ()->addWidget (cpp14::make_unique<WText>("<b>Bu Başvuru Henüz Cevaplanmamış</b>"));
        this->Footer ()->addWidget (cpp14::make_unique<WBreak>());


    }

    auto btn = this->Footer ()->addWidget (cpp14::make_unique<WPushButton>("Başvuru Listesi"));
    btn->addStyleClass (Bootstrap::Button::Primary);

    btn->clicked ().connect ([&](){
        _Closed.emit (NoClass());
    });

}

Signal<NoClass> &BasvuruItemViewWidget::Closed()
{
    return _Closed;
}

Signal<std::string> &BasvuruItemViewWidget::ClickedCevapFile()
{
    return _ClickedCevapFile;
}

