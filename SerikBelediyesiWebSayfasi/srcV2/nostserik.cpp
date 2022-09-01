#include "nostserik.h"
#include "inlinestyle.h"
#include "bootstrap.h"
#include "CSSCustom.h"

#include "db.h"
#include <QImage>
#include <QMimeDatabase>

#include <iterator>
#include <algorithm>
#include <ranges>





v2::NostSerikManager::NostSerikManager(SerikBLDCore::DB *mDB)
    :SerikBLDCore::ListItem<NostItem>(mDB)
{
    this->initController();

    this->setLimit(24);
    this->UpdateList();

}

void v2::NostSerikManager::onList(const QVector<NostItem> *mlist)
{
    this->Content()->clear();

    for( const auto &item : *mlist ){

        auto fileurl = this->downloadFileWeb(item.getThumbNails().c_str());

        auto imgItem = this->Content()->addNew<NostItemThumb>(fileurl,item.getAciklama(),item.getFileOid());
        imgItem->Clicked().connect([=](const std::string &fileOid){

            auto _imgContainer = wApp->root()->addNew<WContainerWidget>();
            _imgContainer->setPositionScheme(PositionScheme::Fixed);
            _imgContainer->setOffsets(0,AllSides);
            _imgContainer->setWidth(WLength("100%"));
            _imgContainer->setHeight(WLength("100%"));
            _imgContainer->setAttributeValue(Style::style,Style::background::color::rgba(50,50,50,0.65));

            auto imgContainer = _imgContainer->addNew<WContainerWidget>();

            imgContainer->setPositionScheme(PositionScheme::Fixed);
            imgContainer->setOffsets(0,AllSides);
            imgContainer->setWidth(WLength("100%"));
            imgContainer->setHeight(WLength("100%"));
            auto fileUrl = this->downloadFileWeb(fileOid.c_str());
            imgContainer->setAttributeValue(Style::style,Style::background::url(fileUrl)
                                            +Style::background::size::contain
                                            +Style::background::position::center_center
                                            +Style::background::repeat::norepeat);

            auto textContainer = imgContainer->addNew<WContainerWidget>();
            textContainer->setWidth(WLength("100%"));
            textContainer->setPositionScheme(PositionScheme::Absolute);
            textContainer->setOffsets(0,Side::Left);
            textContainer->setOffsets(20,Side::Bottom);

            textContainer->setAttributeValue(Style::style,Style::background::color::rgba(50,50,50,0.75)+Style::color::rgb("255,255,255")+Style::font::size::s16px+Style::font::weight::bold);
            textContainer->setContentAlignment(AlignmentFlag::Center);
            textContainer->addNew<WText>(item.getAciklama(),TextFormat::UnsafeXHTML);
            textContainer->setPadding(15,AllSides);
            textContainer->setPadding(100,Side::Left|Side::Right);


            imgContainer->clicked().connect([=](){
                wApp->root()->removeWidget(_imgContainer);
            });
        });

        imgItem->DelClicked().connect([=](const std::string &fileOid ){
            auto askBtn = this->askConfirm("Silmek İstediğinize Emin misiniz?");
            askBtn->clicked().connect([=](){
                for( const auto &_item : *mlist ){
                    if( _item.getFileOid() == fileOid ){
                        if( this->deleteGridFS(fileOid.c_str()) ){
                            if( this->DeleteItem(_item) ){
                                this->showPopUpMessage("Döküman silinemedi");
                            }
                            this->UpdateList();
                        }else{
                            this->showPopUpMessage("Grid FS silinemedi");
                        }
                        break;
                    }
                }
            });
        });
    }

    mCurrentPageInfo->setText(std::to_string(this->currentPage()+1)+"/"+std::to_string(this->totalPage()+1));


}

