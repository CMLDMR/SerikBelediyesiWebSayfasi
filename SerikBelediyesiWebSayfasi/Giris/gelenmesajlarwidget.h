#ifndef GELENMESAJLARWIDGET_H
#define GELENMESAJLARWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/basewidget.h"


namespace PersonelV2 {
    class GelenMesajlarWidget : public BaseClass::ContainerWidget
    {
    public:
        GelenMesajlarWidget(mongocxx::database* _database , bsoncxx::document::value _user);


    private:
        void initHeader();

        void listMesajlar(bsoncxx::document::view filterView);
        void addList(WContainerWidget* rContainer , bsoncxx::document::view filterView);

        const int limit = 20;
        int skip;

        void setDetail(std::string oid);

        class CevapYazWidget : public ContainerWidget
        {
        public:
            CevapYazWidget(mongocxx::database* _database , bsoncxx::document::value _user , std::string mesajoid);

            Signal<NoClass> &SuccessCevap(){ return _SuccessCevap; }

        private:
            std::string mesajOid;

            Signal<NoClass> _SuccessCevap;
        };

    };
}



#endif // GELENMESAJLARWIDGET_H
