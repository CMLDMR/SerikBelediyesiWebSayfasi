
#include "resimitem.h"

namespace TodoList {

ResimItem::ResimItem()
    :BaseItem::BaseItem(BaseItem::Type::RESIM)
{

}
ResimItem &ResimItem::setResimOid(const std::string &resimOid)
{
    this->append(Key::AKIS::resim,bsoncxx::oid{resimOid});
    return *this;
}

std::string ResimItem::getResimOid() const
{
    auto val = element(Key::AKIS::resim);
    if( val ){
        return val.value().view().get_oid().value.to_string();
    }
    return "";
}

} // namespace TodoList



void TodoList::ResimItem::errorOccured(const std::string &errorText)
{
}

void TodoList::ResimItem::initWidget()
{
}
