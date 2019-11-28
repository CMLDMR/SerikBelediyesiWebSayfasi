
QT += gui core xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14 console
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
    SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/bilgiedinmebasvuruwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekcelistwidgetitem.cpp \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekcetcyonetim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekceview.cpp \
    SerikBelediyesiWebSayfasi/srcV2/dilekceyonetim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/externalWidget/haritawidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/externalWidget/nobetcieczanewidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/girisCikisWidget/giriscikiswidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/ik.cpp \
    SerikBelediyesiWebSayfasi/srcV2/kadinailestock.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeclient.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeitem.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeyonetim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetimyonetim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetitem.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetitemwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetlistitemwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetyonetimwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/talepler/taleplistwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepvatandasarayuz.cpp \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepview.cpp \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepwidget.cpp \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepyonetim.cpp \
    SerikBelediyesiWebSayfasi/srcV2/vatandas/vatandaswidget.cpp \
    main.cpp \
    mainapplication.cpp \
    SerikBelediyesiWebSayfasi/src/header.cpp \
    Comman/wjavascriptslider.cpp \
    SerikBelediyesiWebSayfasi/src/body.cpp \
    SerikBelediyesiWebSayfasi/src/footer.cpp \
    SerikBelediyesiWebSayfasi/src/giriswidget.cpp \
    SerikBelediyesiWebSayfasi/chart/donutchart.cpp \
    SerikBelediyesiWebSayfasi/BaseClass/basewidget.cpp \
    SerikBelediyesiWebSayfasi/Giris/gelenmesajlarwidget.cpp \
    SerikBelediyesiWebSayfasi/reporter/reportresource.cpp \
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


INCLUDEPATH += $$PWD/../../Comman
DEPENDPATH += $$PWD/../../Comman

HEADERS += \
    SerikBelediyesiWebSayfasi/BaseClass/containerwiget.h \
    SerikBelediyesiWebSayfasi/BaseClass/dbclass.h \
    SerikBelediyesiWebSayfasi/BaseClass/dialog.h \
    SerikBelediyesiWebSayfasi/BaseClass/itembase.h \
    SerikBelediyesiWebSayfasi/BaseClass/wtheaders.h \
    SerikBelediyesiWebSayfasi/baseItem/tcitem.h \
    SerikBelediyesiWebSayfasi/baseWidget/tcitemwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/bilgiEdinmeBasvuru/bilgiedinmebasvuruwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekcelistwidgetitem.h \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekcetcyonetim.h \
    SerikBelediyesiWebSayfasi/srcV2/dilekce/dilekceview.h \
    SerikBelediyesiWebSayfasi/srcV2/dilekceyonetim.h \
    SerikBelediyesiWebSayfasi/srcV2/externalWidget/haritawidget.h \
    SerikBelediyesiWebSayfasi/srcV2/externalWidget/nobetcieczanewidget.h \
    SerikBelediyesiWebSayfasi/srcV2/girisCikisWidget/giriscikiswidget.h \
    SerikBelediyesiWebSayfasi/srcV2/ik.h \
    SerikBelediyesiWebSayfasi/srcV2/kadinailestock.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeclient.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeitem.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/bilgiedinmeyonetim.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetimyonetim.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetitem.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetitemwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetlistitemwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/sikayetYonetim/sikayetyonetimwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/talepler/taleplistwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepvatandasarayuz.h \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepview.h \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepwidget.h \
    SerikBelediyesiWebSayfasi/srcV2/talepler/talepyonetim.h \
    SerikBelediyesiWebSayfasi/srcV2/vatandas/vatandaswidget.h \
    mainapplication.h \
    Necessary \
    SerikBelediyesiWebSayfasi/src/header.h \
    Comman/wjavascriptslider.h \
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
    SerikBelediyesiWebSayfasi/reporter/reportresource.h \
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

DISTFILES += \
    ../build-SerikBelediyesiWebSayfasi-Desktop_Qt_5_9_1_MSVC2015_64bit2-Release/docroot/css/mainPage.css \
    ../build-SerikBelediyesiWebSayfasi-Desktop_Qt_5_9_1_MSVC2015_64bit2-Release/docroot/css/slider.css \
    ../build-SerikBelediyesiWebSayfasi-Desktop_Qt_5_9_1_MSVC2015_64bit2-Release/docroot/css/eventWidget.css \
    ../build-SerikBelediyesiWebSayfasi-Desktop_Qt_5_9_1_MSVC2015_64bit2-Release/docroot/css/header.css \
    ../build-SerikBelediyesiWebSayfasi-Desktop_Qt_5_9_1_MSVC2015_64bit2-Release/docroot/script/Script.js \
    ../build-SerikBelediyesiWebSayfasi-Desktop_Qt_5_9_1_MSVC2015_64bit2-Release/docroot/script/testscript.js




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lbsoncxx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lbsoncxxd
else:unix: LIBS += -L$$PWD/lib/ -lbsoncxx

INCLUDEPATH += $$PWD/include/bsoncxx/v_noabi
DEPENDPATH += $$PWD/include/bsoncxx/v_noabi



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lmongocxx
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lmongocxxd
else:unix: LIBS += -L$$PWD/lib/ -lmongocxx

INCLUDEPATH += $$PWD/include/mongocxx/v_noabi
DEPENDPATH += $$PWD/include/mongocxx/v_noabi




INCLUDEPATH += C:/boost/boost
DEPENDPATH += C:/boost/boost




win32: LIBS += -LC:/Wt-4.1.2-msvs2015-x64/lib/ -lwt

INCLUDEPATH += C:/Wt-4.1.2-msvs2015-x64/include
DEPENDPATH += C:/Wt-4.1.2-msvs2015-x64/include

win32: LIBS += -LC:/Wt-4.1.2-msvs2015-x64/lib/ -lwthttp

win32: LIBS += -LC:/Wt-4.1.2-msvs2015-x64/lib/ -llibhpdf



win32: LIBS += -LC:/SerikBLDCoreRelease/MSVC2015X64/lib/ -lSerikBLDCore

INCLUDEPATH += C:/SerikBLDCoreRelease/MSVC2015X64/include
DEPENDPATH += C:/SerikBLDCoreRelease/MSVC2015X64/include
