#include "UpdateDialog.h"

#include <Wt/WText.h>
#include <Wt/WGridLayout.h>
#include <Wt/WLineEdit.h>
#include <Wt/WDateEdit.h>
#include <Wt/WDate.h>

using namespace Wt;


namespace v2 {
namespace Karavan {

UpdateDialog::UpdateDialog(const KaravanItem &item)
    :m_karavanItem(std::make_unique<KaravanItem>())
{
    m_karavanItem->setDocumentView(item.view());
    this->initKaravanItemWidget();
}

UpdateDialog::~UpdateDialog()
{

}

void UpdateDialog::initKaravanItemWidget()
{
    setWindowTitle("Bilgiler");


    auto gLayout = contents()->setLayout(std::make_unique<WGridLayout>());

    int row{0};

    auto tcnoLabel = gLayout->addWidget(std::make_unique<WText>("Plaka NO"),row,0,Wt::AlignmentFlag::Left);
    tcnoLabel->setInline(false);
    tcnoLabel->setMinimumSize(150,WLength::Auto);
    tcnoLabel->setPadding(6,Side::Top|Side::Bottom);
    auto tcnoLineEdit = gLayout->addWidget(std::make_unique<WLineEdit>(),row++,1,Wt::AlignmentFlag::Justify);
    tcnoLineEdit->setText(m_karavanItem->getTcNO());
    tcnoLineEdit->setEnabled(false);


    auto isimSoyisimLabel = gLayout->addWidget(std::make_unique<WText>("AdSoyad"),row,0,Wt::AlignmentFlag::Left);
    isimSoyisimLabel->setInline(false);
    isimSoyisimLabel->setMinimumSize(150,WLength::Auto);
    isimSoyisimLabel->setPadding(6,Side::Top|Side::Bottom);
    auto isimSoyisimLineEdit = gLayout->addWidget(std::make_unique<WLineEdit>(),row++,1,Wt::AlignmentFlag::Justify);
    isimSoyisimLineEdit->setText(m_karavanItem->getAdSoyad());
    isimSoyisimLineEdit->setEnabled(false);


    auto plakaLabel = gLayout->addWidget(std::make_unique<WText>("Plaka NO"),row,0,Wt::AlignmentFlag::Left);
    plakaLabel->setInline(false);
    plakaLabel->setMinimumSize(150,WLength::Auto);
    plakaLabel->setPadding(6,Side::Top|Side::Bottom);
    auto plakaLineEdit = gLayout->addWidget(std::make_unique<WLineEdit>(),row++,1,Wt::AlignmentFlag::Justify);
    plakaLineEdit->setText(m_karavanItem->getPlakaNo());
    plakaLineEdit->setPlaceholderText("Araç Plaka No");


    auto telefonLabel = gLayout->addWidget(std::make_unique<WText>("Telefon"),row,0,Wt::AlignmentFlag::Left);
    telefonLabel->setInline(false);
    telefonLabel->setMinimumSize(150,WLength::Auto);
    telefonLabel->setPadding(6,Side::Top|Side::Bottom);
    auto telefonLineEdit = gLayout->addWidget(std::make_unique<WLineEdit>(),row++,1,Wt::AlignmentFlag::Justify);
    telefonLineEdit->setText(m_karavanItem->getTelefon());
    telefonLineEdit->setPlaceholderText("Telefon NO");


    auto baslangicLabel = gLayout->addWidget(std::make_unique<WText>("Başlangıç"),row,0,Wt::AlignmentFlag::Left);
    baslangicLabel->setInline(false);
    baslangicLabel->setMinimumSize(150,WLength::Auto);
    baslangicLabel->setPadding(6,Side::Top|Side::Bottom);
    auto baslangicDateEdit = gLayout->addWidget(std::make_unique<WDateEdit>(),row++,1,Wt::AlignmentFlag::Justify);
    baslangicDateEdit->setDate(WDate::fromJulianDay(m_karavanItem->getBaslangic()));
    baslangicDateEdit->setFormat("dd/MM/yyyy");

    auto bitisLabel = gLayout->addWidget(std::make_unique<WText>("Başlangıç"),row,0,Wt::AlignmentFlag::Left);
    bitisLabel->setInline(false);
    bitisLabel->setMinimumSize(150,WLength::Auto);
    bitisLabel->setPadding(6,Side::Top|Side::Bottom);
    auto bitisDateEdit = gLayout->addWidget(std::make_unique<WDateEdit>(),row++,1,Wt::AlignmentFlag::Justify);
    bitisDateEdit->setDate(WDate::fromJulianDay(m_karavanItem->getBitis()));
    bitisDateEdit->setFormat("dd/MM/yyyy");



}

} // namespace Karavan
} // namespace v2
