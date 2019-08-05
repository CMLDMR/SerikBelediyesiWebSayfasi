#include "bilgiedinmeyonetim.h"



BilgiEdinmeYonetim::BilgiEdinmeYonetim(mongocxx::database *_db, const bsoncxx::document::value &_userValue)
    :DBClass (_db),UserClass (_userValue)
{
    setContainerStyle(ContainerStyleType::ROW);
}
