#ifndef V2_KARAVAN_UPDATEDIALOG_H
#define V2_KARAVAN_UPDATEDIALOG_H


#include "karavanitem.h"

#include <Wt/WDialog.h>


namespace v2 {
namespace Karavan {

class UpdateDialog : public Wt::WDialog
{
public:
    UpdateDialog(const KaravanItem &item);
    ~UpdateDialog();



private:
    std::unique_ptr<KaravanItem> m_karavanItem;

    void initKaravanItemWidget();

};

} // namespace Karavan
} // namespace v2

#endif // V2_KARAVAN_UPDATEDIALOG_H
