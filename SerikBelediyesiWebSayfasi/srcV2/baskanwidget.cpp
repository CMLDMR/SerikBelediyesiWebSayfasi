#include "baskanwidget.h"

BaskanWidget::BaskanWidget(mongocxx::database *_db)
    :SerikBLDCore::DB(_db)
{
    this->clear();

    setPadding(90,Side::Top);
    setWidth(WLength("100%"));

    auto mContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = mContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);
    rContainer->setMaximumSize(1024,WLength::Auto);


    //FOTO
    {
        auto fotoWidget = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        fotoWidget->addStyleClass(Bootstrap::Grid::col_full_12);
        fotoWidget->setHeight(350);
        fotoWidget->setAttributeValue(Style::style,Style::background::url("v2/baskan/baskan1.png")
                                      +Style::background::size::contain
                                      +Style::background::repeat::norepeat
                                      +Style::background::position::center_center);




    }


    // TEXT
    {
        auto textWidget = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        textWidget->addStyleClass(Bootstrap::Grid::col_full_12);
        textWidget->setAttributeValue(Style::style,Style::background::color::color(Style::color::White::Snow));

        auto filter = document{};

        try {
            filter.append(kvp("type","Başkan"));
        } catch (bsoncxx::exception &e) {

        }

        try {
            filter.append(kvp("baskan","Enver APUTKAN"));
        } catch (bsoncxx::exception &e) {

        }


        try {
            auto val = this->db()->collection("Yonetim").find_one(filter.view());

            if( !val.value().view().empty() )
            {
                auto str = val.value().view()["html"].get_string().value.data();
                auto text = textWidget->addWidget(cpp14::make_unique<WText>(str,TextFormat::UnsafeXHTML));
            }

        } catch (mongocxx::exception &e) {

        }


    }


}
