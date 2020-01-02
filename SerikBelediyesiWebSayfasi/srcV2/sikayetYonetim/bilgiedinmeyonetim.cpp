#include "bilgiedinmeyonetim.h"



BilgiEdinmeYonetim::BilgiEdinmeYonetim(mongocxx::database *_db, const bsoncxx::document::value &_userValue)
    :DBClass (_db),UserClass (_userValue)
{
    setContainerStyle(ContainerStyleType::ROW);

    if( this->User().yetki("Çağrı Merkezi") )
    {
        auto list = BilgiEdinmeItem::GetList(this->db());

        for( auto item : list )
        {
            if( item )
            {
                auto container = addWidget(cpp14::make_unique<WContainerWidget>());
                container->addStyleClass(Bootstrap::Grid::col_full_12);
                container->addWidget(cpp14::make_unique<BilgiEdinmeListWidget>(item.get()))->ClickBilgiEdinme().connect(this,&BilgiEdinmeYonetim::initBilgiEdinme);
            }
        }
    }else{
        auto birim = this->User().birim();
        if( birim )
        {

            auto filter = document{};

            try {
                filter.append(kvp(BilgiEdinmeKEY::birim,birim.value()));
            } catch (bsoncxx::exception &e) {
                std::string str = "ERROR: " + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + e.what();
                std::cout << str << std::endl;
            }

            auto list = BilgiEdinmeItem::GetList(this->db(),std::move(filter));

            for( auto item : list )
            {
                if( item )
                {
                    auto container = addWidget(cpp14::make_unique<WContainerWidget>());
                    container->addStyleClass(Bootstrap::Grid::col_full_12);
                    container->addWidget(cpp14::make_unique<BilgiEdinmeListWidget>(item.get()))->ClickBilgiEdinme().connect(this,&BilgiEdinmeYonetim::initBilgiEdinme);
                }
            }

        }
    }



}

void BilgiEdinmeYonetim::initBilgiEdinme(const bsoncxx::oid &oid)
{
    clear();
    addWidget(cpp14::make_unique<BilgiEdinmeWidget>(this->db(),oid,this->User()));
}

BilgiEdinmeListWidget::BilgiEdinmeListWidget( const BilgiEdinmeItem *_item)
    :DBClass (_item->db()),item(_item)
{
    setWidth(WLength("100%"));
    setContainerStyle(ContainerStyleType::ROW);
    addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    decorationStyle().setCursor(Cursor::PointingHand);
    setMargin(10,Side::Top);

    setHeight(50);
    setOverflow(Overflow::Hidden);

    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4);
        container->addWidget(cpp14::make_unique<WText>(this->item->Konu()));
    }

    {
        auto container = addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4);
        container->addWidget(cpp14::make_unique<WText>(this->item->Tarih()));
    }

    {
        auto container = addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_4);
        if( this->item->cevaplandi() )
        {
            container->addWidget(cpp14::make_unique<WText>("Cevaplandı"));
            setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(125,150),
                                                                         this->getRandom(225,250),
                                                                         this->getRandom(125,150)));
        }else{
            container->addWidget(cpp14::make_unique<WText>("Cevaplanmadı"));
            setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(225,250),
                                                                         this->getRandom(125,150),
                                                                         this->getRandom(125,150)));
        }
    }


    clicked().connect([=](){
        _ClickBilgiEdinme.emit(item->oid());
    });

}

Signal<bsoncxx::oid> &BilgiEdinmeListWidget::ClickBilgiEdinme()
{
    return _ClickBilgiEdinme;
}


BilgiEdinmeWidget::BilgiEdinmeWidget(mongocxx::database *_db, const bsoncxx::oid &_oid, const UserClass &user)
    :DBClass (_db),UserClass (user),mOid(_oid)
{

    setPadding(15,Side::Left|Side::Right);






    this->initWidget();


}

