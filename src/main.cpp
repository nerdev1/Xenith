/*
------------------------------------------------------------------------------------------
 File Name        : CS213_A1_Part2_S33_20240355_20240639_20240694.cpp
 Description      : Image filters assignment part 2.
                    15 filters -> (#1 gray scale, #2 black and white, #3 invert, #4 flip, #5 rotate, #6 darken and lighten, #8 Crop, #9 Frame, #10 Edge Detection, #11 Resize, #12 Blur, #13 Wano Sunlight, #14 Old TV, #15 Infrared).
 Section          : S33

 Members          :
 - Amar Ayman Ahmed            (ID: 20240355)    -> (#1 Gray Scale, #4 Merge , #7 Darken And Lighten, #9 Frame, #15 Infrared)
 - Nour Eldin Adel Abdelhamid  (ID: 20240639)    -> (#2 Black And White, #5 Flip, #8 Crop, #11 Resize, #13 Wano Sunlight)
 - Youssef Saleh Abdelaziz     (ID: 20240694)    -> (#3 Invert, #6 Rotate, #10 Edge Detection, #12 Blur, #14 Old TV)  (Did the menu)

 Github repo link : https://github.com/nerdev1/OOP-Assignment-1.git

 Shared doc link  : https://docs.google.com/document/d/1YHjz3mVhxFl6CWGilCxqFjRZy0W5brpxXNafBPqFU88/edit?usp=sharing

 Video link       : https://drive.google.com/file/d/1bSPJvaqwLU3fjhQEBbT-FGl7tdeyRHP0/view?usp=drive_link

 Drive link       : https://drive.google.com/drive/folders/1vvsa0MINioFCfhdAm5TP6xARDonsS4hQ?usp=drive_link
------------------------------------------------------------------------------------------
*/

#include <iostream>
#include <vector>
#include <map>

#include "libs/imgui/imgui.h"
#include "libs/portable_file_dialogs/portable-file-dialogs.h"

#include "libs/imgui/backends/imgui_impl_glfw.h"
#include "libs/imgui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "fonts/Nunito-Bold.hpp"
#include "filters.hpp"

using namespace std;

struct ShowOptions
{
    bool merge = false;
    bool brightness = false;
    bool crop = false;
    bool frame = false;
    bool resize = false;
    bool blur = false;
    bool skew = false;
    bool neon = false;
    bool glowing_edges = false;
    bool sharpness = false;
    bool noise = false;
    bool swirl = false;
} show_options;

bool Ctrl();
bool Shift();
bool Alt();

string get_image_path();
string save_as();

void after_filter_action(vector<Image> &undo, vector<Image> &redo, const Image &image, GLuint &image_texture, int &texture_width, int &texture_height);
void FullWindowImageDisplay(const Image &image, GLuint image_texture, int menu_bar_height);
void LoadTextureFromMemory(const Image &image, GLuint &texture);
void UpdateTexture(const Image &image, GLuint &texture, int &texture_width, int &texture_height);
void render(GLFWwindow *window);

// Program Style
void style()
{
    // Comfortable Dark Cyan style by SouthCraftX from ImThemes
    ImGuiStyle &style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.WindowRounding = 11.5f;
    style.WindowBorderSize = 1.0f;
    style.FrameRounding = 5.0f;
    style.IndentSpacing = 0.0f;
    style.ScrollbarSize = 15.0f;
    style.ScrollbarRounding = 15.9f;
    style.GrabRounding = 10.0f;
    style.ColorButtonPosition = ImGuiDir_Right;

    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.27450982f, 0.31764707f, 0.4509804f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09411765f, 0.101960786f, 0.11764706f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.11372549f, 0.1254902f, 0.15294118f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.12f, 0.12f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.03137255f, 0.9490196f, 0.84313726f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.87843f, 0.3137f, 0.9804f, 1.0f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.22f, 0.22f, 0.27f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15294118f, 0.15294118f, 0.15294118f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.14117648f, 0.16470589f, 0.20784314f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.105882354f, 0.105882354f, 0.105882354f, 1.0f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.14509805f, 0.14509805f, 0.14509805f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.03137255f, 0.9490196f, 0.84313726f, 1.0f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9372549f, 0.9372549f, 0.9372549f, 1.0f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.49803922f, 0.5137255f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26666668f, 0.2901961f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.49803922f, 0.5137255f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.19607843f, 0.1764706f, 0.54509807f, 0.5019608f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.19607843f, 0.1764706f, 0.54509807f, 0.5019608f);
}

