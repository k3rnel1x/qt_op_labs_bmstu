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
        QFont font;
        font.setPixelSize(10);
        p.setFont(font);
        int padd = 20;
        double min_value = *sorted_values.begin();
        double max_value = *(sorted_values.end()-1);
    
        // draw y
        p.drawLine(+2*padd, padd, +2*padd, height());
        int y = height() - 2*padd;
        int y_step = y / func.count();
        for (int i = 0, x_l=2*padd-padd/3, x_r=2*padd+padd/3; i < sorted_values.count(); ++i)
        {
            p.drawLine(x_l, y, x_r, y);
            p.drawText(0, y-padd/4, QString::number(sorted_values[i]));
            y -= y_step;
        }

        // draw x
        // y = padd + y_step*sorted_values.count();
        // y = padd/2 + y_step*sorted_values.count();
        y = height() - 2*padd;
        if(min_value >= 0)
        {
            p.translate(2*padd, y+padd);
        } else if(max_value <= 0){
            p.translate(padd, 0);
        } else {
            double prev = sorted_values[0], curr;
            for(int i = 1; i < sorted_values.count(); ++i)
            {
                curr = sorted_values[i];
                if(prev < 0 && curr > 0)
                {
                    p.translate(y + y_step/2, 2*padd);
                    break;
                } else if(curr == 0 ) { 
                    p.translate(2*padd, y-y_step);
                    break;
                }
                prev = curr;
                y -= y_step;
            }
        }
        
        p.drawLine(-padd*2, 0, width()-2*padd, 0);
        
        int x_step = (width()-2*padd)/func.count();
        int x = x_step;
        int y_u = -padd/3, y_d = +padd/3;
        for(auto iter = func.begin(); iter != func.end(); ++iter)
        {
            p.drawLine(x, y_u, x, y_d);
            p.drawText(x-padd/2, y_d+padd/2, QString::number(iter.key()));
            x += x_step;
        }
        
        int prev_x = x_step * max_value <= 0? -1:0, prev_y = y_step * max_value <= 0? -1:1;
        for(int x = prev_x + x_step, y = prev_y + y_step; abs(x) < width() && abs(y) < height();)
        {
            p.drawLine(prev_x, prev_y, x, y);
            prev_x = x;
            prev_y = y;
            if(max_value <= 0) {
                x += x_step;
                y += y_step;
            } else {
                x -= x_step;
                y -= y_step;
            }
        }
        p.end();
    }
private:

    // double calc_normal_step(double min, double max, int steps)
    // {
    //     double range = max - min;
    //     if (range <= 0) return 1.0;

    //     double raw_step = range / steps; // 152.67
    //     double pw = pow(10, floor(log10(raw_step))); // 2.423 -> 2.0 -> 100.0
    //     qDebug() << "raw_step =" << raw_step;
    //     qDebug() << "pw =" << pw;
        
    //     double residual = raw_step / pw; 
    //     double step;
    //     if (residual < 1.5) step = 1.0;
    //     else if (residual < 7.0) step = 5.0;
    //     else step = 10.0;
        
    //     return step * pw;
    // }
    QMap<size_t, double> func;
    QList<double> sorted_values;
};

#endif //GRAPH_H