#include "data.h"

QVBoxLayout* vertical_layout;
QDropList* drop_list;
QDateTimeEdit* expire_date;
QPushButton* emergency_btn;

void toggleEmergencyBtn() {
    if (emergency_btn -> isHidden()) {
        emergency_btn -> show();
        vertical_layout -> addWidget(emergency_btn);
    } else {
        emergency_btn -> hide();
        vertical_layout -> removeWidget(emergency_btn);
    }
}

void onRestrict() {
    if (restrictSelectedProgram()) {
        expire_date -> setDateTime(QDateTime::currentDateTime());
        toggleEmergencyBtn();
    }
}

void onEmergency() {
    emergency();
    toggleEmergencyBtn();
}

int startApp(int argc, char* argv[]) {
    QApplication window(argc, argv);
    window.setApplicationName(("AntiProcrastinationTool v" + VERSION).c_str());

    QWidget app = QWidget();
    app.setMinimumWidth(350);
    app.setFixedHeight(125);

    vertical_layout = new QVBoxLayout(&app);

    drop_list = new QDropList();
    drop_list -> setPlaceholderText("Select the program you want to restrict");

    expire_date = new QDateTimeEdit();
    expire_date -> setDateTime(QDateTime::currentDateTime());

    QPushButton restrict_btn = QPushButton("Restrict");
    restrict_btn.connect(&restrict_btn, &QPushButton::released, &onRestrict);

    emergency_btn = new QPushButton("Emergency");
    emergency_btn -> connect(emergency_btn, &QPushButton::released, &onEmergency);
    emergency_btn -> hide();

    vertical_layout -> addWidget(drop_list);
    vertical_layout -> addWidget(expire_date);
    vertical_layout -> addWidget(&restrict_btn);
    app.show();

    return window.exec();
}