void v2::NostSerikManager::initController()
{

    this->Header()->clear();

    this->Header()->addNew<WText>("<h3>Galeri Yönetim Paneli</h3>");
    this->Header()->setAttributeValue(Style::style,Style::Border::bottom::border_style("1px solid dimgray"));

    this->Footer()->clear();
    this->Footer()->setAttributeValue(Style::style,Style::Border::top::border_style("1px solid dimgray"));

    this->Footer()->setPadding(20,Side::Top|Side::Bottom);

    auto backContainer = this->Footer()->addWidget(cpp14::make_unique<WContainerWidget>());
    backContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4
                                 +Bootstrap::Grid::Medium::col_md_4
                                 +Bootstrap::Grid::Small::col_sm_4
                                 +Bootstrap::Grid::ExtraSmall::col_xs_4);
    auto backBtn = backContainer->addNew<WPushButton>("Geri");
    backBtn->addStyleClass(Bootstrap::Button::Primary);
    backBtn->clicked().connect([=](){
       this->back(NostItem());
    });


    auto pageTextContainer = this->Footer()->addWidget(cpp14::make_unique<WContainerWidget>());
    pageTextContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2
                                 +Bootstrap::Grid::Medium::col_md_2
                                 +Bootstrap::Grid::Small::col_sm_2
                                 +Bootstrap::Grid::ExtraSmall::col_xs_2);
    mCurrentPageInfo = pageTextContainer->addNew<WText>("Sayfa");




    auto nextContainer = this->Footer()->addWidget(cpp14::make_unique<WContainerWidget>());
    nextContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_4
                                 +Bootstrap::Grid::Medium::col_md_4
                                 +Bootstrap::Grid::Small::col_sm_4
                                 +Bootstrap::Grid::ExtraSmall::col_xs_4);
    auto nextBtn = nextContainer->addNew<WPushButton>("ileri");
    nextBtn->addStyleClass(Bootstrap::Button::Primary);
    nextBtn->clicked().connect([=](){
       this->next(NostItem());
    });

    auto addNewFileContainer = this->Footer()->addWidget(cpp14::make_unique<WContainerWidget>());
    addNewFileContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2
                                 +Bootstrap::Grid::Medium::col_md_2
                                 +Bootstrap::Grid::Small::col_sm_2
                                 +Bootstrap::Grid::ExtraSmall::col_xs_2);
    auto addBtn = addNewFileContainer->addNew<WPushButton>("Yeni Ekle");
    addBtn->addStyleClass(Bootstrap::Button::info);
    addBtn->clicked().connect(this,&NostSerikManager::addNewFile);
}

void v2::NostSerikManager::addNewFile()
{
    auto mDialog = createDialog("Yeni Resim Ekle");


    auto fileUploader = mDialog->contents()->addNew<FileUploaderWidget>("Resim Yükle");
    fileUploader->setType(FileUploaderWidget::Image);

    auto aciklamaLineEdit = mDialog->contents()->addNew<WTextEdit>();
    aciklamaLineEdit->setHeight(350);

    auto textOid = mDialog->contents()->addNew<WText>();
    mDialog->contents()->addNew<WBreak>();

    auto thumbOid = mDialog->contents()->addNew<WText>();
    mDialog->contents()->addNew<WBreak>();

    auto widthSpinBox = mDialog->contents()->addNew<WSpinBox>();
    auto heightSpinBox = mDialog->contents()->addNew<WSpinBox>();
    widthSpinBox->setMaximum(10000);
    heightSpinBox->setMaximum(10000);
    widthSpinBox->setEnabled(false);
    heightSpinBox->setEnabled(false);

    mDialog->contents()->addNew<WBreak>();


    auto textUyari = mDialog->contents()->addNew<WText>();
    textUyari->setText("Resmi Yükleyin ve Açıklama Giriniz");

    fileUploader->Uploaded().connect([=](){
       QImage img;
       if( img.load(fileUploader->fileLocation()) ){

           img = img.scaledToWidth(1024,Qt::SmoothTransformation);
           img.save(fileUploader->fileLocation());


           double width = static_cast<double>(img.width());
           double height = static_cast<double>(img.height());
           widthSpinBox->setValue(width);
           heightSpinBox->setValue(height);


           double ratio = width/height;

           if( ratio > 1.3333 ){
               auto _img = img.scaledToHeight(150);
               img = _img.copy((_img.width()-200)/2,0,200,150);
           }else{
               auto _img = img.scaledToWidth(200);
               img = _img.copy(0,(_img.height()-150)/2,200,150);
           }



           QMimeDatabase imgDatabase;
           auto mimeType = imgDatabase.mimeTypeForFile(fileUploader->fileLocation());

           if( img.save(fileUploader->fileLocation()+"-----."+mimeType.preferredSuffix()) ){
               auto fileOidValue_ = this->uploadfile(fileUploader->fileLocation()+"-----."+mimeType.preferredSuffix());
               thumbOid->setText(fileOidValue_.get_oid().value.to_string());

           }
       }

       auto fileOidValue = this->uploadfile(fileUploader->fileLocation());
       textOid->setText(fileOidValue.get_oid().value.to_string());

    });


    auto saveBtn = mDialog->footer()->addNew<WPushButton>("Kaydet");

    saveBtn->clicked().connect([=](){

        if( textOid->text().toUTF8().size() == 0 ){
            textUyari->setText("Resim Yüklemediniz");
            return;
        }

        if( aciklamaLineEdit->text().toUTF8().size() == 0 ){
            textUyari->setText("Açıklama Girmediniz");
            return;
        }


        NostItem item;
        item.setAciklama(aciklamaLineEdit->text().toUTF8());
        item.setFileOid(textOid->text().toUTF8());
        item.setThumbnail(thumbOid->text().toUTF8());
        item.setWidthHeight(widthSpinBox->value(),heightSpinBox->value());

        auto itemOid = this->InsertItem(item);

        if( !itemOid.empty() ){
            this->removeDialog(mDialog);
            this->UpdateList();
            this->showPopUpMessage("Resim Yüklendi");
        }else{
            textUyari->setText("Bir Hata Oluştu" + this->getLastError().toStdString());
        }

    });

    mDialog->show();
}

