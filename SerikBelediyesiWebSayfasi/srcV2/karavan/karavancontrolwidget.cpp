#include "karavancontrolwidget.h"
#include "SerikBelediyesiWebSayfasi/srcV2/vatandas/vatandaswidget.h"

#include <Wt/WDatePicker.h>
#include <Wt/WCalendar.h>

namespace v2 {
namespace Karavan {


MainWidget::MainWidget(SerikBLDCore::User *_mUser)
    :v2::Karavan::KaravanManager(_mUser->getDB())
{

//    this->setLimit(1000);
    mTCManager = new SerikBLDCore::TCManagerV2(_mUser->getDB());

    auto controller = this->Header()->addWidget(std::make_unique<KaravanControlWidget>());
    controller->saveClicked().connect([=](){


        v2::Karavan::KaravanItem item;
        item.setbaslangicDate(controller->getBaslangicDate());
        item.setbitisDate(controller->getBitisDate());
        item.setplakaNo(controller->getPlakaNo());
        item.setTCNO(controller->getTCNO());
        item.setPersonelOid(_mUser->oid().value());
        item.setTelefon(controller->getTelefon());


        if( this->InsertItem(item).size() ){
            this->UpdateList(mSorguWidget->Filter());
        }
    });


    mSorguWidget = this->Header()->addWidget(std::make_unique<KaravanSorguWidget>());



    controller->tCNOClicked().connect([=]( const std::string &tcno){
        auto confirmStr = QString::fromStdString(tcno);
        int comfirmValue = 0;
        for( int i = 0 ; i < confirmStr.size()-1 ; i++ ){
            comfirmValue += confirmStr.at(i).digitValue();
        }

        if( comfirmValue%10 == confirmStr.at(confirmStr.size()-1).digitValue() ){
            auto val = mTCManager->Load_byTCNO(tcno);
            if( val ){
                controller->setExistTCNO(_mUser->getDB(),val.value());
            }else{
                controller->setNewTCNO(tcno,mTCManager);
            }
        }else{
            this->showPopUpMessage("TCNO Hatalı","warn");
        }
    });



    this->UpdateList();

    auto pageController = this->getContoller();

    pageController.value()->NextClicked().connect([=](){
        this->next(mSorguWidget->Filter());
        pageController.value()->setText(WString("{1}/{2}").arg(this->currentPage()).arg(this->countItem(mSorguWidget->Filter())).toUTF8());

    });

    pageController.value()->BackClicked().connect([=](){
        this->back(mSorguWidget->Filter());
        pageController.value()->setText(WString("{1}/{2}").arg(this->currentPage()).arg(this->countItem(mSorguWidget->Filter())).toUTF8());

    });

    pageController.value()->setText(WString("{1}/{2}").arg(this->currentPage()).arg(this->countItem(KaravanItem())).toUTF8());

    mSorguWidget->sorguClicked().connect([=](){
        this->UpdateList(mSorguWidget->Filter());
        pageController.value()->setText(WString("{1}/{2}").arg(this->currentPage()).arg(this->countItem(mSorguWidget->Filter())).toUTF8());

    });




}




KaravanControlWidget::KaravanControlWidget()
{

    this->init();

}

void KaravanControlWidget::init(const std::string &tcno)
{
    this->Content()->clear();

    this->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::Gainsboro));
    this->setPadding(5,Side::Top|Side::Bottom);


