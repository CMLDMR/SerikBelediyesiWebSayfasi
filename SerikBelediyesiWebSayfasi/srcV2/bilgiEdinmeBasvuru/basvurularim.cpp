#include "basvurularim.h"
//#include "bilgiEdinme/bilgiedinmeitem.h"
#include "basvuruitemviewwidget.h"



V2::Basvurularim::Basvurularim(DB *_db, const SerikBLDCore::TC &_tcUser)
    :ContainerWidget (WString::tr ("Başvurularım").toUTF8 ()),
      SerikBLDCore::BilgiEdinmeManager (_db),
      mCurrentFilter(std::make_unique<SerikBLDCore::BilgiEdinmeItem>())
{
    mCurrentFilter->clear ();
    mCurrentFilter->setTCoid (_tcUser.oid ().value ().to_string ().c_str ());

    this->UpdateList (*mCurrentFilter);
}

V2::Basvurularim::~Basvurularim()
{
}

void V2::Basvurularim::initControlWidget()
{

    this->Footer ()->clear ();
    this->Footer ()->setMargin (15,Side::Top);
    this->Footer ()->addStyleClass ("boxShadow boxRadius");


    auto backContainer_large = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    backContainer_large->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_6+
                                        Bootstrap::Grid::ExtraSmall::col_xs_6+
                                        Bootstrap::ContextualBackGround::bg_primary);
    backContainer_large->setHeight (35);
    backContainer_large->decorationStyle ().setCursor (Cursor::PointingHand);
    auto vLayout = backContainer_large->setLayout (cpp14::make_unique<WVBoxLayout>());
    vLayout->addWidget (cpp14::make_unique<WText>("<< Geri"),0,AlignmentFlag::Middle|AlignmentFlag::Center);

    backContainer_large->clicked ().connect ([&](){
        this->back (*mCurrentFilter);
    });


    auto countContainer_large = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    countContainer_large->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                         Bootstrap::Grid::Medium::col_md_4+
                                         Bootstrap::Grid::Hidden::hidden_sm+
                                         Bootstrap::Grid::Hidden::hidden_xs+
                                         Bootstrap::ContextualBackGround::bg_info);
    countContainer_large->setHeight (35);

    auto v2Layout = countContainer_large->setLayout (cpp14::make_unique<WVBoxLayout>());
    v2Layout->addWidget (cpp14::make_unique<WText>(WString("Sayfa({1}-{2}/{3})")
                                                   .arg (this->skip ())
                                                   .arg ((this->limit ()+this->skip () > this->totalCount () ? this->totalCount () : (this->skip () + this->limit () )))
                                                   .arg (this->totalCount ())),0,AlignmentFlag::Middle|AlignmentFlag::Center);



    auto nextContainer_large = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    nextContainer_large->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_6+
                                        Bootstrap::Grid::ExtraSmall::col_xs_6+
                                        Bootstrap::ContextualBackGround::bg_primary);
    nextContainer_large->setHeight (35);
    nextContainer_large->decorationStyle ().setCursor (Cursor::PointingHand);
    auto v3Layout = nextContainer_large->setLayout (cpp14::make_unique<WVBoxLayout>());
    v3Layout->addWidget (cpp14::make_unique<WText>("İleri >>"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
    nextContainer_large->clicked ().connect ([&](){
        this->next (*mCurrentFilter);
    });

    auto countContainer_small = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    countContainer_small->addStyleClass (Bootstrap::Grid::Hidden::hidden_lg+
                                         Bootstrap::Grid::Hidden::hidden_md+
                                         Bootstrap::Grid::Small::col_sm_12+
                                         Bootstrap::Grid::ExtraSmall::col_xs_12+
                                         Bootstrap::ContextualBackGround::bg_info);
    countContainer_small->setHeight (35);
    countContainer_small->addWidget (cpp14::make_unique<WText>(WString("Sayfa( {1} - {2} / {3} )").arg (this->skip ()).arg ((this->limit ()+this->skip () > this->totalCount () ? this->totalCount () : (this->skip () + this->limit () ))).arg (this->totalCount ())));

}

void V2::Basvurularim::onList( const QVector<SerikBLDCore::BilgiEdinmeItem> *mlist)
{
    this->Content ()->clear ();
    for( auto __item : *mlist )
    {
        auto item = this->Content ()->addWidget (cpp14::make_unique<BasvruListItemWidget>(&__item));
        item->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                             Bootstrap::Grid::Medium::col_md_3+
                             Bootstrap::Grid::Small::col_sm_4+
                             Bootstrap::Grid::ExtraSmall::col_xs_6);

        item->ClickItem ().connect (this,&V2::Basvurularim::initItem);
    }
    this->initControlWidget ();
}

