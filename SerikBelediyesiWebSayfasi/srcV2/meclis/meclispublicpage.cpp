#include "meclispublicpage.h"

MeclisPublicPage::MeclisPublicPage(DB *db)
    :ContainerWidget("Meclis"),SerikBLDCore::Meclis::MeclisManager (db)
{
    this->setMargin (90,Side::Top);
}
