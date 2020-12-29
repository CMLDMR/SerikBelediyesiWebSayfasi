#ifndef CALISMACONTAINERWIDGET_H
#define CALISMACONTAINERWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "calisma/calismaitem.h"


namespace v2{

class CalismaContainerWidget : public ContainerWidget , public SerikBLDCore::Calisma::Calisma
{
public:
    explicit CalismaContainerWidget(const std::vector<std::string> &imgLinkList = std::vector<std::string>() );


    void initCalismaWidget();

    void errorOccured(const std::string &errorText) override;

    Signal<std::string,std::vector<std::string>> &deleteClicked();

private:
    std::vector<std::string> ImgLinkList;

    Signal<std::string,std::vector<std::string>> _deleteClicked;
};


}



#endif // CALISMACONTAINERWIDGET_H
