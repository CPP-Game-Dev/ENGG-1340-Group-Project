#include <ncurses.h>
#include <vector>
#include <string>
class ArrowMenu {
private:
    std::vector<std::string> options;
    int selected = 0;
    int x, y; // Position of the menu

public:
    ArrowMenu(const std::vector<std::string>& opts, int pos_x = 0, int pos_y = 0) 
        : options(opts), x(pos_x), y(pos_y) {}

    void display() {
        for (int i = 0; i < options.size(); ++i) {
            move(y + i, x);
            if (i == selected) {
                attron(A_REVERSE); // Highlight selected option
                printw("> %s", options[i].c_str());
                attroff(A_REVERSE);
            } else {
                printw("  %s", options[i].c_str());
            }
        }
        refresh();
    }

    int run() {
        initscr();
        clear();
        noecho();
        cbreak(); // Line buffering disabled
        keypad(stdscr, TRUE); // Enable special keys
        curs_set(0); // Hide cursor

        display();

        while (true) {
            int ch = getch();
            switch (ch) {
                case KEY_UP:
                    if (selected > 0) {
                        selected--;
                        display();
                    }
                    break;
                case KEY_DOWN:
                    if (selected < options.size() - 1) {
                        selected++;
                        display();
                    }
                    break;
                case '\n': // Enter key
                    endwin();
                    return selected;
                case 27: // ESC key
                    endwin();
                    return -1;
            }
        }
    }
};

int main() {
    std::vector<std::string> mainMenuItems= {
        "Start",
        "Help",
        "Exit",
    };
    std::vector<std::string> difficultyMenuItems = {
        "Easy",
        "Medium",
        "Hard",
        "Back"
    };
    ArrowMenu menu(mainMenuItems, 0, 0);
    int choice = menu.run();

    // if (choice >= 0 && choice < menuItems.size()) {
    //     printf("You selected: %s\n", menuItems[choice].c_str());
    // } else {
    //     printf("No selection made or menu canceled.\n");
    // }

    return 0;
}