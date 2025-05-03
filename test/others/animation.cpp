#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <utility>
#include <iostream>


using namespace std;
class Animations {
public:
    static void drawLoadingScreen(WINDOW* screen) {
        wclear(screen);
        
        vector<string> text = {
            "Generating Levels...", 
            "(This may take a while)"
        };
        
        for (size_t i = 0; i < text.size(); i++) {
            int y, x;
            getmaxyx(screen, y, x);
            mvwaddstr(screen, 
                     y / 2 + i * 2, 
                     (x - text[i].length()) / 2, 
                     text[i].c_str());
            wattron(screen, COLOR_PAIR(6) | A_BOLD);
        }
        wrefresh(screen);
        sleep(1);
    }

    static void drawSavingScreen(WINDOW* screen) {
        wclear(screen);
        string text = "Saving...";
        int y, x;
        getmaxyx(screen, y, x);
        mvwaddstr(screen, y / 2, (x - text.length()) / 2, text.c_str());
        wattron(screen, COLOR_PAIR(6) | A_BOLD);
        wrefresh(screen);
    }

    static void drawIntro(WINDOW* screen) {
        wclear(screen);
        wrefresh(screen);
        drawScrollingText(screen, "Presented by Group M2-2...", make_pair(20, 0));
        usleep(500000);
        
        vector<string> title = {
            " _______ _    _ ______        _               ____  ",
            "|__   __| |  | |  ____|      | |        /\\   |  _ \\ ",
            "   | |  | || | |__         | |       /  \\  | |_) |",
            "   | |  |  __  |  __|        | |      / /\\ \\ |  _ < ",
            "   | |  | |  | | |____       | |____ / ____ \\| |_) |",
            "   ||  ||  |||      |//    \\\\_/ "
        };
        
        int y_max, x_max;
        getmaxyx(screen, y_max, x_max);
        int start_y = (y_max - 10) / 2;
        int x_len = x_max / 2;
        int x_mod = x_max % 2;
        
        WINDOW* title_pad = newpad(9, 52);
        WINDOW* line_pad = newpad(2, x_len * 2 + x_mod);
        waddstr(line_pad, string(x_len * 2 + x_mod, '=').c_str());
        wattr_on(line_pad, COLOR_PAIR(6), nullptr);
        
        for (const auto& row : title) {
            waddstr(title_pad, row.c_str());
            wattr_on(title_pad, COLOR_PAIR(6), nullptr);
        }

        // Draw bars
        for (int i = 0; i < x_len + x_mod; i++) {
            prefresh(line_pad, 0, x_len - (i+1) + x_mod, 
                    start_y, x_len - (i+1) + x_mod, 
                    start_y, x_len + i);
            prefresh(line_pad, 0, x_len - (i+1) + x_mod, 
                    start_y + 9, x_len - (i+1) + x_mod, 
                    start_y + 9, x_len + i);
            usleep(20000);
        }

        WINDOW* logo_subwin = subwin(screen, 8, x_max, start_y + 1, 0);
        
        // Draw scanning effect
        int x_shift = (x_max - 52) / 2;
        for (int i = 0; i < 52/2-2; i++) {
            wclear(logo_subwin);
            wrefresh(logo_subwin);
            prefresh(title_pad, 0, i, start_y + 2, x_shift + i, start_y + 8, x_shift + i);
            prefresh(title_pad, 0, 51 - i, start_y + 2, x_shift + 51 - i, start_y + 8, x_shift + 51 - i);
            usleep(50000);
        }
        
        // Draw logo expansion
        for (int i = 3; i < 52/2; i++) {
            prefresh(title_pad, 0, 52/2 - (i+1), start_y + 2, x_shift + 52/2 - (i+1),
                    start_y + 8, x_shift + 52/2 + i);
            usleep(50000);
        }

        // Draw the whole thing moving to the top
        for (int i = 0; i < start_y + 1; i++) {
            wclear(screen);
            wrefresh(screen);
            prefresh(line_pad, 0, 0, start_y - i, 0, start_y - i, x_max-1);
            prefresh(title_pad, 0, 0, start_y + 2 - i, x_shift, start_y + 8 - i, x_shift + 52);
            prefresh(line_pad, 0, 0, start_y + 9 - i, 0, start_y + 9 - i, x_max-1);
            wrefresh(screen);
            usleep(100000);
        }
        flushinp();
    }

