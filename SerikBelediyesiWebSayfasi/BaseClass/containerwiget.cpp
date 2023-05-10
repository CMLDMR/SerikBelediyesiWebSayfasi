#include "containerwiget.h"

#include <QFile>
#include <QFileInfo>
#include <QUuid>
#include <ostream>
#include <iomanip>


ContainerWidget::ContainerWidget(const std::string &title, ContainerWidget::ContentType _contentType)
    :mTitle(title),initController(false),mContainerStyle(_contentType)
{
    this->initWidget();
}

WContainerWidget *ContainerWidget::Header()
{
    return mHeaderContainer;
}

WContainerWidget *ContainerWidget::Content()
{
    return mContentContainer;
}

WContainerWidget *ContainerWidget::Footer()
{
    return mFootContainer;
}

void ContainerWidget::setTitleBarBackColor(const std::string &color)
{
    mTitleBar->setAttributeValue (Style::style,Style::background::color::color (color));
}

void ContainerWidget::initWidget()
{
    addStyleClass(Bootstrap::Grid::container_fluid);

    if( mTitle.size() )
    {
        mTitleBar = addWidget(cpp14::make_unique<WContainerWidget>());
        mTitleBar->addWidget(cpp14::make_unique<WText>("<h5>"+mTitle+"</h5>",TextFormat::XHTML));
        mTitleBar->setContentAlignment(AlignmentFlag::Center);
        mTitleBar->setMargin (15,Side::Bottom);
        mTitleBar->setMargin (-15,Side::Left|Side::Right);
        mTitleBar->addStyleClass(Bootstrap::Grid::row);
        mTitleBar->addStyleClass (Bootstrap::ContextualBackGround::bg_primary+"boxShadow" );
    }

    if( mContainerStyle == Horizontal )
    {
        auto __container = addWidget (cpp14::make_unique<WContainerWidget>());
        __container->addStyleClass (Bootstrap::Grid::row);

        auto _headerContainer = __container->addWidget (cpp14::make_unique<WContainerWidget>());
        _headerContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                                         Bootstrap::Grid::Medium::col_md_2+
                                         Bootstrap::Grid::Small::col_sm_3+
                                         Bootstrap::Grid::ExtraSmall::col_xs_12);

        mHeaderContainer = _headerContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderContainer->addStyleClass(Bootstrap::Grid::row);

        auto _contentContainer = __container->addWidget (cpp14::make_unique<WContainerWidget>());
        _contentContainer->addStyleClass (Bootstrap::Grid::Large::col_lg_10+
                                          Bootstrap::Grid::Medium::col_md_10+
                                          Bootstrap::Grid::Small::col_sm_9+
                                          Bootstrap::Grid::ExtraSmall::col_xs_12);


        mContentContainer = _contentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mContentContainer->addStyleClass(Bootstrap::Grid::row);





    }else{

        mHeaderContainer = addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderContainer->addStyleClass(Bootstrap::Grid::row);

        mContentContainer = addWidget(cpp14::make_unique<WContainerWidget>());
        mContentContainer->addStyleClass(Bootstrap::Grid::row);
    }

    mFootContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mFootContainer->addStyleClass(Bootstrap::Grid::row);

}


WPushButton* ContainerWidget::askConfirm(const std::string &question)
{
    auto mDialog = wApp->instance()->root()->addChild (cpp14::make_unique<WDialog>());

    mDialog->titleBar ()->addWidget (cpp14::make_unique<WText>("?"));
    mDialog->titleBar ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);

    mDialog->contents ()->addWidget (cpp14::make_unique<WText>(question));
    mDialog->contents ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);

    auto yesBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Evet"));
    yesBtn->addStyleClass (Bootstrap::Button::Primary);

    auto closeBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Hayır"));
    closeBtn->addStyleClass (Bootstrap::Button::Danger);

    closeBtn->clicked ().connect ([=](){
        wApp->instance()->root()->removeChild(mDialog);
    });

    yesBtn->clicked ().connect ([=](){
        wApp->instance()->root()->removeChild(mDialog);
    });

    mDialog->show ();
    return yesBtn;
}

