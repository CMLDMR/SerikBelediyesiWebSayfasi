#include "dilekceview.h"
#include <QDateTime>
#include "dilekceaciklama.h"
#include "personel.h"

using namespace SerikBLDCore;

DilekceView::DilekceView(Dilekce *_dilekce, mongocxx::database* _db ,  User *_user, bool _mPublicLink, bool _mBilgi)
    : ContainerWidget (_mPublicLink ? "Serik Belediyesi Dilekçe Yönetim Sistemi ©2019" : ""),
      Dilekce(_dilekce),
      DilekceManager(_db),
      mPublicLink(_mPublicLink),
      mBilgi(_mBilgi)
{

    if( !mPublicLink )
    {
        mUser = _user;
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
            auto text = container->addWidget (cpp14::make_unique<WText>("Bu Evrak Sadece Bilgilendirme Amaçlıdır"));
            container->addWidget (cpp14::make_unique<WBreak>());
        }
        {
            auto text = container->addWidget (cpp14::make_unique<WText>("Resmi Evrak Yerine Kullanılamaz"));
            container->addWidget (cpp14::make_unique<WBreak>());
        }

    }



    if( _dilekce )
    {
        if( !mPublicLink )
        {
            mTCManager = new TCManager(this->db ());
            mPersonelManager = new PersonelManager(this->db ());
            this->initTCView ();
        }
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

    auto cevaplandi_ = this->Durum ();

    bool cevaplandi = cevaplandi_.toStdString ()== DilekceDurum::Cevaplandi ? true : false;


    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        auto text = container->addWidget (cpp14::make_unique<WText>(this->oid ().value ().to_string ()));
        text->addStyleClass (Bootstrap::Label::Default);
        container->addWidget (cpp14::make_unique<WBreak>());

        Wt::WLink link = Wt::WLink(LinkType::Url,WString("?type=dilekce&_id={1}").arg (this->oid ().value ().to_string ()).toUTF8 ());
        link.setTarget(Wt::LinkTarget::NewWindow);
        std::unique_ptr<Wt::WAnchor> anchor = Wt::cpp14::make_unique<Wt::WAnchor>(link,WString("http://www.serik.bel.tr/?type=dilekce&_id={1}").arg (this->oid ().value ().to_string ()));
//        anchor->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        container->addWidget (std::move(anchor));
        container->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::DimGray));
