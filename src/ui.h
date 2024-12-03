#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateTimeEdit>
#include <QtWidgets/QPushButton>
#include <QtCore/QString>
// Unfortunately, necessary here
#include "window_manager.h"

class QDropList: public QComboBox {
	public:
		QDropList() : QComboBox() {}

		void showPopup() override {
			updateProgramMapList();
			QComboBox::showPopup();
		}
};

extern QDropList* drop_list;
extern QDateTimeEdit* expire_date;
extern QPushButton* emergency_btn;

extern void toggleEmergencyBtn();
int startApp(int argc, char* argv[]);