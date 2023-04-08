#include "drawingarea.h"


DrawingArea::DrawingArea()
{
    set_events(Gdk::BUTTON_PRESS_MASK);
    grid_points(10, std::vector<std::vector<int>>(10, std::vector<int>(2, 0)));
}

void DrawingArea::set_data(float frequency, float duration)
{
    frequency_ = frequency;
    duration_ = duration;
}

bool DrawingArea::draw_frequency(const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->paint();

    auto allocation = get_allocation();
    int width = allocation.get_width();
    int height = allocation.get_height();

    cr->set_source_rgb(0.0, 0.0, 0.0);

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

bool DrawingArea::draw_grid(const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->paint();

    auto allocation = get_allocation();
    width = allocation.get_width();
    height = allocation.get_height();

    cr->set_source_rgb(1.0, 0.0, 0.0);

    // Fill vector with grid points

    for (int column = 0; column < max_columns; column++)
    {
        for (int row = 0; row < max_rows; row++)
        {
            grid_points[column][row][0] = (int) std::round(((float) row / (float) max_rows * (float) width));
            grid_points[column][row][1] = (int) std::round(((float) column / (float) max_columns * (float) height));

            printf("%d\n", grid_points[column][row][0]);
        }
    }

    // Draw grid with grid points
    for (int column = 0; column < max_columns; column++)
    {
        int x1 = grid_points[0][column][0];
        int y1 = 0;

        int x2 = grid_points[0][column][0];
        int y2 = height;

        cr->move_to(x1, y1);
        cr->line_to(x2, y2);
        cr->stroke();
    }

    for (int row = 0; row < max_rows; row++)
    {
        int x1 = 0;
        int y1 = grid_points[row][0][1];

        int x2 = width;
        int y2 = grid_points[row][0][1];

        cr->move_to(x1, y1);
        cr->line_to(x2, y2);
        cr->stroke();
    }

    cr->fill();

    return true;
}

bool DrawingArea::fill_grid(const Cairo::RefPtr<Cairo::Context>& cr)
{
    /* point where rectangle starts

    int x_index;
    int y_index;

    for (int column = 0; column < max_columns - 1; column++)
    {
        int x1 = grid_points[0][column][0];
        int x2 = grid_points[0][column + 1][0];

        if ((int) std::round(x_mouse_) > x1 &&
            (int) std::round(x_mouse_) < x2)

            x_index = column;
    }

    for (int row = 0; row < max_rows - 1; row++)
    {
        int y1 = grid_points[row][0][1];
        int y2 = grid_points[row + 1][0][1];

        if ((int) std::round(y_mouse_) > y1 &&
            (int) std::round(y_mouse_) < y2)

            y_index = row;
    }*/

    // point where rectangle ends

    cr->set_source_rgb(0.0, 1.0, 0.0);
    cr->rectangle(0, 0, 10, 10);
    cr->fill();

    return true;
}

bool DrawingArea::update_grid(const Cairo::RefPtr<Cairo::Context>& cr)
{
    draw_grid(cr);
    fill_grid(cr);

    return true;
}

bool DrawingArea::on_click(GdkEventButton* event)
{
    x_mouse_ = event->x;
    y_mouse_ = event->y;

    queue_draw();

    return true;
}
