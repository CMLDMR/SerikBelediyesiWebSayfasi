#include "dilekceview.h"
#include <QDateTime>

DilekceView::DilekceView(Dilekce *_dilekce, mongocxx::database *_db)
    : ContainerWidget (),Dilekce(_dilekce),DB(_db)
{

    mTCManager = new TCManager(this->db ());

    if( _dilekce )
    {
        this->initTCView ();
        this->initDilekceView ();
        this->initCevapView ();
    }
}

void DilekceView::initTCView()
{
    auto _tcOid = this->tcoid ();
    if( !_tcOid.isEmpty () )
    {
        auto tcItem = mTCManager->Load_byOID ( _tcOid.toStdString () );
        if( tcItem )
        {
            this->Header ()->clear ();
            auto tcContainer = this->Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
            tcContainer->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
            tcContainer->addStyleClass (Bootstrap::ContextualBackGround::bg_info);

            auto rContainer = tcContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            rContainer->addStyleClass (Bootstrap::Grid::row);

            {
                auto tContainer = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                tContainer->addStyleClass (Bootstrap::Grid::col_full_12);
                tContainer->setContentAlignment (AlignmentFlag::Center);
                auto text = tContainer->addWidget (cpp14::make_unique<WText>(tcItem.value ()->TCNO ().toStdString ()));
                text->addStyleClass (Bootstrap::Label::Primary);
                text->setAttributeValue (Style::style,Style::font::size::s12px);
            }

            {
                auto tContainer = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                tContainer->addStyleClass (Bootstrap::Grid::col_full_12);
                tContainer->setContentAlignment (AlignmentFlag::Center);
                auto text = tContainer->addWidget (cpp14::make_unique<WText>(tcItem.value ()->AdSoyad ().toStdString ()));
                text->addStyleClass (Bootstrap::Label::Default);
                text->setAttributeValue (Style::style,Style::font::size::s12px);
            }

            {
                auto tContainer = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                tContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                tContainer->setContentAlignment (AlignmentFlag::Center);
                auto text = tContainer->addWidget (cpp14::make_unique<WText>(tcItem.value ()->CepTelefonu ().toStdString ()));
                tContainer->setAttributeValue (Style::style,Style::font::size::s12px
                                         +Style::color::color (Style::color::White::Snow)
                                         +Style::background::color::color (Style::color::Green::DarkGreen));
            }
            {
                auto tContainer = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                tContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                tContainer->setContentAlignment (AlignmentFlag::Center);
                auto text = tContainer->addWidget (cpp14::make_unique<WText>(tcItem.value ()->Mahalle ().toStdString ()));
                tContainer->setAttributeValue (Style::style,Style::font::size::s12px
                                         +Style::color::color (Style::color::White::Snow)
                                         +Style::background::color::color (Style::color::Purple::Indigo));
            }



        }
    }
}

void DilekceView::initDilekceView()
{
    this->Content ()->clear ();
    this->Content ()->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    this->Content ()->setMargin (20,Side::Top|Side::Bottom);
    this->Content ()->setWidth (WLength("100%"));

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto text = container->addWidget (cpp14::make_unique<WText>(this->oid ().value ().to_string ()));
        text->addStyleClass (Bootstrap::Label::Default);
    }

    // Dilekçe Konu
    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_6
                                  +Bootstrap::Grid::Medium::col_md_6
                                  +Bootstrap::Grid::Small::col_sm_6
                                  +Bootstrap::Grid::ExtraSmall::col_xs_12);
        auto text = container->addWidget (cpp14::make_unique<WText>(this->konu ().toStdString ()));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_1
                                  +Bootstrap::Grid::Medium::col_md_1
                                  +Bootstrap::Grid::Small::col_sm_1
                                  +Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto text = container->addWidget (cpp14::make_unique<WText>(std::to_string (this->sayi ())));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_2
                                  +Bootstrap::Grid::Medium::col_md_2
                                  +Bootstrap::Grid::Small::col_sm_2
                                  +Bootstrap::Grid::ExtraSmall::col_xs_6);
        QDateTime _time;
        _time.setTime_t (this->oid ().value ().get_time_t ());
        auto text = container->addWidget (cpp14::make_unique<WText>(_time.date ().toString ("dd/MM/yyyy").toStdString ()
                                                                    +" - " + _time.time ().toString ("hh:mm").toStdString ()));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                  +Bootstrap::Grid::Medium::col_md_3
                                  +Bootstrap::Grid::Small::col_sm_3
                                  +Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto text = container->addWidget (cpp14::make_unique<WText>(this->icerikTipi ().toStdString ()));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto text = container->addWidget (cpp14::make_unique<WText>(this->icerik ().toStdString ()));
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setMargin (20,Side::Top|Side::Bottom);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::OldLace));
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        auto text = container->addWidget (cpp14::make_unique<WText>("Orjinal Evrak"));
        text->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setMargin (20,Side::Top|Side::Bottom);
        container->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
    }

    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->addWidget (cpp14::make_unique<WText>("<b>Açıklama Ekle</b>"));
        auto text = container->addWidget (cpp14::make_unique<WTextEdit>());
        text->setHeight (350);
        container->setMargin (20,Side::Top|Side::Bottom);
        auto saveBtn = container->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);
    }




}

void DilekceView::initCevapView()
{
    this->Footer ()->clear ();
    this->Footer ()->setMargin (20,Side::Top);
    this->Footer ()->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    this->Footer ()->setWidth (WLength("100%"));
    this->Footer ()->addWidget (cpp14::make_unique<WText>("<strong>Cevap Ekle</strong>"));
    this->Footer ()->addWidget (cpp14::make_unique<WBreak>());
    this->Footer ()->addWidget (cpp14::make_unique<FileUploaderWidget>(this->db ()));
    this->Footer ()->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::LavenderBlush));
}
