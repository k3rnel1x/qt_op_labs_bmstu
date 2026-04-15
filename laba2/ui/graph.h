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

        for (auto iter = func.begin(); iter != func.end(); ++iter)
        {
            qDebug() << iter.key() << " " << iter.value();
        }

        // calc max and min
        max = min = func.value(func.firstKey());
        for (auto iter = func.begin(); iter != func.end(); ++iter){
            max = qMax(max, iter.value());
            min = qMin(min, iter.value());
        }
    }

    void paintEvent(QPaintEvent *)
    {
        QPainter p;
        p.begin(this);
        int padding = 10;
        const int max_xline_elems = 30;

        // ########################## draw coords lines #########################
        p.translate(width() / 2, height() / 2); // move coords to center

        // paint y
        p.drawLine(-width()/2 + padding, -height()/2 + padding/2, -width()/2 + padding, height()/2 - padding/2);

        // paint x
        p.drawLine(-width()/2 + padding, 0, width()/2 - padding, 0);

        // ############################ draw years ##############################
        const int x_offset = (width() - 2*padding) / func.count();
        int x_coord = padding + x_offset;
        int counter = 0;
        QFont font = p.font();
        font.setPixelSize(padding);
        p.setFont(font);
        for (auto iter = func.begin(); iter != func.end(); ++iter)
        {
            if (counter++ > max_xline_elems) break;
            p.drawLine(-width()/2 + x_coord, padding/2, -width()/2 + x_coord, -padding/2);
            p.drawText(-width()/2 + x_coord - padding, 1.5 * padding, QString::number(iter.key()));
            x_coord += x_offset;
        }

        // ############################## draw func ############################
        {
            counter = 0;
            const int y_offset = (height() - padding) / func.count();
            int step = ceil((max - min)  / func.count());
            int num = 0;
            QPoint l(-width()/2 + padding / 2, 0), r(-width()/2 + 1.5 * padding, 0);

            // draw zero
            p.drawText(-width() / 2 + 2*padding, +padding, "0");

            for (int i = 1; i < func.count(); ++i)
            {
                qDebug() << l << r;
                p.drawLine(l.x(), l.y(), r.x(), r.y());
                p.drawText(r.x() + padding, -(r.y() - padding/2), QString::number(num + max));
                p.drawLine(l.x(), -l.y(), r.x(), -r.y());
                p.drawText(r.x() + padding, r.y() - padding/2, QString::number(-num - min));

                l.setY(l.y() + y_offset);
                r.setY(r.y() + y_offset);
                // if (counter++ > max_xline_elems) break;
                num += step;
            }
        }

        counter = 0;
        QPoint prev_point;
        x_coord = padding + x_offset;
        for (auto iter = func.begin(); iter != func.end(); ++iter)
        {
            // qDebug() << -(int)(height() * (iter.value() - min) / (max - min));
            QPoint curr_point = {-width()/2 + x_coord, -(int)((height()/2 - padding) * (iter.value() - min) / (max - min)) };
            if (counter != 0) {
                p.drawLine(prev_point, curr_point);
            }

            // p.drawLine(-width()/2 + padding/2, curr_point.y(), -width()/2 + 3*padding/2, curr_point.y());

            prev_point = curr_point;
            if (counter++ > max_xline_elems) break;
            p.drawPoint(curr_point);
            x_coord += x_offset;
        }
        p.end();
    }
private:
    QMap<size_t, double> func;
    double max;
    double min;
};

#endif //GRAPH_H
