
QT += gui core xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20 console
CONFIG -= app_bundle
#TEMPLATE += app

CONFIG += no_keywords

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    SerikBelediyesiWebSayfasi/BaseClass/containerwiget.cpp \
    SerikBelediyesiWebSayfasi/BaseClass/dbclass.cpp \
    SerikBelediyesiWebSayfasi/BaseClass/dialog.cpp \
    SerikBelediyesiWebSayfasi/BaseClass/itembase.cpp \
    SerikBelediyesiWebSayfasi/baseItem/tcitem.cpp \
    SerikBelediyesiWebSayfasi/baseWidget/tcitemwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/basvrulistitemwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/basvuruitemviewwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/basvurularim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/bilgiedinmebasvuruwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/birimIsleri/birimislericontainer.cpp \
    SerikBelediyesiWebSayfasi/srcV2/calismalar/calismacontainerwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/calismalar/calismamanagercontainer.cpp \
    SerikBelediyesiWebSayfasi/srcV2/device/deviceitem.cpp \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekcelistwidgetitem.cpp \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekcetcyonetim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekceview.cpp \
    SerikBelediyesiWebSayfasi/srcV2/dilekceyonetim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/externalWidget/haritawidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/externalWidget/nobetcieczanewidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/faaliyet/faaliyetraporcontainer.cpp \
    SerikBelediyesiWebSayfasi/srcV2/firma/firmamanagerpage.cpp \
    SerikBelediyesiWebSayfasi/srcV2/girisCikisWidget/giriscikiswidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/imar/mainprojeview.cpp \
    SerikBelediyesiWebSayfasi/srcV2/imar/mimariprojemanagerpage.cpp \
    SerikBelediyesiWebSayfasi/srcV2/imar/yenimimariproje.cpp \
    SerikBelediyesiWebSayfasi/srcV2/kadinailestock.cpp \
    SerikBelediyesiWebSayfasi/srcV2/meclis/meclisitempage.cpp \
    SerikBelediyesiWebSayfasi/srcV2/meclis/meclispage.cpp \
    SerikBelediyesiWebSayfasi/srcV2/meclis/meclispublicpage.cpp \
    SerikBelediyesiWebSayfasi/srcV2/meclis/meclisuyesipage.cpp \
    SerikBelediyesiWebSayfasi/srcV2/meclis/meclisuyesipublicwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/personel/baskanyardimcilariwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/personel/personelmanagerpage.cpp \
    SerikBelediyesiWebSayfasi/srcV2/personel/yenipersonelwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/personel/yonetimwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeclient.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeitem.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeyonetim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetimyonetim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetitem.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetitemwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetlistitemwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetyonetimwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sms/smsmanager.cpp \
    SerikBelediyesiWebSayfasi/srcV2/stok/stokcontainerwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/stok/stokkategoripage.cpp \
    SerikBelediyesiWebSayfasi/srcV2/talepler/taleplistwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepvatandasarayuz.cpp \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepview.cpp \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepyonetim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/vatandas/vatandaswidget.cpp \
    main.cpp \
    mainapplication.cpp \
    SerikBelediyesiWebSayfasi/src/header.cpp \
    SerikBelediyesiWebSayfasi/src/body.cpp \
    SerikBelediyesiWebSayfasi/src/footer.cpp \
    SerikBelediyesiWebSayfasi/src/giriswidget.cpp \
    SerikBelediyesiWebSayfasi/chart/donutchart.cpp \
    SerikBelediyesiWebSayfasi/BaseClass/basewidget.cpp \
    SerikBelediyesiWebSayfasi/Giris/gelenmesajlarwidget.cpp \
    SerikBelediyesiWebSayfasi/src/yenilikler.cpp \
    SerikBelediyesiWebSayfasi/src/anonswidget.cpp \
    SerikBelediyesiWebSayfasi/BaseClass/databasewidget.cpp \
    SerikBelediyesiWebSayfasi/src/basindabizwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/mainpage.cpp \
    SerikBelediyesiWebSayfasi/srcV2/headerpage.cpp \
    SerikBelediyesiWebSayfasi/srcV2/slider.cpp \
    SerikBelediyesiWebSayfasi/srcV2/mainpagecontroller.cpp \
    SerikBelediyesiWebSayfasi/srcV2/baskanwidget.cpp



QMAKE_CXXFLAGS += /wd4251 /wd4275 /wd4267 /wd4189 /wd4100 /wd4101 /wd4715 /wd4189
QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter


INCLUDEPATH += $$PWD/../../Comman
DEPENDPATH += $$PWD/../../Comman

