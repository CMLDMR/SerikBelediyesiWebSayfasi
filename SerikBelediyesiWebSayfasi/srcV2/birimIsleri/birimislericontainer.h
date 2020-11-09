#ifndef BIRIMISLERICONTAINER_H
#define BIRIMISLERICONTAINER_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "user.h"

namespace v2{

class BirimIsleriContainer : public ContainerWidget
{
public:
    BirimIsleriContainer(SerikBLDCore::User* _user);

    void initFirmaYonetimi();



    // Birim Menüleri
public:
    void initImar();

    void initCalismalar();

private:
    SerikBLDCore::User* mUser;

    std::unique_ptr<WContainerWidget> createMenu(const std::string& menuName , Cursor cursor = Cursor::Arrow );
};

}



#endif // BIRIMISLERICONTAINER_H