//        container->addStyleClass ("textShadow");
        container->setMargin (10,Side::Top|Side::Bottom);
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
        this->Content ()->addWidget (cpp14::make_unique<WBreak>());

    }



    if( !cevaplandi && !mPublicLink && !mBilgi ){
        mGorevliPersonelContainer = this->Content ()->addWidget (cpp14::make_unique<ContainerWidget>());
        mGorevliPersonelContainer->setMargin (20,Side::Top|Side::Bottom);
        mGorevliPersonelContainer->setWidth (WLength("100%"));
        mGorevliPersonelContainer->setAttributeValue (Style::style,Style::Border::top::border ("1px solid gray"));
        this->updateGorevliPersonelWidget ();
        this->Content ()->addWidget (cpp14::make_unique<WBreak>());
    }

    if( !mPublicLink ){
        auto havaleListContainer = this->Content ()->addWidget (cpp14::make_unique<ContainerWidget>());
        havaleListContainer->setMargin (20,Side::Top|Side::Bottom);
        havaleListContainer->setWidth (WLength("100%"));
        havaleListContainer->setAttributeValue (Style::style,Style::Border::top::border ("1px solid gray"));
        havaleListContainer->addStyleClass (Bootstrap::Grid::row);

        {
            auto havaleContainer = havaleListContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            havaleContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                            Bootstrap::Grid::Medium::col_md_6+
                                            Bootstrap::Grid::Small::col_sm_6+
                                            Bootstrap::Grid::ExtraSmall::col_xs_6);
            havaleContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            havaleContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::HoneyDew));
            auto layout = havaleContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
            layout->addWidget (cpp14::make_unique<WText>("<b>Havale</b>"),0,AlignmentFlag::Left);
            layout->addWidget (cpp14::make_unique<WText>("✍ "+this->birim ().toStdString ()),0,AlignmentFlag::Left);
        }

        {
            auto bilgiContainer = havaleListContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            bilgiContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_6+
                                            Bootstrap::Grid::Medium::col_md_6+
                                            Bootstrap::Grid::Small::col_sm_6+
                                            Bootstrap::Grid::ExtraSmall::col_xs_6);
            bilgiContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            bilgiContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::OldLace));
            auto layout = bilgiContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
            layout->addWidget (cpp14::make_unique<WText>("<b>Bilgi</b>"),0,AlignmentFlag::Right);
            for( auto item : this->BilgiBirimList () )
            {
                layout->addWidget (cpp14::make_unique<WText>(item.toStdString ()+" 🔍"),0,AlignmentFlag::Right);
            }
        }

        this->Content ()->addWidget (cpp14::make_unique<WBreak>());
    }


    {
        mAciklamaContainer = this->Content ()->addWidget (cpp14::make_unique<ContainerWidget>());
        mAciklamaContainer->setWidth (WLength("100%"));
        mAciklamaContainer->setMargin (20,Side::Top|Side::Bottom);
        mAciklamaContainer->setAttributeValue (Style::style,Style::Border::top::border ("1px solid gray"));

        auto _dilekceOid = this->oid ().value ().to_string ();
        auto list = this->findAciklama (_dilekceOid);
        for( auto item : list )
        {
            this->addAciklama (item);
        }
    }




    if( !mPublicLink && !cevaplandi ){
        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->addWidget (cpp14::make_unique<WText>("<b>Açıklama Ekle</b>"));
        auto text = container->addWidget (cpp14::make_unique<WTextEdit>());
        text->setHeight (350);
        container->setMargin (20,Side::Top|Side::Bottom);
        auto saveBtn = container->addWidget (cpp14::make_unique<WPushButton>("Açıklamayı Kaydet"));
        saveBtn->addStyleClass (Bootstrap::Button::Primary);
        saveBtn->clicked ().connect ([=](){

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
    }else{



    }




}

void DilekceView::initCevapView()
{
    this->Footer ()->clear ();
    this->Footer ()->setMargin (20,Side::Top);


    auto cevaplandi = this->Durum ();
    if( cevaplandi.toStdString () == DilekceDurum::Cevaplandi )
    {
        this->Footer ()->setWidth (WLength("100%"));
        this->Footer ()->addStyleClass (Bootstrap::ImageShape::img_thumbnail);


        auto cevapOid = this->cevapOid ();
        auto cevapView = this->LoadDilekceCevap (cevapOid.toStdString ());
        if( cevapView )
        {


            this->Footer ()->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::SeaShell));
            this->Footer ()->addWidget (cpp14::make_unique<WText>("<h4><b>Bu Dilekçe Cevaplandı</b></h4>"))->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::DimGray));
            this->Footer ()->addWidget (cpp14::make_unique<WBreak>());


            {
                auto cevapFile = this->downloadFileWeb (cevapView.value ()->cevapOid ());

                Wt::WLink link = Wt::WLink(LinkType::Url,cevapFile);
                link.setTarget(Wt::LinkTarget::NewWindow);

                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                            "<h4>Cevap Dosyası</h4>");
                auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                auto text = container->addWidget (std::move(anchor));
                text->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
                container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                container->setMargin (20,Side::Top|Side::Bottom);
                container->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
                container->decorationStyle ().setCursor (Cursor::PointingHand);
            }

            this->Footer ()->addWidget (cpp14::make_unique<WBreak>());


            {
                auto cevapEkList = cevapView.value ()->ekList ();

                int count = 1;
                for( auto eklist : cevapEkList )
                {

                    auto cevapFile = this->downloadFileWeb (eklist);

                    Wt::WLink link = Wt::WLink(LinkType::Url,cevapFile);
                    link.setTarget(Wt::LinkTarget::NewWindow);

                    std::unique_ptr<Wt::WAnchor> anchor =
                            Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                                                QString(" <b>EK %1</b> ").arg (count++).toStdString ());
                    auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                    auto text = container->addWidget (std::move(anchor));
                    text->addStyleClass (Bootstrap::ContextualBackGround::bg_success);
                    container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                    container->setMargin (20,Side::Top|Side::Bottom|Side::Right);
                    container->addStyleClass (Bootstrap::ContextualBackGround::bg_success);
                    container->decorationStyle ().setCursor (Cursor::PointingHand);

                }


            }


            if( mPublicLink )
            {
                auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::SlateBlue)
                                              +Style::color::color (Style::color::White::Snow)
                                              +Style::font::size::s12px);
                container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                container->setWidth (WLength("100%"));
                {
                    auto text = container->addWidget (cpp14::make_unique<WText>("<b>Dilekçenize Ait Cevabın Aslını Belediyemizden TC Numaranız ile Birlikte Alabilirsiniz</b>"));
                    container->addWidget (cpp14::make_unique<WBreak>());
                }
            }



        }else{
            this->Footer ()->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::DarkRed)
                                                +Style::color::color (Style::color::White::Snow)
                                                +Style::font::size::s10px
                                                +Style::font::weight::bold);

            this->Footer ()->addWidget (cpp14::make_unique<WText>("Dilekçe Cevabı Bulunamadı"));
            this->Footer ()->addWidget (cpp14::make_unique<WBreak>());



        }







        return;
    }else{

        if( mPublicLink )
        {
            auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Purple::Indigo)
                                          +Style::color::color (Style::color::White::Snow)
                                          +Style::font::size::s12px);
            container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            container->setWidth (WLength("100%"));
            {
                auto text = container->addWidget (cpp14::make_unique<WText>("<b>Bu Dilekçe Henüz Cevaplanmamış</b>"));
                container->addWidget (cpp14::make_unique<WBreak>());
            }
        }
    }



    if( !mPublicLink )
    {

        if( !mBilgi )
        {
            mCevapContainer = this->Footer ()->addWidget (cpp14::make_unique<ContainerWidget>());
            mCevapContainer->setMargin (20,Side::Top);
            mCevapContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            mCevapContainer->setWidth (WLength("100%"));
            mCevapContainer->addWidget (cpp14::make_unique<WText>("<strong>Cevap Ekle</strong>"));
            mCevapContainer->addWidget (cpp14::make_unique<WBreak>());

            mCevapFileContainer = mCevapContainer->addWidget (cpp14::make_unique<ContainerWidget>());

            mCevapContainer->addWidget (cpp14::make_unique<WBreak>());

            mCevapUploader = mCevapContainer->addWidget (cpp14::make_unique<FileUploaderWidget>(this->db ()));
            mCevapContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::LavenderBlush));
            mCevapUploader->Uploaded ().connect ([=](){

                mCevapFileContainer->clear ();
                auto container = mCevapFileContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                container->setMargin (5,AllSides);
                container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
                container->decorationStyle ().setCursor (Cursor::PointingHand);


                Wt::WLink link = Wt::WLink(LinkType::Url,mCevapUploader->doocRootLocation ().toStdString ());
                link.setTarget(Wt::LinkTarget::NewWindow);

                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                        "Cevap Dosyası");
                auto text = container->addWidget (std::move(anchor));
                text->addStyleClass (Bootstrap::ContextualBackGround::bg_info);



                std::cout << "docRootLocation: " << mCevapUploader->doocRootLocation ().toStdString () << std::endl;
                std::cout << "fileLocation: " << mCevapUploader->fileLocation ().toStdString () << std::endl;



            });

            mCevapEklerContainer = this->Footer ()->addWidget (cpp14::make_unique<ContainerWidget>());
            mCevapEklerContainer->setMargin (20,Side::Top);
            mCevapEklerContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            mCevapEklerContainer->setWidth (WLength("100%"));
            mCevapEklerContainer->addWidget (cpp14::make_unique<WText>("<strong>Ekler</strong>"));
            mCevapEklerContainer->addWidget (cpp14::make_unique<WBreak>());
            mCevapEkUploader = mCevapEklerContainer->addWidget (cpp14::make_unique<FileUploaderWidget>(this->db (),"Ek Dosya Yükle"));
            mCevapEklerContainer->setAttributeValue (Style::style,Style::background::color::color (Style::color::White::HoneyDew));
            mCevapEkUploader->Uploaded ().connect ([=](){


                auto container = mCevapEklerContainer->addWidget (cpp14::make_unique<ContainerWidget>());
                container->setWidth (175);
                container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
                container->setMargin (5,AllSides);
                container->addStyleClass (Bootstrap::ContextualBackGround::bg_info);
                container->setPadding (20,Side::Right);
                container->setPositionScheme (PositionScheme::Relative);



                mUploadedFilePathList.append(mCevapEkUploader->fileLocation ());
                Wt::WLink link = Wt::WLink(LinkType::Url,mCevapEkUploader->doocRootLocation ().toStdString ());
                link.setTarget(Wt::LinkTarget::NewWindow);

                std::unique_ptr<Wt::WAnchor> anchor =
                        Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                        "EK Dosyası "+ QString::number (mUploadedFilePathList.count ()).toStdString () );
                auto textLink = container->addWidget (std::move(anchor));
                textLink->addStyleClass (Bootstrap::ContextualBackGround::bg_info);

                auto delBtn = container->addWidget (cpp14::make_unique<ContainerWidget>());
                delBtn->setPositionScheme (PositionScheme::Absolute);
                delBtn->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
                delBtn->setOffsets (0,Side::Right|Side::Top);
                auto text = delBtn->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
                text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
                delBtn->clicked ().connect ([=](){
                    mCevapEklerContainer->removeWidget(container);
                });

            });

            {
                auto container = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
                container->setWidth (WLength("100%"));
                container->setContentAlignment (AlignmentFlag::Center);

                auto btn = container->addWidget (cpp14::make_unique<WPushButton>("Dilekçeyi Kapat"));
                btn->addStyleClass (Bootstrap::Button::Primary);
                btn->clicked ().connect ([=](){

                    if( !mCevapUploader->isUploaded () )
                    {
                        this->showMessage ("Uyarı","Cevap Eklemeden Dilekçeyi Kapatamazsınız");
                        return;
                    }


                    DilekceCevap mCevap;

                    mCevap.setSaat (QTime::currentTime ().toString ("hh:mm"));
                    mCevap.setJulianDay (QDate::currentDate ().toJulianDay ());
                    mCevap.setPersonelOid (this->mUser->oid ().value ().to_string ().c_str ());
                    mCevap.setPersonelName (this->mUser->AdSoyad ().c_str ());
                    mCevap.setDilekceOid (this->oid ().value ().to_string ().c_str ());
                    auto cevapOid_ = this->uploadfile (mCevapUploader->fileLocation ());

                    mCevap.setCevapOid (cevapOid_.get_oid ().value.to_string ().c_str ());

                    for( auto item : mUploadedFilePathList )
                    {
                        auto cevapOidEk_ = this->uploadfile (item);
                        mCevap.addEkOid (cevapOidEk_.get_oid ().value.to_string ().c_str ());
                    }
                    auto insCevap = this->insertCevap (&mCevap);
                    if( insCevap ){
                        this->SetDurum (DilekceDurum::Cevaplandi.c_str ());
                        this->SetCevapOid (insCevap.value ().to_string ().c_str ());
                        if( this->updateDilekce (this) ){
                            this->showMessage ("Bilgi","Dilekçe Başarılı Bir Şekilde Kapatıldı.");
                            _ClickKapatildi.emit (NoClass());
                        }else{
                            this->showMessage ("Uyarı","Dilekçe Kapatılamadı");
                        }
                    }
                });
            }
        }
    }
}

