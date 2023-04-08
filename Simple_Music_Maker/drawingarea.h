#ifndef GTKMM_DRAWINGAREA_H
#define GTKMM_DRAWINGAREA_H

#include <gtkmm.h>

class DrawingArea : public Gtk::DrawingArea
{
    public:
        DrawingArea();

        void set_data(float frequency, float duration);
        bool on_click(GdkEventButton* event);

        bool draw_grid(const Cairo::RefPtr<Cairo::Context>& cr);
        bool fill_grid(const Cairo::RefPtr<Cairo::Context>& cr);
        bool update_grid(const Cairo::RefPtr<Cairo::Context>& cr);

        bool draw_frequency(const Cairo::RefPtr<Cairo::Context>& cr);

    private:
        double x_mouse_;
        double y_mouse_;

        int width;
        int height;

        int max_rows = 10;
        int max_columns = 10;

        std::vector<std::vector<std::vector<int>>> grid_points;

        float frequency_;
        float duration_;
};

#endif // GTKMM_DRAWINGAREA_H
