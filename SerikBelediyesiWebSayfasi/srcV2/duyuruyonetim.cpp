#include "duyuruyonetim.h"
v2::Duyuru::DuyuruYonetim::DuyuruYonetim(DB *_mdb)
    :ContainerWidget("Duyurular"),SerikBLDCore::ListItem<v2::Duyuru::DuyuruItem>(_mdb)
{
    mControllerWidget = this->getContoller();
    if( mControllerWidget ){
        mControllerWidget.value()->BackClicked().connect([=](){
            this->back(Duyuru::DuyuruItem());
        });
        mControllerWidget.value()->NextClicked().connect([=](){
            this->next(Duyuru::DuyuruItem());
        });
        mControllerWidget.value()->setText(std::to_string(this->currentPage())+"/"+std::to_string(this->totalPage()));
    }
    {
        auto ListeleContainerWidget = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        ListeleContainerWidget->setContentAlignment(AlignmentFlag::Center);
        ListeleContainerWidget->setPadding(5,Side::Top|Side::Bottom);
        ListeleContainerWidget->decorationStyle().setCursor(Cursor::PointingHand);
        ListeleContainerWidget->addNew<WText>("Listele");
        ListeleContainerWidget->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);
        ListeleContainerWidget->clicked().connect([=](){ this->UpdateList();});
        ListeleContainerWidget->addStyleClass(Bootstrap::Grid::Offset::Large::col_lg_8+
                                              Bootstrap::Grid::Offset::Medium::col_md_8+
                                              Bootstrap::Grid::Offset::Small::col_sm_8+
                                              Bootstrap::Grid::Offset::ExtraSmall::col_xs_8+
                                              Bootstrap::Grid::Large::col_lg_2+
                                              Bootstrap::Grid::Medium::col_md_2+
                                              Bootstrap::Grid::Small::col_sm_2+
                                              Bootstrap::Grid::ExtraSmall::col_xs_2);
        this->Header()->setMargin(25,Side::Top|Side::Bottom);
        this->Header()->setContentAlignment(AlignmentFlag::Right);
    }
    {
        auto ListeleContainerWidget = this->Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        ListeleContainerWidget->setContentAlignment(AlignmentFlag::Center);
        ListeleContainerWidget->decorationStyle().setCursor(Cursor::PointingHand);
        ListeleContainerWidget->setPadding(5,Side::Top|Side::Bottom);
        ListeleContainerWidget->addNew<WText>("Yeni Ekle");
        ListeleContainerWidget->addStyleClass(Bootstrap::ContextualBackGround::bg_danger);
        ListeleContainerWidget->clicked().connect(this,&DuyuruYonetim::yeniDuyuruEkle);
        ListeleContainerWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                              Bootstrap::Grid::Medium::col_md_2+
                                              Bootstrap::Grid::Small::col_sm_2+
                                              Bootstrap::Grid::ExtraSmall::col_xs_2);
        this->Header()->setMargin(25,Side::Top|Side::Bottom);
        this->Header()->setContentAlignment(AlignmentFlag::Right);
    }
    this->UpdateList(Duyuru::DuyuruItem());
}
void v2::Duyuru::DuyuruYonetim::onList(const QVector<DuyuruItem> *mlist)
{
    this->Content()->clear();
    if( mControllerWidget ){
        mControllerWidget.value()->setText(std::to_string(this->currentPage())+"/"+std::to_string(this->totalPage()));
    }
    for( const auto &item : *mlist ){
        auto container = Content()->addNew<Widget::ListItem>(item);
        container->setMargin(5,Side::Bottom);
        container->clicked().connect([=](){
            this->initDuyuruItem(item.oid().value().to_string());
        });
    }
}
void v2::Duyuru::DuyuruYonetim::errorOccured(const std::string &errorText)
{
    this->showMessage("HATA",errorText);
}

void v2::Duyuru::DuyuruYonetim::initDuyuruItem(const std::string &duyuruOid)
{
    this->Content()->clear();
    DuyuruItem filter;
    filter.setOid(duyuruOid);

    auto duyuruItem = this->FindOneItem(filter);
    std::map<std::string,std::string> fileMap;
    for( const auto &[fileOid,fileTitle] : duyuruItem.fileOidList() ){
        fileMap[fileTitle]=this->downloadFileWeb(fileOid.c_str());
    }
    auto duyuruWidget = this->Content()->addNew<Widget::DuyuruView>(duyuruItem,this->getDB(),false);
    duyuruWidget->setMargin(25,Side::Left|Side::Right);
    duyuruWidget->UpdateListRequired().connect([=](){
        this->UpdateList();
    });
}

