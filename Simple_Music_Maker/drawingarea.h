#ifndef GTKMM_DRAWINGAREA_H
#define GTKMM_DRAWINGAREA_H


class DrawingArea : public Gtk::DrawingArea
{
    public:
        DrawingArea()
        {
            signal_draw().connect(sigc::mem_fun(*this, &DrawingArea::on_draw));
        }

        void set_data(float frequency, float duration)
        {
            frequency_ = frequency;
            duration_ = duration;
        }

    private:
        float frequency_;
        float duration_;

    protected:
        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override
        {
            cr->set_source_rgb(0.0, 0.0, 0.0);
            cr->paint();

            auto allocation = get_allocation();
            int width = allocation.get_width();
            int height = allocation.get_height();

            cr->set_source_rgb(1.0, 0.0, 0.0);

            for (int x = 0; x < (width - 1); x++)
            {
                int x1 = x;
                int x2 = x + 1;
                int y1 = sin(2.0 * 3.14 * frequency_ * ((float) x / (float) width)) * (height / 2.0) + (height / 2.0);
                int y2 = sin(2.0 * 3.14 * frequency_ * ((float) (x + 1) / (float) width)) * (height / 2.0) + (height / 2.0);

                cr->move_to(x1, y1);
                cr->line_to(x2, y2);
                cr->stroke();
            }

            cr->fill();

            return true;
        }
};

#endif // GTKMM_DRAWINGAREA_H
