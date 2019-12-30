#include "personelmanagerpage.h"



IKManagerPage::IKManagerPage(SerikBLDCore::DB *_db)
    :ContainerWidget ("İnsan Kaynakları Yönetimi"),mDB(_db)
{

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());
        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5+
                                  "boxShadow");
        container->setHeight (100);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::DeepPink));
        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Personel Yönetimi"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::weight::bold);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
        container->clicked ().connect (this,&IKManagerPage::initPersonelManager );
    }

    {
        auto container = Header ()->addWidget (cpp14::make_unique<WContainerWidget>());

        container->addStyleClass (Bootstrap::Grid::Offset::Large::col_lg_4+
                                  Bootstrap::Grid::Offset::Medium::col_md_4+
                                  Bootstrap::Grid::Offset::Small::col_sm_2+
                                  Bootstrap::Grid::Offset::ExtraSmall::col_xs_2+
                                  "boxShadow");

        container->addStyleClass (Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_5+
                                  Bootstrap::Grid::ExtraSmall::col_xs_5+
                                  "boxShadow");
        container->setHeight (100);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Pink::MediumVioletRed));
        auto vLayout = container->setLayout (cpp14::make_unique<WVBoxLayout>());
        auto text = vLayout->addWidget (cpp14::make_unique<WText>("Birim Yönetimi"),0,AlignmentFlag::Middle|AlignmentFlag::Center);
        text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::font::weight::bold);
        container->decorationStyle ().setCursor (Cursor::PointingHand);
    }

    Content ()->setMargin (15,Side::Top);
}

void IKManagerPage::initPersonelManager()
{
    Content ()->clear ();
    Content ()->addWidget (cpp14::make_unique<PersonelManagerPage>(mDB));
}

void IKManagerPage::initBirimManager()
{

}


PersonelManagerPage::PersonelManagerPage(DB *_db)
    :SerikBLDCore::PersonelManager (_db),
      ContainerWidget ("Personel Yönetimi")
{



    this->UpdateList (SerikBLDCore::IK::Personel());

}

void PersonelManagerPage::onList(const QVector<SerikBLDCore::IK::Personel> *mlist)
{



}
