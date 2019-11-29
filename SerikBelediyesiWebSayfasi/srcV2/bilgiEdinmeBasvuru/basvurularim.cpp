#include "basvurularim.h"
//#include "bilgiEdinme/bilgiedinmeitem.h"



V2::Basvurularim::Basvurularim(DB *_db, const SerikBLDCore::TC &_tcUser)
    :ContainerWidget ("Başvurularım"),
      SerikBLDCore::BilgiEdinmeManager (_db),
      mCurrentFilter(std::make_unique<SerikBLDCore::BilgiEdinmeItem>())
{
    mCurrentFilter->clear ();
    mCurrentFilter->setTCoid (_tcUser.oid ().value ().to_string ().c_str ());
    this->setLimit (3);
    this->UpdateList (*mCurrentFilter);
}

V2::Basvurularim::~Basvurularim()
{
}

void V2::Basvurularim::initControlWidget()
{

    this->Footer ()->clear ();
    this->Footer ()->setMargin (15,Side::Top);
    this->Footer ()->addStyleClass ("boxShadow");


    auto backContainer_large = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    backContainer_large->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_6+
                                        Bootstrap::Grid::ExtraSmall::col_xs_6+
                                        Bootstrap::ContextualBackGround::bg_primary);
    backContainer_large->setHeight (35);
    backContainer_large->addWidget (cpp14::make_unique<WText>("Geri"));
    backContainer_large->clicked ().connect ([&](){
        this->back (*mCurrentFilter);
    });


    auto container = cpp14::make_unique<WContainerWidget>();
    container->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);
    container->addWidget (cpp14::make_unique<WText>("Test Object"));


    auto countContainer_large = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    countContainer_large->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                         Bootstrap::Grid::Medium::col_md_4+
                                         Bootstrap::Grid::Hidden::hidden_sm+
                                         Bootstrap::Grid::Hidden::hidden_xs+
                                         Bootstrap::ContextualBackGround::bg_info);
    countContainer_large->setHeight (35);
    countContainer_large->addWidget (cpp14::make_unique<WText>(WString("Sayfa({1}-{2}/{3})").arg (this->skip ()).arg ((this->limit ()+this->skip () > this->totalCount () ? this->totalCount () : (this->skip () + this->limit () ))).arg (this->totalCount ())));



    auto nextContainer_large = this->Footer ()->addWidget (cpp14::make_unique<WContainerWidget>());
    nextContainer_large->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_6+
                                        Bootstrap::Grid::ExtraSmall::col_xs_6+
                                        Bootstrap::ContextualBackGround::bg_primary);
    nextContainer_large->setHeight (35);
    nextContainer_large->addWidget (cpp14::make_unique<WText>("İleri"));
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
    countContainer_small->addWidget (cpp14::make_unique<WText>(WString("Sayfa({1}-{2}/{3})").arg (this->skip ()).arg ((this->limit ()+this->skip () > this->totalCount () ? this->totalCount () : (this->skip () + this->limit () ))).arg (this->totalCount ())));

}



void V2::Basvurularim::onList( const QVector<SerikBLDCore::BilgiEdinmeItem> *mlist)
{
    this->Content ()->clear ();
    for( auto __item : *mlist )
    {
        auto item = this->Content ()->addWidget (cpp14::make_unique<BasvruListItemWidget>(&__item));
        item->addStyleClass (Bootstrap::Grid::Large::col_lg_3);
    }
    this->initControlWidget ();
}

