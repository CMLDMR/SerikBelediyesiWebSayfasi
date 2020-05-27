#include "mimarprojeview.h"



Comman::MimarProjeView::MimarProjeView(const MimariProje &projeItem, const bool &firmaGirisi)
    :SerikBLDCore::Imar::MimariProje::MimariProje (projeItem),
      mFirmaGirisi(firmaGirisi)
{
    setMargin (15,Side::Top|Side::Bottom);


    Content ()->addStyleClass ("boxShadow");
    Content ()->setMargin (20,Side::Top|Side::Bottom);


}




void Comman::MimarProjeView::initMimariProject()
{
    Content ()->clear ();
    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12+
                                  Bootstrap::ImageShape::img_thumbnail);
        container->addWidget (cpp14::make_unique<WText>("<b>Mimari Proje Açıklamaları-Düzeltmeleri</b>"))->setAttributeValue (Style::style,Style::font::family::dosis);
    }

    // Mimari Proje
    {
        auto mimari_DwgContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        mimari_DwgContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        mimari_DwgContainer->setContentAlignment (AlignmentFlag::Center);
        auto mimariDwgContainer = mimari_DwgContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        mimariDwgContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        mimariDwgContainer->setWidth (100);


        mimariDwgContainer->clicked ().connect ([=](){
            _requestMimariProje.emit (bsoncxx::oid{this->dwgOid ()});
        });
        mimariDwgContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        //        mimariDwgContainer->addStyleClass ("boxShadow");
        auto mimariDwgLayout = mimariDwgContainer->setLayout (cpp14::make_unique<WVBoxLayout>());
        mimariDwgLayout->addWidget (cpp14::make_unique<WImage>(WLink("icon/dwgicon.png")),0,AlignmentFlag::Center);
        mimariDwgLayout->addWidget (cpp14::make_unique<WText>("<i>Mimari Proje Dosyası İNDİR</i>"),0,AlignmentFlag::Center)->setAttributeValue (Style::style,Style::font::family::dosis);
    }


    // Project Aktif Edildi İse Yeni Mimari Proje Yüklenemez
    if( !this->projeAktif () && mFirmaGirisi ){
        auto fileUploaderContainer = Content ()->addWidget (cpp14::make_unique<FileUploaderWidget>("Yeni Mimari Proje (DWG) Yükle"));
        fileUploaderContainer->setType (FileUploaderWidget::DWG);
        fileUploaderContainer->Uploaded ().connect ([=](){
            std::cout << "Uploaded File: " << fileUploaderContainer->fileLocation ().toStdString () << std::endl;
        });
    }




    // Kontrol Buttonları
    {

        auto btnGroupContainer = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        btnGroupContainer->addStyleClass (Bootstrap::Grid::col_full_12);

        auto rContainer = btnGroupContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);



        // Firma Düzeltme Ekleyemez
        if( !mFirmaGirisi ){
            auto duzeltmeEkleBtnContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            duzeltmeEkleBtnContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                                     Bootstrap::Grid::Small::col_sm_3+
                                                     Bootstrap::Grid::Medium::col_md_2+
                                                     Bootstrap::Grid::ExtraSmall::col_xs_3);
            duzeltmeEkleBtnContainer->addWidget (cpp14::make_unique<WText>("<b>Düzeltme Ekle</b>"))->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);
            duzeltmeEkleBtnContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (0,25),this->getRandom (0,50),this->getRandom (0,75)));
            duzeltmeEkleBtnContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            duzeltmeEkleBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);
            duzeltmeEkleBtnContainer->clicked ().connect ([=](){

                auto mDialog = createDialog ("Düzeltme Ekle");

                auto rContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
                rContainer->addStyleClass (Bootstrap::Grid::row);
                auto duzeltmeLineEdit = rContainer->addWidget (cpp14::make_unique<WLineEdit>());
                duzeltmeLineEdit->setPlaceholderText ("Düzeltme Metnini Giriniz");

                auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
                saveBtn->clicked ().connect ([=](){
                    _insertDuzeltme.emit (this->oid ().value (),duzeltmeLineEdit->text ().toUTF8 ());
                    remogeDialog (mDialog);
                });

                mDialog->show ();
            });
        }



        auto aciklamaEkleBtnContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        aciklamaEkleBtnContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                                 Bootstrap::Grid::Small::col_sm_3+
                                                 Bootstrap::Grid::Medium::col_md_2+
                                                 Bootstrap::Grid::ExtraSmall::col_xs_3);
        aciklamaEkleBtnContainer->addWidget (cpp14::make_unique<WText>("<b>Açıklama Ekle</b>"))->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::family::dosis);
        aciklamaEkleBtnContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (0,25),this->getRandom (0,50),this->getRandom (0,75)));
        aciklamaEkleBtnContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        aciklamaEkleBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        aciklamaEkleBtnContainer->clicked ().connect ([=](){

            auto mDialog = createDialog ("Açıklama Ekle");

            auto rContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
            rContainer->addStyleClass (Bootstrap::Grid::row);
            auto duzeltmeLineEdit = rContainer->addWidget (cpp14::make_unique<WLineEdit>());
            duzeltmeLineEdit->setPlaceholderText ("Açıklama Metnini Giriniz");

            auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
            saveBtn->clicked ().connect ([=](){
                _insertAciklama.emit (this->oid ().value (),duzeltmeLineEdit->text ().toUTF8 ());
                remogeDialog (mDialog);
            });

            mDialog->show ();
        });




        auto dosyaEkleBtnContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        dosyaEkleBtnContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                                 Bootstrap::Grid::Small::col_sm_3+
                                                 Bootstrap::Grid::Medium::col_md_2+
                                                 Bootstrap::Grid::ExtraSmall::col_xs_3);
        dosyaEkleBtnContainer->addWidget (cpp14::make_unique<WText>("<b>Dosya Ekle</b>"))->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
        dosyaEkleBtnContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (0,25),this->getRandom (0,50),this->getRandom (0,75)));
        dosyaEkleBtnContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        dosyaEkleBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);
        dosyaEkleBtnContainer->clicked ().connect ([=](){

            auto mDialog = createDialog ("Dosya Ekle");

            auto rContainer = mDialog->contents ()->addWidget (cpp14::make_unique<WContainerWidget>());
            rContainer->addStyleClass (Bootstrap::Grid::row);


            auto fileUploader = rContainer->addWidget (cpp14::make_unique<FileUploaderWidget>("Dosya Ekle"));



            auto text = rContainer->addWidget (cpp14::make_unique<WText>("Resim,PDF veya Başka Bir Dosya Ekleyiniz"));
            text->setAttributeValue (Style::style,Style::font::family::dosis);




            auto saveBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kaydet"));
            saveBtn->clicked ().connect ([=](){

                if( fileUploader->isUploaded () ){
                    _insertDosya.emit (this->oid ().value (),fileUploader->fileLocation ().toStdString ());
                    remogeDialog (mDialog);
                }else{
                    text->setAttributeValue (Style::style,Style::color::color (Style::color::Red::Red));
                }

            });

            mDialog->show ();
        });


        if( !mFirmaGirisi ){
            auto onaylaBtnContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
            onaylaBtnContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                               Bootstrap::Grid::Small::col_sm_3+
                                               Bootstrap::Grid::Medium::col_md_2+
                                               Bootstrap::Grid::ExtraSmall::col_xs_3);

            onaylaBtnContainer->addStyleClass (Bootstrap::Grid::Offset::Large::col_lg_4+
                                               Bootstrap::Grid::Offset::Medium::col_md_4);

            auto onaylaText = onaylaBtnContainer->addWidget (cpp14::make_unique<WText>(this->mimariProjeOnay () ? "<b>Onayı Kaldır</b>" : "<b>Onayla</b>" ));
            onaylaText->setAttributeValue (Style::style,Style::color::color (Style::color::Grey::Black)+Style::font::family::dosis);
            if( this->mimariProjeOnay () ){
                onaylaBtnContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (200,250),this->getRandom (0,50),this->getRandom (50,125)));
            }else{
                onaylaBtnContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (0,25),this->getRandom (200,250),this->getRandom (50,125)));
            }
            onaylaBtnContainer->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
            onaylaBtnContainer->decorationStyle ().setCursor (Cursor::PointingHand);
            onaylaBtnContainer->clicked ().connect ([=](){
                if( this->mimariProjeOnay () ){
                    _mimariProjeOnayla.emit (this->oid ().value (),false);
                    onaylaText->setText ("<b>Onayla</b>");
                    onaylaBtnContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (0,25),this->getRandom (200,250),this->getRandom (50,125)));
                }else{
                    _mimariProjeOnayla.emit (this->oid ().value (),true);
                    onaylaText->setText ("<b>Onayı Kaldır</b>");
                    onaylaBtnContainer->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (200,250),this->getRandom (0,50),this->getRandom (50,125)));
                }
            });
        }



        auto logFilterContainer = rContainer->addWidget (cpp14::make_unique<WContainerWidget>());
        logFilterContainer->addStyleClass (Bootstrap::Grid::col_full_12);
        logFilterContainer->setMargin (20,Side::Top);
        mLogFilterComboBox = logFilterContainer->addWidget (cpp14::make_unique<WComboBox>());
        mLogFilterComboBox->addItem ("Hepsi");
        mLogFilterComboBox->addItem ("Düzeltmeler");
        mLogFilterComboBox->addItem ("Açıklamalar");
        mLogFilterComboBox->addItem ("Dosyalar");
        mLogFilterComboBox->addItem ("İşlemler");
        mLogFilterComboBox->sactivated ().connect ([=](){
            _requestLog.emit (this->oid ().value (),mLogFilterComboBox->currentIndex ());
        });

        _requestLog.emit (this->oid ().value (),mLogFilterComboBox->currentIndex ());


    }

}






Signal<bsoncxx::oid> &Comman::MimarProjeView::RequestMimariProje()
{
    return _requestMimariProje;
}



Signal<bsoncxx::oid,std::string> &Comman::MimarProjeView::insertDuzeltmeClicked()
{
    return _insertDuzeltme;
}

Signal<bsoncxx::oid, std::string> &Comman::MimarProjeView::insertAciklama()
{
    return _insertAciklama;
}

Signal<bsoncxx::oid, std::string> &Comman::MimarProjeView::insertDosya()
{
    return _insertDosya;
}

Signal<bsoncxx::oid, int> &Comman::MimarProjeView::RequestLog()
{
    return _requestLog;
}

Signal<bsoncxx::oid, bool> &Comman::MimarProjeView::MimariProjeOnayla()
{
    return _mimariProjeOnayla;
}

