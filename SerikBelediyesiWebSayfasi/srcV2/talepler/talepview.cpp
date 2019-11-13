#include "talepview.h"

TalepView::TalepView(Talep &talepItem, mongocxx::database *_db, User *_mUser, bool _mPublicLink)
    :ContainerWidget (_mPublicLink ? "Serik Belediyesi Talep/Şikayet Yönetim Sistemi ©2019" : ""),
      Talep (talepItem),
      TalepManager (_db),
      mPublicLink(_mPublicLink)
{
    if( !mPublicLink )
    {
        mUser = _mUser;
    }

    if( mPublicLink )
    {
        auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)
                                      +Style::color::color (Style::color::White::Snow)
                                      +Style::font::size::s12px);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setWidth (WLength("100%"));
        {
            auto text = container->addWidget (cpp14::make_unique<WText>("<b>!Dikkat!</b>"));
            container->addWidget (cpp14::make_unique<WBreak>());
        }

        {
            auto text = container->addWidget (cpp14::make_unique<WText>("Bu Sayfa Sadece Bilgilendirme Amaçlıdır"));
            container->addWidget (cpp14::make_unique<WBreak>());
        }
        {
            auto text = container->addWidget (cpp14::make_unique<WText>("Resmi Olarak Kullanılamaz"));
            container->addWidget (cpp14::make_unique<WBreak>());
        }

    }

    if( talepItem )
    {
        if( !mPublicLink )
        {
            mTCManager = new TCManager(this->db ());
            mPersonelManager = new PersonelManager(this->db ());
            this->initTCView ();
        }
        this->initTalepView ();

//        this->initCevapView ();
    }

}

