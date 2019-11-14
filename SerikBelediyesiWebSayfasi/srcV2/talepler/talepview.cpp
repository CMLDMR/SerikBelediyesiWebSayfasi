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
        this->initTalepView ();

        if( !mPublicLink )
        {
            mTCManager = new TCManager(this->db ());
            mPersonelManager = new PersonelManager(this->db ());
            this->initTCView ();
            this->initTalepCevap ();
        }else{
            this->Footer ()->clear ();
            this->Footer ()->setMargin (20,Side::Top|Side::Bottom);
            this->Footer ()->setWidth (WLength("100%"));


            auto container = this->Footer ()->addWidget (cpp14::make_unique<ContainerWidget>());
            container->addStyleClass (Bootstrap::Grid::col_full_12);
            container->setWidth (WLength("100%"));
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

            auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
            rContainer->addStyleClass (Bootstrap::Grid::row);

            if( this->durum ().toStdString () == TalepKey::DurumKey::Tamamlandi )
            {
                {
                    auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                    _container->addStyleClass (Bootstrap::Grid::col_full_12);
                    auto vLayout = _container->setLayout (cpp14::make_unique<WVBoxLayout>());
                    auto text = vLayout->addWidget (cpp14::make_unique<WText>("Bu Talep/Şikayet Tamamlandı"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                    text->addStyleClass ("textShadow");
                    text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+
                                             Style::font::weight::bold);
                    _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::DodgerBlue));
                    _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

                }
                return;
            }
        }

    }

}

Signal<NoClass> &TalepView::DurumChanged()
{
    return _durumChanged;
}

