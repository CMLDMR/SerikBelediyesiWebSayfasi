#include "basindabizwidget.h"



BasindaBizWidget::BasindaBizWidget(mongocxx::database *_db, bool initForBody)
    :DataBaseWidget (_db) , mInitForBody( initForBody )
{

    if( mInitForBody )
    {
        this->initForBody();
    }else{
        this->initForBasin();
    }


    setAttributeValue(Style::style,Style::background::color::rgba(this->getRandom(225),this->getRandom(235),this->getRandom(240)));


}

void BasindaBizWidget::initForBody()
{
    setContentAlignment(AlignmentFlag::Center|AlignmentFlag::Middle);
    setMargin(50,Side::Bottom|Side::Top);

    auto mMainContainer = addWidget(cpp14::make_unique<WContainerWidget>());
    mMainContainer->addStyleClass(Bootstrap::Grid::container_fluid);

    auto rContainer = mMainContainer->addWidget(cpp14::make_unique<WContainerWidget>());
    rContainer->addStyleClass(Bootstrap::Grid::row);


    // Header Arena For Widget
    {
        auto mHeaderContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto mHeaderFluidContainer = mHeaderContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mHeaderFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        mHeaderRowContainer = mHeaderFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        this->headerRowContainer()->addStyleClass(Bootstrap::Grid::row);

        auto container = this->headerRowContainer()->addWidget(cpp14::make_unique<WContainerWidget>());

        auto text = container->addWidget(cpp14::make_unique<WText>(WString::fromUTF8("Basında Biz")));

        text->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);

        text->setPadding(25,Side::Left|Side::Right);
    }




    // MainContainer Areana For Widget
    {
        auto mContentContainer = rContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mContentContainer->addStyleClass(Bootstrap::Grid::col_full_12);

        auto mContentFluidContainer = mContentContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        mContentFluidContainer->addStyleClass(Bootstrap::Grid::container_fluid);

        mRowContainer = mContentFluidContainer->addWidget(cpp14::make_unique<WContainerWidget>());
        this->rowContainer()->addStyleClass(Bootstrap::Grid::row);

        for( int i = 0 ; i < 6 ; i++ )
        {
            this->rowContainer()->addWidget(cpp14::make_unique<BasinItem>(this->getDB()))->addStyleClass(Bootstrap::Grid::Large::col_lg_2
                                                                                                     +Bootstrap::Grid::Medium::col_md_2
                                                                                                     +Bootstrap::Grid::Small::col_sm_4
                                                                                                     +Bootstrap::Grid::ExtraSmall::col_xs_6);
        }


    }





}

void BasindaBizWidget::initForBasin()
{

}

WContainerWidget *BasindaBizWidget::rowContainer() const
{
    return mRowContainer;
}

WContainerWidget *BasindaBizWidget::headerRowContainer() const
{
    return mHeaderRowContainer;
}

BasinItem::BasinItem(mongocxx::database *_db)
    :DataBaseWidget (_db)
{

    setHeight(250);

    addStyleClass(Bootstrap::ImageShape::img_thumbnail);

//    setAttributeValue(Style::style,Style::background::color::rgba( this->getRandom(150,200) , this->getRandom(170,220) , this->getRandom(190,240) ));

    auto vlayout = setLayout(cpp14::make_unique<WVBoxLayout>());

    // Gazete Adı
    {
        auto text = vlayout->addWidget(cpp14::make_unique<WText>(WString::fromUTF8("Gazete Adı")),0,AlignmentFlag::Center);
//        text->addStyleClass(Bootstrap::ContextualBackGround::bg_primary);

    }


    // Resim
    {
        auto imgContainer = vlayout->addWidget(cpp14::make_unique<WContainerWidget>(),0,AlignmentFlag::Justify);
        std::string url_ = "img/"+std::to_string(this->getRandom(0,15))+".jpg";
        imgContainer->setAttributeValue(Style::style,Style::background::url(url_)+Style::background::repeat::norepeat+
                                        Style::background::size::cover);
        imgContainer->addStyleClass(Bootstrap::ImageShape::img_circle);
        imgContainer->setHeight(200);
    }

    // Tarih
    {
        auto text = vlayout->addWidget(cpp14::make_unique<WText>(WString::fromUTF8("08/11/2018")),0,AlignmentFlag::Center);
//        text->addStyleClass(Bootstrap::ContextualBackGround::bg_info);

    }

}