v2::NostItem::NostItem()
    :SerikBLDCore::Item("Galeri")
{

}

v2::NostItem::NostItem(const NostItem &other)
    :SerikBLDCore::Item("Galeri")
{
    this->clear();
    this->setDocumentView(other.view());
}

v2::NostItem::NostItem(const NostItem &&other)
    :SerikBLDCore::Item("Galeri")
{
    this->clear();
    this->setDocumentView(other.view());
}

v2::NostItem &v2::NostItem::operator=(const NostItem &&other)
{
    this->clear();
    this->setDocumentView(other.view());
    return *this;
}

v2::NostItem &v2::NostItem::setThumbnail(const std::string &fileOid)
{
    this->append(NostSerikKey::thumbNails,bsoncxx::oid{fileOid});
    return *this;
}

v2::NostItem &v2::NostItem::operator=(const NostItem &other)
{
    this->clear();
    this->setDocumentView(other.view());
    return *this;
}

v2::NostItem &v2::NostItem::setFileOid(const std::string &fileoid, const std::string &tipname)
{
    this->append(NostSerikKey::fileOid,bsoncxx::oid{fileoid});
    this->append(NostSerikKey::Tip,tipname);
    return *this;
}

v2::NostItem &v2::NostItem::setAciklama(const std::string &aciklama)
{
    this->append(NostSerikKey::Aciklama,aciklama);
    return *this;
}

v2::NostItem &v2::NostItem::setTip(const std::string &tipname)
{
    this->append(NostSerikKey::Tip,tipname);
    return *this;
}

v2::NostItem &v2::NostItem::setWidthHeight(const int &w, const int &h)
{

    this->append(NostSerikKey::width,bsoncxx::types::b_int32{w});
    this->append(NostSerikKey::height,bsoncxx::types::b_int32{h});
    return *this;

}

std::string v2::NostItem::getAciklama() const
{
    auto val = this->element(NostSerikKey::Aciklama);
    if( val ){
        if( val->type() == bsoncxx::type::k_utf8 ){
            return val->get_utf8().value.to_string();
        }
        return "";
    }
    return "";
}

std::string v2::NostItem::getTip() const
{
    auto val = this->element(NostSerikKey::Tip);
    if( val ){
        if( val->type() == bsoncxx::type::k_utf8 ){
            return val->get_utf8().value.to_string();
        }
        return "";
    }
    return "";
}

std::string v2::NostItem::getFileOid() const
{
    auto val = this->element(NostSerikKey::fileOid);
    if( val ){
        if( val->type() == bsoncxx::type::k_oid ){
            return val->get_oid().value.to_string();
        }
        return "";
    }
    return "";
}

std::string v2::NostItem::getThumbNails() const
{
    auto val = this->element(NostSerikKey::thumbNails);
    if( val ){
        if( val->type() == bsoncxx::type::k_oid ){
            return val->get_oid().value.to_string();
        }
        return "";
    }
    return "";
}

int v2::NostItem::getWidth() const
{
    auto val = this->element(NostSerikKey::width);
    if( val ){
        return val->get_int32().value;
    }
    return 0;
}

int v2::NostItem::getHeight() const
{
    auto val = this->element(NostSerikKey::height);
    if( val ){
        return val->get_int32().value;
    }
    return 0;
}

v2::NostItemThumb::NostItemThumb(const std::string &url, const std::string &aciklama, const std::string &fileOid)
    :mFileOid(fileOid)
{
    this->addStyleClass("NostItemThum");
    this->addStyleClass(Bootstrap::Grid::Large::col_lg_2
                        +Bootstrap::Grid::Medium::col_md_2
                        +Bootstrap::Grid::Small::col_sm_3
                        +Bootstrap::Grid::ExtraSmall::col_xs_3);
    this->setPositionScheme(PositionScheme::Relative);

    auto container = this->addNew<WContainerWidget>();
    container->decorationStyle().setCursor(Cursor::PointingHand);

    container->addStyleClass(CSSStyle::Radius::radius3px+CSSStyle::Shadows::shadow8px);
    container->setAttributeValue(Style::style,Style::background::url(url)+Style::background::repeat::norepeat+Style::background::size::cover);

    container->setHeight(150);
    container->setMargin(20,Side::Top|Side::Bottom);

//    if( aciklama.size() > 24 ){
//        QString str = QString::fromStdString(aciklama).mid(0,20)+"...";
//        this->addNew<WText>(str.toStdString());

//    }else{
//        this->addNew<WText>(aciklama);

//    }

    this->setMargin(5,Side::Top|Side::Bottom);

    container->clicked().connect([=](){
       _clicked.emit(mFileOid);
    });


    auto editContainer = addNew<WContainerWidget>();
    editContainer->setPositionScheme(PositionScheme::Absolute);
    editContainer->setWidth(WLength("100%"));
    editContainer->setHeight(20);
    editContainer->setOffsets(0,Side::Left|Side::Top);
    auto delBtn = editContainer->addNew<WContainerWidget>()->addNew<WText>("Sil");
    delBtn->addStyleClass(CSSStyle::Button::Red::IndianRedButton);
    delBtn->setPadding(5,AllSides);
    delBtn->clicked().connect([=](){
       _delClicked.emit(mFileOid);
    });



}

