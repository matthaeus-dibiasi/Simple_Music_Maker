#include "drawingarea.h"


DrawingArea::DrawingArea() :
    grid_points(max_rows + 1, std::vector<std::vector<int>>(max_columns + 1, std::vector<int>(2, 0))),
    grid_tone(max_rows, std::vector<int>(max_columns, 0)),
    grid_state(max_rows, std::vector<bool>(max_columns, false)),
    grid_tone_frequency(max_columns, 0)

{
    set_events(Gdk::BUTTON_PRESS_MASK);
}

bool DrawingArea::draw_tones(const Cairo::RefPtr<Cairo::Context>& cr)
{
    cr->set_source_rgb(0.0, 0.0, 0.0);
    cr->paint();

    auto allocation = get_allocation();
    int width = allocation.get_width();
    int height = allocation.get_height();

    cr->set_source_rgb(1.0, 0.0, 0.0);

    int pixel_per_tone = std::round((float) width / grid_tone_frequency.size());

    for (int tone = 0; tone < static_cast<int>(grid_tone_frequency.size()); tone++)
    {
        for (int x = pixel_per_tone * tone;
                 x < pixel_per_tone * (tone + 1);
                 x++)
        {
            int x1 = x;
            int x2 = x + 1;

            float x1_scaled = (float) (x1 - pixel_per_tone * tone) / (float) pixel_per_tone;
            float x2_scaled = (float) (x2 - pixel_per_tone * tone) / (float) pixel_per_tone;

            int y1 = sin(2.0 * 3.14 * grid_tone_frequency[tone] * x1_scaled) * (height / 2.0) + (height / 2.0);
            int y2 = sin(2.0 * 3.14 * grid_tone_frequency[tone] * x2_scaled) * (height / 2.0) + (height / 2.0);

            cr->move_to(x1, y1);
            cr->line_to(x2, y2);
            cr->stroke();
        }
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

    /* Fill vector with grid points */
    for (int column = 0; column <= max_columns; column++)
    {
        for (int row = 0; row <= max_rows; row++)
        {
            grid_points[row][column][0] = (int) std::round(((float) column / (float) max_columns * (float) width));
            grid_points[row][column][1] = (int) std::round(((float) row / (float) max_rows * (float) height));
        }
    }

    /* Draw horizontal grid lines */
    for (int row = 0; row <= max_rows; row++)
    {
        int x1 = 0;
        int y1 = grid_points[row][0][1];

        int x2 = width;
        int y2 = grid_points[row][0][1];

        cr->move_to(x1, y1);
        cr->line_to(x2, y2);
        cr->stroke();
    }

    /* Draw vertical grid lines */
    for (int column = 0; column <= max_columns; column++)
    {
        int x1 = grid_points[0][column][0];
        int y1 = 0;

        int x2 = grid_points[0][column][0];
        int y2 = height;

        cr->move_to(x1, y1);
        cr->line_to(x2, y2);
        cr->stroke();
    }

    cr->fill();

    return true;
}

bool DrawingArea::fill_grid(const Cairo::RefPtr<Cairo::Context>& cr)
{
    /* Fill field */
    cr->set_source_rgb(1.0, 0.0, 0.0);

    for (int column = 0; column < max_columns; column++)
    {
        for (int row = 0; row < max_rows; row++)
        {
            if ((int) grid_state[row][column] == 1)
            {
                int x1 = grid_points[row][column][0];
                int y1 = grid_points[row][column][1];

                int x2 = grid_points[row + 1][column + 1][0];
                int y2 = grid_points[row + 1][column + 1][1];

                cr->rectangle(x1, y1, x2 - x1, y2 - y1);
            }
        }
    }

    cr->fill();

    return true;
}

void DrawingArea::set_field_state_from_click()
{
    /* Find correct column */
    for (int column = 0; column <= max_columns - 1; column++)
    {
        int x1 = grid_points[0][column][0];
        int x2 = grid_points[0][column + 1][0];

        if ((int) std::round(x_mouse_) > x1 &&
            (int) std::round(x_mouse_) < x2)
        {
            x_index = column;
            break;
        }
    }

    /* Find correct row */
    for (int row = 0; row <= max_rows - 1; row++)
    {
        int y1 = grid_points[row][0][1];
        int y2 = grid_points[row + 1][0][1];

        if ((int) std::round(y_mouse_) > y1 &&
            (int) std::round(y_mouse_) < y2)
        {
            y_index = row;
            break;
        }
    }

    grid_state[y_index][x_index] = !grid_state[y_index][x_index];

    /* Write tone to buffer */
    int tone = (1 + y_index) * grid_state[y_index][x_index];
    grid_tone[y_index][x_index] = tone;

    /* Convert tones to single frequency */
    for (int column = 0; column < max_columns; column++)
    {
        grid_tone_frequency[column] = 0;

        for (int row = 0;  row < max_rows; row++)
        {
            grid_tone_frequency[column] += grid_tone[row][column];
        }

        printf("col: %d, %d\n", column, grid_tone_frequency[column]);
    }

    printf("\n");
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

    set_field_state_from_click();

    queue_draw();

    return true;
}

std::vector<int> DrawingArea::get_sound_data()
{
    return grid_tone_frequency;
}

void DrawingArea::set_sound_data(const std::vector<int> sound_data)
{
    grid_tone_frequency.assign(sound_data.begin(), sound_data.end());
}
