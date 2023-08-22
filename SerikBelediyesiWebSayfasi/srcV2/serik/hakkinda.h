#ifndef V2_SERIK_HAKKINDA_H
#define V2_SERIK_HAKKINDA_H

#include "SerikBelediyesiWebSayfasi/BaseClass/page.h"



namespace v2 {
namespace Serik {


namespace Key{
inline const std::string Collection{"hakkinda"};
inline const std::string pageName{"itemName"};
inline const std::string kategoriName{"kategoriName"};
}







class Hakkinda : public Comman::Page
{
public:
    Hakkinda();
};

} // namespace Serik
} // namespace v2

#endif // V2_SERIK_HAKKINDA_H