Signal<std::string> &v2::NostItemThumb::Clicked()
{
    return _clicked;
}

Signal<std::string> &v2::NostItemThumb::DelClicked()
{
    return _delClicked;
}



v2::NostSerik::NostSerik(SerikBLDCore::DB *mDB)
    :SerikBLDCore::ListItem<NostItem>(mDB)
{

    this->setAttributeValue(Style::style,Style::background::url("img/nostback.jpeg")
                            +Style::background::size::cover
                            +Style::background::repeat::norepeat);

    this->setPositionScheme(PositionScheme::Fixed);
    this->setOffsets(0,AllSides);

    this->setContentAlignment(AlignmentFlag::Center);
    this->clear();


    mContentContainer = this->addWidget(cpp14::make_unique<WContainerWidget>());
    mContentContainer->setMargin(90,Side::Top);
    mContentContainer->setMargin(50,Side::Bottom);
    mContentContainer->addStyleClass("NostListItems");
//    mContentContainer->setHtmlTagName("section");

    mContentContainer->setMaximumSize(1280,WLength::Auto);
    mContentContainer->setWidth(WLength("100%"));
    mContentContainer->setHeight(WLength("100%"));
    mContentContainer->setOverflow(Overflow::Scroll);
    mContentContainer->setContentAlignment(AlignmentFlag::Center);




    this->setLimit(1000);
    this->UpdateList();



}

void v2::NostSerik::onList(const QVector<NostItem> *mlist)
{

    Node *node;
    NostItem* item;
    Node *firstNode;

    Node *oldNode;

    for( const auto &item : *mlist ){
        mFList.append(item);
    }

    for( int i = 0 ; i < mlist->count() ; i++ ){


        if( i == 0 ){
            item = new NostItem();
            node = new Node();
            node->index = i;
            node->total = mlist->count();
            firstNode = node;
            item->setDocumentView(mlist->at(i).view());
            node->item = item;
            oldNode = node;

            item = new NostItem();
            if( mlist->count() > 1 ){
                item->setDocumentView(mlist->at(i+1).view());
                node->next = new Node(item);
                node->prev = nullptr;
            }

        }else{
            if( i == mlist->count()-1 ){
                node = node->next;
                node->index = i;
                node->prev = oldNode;

                node->next = firstNode;
                node->total = mlist->count();
                oldNode = node;
                firstNode->prev = node;
            }else{
                node->prev = node;
                node = node->next;
                node->index = i;
                //node->prev = oldNode;

                item = new NostItem();
                item->setDocumentView(mlist->at(i+1).view());
                node->next = new Node(item);
                node->total = mlist->count();
                oldNode = node;
            }


        }

    }

//    this->showItem(firstNode);
    this->showItem();

}

