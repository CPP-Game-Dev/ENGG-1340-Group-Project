#include <ncurses.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <utility>
#include <iostream>


using namespace std;
class Animations {
public:

    static void drawIntro(WINDOW* screen) {
        wclear(screen);
        wrefresh(screen);
        drawScrollingText(screen, "Presented by Group 137...                   ", make_pair(20, 0));
        
        vector<string> title = {
            "       HH   HH  EEEEEEE  LL       LL       \n",
            "       HH   HH  EE       LL       LL       \n",
            "       HHHHHHH  EEEEE    LL       LL       \n",
            "       HH   HH  EE       LL       LL       \n",
            "       HH   HH  EEEEEEE  LLLLLLL  LLLLLLL  \n",
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
        flushinp();
    }


    static void drawScrollingText(WINDOW* screen, const string& text, pair<int, int> pos = {-1, -1}, 
                                int updates = 10, bool mirrored = false, bool flush_screen = false, 
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

};

int main() {
    // This would be in your main game file
    //draw animation
    initscr();
    Animations::drawIntro(stdscr);

    return 0;
}