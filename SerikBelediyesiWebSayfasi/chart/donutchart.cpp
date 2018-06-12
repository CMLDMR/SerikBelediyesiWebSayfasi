#include "donutchart.h"



BChart::DonutChart::DonutChart()
{
    setMaximumSize(450,450);
    canvas = addWidget(cpp14::make_unique<WContainerWidget>());
    canvas->setHtmlTagName("canvas");

}

void BChart::DonutChart::setDataList(std::vector<BChart::DonutChart::item> list)
{




    QString labels;
    QString rgbs;
    QString values;

    for( auto _item : list )
    {
        labels.append(QString("\"")+_item.label.c_str()+QString("\","));
        rgbs.append(QString("\"")+_item.rgb.c_str()+QString("\","));
        values.append(QString::number(_item.value)+QString(","));
    }

    WString scriptCode = WString("var ctx = document.getElementById(\"{1}\").getContext('2d');"
                                           "var myChart = new Chart(ctx, {"
                                           "options: {"
                                           "tooltips: {"
                                           "callbacks: {"
                                           "labelColor: function(tooltipItem, chart) {"
                                           "return {"
                                           "borderColor: 'rgb(255, 0, 0)',"
                                           "backgroundColor: 'rgb(255, 0, 0)'"
                                           "}"
                                           "},"
                                           "labelTextColor:function(tooltipItem, chart){"
                                           "return '#ffffff';"
                                           "}"
                                           "}"
                                           "},"
                                           "legend: {"
                                           "display: true,"
                                           "labels: {"
                                           "fontColor: 'rgb(255, 255, 255)'"
                                           "}"
                                           "},"
                                           "cutoutPercentage: 50, "
                                           "title: {"
                                           "display: false,"
                                           "text: 'DOnut Chart'"
                                           "}"
                                           "},"
                                           "type: 'pie',"
                                           "data: {"
                                           "labels: [{2}],"
                                           "datasets: [{"
                                           "backgroundColor: [{3}],"
                                           "data: [{4}]"
                                           "}]"
                                           "}"
                                           "});").arg(canvas->id()).arg(labels.toStdString().c_str()).arg(rgbs.toStdString().c_str()).arg(values.toStdString().c_str()).toUTF8();

    this->doJavaScript(scriptCode.toUTF8());
}

void BChart::DonutChart::setCanvasHeight(int height)
{
    canvas->setHeight(height);
}



//this->doJavaScript(WString("var ctx = document.getElementById(\"{1}\").getContext('2d');"
//                           "var myChart = new Chart(ctx, {"
//                               "options: {"
//                               "tooltips: {"
//                                       "callbacks: {"
//                                           "labelColor: function(tooltipItem, chart) {"
//                                               "return {"
//                                                   "borderColor: 'rgb(255, 0, 0)',"
//                                                   "backgroundColor: 'rgb(255, 0, 0)'"
//                                                  "}"
//                                           "},"
//                                           "labelTextColor:function(tooltipItem, chart){"
//                                               "return '#543453';"
//                                           "}"
//                                       "}"
//                                   "},"
//                               "legend: {"
//                                       "display: true,"
//                                       "labels: {"
//                                           "fontColor: 'rgb(125, 99, 132)'"
//                                       "}"
//                                   "},"
//                               "cutoutPercentage: 50, "
//                                 "title: {"
//                                   "display: true,"
//                                   "text: 'Custom Chart Title'"
//                                 "}"
//                               "},"
//                             "type: 'pie',"
//                             "data: {"
//                               "labels: [\"Toplam\", \"Tamamlanan\", \"Devam Eden\", \"Yapılacak\"],"
//                               "datasets: [{"
//                                 "backgroundColor: ["
//                                   "\"rgb(45,150,89)\","
//                                   "\"#3498db\","
//                                   "\"#95a5a6\","
//                                   "\"#9b59b6\","
//                                   "\"#f1c40f\""
//                                 "],"
//                                 "data: [12, 19, 3, 17, 28]"
//                               "}]"
//                             "}"
//                           "});").arg(canvas->id()).toUTF8());
