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
    void update_data(const char*** year_sorted_table, size_t len, size_t calculated_collum_idx, double mediana)
    {
        if (!year_sorted_table)
            throw std::invalid_argument("table_content is null");

        data.clear();
        for (size_t i = 0; i < len; ++i)
        {
            size_t year = atol(year_sorted_table[i][0]);
            if (!data.contains(year))
                data.insert(year, atof(year_sorted_table[i][calculated_collum_idx]));
        }
    }

    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        const int steps_count = 5;
        const int margin = 50;

        int w = width();
        int h = height();

        // **************** Find max`s and min`s *******************
        size_t min_x = data.firstKey();
        size_t max_x = data.lastKey();

        auto iter = data.begin();
        double min_y = iter.value();
        double max_y = iter.value();

        for (; iter != data.end(); ++iter) {
            double y = iter.value();
            min_y = std::min(min_y, y);
            max_y = std::max(max_y, y);
        }

        // ********************** Map func ***********************
        auto map_x = [&](size_t x) { return margin + double(x - min_x) / (max_x - min_x) * (w - 2 * margin); };
        auto map_y = [&](double y) { return h - margin - (y - min_y)   / (max_y - min_y) * (h - 2 * margin); };

        // ******************* x axis y-coord ********************
        int x_axis_y;
        if (min_y > 0)
            x_axis_y = h - margin;
        else if (max_y < 0)
            x_axis_y = margin;
        else
            x_axis_y = map_y(0);

        // ***************** Draw axis **********************
        painter.setPen(Qt::black);
        painter.drawLine(margin, x_axis_y, w - margin, x_axis_y);
        painter.drawLine(margin, margin, margin, h - margin);

        // ***************** Steps **********************

        // X
        for (int i = 0; i <= steps_count; ++i) {
            double x_val = min_x + (i * (max_x - min_x) / steps_count);
            int x = map_x(x_val);
            painter.drawLine(x, x_axis_y - 5, x, x_axis_y + 5);
            painter.drawText(x - margin/3 / 2, x_axis_y + 20, QString::number(x_val));
        }

        // Y
        for (int i = 0; i <= steps_count; ++i) {
            double y_val = min_y + (i * (max_y - min_y) / steps_count);
            int y = map_y(y_val);
            painter.drawLine(margin - 5, y, margin + 5, y);
            painter.drawText(5, y + margin/3 / 2, QString::number(y_val));
        }

        // ***************** Draw graph ************************
        auto line_iter = data.constBegin();
        QPoint prev(map_x(line_iter.key()), map_y(line_iter.value()));
        ++line_iter;

        for (; line_iter != data.constEnd(); ++line_iter) {
            QPoint curr(map_x(line_iter.key()), map_y(line_iter.value()));
            painter.drawLine(prev, curr);
            prev = curr;
        }

        // ***************** Graph point *********************
        painter.setBrush(Qt::red);
        for (auto point_iter = data.constBegin(); point_iter != data.constEnd(); ++point_iter) {
            painter.drawEllipse(QPoint(map_x(point_iter.key()), map_y(point_iter.value())), 1.5, 1.5);
        }
    }
private:
    QMap<size_t, double> data;
};

#endif //GRAPH_H