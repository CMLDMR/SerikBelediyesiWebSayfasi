#ifndef MIMARPROJEVIEW_H
#define MIMARPROJEVIEW_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "imar/mimariproje.h"
#include "imar/mimarilog.h"
#include "user.h"

namespace Comman{

class MimarProjeView : public SerikBLDCore::Imar::MimariProje::MimariProje, public ContainerWidget
{
public:
    explicit MimarProjeView(const SerikBLDCore::Imar::MimariProje::MimariProje& projeItem , const bool& firmaGirisi = false );

    void initInformation();




    void enableFileUploading();

    void initMimariProject();




    Signal<bsoncxx::oid> &RequestMimariProje();


    Signal<bsoncxx::oid,std::string> &insertDuzeltmeClicked();
    Signal<bsoncxx::oid,std::string> &insertAciklama();
    Signal<bsoncxx::oid,std::string> &insertDosya();
    Signal<bsoncxx::oid,int> &RequestLog();
    Signal<bsoncxx::oid,bool> &MimariProjeOnayla();



private:
    Signal<bsoncxx::oid> _requestMimariProje;


    Signal<bsoncxx::oid,std::string> _insertDuzeltme;
    Signal<bsoncxx::oid,std::string> _insertAciklama;
    Signal<bsoncxx::oid,std::string> _insertDosya;
    Signal<bsoncxx::oid,int> _requestLog;
    Signal<bsoncxx::oid,bool> _mimariProjeOnayla;


private:


    bool mFirmaGirisi;
    WComboBox* mLogFilterComboBox = nullptr;





    WContainerWidget* mStatikProjeContainer;
    WText* mStatikOnayText = nullptr;
    WContainerWidget* mStatikOnayTextContainer = nullptr;
};

}


#endif // MIMARPROJEVIEW_H
