#include "kvkkcontainer.h"

#include "link.h"

namespace v2 {
namespace KVKK {

KVKKContainer::KVKKContainer()
    :Comman::Page("KVKK")
{


    content()->setPadding(25);
    auto titleText = content()->addNew<WText>("<h3>Kişisel Verileri Koruma Kanunu Politikası</h3>");
    titleText->addStyleClass(Bootstrap::Grid::col_full_12);

    addLink("Belediye KVKK Politikası","kvkk/Belediye_KVKK_Politikasi.pdf");
    addLink("Belediye Aydınlatma Metni","kvkk/BelediyeAydinlatmaMetni.pdf");
    addLink("Belediye Bilgi Güvenliği Politikası","kvkk/BelediyeBilgiGüvenligiPolitikasi.pdf");
    addLink("Belediye Kişisel Veri Saklama ve İmha Politikası","kvkk/BelediyeKisiselVeriSaklamaveimhaPolitikasi.pdf");
    addLink("Belediye Özel Nitelikli Kişisel Verilerin Korunması ve İşlenmesi Politikası","kvkk/BelediyeOzelNitelikliKisiselVerilerinKorunmasiveislenmesiPolitikasi.pdf");
    addLink("Belediye Sosyal Medya Aydınlatma Metni","kvkk/Belediye_Sosyal_Medya_Aydinlatma_Metni.pdf.pdf");

    addLink("Belediye- Veri İhlali Müdahale Politikası","kvkk/Belediye_KVKK_Politikası.pdf");
    addLink("Belediye Veri ihlali Müdahale Politikası","kvkk/Belediye_Veri_ihlali_Mudahale_Politikasi.pdf");

    addLink("Belediye-Kişisel Veri İhlali Disiplin Politikası","kvkk/Belediye_Kisisel_veri_ihlali_Disiplin_Politikasi.pdf");

    addLink("Bilgi Edinme Başvurusu İletişim Formu Aydınlatma Metni","kvkk/Bilgi_Edinme_Basvurusu_iletisim_Formu_Aydinlatma_Metni.pdf");

    addLink("Çalışan Adayı Aydınlatma Metni","kvkk/Calisan_Adayi_Aydinlatma_Metni.pdf.pdf");
    addLink("Çalışan Aydınlatma Metni","kvkk/calisan_Aydinlatma_Metni.pdf");
    addLink("Çerez Politikası","kvkk/Cerez_Politikasi.pdf");

    addLink("Kamera Aydınlatma Metni","kvkk/Kamera_Aydinlatma_Metni.pdf");
    addLink("KVKK Başvuru Formu","kvkk/KVKK_Basvuru_Formu.pdf");
    addLink("Talep ve Şikayet Formu Aydınlatma Metni","kvkk/Talep_ve_sikayet_Formu_Aydinlatma_Metni.pdf.pdf");

}

void KVKKContainer::addLink(const std::string &title, const std::string &url)
{
            auto linkContainer = content()->addNew<Link>("<b>"+title+"</b>",url);
            linkContainer->addStyleClass(Bootstrap::Grid::Large::col_lg_6);
            linkContainer->setAttributeValue(Style::style,Style::Border::bottom::border("1px solid lightgray"));
}

} // namespace KVKK
} // namespace v2