void ContainerWidget::warnDialog(const std::string &warnMessage)
{

    auto mDialog = wApp->instance()->root()->addChild (cpp14::make_unique<WDialog>());


    mDialog->titleBar ()->addWidget (cpp14::make_unique<WText>("<h3><b>! Uyarı</b></h3>"));
    mDialog->titleBar ()->addStyleClass (Bootstrap::ContextualBackGround::bg_warning);


    mDialog->contents ()->addWidget (cpp14::make_unique<WText>(warnMessage));
    mDialog->contents ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);

    auto closeBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Tamam"));
    closeBtn->addStyleClass (Bootstrap::Button::Warning);

    closeBtn->clicked ().connect ([=](){
        wApp->instance()->root()->removeChild(mDialog);
    });
    mDialog->show ();
}

void ContainerWidget::criticDialog(const std::string &criticMessage)
{
    auto mDialog = wApp->instance()->root()->addChild (cpp14::make_unique<WDialog>());


    mDialog->titleBar ()->addWidget (cpp14::make_unique<WText>("<h3><b>!Hata</b></h3>"));
    mDialog->titleBar ()->addStyleClass (Bootstrap::ContextualBackGround::bg_danger);


    mDialog->contents ()->addWidget (cpp14::make_unique<WText>(criticMessage));
    mDialog->contents ()->addStyleClass (Bootstrap::ContextualBackGround::bg_warning);

    auto closeBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Tamam"));
    closeBtn->addStyleClass (Bootstrap::Button::Danger);

    closeBtn->clicked ().connect ([=](){
        wApp->instance()->root()->removeChild(mDialog);
    });
    mDialog->show ();
}

void ContainerWidget::informDialog(const std::string &informMessage)
{
    auto mDialog = wApp->instance()->root()->addChild (cpp14::make_unique<WDialog>());


    mDialog->titleBar ()->addWidget (cpp14::make_unique<WText>("<h5><b>Bilgi</b></h5>"));
    mDialog->titleBar ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);


    mDialog->contents ()->addWidget (cpp14::make_unique<WText>(informMessage));
    mDialog->contents ()->addStyleClass (Bootstrap::ContextualBackGround::bg_info);

    auto closeBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Tamam"));
    closeBtn->addStyleClass (Bootstrap::Button::Primary);

    closeBtn->clicked ().connect ([=](){
        wApp->instance()->root()->removeChild(mDialog);
    });
    mDialog->show ();
}



std::unique_ptr<WContainerWidget> ContainerWidget::createButton(const std::string &menuName, const Cursor cursor)
{
    auto container = cpp14::make_unique<WContainerWidget>();
    container->addStyleClass (Bootstrap::Grid::Large::col_lg_2+
                              Bootstrap::Grid::Medium::col_md_2+
                              Bootstrap::Grid::Small::col_sm_3+
                              Bootstrap::Grid::ExtraSmall::col_xs_3+
                              Bootstrap::ImageShape::img_thumbnail);
    container->setAttributeValue (Style::style,Style::background::color::rgb (this->getRandom (),
                                                                              this->getRandom (),
                                                                              this->getRandom ()));
    auto btnText = container->addWidget (cpp14::make_unique<WText>("<b>"+menuName+"</b>"));
    btnText->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
    container->decorationStyle ().setCursor (cursor);
    container->addStyleClass (CSSStyle::Button::blueButton);
    container->setHeight (40);
    return container;
}

WDialog *ContainerWidget::createDialog(const std::string &title)
{
    auto mDialog = wApp->instance()->root()->addChild (cpp14::make_unique<WDialog>());

    if( title.size () )
    {
        mDialog->titleBar ()->addWidget (cpp14::make_unique<WText>(title));
        mDialog->titleBar ()->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
    }


    auto closeBtn = mDialog->footer ()->addWidget (cpp14::make_unique<WPushButton>("Kapat"));
    closeBtn->addStyleClass (Bootstrap::Button::Danger);

    closeBtn->clicked ().connect ([=](){
        wApp->instance()->root()->removeChild(mDialog);
    });

    mDialog->show ();
    return mDialog;
}

