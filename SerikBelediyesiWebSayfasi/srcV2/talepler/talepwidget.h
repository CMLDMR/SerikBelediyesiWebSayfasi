#ifndef TALEPWIDGET_H
#define TALEPWIDGET_H

#include "SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h"
#include "Talep/talepmanager.h"
#include "tcmanager.h"


namespace TalepWidget {
class TCWidget;
class TalepItemWidget;

class TalepWidget : public ContainerWidget , public TalepManager
{
public:
    TalepWidget(DB* db);
    TalepWidget( mongocxx::database* _db );

    void init();

    TCWidget* tcWidget;
    TalepItemWidget* talepWidget;
};




class TCWidget : public ContainerWidget , public TCManager
{
public:
    TCWidget(mongocxx::database* db);

    void TCChanged();
    void setDefault();
    boost::optional<TC> TCItem();

private:

    WLineEdit* mTcNO;
    WLineEdit* mAdSoyad;
    WComboBox* mMahalle;
    WLineEdit* mTelefon;

    std::string mTCOid;

};


class TalepItemWidget : public ContainerWidget , public Talep
{
public:
    TalepItemWidget(QVector<QString> mahalleList);

    std::string mahalleString() const;
    std::string talepString() const;
    std::string adresString() const;

    void setDefault();

private:
    WComboBox* mTalepMahalle;
    WTextEdit* mEdit;
    WLineEdit* mAdres;
};
}


#endif // TALEPWIDGET_H