// Flags of main windows
const ImGuiWindowFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse;
const ImGuiWindowFlags filters_flags = flags | ImGuiWindowFlags_NoScrollbar;

signed main()
{
    // Initialize GLFW (window + input)
    if (!glfwInit())
        return 1;

    // Starting glfw window and maximizing it
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
    GLFWwindow *window = glfwCreateWindow(1280, 700, "Xenith", NULL, NULL);
    glfwMaximizeWindow(window);
    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1);

    // Adding icon to the window
    Image icon_image("misc/icon/icon.png");
    GLFWimage icon = {icon_image.width, icon_image.height, icon_image.imageData};
    glfwSetWindowIcon(window, 1, &icon);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    // Initialize platform/renderer bachends
    ImGui_ImplGlfw_InitForOpenGL(window, true);

    const char *glsl_version =
#ifdef __APPLE__
        "#version 150";
#else
        "#version 130";
#endif

    ImGui_ImplOpenGL3_Init(glsl_version);

    // Custom font
    const float font_size = 24.0f;
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(NunitoBold_compressed_data_base85, font_size);

    // Image variables
    string image_path = "";
    Image image;
    GLuint image_texture = 0;
    int texture_width = 0;
    int texture_height = 0;
    Image original_image;
    GLuint original_image_texture = 0;
    float zoom_factor = 1;
    bool update_image = false;

    // Opening the main windows if image is loaded successfully
    bool image_loaded = false;

    // Showing a window of sub-options for the filters that has one if its button was clicked
    bool filter_options = false;

    // Filters options variables
    const char *merge_combo[] = {"Pick a merge option", "Merge common area", "Resize bigger image to smaller dimensions", "Resize smaller image to bigger dimensions"};
    const char *neon_combo[] = {"Original Image", "Standard", "Xenon", "Galaxy"};
    const char *glowing_combo[] = {"Original Image", "1", "2"};
    int merge_combo_current;
    int neon_combo_current;
    int glowing_combo_current;
    int merge_percent;
    int glowing_edge_percent;
    int glowing_original_percent;
    int brightness_percent;
    int x_crop, y_crop, width_crop, height_crop;
    int width_resize, height_resize;
    int frame_size;
    int frame_option;
    ImVec4 frame_color;
    int blur_level;
    int skew_angle;
    int sharpness_strength;
    int noise_factor;
    int swirl_factor;

    // Undo and Redo vectors
    vector<Image> undo;
    vector<Image> redo;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Update input (keyboard, mouse...)
        glfwPollEvents();

        // Start new frame
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        // Style
        style();

        // ------- Keyboard Shorcuts -------
        if (Ctrl() and ImGui::IsKeyPressed(ImGuiKey_O))
        {
            image_path = get_image_path();

            if (image_path != "")
            {
                if (image.loadNewImage(image_path))
                {
                    image_loaded = true;

                    LoadTextureFromMemory(image, image_texture);
                    texture_width = image.width;
                    texture_height = image.height;

                    original_image = image;
                    LoadTextureFromMemory(original_image, original_image_texture);

                    undo.clear();
                    redo.clear();
                    undo.push_back(image);
                }
            }
        }
        if (Ctrl() and ImGui::IsKeyPressed(ImGuiKey_Q))
        {
            break;
        }
        if (image_loaded and !Alt())
        {
            if (Ctrl() and Shift() and ImGui::IsKeyPressed(ImGuiKey_S))
            {
                if (image_loaded)
                {
                    string save_as_image_path = save_as();

                    if (save_as_image_path != "")
                    {
                        if (image.saveImage(save_as_image_path))
                            pfd::message("Done", "Image saved successfully", pfd::choice::ok, pfd::icon::info);
                    }
                }
            }
            else if (Ctrl() and ImGui::IsKeyPressed(ImGuiKey_S))
            {
                if (image_loaded)
                {
                    if (image.saveImage(image_path))
                        pfd::message("Done", "Image saved successfully", pfd::choice::ok, pfd::icon::info);
                }
            }
            if (Ctrl() and ImGui::IsKeyPressed(ImGuiKey_Z) and undo.size() > 1)
            {
                redo.push_back(undo.back());
                undo.pop_back();
                image = undo.back();
                UpdateTexture(image, image_texture, texture_width, texture_height);
            }
            if (Ctrl() and ImGui::IsKeyPressed(ImGuiKey_Y) and redo.size() > 0)
            {
                undo.push_back(redo.back());
                redo.pop_back();
                image = undo.back();
                UpdateTexture(image, image_texture, texture_width, texture_height);
            }
            if (Ctrl() and ImGui::IsKeyPressed(ImGuiKey_Equal))
            {
                zoom_factor = min(zoom_factor * 1.05f, 8.0f);
            }
            if (Ctrl() and ImGui::IsKeyPressed(ImGuiKey_Minus))
            {
                zoom_factor = max(zoom_factor / 1.05f, 0.125f);
            }
            if (Ctrl() and io.MouseWheel > 0)
            {
                zoom_factor = min(zoom_factor * 1.05f, 8.0f);
            }
            if (Ctrl() and io.MouseWheel < 0)
            {
                zoom_factor = max(zoom_factor / 1.05f, 0.125f);
            }
        }

        // ------- Building the gui each frame -------

        // The main menu bar and its keyboard shortcuts
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open File", "Ctrl + O"))
                {
                    image_path = get_image_path();

                    if (image_path != "")
                    {
                        if (image.loadNewImage(image_path))
                        {
                            image_loaded = true;

                            LoadTextureFromMemory(image, image_texture);
                            texture_width = image.width;
                            texture_height = image.height;

                            original_image = image;
                            LoadTextureFromMemory(original_image, original_image_texture);

                            undo.clear();
                            redo.clear();
                            undo.push_back(image);
                        }
                    }
                }

                ImGui::Separator();
                if ((ImGui::MenuItem("Save", "Ctrl + S")) and image_path != "")
                {
                    if (image_loaded)
                    {
                        if (image.saveImage(image_path))
                            pfd::message("Done", "Image saved successfully", pfd::choice::ok, pfd::icon::info);
                    }
                }

                ImGui::Separator();
                if (ImGui::MenuItem("Save as", "Ctrl + Shift + S") and image_path != "")
                {
                    if (image_loaded)
                    {
                        string save_as_image_path = save_as();

                        if (save_as_image_path != "")
                        {
                            if (image.saveImage(save_as_image_path))
                                pfd::message("Done", "Image saved successfully", pfd::choice::ok, pfd::icon::info);
                        }
                    }
                }

                ImGui::Separator();
                if (ImGui::MenuItem("Exit", "Ctrl + Q"))
                {
                    break;
                }

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        float menu_bar_height = ImGui::GetFrameHeight();

        // Filter and image display window open if image file is opened otherwise open a blank window with a message
        if (!Alt())
        {
            if (image_loaded)
            {
                // Setting up size and position of images window
                ImGui::SetNextWindowPos(ImVec2(0, menu_bar_height));
                ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, (io.DisplaySize.y - menu_bar_height) * 0.78));

                // Images window
                if (ImGui::Begin("Image Display", NULL, flags | ImGuiWindowFlags_HorizontalScrollbar))
                {
                    // Display image on imgui image window
                    ImVec2 display_size;
                    ImVec2 avail_size = ImGui::GetContentRegionAvail();
                    float image_aspect = (float)texture_width / texture_height;
                    float window_aspect = avail_size.x / avail_size.y;

                    if (window_aspect > image_aspect)
                    {
                        display_size = {avail_size.y * image_aspect, avail_size.y};
                    }
                    else
                    {
                        display_size = {avail_size.x, avail_size.x / image_aspect};
                    }

                    display_size.x *= zoom_factor;
                    display_size.y *= zoom_factor;

                    ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - display_size.x / 2, ImGui::GetWindowHeight() / 2 - display_size.y / 2));

                    ImGui::Image((ImTextureID)(intptr_t)image_texture, display_size);

                    ImGui::End();
                }

                // Setting up size and position of filters window
                ImGui::SetNextWindowPos(ImVec2(0, (io.DisplaySize.y - menu_bar_height) * 0.78 + menu_bar_height));
                ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, (io.DisplaySize.y - menu_bar_height) * 0.22));

                // Filters window
                if (!filter_options)
                {
                    // Reseting Filters options variables
                    merge_combo_current = 0;
                    neon_combo_current = 0;
                    glowing_combo_current = 0;
                    merge_percent = 0;
                    glowing_edge_percent = 75;
                    glowing_original_percent = 68;
                    brightness_percent = 100;
                    x_crop = 0, y_crop = 0, width_crop = image.width, height_crop = image.height;
                    width_resize = image.width, height_resize = image.height;
                    frame_option = 0;
                    frame_size = 0;
                    blur_level = 0;
                    skew_angle = 0;
                    sharpness_strength = 0;
                    frame_color = {0.0f, 0.0f, 0.0f, 1.0f};
                    noise_factor = 0;
                    swirl_factor = 0;

                    if (ImGui::Begin("Filters", NULL, filters_flags))
                    {
                        ImGui::GetStyle().ItemSpacing.x = 5;
                        ImGui::GetStyle().ItemSpacing.y = 5;

                        // Filters and undo buttons variables
                        int total_filter_buttons = 27;
                        int buttons_per_row = 9;

                        ImVec2 available_width = ImGui::GetContentRegionAvail();
                        float spacing = ImGui::GetStyle().ItemSpacing.x;
                        float button_width = (available_width.x - (spacing * (buttons_per_row - 1))) / buttons_per_row;
                        float button_height = 43.0f;
                        ImVec2 undo_button_size = {100, 30};

                        // Image size text format
                        char format[50];
                        sprintf(format, "Image Size = %d x %d", image.width, image.height);
                        const char *image_size = format;

                        // Displaying image size on the top of filters window
                        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetStyle().WindowPadding.y - 3 * button_height - 20 - undo_button_size.y - ImGui::CalcTextSize(image_size).y);

                        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - ImGui::CalcTextSize(image_size).x / 2);
                        ImGui::Text("%s", image_size);

                        // Undo, Redo and clear buttons

                        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - undo_button_size.x * 4) / 2 - 8);

                        ImGui::BeginDisabled(undo.size() < 2);
                        if (ImGui::Button("Undo", undo_button_size))
                        {
                            redo.push_back(undo.back());
                            undo.pop_back();
                            image = undo.back();
                            UpdateTexture(image, image_texture, texture_width, texture_height);
                        }
                        ImGui::EndDisabled();

                        ImGui::SameLine();
                        ImGui::BeginDisabled(redo.empty());
                        if (ImGui::Button("Redo", undo_button_size))
                        {
                            undo.push_back(redo.back());
                            redo.pop_back();
                            image = undo.back();
                            UpdateTexture(image, image_texture, texture_width, texture_height);
                        }
                        ImGui::EndDisabled();

                        ImGui::SameLine();
                        ImGui::BeginDisabled(undo.size() < 2 and redo.empty());
                        if (ImGui::Button("Clear", undo_button_size))
                        {
                            image = undo[0];
                            undo.clear();
                            redo.clear();
                            undo.push_back(image);
                            UpdateTexture(image, image_texture, texture_width, texture_height);
                        }
                        ImGui::EndDisabled();

                        ImGui::SameLine();
                        ImGui::BeginDisabled(zoom_factor == 1);
                        if (ImGui::Button("Align", undo_button_size))
                        {
                            zoom_factor = 1;
                        }
                        ImGui::EndDisabled();

                        // Filters buttons and their effects
                        ImGui::SetCursorPosY(ImGui::GetWindowHeight() - ImGui::GetStyle().WindowPadding.y - 3 * button_height - 10);

                        for (int i = 0; i < total_filter_buttons; i++)
                        {
                            switch (i)
                            {
                            case 0:
                                if (ImGui::Button("Gray Scale", ImVec2(button_width, button_height)))
                                {
                                    gray_scale(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 1:
                                if (ImGui::Button("Black & White", ImVec2(button_width, button_height)))
                                {
                                    black_and_white(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 2:
                                if (ImGui::Button("Invert", ImVec2(button_width, button_height)))
                                {
                                    invert(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 3:
                                if (ImGui::Button("Merge", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.merge = true;
                                }
                                break;

                            case 4:
                                if (ImGui::Button("Vertical Flip", ImVec2(button_width, button_height)))
                                {
                                    vertical_flip(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 5:
                                if (ImGui::Button("Horizontal Flip", ImVec2(button_width, button_height)))
                                {
                                    horizontal_flip(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 6:
                                if (ImGui::Button("Rotate", ImVec2(button_width, button_height)))
                                {
                                    rotate(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 7:
                                if (ImGui::Button("Brightness", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.brightness = true;
                                }
                                break;

                            case 8:
                                if (ImGui::Button("Crop", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.crop = true;
                                }
                                break;

                            case 9:
                                if (ImGui::Button("Frame", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.frame = true;
                                }
                                break;

                            case 10:
                                if (ImGui::Button("Detect Edges", ImVec2(button_width, button_height)))
                                {
                                    edge_detection(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 11:
                                if (ImGui::Button("Resize", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.resize = true;
                                }
                                break;

                            case 12:
                                if (ImGui::Button("Blur", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.blur = true;
                                }
                                break;

                            case 13:
                                if (ImGui::Button("Sunlight", ImVec2(button_width, button_height)))
                                {
                                    yellow(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 14:
                                if (ImGui::Button("Oil Paint", ImVec2(button_width, button_height)))
                                {
                                    oil_paint(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 15:
                                if (ImGui::Button("Retro", ImVec2(button_width, button_height)))
                                {
                                    retro(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 16:
                                if (ImGui::Button("Purple", ImVec2(button_width, button_height)))
                                {
                                    purple(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 17:
                                if (ImGui::Button("Infrared", ImVec2(button_width, button_height)))
                                {
                                    infrared(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 18:
                                if (ImGui::Button("Skew", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.skew = true;
                                }
                                break;

                            case 19:
                                if (ImGui::Button("Emboss", ImVec2(button_width, button_height)))
                                {
                                    emboss(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 20:
                                if (ImGui::Button("Neon", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.neon = true;
                                }
                                break;

                            case 21:
                                if (ImGui::Button("Glowing edges", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.glowing_edges = true;
                                }
                                break;

                            case 22:
                                if (ImGui::Button("Fire", ImVec2(button_width, button_height)))
                                {
                                    fire(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 23:
                                if (ImGui::Button("Sepia", ImVec2(button_width, button_height)))
                                {
                                    sepia(image);
                                    after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);
                                }
                                break;

                            case 24:
                                if (ImGui::Button("Sharpness", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.sharpness = true;
                                }
                                break;

                            case 25:
                                if (ImGui::Button("Noise", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.noise = true;
                                }
                                break;

                            case 26:
                                if (ImGui::Button("Swirl", ImVec2(button_width, button_height)))
                                {
                                    filter_options = true;
                                    show_options.swirl = true;
                                }
                                break;
                            }

                            // Put buttons on the same line except after every row
                            if ((i + 1) % buttons_per_row != 0)
                                ImGui::SameLine();
                        }

                        ImGui::End();
                    }
                }
                else
                {
                    if (ImGui::Begin("Filters Options", NULL, filters_flags))
                    {
                        image = undo.back();
                        update_image = false;

                        ImGui::GetStyle().ItemSpacing.x = 10;
                        ImGui::GetStyle().ItemSpacing.y = 10;

                        int slider_width = 500;
                        ImVec2 apply_button_size = {150, 40};

                        ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - slider_width / 2);
                        ImGui::SetNextItemWidth(slider_width);

                        if (show_options.merge == true)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

                            ImGui::Combo("##merge_combo", &merge_combo_current, merge_combo, IM_ARRAYSIZE(merge_combo));

                            ImGui::SetNextItemWidth(slider_width);

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - slider_width / 2);
                            ImGui::SliderInt("Merge Percentage", &merge_percent, 0, 100);

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.merge = false;
                            }

                            ImGui::SameLine();
                            ImGui::BeginDisabled(merge_combo_current == 0);
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                Image merge_image;
                                string merge_image_path = get_image_path();

                                if (merge_image_path != "")
                                {
                                    if (merge_image.loadNewImage(merge_image_path))
                                    {
                                        merge(image, merge_image, merge_combo_current, merge_percent);
                                        after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                        filter_options = false;
                                        show_options.merge = false;
                                    }
                                }
                            }
                            ImGui::EndDisabled();
                        }
                        else if (show_options.brightness == true)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);

                            if (ImGui::SliderInt("Brightness Percentage", &brightness_percent, 0, 200))
                                update_image = true;

                            if (update_image)
                            {
                                brightness(image, brightness_percent);
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.brightness = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                brightness(image, brightness_percent);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.brightness = false;
                            }
                        }
                        else if (show_options.crop == true)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);

                            width_crop = min(width_crop, image.width - x_crop - 1);
                            height_crop = min(height_crop, image.height - y_crop - 1);
                            x_crop = min(x_crop, image.width - width_crop - 1);
                            y_crop = min(y_crop, image.height - height_crop - 1);

                            ImGui::PushItemWidth(slider_width);

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - slider_width - 25);
                            if (ImGui::SliderInt("Width", &width_crop, 0, image.width - x_crop - 1))
                                update_image = true;

                            ImGui::SameLine();
                            if (ImGui::SliderInt("Height", &height_crop, 0, image.height - y_crop - 1))
                                update_image = true;

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - (slider_width + 5));

                            if (ImGui::SliderInt("X", &x_crop, 0, image.width - width_crop - 1))
                                update_image = true;

                            ImGui::SameLine();
                            if (ImGui::SliderInt("Y", &y_crop, 0, image.height - height_crop - 1))
                                update_image = true;

                            ImGui::PopItemWidth();

                            if (update_image)
                            {
                                crop_darken_and_lighten(image, x_crop, y_crop, width_crop, height_crop);
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.crop = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                crop(image, x_crop, y_crop, width_crop, height_crop);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.crop = false;
                            }
                        }
                        else if (show_options.frame == true)
                        {
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 335);
                            ImGui::SetNextItemWidth(235);

                            if (ImGui::ColorPicker3("##Frame Color", (float *)&frame_color, ImGuiColorEditFlags_NoInputs))
                                update_image = true;

                            ImGui::SetNextItemWidth(slider_width);

                            ImGui::SameLine();
                            if (ImGui::SliderInt("Frame Size", &frame_size, 0, 100))
                                update_image = true;

                            int slider_height = ImGui::GetItemRectSize().y;

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - slider_width / 2 + 70);
                            ImGui::SetCursorPosY(slider_height + ImGui::GetStyle().WindowPadding.y + 10);

                            if (ImGui::RadioButton("Simple", &frame_option, 0))
                                update_image = true;

                            ImGui::SameLine();
                            if (ImGui::RadioButton("Decorated 1", &frame_option, 1))
                                update_image = true;

                            ImGui::SameLine();
                            if (ImGui::RadioButton("Decorated 2", &frame_option, 2))
                                update_image = true;

                            if (update_image)
                            {
                                frame(image, frame_option, frame_size, frame_color.x * 255.0f, frame_color.y * 255.0f, frame_color.z * 255.0f);
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);
                            ImGui::SetCursorPosY(slider_height + apply_button_size.y + ImGui::GetStyle().WindowPadding.y + 20);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.frame = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                frame(image, frame_option, frame_size, frame_color.x * 255.0f, frame_color.y * 255.0f, frame_color.z * 255.0f);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.frame = false;
                            }
                        }
                        else if (show_options.resize == true)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);

                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - slider_width / 2 - 5);
                            ImGui::PushItemWidth(slider_width);

                            if (ImGui::SliderInt("Width", &width_resize, 1, image.width * 3))
                                update_image = true;

                            ImGui::SameLine();
                            if (ImGui::SliderInt("Height", &height_resize, 1, image.height * 3))
                                update_image = true;

                            ImGui::PopItemWidth();

                            if (update_image)
                            {
                                resize(image, width_resize, height_resize);
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.resize = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                resize(image, width_resize, height_resize);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.resize = false;
                            }
                        }
                        else if (show_options.blur == true)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);

                            if (ImGui::SliderInt("Blur level", &blur_level, 0, 20))
                                update_image = true;

                            if (update_image)
                            {
                                blur(image, blur_level);
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.blur = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                blur(image, blur_level);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.blur = false;
                            }
                        }
                        else if (show_options.skew == true)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);

                            if (ImGui::SliderInt("Angle", &skew_angle, -80, 80))
                                update_image = true;

                            if (update_image)
                            {
                                skew(image, skew_angle);
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.skew = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                skew(image, skew_angle);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.skew = false;
                            }
                        }
                        else if (show_options.neon == true)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

                            if (ImGui::Combo("##neon_combo", &neon_combo_current, neon_combo, IM_ARRAYSIZE(neon_combo)))
                                update_image = true;

                            if (update_image)
                            {
                                if (neon_combo_current != 0)
                                {
                                    neon(image, neon_combo_current);
                                    crop(image, 1, 1, image.width - 2, image.height - 2);
                                }
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.neon = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            ImGui::BeginDisabled(neon_combo_current == 0);
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                neon(image, neon_combo_current);
                                crop(image, 1, 1, image.width - 2, image.height - 2);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.neon = false;
                            }
                            ImGui::EndDisabled();
                        }
                        else if (show_options.glowing_edges == true)
                        {
                            ImGui::PushItemWidth(slider_width);

                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

                            if (ImGui::Combo("##glowing_combo", &glowing_combo_current, glowing_combo, IM_ARRAYSIZE(glowing_combo)))
                                update_image = true;

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - slider_width / 2);

                            ImGui::BeginDisabled(glowing_combo_current == 0);
                            if (ImGui::SliderInt("Edge percentage", &glowing_edge_percent, 0, 100))
                                update_image = true;

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - slider_width / 2);

                            if (ImGui::SliderInt("Original percentage", &glowing_original_percent, 0, 100))
                                update_image = true;
                            ImGui::EndDisabled();

                            ImGui::PopItemWidth();

                            if (update_image)
                            {
                                if (glowing_combo_current != 0)
                                    glowing_edges(image, glowing_combo_current, glowing_edge_percent, glowing_original_percent);

                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.glowing_edges = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            ImGui::BeginDisabled(glowing_combo_current == 0);
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                glowing_edges(image, glowing_combo_current, glowing_edge_percent, glowing_original_percent);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.glowing_edges = false;
                            }
                            ImGui::EndDisabled();
                        }
                        else if (show_options.sharpness == true)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);

                            if (ImGui::SliderInt("Strength", &sharpness_strength, 0, 100))
                                update_image = true;

                            if (update_image)
                            {
                                sharpness(image, sharpness_strength);
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.sharpness = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                sharpness(image, sharpness_strength);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.sharpness = false;
                            }
                        }
                        else if (show_options.noise == true)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);

                            if (ImGui::SliderInt("Strength", &noise_factor, 0, 100))
                                update_image = true;

                            if (update_image)
                            {
                                noise(image, noise_factor);
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.noise = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                noise(image, noise_factor);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.noise = false;
                            }
                        }
                        else if (show_options.swirl == true)
                        {
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);

                            if (ImGui::SliderInt("Swirl Factor", &swirl_factor, -20, 20))
                                update_image = true;

                            if (update_image)
                            {
                                swirl(image, swirl_factor);
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SetCursorPosX(ImGui::GetWindowWidth() / 2 - apply_button_size.x - 5);

                            if (ImGui::Button("Back", apply_button_size))
                            {
                                filter_options = false;
                                show_options.swirl = false;
                                UpdateTexture(image, image_texture, texture_width, texture_height);
                            }

                            ImGui::SameLine();
                            if (ImGui::Button("Apply", apply_button_size))
                            {
                                swirl(image, swirl_factor);
                                after_filter_action(undo, redo, image, image_texture, texture_width, texture_height);

                                filter_options = false;
                                show_options.swirl = false;
                            }
                        }

                        ImGui::End();
                    }
                }
            }
            else
            {
                // Blank window if no file is opened yet
                ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, io.DisplaySize.y - menu_bar_height));
                ImGui::SetNextWindowPos(ImVec2(0, menu_bar_height));

                if (ImGui::Begin("Blank", NULL, flags))
                {
                    const char *message = "Please click on \"File\" above or use \"Ctrl + O\" on your keyboard to open an image and start editing it";
                    ImGui::SetWindowFontScale(1.5);
                    ImGui::SetCursorPos(ImVec2(io.DisplaySize.x / 2 - ImGui::CalcTextSize(message).x / 2,
                                               io.DisplaySize.y / 2 - menu_bar_height - ImGui::CalcTextSize(message).y / 2));
                    ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.0f, 1.0f), "%s", message);

                    ImGui::End();
                }
            }
        }
        else if (image_loaded and Alt() and Shift())
        {
            FullWindowImageDisplay(image, image_texture, menu_bar_height);
        }
        else if (image_loaded and Alt())
        {
            FullWindowImageDisplay(original_image, original_image_texture, menu_bar_height);
        }

        // ------- Rendering -------
        render(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

// Checks if control, shift or alt keys is held down
bool Ctrl() { return ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl); }
bool Shift() { return ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift); }
bool Alt() { return ImGui::IsKeyDown(ImGuiKey_LeftAlt) || ImGui::IsKeyDown(ImGuiKey_RightAlt); };

// Getting image path from portable file dialogs
string get_image_path()
{
    while (true)
    { // Open an image file using pfd
        auto result = pfd::open_file("Select an image file", ".",
                                     {"Image Files", "*.png *.jpg *.jpeg *.bmp *.tga"})
                          .result();

        if (result.empty())
            return "";

        string image_path = result[0];

        // Getting the extension
        string ext;
        size_t dot_pos = image_path.find_last_of('.');

        if (dot_pos != string::npos)
            ext = image_path.substr(dot_pos + 1);

        // Turning the extension to lower case
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        // Checking if the extension is supported
        if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "bmp" || ext == "tga")
            return image_path;

        pfd::message("Unsupported file extension",
                     "Please select an image file with one of the following extensions (.png, .jpg, .jpeg, .bmp, .tga)",
                     pfd::choice::ok,
                     pfd::icon::error)
            .result();
    }
}