void v2::Duyuru::DuyuruYonetim::yeniDuyuruEkle()
{

    if( newFileList.size() ){
        bool deleted = true;
        for( const auto &[ fileOid , filetitle ] : newFileList ){
            if( deleted ){
                if( this->deleteGridFS(fileOid.c_str()) ){
                    deleted = false;
                }
            }
        }
        if( deleted ){
            newFileList.clear();
        }
    }

    this->Content()->clear();

    auto baslikLineEdit = this->Content()->addNew<WLineEdit>();
    baslikLineEdit->setPlaceholderText("Başlık Giriniz");
    baslikLineEdit->addStyleClass(Bootstrap::Grid::col_full_12);
    this->Content()->addNew<WBreak>();
    auto icerikContainer = this->Content()->addNew<WContainerWidget>();
    icerikContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    auto icerikTextEdit = icerikContainer->addNew<WTextEdit>();
    icerikContainer->setMargin(25,Side::Top|Side::Bottom);
    icerikTextEdit->setHeight(350);


    auto startContainer = Content()->addNew<WContainerWidget>();
    startContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
    auto startTarih = startContainer->addNew<WDateEdit>();
    startTarih->setPlaceholderText("İlk Yayınlanma Tarihi");

    auto lastContainer = Content()->addNew<WContainerWidget>();
    lastContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+
                                 Bootstrap::Grid::Medium::col_md_4+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_4);
    auto lastTarih = lastContainer->addNew<WDateEdit>();
    lastTarih->setPlaceholderText("Son Yayınlanma Tarihi");

    auto containerComboBox = Content()->addNew<WContainerWidget>();
    containerComboBox->addStyleClass(Bootstrap::Grid::Large::col_lg_4+
                                     Bootstrap::Grid::Medium::col_md_4+
                                     Bootstrap::Grid::Small::col_sm_4+
                                     Bootstrap::Grid::ExtraSmall::col_xs_4);
    auto birimComboBox = containerComboBox->addNew<WComboBox>();
    auto list = this->getBirimler();
    for( const auto &item : list ){
        birimComboBox->addItem(item.toStdString());
    }




    auto fileListContainer = Content()->addNew<WContainerWidget>();
    fileListContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    fileListContainer->setMinimumSize(WLength::Auto,50);
    //    fileListContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_info);
    fileListContainer->setMargin(15,Side::Top|Side::Bottom);

    auto addFileContainer = [=]( const std::string &fileOid , const std::string &fileTitle ){

        newFileList[fileOid] = fileTitle;
        auto container = fileListContainer->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->addStyleClass(CSSStyle::Shadows::shadow8px+CSSStyle::Radius::radius3px+CSSStyle::Gradient::grayGradient90);
        container->setMargin(5,Side::Top|Side::Bottom);


        auto mFileTitleLineEdit = container->addNew<WText>();
        mFileTitleLineEdit->addStyleClass(Bootstrap::Grid::Large::col_lg_8+
                                          Bootstrap::Grid::Medium::col_md_8+
                                          Bootstrap::Grid::Small::col_sm_8+
                                          Bootstrap::Grid::ExtraSmall::col_xs_8);
        mFileTitleLineEdit->setText(fileTitle);
        mFileTitleLineEdit->setAttributeValue(Style::style,Style::color::color(Style::color::Grey::Black));

        auto mFileOidText = container->addNew<WText>();
        mFileOidText->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                                    Bootstrap::Grid::Medium::col_md_3+
                                    Bootstrap::Grid::Small::col_sm_3+
                                    Bootstrap::Grid::ExtraSmall::col_xs_3);
        mFileOidText->setText(fileOid);


        auto mFileOidDelText = container->addNew<WText>("X");
        mFileOidDelText->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                       Bootstrap::Grid::Medium::col_md_1+
                                       Bootstrap::Grid::Small::col_sm_1+
                                       Bootstrap::Grid::ExtraSmall::col_xs_1);
        mFileOidDelText->setAttributeValue(Style::style,Style::color::color(Style::color::White::AliceBlue)
                                           +Style::background::color::color(Style::color::Red::DarkRed));

        mFileOidDelText->decorationStyle().setCursor(Cursor::PointingHand);

        mFileOidDelText->clicked().connect([=](){
            if( this->deleteGridFS(mFileOidText->text().toUTF8().c_str()) ){
                this->showMessage("Bilgi","Dosya Silindi");
                fileListContainer->removeWidget(container);
                newFileList.erase(mFileOidText->text().toUTF8().c_str());
            }else{
                this->showPopUpMessage("Hata: " + this->getLastError().toStdString(),"warn");
            }
        });


    };

    if( newFileList.size() ){
        fileListContainer->removeStyleClass(Bootstrap::ContextualBackGround::bg_info);
        fileListContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_danger);
        fileListContainer->addNew<WText>("Silinmemiş Dosyalar Var. Önce Onları Siliniz");
    }else{


        auto mFileTitleLineEdit = Content()->addNew<WLineEdit>();
        mFileTitleLineEdit->addStyleClass(Bootstrap::Grid::col_full_12);
        mFileTitleLineEdit->setPlaceholderText("Dosya Adını Giriniz");


        auto mFileUploaderWidget = Content()->addNew<FileUploaderWidget>("PDF Dosya Yükle");
        mFileUploaderWidget->addStyleClass(Bootstrap::Grid::col_full_12);
        mFileUploaderWidget->setType(FileUploaderWidget::Pdf);

        mFileUploaderWidget->Uploaded().connect([=](){

            if( mFileTitleLineEdit->text().toUTF8().size() < 5 ){
                this->showMessage("Uyarı","Dosya Açıklaması Çok Az");
                return;
            }

            auto fileOid = this->uploadfile(mFileUploaderWidget->fileLocation());

            addFileContainer(fileOid.get_oid().value.to_string(),mFileTitleLineEdit->text().toUTF8());

        });

        auto saveBtnContainer = this->Content()->addNew<WContainerWidget>();
        saveBtnContainer->addStyleClass(Bootstrap::Grid::col_full_12);
        saveBtnContainer->addStyleClass(CSSStyle::Button::blueButton);
        saveBtnContainer->addNew<WText>("Kaydet");
        saveBtnContainer->setPadding(5,Side::Top|Side::Bottom);

        saveBtnContainer->clicked().connect([=](){

            DuyuruItem duyuruItem;
            duyuruItem.setBaslik(baslikLineEdit->text().toUTF8());
            duyuruItem.setIcerik(icerikTextEdit->text().toUTF8());
            duyuruItem.setJulianDate(startTarih->date().toJulianDay(),lastTarih->date().toJulianDay());
            duyuruItem.setYayinda(false);
            duyuruItem.setBirim(birimComboBox->currentText().toUTF8());
            for( const auto &[ fileOid , fileTitle ] : newFileList ){
                duyuruItem.addFile(fileTitle,fileOid);
            }

            if( this->InsertItem(duyuruItem).size() ){
                this->showMessage("Bilgi","Ekledi");
                this->UpdateList();
            }else{
                this->showMessage("Hata","Duyuru Eklenemedi");

            }
        });
    }
}