void V2::Basvurularim::initItem(const std::string &itemOid)
{

    this->Content ()->clear ();

    auto item = this->itemAt (itemOid.c_str ());

    auto itemView = this->Content ()->addWidget (cpp14::make_unique<BasvuruItemViewWidget>(std::move(item)));

    itemView->ClickedCevapFile ().connect ([&](const std::string cevapOid ){
        auto filePath = this->downloadFileWeb (cevapOid.c_str ());
        Wt::WLink link = Wt::WLink(LinkType::Url,filePath);
        link.setTarget(Wt::LinkTarget::NewWindow);
        wApp->doJavaScript ("window.open(\""+link.url ()+"\",\"_blank\");");
    });

    itemView->Closed ().connect ([&](){
        this->UpdateList (*mCurrentFilter);
    });

}









V2::GelenBasvurular::GelenBasvurular(SerikBLDCore::DB *_db, bsoncxx::document::value _user )
    :ContainerWidget ("Gelen Başvurular"),
      SerikBLDCore::BilgiEdinmeManager (_db),
      mUser( new SerikBLDCore::User(_db->db () , _user) ),
      mCurrentFilter(std::make_unique<SerikBLDCore::BilgiEdinmeItem>())
{
    mCurrentFilter->setBirim (mUser->Birimi ().c_str ());
    this->UpdateList (*mCurrentFilter);
}

V2::GelenBasvurular::~GelenBasvurular()
{

}

