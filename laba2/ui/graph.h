//
// Created by k3rnel1x on 13.04.2026.
//

#ifndef GRAPH_H
#define GRAPH_H
#include <QWidget>
#include <QPainter>

class MetrixGraph : public QWidget
{
public:
    void update_data(const char*** year_sorted_table, size_t len)
    {
        if (!year_sorted_table) throw std::invalid_argument("table_content is null");
        for (size_t i = 0; i < len; ++i)
        {
            size_t year = atol(year_sorted_table[i][0]);
            if (!func.contains(year))
                func.insert(year, atof(year_sorted_table[i][2]));
            qDebug() << year_sorted_table[i][0];
        }
        // qDebug() << "len = " << len;
    }

    void paintEvent(QPaintEvent *)
    {
        QPainter painter;
        painter.begin(this);

        int i = 0;
        for(auto iter = func.begin(); iter != func.end(); ++iter)
            painter.drawText(100, 100 + (i += 20), QString::number(iter.key()) + QString::number(iter.value()));
        // qDebug() << i;
        painter.end();

    }
private:
    QMap<size_t, double> func;
};

#endif //GRAPH_H