std::string v2::Duyuru::DuyuruItem::Baslik() const
{
    auto val = this->element(Key::baslik);
    if( val ){
        return val->get_utf8().value.to_string();
    }
    return "";
}
bool v2::Duyuru::DuyuruItem::Yayinda() const
{
    auto val = this->element(Key::yayinda);
    if( val ){
        return val->get_utf8().value.to_string() == "Yayında" ? true : false;
    }
    return false;
}
std::string v2::Duyuru::DuyuruItem::Birim() const
{
    auto val = this->element(Key::birim);
    if( val ){
        return val->get_utf8().value.to_string();
    }
    return "";
}
std::string v2::Duyuru::DuyuruItem::SonTarihText() const
{
    auto val = this->element(Key::julianBitis);
    if( val ){
        auto str = QDate::fromJulianDay(val.value().get_int64().value).toString("dd/MM/yyyy").toStdString();
        return str;
    }
    return "";
}
std::string v2::Duyuru::DuyuruItem::Icerik() const
{
    auto val = this->element(Key::icerik);
    if( val ){
        return val.value().get_utf8().value.to_string();
    }
    return "";
}
int64_t v2::Duyuru::DuyuruItem::BaslangicDate() const
{
    auto val = this->element(Key::julianBaslangic);
    if( val ){
        return val->get_int64().value;
    }
    return 0;
}
int64_t v2::Duyuru::DuyuruItem::BitisDate() const
{
    auto val = this->element(Key::julianBitis);
    if( val ){
        return val->get_int64().value;
    }
    return 0;
}

std::vector<std::string> v2::Duyuru::DuyuruItem::fileList()
{
    std::vector<std::string> list;

    auto val = this->element(Key::fileList);
    if( val ){
        auto ar = val->get_array().value;

        for( const auto &item : ar ){
            list.push_back(item.get_oid().value.to_string());
        }
    }
    return list;
}

