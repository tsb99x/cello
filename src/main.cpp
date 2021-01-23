#include "grid.hpp"
#include "glfw_w.hpp"

using namespace std;

class MainScene : public Scene {

    Glfw &glfw;
    Window &win;
    Render &gfx;
    Grid grid;

    double prev = glfw.current_time();
    double draw_time = 0;
    int iter = 0;

public:

    MainScene(Glfw &glfw, Window &win, Render &gfx, Grid grid)
        : glfw{glfw}, win{win}, gfx{gfx}, grid{grid}
    { }

    void loop() final
    {
        double before_draw = glfw.current_time();
        gfx.clear();
        grid.draw(gfx);
        draw_time += glfw.current_time() - before_draw;

        grid.do_tick();

        if (++iter >= 100) {
                string title = "Cello iter: 100@" + to_string(glfw.current_time() - prev)
                             + " draw: " + to_string(draw_time);
                win.update_title(title);
                draw_time = 0;
                prev = glfw.current_time();
                iter = 0;
        }
    }

    void on_key_event(const KeyEvent &event) final
    {
        if (event.key == Key::escape && event.action == Action::press) {
            win.close();
        }
    }

};

int main()
{
    const Size screen_size{1920, 1080};

    Glfw glfw;

    Window win{glfw, screen_size, "Cello"};
    Render gfx{win};
    gfx.configure_camera_ortho(screen_size);
    Grid grid{screen_size};
    grid.randomize();

    win.run_scene(make_unique<MainScene>(glfw, win, gfx, grid));

    return EXIT_SUCCESS;
}
