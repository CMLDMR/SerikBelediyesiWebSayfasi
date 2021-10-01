#include "baskanyardimcilariwidget.h"

v2::BaskanYardimcilariWidget::BaskanYardimcilariWidget(DB *_mDB)
    :SerikBLDCore::PersonelManager (_mDB)
{
    this->setMargin(100,Side::Top);
    Header ()->addWidget (cpp14::make_unique<WText>("Başkan Yardımcıları"));


    SerikBLDCore::IK::Personel filter;
    filter.setStatu (SerikBLDCore::IK::Statu::BaskanYardimcisi.c_str ());

    this->UpdateList (filter);
}

void v2::BaskanYardimcilariWidget::onList(const QVector<SerikBLDCore::IK::Personel> *mlist)
{

    this->Content ()->clear ();


    for( const auto &item : *mlist ){

        auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addWidget (cpp14::make_unique<WText>(item.AdSoyad ().toStdString ()));

    }

}

void v2::BaskanYardimcilariWidget::initWidget()
{

}