std::map<std::string,std::string> v2::Duyuru::DuyuruItem::fileOidList() const
{
    std::map<std::string,std::string> list;

    auto val = this->element(Key::fileOidList);
    if( val ){
        auto ar = val->get_array().value;

        for( const auto &item : ar ){
            try {
                list[item.get_document().value[Key::FILEKEY::fileOid].get_oid().value.to_string()] = item.get_document().value[Key::FILEKEY::fileTitle].get_utf8().value.to_string();
            } catch (bsoncxx::exception &e) {
                LOG << e.what();
            }
        }
    }
    return list;
}


v2::Duyuru::DuyuruItem &v2::Duyuru::DuyuruItem::setBaslik(const std::string &baslik)
{
    this->append(Duyuru::Key::baslik,baslik);
    return *this;
}

v2::Duyuru::DuyuruItem &v2::Duyuru::DuyuruItem::setYayinda(const bool &yayinda)
{
    this->append(Duyuru::Key::yayinda,(yayinda ? "Yayında" : "Yayında Değil"));
    return *this;
}

v2::Duyuru::DuyuruItem &v2::Duyuru::DuyuruItem::setJulianDate(const int64_t &start_date, const int64_t &end_date)
{
    this->append(Duyuru::Key::julianBaslangic,start_date);
    this->append(Duyuru::Key::julianBitis,end_date);
    return *this;
}

v2::Duyuru::DuyuruItem &v2::Duyuru::DuyuruItem::setIcerik(const std::string &icerik)
{
    this->append(Duyuru::Key::icerik,icerik);
    return *this;
}

v2::Duyuru::DuyuruItem &v2::Duyuru::DuyuruItem::addFile(const std::string &fileTitle, const std::string &fileOid)
{
    SerikBLDCore::Item _item("");
    _item.append(Key::FILEKEY::fileOid,bsoncxx::oid{fileOid});
    _item.append(Key::FILEKEY::fileTitle,fileTitle);
    this->pushArray(Key::fileOidList,_item);
    return *this;
}

v2::Duyuru::DuyuruItem &v2::Duyuru::DuyuruItem::removeFile(const std::string &fileOid)
{
    auto arList = this->element(Key::fileOidList);
    if( arList ){

        std::vector<SerikBLDCore::Item> arItemList;
        for( const auto &item : arList.value().get_array().value ){
            SerikBLDCore::Item __item("");
            __item.setDocumentView(item.get_document().value);
            arItemList.push_back(__item);
        }
        this->removeElement(Key::fileOidList);
        for( const auto &item : arItemList ){
            auto file_oid = item.element(Key::FILEKEY::fileOid);
            if( file_oid ){
                if( file_oid->get_oid().value.to_string() != fileOid ){
                    this->pushArray(Key::fileOidList,item);
                }
            }
        }
    }
    return *this;
}

v2::Duyuru::DuyuruItem &v2::Duyuru::DuyuruItem::setBirim(const std::string &birim)
{
    this->append(Duyuru::Key::birim,birim);
    return *this;
}



v2::Duyuru::Widget::ListItem::ListItem(const DuyuruItem &item)
    :DuyuruItem(item)
{
    this->addStyleClass(CSSStyle::Shadows::shadow8px+CSSStyle::Radius::radius3px);
    this->addStyleClass(Bootstrap::Grid::col_full_12);
    //Birim
    {
        auto container = this->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_3+
                                 Bootstrap::Grid::Medium::col_md_3+
                                 Bootstrap::Grid::Small::col_sm_3+
                                 Bootstrap::Grid::ExtraSmall::col_xs_3);
        container->addNew<WText>(this->Birim());
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::AntiqueWhite));
    }
    //Başlık
    {
        auto container = this->addNew<WContainerWidget>();
        container->setContentAlignment(AlignmentFlag::Left);
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_7+
                                 Bootstrap::Grid::Medium::col_md_7+
                                 Bootstrap::Grid::Small::col_sm_7+
                                 Bootstrap::Grid::ExtraSmall::col_xs_7);
        container->addNew<WText>(this->Baslik().size() > 70 ? this->Baslik().substr(0,70)+"..." : this->Baslik() );
    }
    //Yayında
    {
        auto container = this->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                 Bootstrap::Grid::Medium::col_md_1+
                                 Bootstrap::Grid::Small::col_sm_1+
                                 Bootstrap::Grid::ExtraSmall::col_xs_1);
        std::string sonDurum = "";
        if( this->Yayinda() ){
            if( QDate::currentDate().toJulianDay() < this->BaslangicDate() ){
                sonDurum = "Yaklaşıyor";
            }else{
                if( QDate::currentDate().toJulianDay() > this->BitisDate() ){
                    sonDurum = "Geçmiş";
                }else{
                    sonDurum = "Yayında";
                }
            }
        }else{
            sonDurum = "Değil";
        }
        container->addNew<WText>(sonDurum);
        if( sonDurum == "Yayında" ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::LightGreen));
        }else if( sonDurum == "Yaklaşıyor" ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::Green));
        }else if( sonDurum == "Geçmiş" ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Green::DarkSeaGreen));
        }else if( sonDurum == "Değil" ){
            container->setAttributeValue(Style::style,Style::background::color::color(Style::color::Red::Red));
        }
    }
    //Son Tarih
    {
        auto container = this->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_1+
                                 Bootstrap::Grid::Medium::col_md_1+
                                 Bootstrap::Grid::Small::col_sm_1+
                                 Bootstrap::Grid::ExtraSmall::col_xs_1);
        container->addNew<WText>(this->SonTarihText());
    }
    decorationStyle().setCursor(Cursor::PointingHand);
}

