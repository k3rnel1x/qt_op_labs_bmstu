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
        // qDebug() << func.values();
        this->mediana = mediana;

        int i = 0;
        QList<double> vals = func.values();
        for (auto iter = vals.begin(); iter != vals.end(); ++iter, ++i)
        {
            if ( *iter == *sorted_values.begin() )
            {
                this->minimana_idx = i;
            }
            if ( *iter == *(sorted_values.end()-1) )
            {
                this->maxiana_idx = i;
            }
        }
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

        // draw graph
        y_step = y_step * (max_value <= 0? 1:-1);
        int prev_x = x_step;
        x = prev_x + x_step;

        auto iter = func.begin();
        int prev_y = y_step * ( sorted_values.indexOf(iter.value()) + 1 ); ++iter;
        y = y_step * ( sorted_values.indexOf(iter.value()) + 1 ); ++iter;

        // qDebug() << iter.value();
        while (iter != func.end())
        {
            p.drawLine(prev_x, prev_y, x, y);
            prev_x = x;
            prev_y = y;
            x += x_step;
            // qDebug() << iter.value();
            y = y_step * (sorted_values.indexOf(iter.value()) + 1);
            ++iter;
        }
        QPen pen;
        pen.setColor(Qt::red); // Change color
        pen.setWidth(5);       // Change size
        p.setPen(pen);
        // qDebug() << minimana_idx;
        // qDebug() << maxiana_idx;
        p.drawPoint(x_step * (minimana_idx + 1), y_step * (sorted_values.indexOf(*(func.begin() + minimana_idx)) + 1));
        p.drawPoint(x_step * (maxiana_idx + 1), y_step * (sorted_values.indexOf(*(func.begin() + maxiana_idx)) + 1));
        p.end();
    }
private:
    double maxiana_idx;
    double mediana;
    double minimana_idx;
    QMap<size_t, double> func;
    QList<double> sorted_values;
};

#endif //GRAPH_H