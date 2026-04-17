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
            // qDebug() << year_sorted_table[i][calculated_collum_idx];
        }
        // for (auto iter = func.begin(); iter != func.end(); ++iter)
        // {
        //     qDebug() << iter.key() << " " << iter.value();
        // }

        // calc max and min
        min_val = min_val = func.begin().value();
        for (auto iter = func.begin(); iter != func.end(); ++iter){
            max_val = qMax(max_val, iter.value());
            min_val = qMin(min_val, iter.value());
        }

        // qDebug() << "max_val = " << max_val;
        // qDebug() << "min_val = " << min_val;
    }

    void paintEvent(QPaintEvent *)
    {
        QPainter p;
        p.begin(this);
        int padd = 20;
        int y_steps_count = 8;   
        const int max_xline_elems = 30;

        p.translate(padd, (min_val >= 0? height() - padd : height()/2));
        int max_y = (min_val >= 0? -height() + padd : -height()/2 + padd);
        int min_y = (min_val >= 0? +padd : +height()/2 - padd);
        int max_x = width() - padd;

        // ############################## coord lines ############################
        p.drawLine(0-padd, 0, max_x, 0); // Ox
        p.drawLine(0, min_y, 0, max_y); // Oy

        // ############################## draw cuts, year, func ##############################

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
            
            // func
            int func_scale = max_y / max_val;

            for (auto iter = func.begin(); iter != func.end(); ++iter) {
                // draw cutout
                p.drawLine(x, y_up, x, y_down);
                // draw year
                p.drawText(x_txt, y_txt, QString::number(iter.key()));
                p.drawPoint(x, func_scale*iter.value());
                x += offset;
                x_txt += offset;
            }
        }

        // ############################## draw y-cuts ##############################

        {
            // 0.13 ... 0.77 0.64
            // 0.64 / 10 =      xz ,nf fdhj, nmcyvfgf ggzpIX{DS Rk rg gefd}

            double cut_offset = calc_normal_step(min_val, max_val, y_steps_count); 

            double num = int(min_val) + cut_offset;

            int screen_offset = max_y / y_steps_count;
            int y = screen_offset;
            int x_l = -padd/3;
            int x_r = +padd/3;

            int y_txt = screen_offset + padd/2;
            int x_txt = padd/3;
            // if(min_val >= 0)
            {
                for (auto iter = func.begin(); iter != func.end(); ++iter) {
                    p.drawLine(x_l, y, x_r, y);
                    if(min_val < 0)
                        p.drawLine(x_l, -y, x_r, -y);

                    // qDebug() << "i =" << i << "y = " << y;
                    p.drawText(x_txt, y_txt, QString::number(num));
                    if(min_val < 0)
                        p.drawText(x_txt, -(y_txt - padd), QString::number(-num));

                    y += screen_offset;
                    y_txt += screen_offset;
                    num += cut_offset;
                }
            } 
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
    double max_val;
    double min_val;
};

#endif //GRAPH_H