v2::Duyuru::Widget::DuyuruView::DuyuruView(const DuyuruItem &item ,SerikBLDCore::DB* _mDB, const bool &publicView)
    :DuyuruItem(item),mPublicView(publicView),mDB(_mDB)
{

    this->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::AliceBlue));
    if( mPublicView ){
        this->loadPublic();
        this->addStyleClass(CSSStyle::Shadows::shadow8px);
    }else{
        this->loadEditable();
    }
}

void v2::Duyuru::Widget::DuyuruView::loadPublic()
{

    auto birimText = this->Content()->addNew<WText>("<h4>"+this->Birim()+"</h4>",TextFormat::UnsafeXHTML);
    birimText->addStyleClass(Bootstrap::Grid::col_full_12);
    birimText->addStyleClass(Bootstrap::ContextualBackGround::bg_info);


    auto baslikText = this->Content()->addNew<WText>("<h4>"+this->Baslik()+"</h4>",TextFormat::UnsafeXHTML);
    baslikText->addStyleClass(Bootstrap::Grid::col_full_12);
    baslikText->setAttributeValue(Style::style,Style::background::color::color(Style::color::Grey::DimGray)+Style::color::color(Style::color::White::AliceBlue));

    auto icerikText = this->Content()->addNew<WText>(this->Icerik(),TextFormat::UnsafeXHTML);
//    icerikText->setPadding(25,Side::Top|Side::Bottom);
    icerikText->addStyleClass(Bootstrap::Grid::col_full_12);
    icerikText->setMargin(25,Side::Bottom);
    icerikText->setPadding(15,Side::Left|Side::Right);

    auto fileListContainer = this->Content()->addNew<WContainerWidget>();
    fileListContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    fileListContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::FloralWhite));
    fileListContainer->setPadding(10,Side::Top|Side::Bottom);
    fileListContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    {
        auto container = fileListContainer->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->addNew<WText>("Ekler");
    }
    for( const auto &[fileOid,fileTitle] : this->fileOidList() ){
        auto fileUrl = this->mDB->downloadFileWeb(fileOid.c_str());
        auto container = fileListContainer->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        Wt::WLink link = Wt::WLink(LinkType::Url,fileUrl);
        link.setTarget(Wt::LinkTarget::NewWindow);
        std::unique_ptr<Wt::WAnchor> anchor =
                std::make_unique<Wt::WAnchor>(link,
                                              "<b>"+fileTitle+"</b>");
        container->addWidget(std::move(anchor));
    }

    auto tarihText = this->Content()->addNew<WText>("<h5>"+QDate::fromJulianDay(this->BaslangicDate()).toString("dd/MM/yyyy").toStdString()+"  -  " + QDate::fromJulianDay(this->BitisDate()).toString("dd/MM/yyyy").toStdString()+"</h5>",TextFormat::UnsafeXHTML);
    tarihText->addStyleClass(Bootstrap::Grid::col_full_12);
    tarihText->addStyleClass(Bootstrap::ContextualBackGround::bg_danger);


    if( this->BitisDate() > QDate::currentDate().toJulianDay() ){
        auto kalanGunText = this->Content()->addNew<WText>("Yayından Kalkmasına " + std::to_string(this->BitisDate() - QDate::currentDate().toJulianDay())+" gün var",TextFormat::UnsafeXHTML);
        kalanGunText->addStyleClass(Bootstrap::Grid::col_full_12);
        kalanGunText->addStyleClass(Bootstrap::ContextualBackGround::bg_info);
    }else{
        auto kalanGunText = this->Content()->addNew<WText>("Süresi Dolmuş",TextFormat::UnsafeXHTML);
        kalanGunText->addStyleClass(Bootstrap::Grid::col_full_12);
        kalanGunText->addStyleClass(Bootstrap::ContextualBackGround::bg_info);
    }
}

