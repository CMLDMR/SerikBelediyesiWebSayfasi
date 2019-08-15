#include "sikayetyonetimwidget.h"

SikayetYonetimWidget::SikayetYonetimWidget(mongocxx::database *_db, bsoncxx::document::value &userValue)
    :UserClass (userValue),DBClass(_db)
{


    {
        auto container = this->Header()->addWidget(cpp14::make_unique<ContainerWiget>());
        container->setContainerStyle(ContainerStyleType::ROW);
        container->setContentAlignment(AlignmentFlag::Center);
        container->setWidth(WLength("100%"));
        auto text = container->addWidget(cpp14::make_unique<WText>("<h4>Şikayet Talep Yönetim Paneli</h4>"));
    }

    // Kontrol Paneli
    {
        auto container = this->Header()->addWidget(cpp14::make_unique<ContainerWiget>());
        container->setContainerStyle(ContainerStyleType::ROW);

        mongocxx::pipeline pipline;

        pipline.group(make_document(kvp("_id","$Durum"),kvp("count",make_document(kvp("$sum",1)))));

        auto cursor = this->db()->collection(Sikayet::KEY::collection).aggregate(pipline);

        for (auto&& doc : cursor) {

            auto _container = container->addWidget(cpp14::make_unique<ContainerWiget>());
            _container->addStyleClass(Bootstrap::Grid::Large::col_lg_2+
                                      Bootstrap::Grid::Medium::col_md_2+
                                      Bootstrap::Grid::Small::col_sm_2+
                                      Bootstrap::Grid::ExtraSmall::col_xs_6);
            _container->addStyleClass(Bootstrap::ImageShape::img_thumbnail);
            _container->setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(150,200),this->getRandom(170,220),this->getRandom(180,230)));

            auto vLayout = _container->setLayout(cpp14::make_unique<WVBoxLayout>());


            {
                auto text = vLayout->addWidget(cpp14::make_unique<WText>(),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setText(WString("<h5><b>{1}</b></h5>").arg(doc["_id"].get_utf8().value.to_string()));
                text->setAttributeValue(Style::style,Style::color::color(Style::color::Grey::Black));
            }


            {
                auto text = vLayout->addWidget(cpp14::make_unique<WText>(),0,AlignmentFlag::Center|AlignmentFlag::Middle);
                text->setText(WString("<h4>{1}</h4>").arg(Bootstrap::Badges::badget(std::to_string(doc["count"].get_int32().value))));
            }
        }
    }

}
