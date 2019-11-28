#include "basvurularim.h"

V2::Basvurularim::Basvurularim(DB *_db, const SerikBLDCore::TC &_tcUser)
    :ContainerWidget ("Başvurularım"),SerikBLDCore::BilgiEdinme::BilgiEdinmeManager (_db)
{




    auto cursor = this->UpdateList (SerikBLDCore::BilgiEdinme::BilgiEdinmeItem().setTCoid (_tcUser.oid ().value ().to_string ().c_str ()));





    for( auto __item : cursor )
    {
        auto item = this->Content ()->addWidget (cpp14::make_unique<BasvruListItemWidget>(&__item));
        item->addStyleClass (Bootstrap::Grid::Large::col_lg_3);
    }



}