void v2::NostSerik::showItem( Node *item)
{

    mContentContainer->clear();

    auto fileName = this->downloadFileWeb(item->item->getFileOid().c_str());



    auto imgContainer = mContentContainer->addNew<WContainerWidget>();
    imgContainer->setMaximumSize(750,WLength::Auto);

    imgContainer->addStyleClass("NostListItem");
    imgContainer->setMargin(45,Side::Top);
    imgContainer->setWidth(WLength("100%"));
    imgContainer->setHeight(static_cast<double>(item->item->getHeight())*0.73);
    imgContainer->setPositionScheme(PositionScheme::Relative);
    imgContainer->setAttributeValue(Style::style,Style::background::url(fileName.c_str())
                                    +Style::background::size::contain
                                    +Style::background::repeat::norepeat);
    imgContainer->decorationStyle().setCursor(Cursor::PointingHand);


    imgContainer->clicked().connect([=](){

        auto _imgContainer = wApp->root()->addNew<WContainerWidget>();
        _imgContainer->setAttributeValue(Style::style,Style::background::color::rgba(255,255,255,0.99));


        auto text_Container = _imgContainer->addNew<WContainerWidget>();
        auto text = text_Container->addNew<WText>(item->item->getAciklama(),TextFormat::UnsafeXHTML);
        text_Container->setWidth(WLength("100%"));
        text_Container->setMaximumSize(1024,WLength::Auto);
        text_Container->setAttributeValue(Style::style,Style::color::color(Style::color::Grey::Black)+Style::font::size::s14px);
        text_Container->addStyleClass("nostTextShadow");
        text_Container->setMargin(60,Side::Top);


        _imgContainer->setPositionScheme(PositionScheme::Fixed);
        _imgContainer->setWidth(WLength("100%"));
        _imgContainer->setHeight(WLength("100%"));
        _imgContainer->setZIndex(12);
        _imgContainer->setOffsets(0,AllSides);
        _imgContainer->setContentAlignment(AlignmentFlag::Center);
        _imgContainer->setOverflow(Overflow::Auto);

        auto img = _imgContainer->addNew<WContainerWidget>();
        img->setAttributeValue(Style::style,Style::background::url(fileName.c_str())
                                        +Style::background::size::contain
                                        +Style::background::repeat::norepeat);
        img->setWidth(WLength("100%"));
        img->setMaximumSize(1024,WLength::Auto);
        img->setHeight(item->item->getHeight());
        img->addStyleClass("nostShadow");




        auto index_Container = img->addNew<WContainerWidget>();
        auto indexText = index_Container->addNew<WText>(std::to_string(item->index+1)+"/"+std::to_string(item->total)+"-"+std::to_string(item->item->getHeight()),TextFormat::UnsafeXHTML);
        index_Container->setAttributeValue(Style::style,Style::background::color::rgba(175,175,75,.75)+Style::color::color(Style::color::White::AliceBlue+Style::font::size::s20px+Style::font::weight::bold));
        index_Container->setPositionScheme(PositionScheme::Absolute);
        index_Container->setOffsets(0,Side::Top);
        index_Container->setOffsets(WLength("50%"),Side::Left);






        auto back_Container = img->addNew<WContainerWidget>();
        auto backText = back_Container->addNew<WText>("⬅",TextFormat::UnsafeXHTML);
        backText->setPadding(5,AllSides);
        back_Container->setAttributeValue(Style::style,Style::background::color::rgba(175,175,75,.75)+Style::color::color(Style::color::White::AliceBlue+Style::font::size::s36px+Style::font::weight::bold));
        back_Container->setPositionScheme(PositionScheme::Absolute);
        back_Container->setOffsets(0,Side::Top);
        back_Container->setOffsets(WLength("40%"),Side::Left);
        back_Container->decorationStyle().setCursor(Cursor::PointingHand);


        back_Container->clicked().connect([=]()mutable-> Node{
            if( item->prev ){

                item = item->prev;
                auto file_Name = this->downloadFileWeb(item->item->getFileOid().c_str());

                img->setAttributeValue(Style::style,Style::background::url(file_Name.c_str())
                                                +Style::background::size::contain
                                                +Style::background::repeat::norepeat
                                       +Style::background::position::center_center+"height:"+std::to_string(item->item->getHeight())+"px;");
                img->setMargin(WLength::Auto,Side::Left|Side::Right);
                img->setWidth(WLength("100%"));
                img->setMaximumSize(1024,WLength::Auto);
                text->setText(item->item->getAciklama());
                if( img->height() == 0 ){
                    img->setHeight(item->item->getHeight());
                }

                indexText->setText(std::to_string(item->index+1)+"/"+std::to_string(item->total));


            }else{
                this->showPopUpMessage("Son Resimdeniz");
            }

        });


        auto next_Container = img->addNew<WContainerWidget>();
        auto nextText = next_Container->addNew<WText>("➡",TextFormat::UnsafeXHTML);
        nextText->setPadding(5,AllSides);
        next_Container->setAttributeValue(Style::style,Style::background::color::rgba(175,175,75,.75)+Style::color::color(Style::color::White::AliceBlue+Style::font::size::s36px+Style::font::weight::bold));
        next_Container->setPositionScheme(PositionScheme::Absolute);
        next_Container->setOffsets(0,Side::Top);
        next_Container->setOffsets(WLength("60%"),Side::Left);
        next_Container->decorationStyle().setCursor(Cursor::PointingHand);


        next_Container->clicked().connect([=]()mutable-> Node{
            if( item->next ){

                item = item->next;
                auto file_Name = this->downloadFileWeb(item->item->getFileOid().c_str());

                img->setAttributeValue(Style::style,Style::background::url(file_Name.c_str())
                                                +Style::background::size::contain
                                                +Style::background::repeat::norepeat
                                       +Style::background::position::center_center+"height:"+std::to_string(item->item->getHeight())+"px;");
                img->setMargin(WLength::Auto,Side::Left|Side::Right);
                img->setWidth(WLength("100%"));
                img->setMaximumSize(1024,WLength::Auto);
                text->setText(item->item->getAciklama());
                if( img->height() == 0 ){
                    img->setHeight(item->item->getHeight());
                }

                indexText->setText(std::to_string(item->index+1)+"/"+std::to_string(item->total));


            }else{
                this->showPopUpMessage("Son Resimdeniz");
            }

        });


        auto close_Container = img->addNew<WContainerWidget>();
        auto closeText = close_Container->addNew<WText>("✘",TextFormat::UnsafeXHTML);
        close_Container->setPadding(5,AllSides);
        close_Container->setAttributeValue(Style::style,Style::background::color::rgba(175,75,75,.75)+Style::color::color(Style::color::White::AliceBlue+Style::font::size::s24px+Style::font::weight::bold));
        close_Container->setPositionScheme(PositionScheme::Absolute);
        close_Container->setOffsets(0,Side::Top);
        close_Container->setOffsets(WLength("70%"),Side::Left);
        close_Container->decorationStyle().setCursor(Cursor::PointingHand);


        close_Container->clicked().connect([=](){

//            auto _imgContainer = wApp->root()->addNew<WContainerWidget>();

            wApp->root()->removeWidget(_imgContainer);

        });


//        auto _mNextContainer = _imgContainer->addNew<WContainerWidget>();
//        _mNextContainer->addStyleClass(CSSStyle::Button::blueButton);
//        _mNextContainer->setPositionScheme(PositionScheme::Absolute);
//        _mNextContainer->setOffsets(80,Side::Top);
//        _mNextContainer->setOffsets(136,Side::Right);
//        _mNextContainer->setWidth(50);
//        _mNextContainer->setHeight(50);
//        _mNextContainer->addNew<WText>("▶");
//        _mNextContainer->setAttributeValue(Style::style,Style::font::size::s36px+Style::font::weight::bold+Style::color::rgb("255,255,255"));
//        _mNextContainer->clicked().connect([=](){
//            if( item->next ){
//                this->showItem(item->next);
//            }else{
//                this->showPopUpMessage("Son Resimdeniz");
//            }
//        });

//        auto _mBackContainer = _imgContainer->addNew<WContainerWidget>();
//        _mBackContainer->addStyleClass(CSSStyle::Button::blueButton);
//        _mBackContainer->setPositionScheme(PositionScheme::Absolute);
//        _mBackContainer->setOffsets(80,Side::Top);
//        _mBackContainer->setOffsets(-50,Side::Left);
//        _mBackContainer->setWidth(50);
//        _mBackContainer->setHeight(50);
//        _mBackContainer->addNew<WText>("◀");
//        _mBackContainer->setAttributeValue(Style::style,Style::font::size::s36px+Style::font::weight::bold+Style::color::rgb("255,255,255"));
//        _mBackContainer->clicked().connect([=](){
//            if( item->prev ){
//                this->showItem(item->prev);
//            }else{
//                this->showPopUpMessage("İlk Resimdeniz");
//            }
//        });


    });

    auto mNextContainer = imgContainer->addNew<WContainerWidget>();
    mNextContainer->addStyleClass(CSSStyle::Button::blueButton);
    mNextContainer->setPositionScheme(PositionScheme::Absolute);
    mNextContainer->setOffsets(80,Side::Top);
    mNextContainer->setOffsets(-50,Side::Right);
    mNextContainer->setWidth(50);
    mNextContainer->setHeight(50);
    mNextContainer->addNew<WText>("▶");
    mNextContainer->setAttributeValue(Style::style,Style::font::size::s36px+Style::font::weight::bold+Style::color::rgb("255,255,255"));
    mNextContainer->clicked().connect([=](){
        if( item->next ){
            this->showItem(item->next);
        }else{
            this->showPopUpMessage("Son Resimdeniz");
        }
    });

    auto mBackContainer = imgContainer->addNew<WContainerWidget>();
    mBackContainer->addStyleClass(CSSStyle::Button::blueButton);
    mBackContainer->setPositionScheme(PositionScheme::Absolute);
    mBackContainer->setOffsets(80,Side::Top);
    mBackContainer->setOffsets(-50,Side::Left);
    mBackContainer->setWidth(50);
    mBackContainer->setHeight(50);
    mBackContainer->addNew<WText>("◀");
    mBackContainer->setAttributeValue(Style::style,Style::font::size::s36px+Style::font::weight::bold+Style::color::rgb("255,255,255"));
    mBackContainer->clicked().connect([=](){
        if( item->prev ){
            this->showItem(item->prev);
        }else{
            this->showPopUpMessage("İlk Resimdeniz");
        }
    });

    auto textContainer = mContentContainer->addNew<WContainerWidget>();
    textContainer->addNew<WText>(item->item->getAciklama(),TextFormat::UnsafeXHTML);
    textContainer->setWidth(WLength("100%"));
    textContainer->setAttributeValue(Style::style,Style::background::color::rgba(25,25,25,.75)+Style::color::color(Style::color::White::AliceBlue)+Style::font::size::s14px);


    auto indexContainer = imgContainer->addNew<WContainerWidget>();
    indexContainer->addNew<WText>(std::to_string(item->index+1)+"/"+std::to_string(item->total),TextFormat::UnsafeXHTML);
    indexContainer->setAttributeValue(Style::style,Style::background::color::rgba(175,175,75,.75)+Style::color::color(Style::color::White::AliceBlue+Style::font::size::s20px+Style::font::weight::bold));
    indexContainer->setPositionScheme(PositionScheme::Absolute);
    indexContainer->setOffsets(0,Side::Top);
    indexContainer->setOffsets(WLength("50%"),Side::Left);





    auto controllerContainer = mContentContainer->addNew<WContainerWidget>();
    controllerContainer->setHeight(250);

}

