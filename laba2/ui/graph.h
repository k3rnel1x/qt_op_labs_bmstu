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
        // qDebug() << "calculated_collum_idx =" << calculated_collum_idx;
        for (size_t i = 0; i < len; ++i)
        {
            size_t year = atol(year_sorted_table[i][0]);
            if (!func.contains(year))
                func.insert(year, atof(year_sorted_table[i][calculated_collum_idx]));
        }
        sorted_values = func.values();
        std::sort(sorted_values.begin(), sorted_values.end());
    }

    void paintEvent(QPaintEvent *)
    {
        QPainter p;
        p.begin(this);
        int padd = 20;
        double min_value = *sorted_values.begin();
        double max_value = *(sorted_values.end()-1);
        const int max_xline_elems = 30;

        int y_step = height() / (func.count()+1);
        int start_y = y_step;
        p.drawLine(padd, height(), padd, 0);
        if (min_value >= 0)
        {
            p.translate(padd, height() - padd);
            y_step = -y_step;
            start_y = -start_y;
        }
        else if (max_value <= 0)
        {
            p.translate(padd, padd);
        }
        else
        {
            y_step = height() / func.count();
            p.translate(padd, height()/2);
            y_step = -y_step;
            start_y = -y_step * func.count() / 2;
        }


        int x_l = -padd/3, x_r = +padd/3;
        for (int i = 0; i < func.count(); ++i)
        {
            p.drawLine(x_l, start_y, x_r, start_y);
            p.drawText(x_r, start_y, QString::number(sorted_values[i]));
            start_y += y_step;
        }

        if (!(max_value <= 0) && !(min_value >= 0))
        {
            start_y = -y_step * func.count() / 2;
            double prev = sorted_values[0], curr;
            for (int i = 1; i < func.count(); ++i)
            {
                curr = sorted_values[i];
                if (prev < 0 && curr > 0)
                {
                    p.drawLine(-padd, start_y, width() - padd, start_y);
                    break;
                }
                start_y += y_step;
            }
        } else p.drawLine(-padd, 0, width() - padd, 0);


        int x_step = (width()-padd) / (func.count()+1);
        int y_u = -padd/3, y_d = +padd/3;
        for (int x = x_step; x < width() - padd; ++x)
        {
            prin
        }

        p.end();
    }
private:

    double calc_normal_step(double min, double max, int steps)
    {
        double range = max - min;
        if (range <= 0) return 1.0;

        double raw_step = range / steps; // 152.67
        double pw = pow(10, floor(log10(raw_step))); // 2.423 -> 2.0 -> 100.0
        qDebug() << "raw_step =" << raw_step;
        qDebug() << "pw =" << pw;
        
        double residual = raw_step / pw;
        double step;
        if (residual < 1.5) step = 1.0;
        else if (residual < 7.0) step = 5.0;
        else step = 10.0;
        
        return step * pw;
    }
    QMap<size_t, double> func;
    QList<double> sorted_values;
};

#endif //GRAPH_H