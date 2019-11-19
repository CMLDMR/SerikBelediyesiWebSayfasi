#include "talepvatandasarayuz.h"
#include "taleplistwidget.h"
#include "talepview.h"



TalepVatandasArayuz::TalepVatandasArayuz(mongocxx::database *_db, bsoncxx::document::value *_userValue)
    :SerikBLDCore::TalepManager (_db),mTCUser(_userValue)
{
    this->initList ();
}

void TalepVatandasArayuz::initList()
{
    this->Content ()->clear ();
    auto cursor = this->findTalep (SerikBLDCore::Talep().setTCOID (mTCUser.oid ().value ().to_string ().c_str ()));
    for( auto item : cursor )
    {
        this->Content ()->addWidget (cpp14::make_unique<TalepListWidget>(item.oid ().toStdString (),
                                                                          item.mahalle ().toStdString (),
                                                                          item.tarih ().toStdString (),
                                                                          item.durum ().toStdString (),
                                                                         item.durumColor ().toStdString ()))->ClickItem ().connect ([=](std::string oid){
            this->initTalep (oid);
        });
    }
}

void TalepVatandasArayuz::initTalep(const std::string &oid)
{
    SerikBLDCore::Talep filter;
    filter.setOid (oid);

    auto mTalep = this->findOneTalep (filter);

    if( !mTalep )
    {
        this->showMessage ("Uyarı","Bu Talep Yüklenemedi. Lütfen Daha Sonra Tekrar Deneyiniz");
    }else{
        this->Content ()->clear ();

        {
            auto container = this->Content ()->addWidget (cpp14::make_unique<WContainerWidget>());
            container->setContentAlignment (AlignmentFlag::Center);
            container->addWidget (cpp14::make_unique<WText>("Listeye Geri Dön"));
            container->addStyleClass (Bootstrap::ContextualBackGround::bg_primary);
            container->clicked ().connect (this,&TalepVatandasArayuz::initList );
        }

        auto talepView = this->Content ()->addWidget (cpp14::make_unique<TalepView>(std::move(mTalep),
                                                                                    this->db (),
                                                                                    &this->mTCUser));


    }
}