void v2::Duyuru::Widget::DuyuruView::loadEditable()
{

    this->Content()->clear();
    auto baslikText = this->Content()->addNew<WLineEdit>();
    baslikText->setText(this->Baslik());
    baslikText->addStyleClass(Bootstrap::Grid::col_full_12);
    baslikText->setMargin(30,Side::Bottom);

    auto icerikTextContainer = this->Content()->addNew<WContainerWidget>();
    icerikTextContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    icerikTextContainer->setMargin(25,Side::Top|Side::Bottom);

    auto icerikText = icerikTextContainer->addNew<WTextEdit>();
    icerikText->setText(this->Icerik());
    icerikText->setMargin(25,Side::Bottom);
    icerikText->setMargin(63,Side::Top);
    icerikText->setHeight(350);

    Content()->addNew<WContainerWidget>()->setHeight(10);

    auto yayindaContainer = this->Content()->addNew<WContainerWidget>();
    yayindaContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    auto yayindaCheckBox = yayindaContainer->addNew<WCheckBox>();
    yayindaCheckBox->setChecked(this->Yayinda());

    auto startContainer = Content()->addNew<WContainerWidget>();
    startContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+
                                  Bootstrap::Grid::Medium::col_md_4+
                                  Bootstrap::Grid::Small::col_sm_4+
                                  Bootstrap::Grid::ExtraSmall::col_xs_4);
    auto startTarih = startContainer->addNew<WDateEdit>();
    startTarih->setPlaceholderText("İlk Yayınlanma Tarihi");
    startTarih->setDate(WDate::fromJulianDay(this->BaslangicDate()));

    auto lastContainer = Content()->addNew<WContainerWidget>();
    lastContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4+
                                 Bootstrap::Grid::Medium::col_md_4+
                                 Bootstrap::Grid::Small::col_sm_4+
                                 Bootstrap::Grid::ExtraSmall::col_xs_4);
    auto lastTarih = lastContainer->addNew<WDateEdit>();
    lastTarih->setPlaceholderText("Son Yayınlanma Tarihi");
    lastTarih->setDate(WDate::fromJulianDay(this->BitisDate()));

    auto containerComboBox = Content()->addNew<WContainerWidget>();
    containerComboBox->addStyleClass(Bootstrap::Grid::Large::col_lg_4+
                                     Bootstrap::Grid::Medium::col_md_4+
                                     Bootstrap::Grid::Small::col_sm_4+
                                     Bootstrap::Grid::ExtraSmall::col_xs_4);
    auto birimComboBox = containerComboBox->addNew<WComboBox>();
    std::int64_t count = 0;
    auto list = mDB->getBirimler();
    for( const auto &item : list ){
        birimComboBox->addItem(item.toStdString());
        if( this->Birim() == item.toStdString() ){
            birimComboBox->setCurrentIndex(count);
        }
        count++;
    }

    auto fileListContainer = this->Content()->addNew<WContainerWidget>();
    fileListContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    fileListContainer->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::FloralWhite));
    fileListContainer->setPadding(10,Side::Top|Side::Bottom);
    fileListContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    fileListContainer->setMargin(3,Side::Top|Side::Bottom);

    for( const auto &[fileOid,fileTitle] : this->fileOidList() ){
        auto fileUrl = this->mDB->downloadFileWeb(fileOid.c_str());
        auto container = fileListContainer->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setMargin(3,Side::Top|Side::Bottom);
        container->setAttributeValue(Style::dataoid,fileOid.c_str());

        auto fileNameContainer = container->addNew<WContainerWidget>();
        fileNameContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_10+
                                         Bootstrap::Grid::Medium::col_md_10+
                                         Bootstrap::Grid::Small::col_sm_10+
                                         Bootstrap::Grid::ExtraSmall::col_xs_10);


        Wt::WLink link = Wt::WLink(LinkType::Url,fileUrl);
        link.setTarget(Wt::LinkTarget::NewWindow);
        std::unique_ptr<Wt::WAnchor> anchor =
                std::make_unique<Wt::WAnchor>(link,
                                              "<b>"+fileTitle+"</b>");
        fileNameContainer->addWidget(std::move(anchor));

        auto delFileNameContainer = container->addNew<WContainerWidget>();
        delFileNameContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                            Bootstrap::Grid::Medium::col_md_2+
                                            Bootstrap::Grid::Small::col_sm_2+
                                            Bootstrap::Grid::ExtraSmall::col_xs_2);
        delFileNameContainer->addStyleClass(Bootstrap::Button::Danger);
        delFileNameContainer->addNew<WText>("Sil?");
        delFileNameContainer->decorationStyle().setCursor(Cursor::PointingHand);
        delFileNameContainer->clicked().connect([=](){
            auto delDialogBtn = askConfirm("Silmek İstediğinize Emin misiniz?");
            delDialogBtn->clicked().connect([=](){

                this->removeFile(container->attributeValue(Style::dataoid).toUTF8());
                SerikBLDCore::Item delFilter(Key::Collection);
                delFilter.setOid(this->oid().value().to_string());

                SerikBLDCore::Item delElement("");
                delElement.append(Key::FILEKEY::fileOid,bsoncxx::oid{container->attributeValue(Style::dataoid).toUTF8()});

                auto upt = this->mDB->pullValue(delFilter,Key::fileOidList,delElement);
                if( upt ){
                    if( upt.value().modified_count() ){

                        if( this->mDB->deleteGridFS(container->attributeValue(Style::dataoid).toUTF8().c_str() ) ){
                            this->showPopUpMessage("Başarılı Bir Şekilde Silindi.");
                        }else{
                            this->showMessage("Uyarı","Dosya Duyurudan Ayrıldı ancak DB den Silinemedi");
                        }
                        this->loadEditable();
                    }else{
                        this->showPopUpMessage("Silinemedi!!! "+this->mDB->getLastError().toStdString());
                    }
                }else{
                    this->showPopUpMessage("Silinemedi! "+this->mDB->getLastError().toStdString());
                }


            });
        });

    }



    auto delContainer = this->Content()->addNew<WContainerWidget>();
    delContainer->addStyleClass(Bootstrap::Grid::col_full_12);
    delContainer->setMargin(10,Side::Top|Side::Bottom);
    auto delBtn = delContainer->addNew<WContainerWidget>();
    delBtn->decorationStyle().setCursor(Cursor::PointingHand);
    delBtn->setPadding(5,Side::Bottom|Side::Top);
    delBtn->addNew<WText>("Bu İlanı Sil?");
    delBtn->addStyleClass(Bootstrap::Button::Danger);
    delBtn->clicked().connect([=](){
        auto delDialogBtn = askConfirm("Silmek İstediğinize Emin misiniz?");
        delDialogBtn->clicked().connect([=](){
            bool delSuccesfully = true;
            for( const auto &[fileOid , fileTitle] : this->fileOidList() ){
                if( !this->mDB->deleteGridFS(fileOid.c_str()) ){
                    delSuccesfully = false;
                }
            }
            if( delSuccesfully ){
                if( mDB->deleteItem(*this) ){
                    this->showPopUpMessage("Bilgi: Duyuru Silindi");
                    this->_updateList.emit(NoClass());
                }
            }

        });
    });

    auto saveBtn = delContainer->addNew<WContainerWidget>();
    saveBtn->decorationStyle().setCursor(Cursor::PointingHand);
    saveBtn->setMargin(5,Side::Top);
    saveBtn->setPadding(5,Side::Bottom|Side::Top);
    saveBtn->addNew<WText>("Değiştir?");
    saveBtn->addStyleClass(Bootstrap::Button::Primary);
    saveBtn->clicked().connect([=](){
        auto delDialogBtn = askConfirm("Değiştirmek İstediğinize Emin misiniz?");
        delDialogBtn->clicked().connect([=](){
            this->setBaslik(baslikText->text().toUTF8());
            this->setIcerik(icerikText->text().toUTF8());
            this->setBirim(birimComboBox->currentText().toUTF8());
            this->setJulianDate(startTarih->date().toJulianDay(),lastTarih->date().toJulianDay());
            this->setYayinda(yayindaCheckBox->isChecked());
            auto upt = mDB->updateItem(*this);

            if( upt ){
                if( upt.value().result().modified_count() ){
                    this->showPopUpMessage("Değişiklik Kaydedildi");
                }else{
                    this->showPopUpMessage("Değişiklikler Kayıt Edilemedi");
                }
            }else{
                this->showPopUpMessage("Değişiklikler Kayıt Edilemedi");
            }
        });
    });
}

