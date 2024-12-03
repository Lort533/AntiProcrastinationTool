#include "data.h"

// This class should be in a header file, doesn't work, can't be asked to do that now.
class RestrictedProgram {
    private:
        HWND hwnd;
        HWINEVENTHOOK event_hook;
        qint64 end_timestamp;
    public:
        RestrictedProgram(HWND hwnd, HWINEVENTHOOK event_hook, qint64 end_timestamp) {
            this -> hwnd = hwnd;
            this -> event_hook = event_hook;
            this -> end_timestamp = end_timestamp;
        }

        HWND getHwnd() {
            return hwnd;
        }

        HWINEVENTHOOK getEventHook() {
            return event_hook;
        }

        qint64 getEndTimestamp() {
            return end_timestamp;
        }
};

qint64 unique_id = 1;
std::map<qint64, HWND> program_list {};
std::vector<RestrictedProgram> restricted_programs {};

static BOOL CALLBACK update_program_list(HWND hwnd, LPARAM l_param) {
    if (!IsWindowVisible(hwnd) || !IsWindowEnabled(hwnd)) {
        return true;
    }

    DWORD process_id;
    GetWindowThreadProcessId(hwnd, &process_id);

    HANDLE handle = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, process_id);
    
    std::string file_name = get_file_name(handle) + ": \"" + get_window_title(hwnd) + "\" (PID " + to_string(process_id) + ")";
    
    if ((file_name._Starts_with("explorer.exe")) || (file_name._Starts_with("AntiProcrastinationTool.exe"))) {
        return true;
    }
    
    program_list.insert({unique_id, hwnd});
    drop_list -> addItem(file_name.c_str(), unique_id++);

    return true;
}

void win_event_hook(HWINEVENTHOOK h_win_event_hook, DWORD event, HWND hwnd, LONG object_id, LONG child_id, DWORD event_thread_id, DWORD dwms_event_time) {
    for (RestrictedProgram restricted_program : restricted_programs) {
        if (hwnd == restricted_program.getHwnd()) {
            qint64 test = QDateTime::currentDateTime().toSecsSinceEpoch();
            if (restricted_program.getEndTimestamp() > test) {
                ShowWindow(hwnd, SW_MINIMIZE);
            } else {
                if (restricted_programs.size() == 1) {
                    restricted_programs.clear();
                    toggleEmergencyBtn();
                }

                UnhookWinEvent(h_win_event_hook);
            }

            return;
        }
    }
}

void updateProgramMapList() {
    drop_list -> clear();
    EnumWindows(update_program_list, NULL);
}

bool restrictSelectedProgram() {
    int current_index = drop_list -> currentIndex();

    if (current_index == -1) {
        show_error(L"You have to select a program first!");
        return false;
    }

    drop_list -> setCurrentIndex(-1);

    qint64 uuid = drop_list -> itemData(current_index).toInt();
    HWND hwnd = program_list[uuid];

    DWORD process_id;
    if (GetWindowThreadProcessId(hwnd, &process_id) == 0) {
        show_error(L"Selected program is already closed!");
        return false;
    }
    
    DWORD thread_id = GetWindowThreadProcessId(hwnd, NULL);
    if (thread_id == 0) {
        show_error(L"Couldn't get process thread ID.");
        return false;
    }
    
    HWINEVENTHOOK event_hook = SetWinEventHook(EVENT_OBJECT_LOCATIONCHANGE, EVENT_OBJECT_LOCATIONCHANGE, GetModuleHandle(NULL), &win_event_hook, process_id, thread_id, WINEVENT_SKIPOWNPROCESS);
    if (event_hook == 0) {
        show_error(L"Couldn't restrict process.");
        return false;
    }

    RestrictedProgram restricted_program(hwnd, event_hook, (expire_date -> dateTime().toSecsSinceEpoch()));
    restricted_programs.push_back(restricted_program);
    ShowWindow(hwnd, SW_MINIMIZE);

    return true;
}

void emergency() {
    for (RestrictedProgram restricted_program : restricted_programs) {
        UnhookWinEvent(restricted_program.getEventHook());
    }

    restricted_programs.clear();
}