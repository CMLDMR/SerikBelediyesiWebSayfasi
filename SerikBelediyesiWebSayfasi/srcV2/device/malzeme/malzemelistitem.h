#ifndef TODOLIST_MALZEMELISTITEM_H
#define TODOLIST_MALZEMELISTITEM_H

#include <item.h>

namespace TodoList {
namespace Key {

namespace AKIS {

namespace MALZEMELIST{
inline const std::string malzemeAdi{"malzemeAdi"};
inline const std::string miktari{"miktari"};
inline const std::string metric{"metric"};
}
}
}



class MalzemeListItem : public SerikBLDCore::Item{
public:
    explicit MalzemeListItem(const std::string &malzemeAdi, const double &miktar, const std::string &metric ):SerikBLDCore::Item(""){
        this->append(Key::AKIS::MALZEMELIST::malzemeAdi,malzemeAdi);
        this->append(Key::AKIS::MALZEMELIST::miktari,miktar);
        this->append(Key::AKIS::MALZEMELIST::metric,metric);
    }
    MalzemeListItem( const bsoncxx::document::view &view ): SerikBLDCore::Item(""){
        this->setDocumentView(view);
    }

    MalzemeListItem( const MalzemeListItem &other ):SerikBLDCore::Item(""){this->setDocumentView(other.view());}
    MalzemeListItem( MalzemeListItem &&other ):SerikBLDCore::Item(""){this->setDocumentView(other.view());}


    MalzemeListItem& operator=( const MalzemeListItem &other){ this->setDocumentView(other.view()); return *this;}
    MalzemeListItem& operator=( MalzemeListItem &&other){ this->setDocumentView(other.view()); return *this;}

    std::string getMalzemeAdi() const;
    double getMiktar() const;
    std::string getMetric() const;

private:
    std::string mMalzemeAdi;
    double mMalzemeMiktari;
    std::string mMetric;
};

} // namespace TodoList

#endif // TODOLIST_MALZEMELISTITEM_H