void v2::NostSerik::showItem()
{

    if( image_index < 0 ) image_index = mFList.count()-1;

    image_index = image_index%mFList.count();

    auto item = mFList[image_index];


    mContentContainer->clear();

    auto fileName = this->downloadFileWeb(item.getFileOid().c_str());



    auto imgContainer = mContentContainer->addNew<WContainerWidget>();
    imgContainer->setMaximumSize(750,WLength::Auto);

    imgContainer->addStyleClass("NostListItem");
    imgContainer->setMargin(45,Side::Top);
    imgContainer->setWidth(WLength("100%"));
    imgContainer->setHeight(static_cast<double>(item.getHeight())*0.73);
    imgContainer->setPositionScheme(PositionScheme::Relative);
    imgContainer->setAttributeValue(Style::style,Style::background::url(fileName.c_str())
                                    +Style::background::size::contain
                                    +Style::background::repeat::norepeat);
    imgContainer->decorationStyle().setCursor(Cursor::PointingHand);


    imgContainer->clicked().connect(this,&NostSerik::showItemFull);

    auto mNextContainer = imgContainer->addNew<WContainerWidget>();
    mNextContainer->addStyleClass(CSSStyle::Button::blueButton);
    mNextContainer->setPositionScheme(PositionScheme::Absolute);
    mNextContainer->setOffsets(80,Side::Top);
    mNextContainer->setOffsets(-50,Side::Right);
    mNextContainer->setWidth(50);
    mNextContainer->setHeight(50);
    mNextContainer->addNew<WText>("▶");
    mNextContainer->setAttributeValue(Style::style,Style::font::size::s36px+Style::font::weight::bold+Style::color::rgb("255,255,255"));
    mNextContainer->clicked().connect([=](){
        image_index++;
        this->showItem();
    });

    auto mBackContainer = imgContainer->addNew<WContainerWidget>();
    mBackContainer->addStyleClass(CSSStyle::Button::blueButton);
    mBackContainer->setPositionScheme(PositionScheme::Absolute);
    mBackContainer->setOffsets(80,Side::Top);
    mBackContainer->setOffsets(-50,Side::Left);
    mBackContainer->setWidth(50);
    mBackContainer->setHeight(50);
    mBackContainer->addNew<WText>("◀");
    mBackContainer->setAttributeValue(Style::style,Style::font::size::s36px+Style::font::weight::bold+Style::color::rgb("255,255,255"));
    mBackContainer->clicked().connect([=](){
        image_index--;
        this->showItem();
    });

    auto textContainer = mContentContainer->addNew<WContainerWidget>();
    textContainer->addNew<WText>(item.getAciklama(),TextFormat::UnsafeXHTML);
    textContainer->setWidth(WLength("100%"));
    textContainer->addStyleClass("nostTextShadow");


    auto indexContainer = imgContainer->addNew<WContainerWidget>();
    indexContainer->addNew<WText>(std::to_string(image_index+1)+"/"+std::to_string(mFList.size()),TextFormat::UnsafeXHTML);
    indexContainer->setAttributeValue(Style::style,Style::background::color::rgba(175,175,75,.75)+Style::color::color(Style::color::White::AliceBlue+Style::font::size::s20px+Style::font::weight::bold));
    indexContainer->setPositionScheme(PositionScheme::Absolute);
    indexContainer->setOffsets(-30,Side::Top);
    indexContainer->setOffsets(WLength("45%"),Side::Left);





    auto controllerContainer = mContentContainer->addNew<WContainerWidget>();
    controllerContainer->setHeight(250);


}