void TalepView::initTCView()
{
    auto _tcOid = this->tcOid ();
    printView ();
    if( !_tcOid.isEmpty () )
    {
        auto tcItem = mTCManager->Load_byOID ( _tcOid.toStdString () );

        std::cout << "TCNUMARASI: "<<_tcOid.toStdString () << std::endl;

        if( tcItem )
        {
            tcItem.value ()->printView ();

            this->Header ()->clear ();
            auto tcContainer = this->Header ()->addWidget (cpp14::make_unique<ContainerWidget>());
            tcContainer->addStyleClass (Bootstrap::Grid::col_full_12+Bootstrap::ImageShape::img_thumbnail);
            tcContainer->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
            tcContainer->setMargin (10,Side::Top);

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

    if( !mPublicLink )
    {
        if( this->mUser->Statu () == User::Mudur )
        {
            {
                auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                           Bootstrap::Grid::Medium::col_md_4+
                                           Bootstrap::Grid::Small::col_sm_6+
                                           Bootstrap::Grid::ExtraSmall::col_xs_6);
                _container->setContentAlignment (AlignmentFlag::Center);
                _container->setMargin (5,Side::Bottom|Side::Top);
                _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::Linen));
                _container->addWidget (cpp14::make_unique<WText>(this->birim ().toStdString ()));
            }

            {
                auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                _container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                           Bootstrap::Grid::Medium::col_md_4+
                                           Bootstrap::Grid::Small::col_sm_12+
                                           Bootstrap::Grid::ExtraSmall::col_xs_12);
                _container->setContentAlignment (AlignmentFlag::Center);
                _container->setMargin (5,Side::Bottom|Side::Top);
                _container->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
                auto gorevlendirBtn = _container->addWidget (cpp14::make_unique<WText>("<b>Personel Görevlendir</b>"));
                _container->decorationStyle ().setCursor (Cursor::PointingHand);
                _container->clicked ().connect (this,&TalepView::gorevliEkle);
            }
        }
    }else{
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

    if( !mPublicLink ){
        mGorevliPersonelContainer = this->Content ()->addWidget (cpp14::make_unique<ContainerWidget>());
        mGorevliPersonelContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        mGorevliPersonelContainer->setWidth (WLength("100%"));
        mGorevliPersonelContainer->setAttributeValue (Style::style,Style::background::color::color ("#6f6f6f"));
        mGorevliPersonelContainer->setMargin (20,Side::Top|Side::Bottom);
        mGorevliPersonelContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        mGorevliPersonelContainer->setContentAlignment (AlignmentFlag::Left);
        auto text = mGorevliPersonelContainer->addWidget (cpp14::make_unique<WText>("Görevli Personeller"));
        text->addStyleClass ("textShadow");
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        this->updateGorevliPersonelWidget ();
    }


    {

        auto cursor = this->findTalepSubItem (this->oid ());

        auto eventContainer = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        eventContainer->addStyleClass (Bootstrap::Grid::col_full_12);

        for( auto item : cursor )
        {

            auto itemContainer = eventContainer->addWidget (cpp14::make_unique<WContainerWidget>());

            if( item.type() == TalepSubItem::ItemType::Log )
            {
            }else if ( item.type() == TalepSubItem::ItemType::Aciklama ) {
            }else if ( item.type() == TalepSubItem::ItemType::Sms ) {
            }else{
                itemContainer->setHeight (100);
            }


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
                sContainer->setPositionScheme (PositionScheme::Relative);
                sContainer->setMargin (10,Side::Top);
                sContainer->setWidth (WLength("100%"));
                auto vLayout = sContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
                auto text = vLayout->addWidget (cpp14::make_unique<WText>(item.aciklama ().toStdString ()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->addStyleClass ("textShadow");
                text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            }

            if( item.type () == TalepSubItem::ItemType::Log )
            {
                auto sContainer = itemContainer->addWidget (cpp14::make_unique<WContainerWidget>());
                sContainer->setPositionScheme (PositionScheme::Relative);
                sContainer->setMargin (10,Side::Top);
                sContainer->setWidth (WLength("100%"));
                auto vLayout = sContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
                auto text = vLayout->addWidget (cpp14::make_unique<WText>(item.log ().toStdString ()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
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
                sContainer->setPositionScheme (PositionScheme::Relative);
                sContainer->setMargin (10,Side::Top);
                sContainer->setWidth (WLength("100%"));
                auto vLayout = sContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
                auto text = vLayout->addWidget (cpp14::make_unique<WText>(item.sms ().toStdString ()),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->addStyleClass ("textShadow");
                text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            }







        }


    }

}

void TalepView::initTalepCevap()
{

    this->Footer ()->clear ();
    this->Footer ()->setMargin (20,Side::Top);
    this->Footer ()->setMargin (150,Side::Bottom);
    this->Footer ()->setWidth (WLength("100%"));


    auto container = this->Footer ()->addWidget (cpp14::make_unique<ContainerWidget>());
    container->addStyleClass (Bootstrap::Grid::col_full_12);
    container->setWidth (WLength("100%"));

    auto rContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass (Bootstrap::Grid::row);

    if( this->durum ().toStdString () == TalepKey::DurumKey::Tamamlandi )
    {
        {
            auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            _container->addStyleClass (Bootstrap::Grid::col_full_12);
            auto vLayout = _container->setLayout (cpp14::make_unique<WVBoxLayout>());
            auto text = vLayout->addWidget (cpp14::make_unique<WText>("Bu Talep/Şikayet Tamamlandı"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
            text->addStyleClass ("textShadow");
            text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+
                                     Style::font::weight::bold);
            _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::DodgerBlue));
            _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

        }
        return;
    }


    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                   Bootstrap::Grid::Medium::col_md_2+
                                   Bootstrap::Grid::Small::col_sm_4+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto vLayout = _container->setLayout (cpp14::make_unique<WHBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Diğer Ekle"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->addStyleClass ("textShadow");
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+
                                 Style::font::weight::bold);
        _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::SlateGray));
        _container->decorationStyle ().setCursor (Cursor::PointingHand);




        auto popupPtr = Wt::cpp14::make_unique<Wt::WPopupMenu>();
        auto popup = popupPtr.get();

        vLayout->addChild (std::move(popupPtr));


        auto smsGonderMenu = popup->addItem("Sms Gönder");
        smsGonderMenu->triggered().connect([=] {
            this->addEventItem (TalepSubItem::ItemType::Sms);
        });
        smsGonderMenu->decorationStyle ().setCursor (Cursor::PointingHand);

        auto fotoEkleMenu = popup->addItem("Fotoğraf Ekle");
        fotoEkleMenu->triggered().connect([=] {
            this->addEventItem (TalepSubItem::ItemType::Fotograf);
        });
        fotoEkleMenu->decorationStyle ().setCursor(Cursor::PointingHand);


        auto pdfEkleMenu = popup->addItem("Pdf Ekle");
        pdfEkleMenu->triggered().connect([=] {
            this->addEventItem (TalepSubItem::ItemType::Pdf);
        });
        pdfEkleMenu->decorationStyle ().setCursor (Cursor::PointingHand);

        _container->clicked ().connect ([=](){
            popup->popup (_container,Orientation::Vertical);
        });

    }


    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                   Bootstrap::Grid::Medium::col_md_2+
                                   Bootstrap::Grid::Small::col_sm_4+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto vLayout = _container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Tamamla"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->addStyleClass ("textShadow");
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+
                                 Style::font::weight::bold);
        _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::DodgerBlue));
        _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        _container->decorationStyle ().setCursor (Cursor::PointingHand);

        _container->clicked ().connect ([=](){
            this->setDurum (TalepKey::DurumKey::TeyitEdilmemis.c_str ());
            if( this->updateTalep (this) )
            {
                TalepSubItem *item = new TalepSubItem;
                item->setType (TalepSubItem::ItemType::Log);
                item->setLog ("Talep/Şikayet Tamamlandı, Doğrulanmadı!");
                item->setTalepOid (this->oid ());
                item->setPersonelOid (this->mUser->UserOid ());
                item->setPersonelName (this->mUser->AdSoyad ().c_str ());
                if( this->insertTalepSubItem (item) ){
                    this->initTalepView ();
                    _durumChanged.emit (NoClass());
                }
            }
        });

    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                   Bootstrap::Grid::Medium::col_md_2+
                                   Bootstrap::Grid::Small::col_sm_4+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto vLayout = _container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Red Et"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->addStyleClass ("textShadow");
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+
                                 Style::font::weight::bold);
        _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::Crimson));
        _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        _container->decorationStyle ().setCursor (Cursor::PointingHand);

        _container->clicked ().connect ([=](){

            if( this->durum ().toStdString ()== TalepKey::DurumKey::RedEdildi )
            {
                this->showMessage ("Hata","Bu Talep/Şikayeti Zaten Red Ettin!");
            }else{
                this->addLogEventItem (TalepKey::DurumKey::RedEdildi,"Talep/Şikayet Red Edildi. Gerekçe: ");
            }


        });

    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                   Bootstrap::Grid::Medium::col_md_2+
                                   Bootstrap::Grid::Small::col_sm_4+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto vLayout = _container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Tekrar Aç"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->addStyleClass ("textShadow");
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+
                                 Style::font::weight::bold);
        _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::LightSeaGreen));
        _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        _container->decorationStyle ().setCursor (Cursor::PointingHand);

        _container->clicked ().connect ([=](){
            if( this->durum ().toStdString ()== TalepKey::DurumKey::DevamEdiyor )
            {
                this->showMessage ("Hata","Bu Talep/Şikayet Zaten Devam Ediyor!");
            }else{
                this->addLogEventItem (TalepKey::DurumKey::DevamEdiyor,"Talep/Şikayet Tekrar Açıldı. Gerekçe: ");
            }
        });
    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                   Bootstrap::Grid::Medium::col_md_2+
                                   Bootstrap::Grid::Small::col_sm_4+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto vLayout = _container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Beklemeye Al"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->addStyleClass ("textShadow");
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+
                                 Style::font::weight::bold);
        _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Orange::GoldenRod));
        _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        _container->decorationStyle ().setCursor (Cursor::PointingHand);

        _container->clicked ().connect ([=](){
            if( this->durum ().toStdString ()== TalepKey::DurumKey::Beklemede )
            {
                this->showMessage ("Hata","Bu Talep/Şikayet Zaten Beklemede!");
            }else{
                this->addLogEventItem (TalepKey::DurumKey::Beklemede,"Talep/Şikayet Beklemeye Alındı. Gerekçe: ");
            }
        });
    }

    {
        auto _container = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                   Bootstrap::Grid::Medium::col_md_2+
                                   Bootstrap::Grid::Small::col_sm_4+
                                   Bootstrap::Grid::ExtraSmall::col_xs_6);
        auto vLayout = _container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Açıklama Ekle"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        text->addStyleClass ("textShadow");
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+
                                 Style::font::weight::bold);
        _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Grey::DimGray));
        _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        _container->decorationStyle ().setCursor (Cursor::PointingHand);
        _container->clicked ().connect ([=](){
            this->addEventItem (TalepSubItem::ItemType::Aciklama);
        });
    }




}

