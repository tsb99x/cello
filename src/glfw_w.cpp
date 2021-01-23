#include "glfw_w.hpp"

#include <stdexcept>

extern "C" {
    #include <GLFW/glfw3.h>
}

using namespace std;

Glfw::Glfw()
{
    if (!glfwInit())
        throw runtime_error("failed to init the GLFW subsystem");
}

Glfw::~Glfw()
{
    glfwTerminate();
}

double Glfw::current_time()
{
    return glfwGetTime();
}

void Glfw::poll_events()
{
    glfwPollEvents();
}

Size::Size(int w, int h)
    : w{w}, h{h}
{
    if (w < 0 || h < 0)
        throw runtime_error("size components w, h must be >= 0");
}

int Size::area() const
{
    return w * h;
}

Color::Color(float r, float g, float b)
    : r{r}, g{g}, b{b}
{
    if (r < 0 || g < 0 || b < 0)
        throw runtime_error("color components r, g, b must be >= 0");
}

Window::Window(Glfw &glfw, const Size &size, const string &title)
    : glfw{glfw}
{
    handle = glfwCreateWindow(size.w, size.h, title.c_str(), nullptr, nullptr);
    if (!handle)
        throw runtime_error("failed to create a window");
    glfwSetWindowUserPointer(handle, this);
    glfwMakeContextCurrent(handle);
    glfwSwapInterval(0);
}

Window::~Window()
{
    glfwDestroyWindow(handle);
}

void Window::update_title(const string &title)
{
    glfwSetWindowTitle(handle, title.c_str());
}

void key_cb_wrapper(GLFWwindow *handle, int key, int scancode, int action, int mods)
{
    // FIXME : key and action casts are just bad
    Window *window = static_cast<Window *>(glfwGetWindowUserPointer(handle));
    Key key_pressed = static_cast<Key>(key);
    Action action_done = static_cast<Action>(action);
    window->scene->on_key_event({key_pressed, action_done});
}

void Window::close()
{
    glfwSetWindowShouldClose(handle, 1);
}

void Window::run_scene(unique_ptr<Scene> next_scene)
{
    scene = move(next_scene);
    glfwSetKeyCallback(handle, key_cb_wrapper);
    while (!glfwWindowShouldClose(handle)) {
        scene->loop();
        glfwSwapBuffers(handle);
        glfw.poll_events();
    }
}

Render::Render(const Window &win)
{ }

void Render::clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Render::configure_camera_ortho(const Size &size)
{
    glViewport(0, 0, size.w, size.h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, size.w, size.h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Render::update_color(const Color &color)
{
    glColor3f(color.r, color.g, color.b);
}

void Render::in_points_mode(function<void()> fn)
{
    glBegin(GL_POINTS);
    fn();
    glEnd();
}

void Render::place(const Point &point)
{
    glVertex2i(point.x, point.y);
}
