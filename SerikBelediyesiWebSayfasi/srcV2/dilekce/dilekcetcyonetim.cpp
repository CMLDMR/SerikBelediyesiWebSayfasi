#include "dilekcetcyonetim.h"

DilekceTCYonetim::DilekceTCYonetim(mongocxx::database *_db, bsoncxx::document::value *_user)
    : ContainerWidget ("Dilekçelerim") , DilekceManager (_db) , mTCUser(_user)
{

    auto container = this->Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
    container->addStyleClass (Bootstrap::ContextualBackGround::bg_warning+Bootstrap::ImageShape::img_thumbnail);
    container->addWidget (cpp14::make_unique<WText>("<h5>Dilekçeleri Listele</h5>"));
    container->decorationStyle ().setCursor (Cursor::PointingHand);
    container->setMargin (15,Side::Bottom|Side::Top);

    list ();

    container->clicked ().connect ([=](){
        list ();
    });

    _clickDilekceItem.connect (this,&DilekceTCYonetim::initBilgiDilekce );

}

void DilekceTCYonetim::list()
{
    this->Content ()->clear ();
    SerikBLDCore::Dilekce filter;
    filter.SetTCOid (mTCUser.oid ().value ().to_string ().c_str ());

    auto cursor = this->findDilekce (filter);

    for( auto item : cursor )
    {

        auto dItem = this->Content ()->addWidget (cpp14::make_unique<DilekceListWidgetItem>(item));
        dItem->addStyleClass (Bootstrap::Grid::col_full_12);
        dItem->ClickDilekceWidget ().connect ([=](const std::string& oid){
            _clickDilekceItem.emit (oid);
        });
    }
}

void DilekceTCYonetim::listBilgiDilekce(const SerikBLDCore::Dilekce &filterDilekce)
{


    this->Content ()->clear ();


    /// Header SATIR
    {
        auto container = this->Content ()->addWidget (cpp14::make_unique<ContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);

        auto rContainer = container->addWidget (cpp14::make_unique<ContainerWidget>());
        rContainer->addStyleClass (Bootstrap::Grid::row);
        rContainer->setWidth (WLength("100%"));
        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>#</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_1
                                       +Bootstrap::Grid::Medium::col_md_1
                                       +Bootstrap::Grid::Small::col_sm_1
                                       +Bootstrap::Grid::ExtraSmall::col_xs_1);
        }

        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>Konu</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_5
                                       +Bootstrap::Grid::Medium::col_md_5
                                       +Bootstrap::Grid::Small::col_sm_5
                                       +Bootstrap::Grid::ExtraSmall::col_xs_5);
        }

        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>Tarih</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                       +Bootstrap::Grid::Medium::col_md_3
                                       +Bootstrap::Grid::Small::col_sm_3
                                       +Bootstrap::Grid::ExtraSmall::col_xs_3);
        }

        {
            auto _container = rContainer->addWidget (cpp14::make_unique<ContainerWidget>());
            _container->addWidget (cpp14::make_unique<WText>("<b>Sayı</b>"));
            _container->addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                       +Bootstrap::Grid::Medium::col_md_3
                                       +Bootstrap::Grid::Small::col_sm_3
                                       +Bootstrap::Grid::ExtraSmall::col_xs_3);
        }
    }

    auto cursor = this->findDilekce (filterDilekce);

    for( auto item : cursor )
    {

        auto dItem = this->Content ()->addWidget (cpp14::make_unique<DilekceListWidgetItem>(item));
        dItem->addStyleClass (Bootstrap::Grid::col_full_12);
        dItem->ClickDilekceWidget ().connect ([=](const std::string& oid){
            _clickDilekceItem.emit (oid);
        });

    }
}

void DilekceTCYonetim::initBilgiDilekce(const std::string &dilekceOid)
{
    auto dilekce = this->LoadDilekce (dilekceOid);
    if( dilekce )
    {
        this->Content ()->clear ();
        auto dilekceView = this->Content ()->addWidget (cpp14::make_unique<DilekceView>(dilekce.value()
                                                                                        ,this->db ()
                                                                                        ,nullptr
                                                                                        ,true
                                                                                        ,false));

        dilekceView->removeTitleBar ();
    }else{
        this->showMessage ("Hata","Bu Dilekçe Yüklenemedi");
    }
}
