#include "karavansorguwidget.h"

namespace v2 {
namespace Karavan {

KaravanSorguWidget::KaravanSorguWidget()
{
//    this->setMargin(5,Side::Top|Side::Bottom);
    this->setMargin(25,Side::Top);
this->setPadding(5,Side::Top|Side::Bottom);

    this->init();
}

void KaravanSorguWidget::init()
{

    this->Content()->clear();

    setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::LightGray));

    {
        auto _container = this->Content()->addWidget(std::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        mTCNOLineEdit = _container->addWidget(std::make_unique<WLineEdit>());
        mTCNOLineEdit->setPlaceholderText("TC NO Giriniz");
        mTCNOLineEdit->setInputMask("99999999999");

    }


    {
        auto _container = this->Content()->addWidget(std::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        mPlakaNoLineEdit = _container->addWidget(std::make_unique<WLineEdit>());
        mPlakaNoLineEdit->setPlaceholderText("Plaka No");
        mPlakaNoLineEdit->setInputMask("99->AAA-99999");
    }


    {
        auto _container = this->Content()->addWidget(std::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        mSureComboBox = _container->addWidget(std::make_unique<WComboBox>());
        mSureComboBox->addItem("Hepsi");
        mSureComboBox->addItem("Son Gün");
        mSureComboBox->addItem("Süresi Dolmuş");
        mSureComboBox->addItem("Devam Eden");
    }


    {
        auto _container = this->Content()->addWidget(std::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        mSorguButton = _container->addWidget(std::make_unique<WPushButton>("Sorgula"));
        mSorguButton->addStyleClass(Bootstrap::Button::Primary);

        mSorguButton->clicked().connect([=](){
            _SorguClicked.emit(NoClass());
        });
    }
}

KaravanItem KaravanSorguWidget::Filter()
{
    KaravanItem filter;

    if( mTCNOLineEdit->text().toUTF8().size() ){
        filter.setTCNO(mTCNOLineEdit->text().toUTF8());
    }

    if( mPlakaNoLineEdit->text().toUTF8().size() ){
        filter.setplakaNo(mPlakaNoLineEdit->text().toUTF8());
    }

    if( mSureComboBox->currentText().toUTF8() == "Hepsi" ){

    }else if( mSureComboBox->currentText().toUTF8() == "Son Gün" ){
        filter.setbitisDate(WDate::currentDate().toJulianDay());
    }else if( mSureComboBox->currentText().toUTF8() == "Süresi Dolmuş" ){
        SerikBLDCore::Item item("");
        item.append("$lt",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()});
        filter.append(Key::bitisDate,item);
    }else if( mSureComboBox->currentText().toUTF8() == "Devam Eden" ){
        SerikBLDCore::Item item("");
        item.append("$gte",bsoncxx::types::b_int64{WDate::currentDate().toJulianDay()});
        filter.append(Key::bitisDate,item);
    }

    return filter;
}

Signal<NoClass> &KaravanSorguWidget::sorguClicked()
{
    return _SorguClicked;
}


} // namespace Karavan
} // namespace v2