void v2::NostSerik::showItemFull()
{
    auto item = mFList[image_index];
    auto fileName = this->downloadFileWeb(item.getFileOid().c_str());

    if( !mImgFullContainer ){
        mImgFullContainer = wApp->root()->addNew<WContainerWidget>();
        mImgFullContainer->setAttributeValue(Style::style,Style::background::color::rgba(255,255,255,0.99));
    }else{
        mImgFullContainer->clear();
    }



    auto text_Container = mImgFullContainer->addNew<WContainerWidget>();
    auto text = text_Container->addNew<WText>(item.getAciklama(),TextFormat::UnsafeXHTML);
    text_Container->setWidth(WLength("100%"));
    text_Container->setMaximumSize(1024,WLength::Auto);
    text_Container->setAttributeValue(Style::style,Style::color::color(Style::color::Grey::Black)+Style::font::size::s14px);
    text_Container->addStyleClass("nostTextShadow");
    text_Container->setMargin(60,Side::Top);


    mImgFullContainer->setPositionScheme(PositionScheme::Fixed);
    mImgFullContainer->setWidth(WLength("100%"));
    mImgFullContainer->setHeight(WLength("100%"));
    mImgFullContainer->setZIndex(12);
    mImgFullContainer->setOffsets(0,AllSides);
    mImgFullContainer->setContentAlignment(AlignmentFlag::Center);
    mImgFullContainer->setOverflow(Overflow::Auto);

    auto img = mImgFullContainer->addNew<WContainerWidget>();
    img->setAttributeValue(Style::style,Style::background::url(fileName.c_str())
                                    +Style::background::size::contain
                                    +Style::background::repeat::norepeat);
    img->setWidth(WLength("100%"));
    img->setMaximumSize(1024,WLength::Auto);
    img->setHeight(item.getHeight());
    img->addStyleClass("nostShadow");




    auto index_Container = img->addNew<WContainerWidget>();
    auto indexText = index_Container->addNew<WText>(std::to_string(image_index+1)+"/"+std::to_string(mFList.size()),TextFormat::UnsafeXHTML);
    index_Container->setAttributeValue(Style::style,Style::background::color::rgba(175,175,75,.75)+Style::color::color(Style::color::White::AliceBlue+Style::font::size::s20px+Style::font::weight::bold));
    index_Container->setPositionScheme(PositionScheme::Absolute);
    index_Container->setOffsets(0,Side::Top);
    index_Container->setOffsets(WLength("50%"),Side::Left);






    auto back_Container = img->addNew<WContainerWidget>();
    auto backText = back_Container->addNew<WText>("⬅",TextFormat::UnsafeXHTML);
    backText->setPadding(5,AllSides);
    back_Container->setAttributeValue(Style::style,Style::background::color::rgba(175,175,75,.75)+Style::color::color(Style::color::White::AliceBlue+Style::font::size::s36px+Style::font::weight::bold));
    back_Container->setPositionScheme(PositionScheme::Absolute);
    back_Container->setOffsets(0,Side::Top);
    back_Container->setOffsets(WLength("40%"),Side::Left);
    back_Container->decorationStyle().setCursor(Cursor::PointingHand);


    back_Container->clicked().connect([=](){
        image_index--;
        image_index = image_index%mFList.count();
        if( image_index < 0 ) image_index = mFList.count()-1;

        this->showItemFull();
    });


    auto next_Container = img->addNew<WContainerWidget>();
    auto nextText = next_Container->addNew<WText>("➡",TextFormat::UnsafeXHTML);
    nextText->setPadding(5,AllSides);
    next_Container->setAttributeValue(Style::style,Style::background::color::rgba(175,175,75,.75)+Style::color::color(Style::color::White::AliceBlue+Style::font::size::s36px+Style::font::weight::bold));
    next_Container->setPositionScheme(PositionScheme::Absolute);
    next_Container->setOffsets(0,Side::Top);
    next_Container->setOffsets(WLength("60%"),Side::Left);
    next_Container->decorationStyle().setCursor(Cursor::PointingHand);


    next_Container->clicked().connect([=](){
        image_index++;
        image_index = image_index%mFList.count();
        this->showItemFull();
    });


    auto close_Container = img->addNew<WContainerWidget>();
    auto closeText = close_Container->addNew<WText>("✘",TextFormat::UnsafeXHTML);
    close_Container->setPadding(5,AllSides);
    close_Container->setAttributeValue(Style::style,Style::background::color::rgba(175,75,75,.75)+Style::color::color(Style::color::White::AliceBlue+Style::font::size::s24px+Style::font::weight::bold));
    close_Container->setPositionScheme(PositionScheme::Absolute);
    close_Container->setOffsets(0,Side::Top);
    close_Container->setOffsets(WLength("70%"),Side::Left);
    close_Container->decorationStyle().setCursor(Cursor::PointingHand);


    close_Container->clicked().connect([=](){
        wApp->root()->removeWidget(mImgFullContainer);
        mImgFullContainer = nullptr;
        this->showItem();
    });
}









