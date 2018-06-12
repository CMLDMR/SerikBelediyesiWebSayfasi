#include "basewidget.h"


BaseClass::BaseWidget::BaseWidget(mongocxx::database *_db, std::string tel)
    :WContainerWidget(),
      __db(_db),
      _User(document{}.view())
{
    auto filter = document{};

    try {
        filter.append(kvp(SBLDKeys::Personel::telefon,tel.c_str()));
    } catch (bsoncxx::exception &e) {
        this->showMessage("Error","Hata Oluştu",e);
        return;
    }

    try {
        auto val = __db->collection(SBLDKeys::Personel::collection).find_one(filter.view());
        if( val )
        {
            if( val.value().view().empty() )
            {
                this->showMessage("Error","Dosya Boş");
                return;
            }else{
                this->setUser(val.value());
            }
        }else{
            this->showMessage("Error","DB cevap vermiyor");
            return;
        }
    } catch (mongocxx::exception &e) {
        this->showMessage("Error","Hata Oluştu",e);
        return;
    }
}

BaseClass::BaseWidget::BaseWidget(mongocxx::database *_db, bsoncxx::document::value _user)
    :WContainerWidget(),
      __db(_db),
      _User(_user)
{

}

void BaseClass::BaseWidget::showMessage(std::string title, std::string msg, std::string btnText)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 msg,
                 Wt::Icon::Information, StandardButton::Ok));
    if( btnText != "OK" )
    {
        auto btn = messageBox->button(StandardButton::Ok);
        btn->setText(btnText);
    }


        messageBox->buttonClicked().connect([=] {
            this->removeChild(messageBox);
        });

        messageBox->show();
}

void BaseClass::BaseWidget::showMessage(std::string title, std::string msg, bsoncxx::exception &e, Icon icon)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 WString("{1} : {2}").arg(msg.c_str()).arg(e.what()).toUTF8(),
                 icon, Wt::StandardButton::Ok));

        messageBox->buttonClicked().connect([=] {
            this->removeChild(messageBox);
        });

        messageBox->show();
}

void BaseClass::BaseWidget::showMessage(std::string title, std::string msg, mongocxx::exception &e, Icon icon)
{
    auto messageBox = this->addChild(
                Wt::cpp14::make_unique<Wt::WMessageBox>
                (title,
                 WString("{1} : {2}").arg(msg.c_str()).arg(e.what()).toUTF8(),
                 icon, Wt::StandardButton::Ok));

        messageBox->buttonClicked().connect([=] {
            this->removeChild(messageBox);
        });

        messageBox->show();
}

mongocxx::database *BaseClass::BaseWidget::db() const
{
    return this->__db;
}

bsoncxx::document::value BaseClass::BaseWidget::User() const
{
    return _User;
}

std::unique_ptr<WPushButton> BaseClass::BaseWidget::createColorButton(const char *className, const WString &text)
{
    auto button = Wt::cpp14::make_unique<Wt::WPushButton>();
    button->setTextFormat(Wt::TextFormat::XHTML);
    button->setText(text);
    button->addStyleClass(className);
    return button;
}

int64_t BaseClass::BaseWidget::count(std::string collection, bsoncxx::document::view &filter)
{
    std::int64_t count = 0;
    try {
        count = this->db()->collection(collection).count(filter);
    } catch (mongocxx::exception &e) {

    }
    return count;
}

int64_t BaseClass::BaseWidget::count(std::string collection, bsoncxx::document::value val)
{
    std::int64_t count = 0;
    try {
        count = this->db()->collection(collection).count(val.view());
    } catch (mongocxx::exception &e) {

    }
    return count;
}

void BaseClass::BaseWidget::setUser(const bsoncxx::document::value &User)
{
    _User = User;
}
BaseClass::ContainerWidget::ContainerWidget(mongocxx::database *_db, bsoncxx::document::value _user, std::string ContainerTitle)
    :BaseWidget::BaseWidget(_db,_user)
{
    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);

    if( ContainerTitle != "NOCONTAINER" ){
        auto _container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        _container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
        auto _layout = _container->setLayout(cpp14::make_unique<WHBoxLayout>());
        {
            auto container = _layout->addWidget(cpp14::make_unique<WContainerWidget>());
            container->addStyleClass(Bootstrap::Grid::Large::col_lg_12);
            auto text = container->addWidget(cpp14::make_unique<WText>(ContainerTitle));
            text->setAttributeValue(Style::style,Style::font::size::s18px+Style::font::weight::bold);
        }
        _container->setAttributeValue(Style::style,Style::Border::border("1px solid gray")+
                                     Style::background::color::color(Style::color::Grey::Gainsboro));

        _container->clicked().connect([=](){
            _mClickReinit.emit(NoClass());
        });
        _container->decorationStyle().setCursor(Cursor::PointingHand);
        _container->setToolTip("Sayfayı Yenile");
    }


    // Header Container
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto fContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        mHeaderRowContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderRowContainer->addStyleClass(Bootstrap::Grid::row);
    }

    // Content Container
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto fContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        mContentRowContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mContentRowContainer->addStyleClass(Bootstrap::Grid::row);
    }

    // Footer Container
    {
        auto container = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        container->addStyleClass(Bootstrap::Grid::Large::col_lg_12+Bootstrap::Grid::Medium::col_md_12+Bootstrap::Grid::Small::col_sm_12+Bootstrap::Grid::ExtraSmall::col_xs_12);

        auto fContainer = container->addWidget(cpp14::make_unique<WContainerWidget>());
        fContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        mFooterRowContainer = fContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mFooterRowContainer->addStyleClass(Bootstrap::Grid::row);
    }


}

std::string BaseClass::ContainerWidget::getBackGroundImg() const
{
    std::string imgPath = "img/"+std::to_string(this->getRandom(1,RandomImageCount))+".jpg";
    return imgPath;
}