Signal<NoClass> &v2::Duyuru::Widget::DuyuruView::UpdateListRequired()
{
    return _updateList;
}





v2::Duyuru::Widget::PublicListView::PublicListView(DB *_mdb)
    :SerikBLDCore::ListItem<DuyuruItem>(_mdb)
{
    this->setLimit(100);
    Header()->setContentAlignment(AlignmentFlag::Center);
    Content()->setContentAlignment(AlignmentFlag::Center);

    {
        auto container = Header()->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::row);
        container->setMaximumSize(1280,WLength::Auto);
        container->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::Azure));

        {
            std::string path = "img/baskanBackGround.JPG";

            auto img = container->addWidget(cpp14::make_unique<WContainerWidget>());
            img->addStyleClass(Bootstrap::Grid::Large::col_lg_12);

            img->setAttributeValue(Style::style,Style::background::url(path)+Style::background::size::cover+Style::background::repeat::norepeat+Style::background::position::center_center);
            img->setHeight(100);
            img->setPadding(0,AllSides);
            img->decorationStyle().setCursor(Cursor::PointingHand);

            auto gradientContainer = img->addWidget(cpp14::make_unique<WContainerWidget>());
            gradientContainer->setHeight(100);
            gradientContainer->addStyleClass("SliderDetailTextBackground");
            auto layout = gradientContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
            layout->addStretch(1);
            auto serik = layout->addWidget(cpp14::make_unique<WText>("Duyurular"),0,AlignmentFlag::Bottom|AlignmentFlag::Center);
            serik->setAttributeValue(Style::style,Style::font::size::s36px+Style::color::color(Style::color::White::AliceBlue));

            img->clicked().connect(this,&PublicListView::initList);
        }
    }



    auto _container = this->Content()->addNew<WContainerWidget>();
