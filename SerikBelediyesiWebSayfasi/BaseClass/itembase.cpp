#include "itembase.h"

ItemBase::ItemBase(mongocxx::database *_db, const std::string &collection)
    : DBClass (_db),
      collectionName(collection),
      mIsValid(true),
    doc(document{})
{
    try {
        auto ins = this->db()->collection(collectionName).insert_one(doc.view());
        if( ins )
        {
            this->mOid = ins.value().inserted_id().get_oid().value;
        }else{
            mIsValid = false;
        }
    } catch (mongocxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
        mIsValid = false;
    }
}

ItemBase::ItemBase(mongocxx::database *_db, const std::string &collection, bsoncxx::document::view &_view)
    : DBClass (_db),
      collectionName(collection),
      mIsValid(true)
{
    this->doc = document{};
    for( auto it = _view.cbegin() ; it != _view.cend() ; it++ )
    {

        try {
            this->doc.append(kvp(it->key(),it->get_value()));
        } catch (bsoncxx::exception &e) {
            std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
            mIsValid = false;
        }

    }

    if( mIsValid )
    {
        try {
            mOid = this->doc.view()["_id"].get_oid().value;
        } catch (bsoncxx::exception &e) {
            std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
            mIsValid = false;
        }
    }


}

//ItemBase::~ItemBase()
//{

//    std::cout << "Destructor ItemBase" << std::endl;
//}

boost::optional<bsoncxx::builder::basic::document> ItemBase::filter()
{
    auto filter_ = document{};
    try {
        filter_.append(kvp("_id",mOid));
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
        return boost::none;
    }
    return std::move(filter_);
}

QStringList ItemBase::keyList()
{
    QStringList list;
    for (auto it = this->doc.view().cbegin() ; it != this->doc.view().cend() ; it++ ) {
        list.push_back(it->key().to_string().c_str());
    }
    return list;
}

bsoncxx::document::view ItemBase::view()
{
    return this->doc.view();
}

bool ItemBase::deleteItem()
{
    auto _filter = this->filter();

    if( _filter )
    {
        try {
            auto del = this->db()->collection(collectionName).delete_one(_filter.value().view());
            if( del )
            {
                if( del.value().deleted_count() )
                {
                    return true;
                }
            }
        } catch (mongocxx::exception &e) {
            std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " " << e.what() << std::endl;
        }
    }

    return false;

}

bsoncxx::oid ItemBase::oid() const
{
    return mOid;
}

bool ItemBase::isValid() const
{
    return mIsValid;
}


boost::optional<bsoncxx::types::value> ItemBase::Element(const std::string &key)
{

    try {
        return this->view()[key].get_value();
    } catch (bsoncxx::exception &e) {
        std::cout << "ERROR: " << __LINE__ << " " << __FUNCTION__ << " "<<key << ": " << e.what() << std::endl;
        return boost::none;
    }

}