Signal<NoClass> &DilekceView::ClickKapatildi()
{
    return _ClickKapatildi;
}

void DilekceView::addAciklama(const DilekceAciklama &aciklama)
{
    auto container = mAciklamaContainer->addWidget (cpp14::make_unique<WContainerWidget>());
    container->setPositionScheme (PositionScheme::Relative);


    if( !mPublicLink ){

        auto oid_ = aciklama.oid ();

        if( oid_ && ( this->Durum ().toStdString () != DilekceDurum::Cevaplandi ) )
        {
            auto closeAciklama = container->addWidget (cpp14::make_unique<WContainerWidget>());
            closeAciklama->setPositionScheme (PositionScheme::Absolute);
            closeAciklama->setOffsets (0,Side::Right|Side::Top);
            auto text = closeAciklama->addWidget (cpp14::make_unique<WText>("<b>X</b>"));
            text->setAttributeValue (Style::style,Style::font::size::s14px);
            closeAciklama->addStyleClass (Bootstrap::Label::Danger);
            closeAciklama->decorationStyle ().setCursor (Cursor::PointingHand);

            closeAciklama->setAttributeValue (Style::dataoid,aciklama.oid ().value ().to_string ());
            closeAciklama->clicked ().connect ([=](){
                auto messageBox =
                      closeAciklama->addChild(Wt::cpp14::make_unique<Wt::WMessageBox>(
                                  "Uyarı",
                              "<p>Bu Açıklamayı Silmek İstediğinize Eminmisiniz<b>?</b></p>",
                                  Wt::Icon::Information,
                                  Wt::StandardButton::Yes | Wt::StandardButton::No));

                    messageBox->setModal(false);

                    messageBox->buttonClicked().connect([=] {
                        if( messageBox->buttonResult () == Wt::StandardButton::Yes )
                        {
                            if( this->deleteAciklama (closeAciklama->attributeValue (Style::dataoid).toUTF8 ()) ){

                                closeAciklama->removeChild(messageBox);

                                mAciklamaContainer->removeWidget(container);

                            }
                        }else{
                            closeAciklama->removeChild(messageBox);
                        }
                    });

                    messageBox->show();
            });
        }

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

void DilekceView::gorevliEkle()
{

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
            _container->addStyleClass (Bootstrap::ContextualBackGround::bg_warning);
        }


        auto gBtn = _container->addWidget (cpp14::make_unique<WText>("Görevlendir"));
        gBtn->setMargin (10,Side::Left|Side::Right);
        gBtn->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
        gBtn->decorationStyle ().setCursor (Cursor::PointingHand);
        gBtn->setPadding (10,Side::Left|Side::Right);
        _container->addWidget (cpp14::make_unique<WText>(personel.AdSoyad ().toStdString ()));
        container->addWidget (cpp14::make_unique<WBreak>());

        gBtn->clicked ().connect ([=](){


            if( _container->attributeValue (Style::dataoid).toUTF8 () != "null" )
            {
                IK::Personel per;
                per.setOid (personel.oid ().value ().to_string ());
                per.setAdSoyad (personel.AdSoyad ());
                per.setFotoOid (personel.FotoOid ());
                this->AddGorevliPersonel (per);
                if( this->updateDilekce (this) ){
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

void DilekceView::updateGorevliPersonelWidget()
{

    mGorevliPersonelContainer->clear ();

    mGorevliPersonelContainer->addWidget (cpp14::make_unique<WText>("<b>Görevli Personeller</b>"))->setMargin (10,Side::Right);
    auto ekleBtn = mGorevliPersonelContainer->addWidget (cpp14::make_unique<WPushButton>("Ekle"));
    ekleBtn->addStyleClass (Bootstrap::Button::Primary);
    mGorevliPersonelContainer->addWidget (cpp14::make_unique<WBreak>());
    ekleBtn->clicked ().connect (this,&DilekceView::gorevliEkle );

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
                        IK::Personel per;
                        per.setOid (deleteContainer->attributeValue (Style::dataoid).toUTF8 ());
                        this->DeleteGorevliPersonel (per);
                        if( this->updateDilekce (this) ){
                            this->updateGorevliPersonelWidget ();
                        }



                    }else{
                        deleteContainer->removeChild (messageBox);
                    }
                });
                messageBox->show();
        });
    }
}