// Save image with different path
string save_as()
{
    while (true)
    {
        auto image_path = pfd::save_file("Enter file name with one of the supported extensions", ".",
                                         {"Image Files", "*.png *.jpg *.jpeg *.bmp *.tga"})
                              .result();

        if (image_path.empty())
            return "";

        string ext;
        size_t dot_pos = image_path.find_last_of('.');

        if (dot_pos != string::npos)
            ext = image_path.substr(dot_pos + 1);

        // Turning the extension to lower case
        transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

        // Checking if the extension is supported
        if (ext == "png" || ext == "jpg" || ext == "jpeg" || ext == "bmp" || ext == "tga")
            return image_path;

        pfd::message("Unsupported file extension",
                     "Please use one of these extensions (.png, .jpg, .jpeg, .bmp, .tga)",
                     pfd::choice::ok,
                     pfd::icon::error)
            .result();
    }
}

// Display any image alone on the entire window
void FullWindowImageDisplay(const Image &image, GLuint image_texture, int menu_bar_height)
{
    // Setting up size and position of images window
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - menu_bar_height));
    ImGui::SetNextWindowPos(ImVec2(0, menu_bar_height));

    // Original image Display window
    if (ImGui::Begin("Original Image Display", NULL, flags | ImGuiWindowFlags_HorizontalScrollbar))
    {
        // Display image on imgui original image display window
        ImVec2 display_size;
        ImVec2 avail_size = ImGui::GetContentRegionAvail();
        float image_aspect = (float)image.width / image.height;
        float window_aspect = avail_size.x / avail_size.y;

        if (window_aspect > image_aspect)
        {
            display_size = {avail_size.y * image_aspect, avail_size.y};
        }
        else
        {
            display_size = {avail_size.x, avail_size.x / image_aspect};
        }

        ImGui::SetCursorPos(ImVec2(ImGui::GetWindowWidth() / 2 - display_size.x / 2, ImGui::GetWindowHeight() / 2 - display_size.y / 2));

        ImGui::Image((ImTextureID)(intptr_t)image_texture, display_size);

        ImGui::End();
    }
}

// After applying a filter update the image texture, the undo vector and clear the redo vector if not empty
void after_filter_action(vector<Image> &undo, vector<Image> &redo, const Image &image, GLuint &image_texture, int &texture_width, int &texture_height)
{
    UpdateTexture(image, image_texture, texture_width, texture_height);
    undo.push_back(image);
    redo.clear();
}

// Create a new texture and load an image to it
void LoadTextureFromMemory(const Image &image, GLuint &texture)
{
    if (texture != 0)
        glDeleteTextures(1, &texture);

    unsigned char *image_data = image.imageData;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload pixels into texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.width, image.height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);

    texture = image_texture;
}

// Update existing texture
void UpdateTexture(const Image &image, GLuint &texture, int &texture_width, int &texture_height)
{
    if (texture == 0 or texture_width != image.width or texture_height != image.height)
    {
        LoadTextureFromMemory(image, texture);
        texture_width = image.width;
        texture_height = image.height;
        return;
    }

    unsigned char *image_data = image.imageData;

    glBindTexture(GL_TEXTURE_2D, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image.width, image.height, GL_RGB, GL_UNSIGNED_BYTE, image_data);
}

// Rendering each frame update
void render(GLFWwindow *window)
{
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}
