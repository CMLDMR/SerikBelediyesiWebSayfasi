#include "dilekceview.h"


DilekceView::DilekceView(Dilekce *_dilekce, mongocxx::database *_db)
    : ContainerWidget (),Dilekce(_dilekce),DB(_db)
{

    mTCManager = new TCManager(this->db ());

    if( _dilekce )
    {
        this->initTCView ();
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
