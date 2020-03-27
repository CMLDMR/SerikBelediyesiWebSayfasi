#include "taleplistwidget.h"

#include "Talep/talep.h"

TalepListWidget::TalepListWidget(const std::string &oid,
                                 const std::string &mahalle,
                                 const std::string &tarih,
                                 const std::string &durum,
                                 const std::string &durumColor,
                                 const std::string &kategoriName)
    :mOid(oid),
      mMahalle(mahalle),
      mTarih(tarih),
      mDurum(durum),
      mDurumColor(durumColor),
      mKategoriName(kategoriName)
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


    {
        auto _container = addWidget (cpp14::make_unique<WContainerWidget>());
        _container->setWidth (WLength(std::to_string (100.0)+"%"));


        if( mKategoriName == "Covid-19" )
        {
            auto text = _container->addWidget (cpp14::make_unique<WText>(mKategoriName));
            text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            if( mDurum == SerikBLDCore::TalepKey::DurumKey::DevamEdiyor )
            {
                _container->addStyleClass ("blink ");
                _container->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::background::color::color (Style::color::Red::Crimson));
            }else{
                _container->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow)+Style::background::color::color (Style::color::Green::SeaGreen));
            }
            _container->addStyleClass ("textShadow");
        }else{
            auto text = _container->addWidget (cpp14::make_unique<WText>("."));
            text->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            _container->setAttributeValue (Style::style,Style::color::color (Style::color::White::Snow));
            _container->addStyleClass ("textShadow");
        }

    }



}

Signal<std::string> &TalepListWidget::ClickItem()
{
    return _clickItem;
}