void ContainerWidget::removeDialog(WDialog *mDialog)
{
    wApp->instance()->root()->removeChild(mDialog);
}

DialogContainerWidget* ContainerWidget::createFlatDialog(const std::string &title, const bool &autoDel)
{
    auto container = Wt::WApplication::instance()->root()->addNew<DialogContainerWidget>(title);
    container->setId("flatDialog");
    container->setPositionScheme(PositionScheme::Fixed);
    container->setWidth(WLength("100%"));
    container->setHeight(WLength("100%"));
    container->addStyleClass(CSSStyle::NewDialog::newDialog);
    container->setContentAlignment(AlignmentFlag::Center);

    if( autoDel ){
        container->Rejected().connect([=](){
            Wt::WApplication::instance()->root()->removeWidget(container);
        });
    }



    return container;
}

void ContainerWidget::removeDialog(DialogContainerWidget *removedContainer)
{
    WApplication::instance()->root()->removeWidget(removedContainer);
}

void ContainerWidget::setContainerStyle(ContainerWidget::ContainerStyleType type)
{

    this->removeStyleClass(Bootstrap::Grid::container_fluid);

    switch (type) {

    case ROW:
        this->addStyleClass(Bootstrap::Grid::row);
        break;

    case CONTAINERFLUID:
        this->addStyleClass(Bootstrap::Grid::container_fluid);
        break;

    default:
        break;
    }
}


void ContainerWidget::showMessage(std::string title, std::string msg, std::string btnText)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, StandardButton::Ok));
    if( btnText != "Tamam" )
    {
        auto btn = messageBox->button(StandardButton::Ok);
        btn->setText(btnText);
    }



    messageBox->buttonClicked().connect([=] {
        this->removeChild(messageBox);
    });

    messageBox->webWidget()->setZIndex(1000);
    messageBox->show();
}



void ContainerWidget::showPopUpMessage(const std::string &msg, const std::string &infoType)
{

    auto container = wApp->root ()->addWidget (cpp14::make_unique<WContainerWidget>());
    container->setPositionScheme (PositionScheme::Fixed);
    container->addStyleClass ("boxShadow boxRadius");
    container->setZIndex (10000);
    if( infoType == "info" )
    {
        container->setOffsets (WLength("100%"),Side::Top);
        container->setOffsets (WLength(0),Side::Right);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::ForestGreen)+
                                      Style::color::color (Style::color::White::Snow)+
                                      Style::font::weight::lighter+Style::font::size::s16px);

        container->addWidget (cpp14::make_unique<WText>("  "+msg+"  ",TextFormat::UnsafeXHTML))->setPadding (10,Side::Left|Side::Right);
        container->addStyleClass ("popMessage");

    }else if ( infoType == "msg" ) {
        container->setOffsets (WLength("100%"),Side::Top);
        container->setOffsets (WLength("35%"),Side::Right);
        container->setPadding (25);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Green::DarkCyan)+
                                      Style::color::color (Style::color::White::Snow)+
                                      Style::font::weight::bold);

        container->addWidget (cpp14::make_unique<WText>(msg,TextFormat::UnsafeXHTML));
        container->addStyleClass ("popMessageError");
    }else{
        container->setOffsets (WLength("100%"),Side::Top);
        container->setOffsets (WLength("35%"),Side::Right);
        container->setPadding (25);
        container->setAttributeValue (Style::style,Style::background::color::color (Style::color::Red::FireBrick)+
                                      Style::color::color (Style::color::White::Snow)+
                                      Style::font::weight::bold);

        container->addWidget (cpp14::make_unique<WText>(msg,TextFormat::UnsafeXHTML));
        container->addStyleClass ("popMessageError");
    }



    WTimer::singleShot(std::chrono::milliseconds(5000),[=](){
        wApp->root ()->removeWidget(container);
    });


}

