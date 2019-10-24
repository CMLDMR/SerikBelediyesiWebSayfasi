#include "dilekceview.h"
#include <QDateTime>
#include "dilekceaciklama.h"


DilekceView::DilekceView(Dilekce *_dilekce, mongocxx::database* _db ,  User *_user, bool _mPublicLink)
    : ContainerWidget (_mPublicLink ? "Serik Belediyesi Dilekçe Yönetim Sistemi ©2019" : ""),Dilekce(_dilekce),DilekceManager(_db),mPublicLink(_mPublicLink)
{

    if( !mPublicLink )
    {
        mUser = _user;
    }


    mTCManager = new TCManager(this->db ());

    if( _dilekce )
    {
        if( !mPublicLink )
        {
            this->initTCView ();
        }
        this->initDilekceView ();

        if( !mPublicLink )
        {
            this->initCevapView ();
        }
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

        auto fileName = this->downloadFileWeb (this->dilekceOid ());
        if( fileName.empty () )
        {
            this->showMessage ("Hata","Taranmış Dosya Yok");
        }else{

            Wt::WLink link = Wt::WLink(LinkType::Url,fileName);
            link.setTarget(Wt::LinkTarget::NewWindow);

            std::unique_ptr<Wt::WAnchor> anchor =
                    Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                    "Orjinal Evrak");

            auto text = container->addWidget (std::move(anchor));
            text->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->setMargin (20,Side::Top|Side::Bottom);
            container->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
            container->decorationStyle ().setCursor (Cursor::PointingHand);

        }

    }
        this->Content ()->addWidget (cpp14::make_unique<WBreak>());

    {
        auto eklerList = this->EkOidList ();
        int count = 1;
        for( auto item : eklerList )
        {
            auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            auto fileName = this->downloadFileWeb (item);
            if( fileName.empty () )
            {
                this->showMessage ("Hata","Taranmış Dosya Yok");
            }else{
                Wt::WLink link = Wt::WLink(LinkType::Url,fileName);
                link.setTarget(Wt::LinkTarget::NewWindow);

                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                        "Ek " + std::to_string (count++));
                auto text = container->addWidget (std::move(anchor));
                text->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
                container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                container->setMargin (5,AllSides);
                container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
                container->decorationStyle ().setCursor (Cursor::PointingHand);
            }
        }
    }
        this->Content ()->addWidget (cpp14::make_unique<WBreak>());



    {
        mAciklamaContainer = this->Content ()->addWidget (cpp14::make_unique<ContainerWidget>());
        mAciklamaContainer->setWidth (WLength("100%"));
        auto _dilekceOid = this->oid ().value ().to_string ();
        auto list = this->findAciklama (_dilekceOid);
        for( auto item : list )
        {
            this->addAciklama (item);
        }
    }

    if( !mPublicLink ){
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->addWidget (cpp14::make_unique<WText>("<b>Açıklama Ekle</b>"));
        auto text = container->addWidget (cpp14::make_unique<WTextEdit>());
        text->setHeight (350);
        container->setMargin (20,Side::Top|Side::Bottom);
        auto saveBtn = container->addWidget (cpp14::make_unique<WPushButton>("Açıklamayı Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);
        saveBtn->clicked ().connect ([=](){

            std::cout << "Açıklama Kaydet" << std::endl;
            if( text->text ().toUTF8 ().size () < 50 )
            {
                this->showMessage ("UYARI","Açıklamanız Yeterli Uzunlukta Değil. Min: 50 Karakter Yazmalısınız");
            }else{
                DilekceAciklama *aciklama = new DilekceAciklama();
                aciklama->setDilekceOid (this->oid ().value ().to_string ());
                aciklama->setAciklama (text->text ().toUTF8 ());
                aciklama->setPersonelOid (mUser->oid ().value ().to_string ());
                aciklama->setJulianDay (QDate::currentDate ().toJulianDay ());
                aciklama->setPersonelName (mUser->AdSoyad ());
                aciklama->setSaat (QTime::currentTime ().toString ("hh:mm").toStdString ());
                if( this->insertAciklama (aciklama) ){
                    this->addAciklama (*aciklama);
                    text->setText ("");
                    this->showMessage ("UYARI","Açıklamanız Başarılı Bir Şekilde Eklendi.");
                }else{
                    this->showMessage ("Hata","Bir Hata Oluştu. Açıklama Eklenemedi.");
                }
                delete aciklama;
            }
        });
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

void DilekceView::addAciklama(const DilekceAciklama &aciklama)
{
    auto container = mAciklamaContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    container->setPositionScheme (PositionScheme::Relative);

    if( !mPublicLink ){
        auto closeAciklama = container->addWidget (cpp14::make_unique<WContainerWidget>());
        closeAciklama->setPositionScheme (PositionScheme::Absolute);
        closeAciklama->setOffsets (0,Side::Right|Side::Top);
        auto text = closeAciklama->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
        text->setAttributeValue (Style::style,Style::font::size::s14px);
        closeAciklama->addStyleClass (Bootstrap::Label::Danger);
        closeAciklama->decorationStyle ().setCursor (Cursor::PointingHand);
    }

    {
        auto closeAciklama = container->addWidget (cpp14::make_unique<WContainerWidget>());
        closeAciklama->setPositionScheme (PositionScheme::Absolute);
        closeAciklama->setOffsets (0,Side::Left|Side::Top);
        auto str = aciklama.Tarih ().toStdString ();
        if( !mPublicLink ){
            str += " | " + aciklama.Saat().toStdString ();
            str += " - " + aciklama.PersonelName().toStdString ();
        }
        auto text = closeAciklama->addWidget (cpp14::make_unique<WText>(str));
        text->setAttributeValue (Style::style,Style::font::size::s14px);
        closeAciklama->addStyleClass (Bootstrap::Label::Primary);

    }


    container->setPadding (20,Side::Right|Side::Top|Side::Left);
    container->setMargin (10,Side::Top);
    container->setWidth (WLength("100%"));
    container->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::AntiqueWhite));
    container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    container->addWidget (cpp14::make_unique<WText>(aciklama.Aciklama ().toStdString ()));
    mAciklamaContainer->addWidget (cpp14::make_unique<WBreak>());
}