void V2::GelenBasvurular::initControlWidget()
{
    this->Footer ()->clear ();
    this->Footer ()->setMargin (15,Side::Top);
    this->Footer ()->addStyleClass ("boxShadow boxRadius");


    auto backContainer_large = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    backContainer_large->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_6+
                                        Bootstrap::Grid::ExtraSmall::col_xs_6+
                                        Bootstrap::ContextualBackGround::bg_primary);
    backContainer_large->setHeight (35);
    backContainer_large->decorationStyle ().setCursor (Cursor::PointingHand);
    auto vLayout = backContainer_large->setLayout (cpp14::make_unique<WVBoxLayout>());
    vLayout->addWidget (cpp14::make_unique<WText>("<< Geri"),0,AlignmentFlag::Middle|AlignmentFlag::Center);

    backContainer_large->clicked ().connect ([&](){
        this->back (*mCurrentFilter);
    });


    auto countContainer_large = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    countContainer_large->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                         Bootstrap::Grid::Medium::col_md_4+
                                         Bootstrap::Grid::Hidden::hidden_sm+
                                         Bootstrap::Grid::Hidden::hidden_xs+
                                         Bootstrap::ContextualBackGround::bg_info);
    countContainer_large->setHeight (35);

    auto v2Layout = countContainer_large->setLayout (cpp14::make_unique<WVBoxLayout>());
    v2Layout->addWidget (cpp14::make_unique<WText>(WString("Sayfa({1}-{2}/{3})")
                                                   .arg (this->skip ())
                                                   .arg ((this->limit ()+this->skip () > this->totalCount () ? this->totalCount () : (this->skip () + this->limit () )))
                                                   .arg (this->totalCount ())),0,AlignmentFlag::Middle|AlignmentFlag::Center);



    auto nextContainer_large = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    nextContainer_large->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_6+
                                        Bootstrap::Grid::ExtraSmall::col_xs_6+
                                        Bootstrap::ContextualBackGround::bg_primary);
    nextContainer_large->setHeight (35);
    nextContainer_large->decorationStyle ().setCursor (Cursor::PointingHand);
    auto v3Layout = nextContainer_large->setLayout (cpp14::make_unique<WVBoxLayout>());
    v3Layout->addWidget (cpp14::make_unique<WText>("İleri >>"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
    nextContainer_large->clicked ().connect ([&](){
        this->next (*mCurrentFilter);
    });

    auto countContainer_small = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    countContainer_small->addStyleClass (Bootstrap::Grid::Hidden::hidden_lg+
                                         Bootstrap::Grid::Hidden::hidden_md+
                                         Bootstrap::Grid::Small::col_sm_12+
                                         Bootstrap::Grid::ExtraSmall::col_xs_12+
                                         Bootstrap::ContextualBackGround::bg_info);
    countContainer_small->setHeight (35);
    countContainer_small->addWidget (cpp14::make_unique<WText>(WString("Sayfa( {1} - {2} / {3} )").arg (this->skip ()).arg ((this->limit ()+this->skip () > this->totalCount () ? this->totalCount () : (this->skip () + this->limit () ))).arg (this->totalCount ())));

}

void V2::GelenBasvurular::onList(const QVector<SerikBLDCore::BilgiEdinmeItem> *mlist)
{
    this->Content ()->clear ();
    for( auto __item : *mlist )
    {
        auto item = this->Content ()->addWidget (cpp14::make_unique<BasvruListItemWidget>(&__item));
        item->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                             Bootstrap::Grid::Medium::col_md_3+
                             Bootstrap::Grid::Small::col_sm_4+
                             Bootstrap::Grid::ExtraSmall::col_xs_6);

        item->ClickItem ().connect (this,&V2::GelenBasvurular::initItem);

    }
    this->initControlWidget ();
}

void V2::GelenBasvurular::initItem(const std::string &itemOid)
{
    this->Content ()->clear ();

    auto item = this->itemAt (itemOid.c_str ());

    auto itemView = this->Content ()->addWidget (cpp14::make_unique<BasvuruItemViewWidget>(item));



    if( item.cevaplandi () )
    {
        this->Content ()->addWidget (cpp14::make_unique<WBreak>());

        itemView->ClickedCevapFile ().connect ([&](const std::string cevapOid ){
            auto filePath = this->downloadFileWeb (cevapOid.c_str ());
            Wt::WLink link = Wt::WLink(LinkType::Url,filePath);
            link.setTarget(Wt::LinkTarget::NewWindow);
            wApp->doJavaScript ("window.open(\""+link.url ()+"\",\"_blank\");");
        });

        itemView->Closed ().connect ([&](){
            this->UpdateList (*mCurrentFilter);
        });

    }else{
        this->Content ()->addWidget (cpp14::make_unique<WBreak>());
        auto uploadWidget = this->Content ()->addWidget (cpp14::make_unique<FileUploaderWidget>());
        uploadWidget->setType (FileUploaderWidget::Pdf);
        uploadWidget->Uploaded ().connect ([=](){
            auto fileid = this->uploadfile ( uploadWidget->fileLocation () );
            decltype (item) _item = item;
            _item.setCevap (fileid.view().get_oid ().value.to_string ().c_str ());
            if( this->UpdateItem (_item) )
            {
                this->initItem (_item.oid ().value ().to_string () );
            }
        });
    }

    itemView->Closed ().connect ([&](){
        this->UpdateList (*mCurrentFilter);
    });


}
