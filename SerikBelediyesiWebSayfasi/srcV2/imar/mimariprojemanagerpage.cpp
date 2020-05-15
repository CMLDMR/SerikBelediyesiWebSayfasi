#include "mimariprojemanagerpage.h"



v2::MimariProjeManagerPage::MimariProjeManagerPage( SerikBLDCore::DB* _db )
    :SerikBLDCore::Imar::MimariProjeManager (_db),
      ContainerWidget ("Başvurulan Projeler")
{

    this->setLimit (10000);
}

void v2::MimariProjeManagerPage::onList(const QVector<SerikBLDCore::Imar::MimariProje::MimariProje> *mlist)
{
    for( auto item : *mlist ){
        auto container = Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::col_full_12);
        container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);
        container->setMargin (5,Side::Top|Side::Bottom);
        container->addWidget (cpp14::make_unique<WText>(item.adi ()));
    }

    this->showPopUpMessage ("Proje Listesi Güncellendi");
}

void v2::MimariProjeManagerPage::errorOccured(const std::string &errorText)
{
    this->showPopUpMessage ("Hata: " + errorText,"err");
}

void v2::MimariProjeManagerPage::setFirmaOid(const std::string &firmaOid)
{
    mFirmaOid = firmaOid;

    SerikBLDCore::Imar::MimariProje::MimariProje filter;
    filter.setFirmaOid (bsoncxx::oid{firmaOid});

    this->UpdateList (filter);
}

