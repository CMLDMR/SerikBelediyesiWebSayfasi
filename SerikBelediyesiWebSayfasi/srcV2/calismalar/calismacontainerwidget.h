#ifndef CALISMACONTAINERWIDGET_H
#define CALISMACONTAINERWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "calisma/calismaitem.h"


namespace v2{

class CalismaContainerWidget : public ContainerWidget , public SerikBLDCore::Calisma
{
public:
    explicit CalismaContainerWidget(const std::vector<std::string> &imgLinkList = std::vector<std::string>() );


    void initCalismaWidget();

    void errorOccured(const std::string &errorText) override;

private:
    std::vector<std::string> ImgLinkList;
};


}



#endif // CALISMACONTAINERWIDGET_H