void ContainerWidget::removeTitleBar()
{
    if( mTitle.size () )
    {
        removeWidget(mTitleBar);
    }
}

WContainerWidget *ContainerWidget::titleBar() const
{
    return mTitleBar;
}

std::string ContainerWidget::doubleToString(const double &value, const int &precision)
{
    std::ostringstream ss;
    ss << std::fixed;
    ss << std::setprecision(precision);
    ss << value;
    return ss.str();
}





FileUploaderWidget::FileUploaderWidget(const std::string &title)
//    :DBClass (_db)
{

    mIsUploaded = false;

    auto fileUploaderContainer = this->addWidget(cpp14::make_unique<WContainerWidget>());
    fileUploaderContainer->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(150,170),this->getRandom(170,190),this->getRandom(200,220)));
    fileUploaderContainer->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
    fileUploaderContainer->setMargin(10,Side::Top|Side::Bottom);
    //    fileUploaderContainer->setWidth (WLength("100%"));

    mFileUploader = fileUploaderContainer->addWidget(cpp14::make_unique<WFileUpload>());
    auto progresBar = fileUploaderContainer->addWidget(cpp14::make_unique<WProgressBar>());
    mFileUploader->setFileTextSize(6000); // Set the maximum file size to 2048 kB.
    mFileUploader->setMargin(10, Wt::Side::Right);
    mFileUploader->setFilters("application/pdf");
    mFileUploader->setMultiple(false);
    mFileUploader->setProgressBar(progresBar);

    mFileUploader->fileTooLarge().connect([=] {
        this->showMessage("Hata","Dosya Boyutu Fazla Büyük. Max: 6MB Olmalı");
    });

    Wt::WPushButton *uploadButton = fileUploaderContainer->addWidget(Wt::cpp14::make_unique<Wt::WPushButton>(title));
    uploadButton->setMargin(10, Wt::Side::Left | Wt::Side::Right);

    uploadButton->clicked().connect([=] {
        mFileUploader->progressBar()->setValue(0);
        mFileUploader->upload();
        mFileUploader->enable();
        mFileUploader->setHidden(false);
    });

    mFileUploader->uploaded().connect([=] {

        auto list = mFileUploader->uploadedFiles();

        for( const auto &item : list )
        {
            QFileInfo info(item.clientFileName().c_str());

            QString _fileName = QString("%1%2.%3").arg(QUuid::createUuid().toString(QUuid::StringFormat::WithoutBraces)).arg(item.clientFileName().c_str()).arg(info.suffix());

            mFileLocation = QString("docroot/tempfile/")+_fileName;
            mDoocRootLocation = QString("tempfile/")+ _fileName;
            if( QFile::copy(item.spoolFileName().c_str(),mFileLocation) )
            {
                mIsUploaded = true;
                _Uploaded.emit(NoClass());
            }else{
                mIsUploaded = false;
            }
        }

    });
    setType (Image);
}

bool FileUploaderWidget::isUploaded() const
{
    return mIsUploaded;
}

void FileUploaderWidget::setType(FileUploaderWidget::FilterType type)
{
    mType = type;

    switch (mType) {
    case Pdf:
        mFileUploader->setFilters ("application/pdf");
        break;
    case Image:
        mFileUploader->setFilters ("image/*");
        break;
    case DWG:
        mFileUploader->setFilters ("application/acad");
        break;
    default:
        break;
    }
}

QString FileUploaderWidget::fileLocation() const
{
    return mFileLocation;
}

QString FileUploaderWidget::doocRootLocation() const
{
    return mDoocRootLocation;
}

Signal<NoClass> &FileUploaderWidget::Uploaded()
{
    return _Uploaded;
}




