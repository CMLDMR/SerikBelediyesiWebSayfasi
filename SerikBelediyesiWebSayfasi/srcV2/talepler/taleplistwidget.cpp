#include "taleplistwidget.h"

TalepListWidget::TalepListWidget(const std::string &oid,
                                 const std::string &mahalle,
                                 const std::string &tarih,
                                 const std::string &durum,
                                 const std::string &durumColor)
    :mOid(oid),
      mMahalle(mahalle),
      mTarih(tarih),
      mDurum(durum),
      mDurumColor(durumColor)
{

    decorationStyle ().setCursor (Cursor::PointingHand);
    addStyleClass (Bootstrap::ImageShape::img_thumbnail);
    setMargin (5,Side::Top|Side::Bottom);
    addStyleClass (Bootstrap::Grid::Large::col_lg_3
                                      +Bootstrap::Grid::Medium::col_md_4
                                      +Bootstrap::Grid::Small::col_sm_6
                                      +Bootstrap::Grid::ExtraSmall::col_xs_12);

    setAttributeValue (Style::style,Style::background::color::rgba (this->rgbaRandom ()));

    clicked ().connect ([=](){
                _clickItem.emit (mOid);
            });

    {
        auto _container = addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addWidget (cpp14::make_unique<WText>(mTarih));
        _container->setWidth (WLength(std::to_string (100.0)+"%"));
    }
    {
        auto _container = addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addWidget (cpp14::make_unique<WText>(mMahalle));
        _container->setWidth (WLength(std::to_string (100.0)+"%"));
    }

    {
        auto _container = addWidget (cpp14::make_unique<WContainerWidget>());
        _container->addWidget (cpp14::make_unique<WText>(mDurum));
        _container->setWidth (WLength(std::to_string (100.0)+"%"));
        _container->setAttributeValue (Style::style,Style::background::color::color (durumColor+";")+
                                       Style::color::color (Style::color::White::Snow)+
                                       Style::font::weight::bold);
        _container->addStyleClass (Bootstrap::ImageShape::img_thumbnail);

    }

}

Signal<std::string> &TalepListWidget::ClickItem()
{
    return _clickItem;
}
