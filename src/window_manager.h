#include <string>
#include <Windows.h>
#include <Psapi.h>
#include <WinUser.h>

class RestrictedProgram;

extern std::map<qint64, HWND> program_list;
extern std::vector<RestrictedProgram> restricted_programs;

void updateProgramMapList();
bool restrictSelectedProgram();
void emergency();