    {
        auto _container = this->Content()->addWidget(std::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        mTCNOLineEdit = _container->addWidget(std::make_unique<WLineEdit>());
        mTCNOLineEdit->setPlaceholderText("TC NO Giriniz");
        mTCNOLineEdit->setInputMask("99999999999");

        if( tcno.size() == 11 ){
            mTCNOLineEdit->setText(tcno);
            mTCNOClicked.emit(tcno);

        }

        mTCNOLineEdit->textInput().connect([=](){
            if( mTCNOLineEdit->text().toUTF16().size() == 11 ){
                mTCNOClicked.emit(mTCNOLineEdit->text().toUTF8());
            }
        });


    }


    {
        auto _container = this->Content()->addWidget(std::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        mPlakaNoLineEdit = _container->addWidget(std::make_unique<WLineEdit>());
        mPlakaNoLineEdit->setPlaceholderText("Plaka No");
        mPlakaNoLineEdit->setInputMask("99->AAA-99999");
    }


    {
        auto _container = this->Content()->addWidget(std::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        mBaslangicDate = _container->addWidget(std::make_unique<WDateEdit>());
        mBaslangicDate->setDate(WDate::currentDate());
        mBaslangicDate->setFormat("dd/MM/yyyy");
    }


    {
        auto _container = this->Content()->addWidget(std::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                                  Bootstrap::Grid::Medium::col_md_3+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        mBitisDate = _container->addWidget(std::make_unique<WDateEdit>());
        mBitisDate->setEnabled(false);
        mBitisDate->setDate(WDate::fromJulianDay(WDate::currentDate().toJulianDay()+15));

        mBaslangicDate->calendar()->activated().connect([=](){
            mBitisDate->setDate(WDate::fromJulianDay(mBaslangicDate->date().toJulianDay()+15));
        });
    }


    {
        auto _container = this->Content()->addWidget(std::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_6+
                                  Bootstrap::Grid::ExtraSmall::col_xs_6);

        mKaydetButton = _container->addWidget(std::make_unique<WPushButton>("Kaydet"));

        mKaydetButton->clicked().connect([=](){

            if( mTCNOLineEdit->text().empty() ){
                this->showPopUpMessage("TC NO Girmediniz","warn");
                return;
            }

            if( mPlakaNoLineEdit->text().empty() ){
                this->showPopUpMessage("Plaka Girmediniz","warn");
                return;
            }

            if( mBaslangicDate->date().toJulianDay() < WDate::currentDate().toJulianDay()-15 ){
                this->showPopUpMessage("Başlangıç Tarihi Çok Eski","warn");
                return;
            }

            mSaveClicked.emit(NoClass());
        });
    }
}

void KaravanControlWidget::setExistTCNO(SerikBLDCore::DB* db,SerikBLDCore::TC *tcvalue)
{
    this->Header()->clear();

    auto tcview = this->Header()->addNew<VatandasWidget>(db,tcvalue);
    tcview->initTCView(tcvalue);
    mTelefonNo = tcvalue->CepTelefonu().toStdString();
}

void KaravanControlWidget::setNewTCNO(const std::string &tcno, SerikBLDCore::TCManagerV2 *mTCManager)
{
    this->Header()->clear();

    auto container = this->Header()->addNew<WContainerWidget>();
    container->addStyleClass(Bootstrap::Grid::col_full_12);
    container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

    auto titleContainer = container->addNew<WContainerWidget>();
    titleContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    titleContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DimGray));
    titleContainer->addNew<WText>("Yeni TC Kayıt");


    auto tcnoLineEdit = container->addNew<WLineEdit>();
    tcnoLineEdit->setPlaceholderText("TCNO Giriniz");
    tcnoLineEdit->addStyleClass(Bootstrap::Grid::col_full_12);
    tcnoLineEdit->setText(tcno);


    auto adsoyadLineEdit = container->addNew<WLineEdit>();
    adsoyadLineEdit->setPlaceholderText("Ad Soyad Giriniz");
    adsoyadLineEdit->addStyleClass(Bootstrap::Grid::col_full_12);
    adsoyadLineEdit->setFocus();

    auto ceptelLineEdit = container->addNew<WLineEdit>();
    ceptelLineEdit->setPlaceholderText("Cep Telefonu Giriniz");
    ceptelLineEdit->addStyleClass(Bootstrap::Grid::col_full_12);

    auto adresLineEdit = container->addNew<WLineEdit>();
    adresLineEdit->setPlaceholderText("Adres Giriniz");
    adresLineEdit->addStyleClass(Bootstrap::Grid::col_full_12);

    auto saveBtn = container->addNew<WPushButton>("Kaydet");

    saveBtn->addStyleClass(Bootstrap::Button::Primary);

    saveBtn->clicked().connect([=](){


        SerikBLDCore::TC tc;
        tc.setAdSoyad(adsoyadLineEdit->text().toUTF8().c_str())
            .setCepTelefonu(ceptelLineEdit->text().toUTF8().c_str())
            .setTCNO(tcnoLineEdit->text().toUTF8().c_str())
            .setTamAdress(adresLineEdit->text().toUTF8().c_str())
            .setMahalle("Karavan");

        mTelefonNo = ceptelLineEdit->text().toUTF8();


        if( mTCManager->InsertItem(tc).size() ){
            mTCNOClicked.emit(tcno);
        }

    });

}

Signal<NoClass> &KaravanControlWidget::saveClicked()
{
    return mSaveClicked;
}

std::string KaravanControlWidget::getTCNO() const
{
    return mTCNOLineEdit->text().toUTF8();
}

std::string KaravanControlWidget::getPlakaNo() const
{
    return mPlakaNoLineEdit->text().toUTF8();
}

int64_t KaravanControlWidget::getBaslangicDate() const
{
    return mBaslangicDate->date().toJulianDay();
}

int64_t KaravanControlWidget::getBitisDate() const
{
    return mBitisDate->date().toJulianDay();
}

std::string KaravanControlWidget::getTelefon() const
{
    return mTelefonNo;
}

Signal<std::string>& KaravanControlWidget::tCNOClicked()
{
    return mTCNOClicked;
}

ListWidget::ListWidget(const int &index , const KaravanItem &item)
    :v2::Karavan::KaravanItem(item)
{

    setPadding(3,Side::Top|Side::Bottom);
    const auto bootStr = Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3;

    auto indexContainer = this->addNew<WText>("<b>#"+std::to_string(index)+"</b>");
    indexContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

    auto plakaContainer = this->addNew<WText>(this->getPlakaNo());
    plakaContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

    auto tcnoContainer = this->addNew<WText>(this->getTcNO());
    tcnoContainer->addStyleClass(bootStr);

    auto telefonContainer = this->addNew<WText>(this->getTelefon());
    telefonContainer->addStyleClass(bootStr);

    auto baslangicContainer = this->addNew<WText>(WString("{1}").arg(QDate::fromJulianDay(this->getBaslangic()).toString("dd/MM/yyyy").toStdString()));
    baslangicContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);

    auto bitisContainer = this->addNew<WText>(WString("{1}").arg(QDate::fromJulianDay(this->getBitis()).toString("dd/MM/yyyy").toStdString()));
    bitisContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);