void TalepView::initTCView()
{
    auto _tcOid = this->tcOid ();
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

void TalepView::initTalepView()
{
    this->Content ()->clear ();
    this->Content ()->setMargin (20,Side::Top|Side::Bottom);
    this->Content ()->setWidth (WLength("100%"));


    auto container = this->Content ()->addWidget (cpp14::make_unique<ContainerWidget>());
    container->addStyleClass (Bootstrap::Grid::col_full_12);
    container->setWidth (WLength("100%"));
    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::LightSkyBlue));
    container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

    auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass (Bootstrap::Grid::row);

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::col_full_12);
        _container->setContentAlignment (AlignmentFlag::Center);
        _container->setMargin (5,Side::Bottom|Side::Top);
        auto title = _container->addWidget (cpp14::make_unique<WText>("<b>Talep/Şikayet Bilgileri</b>"));
        title->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        title->addStyleClass ("textShadow");
    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                   Bootstrap::Grid::Medium::col_md_4+
                                   Bootstrap::Grid::Small::col_sm_6+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        _container->setContentAlignment (AlignmentFlag::Center);
        _container->setMargin (5,Side::Bottom|Side::Top);
        auto title = _container->addWidget (cpp14::make_unique<WText>("<b>Görevli Birim</b>"));
        title->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        title->addStyleClass ("textShadow");
    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_8+
                                   Bootstrap::Grid::Medium::col_md_8+
                                   Bootstrap::Grid::Small::col_sm_6+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        _container->setContentAlignment (AlignmentFlag::Center);
        _container->setMargin (5,Side::Bottom|Side::Top);
        _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Linen));
        _container->addWidget (cpp14::make_unique<WText>(this->birim ().toStdString ()));
    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                   Bootstrap::Grid::Medium::col_md_3+
                                   Bootstrap::Grid::Small::col_sm_3+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        _container->setContentAlignment (AlignmentFlag::Center);
        _container->setMargin (5,Side::Bottom|Side::Top);
        auto title = _container->addWidget (cpp14::make_unique<WText>(this->tarih ().toStdString ()));
        title->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        title->addStyleClass ("textShadow");
    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                   Bootstrap::Grid::Medium::col_md_3+
                                   Bootstrap::Grid::Small::col_sm_3+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        _container->setContentAlignment (AlignmentFlag::Center);
        _container->setMargin (5,Side::Bottom|Side::Top);
        auto title = _container->addWidget (cpp14::make_unique<WText>(this->saat ().toStdString ()));
        title->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        title->addStyleClass ("textShadow");
    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                   Bootstrap::Grid::Medium::col_md_3+
                                   Bootstrap::Grid::Small::col_sm_3+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        _container->setContentAlignment (AlignmentFlag::Center);
        _container->setMargin (5,Side::Bottom|Side::Top);
        auto title = _container->addWidget (cpp14::make_unique<WText>(this->mahalle ().toStdString ()));
        title->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        title->addStyleClass ("textShadow");
    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                   Bootstrap::Grid::Medium::col_md_3+
                                   Bootstrap::Grid::Small::col_sm_3+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        _container->setContentAlignment (AlignmentFlag::Center);
        _container->setMargin (5,Side::Bottom|Side::Top);
        _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        _container->setAttributeValue (Style::style,Style::background::color::color (this->durumColor ().toStdString ()));
        auto title = _container->addWidget (cpp14::make_unique<WText>(this->durum ().toStdString ()));
        title->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        title->addStyleClass ("textShadow");
    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::col_full_12);
        _container->setContentAlignment (AlignmentFlag::Center);
        _container->setMargin (5,Side::Bottom|Side::Top);
        auto title = _container->addWidget (cpp14::make_unique<WText>(this->konu ().toStdString ()));
        title->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        title->addStyleClass ("textShadow");
    }



    {
        auto adrescontainer = this->Content ()->addWidget (cpp14::make_unique<ContainerWidget>());
        adrescontainer->addStyleClass (Bootstrap::Grid::col_full_12);
        adrescontainer->setWidth (WLength("100%"));
        adrescontainer->setAttributeValue (Style::style,Style::background::color::color ("#292929"));
        adrescontainer->setMargin (20,Side::Top|Side::Bottom);
        adrescontainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        adrescontainer->setContentAlignment (AlignmentFlag::Left);
        auto text = adrescontainer->addWidget (cpp14::make_unique<WText>(this->adres ().toStdString ()));
        text->addStyleClass ("textShadow");
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
    }


    {

        auto cursor = this->findTalepSubItem (this->oid ());

        auto eventContainer = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        eventContainer->addStyleClass (Bootstrap::Grid::col_full_12);

        for( auto item : cursor )
        {

            auto itemContainer = eventContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            itemContainer->setHeight (100);
            itemContainer->setWidth (WLength("100%"));
            itemContainer->setMargin (10,Side::Top|Side::Bottom);
            itemContainer->setAttributeValue (Style::style,Style::background::color::color (item.typeColor ().toStdString ()));
            itemContainer->setPositionScheme (PositionScheme::Relative);
            itemContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            itemContainer->setContentAlignment (AlignmentFlag::Center);


            {
                auto titleContainer = itemContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                titleContainer->setPositionScheme (PositionScheme::Absolute);
                titleContainer->setOffsets (WLength(0),Side::Left|Side::Top);

                std::string typeName;
//                switch (item.type () ) {
//                case TalepSubItem::ItemType::Aciklama:
//                    typeName = "Açıklama";
//                    break;
//                case TalepSubItem::ItemType::Pdf:
//                    typeName = "Pdf";
//                    break;
//                case TalepSubItem::ItemType::Sms:
//                    typeName = "Sms";
//                    break;
//                case TalepSubItem::ItemType::Fotograf:
//                    typeName = "Fotograf";
//                    break;

//                default:
//                    break;
//                }

#define MAKE_STRINGS(VAR) #VAR


                typeName = MAKE_STRINGS (item.type());


                auto titleText = titleContainer->addWidget (cpp14::make_unique<WText>(item.personelName ().toStdString ()+
                                                                                      " / "+
                                                                                      item.tarih ().toStdString ()+
                                                                                      " " + item.saat ().toStdString ()+
                                                                                      " " + item.typeStr ().toStdString () ));
                titleText->addStyleClass ("textShadow");
                titleText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            }

            if( item.type () == TalepSubItem::ItemType::Aciklama )
            {
                auto sContainer = itemContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                sContainer->setPositionScheme (PositionScheme::Absolute);
                sContainer->setWidth (WLength("100%"));
                sContainer->setHeight (WLength("100%"));
                auto vLayout = sContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
                auto text = vLayout->addWidget (cpp14::make_unique<WText>(item.aciklama ().toStdString ()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->addStyleClass ("textShadow");
                text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            }

            if( item.type () == TalepSubItem::ItemType::Fotograf )
            {
                auto fotoPath = this->downloadFileWeb (item.fotografOid ());
                itemContainer->setOverflow (Overflow::Hidden);

                auto sContainer = itemContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                sContainer->setPositionScheme (PositionScheme::Absolute);
                sContainer->setWidth (WLength("100%"));
                sContainer->setHeight (WLength("100%"));
                sContainer->decorationStyle ().setCursor (Cursor::PointingHand);

                Wt::WLink link = Wt::WLink(LinkType::Url,fotoPath);
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor = Wt::cpp14::make_unique<Wt::WAnchor>(link);
                auto sContainer_ = anchor->addNew<Wt::WContainerWidget>();
                sContainer_->setWidth (WLength("100%"));
                sContainer_->setHeight (WLength("100%"));
                sContainer_->setAttributeValue (Style::style,Style::background::url (fotoPath)+
                                               Style::background::repeat::norepeat+
                                               Style::background::size::contain+
                                               Style::background::position::center_center);
                sContainer->addWidget (std::move(anchor));
            }

            if( item.type () == TalepSubItem::ItemType::Pdf )
            {
                auto pdfPath = this->downloadFileWeb (item.pdfOid ());
                itemContainer->setOverflow (Overflow::Hidden);

                auto sContainer = itemContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                sContainer->setPositionScheme (PositionScheme::Absolute);
                sContainer->setWidth (WLength("100%"));
                sContainer->setHeight (WLength("100%"));
                sContainer->decorationStyle ().setCursor (Cursor::PointingHand);

                Wt::WLink link = Wt::WLink(LinkType::Url,pdfPath);
                link.setTarget(Wt::LinkTarget::NewWindow);
                std::unique_ptr<Wt::WAnchor> anchor = Wt::cpp14::make_unique<Wt::WAnchor>(link);
                auto sContainer_ = anchor->addNew<Wt::WContainerWidget>();
                sContainer_->setWidth (WLength("100%"));
                sContainer_->setHeight (WLength("100%"));
                sContainer_->setAttributeValue (Style::style,Style::background::url ("icon/pdf.png")+
                                               Style::background::repeat::norepeat+
                                               Style::background::size::contain+
                                               Style::background::position::center_center);
                sContainer->addWidget (std::move(anchor));

            }

            if( item.type () == TalepSubItem::ItemType::Sms )
            {
                auto sContainer = itemContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                sContainer->setPositionScheme (PositionScheme::Absolute);
                sContainer->setWidth (WLength("100%"));
                sContainer->setHeight (WLength("100%"));
                auto vLayout = sContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
                auto text = vLayout->addWidget (cpp14::make_unique<WText>(item.sms ().toStdString ()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->addStyleClass ("textShadow");
                text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            }







        }


    }

}
