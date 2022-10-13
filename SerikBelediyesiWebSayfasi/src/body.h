#ifndef BODY_H
#define BODY_H

#include <QtCore/qglobal.h>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QDate>

#include <Wt/WContainerWidget.h>
#include <Wt/WText.h>
#include <Wt/WEvent.h>
#include <Wt/WSignal.h>
#include <Wt/WMenu.h>
#include <Wt/WMenuItem.h>
#include <Wt/WHBoxLayout.h>
#include <Wt/WVBoxLayout.h>
#include <Wt/WImage.h>
#include <Wt/WLink.h>
#include <Wt/WCssDecorationStyle.h>
#include <Wt/WPushButton.h>
#include <Wt/WDate.h>
#include <Wt/WMediaPlayer.h>
#include <Wt/WStackedWidget.h>
#include <Wt/WTable.h>
#include <Wt/WTableCell.h>
#include <Wt/WMessageBox.h>
#include <Wt/WAnchor.h>
#include <Wt/WGridLayout.h>
#include <Wt/WLineEdit.h>
#include <Wt/WComboBox.h>
#include <Wt/WTextEdit.h>
#include <Wt/WMessageBox.h>
#include <Wt/WStandardItem.h>
#include <Wt/WStandardItemModel.h>
#include <Wt/WTableView.h>
#include <Wt/Chart/WPieChart.h>
#include <Wt/WItemDelegate.h>
#include <Wt/WAbstractItemView.h>
#include <Wt/Chart/WDataSeries.h>
#include <Wt/Chart/WCartesianChart.h>
#include <Wt/WTime.h>



#include "bootstrap.h"
#include "inlinestyle.h"
#include "mongoheaders.h"
#include "serikbeltr.h"
#include "basindabizwidget.h"
#include "db.h"


//#include <iostream>
//#include <memory>
//#include <iterator>
//#include <stdexcept>
//#include <random>



//using bsoncxx::builder::basic::document;
//using bsoncxx::builder::basic::kvp;
//using bsoncxx::builder::basic::array;
//using bsoncxx::builder::basic::make_document;
//using bsoncxx::builder::basic::make_array;

//using namespace Wt;


namespace Body {


    class Slider;
    class NewsAnnounceContent;
    class ContentWidget;
    class Meclis;



    class Body : public WContainerWidget
    {
        SerikBLDCore::DB* mDB;
    public:
        Body(mongocxx::database* _db, mongocxx::gridfs::bucket* _bucket);

        void initBody();


        void initProje();
        void initHaberler();
        void initEtkinlikler();
//        void initVideo();
        void initBilgiEdin();
        void initiletisim();
        void initTalep();
        void initGiris();
        void initHakkinda();
        void initCalismalar();



        NewsAnnounceContent* mNewsAnnounceContent;
        void setNewsDetail(std::string oid);
        void setAnnounceDetail(std::string oid);
        void setVideoDetail(std::string oid);

        ContentWidget* mContentWidget;
        void setBaskanDetail(std::string oid);
        void setEventDetail(std::string oid);
        void setProjeDetail(std::string oid);
        void setCalismaDetail(std::string oid);

    private:
        WContainerWidget* mMainContainer;

        mongocxx::database* db;
        mongocxx::gridfs::bucket* Bucket;

        mongocxx::collection SliderCollection;
        mongocxx::collection HaberlerCollection;
        mongocxx::collection DuyurularCollection;
        mongocxx::collection VideolarCollection;

        std::string downloadifNotExist(bsoncxx::types::value oid, bool forceFilename = false);
        std::string downloadifNotExist(std::string oid, bool forceFilename = false);


        Signal<NoClass> _clickBack;


        void showMessage( std::string title , std::string msg );

