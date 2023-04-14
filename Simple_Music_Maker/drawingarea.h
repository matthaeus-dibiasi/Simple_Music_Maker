#ifndef GTKMM_DRAWINGAREA_H
#define GTKMM_DRAWINGAREA_H

#include <gtkmm.h>

class DrawingArea : public Gtk::DrawingArea
{
    public:
        DrawingArea();

        bool on_click(GdkEventButton* event);

        bool draw_grid(const Cairo::RefPtr<Cairo::Context>& cr);
        bool fill_grid(const Cairo::RefPtr<Cairo::Context>& cr);
        bool update_grid(const Cairo::RefPtr<Cairo::Context>& cr);

        bool draw_tones(const Cairo::RefPtr<Cairo::Context>& cr);

        std::vector<int> get_sound_data();
        void set_sound_data(const std::vector<int> sound_data);

    private:
        double x_mouse_;
        double y_mouse_;

        int width;
        int height;

        int max_rows = 3;
        int max_columns = 10;

        int x_index = 0;
        int y_index = 0;

        std::vector<std::vector<std::vector<int>>> grid_points;
        std::vector<std::vector<int>> grid_tone;
        std::vector<std::vector<bool>> grid_state;
        std::vector<int> grid_tone_frequency;

        void set_field_state_from_click();
};

#endif // GTKMM_DRAWINGAREA_H
