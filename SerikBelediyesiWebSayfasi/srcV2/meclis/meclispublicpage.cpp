#include "meclispublicpage.h"



v2::MeclisPublicPage::MeclisPublicPage(SerikBLDCore::DB *db)
    :ContainerWidget("Meclis v2"),SerikBLDCore::Meclis::MeclisManager (db)
{
    this->setMargin (90,Side::Top);


    SerikBLDCore::FindOptions findOptions;

    findOptions.setSort (SerikBLDCore::Item("none").append("_id",1));


    this->UpdateList (SerikBLDCore::Meclis::MeclisItem(),findOptions);
}

void v2::MeclisPublicPage::onList(const QVector<SerikBLDCore::Meclis::MeclisItem> *mlist)
{

    Content ()->clear ();


    for( auto item : *mlist )
    {
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6+
                                  Bootstrap::ImageShape::img_thumbnail);

        container->addWidget (cpp14::make_unique<WText>(item.ay ().toStdString ()));
    }


}
