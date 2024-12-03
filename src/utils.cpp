#include "data.h"

// Is there really no better way of achieving something this simple?
std::string to_string(int input) {
    std::string output = "";

    do {
        output = (char)(48 + (input % 10)) + output;
        input /= 10;
    } while (input > 0);

    return output;
}

std::string get_file_name(HANDLE handle) {
    TCHAR raw_path[32767]{};
    GetModuleFileNameEx(handle, NULL, raw_path, 32767);

    std::string path = "";
    for (TCHAR character : raw_path) {
        if (((char)character) == '\0') {
            break;
        }

        path += character;
    }

    std::string file_name = "";

    // NEEDS FIX: Null terminator in the end of the path.
    for (int i = path.length() - 1; i >= 0; i--) {
        if (path[i] == '\\') {
            break;
        }

        file_name = path[i] + file_name;
    }

    return file_name;
}

std::string get_window_title(HWND hwnd) {
    TCHAR raw_path[256]{};
    GetWindowText(hwnd, raw_path, 256);

    // Repetitive code comparing to above, but iirc
    // there are issues with making this a function.
    std::string title = "";
    for (TCHAR character : raw_path) {
        if (((char)character) == '\0') {
            break;
        }

        title += character;
    }

    return title;
}

void show_error(LPCTSTR text) {
    MessageBox(NULL, text, NULL, NULL);
}