        inline int getRandom(int begin = 0 , int end = 255 )
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(begin,end);
            return dist(mt);
        }

    };




    class NewsAnnounceContent : public WContainerWidget
    {
    public:
        NewsAnnounceContent(mongocxx::database* _database);

        class NewsPanel : public WContainerWidget
        {
        public:
            NewsPanel(mongocxx::database* _db);


            class ControlPanel : public WContainerWidget
            {
            public:
                ControlPanel();
                Signal<std::string> &mGetClickNews();
                Signal<std::string> &mGetClickZiyaret();
                Signal<std::string> &mGetClickVideolar();
                Signal<std::string> &mGetClickEtkinlikler();
                Signal<std::string> &mGetClickProjects();
            private:
                Signal<std::string> _ClickPanel;

                Signal<std::string> _ClickZiyaret;

                Signal<std::string> _ClickVideolar;

                Signal<std::string> _ClickEtkinlikler;

                Signal<std::string> _ClickProjects;

            };



            class NewsList : public WContainerWidget
            {

            public:

                NewsList(mongocxx::database* _db, mongocxx::collection* _collection,mongocxx::collection* _Videcollection, mongocxx::gridfs::bucket* _Bucket);


                void initList(std::string key = SBLDKeys::Haberler::oid );

                void initZiyaretList(std::string key);

                void initVideoList(std::string key);

                void initEtkinlikList(std::string key);

                void initProjectList(std::string key);

                Signal<std::string> &mGetClickOid();
                Signal<std::string> &mGetClickVideoOid();
                Signal<std::string> &mGetClickEtkinlikOid();
                Signal<std::string> &mGetClickProjeOid();

            private:
                void addItem(std::string title,std::string text , std::string imgPath , std::string oid = "null", std::string type = "news");

                mongocxx::collection* Collection;
                mongocxx::collection* VideoCollection;
                mongocxx::gridfs::bucket* Bucket;
                mongocxx::database* db;

                WContainerWidget* mMainContainer;

                std::string currentKey;

                Signal<std::string> _ClickOid;
                Signal<std::string> _ClickVideoOid;
                Signal<std::string> _ClickEtkinlikOid;
                Signal<std::string> _ClickProjeOid;


            };


            mongocxx::database* db;

            mongocxx::collection nCollection;
            mongocxx::collection nVCollection;
            mongocxx::gridfs::bucket Bucket;

            NewsList* mNewsList;


        };





        class AnnouncePanel : public WContainerWidget
        {
        public:
            AnnouncePanel(mongocxx::database* _db);



            class AnnounceList : public WContainerWidget
            {
            public:
                AnnounceList(mongocxx::database* _db);


                Signal<std::string> &mGetOid();

                class AnnounceItem : public WContainerWidget
                {
                public:
                    AnnounceItem( std::string oid , std::string title , std::string birim );

                    Signal<std::string> _ClickItem;
                };

            private:
                mongocxx::database* db;
                mongocxx::collection Collection;
                void _ClickItem(std::string oid);
                Signal<std::string> mClickoid;
            };



            class TaskList : public WContainerWidget
            {
            public:
                TaskList(mongocxx::database* _db);

                class TaskItem : public WContainerWidget
                {
                public:
                    TaskItem(std::string title, std::string birim , std::string mahalle, std::string tarih);
                };

            private:
                mongocxx::database* db;
                mongocxx::collection Collection;
            };



            AnnounceList* mAnnounceList;
        private:
            mongocxx::database* db;

            Signal<std::string> _ClickAnnounceDetail;
        };


        NewsPanel* mNewsPanel;
        AnnouncePanel* mAnnouncePanel;

    };

    class IstatistikAnket : public WContainerWidget
    {
    public:
        IstatistikAnket();
    };

    class ProjectPanel : public WContainerWidget
    {
    public:
        ProjectPanel(mongocxx::database* _db);

        struct ProjectHeader
        {
            std::string oid;
            std::string title;
            std::string img;
        };

        struct CurrentItemKey
        {
            std::string imgPath;
            std::string text;
            int index;
        };


        void setCurrentProject();
        void refreshList();

    private:
        mongocxx::database* db;

        WContainerWidget* mMainContainer;

        void setstatus(bsoncxx::exception &e);
        void setstatus(mongocxx::exception &e);
        void setstatus(WString e);
        std::vector<ProjectHeader> list;

        int CurrentIndex;
        int SubCurrentIndex;


    };

    class BaskanaMesajWidget : public ContainerWidget, public SerikBLDCore::DB
    {
    public:
        BaskanaMesajWidget(mongocxx::database* _database);
        void initializetion(WContainerWidget* row);
    };







    namespace Job {

        class ProjectBlock;

        class TaskBlock;

        class Block : public WContainerWidget, SerikBLDCore::DB
        {
        public:
            Block(mongocxx::database* _db);

            WContainerWidget* mContainerFluid;

            ProjectBlock* mProjectBlock;
            TaskBlock* mTaskBlock;
        };

        class ProjectBlock : public ContainerWidget, public SerikBLDCore::DB
        {
        public:
            ProjectBlock(mongocxx::database* _db , bool _subPage = false);

            Signal<std::string> &ClickLastProject()
            {
                return this->_clickProject;
            }

            Signal<NoClass> &ClickProjects()
            {
                return this->_clickProjects;
            }
        private:
            WContainerWidget* mContainerFluid;

            WContainerWidget* mDurumDetailContainer;

            void displayinYear(int year = 0 );

            bool subPage;

            Signal<std::string> _clickProject;

            Signal<NoClass> _clickProjects;

        };

        class TaskBlock : public ContainerWidget, public SerikBLDCore::DB
        {
        public:
            TaskBlock(mongocxx::database* _db);

            Signal<std::string> &ClickTasks()
            {
                return this->_clickTask;
            }

        private:

            WContainerWidget* mContainerFluid;

            WContainerWidget* mDurumDetailContainer;

            Signal<std::string> _clickTask;


            void displayinYear(int year = 0 );


        };

    }









    class Iletisim : public WContainerWidget
    {
    public:
        Iletisim();
        Signal<NoClass> &mClickTalep();
    private:
        Signal<NoClass> _mClickTalep;

        void showMessage( std::string title , std::string msg );

    };



    class Proje : public ContainerWidget, public SerikBLDCore::DB
    {
    public:
        Proje(mongocxx::database* _db);
        Signal<NoClass> &mGetBack();

    private:
        Signal<NoClass> _ClickBack;
        Signal<std::string> _ClickHaber;

        WContainerWidget* mMainContainer,*container,*contentContainer;
        WContainerWidget* mStatisticContainer;

        void initIstatistic();
        void initMahallelereGoreDagilim();
        void initBirimlereGoreDagilim();
        void initYillaraGoreDagilim();
        void initDurumaGoreDagilim();

        void initPage();
        void setstatus(bsoncxx::exception &e);
        void setstatus(mongocxx::exception &e);
        void setstatus(std::string e);


        struct item
        {
            std::string oid;
            std::string iconPath;
            std::string title;
        };

        std::vector<item> list;

        void addItem(WContainerWidget* widget, item _item, bool OneColoum = false );

        void setselectedProject(std::string oid);


        bool initListinProje;
        WContainerWidget* ProjectDetailContainer;



        class NumericItem : public WStandardItem {
        public:
            virtual std::unique_ptr<WStandardItem> clone() const {
                return cpp14::make_unique<NumericItem>();
            }

            virtual void setData(const cpp17::any &data, int role = ItemDataRole::User) {
                if (role == ItemDataRole::Edit) {
                    cpp17::any dt;

                    double d = asNumber(data);

                    if (d != d)
                        dt = data;
                    else
                        dt = cpp17::any(d);
                    WStandardItem::setData(dt, role);

                } else {
                    WStandardItem::setData(data, role);
                }
            }
        };



    };

    class Calisma : public ContainerWidget, public SerikBLDCore::DB
    {
    public:
        Calisma(mongocxx::database* _db);

        void zamanTuneli();

        void addZamanTuneli(WContainerWidget* _rowContainer , WContainerWidget* LeftRowContainer, WContainerWidget*  RightRowContainer);
        void addCalismaItemToZamanTuneli(WContainerWidget* widget, bsoncxx::document::view doc );
        void setCalismaDetail(std::string oid);

        int skip = 0;
        bool LeftSide;

        WContainerWidget* mMainContainer;
        WContainerWidget* container;

        WContainerWidget* mContentContainer;

        WContainerWidget* mTableContainer;



        bsoncxx::document::value TableFilter;
        void initTable(bsoncxx::document::view view);
        void setSelectedCalisma(std::string oid);

        void initPage();

        void initMenu();
        WContainerWidget* mMenuContainer;

        Signal<NoClass> _ClickBack;


        const int limit = 50;
        std::int64_t CalismaCount;

        struct CalismaItem
        {
            std::string oid;
            std::string baslik;
            std::int64_t tarih;
            std::vector<std::string> mahalleList;
            std::string birim;
            std::string durum;
        };

        std::vector<CalismaItem> CalismaList;


        void initIstatistic();
        WContainerWidget* mChartesianChartWidget;

        void initIstatisticBirimlereGore();
        void initIstatisticMahallereGore();
        void initIstatisticBirimMahalle();


    };

    class Haber : public WContainerWidget
    {
    public:
        Haber(mongocxx::database* _db);

        Signal<NoClass> &mGetBack();
        Signal<std::string> &mGetHaber();


    private:
        mongocxx::database *db;
        Signal<NoClass> _ClickBack;
        Signal<std::string> _ClickHaber;

        WContainerWidget* mMainContainer,*container,*contentContainer;

        void initPage();
        void setstatus(bsoncxx::exception &e);
        void setstatus(mongocxx::exception &e);
        void setstatus(std::string e);


        struct item
        {
            std::string oid;
            std::string iconPath;
            std::string title;
        };

        std::vector<item> list;

        void addItem(WContainerWidget* widget, item _item);
        void setNews(std::string oid);

        const int limit = 12;
        int skip;
        std::int64_t count;
    };

    class Etkinlik : public WContainerWidget
    {
    public:
        Etkinlik(mongocxx::database* _db);

        Signal<NoClass> &mGetBack();
        Signal<std::string> &mGetEtkinlik();


    private:
        mongocxx::database *db;
        Signal<NoClass> _ClickBack;
        Signal<std::string> _ClickEtkinlik;

        WContainerWidget* mMainContainer,*container,*contentContainer;

        void initPage();
        void setstatus(bsoncxx::exception &e);
        void setstatus(mongocxx::exception &e);
        void setstatus(std::string e);


        struct item
        {
            std::string oid;
            std::string iconPath;
            std::string title;
        };

        std::vector<item> list;

        void addItem(WContainerWidget* widget, item _item);
        void setEtkinlik(std::string oid);

        const int limit = 12;
        int skip;
        std::int64_t count;
    };