void TalepView::updateGorevliPersonelWidget()
{
    mGorevliPersonelContainer->clear ();

    auto text = mGorevliPersonelContainer->addWidget (cpp14::make_unique<WText>("<b>Görevli Personeller</b>"));
    text->setMargin (10,Side::Right);
    text->addStyleClass ("textShadow");
    text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
    mGorevliPersonelContainer->addWidget (cpp14::make_unique<WBreak>());

    auto prContainer = mGorevliPersonelContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    prContainer->addStyleClass (Bootstrap::Grid::row);

    for( auto per : this->GorevliList () )
    {
        auto container = prContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        container->setHeight (152);
        container->setOverflow (Overflow::Hidden);
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_1
                                  +Bootstrap::Grid::Medium::col_md_1
                                  +Bootstrap::Grid::Small::col_sm_2
                                  +Bootstrap::Grid::ExtraSmall::col_xs_3);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setPadding (0,AllSides);
        container->setContentAlignment (AlignmentFlag::Center);
        auto photoFilePath = this->downloadFileWeb (per.FotoOid ());


        auto photoContainer = container->addWidget (cpp14::make_unique<ContainerWidget>());
        photoContainer->setWidth (WLength("100%"));
        photoContainer->setHeight (90);
        photoContainer->setAttributeValue (Style::style,Style::background::url (photoFilePath)
                                           +Style::background::size::cover
                                           +Style::background::repeat::norepeat);

        container->addWidget (cpp14::make_unique<WBreak>());
        auto adSoyadText = container->addWidget (cpp14::make_unique<WText>(per.AdSoyad ().toStdString ()));
        adSoyadText->setAttributeValue (Style::style,Style::font::size::s10px);

        auto deleteContainer = container->addWidget (cpp14::make_unique<WContainerWidget>());
        deleteContainer->setPositionScheme (PositionScheme::Absolute);
        deleteContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        deleteContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed));
        deleteContainer->setPadding (2,Side::Right|Side::Left);
        auto delText = deleteContainer->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
        delText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::size::s9px);
        deleteContainer->setOffsets (0,Side::Top|Side::Right);
        deleteContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        deleteContainer->setAttributeValue (Style::dataoid,per.oid ().value ().to_string ());

        deleteContainer->clicked ().connect ([=](){
            auto messageBox =
                    deleteContainer->addChild(Wt::cpp14::make_unique<Wt::WMessageBox>(
                                                  "Uyarı",
                                                  "<p>Bu Personeli Kaldırmak İstediğinize Emin misiniz<b>?</b></p>",
                                                  Wt::Icon::Information,
                                                  Wt::StandardButton::Yes | Wt::StandardButton::No));
            messageBox->setModal(false);
            messageBox->buttonClicked().connect([=] {
                if( messageBox->buttonResult () == Wt::StandardButton::Yes )
                {
                    Personel per;
                    per.setOid (deleteContainer->attributeValue (Style::dataoid).toUTF8 ());
                    this->DeleteGorevliPersonel (per);
                    if( this->updateTalep (this) ){
                        this->updateGorevliPersonelWidget ();
                    }else{
                        std::cout << "can not remove personel Widget" << std::endl;
                    }
                    per.printView ();

                }else{
                    deleteContainer->removeChild (messageBox);
                }
            });
            messageBox->show();
        });
    }
}