    if( WDate::currentDate().toJulianDay() == this->getBitis() ){
        auto kalanContainer = this->addNew<WText>(WString("Son Gün"));
        kalanContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kalanContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Red::Crimson)+Style::color::color("white"));

    }else if( WDate::currentDate().toJulianDay() > this->getBitis() ){
        auto kalanContainer = this->addNew<WText>(WString("Süresi Dolmuş"));
        kalanContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        kalanContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Red::DarkRed)+Style::color::color("white"));
    }else{
        auto kalanContainer = this->addNew<WText>(WString("Kalan:{1} Gün").arg(this->getBitis()-WDate::currentDate().toJulianDay()));
        kalanContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
//      kalanContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::Green)+Style::color::color("white"));
    }


    auto silContainer = this->addNew<WText>(WString("Sil?"));
    silContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
    silContainer->addStyleClass(CSSStyle::Button::redButton);
    silContainer->clicked().connect([=](){
        _silClicked.emit(this->oid().value().to_string());
    });


//    auto controlBtnContainer = this->addNew<WContainerWidget>();
//    auto controlBtnText = controlBtnContainer->addNew<WText>("Kapat");
//    controlBtnContainer->addStyleClass(bootStr);
    //    controlBtnContainer->addStyleClass(CSSStyle::Button::redButton);

//    this->decorationStyle().setCursor(Cursor::PointingHand);
    //    this->addStyleClass(CSSStyle::Button::grayButton);
}

Signal<std::string> &ListWidget::silClicked()
{
    return _silClicked;
}






} // namespace Karavan
} // namespace v2


void v2::Karavan::MainWidget::onList(const QVector<KaravanItem> *mlist)
{


    this->Content()->clear();
//    this->Content()->setMargin(25,Side::Top);


    const auto bootStr = Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3;

    auto addTitleCOntainer = [=](const std::string &title, const bool &color = false ){
        auto container = this->Content()->addNew<WContainerWidget>();
        container->addNew<WText>(title);
        container->addStyleClass(bootStr);
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DarkGray)+Style::color::color(Style::color::White::AliceBlue));
    };

    {
        auto container = this->Content()->addNew<WContainerWidget>();
        container->addNew<WText>("index");
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DarkGray)+Style::color::color(Style::color::White::AliceBlue));
    }
    addTitleCOntainer("Plaka");
    addTitleCOntainer("TCNO",true);
    addTitleCOntainer("Telefon");
//    addTitleCOntainer("Başlangıç");
//    addTitleCOntainer("Bitiş");

    {
        auto container = this->Content()->addNew<WContainerWidget>();
        container->addNew<WText>("Başlangıç");
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DarkGray)+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = this->Content()->addNew<WContainerWidget>();
        container->addNew<WText>("Bitiş");
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DarkGray)+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = this->Content()->addNew<WContainerWidget>();
        container->addNew<WText>("Kalan");
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+Bootstrap::Grid::Medium::col_md_2+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DarkGray)+Style::color::color(Style::color::White::AliceBlue));
    }

    {
        auto container = this->Content()->addNew<WContainerWidget>();
        container->addNew<WText>("##");
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+Bootstrap::Grid::Medium::col_md_1+Bootstrap::Grid::Small::col_sm_3+Bootstrap::Grid::ExtraSmall::col_xs_3);
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DarkGray)+Style::color::color(Style::color::White::AliceBlue));
    }
//    addTitleCOntainer("#");


    auto marginContainer = Content()->addNew<WContainerWidget>();
    marginContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    marginContainer->setMargin(10,Side::Bottom);



    bool color = false;
    int counter = 1;
    for(const auto &item : *mlist ){
        auto container = this->Content()->addNew<ListWidget>(this->skip()+counter++,item);
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        if( color ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::AliceBlue));
//            container->addStyleClass(CSSStyle::Button::grayButton);
        }else{
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::LightGray)/*+Style::color::color(Style::color::White::White)*/);
//            container->addStyleClass(CSSStyle::Button::blueButton);
        }
        container->setMargin(1,Side::Bottom);

//        container->clicked().connect([=](){
//            auto mDialog = createFlatDialog("Bilgiler");
//            mDialog->show();
//        });

        container->silClicked().connect([=](const std::string &itemOid){
            auto okBtn = this->askConfirm("Silmek İstediğinize Eminmisiniz?");
            okBtn->clicked().connect([=](){
                KaravanItem filter;
                filter.setOid(itemOid);

                auto del = this->DeleteItem(filter);

                if( del ){
                    this->UpdateList(mSorguWidget->Filter());
                }else{
                    this->showPopUpMessage(this->getLastError().toStdString(),"warn");
                }
            });
        });
        color = !color;
    }
}