//    class Video : public WContainerWidget
//    {
//    public:
//        Video(mongocxx::database* _db);

//        Signal<NoClass> &mGetBack();
//        Signal<std::string> &mGetVideo();


//    private:
//        mongocxx::database *db;
//        Signal<NoClass> _ClickBack;
//        Signal<std::string> _ClickVideo;

//        WContainerWidget* mMainContainer,*container,*contentContainer;

//        void initPage();
//        void setstatus(bsoncxx::exception &e);
//        void setstatus(mongocxx::exception &e);
//        void setstatus(std::string e);


//        struct item
//        {
//            std::string oid;
//            std::string iconPath;
//            std::string title;
//        };

//        std::vector<item> list;

//        void addItem(WContainerWidget* widget, item _item);
//        void setVideo(std::string oid);

//        const int limit = 12;
//        int skip;
//        std::int64_t count;
//    };



    class Talep : public WContainerWidget
    {
    public:
        Talep(mongocxx::database* _db);




    private:
        mongocxx::database* db;

        WContainerWidget* mMainContainer;

        WLineEdit* mTcNO;
        WLineEdit* mAdSoyad;
        WComboBox* mMahalle;
        WLineEdit* mTelefon;
        WLineEdit* mAdres;
        WPushButton* mSave;

        WComboBox* mTalepMahalle;
        WTextEdit* mEdit;


        void showMessage( std::string title , std::string msg );

        void Save();

        void TCChanged();
        void saveTC();
        bool mQueried;
        std::string oldtcno;
        std::string oldadsoyad;
        std::string oldmahalle;
        std::string oldtelefon;
    };





    namespace BilgiEdin {

    class BilgiEdin : public WContainerWidget
    {
    public:
        BilgiEdin(mongocxx::database* _db);

        enum Sayfa
        {
            ControlSayfa = 0,
            BilgiEdinSayfa,
            StratejikPlanSayfa,
            PerformansSayfa,
            FaaliyetSayfa,
            DenetimRaporSayfa,
            MaliDurumBeklentiSayfa,
            KamuHizmet,
            BagisSayfa,
            SunulanHizmet
        };


    private:
        mongocxx::database *db;

        WContainerWidget* mMainContainer;
        WContainerWidget* mHeaderContainer;

        WStackedWidget* mContentContainer;

        void initHeader();



        // Yeni Koyulan Sayfalar için Bu Fonksiyondaki Switch Case Kapatılacak
        void setsayfa(Sayfa sayfa);
        void showMessage( std::string title , std::string msg );
    };

    class Controller : public WContainerWidget
    {
    public:
        Controller();


        Signal<BilgiEdin::Sayfa> &clickedbasvuru();

    private:
        void addButton(std::string title, BilgiEdin::Sayfa sayfa = BilgiEdin::Sayfa::ControlSayfa);

        WContainerWidget* row;

        Signal<BilgiEdin::Sayfa> _clickbasvuru_;

        void showMessage( std::string title , std::string msg );
    };


    class StratejikPlan : public WContainerWidget
    {
    public:
        StratejikPlan(mongocxx::database* _db);


    };

    class Performans : public WContainerWidget
    {
    public:
        Performans();
    };

    class faaliyetRapor : public WContainerWidget
    {
    public:
        faaliyetRapor();
    };

    class DenetimRapor : public WContainerWidget
    {
    public:
        DenetimRapor();
    };

    class MaliRapor : public WContainerWidget
    {
    public:
        MaliRapor();
    };

    class KamuHizmetRapor : public WContainerWidget
    {
    public:
        KamuHizmetRapor();
    };

    }




    namespace Serik {

    class Hakkinda : public WContainerWidget
    {
    public:
        Hakkinda(mongocxx::database* _db):WContainerWidget(),db(_db) {this->initPage();}

    private:
        mongocxx::database* db;

        void initPage();

    };

    class Content : public WContainerWidget
    {
    public:
        Content( mongocxx::database* _db , std::string kategoriOid ):db(_db),mKategoriOid(kategoriOid) {this->initContent();}

    private:
        mongocxx::database* db;
        std::string mKategoriOid;

        WContainerWidget* ContentContainer;

        void initContent();

        void selectedContent(std::string oid);

    };

    }


}



#endif // BODY_H
