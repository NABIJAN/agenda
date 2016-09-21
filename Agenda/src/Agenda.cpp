#include "AgendaUI.hpp"
#include "AgendaService.hpp"
#include "Storage.hpp"
#include "Meeting.hpp"
#include "Date.hpp"
#include "User.hpp"

AgendaUI aui;

int main() {
    aui.OperationLoop();
    return 0;
}
