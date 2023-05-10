#include "malzemelistitem.h"

namespace TodoList {

std::string MalzemeListItem::getMalzemeAdi() const
{
    auto val = this->element(Key::AKIS::MALZEMELIST::malzemeAdi);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";

}

double MalzemeListItem::getMiktar() const
{
    auto val = this->element(Key::AKIS::MALZEMELIST::miktari);
    if( val ){
        return val.value().view().get_double().value;
    }
    return 0;
}

std::string MalzemeListItem::getMetric() const
{
    auto val = this->element(Key::AKIS::MALZEMELIST::metric);
    if( val ){
        return val.value().view().get_string().value.data();
    }
    return "";
}

} // namespace TodoList