void TalepView::gorevliEkle()
{

    if( this->durum ().toStdString ()== TalepKey::DurumKey::Tamamlandi )
    {
        this->showMessage ("Uyarı","Bu Talep/Şikayet Tamamlanmış. Üzerinde İşlem Yapamazsınız");
        return;
    }


    auto dialog = WApplication::instance ()->root ()->addChild (cpp14::make_unique<WDialog>());
    dialog->setHeight (450);

    dialog->titleBar ()->addWidget (cpp14::make_unique<WText>("Personel Görevlendir | <b>" + this->mUser->Birimi () + " Listesi</b>"));


    dialog->contents ()->setOverflow (Overflow::Scroll);
    auto container = dialog->contents ()->addWidget (cpp14::make_unique<ContainerWidget>());



    auto list = mPersonelManager->PersonelList (this->mUser->Birimi ().c_str ());


    for( auto personel : list )
    {


        auto _container = container->addWidget (cpp14::make_unique<WContainerWidget>());
        _container->setMargin (5,Side::Bottom);
        _container->setWidth (WLength("100%"));
        _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

        auto personelOid = personel.oid ();
        if( personelOid )
        {
            _container->setAttributeValue (Style::dataoid,personelOid.value ().to_string ());
        }else{
            _container->setAttributeValue (Style::dataoid,"null");
        }

        if( personel.AdSoyad ().toStdString () != this->mUser->AdSoyad () )
        {
            _container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
        }else{
            _container->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
            _container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::FireBrick)+Style::color::color (Style::color::White::Snow));
        }


        auto gBtn = _container->addWidget (cpp14::make_unique<WText>("Görevlendir"));
        gBtn->setMargin (10,Side::Left|Side::Right);
        gBtn->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
        gBtn->decorationStyle ().setCursor (Cursor::PointingHand);
        gBtn->setPadding (10,Side::Left|Side::Right);
        _container->addWidget (cpp14::make_unique<WText>(personel.AdSoyad ().toStdString ()));
        container->addWidget (cpp14::make_unique<WBreak>());

        gBtn->clicked ().connect ([=](){

            if( this->mUser->AdSoyad () == personel.AdSoyad ().toStdString ())
            {
                WApplication::instance ()->root ()->removeChild (dialog);
                this->showMessage ("Hata","Kendini Görevlendiremezsin");
                return;
            }

            if( _container->attributeValue (Style::dataoid).toUTF8 () != "null" )
            {
                Personel per;
                per.setOid (personel.oid ().value ().to_string ());
                per.setAdSoyad (personel.AdSoyad ());
                per.setFotoOid (personel.FotoOid ());
                this->AddGorevliPersonel (per);
                if( this->updateTalep (this) ){
                    WApplication::instance ()->root ()->removeChild (dialog);
                    this->updateGorevliPersonelWidget ();
                }
            }
        });
    }




    auto closeBtn = dialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kapat"));
    closeBtn->clicked ().connect ([=](){
        WApplication::instance ()->root ()->removeChild (dialog);
    });

    dialog->show ();

}

