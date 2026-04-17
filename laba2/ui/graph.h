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
    void update_data(const char*** year_sorted_table, size_t len, size_t calculated_collum_idx)
    {
        if (!year_sorted_table) throw std::invalid_argument("table_content is null");
        func.clear();
        for (size_t i = 0; i < len; ++i)
        {
            size_t year = atol(year_sorted_table[i][0]);
            if (!func.contains(year))
                func.insert(year, atof(year_sorted_table[i][calculated_collum_idx]));
            // qDebug() << year_sorted_table[i][0];
        }

        // for (auto iter = func.begin(); iter != func.end(); ++iter)
        // {
        //     qDebug() << iter.key() << " " << iter.value();
        // }

        // calc max and min
        min_val = min_val = func.value(func.firstKey());
        for (auto iter = func.begin(); iter != func.end(); ++iter){
            max_val = qMax(max_val, iter.value());
            min_val = qMin(min_val, iter.value());
        }
    }

    void paintEvent(QPaintEvent *)
    {
        QPainter p;
        p.begin(this);
        int padd = 20;
        const int max_xline_elems = 30;
        p.translate(padd, (min_val >= 0? height() - padd : height()/2));
        int max_y = (min_val >= 0? -height() + padd : -height()/2 + padd);
        int min_y = (min_val >= 0? +padd : +height()/2 - padd);
        int max_x = width() - padd;

        // ############################## coord lines ############################
        p.drawLine(0-padd, 0, max_x, 0); // Ox
        p.drawLine(0, min_y, 0, max_y); // Oy

        // ############################## years ##################################
        {
            QFont font = p.font();
            font.setPixelSize(padd/2);
            font.setBold(true);
            p.setFont(font);
            int offset = max_x / func.count();
            int x = offset;
            int y_up   = -padd/3;
            int y_down = padd/3;
            int y_txt  = padd;
            int x_txt  = offset - padd/2;
            for (auto iter = func.begin(); iter != func.end(); ++iter) {
                // draw cutout
                p.drawLine(x, y_up, x, y_down);
                // draw year
                p.drawText(x_txt, y_txt, QString::number(iter.key()));
                x += offset;
                x_txt += offset;
            }
        }

        p.end();
    }
private:
    void draw_with_negative();
    void draw_only_positive();
    QMap<size_t, double> func;
    double max_val;
    double min_val;
};

#endif //GRAPH_H