HEADERS += \
    ../../Comman/CSSCustom.h \
    SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h \
    SerikBelediyesiWebSayfasi/BaseClass/dbclass.h \
    SerikBelediyesiWebSayfasi/BaseClass/dialog.h \
    SerikBelediyesiWebSayfasi/BaseClass/itembase.h \
    SerikBelediyesiWebSayfasi/BaseClass/wtheaders.h \
    SerikBelediyesiWebSayfasi/baseItem/tcitem.h \
    SerikBelediyesiWebSayfasi/baseWidget/tcitemwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/basvrulistitemwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/basvuruitemviewwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/basvurularim.h \
    SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/bilgiedinmebasvuruwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/birimIsleri/birimislericontainer.h \
    SerikBelediyesiWebSayfasi/srcV2/calismalar/calismacontainerwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/calismalar/calismamanagercontainer.h \
    SerikBelediyesiWebSayfasi/srcV2/device/deviceitem.h \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekcelistwidgetitem.h \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekcetcyonetim.h \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekceview.h \
    SerikBelediyesiWebSayfasi/srcV2/dilekceyonetim.h \
    SerikBelediyesiWebSayfasi/srcV2/externalWidget/haritawidget.h \
    SerikBelediyesiWebSayfasi/srcV2/externalWidget/nobetcieczanewidget.h \
    SerikBelediyesiWebSayfasi/srcV2/faaliyet/faaliyetraporcontainer.h \
    SerikBelediyesiWebSayfasi/srcV2/firma/firmamanagerpage.h \
    SerikBelediyesiWebSayfasi/srcV2/girisCikisWidget/giriscikiswidget.h \
    SerikBelediyesiWebSayfasi/srcV2/imar/mainprojeview.h \
    SerikBelediyesiWebSayfasi/srcV2/imar/mimariprojemanagerpage.h \
    SerikBelediyesiWebSayfasi/srcV2/imar/yenimimariproje.h \
    SerikBelediyesiWebSayfasi/srcV2/kadinailestock.h \
    SerikBelediyesiWebSayfasi/srcV2/meclis/meclisitempage.h \
    SerikBelediyesiWebSayfasi/srcV2/meclis/meclispage.h \
    SerikBelediyesiWebSayfasi/srcV2/meclis/meclispublicpage.h \
    SerikBelediyesiWebSayfasi/srcV2/meclis/meclisuyesipage.h \
    SerikBelediyesiWebSayfasi/srcV2/meclis/meclisuyesipublicwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/personel/baskanyardimcilariwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/personel/personelmanagerpage.h \
    SerikBelediyesiWebSayfasi/srcV2/personel/yenipersonelwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/personel/yonetimwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeclient.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeitem.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeyonetim.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetimyonetim.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetitem.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetitemwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetlistitemwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetyonetimwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/sms/smsmanager.h \
    SerikBelediyesiWebSayfasi/srcV2/stok/stokcontainerwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/stok/stokkategoripage.h \
    SerikBelediyesiWebSayfasi/srcV2/talepler/taleplistwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepvatandasarayuz.h \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepview.h \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepyonetim.h \
    SerikBelediyesiWebSayfasi/srcV2/vatandas/vatandaswidget.h \
    mainapplication.h \
    Necessary \
    SerikBelediyesiWebSayfasi/src/header.h \
    ../../Comman/bootstrap.h \
    ../../Comman/inlinestyle.h \
    ../../Comman/inlinestyle.h \
    SerikBelediyesiWebSayfasi/src/body.h \
    SerikBelediyesiWebSayfasi/src/footer.h \
    ../../Comman/serikbeltr.h \
    SerikBelediyesiWebSayfasi/src/giriswidget.h \
    SerikBelediyesiWebSayfasi/chart/donutchart.h \
    SerikBelediyesiWebSayfasi/BaseClass/basewidget.h \
    SerikBelediyesiWebSayfasi/Giris/gelenmesajlarwidget.h \
    ../url.h \
    SerikBelediyesiWebSayfasi/src/yenilikler.h \
    SerikBelediyesiWebSayfasi/src/anonswidget.h \
    SerikBelediyesiWebSayfasi/BaseClass/databasewidget.h \
    SerikBelediyesiWebSayfasi/src/basindabizwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/mainpage.h \
    SerikBelediyesiWebSayfasi/srcV2/headerpage.h \
    SerikBelediyesiWebSayfasi/srcV2/slider.h \
    SerikBelediyesiWebSayfasi/srcV2/mainpagecontroller.h \
    SerikBelediyesiWebSayfasi/srcV2/baskanwidget.h

DISTFILES +=


    MSVC_VER = $$(VisualStudioVersion)

    message("MSVC_VER : " + $$MSVC_VER)




    equals(MSVC_VER, 17.0){
        message("msvc 2022 x64")

        INCLUDEPATH += C:/Mongo/msvc2022x64/include/bsoncxx/v_noabi
        DEPENDPATH += C:/Mongo/msvc2022x64/include/bsoncxx/v_noabi

        win32: LIBS += -LC:/Mongo/msvc2022x64/lib/ -lmongocxx
        win32: LIBS += -LC:/Mongo/msvc2022x64/lib/ -lbsoncxx

        INCLUDEPATH += C:/Mongo/msvc2022x64/include/mongocxx/v_noabi
        DEPENDPATH += C:/Mongo/msvc2022x64/include/mongocxx/v_noabi



        win32: LIBS += -LC:/Wt_4.7.0_msvs2022_x64/lib/ -lwt

        INCLUDEPATH += C:/Wt_4.7.0_msvs2022_x64/include
        DEPENDPATH += C:/Wt_4.7.0_msvs2022_x64/include

        win32: LIBS += -LC:/Wt_4.7.0_msvs2022_x64/lib/ -lwthttp

        win32: LIBS += -LC:/Wt_4.7.0_msvs2022_x64/lib/ -llibhpdf


        win32: LIBS += -LC:/SerikBLDCoreRelease/MSVC2022x64/lib/ -lSerikBLDCore

        INCLUDEPATH += C:/SerikBLDCoreRelease/MSVC2022x64/include
        DEPENDPATH += C:/SerikBLDCoreRelease/MSVC2022x64/include


        INCLUDEPATH += C:/SerikBLDCoreRelease/MSVC2022x64/include/bilgiEdinme
        DEPENDPATH += C:/SerikBLDCoreRelease/MSVC2022x64/include/bilgiEdinme


    }








INCLUDEPATH += C:/boost/boost
DEPENDPATH += C:/boost/boost





