#include <Rectangle.hpp>
#include <Text.hpp>
#include <Vector2.hpp>
#include <Window.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <raylib-cpp.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <map>

const std::string filename = "tmp_DMPV_output.raw";
using tips_t = std::map<std::string, float>;

tips_t get_tips() {
    // check if python exists
    std::cout << "checking python3 version...\n";
    int ret = std::system("python3 --version");
    if (ret != 0) {
        std::cout << "python3: bad\n";
        std::cerr << "python3 could not be found, have you installed python?\n";
        std::exit(ret);
    }

    std::cout << "python3: [unknown] ok\n";
    std::system("python3 src/py/main.py"); // [FIXME] DEBUG PATH

    // OKAY WE GOT THE OUTPUT

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "file: tmp_DMPV_output.raw couldn't be found or opened\n";
        std::exit(1);
    }

    std::string line;
    tips_t values;
    while (std::getline(file, line)) {
        std::stringstream ss(line);

        std::string day;
        float tip;
        ss >> day >> tip;

        if (day.starts_with("Sun")) {
            day = "Sunday";
        } else if (day.starts_with("Mon")) {
            day = "Monday";
        } else if (day.starts_with("Tue")) {
            day = "Tuesday";
        } else if (day.starts_with("Wed")) {
            day = "Wednesday";
        } else if (day.starts_with("Thu")) {
            day = "Thursday";
        } else if (day.starts_with("Fri")) {
            day = "Friday";
        } else if (day.starts_with("Sat")) {
            day = "Saturday";
        }

        values[day] = tip;
    }
    file.close();

    std::cout << "ipc: ok\n";

    std::filesystem::remove(filename);

    return values;
}

raylib::Vector2 measure_text(const std::string& text, float font_size, float spacing = 1.0f) {
    Font font = GetFontDefault();
    return MeasureTextEx(font, text.c_str(), font_size, spacing);
}

int main(int argc, char **argv) {
    raylib::Window window(1280, 720, "dmpv", FLAG_MSAA_4X_HINT);
    window.SetTargetFPS(60);
    window.SetExitKey(KEY_NULL);

    window.BeginDrawing();
    window.ClearBackground(WHITE);
    raylib::Vector2 center = window.GetSize() / 2;
    raylib::Text loading_text = {"Loading Data...", 64, BLACK, ::GetFontDefault(), 1};
    loading_text.Draw(center - measure_text(loading_text.text, loading_text.fontSize, 1) / 2);
    window.EndDrawing();

    tips_t tips = get_tips();
    Color colors[] = { RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE, RED, BLUE, GREEN, ORANGE };

    std::cout << "loaded all\n";

    while (!window.ShouldClose()) {
        window.BeginDrawing();
        window.ClearBackground(BLACK);
        {
            raylib::Vector2 center = window.GetSize() / 2;

            float total = 0;
            for (auto &[k, v] : tips) {
                total += v;
            }

            float start = 0.f;

            int i = 0;
            std::vector<std::string> labels;
            for (auto &[k, v] : tips) {
                labels.push_back(k);
                float angle = (tips[k] / total) * 360.f;

                DrawCircleSector(center, 216, start, start + angle, 
                        256, colors[i]);

                ++i;

                start += angle;
            }

            raylib::Rectangle legend(10, 10, 216, 33 * tips.size());
            legend.DrawRounded(0.1, 32, WHITE);
            float startx = legend.x + 10;
            float starty = legend.y + 10;
            i = 0;
            for (auto &l : labels) {
                raylib::Rectangle color = {startx, starty, 20, 20};
                color.DrawRounded(1, 32, colors[i]);
                DrawText(l.c_str(), color.x + color.width + 10, color.y, 24, BLACK);
                int pct = static_cast<int>((tips[l] / total) * 100.f);
                std::string str = std::to_string(pct) + "%";
                raylib::Text text(str.c_str(), 24, BLACK, ::GetFontDefault(), 1);
                text.Draw(legend.x + legend.width - measure_text(str.c_str(), 24).x - 10, color.y);
                ++i;

                starty += color.height + 10;
            }

            DrawFPS(10, 694);
        }
        window.EndDrawing();
    }

    window.Close();
    return 0;
}