    static void drawGameOverAnimation(WINDOW* screen) {
        init_pair(1, COLOR_RED, COLOR_BLACK);
        vector<string> image = {
            "  ___   __   _  _  ____ ",
            " / __) / _\\ ( \\/ )(  __)",
            "( (_ \\/    \\/ \\/ \\ ) _) ",
            " \\/\\/\\/\\)(/(_)",
            "",
            "  __   _  _  ____  ____ ",
            " /  \\ / )( \\(  __)(  _ \\",
            "(  O )\\ \\/ / ) _)  )   /",
            " \\/  \\/ ()(\\_)"
        };

        WINDOW* pad = newpad(10, 24);
        for (const auto& row : image) {
            waddstr(pad, row.c_str());
            wattron(pad, COLOR_PAIR(1) | A_BOLD);
        }

        // Draw increasing area of the pad from the center
        int y_max, x_max;
        getmaxyx(screen, y_max, x_max);
        for (int i = 0; i < 24/2; i++) {
            prefresh(pad, 0, 24/2 - (i+1), 
                    (y_max-9)/2, x_max/2 - (i+1), 
                    (y_max-9)/2 + 9, x_max/2 + i);
            usleep(100000);
        }
        sleep(1);
        flushinp();
    }

    // static void drawLevelCompleteAnimation(WINDOW* screen, Level* level) {
    //     WINDOW* eraser = newpad(2, 2);
    //     waddstr(eraser, "  ");
    //     wattron(eraser, COLOR_PAIR(1));
        
    //     auto grid_pos = level->path[0];
    //     pair<int, int> pos = {
    //         ((getmaxy(screen) - level->size * 3)/2 + 1 + grid_pos.first * 3),
    //         ((getmaxx(screen) - level->size * 6)/2 + 2 + grid_pos.second * 6)
    //     };
        
    //     prefresh(eraser, 0, 0, pos.first, pos.second, pos.first, pos.second + 1);
        
    //     for (size_t i = 1; i < level->path.size(); i++) {
    //         auto move = Directions({
    //             level->path[i].first - grid_pos.first, 
    //             level->path[i].second - grid_pos.second
    //         });
    //         grid_pos = level->path[i];
            
    //         for (int j = 0; j < 3; j++) {
    //             pos = {pos.first + move.value.first, pos.second + move.value.second * 2};
    //             prefresh(eraser, 0, 0, pos.first, pos.second, pos.first, pos.second + 1);
    //             usleep(50000);
    //         }
    //     }
    //     flushinp();
    // }

    static void drawScrollingText(WINDOW* screen, const string& text, pair<int, int> pos = {-1, -1}, 
                                int updates = 5, bool mirrored = false, bool flush_screen = false, 
                                bool bold = false) {
        int y_max, x_max;
        getmaxyx(screen, y_max, x_max);
        
        if (pos.first == -1 || pos.second == -1) {
            pos = {y_max/2, x_max/2};
        }
        
        if (flush_screen) {
            wclear(screen);
            wrefresh(screen);
        }
        
        WINDOW* pad = newpad(10, 100);
        int start_x = pos.second - text.length()/2;
        waddstr(pad, text.c_str());
        if (bold) {
            wattron(pad, COLOR_PAIR(6) | A_BOLD);
        } else {
            wattron(pad, COLOR_PAIR(6));
        }
        
        for (size_t i = 0; i < text.length(); i++) {
            if (!mirrored) {
                prefresh(pad, 0, 0, pos.first, start_x, pos.first, start_x + i);
            } else {
                prefresh(pad, 0, 0, pos.first, start_x + text.length() - i, 
                        pos.first, start_x + text.length());
            }
            usleep(1000000 / updates);
        }
        
        mvwaddstr(screen, pos.first, pos.second, text.c_str());
        if (bold) {
            wattron(screen, COLOR_PAIR(6) | A_BOLD);
        } else {
            wattron(screen, COLOR_PAIR(6));
        }
        flushinp();
    }

    static void drawLevelCutscene(WINDOW* screen, int level) {
        drawScrollingText(screen, "-- Level " + to_string(level) + " --", make_pair(10, 0), false, true);
        sleep(1);
        wclear(screen);
        wrefresh(screen);
    }
};

int main() {
    // This would be in your main game file
    //draw animation
    initscr();
    start_color();
    init_pair(6, COLOR_WHITE, COLOR_BLACK);
    noecho();
    cbreak();
    curs_set(0);
    Animations::drawIntro(stdscr);
    Animations::drawLoadingScreen(stdscr);
    Animations::drawSavingScreen(stdscr);
    Animations::drawGameOverAnimation(stdscr);
    // Animations::drawLevelCompleteAnimation(stdscr, nullptr);
    Animations::drawLevelCutscene(stdscr, 1);
    Animations::drawScrollingText(stdscr, "Hello World!", make_pair(10, 0), 5, false, true);
    // Animations::drawScrollingText(stdscr, "Hello World!", make_pair(10, 0), 5, true, true);
    // cout << "Please launch the game with the command './game'." << endl;
    return 0;
}