//    _container->addStyleClass(Bootstrap::Grid::col_full_12);
    _container->setMaximumSize(1280,WLength::Auto);

    mListContainerWidget = _container->addNew<WContainerWidget>();
    mListContainerWidget->addStyleClass(Bootstrap::Grid::Large::col_lg_4+
                                        Bootstrap::Grid::Medium::col_md_4+
                                        Bootstrap::Grid::Small::col_sm_4+
                                        Bootstrap::Grid::ExtraSmall::col_xs_12);

    mContentContainer = _container->addNew<WContainerWidget>();
    mContentContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_8+
                                        Bootstrap::Grid::Medium::col_md_8+
                                        Bootstrap::Grid::Small::col_sm_8+
                                        Bootstrap::Grid::ExtraSmall::col_xs_12);

    this->initList();
}

void v2::Duyuru::Widget::PublicListView::onList(const QVector<DuyuruItem> *mlist)
{
    mListContainerWidget->clear();
    for( const auto &item : *mlist ){
        auto container = mListContainerWidget->addNew<WContainerWidget>();
        container->addStyleClass(Bootstrap::Grid::col_full_12);
        container->setContentAlignment(AlignmentFlag::Center);
        auto itemContainer = container->addNew<WContainerWidget>();
        itemContainer->setPadding(5,Side::Top|Side::Bottom);
        itemContainer->setMaximumSize(1280,WLength::Auto);
        itemContainer->addNew<WText>("<b>"+item.Birim()+"</b>");
        itemContainer->addNew<WBreak>();
        itemContainer->addNew<WText>(item.Baslik());
        itemContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_info+CSSStyle::Shadows::shadow8px+CSSStyle::Radius::radius3px);
        itemContainer->setMargin(2,Side::Bottom);
        itemContainer->decorationStyle().setCursor(Cursor::PointingHand);
        itemContainer->clicked().connect([=](){
            this->initDuyuruItem(item.oid().value().to_string());
        });
    }
    this->initDuyuruItem(mlist->first().oid()->to_string());
}

void v2::Duyuru::Widget::PublicListView::initList()
{
    DuyuruItem filter;
    filter.append(Duyuru::Key::julianBaslangic,bsoncxx::builder::basic::make_document(kvp("$lte",QDate::currentDate().toJulianDay())));
    filter.append(Duyuru::Key::julianBitis,bsoncxx::builder::basic::make_document(kvp("$gte",QDate::currentDate().toJulianDay())));
    filter.setYayinda(true);
    this->UpdateList(filter);
}

void v2::Duyuru::Widget::PublicListView::initDuyuruItem(const std::string &duyuruOid)
{
    DuyuruItem filter;
    filter.setOid(duyuruOid);
    auto val = this->FindOneItem(filter);
    mContentContainer->clear();

    auto item = mContentContainer->addNew<DuyuruView>(val,this->getDB());
    item->setMaximumSize(1280,WLength::Auto);
}