DialogContainerWidget::DialogContainerWidget(const std::string &title)
{

    contentContainer = this->addNew<WContainerWidget>();
    contentContainer->setMaximumSize(1024,WLength::Auto);
    contentContainer->setPositionScheme(PositionScheme::Relative);
    contentContainer->setHeight(WLength("100%"));
    contentContainer->setWidth(WLength("100%"));


    auto vLayout = contentContainer->setLayout(cpp14::make_unique<WVBoxLayout>());
    vLayout->addStretch(1);

    auto mTitleContainer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    mTitleContainer->addNew<WText>(title)->setMargin(0,Side::Bottom);
    mTitleContainer->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);

    mContent = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    mContent->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::AliceBlue));
    mContent->setMaximumSize(WLength::Auto,WLength::Auto);
    mContent->addStyleClass(Bootstrap::Grid::row);
    mContent->setMargin(0,Side::Top);
    mContent->setPadding(15,Side::Top|Side::Bottom);
    mContent->setContentAlignment(AlignmentFlag::Center);


    auto footer = vLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    footer->setMaximumSize(WLength::Auto,50);
    footer->setAttributeValue(Style::style,Style::background::color::rgba(255,255,255,0.75));

    auto hLayout = footer->setLayout(cpp14::make_unique<WHBoxLayout>());

    auto acceptContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    acceptContainer->addStyleClass(Bootstrap::Button::Primary);
    acceptContainer->setPadding(5,Side::Top|Side::Bottom);
    acceptContainer->addNew<WText>("<b>Kaydet</b>",TextFormat::UnsafeXHTML);
    acceptContainer->decorationStyle().setCursor(Cursor::PointingHand);

    acceptContainer->clicked().connect([=](){
        _Accepted.emit(NoClass());
    });

    auto closeContainer = hLayout->addWidget(cpp14::make_unique<WContainerWidget>());
    closeContainer->addStyleClass(Bootstrap::Button::Default);
    closeContainer->setPadding(5,Side::Top|Side::Bottom);
    closeContainer->addNew<WText>("<b>Kapat</b>",TextFormat::UnsafeXHTML);
    closeContainer->decorationStyle().setCursor(Cursor::PointingHand);
    closeContainer->clicked().connect([=](){
        _Rejected.emit(NoClass());
    });
    vLayout->addStretch(1);
}

WContainerWidget *DialogContainerWidget::Content()
{
    return mContent;
}

void DialogContainerWidget::setContentWidth(const WLength &value)
{
    contentContainer->setWidth(value);
}

Signal<NoClass> &DialogContainerWidget::Accepted()
{
    return _Accepted;
}

Signal<NoClass> &DialogContainerWidget::Rejected()
{
    return _Rejected;
}

ControllerWidget::ControllerWidget()
{
    mBackContainer = this->addNew<WContainerWidget>();
    mBackContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
    mBackContainer->addStyleClass(Bootstrap::Button::Primary);
    mBackContainer->addNew<WText>("Geri");
    mBackContainer->clicked().connect([=](){ _backClicked.emit(NoClass());});
    mBackContainer->setPadding(5,Side::Top|Side::Bottom);
    mBackContainer->decorationStyle().setCursor(Cursor::PointingHand);

    mInfoContainer = this->addNew<WContainerWidget>();
    mInfoContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_8+
                                  Bootstrap::Grid::Medium::col_md_8+
                                  Bootstrap::Grid::Small::col_sm_8+
                                  Bootstrap::Grid::ExtraSmall::col_xs_8);
    mInfoContainer->addStyleClass(Bootstrap::Label::info);
    mText = mInfoContainer->addNew<WText>("Info");
    mInfoContainer->setPadding(5,Side::Top|Side::Bottom);

    mNextContainer = this->addNew<WContainerWidget>();
    mNextContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                  Bootstrap::Grid::Medium::col_md_2+
                                  Bootstrap::Grid::Small::col_sm_2+
                                  Bootstrap::Grid::ExtraSmall::col_xs_2);
    mNextContainer->addStyleClass(Bootstrap::Button::Primary);
    mNextContainer->addNew<WText>("İleri");
        mNextContainer->clicked().connect([=](){ _nextClicked.emit(NoClass());});
    mNextContainer->setPadding(5,Side::Top|Side::Bottom);
    mNextContainer->decorationStyle().setCursor(Cursor::PointingHand);
}
