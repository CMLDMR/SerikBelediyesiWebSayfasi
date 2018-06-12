#ifndef DONUTCHART_H
#define DONUTCHART_H

#include <QtCore/qglobal.h>
#include <QString>
#include <Wt/WContainerWidget.h>
#include <iostream>
#include <memory>
#include <iterator>
#include <random>
#include "mongoheaders.h"

using bsoncxx::builder::basic::document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::array;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;

using namespace Wt;


namespace BChart {


    class DonutChart : public WContainerWidget
    {
    public:
        DonutChart();


        struct item
        {
            std::string label;
            std::string rgb;
            int value;
        };

        void setDataList(std::vector<BChart::DonutChart::item> list);

        void setCanvasHeight(int height);

    private:
        WContainerWidget* canvas;
    };
}



#endif // DONUTCHART_H
