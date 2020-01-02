#ifndef YENIPERSONELWIDGET_H
#define YENIPERSONELWIDGET_H

#include "personel.h"
#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"


class YeniPersonelWidget : public ContainerWidget , public SerikBLDCore::IK::Personel
{
public:
    YeniPersonelWidget();


    Signal<NoClass> &PersonelSaved();
    QString photoFilePath() const;

private:

    void errorOccured(const std::string &errorText) override;

    QString mPhotoFilePath;
    WCheckBox* ofisPersonel;
    WLineEdit* telefonLineEdit,*adsoyadLineEdit;

    void savePersonel();


    Signal<NoClass> _personelSaved;
};

#endif // YENIPERSONELWIDGET_H
