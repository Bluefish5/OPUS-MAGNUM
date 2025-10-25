// OpenGL Pencil Drawing - single-file example (C++17)
// Uses GLFW and GLAD. Draw with left mouse button (press + move).
// 
// STEP-BY-STEP DESCRIPTION (HOW IT WORKS):
// (1) Program initializes GLFW and creates an OpenGL context/window.
// (2) GLAD is initialized to load OpenGL functions.
// (3) Simple GLSL vertex and fragment shaders are compiled and linked into a program.
// (4) We create a VAO/VBO pair to stream vertex (2D) positions to the GPU.
// (5) Input callbacks are registered:
//      - mouse button callback: starts/ends a stroke when left button pressed/released
//      - cursor position callback: while mouse is down, collects points into current stroke
//      - framebuffer size callback: updates viewport on window resize
// (6) The main loop polls events, handles simple keyboard commands (ESC, C),
//    and draws all saved strokes and the currently drawing stroke each frame.
// (7) Each stroke is drawn with GL_LINE_STRIP; the VBO is updated per-stroke with
//    glBufferData(GL_ARRAY_BUFFER, ...), which is simple and sufficient for a drawing app.
// (8) On exit we delete GL objects and terminate GLFW.
//
// NOTES & EXTENSIONS:
// - This uses normalized device coordinates (NDC). Mouse positions (pixels) are converted to NDC
//   so geometry can be drawn in clip space without any projection matrix.
// - The app stores strokes as vectors of 2D points. Each stroke is a contiguous polyline.
// - For production, consider batching, smoothing input, saving to image/SVG, and adding UI.

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>
#include <iostream>
#include <string>
#include <cmath>

// Simple 2D vector for vertex positions (x,y)
struct Vec2 { float x, y; };

// --- Shader sources -------------------------------------------------------
// Vertex shader: expects vec2 positions in clip/NDC space and sets gl_Position
static const char* vertex_shader_src = R"glsl(
#version 330 core
layout(location = 0) in vec2 aPos;
void main() {
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)glsl";

// Fragment shader: uniform color for the pencil
static const char* fragment_shader_src = R"glsl(
#version 330 core
out vec4 FragColor;
uniform vec3 uColor;
void main() {
    FragColor = vec4(uColor, 1.0);
}
)glsl";

// --- Shader compilation utility -------------------------------------------
GLuint compile_shader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    int ok; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char buf[1024]; glGetShaderInfoLog(s, 1024, nullptr, buf);
        std::cerr << "Shader compile error: " << buf << std::endl;
    }
    return s;
}

// Link vertex and fragment into a program
GLuint create_program() {
    GLuint v = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
    GLuint f = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);
    GLuint p = glCreateProgram();
    glAttachShader(p, v); glAttachShader(p, f);
    glLinkProgram(p);
    int ok; glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        char buf[1024]; glGetProgramInfoLog(p, 1024, nullptr, buf);
        std::cerr << "Program link error: " << buf << std::endl;
    }
    // shaders can be deleted after linking
    glDeleteShader(v); glDeleteShader(f);
    return p;
}

// --- Global state ---------------------------------------------------------
int g_win_w = 800, g_win_h = 600; // window size (updated on resize)

// Convert window coordinates (pixels, origin top-left) to NDC (-1..1, origin center)
inline Vec2 wnd_to_ndc(double sx, double sy) {
    float x = (float)(sx / g_win_w * 2.0 - 1.0);
    float y = (float)(1.0 - sy / g_win_h * 2.0);
    return { x, y };
}

bool g_mouse_down = false; // is left mouse button held?
std::vector<std::vector<Vec2>> g_strokes; // list of finished strokes
std::vector<Vec2> g_current; // current stroke being recorded

// --- Input callbacks ------------------------------------------------------
// Called when a mouse button is pressed or released
void mouse_button_cb(GLFWwindow* win, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            // start a new stroke
            g_mouse_down = true;
            g_current.clear();
            double mx, my; glfwGetCursorPos(win, &mx, &my);
            g_current.push_back(wnd_to_ndc(mx, my));
        }
        else if (action == GLFW_RELEASE) {
            // finish stroke: push to list of strokes (but only if there's content)
            g_mouse_down = false;
            if (!g_current.empty()) {
                g_strokes.push_back(g_current);
                g_current.clear();
            }
        }
    }
}

// Called whenever the cursor moves
void cursor_pos_cb(GLFWwindow* win, double x, double y) {
    if (g_mouse_down) {
        Vec2 p = wnd_to_ndc(x, y);
        // Avoid adding many nearly-identical points: only push when distance exceeds threshold
        if (g_current.empty()) { g_current.push_back(p); return; }
        Vec2 last = g_current.back();
        float dx = p.x - last.x; float dy = p.y - last.y;
        if (dx * dx + dy * dy > 1e-6f) g_current.push_back(p);
    }
}

// Window resized -> update viewport and stored window size
void framebuffer_size_cb(GLFWwindow* win, int w, int h) {
    g_win_w = w; g_win_h = h;
    glViewport(0, 0, w, h);
}

// --- main -----------------------------------------------------------------
int main() {
    // (1) Initialize GLFW
    if (!glfwInit()) { std::cerr << "Failed to init GLFW\n"; return -1; }

    // Request OpenGL 3.3 core profile (reasonable baseline)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(g_win_w, g_win_h, "OpenGL Pencil", nullptr, nullptr);
    if (!window) { std::cerr << "Failed to create window\n"; glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);

    // 2) Initialize GLAD to load OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n"; return -1;
    }

    // Register callbacks
    glfwSetMouseButtonCallback(window, mouse_button_cb);
    glfwSetCursorPosCallback(window, cursor_pos_cb);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_cb);

    // 3) Create shader program
    GLuint program = create_program();
    GLint color_loc = glGetUniformLocation(program, "uColor");

    // 4) Setup VAO/VBO for streaming vertex data
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // allocate empty buffer (we will update with glBufferData each draw)
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2), (void*)0);
    glBindVertexArray(0);

    // Enable blending for smoother line edges (useful if we later add alpha)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(2.5f); // pencil stroke width (can be changed dynamically)

    // White background (like paper)
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Simple keyboard handling: ESC to close, C to clear canvas
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) { g_strokes.clear(); g_current.clear(); }

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        // dark pencil color (close to black but a bit soft)
        glUniform3f(color_loc, 0.05f, 0.05f, 0.05f);

        glBindVertexArray(vao);

        // Draw all finished strokes. Each stroke is uploaded separately.
        for (const auto& s : g_strokes) {
            if (s.size() < 2) continue; // need at least two points to draw a line
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            // stream stroke points into VBO
            glBufferData(GL_ARRAY_BUFFER, s.size() * sizeof(Vec2), s.data(), GL_DYNAMIC_DRAW);
            // draw as a continuous polyline
            glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)s.size());
        }

        // Draw the currently-being-recorded stroke as well
        if (g_current.size() >= 2) {
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, g_current.size() * sizeof(Vec2), g_current.data(), GL_DYNAMIC_DRAW);
            glDrawArrays(GL_LINE_STRIP, 0, (GLsizei)g_current.size());
        }

        glBindVertexArray(0);
        glUseProgram(0);

        glfwSwapBuffers(window);
    }

    // Cleanup
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);

    glfwTerminate();
    return 0;
}