void TalepView::addEventItem(TalepSubItem::ItemType type_ , const std::string &islemGerekce )
{


    auto dialog = WApplication::instance ()->root ()->addChild (cpp14::make_unique<WDialog>());
    dialog->setHeight (450);

    switch (type_) {
    case TalepSubItem::ItemType::Pdf:
        dialog->titleBar ()->addWidget (cpp14::make_unique<WText>("<b>Pdf Ekle</b>"));
        break;
    case TalepSubItem::ItemType::Aciklama:
        dialog->titleBar ()->addWidget (cpp14::make_unique<WText>("<b>Açıklama Ekle</b>"));
        break;
    case TalepSubItem::ItemType::Sms:
        dialog->titleBar ()->addWidget (cpp14::make_unique<WText>("<b>Sms Gönder</b>"));
        break;
    case TalepSubItem::ItemType::Fotograf:
        dialog->titleBar ()->addWidget (cpp14::make_unique<WText>("<b>Fotoğraf Ekle</b>"));
        break;
    case TalepSubItem::ItemType::Log:
        dialog->titleBar ()->addWidget (cpp14::make_unique<WText>("<b>İşlem Yapma Gerekçesini Yazınız</b>"));
        break;
    default:
        break;
    }

    dialog->titleBar ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);


    auto container = dialog->contents ()->addWidget (cpp14::make_unique<ContainerWidget>());
    container->setContentAlignment (AlignmentFlag::Center);
    auto vlayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());


    switch (type_) {
    case TalepSubItem::ItemType::Pdf:
    {
        auto fileUploader = vlayout->addWidget (cpp14::make_unique<FileUploaderWidget>(this->db (),"PDF Yükle"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        fileUploader->setType (FileUploaderWidget::Pdf);
        auto ekleBtn = vlayout->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
        ekleBtn->addStyleClass (Bootstrap::Button::Success);
        ekleBtn->setEnabled (false);
        fileUploader->Uploaded ().connect ([=](){
            ekleBtn->setEnabled (true);
        });

        ekleBtn->clicked().connect([=](){
            auto pdfOid = this->uploadfile (fileUploader->fileLocation ());
            TalepSubItem* item = new TalepSubItem;
            item->setType (TalepSubItem::ItemType::Pdf);
            item->setTalepOid (this->oid ());
            item->setPdf (pdfOid.get_oid ().value.to_string ().c_str ());
            item->setPersonelOid (this->mUser->UserOid ());
            item->setPersonelName (this->mUser->AdSoyad ().c_str ());

            this->insertTalepSubItem (item);
            this->initTalepView ();
            WApplication::instance ()->root ()->removeChild (dialog);
        });
    }
        break;
    case TalepSubItem::ItemType::Aciklama:
    {
        auto textEdit = vlayout->addWidget (cpp14::make_unique<WTextEdit>());
        textEdit->setHeight (250);
        textEdit->setMinimumSize (WLength::Auto,250);

        auto ekleBtn = vlayout->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
        ekleBtn->clicked().connect([=](){
            TalepSubItem* item = new TalepSubItem;
            item->setType (TalepSubItem::ItemType::Aciklama);
            item->setTalepOid (this->oid ());
            item->setAciklama (textEdit->text ().toUTF8 ().c_str ());
            item->setPersonelOid (this->mUser->UserOid ());
            item->setPersonelName (this->mUser->AdSoyad ().c_str ());

            this->insertTalepSubItem (item);
            this->initTalepView ();
            WApplication::instance ()->root ()->removeChild (dialog);
        });


    }
        break;

    case TalepSubItem::ItemType::Log:
    {
        auto textEdit = vlayout->addWidget (cpp14::make_unique<WTextEdit>());
        textEdit->setText (islemGerekce);
        textEdit->setHeight (250);
        textEdit->setMinimumSize (WLength::Auto,250);

        auto ekleBtn = vlayout->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
        ekleBtn->clicked().connect([=](){
            TalepSubItem* item = new TalepSubItem;
            item->setType (TalepSubItem::ItemType::Log);
            item->setTalepOid (this->oid ());
            item->setLog (textEdit->text ().toUTF8 ().c_str ());
            item->setPersonelOid (this->mUser->UserOid ());
            item->setPersonelName (this->mUser->AdSoyad ().c_str ());
            if( this->insertTalepSubItem (item) ){

                this->initTalepView ();

            }
            WApplication::instance ()->root ()->removeChild (dialog);
        });


    }
        break;

    case TalepSubItem::ItemType::Sms:
    {
        auto textEdit = vlayout->addWidget (cpp14::make_unique<WTextEdit>());
        textEdit->setHeight (250);

        auto ekleBtn = vlayout->addWidget (cpp14::make_unique<WPushButton>("Gönder"));
        ekleBtn->clicked().connect([=](){
            TalepSubItem* item = new TalepSubItem;
            item->setType (TalepSubItem::ItemType::Sms);
            item->setTalepOid (this->oid ());
            item->setSms (textEdit->text ().toUTF8 ().c_str ());
            item->setPersonelOid (this->mUser->UserOid ());
            item->setPersonelName (this->mUser->AdSoyad ().c_str ());

            this->insertTalepSubItem (item);
            this->initTalepView ();
            WApplication::instance ()->root ()->removeChild (dialog);
        });
    }
        break;
    case TalepSubItem::ItemType::Fotograf:
    {
        auto fileUploader = vlayout->addWidget (cpp14::make_unique<FileUploaderWidget>(this->db (),"Resim Yükle"),0,AlignmentFlag::Center|AlignmentFlag::Middle);
        fileUploader->setType (FileUploaderWidget::Image);
        auto ekleBtn = vlayout->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
        ekleBtn->addStyleClass (Bootstrap::Button::Success);
        ekleBtn->setEnabled (false);
        fileUploader->Uploaded ().connect ([=](){
            ekleBtn->setEnabled (true);
        });

        ekleBtn->clicked().connect([=](){
            auto fotoOid = this->uploadfile (fileUploader->fileLocation ());
            TalepSubItem* item = new TalepSubItem;
            item->setType (TalepSubItem::ItemType::Fotograf);
            item->setTalepOid (this->oid ());
            item->setFotograf (fotoOid.get_oid ().value.to_string ().c_str ());
            item->setPersonelOid (this->mUser->UserOid ());
            item->setPersonelName (this->mUser->AdSoyad ().c_str ());

            this->insertTalepSubItem (item);
            this->initTalepView ();
            WApplication::instance ()->root ()->removeChild (dialog);
        });
    }
        break;
    default:
        break;
    }





    auto closeBtn = dialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kapat"));
    closeBtn->addStyleClass (Bootstrap::Button::Primary);
    closeBtn->clicked ().connect ([=](){
        WApplication::instance ()->root ()->removeChild (dialog);
    });

    dialog->show ();




}

void TalepView::addLogEventItem(const std::string &yeniDurum, const std::string &islemGerekcesi)
{
    auto dialog = WApplication::instance ()->root ()->addChild (cpp14::make_unique<WDialog>());
    dialog->setHeight (450);


    dialog->titleBar ()->addWidget (cpp14::make_unique<WText>("<b>İşlem Yapma Gerekçesini Yazınız</b>"));

    dialog->titleBar ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);


    auto container = dialog->contents ()->addWidget (cpp14::make_unique<ContainerWidget>());
    container->setContentAlignment (AlignmentFlag::Center);
    auto vlayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());

    {
        auto textEdit = vlayout->addWidget (cpp14::make_unique<WTextEdit>());
        textEdit->setText (islemGerekcesi);
        textEdit->setHeight (250);
        textEdit->setMinimumSize (WLength::Auto,250);

        auto ekleBtn = vlayout->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
        ekleBtn->clicked().connect([=](){
            TalepSubItem* item = new TalepSubItem;
            item->setType (TalepSubItem::ItemType::Log);
            item->setTalepOid (this->oid ());
            item->setLog (textEdit->text ().toUTF8 ().c_str ());
            item->setPersonelOid (this->mUser->UserOid ());
            item->setPersonelName (this->mUser->AdSoyad ().c_str ());
            if( this->insertTalepSubItem (item) ){
                this->setDurum (yeniDurum.c_str ());
                if( this->updateTalep (this) )
                {
                    this->initTalepView ();
                    _durumChanged.emit (NoClass());
                }
            }
            WApplication::instance ()->root ()->removeChild (dialog);
        });


    }


    auto closeBtn = dialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kapat"));
    closeBtn->addStyleClass (Bootstrap::Button::Primary);
    closeBtn->clicked ().connect ([=](){
        WApplication::instance ()->root ()->removeChild (dialog);
    });

    dialog->show ();
}