void BilgiEdinmeWidget::initWidget()
{

    clear();



    auto val = BilgiEdinmeItem::LoadBilgiEdinmeItem(this->db(),mOid.to_string());


    if( !val )
    {
        auto rContainer = addWidget(cpp14::make_unique<ContainerWidget>());
        rContainer->setContainerStyle(ContainerStyleType::ROW);
        rContainer->addWidget(cpp14::make_unique<WText>("Bilgi Edinme Dosyası Yüklenemedi"))
                ->addStyleClass(Bootstrap::Label::Danger);
        return;
    }

    mItem = (val.value());


    auto mContainer = addWidget(cpp14::make_unique<ContainerWidget>());
    mContainer->setContainerStyle(ContainerStyleType::CONTAINERFLUID);

    auto rContainer = mContainer->addWidget(cpp14::make_unique<ContainerWidget>());

    rContainer->setContainerStyle(ContainerStyleType::ROW);

    {
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        auto text = container->addWidget(cpp14::make_unique<WText>(mItem->Tarih()));
        container->setContentAlignment(AlignmentFlag::Center);
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        auto text = container->addWidget(cpp14::make_unique<WText>("<b><u>"+mItem->Konu()+"</u></b>"));
        container->setContentAlignment(AlignmentFlag::Center);
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        auto text = container->addWidget(cpp14::make_unique<WText>("TCNO: <b>"+mItem->TCNO()+"</b>"));
        container->setContentAlignment(AlignmentFlag::Center);
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        auto text = container->addWidget(cpp14::make_unique<WText>("Ad Soyad: <b>"+mItem->AdSoyad()+"</b>"));
        container->setContentAlignment(AlignmentFlag::Center);
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        auto text = container->addWidget(cpp14::make_unique<WText>("ePosta: "+mItem->ePosta()));
        container->setContentAlignment(AlignmentFlag::Center);
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        auto text = container->addWidget(cpp14::make_unique<WText>("Telefon: <b>"+mItem->telefon()+"</b>"));
        container->setContentAlignment(AlignmentFlag::Center);
    }

    {
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        auto text = container->addWidget(cpp14::make_unique<WText>("Adres: "+mItem->adres()));
        container->setContentAlignment(AlignmentFlag::Center);
    }


    {
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);

        container->setMargin(15,Side::Top);
        container->setAttributeValue(Style::style,Style::background::color::rgb(this->getRandom(200,225),this->getRandom(200,225),this->getRandom(200,225)));
        auto _rContainer = container->addWidget(cpp14::make_unique<ContainerWidget>());
        _rContainer->setContainerStyle(ContainerStyleType::ROW);



        auto container__ = _rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container__->addStyleClass(Bootstrap::Grid::Large::col_lg_9+
                                 Bootstrap::Grid::Medium::col_md_9+
                                 Bootstrap::Grid::Small::col_sm_9+
                                 Bootstrap::Grid::ExtraSmall::col_xs_8);
        auto vLayout_ = container__->setLayout(cpp14::make_unique<WVBoxLayout>());


        auto birimComboBox = vLayout_->addWidget(cpp14::make_unique<WComboBox>(),1,AlignmentFlag::Middle);

        auto list = this->BirimList();
        int _index = 0;
        bool Increment = true;
        birimComboBox->addItem("Yok");
        for( auto item : list )
        {
            if( mItem->birim() == item )
            {
                Increment = false;
            }else{
                if( Increment )
                {
                    _index++;
                }
            }
            birimComboBox->addItem(item);
        }

        if( mItem->birim().size() )
        {
            birimComboBox->setCurrentIndex(_index+1);
        }

        auto container___ = _rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container___->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                                    Bootstrap::Grid::Medium::col_md_3+
                                    Bootstrap::Grid::Small::col_sm_3+
                                    Bootstrap::Grid::ExtraSmall::col_xs_4);
        auto vLayout__ = container___->setLayout(cpp14::make_unique<WVBoxLayout>());

        auto svBtn = vLayout__->addWidget(cpp14::make_unique<WPushButton>("Kaydet"),0,AlignmentFlag::Middle);
        svBtn->addStyleClass(Bootstrap::Button::Primary);
        container___->setContentAlignment(AlignmentFlag::Justify);

        svBtn->clicked().connect([=](){
            if( birimComboBox->currentText() != "Yok" )
            {
                if( mItem->setBirim(birimComboBox->currentText().toUTF8()) ){
                    this->showMessage("Bilgi","Birim Değiştirildi.");
                }else{
                    this->showMessage("Hata","Bir Hata Oluştu");
                }
            }else{
                this->showMessage("Hata","Birim Seçmediniz");
            }

        });
    }


    {
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
        container->setMargin(15,Side::Top);

        auto text = container->addWidget(cpp14::make_unique<WText>("<h4><b>Mesaj</b></h4><p>"+mItem->mesaj()+"</p>"));
        container->setContentAlignment(AlignmentFlag::Center);
    }


    if( !mItem->cevaplandi() )
    {
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());


        layout->addWidget(cpp14::make_unique<WText>("Cevap Yok"),0,AlignmentFlag::Top|AlignmentFlag::Center);


        auto cContainer = rContainer->addWidget(cpp14::make_unique<FileUploaderWidget>());


        cContainer->Uploaded().connect([=](){

            BilgiEdinmeItem::Cevap cevap;
            cevap.mSaat = WTime::currentServerTime().toString("hh:mm").toUTF8();
            cevap.mTarih = WDate::currentServerDate().toJulianDay();

            auto val = this->uploadfile(cContainer->fileLocation());
            cevap.mCevapOid = val.get_oid().value.to_string();


            if( mItem->setCevap(cevap) ){
                this->initWidget();
            }
        });





    }else{
        auto container = rContainer->addWidget(cpp14::make_unique<ContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(15,Side::Top);
        container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);

        auto layout = container->setLayout(cpp14::make_unique<WVBoxLayout>());


        layout->addWidget(cpp14::make_unique<WText>("Cevap"),0,AlignmentFlag::Top|AlignmentFlag::Center);

        layout->addWidget(cpp14::make_unique<WText>("Saat: "+mItem->cevap().mSaat),0,AlignmentFlag::Top|AlignmentFlag::Center);

        layout->addWidget(cpp14::make_unique<WText>("Tarih :"+ WDate::fromJulianDay(mItem->cevap().mTarih).toString("dd/MM/yyyy")),0,AlignmentFlag::Top|AlignmentFlag::Center);


        auto fileName = this->downloadFile(mItem->cevap().mCevapOid);

        Wt::WLink link = Wt::WLink(LinkType::Url,"/"+fileName);
        link.setTarget(Wt::LinkTarget::NewWindow);

        std::unique_ptr<Wt::WAnchor> anchor =
                Wt::cpp14::make_unique<Wt::WAnchor>(link,
                                "<h5>Cevap Dosyası</h5>");

        anchor->addStyleClass(Bootstrap::Label::Primary);

        layout->addWidget(std::move(anchor),0,AlignmentFlag::Top|AlignmentFlag::Center);

    }

}
