#include "mainwindow.hpp"

enum EXIT_CODES {
  NORMAL = 0,
  RESTART = 250
};

QVector<QString> dates;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentToggleNew(false), currentToggleAdd(false),
      currentToggleView(false) {
  ui->setupUi(this);
  ReadWrite::read(eventList);
  ui->calendarWidget->setMinimumDate(QDate::currentDate());
}

MainWindow::~MainWindow() {
  foreach (Event e, eventList) { ReadWrite::write(e); }
  delete ui;
}

void MainWindow::on_btnNew_clicked() {
  ui->stackedWidget->setCurrentWidget(ui->pageNewDate);
}

void MainWindow::on_btnSelecExist_clicked() {
  ui->lstListEvents->clear();
  foreach (Event e, eventList) { ui->lstListEvents->addItem(e.getName()); }
  ui->stackedWidget->setCurrentWidget(ui->pageListAttendance);
}

void MainWindow::on_btnNewDateBack_clicked() {
  QCoreApplication::exit(RESTART);
}

void MainWindow::on_btnNewDateNext_clicked() {
  for (auto date : dates) {
    for (int i = 0; i < 48; i++) {
      QCheckBox *box = new QCheckBox;
      QString sTime = QString::number(i/2);
      sTime += (i % 2) ? ":30" : ":00";
      if (i < 20) sTime.prepend("0");
      QDateTime time;
      time.setTime(QDateTime::fromString(sTime, "hh:mm").time());
      box->setText(date + " - " + time.time().toString("hh:mm"));
      ui->gridLayout_17->addWidget(box);
    }
  }
  ui->stackedWidget->setCurrentWidget(ui->pageNewTime);
}
void MainWindow::on_btnNewDateAdd_clicked() {
  if (!dates.contains(ui->calendarWidget->selectedDate().toString("MM/dd/yyyy"))) {
    dates.append(ui->calendarWidget->selectedDate().toString("MM/dd/yyyy"));
  }
  ui->btnNewDateNext->setEnabled(ui->eventName->text() != "");
}
void MainWindow::on_btnNewTimeBack_clicked() {
  QCoreApplication::exit(RESTART);
}

void MainWindow::on_btnNewTimeSave_clicked() {
  QVector<QString> timeSlots;
  QVector<QString> tasks;
  QVector<QString> creatorTasks;
  QObjectList list = ui->scrollArea->widget()->children();
  list.removeFirst(); // Removes the Grid from the list.
  for (auto item : list) {
    QCheckBox *box = qobject_cast<QCheckBox *>(item);
    if (box->isChecked()) {
      QString time = box->text();
      if (currentToggleNew) {
        time = QDateTime::fromString(time, "MM/dd/yyyy - h:mm AP").toString("MM/dd/yyyy - hh:mm");
      }
      timeSlots.append(time);
    }
  }
  Event event(ui->eventName->text(), dates, ui->txtName->text(), timeSlots, tasks);
  Attendee creator(ui->txtName->text(), timeSlots, creatorTasks);
  event.addAttendee(creator);
  eventList.append(event);

  ui->stackedWidget->setCurrentWidget(ui->pageReturn);
}

void MainWindow::on_btnNewTimeAddTasks_clicked() {
  ui->stackedWidget->setCurrentWidget(ui->pageNewTasks);
  ui->gridLayout_20->setAlignment(Qt::AlignTop);
}

void MainWindow::on_btnNewTimeToggle_clicked() {
  QWidget *list = ui->scrollArea->widget();
  QObjectList newList = list->children();
  newList.removeFirst();
  foreach (QObject *box, newList) {
    QCheckBox *thatBox = qobject_cast<QCheckBox *>(box);
    if (!currentToggleNew) {
      thatBox->setText(QDateTime::fromString(thatBox->text(), "MM/dd/yyyy - hh:mm").toString("MM/dd/yyyy - h:mm AP"));
    } else {
      thatBox->setText(QDateTime::fromString(thatBox->text(), "MM/dd/yyyy - h:mm AP").toString("MM/dd/yyyy - hh:mm"));
    }
  }
  currentToggleNew = !currentToggleNew;
}

void MainWindow::on_txtTask_textChanged() {
  ui->btnNewTaskAdd->setEnabled(ui->txtTask->text() != "");
}

void MainWindow::on_btnNewTaskAdd_clicked() {
  QLabel *label = new QLabel;
  label->setText(ui->txtTask->text());
  ui->gridLayout_20->addWidget(label);
  ui->txtTask->setText("");
}

void MainWindow::on_btnNewTaskDone_clicked() {
  QVector<QString> timeSlots;
  QVector<QString> tasks;
  QVector<QString> creatorTasks;
  QWidget *list = ui->scrollArea->widget();
  QObjectList newList = list->children();
  newList.removeFirst(); // Removes the Grid from the list.
  foreach (QObject *box, newList) {
    QCheckBox *thatBox = qobject_cast<QCheckBox *>(box);
    if (thatBox->isChecked()) {
      QString time = thatBox->text();
      timeSlots.append(time);
    }
  }
  QWidget *list2 = ui->scrollArea_3->widget();
  QObjectList newList2 = list2->children();
  newList2.removeFirst(); // Removes the Grid from the list.
  foreach (QObject *label, newList2) {
    QLabel *thatLabel = qobject_cast<QLabel *>(label);
    tasks.append(thatLabel->text());
  }
  Event event(ui->eventName->text(), dates, ui->txtName->text(), timeSlots, tasks);
  Attendee creator(ui->txtName->text(), timeSlots, creatorTasks);
  event.addAttendee(creator);
  eventList.append(event);

  ui->stackedWidget->setCurrentWidget(ui->pageReturn);
}

void MainWindow::on_btnExit_clicked() {
  QCoreApplication::exit(NORMAL);
}

void MainWindow::on_btnReturnToStart_clicked() {
  QCoreApplication::exit(RESTART);
}

void MainWindow::on_btnListAttendanceBack_clicked() {
  QCoreApplication::exit(RESTART);
}

void MainWindow::on_btnListAttendanceNext_clicked() {
  if (ui->rdAdd->isChecked()) {
    // Figure out which event to use because they're stored in a vector instead of a map (why)
    Event currentEventE;
    for (auto e : eventList) {
      if (e.getName() == currentEvent) {
        currentEventE = e;
        break;
      }
    }

    QVector<QString> times = currentEventE.getSlots();
    for (auto time : times) {
      QCheckBox *box = new QCheckBox;
      box->setText(time);
      ui->gridLayout_18->addWidget(box);
    }

    QVector<Attendee> attendees = currentEventE.getAttendees();
    QVector<QString> tasksTaken;
    for (auto attendee : attendees) {
      for (auto task : attendee.getTasks()) {
        tasksTaken.append(task);
      }
    }

    QVector<QString> tasks = currentEventE.getTasks();
    for (auto task : tasks) {
      QCheckBox *box = new QCheckBox;
      box->setText(task);
      ui->gridLayout_21->addWidget(box);
      if (tasksTaken.contains(task)) {
        box->setEnabled(false);
      }
    }

    ui->gridLayout_18->setAlignment(Qt::AlignTop);
    ui->gridLayout_21->setAlignment(Qt::AlignTop);

    ui->stackedWidget->setCurrentWidget(ui->pageAddAttendance);
  } else if (ui->rdView->isChecked()) {
    // Zero Out Table on Page Load.
    ui->tableTimeSlots->clear();
    ui->tableTimeSlots->setRowCount(0);
    ui->tableTimeSlots->setColumnCount(0);

    // Finds event to be viewed from current event.
    Event currentEventE;
    foreach (Event e, eventList) {
      if (e.getName() == currentEvent) {
        currentEventE = e;
        break;
      }
    }

    // Initialize the time slot table
    ui->tableTimeSlots->insertRow(0);
    ui->tableTimeSlots->insertColumn(0);

    // Populate the time slot column heads
    auto timeSlots = currentEventE.getSlots();
    auto timeSlotsDisp = currentEventE.getSlots();
    for (int i = 0; i < timeSlots.size(); i++) {
      ui->tableTimeSlots->insertColumn(i+1);
      QTableWidgetItem *labelASlots = new QTableWidgetItem(timeSlotsDisp[i].replace(" - ", "\n"));
      ui->tableTimeSlots->setItem(0, i+1, labelASlots);
    }

    // Populate the attendee rows
    auto attendees = currentEventE.getAttendees();
    for (int i = 0; i < attendees.size(); i++) {
      ui->tableTimeSlots->insertRow(i+1);
      QTableWidgetItem *labelAName = new QTableWidgetItem(attendees[i].getName());
      ui->tableTimeSlots->setItem(i+1, 0, labelAName);
      for (int j = 0; j < timeSlots.size(); j++) {
        QTableWidgetItem *labelASlots = new QTableWidgetItem();
        labelASlots->setBackground((attendees[i].getSlots().contains(timeSlots[j])) ? Qt::green : Qt::red);
        ui->tableTimeSlots->setItem(i+1, j+1, labelASlots);
      }
    }

    // Create the total attendees per time slot row
    ui->tableTimeSlots->insertRow(attendees.size()+1);
    for (int i = 0; i < timeSlots.size(); i++) {
      int total = 0;
      for (auto attendee : attendees) {
        total += (attendee.getSlots().contains(timeSlots[i])) ? 1 : 0;
      }
      QTableWidgetItem *totalSlots = new QTableWidgetItem(QString::number(total));
      ui->tableTimeSlots->setItem(attendees.size()+1, i+1, totalSlots);
    }

    // Initialize the task table
    ui->tableTasks->insertRow(0);
    ui->tableTasks->insertColumn(0);
    ui->tableTasks->insertColumn(1);
    QTableWidgetItem *labelATasks = new QTableWidgetItem("Task name");
    QTableWidgetItem *labelTTasks = new QTableWidgetItem("Task assignee");
    ui->tableTasks->setItem(0, 0, labelATasks);
    ui->tableTasks->setItem(0, 1, labelTTasks);

    // Set Row Count for the amount of tasks, and read everything into the table.
    ui->tableTasks->setRowCount(currentEventE.getTasks().count() + 1);
    ui->tableTasks->setCurrentCell(1, 0);
    foreach (QString task, currentEventE.getTasks()) {
      QTableWidgetItem *newTask = new QTableWidgetItem(task);
      ui->tableTasks->setItem(ui->tableTasks->currentRow(), 0, newTask);
      QString taskAssignee = "";
      for (auto attendee : currentEventE.getAttendees()) {
        if (attendee.getTasks().contains(task)) {
          taskAssignee = attendee.getName();
          break;
        }
      }
      QTableWidgetItem *newTaskAssign = new QTableWidgetItem(taskAssignee);
      ui->tableTasks->setItem(ui->tableTasks->currentRow(), 1, newTaskAssign);

      ui->tableTasks->setCurrentCell(ui->tableTasks->currentRow() + 1, 0);
    }

    ui->stackedWidget->setCurrentWidget(ui->pageViewAttendance);
  }
}

void MainWindow::on_btnAddAttendanceBack_clicked() {
  QCoreApplication::exit(RESTART);
}

void MainWindow::on_btnAddAttendanceSave_clicked() {
  QVector<QString> timeSlots;
  QVector<QString> tasks;
  QObjectList list = ui->scrollArea_2->widget()->children();
  list.removeFirst(); // Removes the Grid from the list.
  for (auto item : list) {
    QCheckBox *box = qobject_cast<QCheckBox *>(item);
    if (box->isChecked()) {
      QString time = box->text();
      if (currentToggleAdd) {
        time = QDateTime::fromString(time, "MM/dd/yyyy - h:mm AP").toString("MM/dd/yyyy - hh:mm");
      }
      timeSlots.append(time);
    }
  }
  QObjectList list2 = ui->scrollArea_4->widget()->children();
  list2.removeFirst(); // Removes the Grid from the list.
  for (auto item : list2) {
    QCheckBox *box = qobject_cast<QCheckBox *>(item);
    if (box->isChecked()) {
      tasks.append(box->text());
    }
  }
  Attendee attendee(ui->txtName->text(), timeSlots, tasks);
  Event CurrentEventE;
  for (auto e : eventList) {
    if (e.getName().trimmed() == currentEvent.trimmed()) {
      CurrentEventE = e;
      break;
    }
  }
  CurrentEventE.addAttendee(attendee);
  eventList.removeLast();
  eventList.append(CurrentEventE);
  ui->stackedWidget->setCurrentWidget(ui->pageReturn);
}

void MainWindow::on_btnViewAttendanceReturn_clicked() {
  QCoreApplication::exit(RESTART);
}

void MainWindow::on_btnViewAttendanceQuit_clicked() {
  QCoreApplication::exit(NORMAL);
}

void MainWindow::on_txtName_textChanged() {
  if (ui->txtName->text() != "") {
    ui->btnNew->setEnabled(true);
    ui->btnSelecExist->setEnabled(true);
  } else {
    ui->btnNew->setEnabled(false);
    ui->btnSelecExist->setEnabled(false);
  }
}

void MainWindow::on_rdAdd_clicked() {
  ui->btnListAttendanceNext->setEnabled(true);
}

void MainWindow::on_rdView_clicked() {
  ui->btnListAttendanceNext->setEnabled(true);
}

void MainWindow::on_eventName_textChanged() {
  ui->btnNewDateNext->setEnabled(ui->eventName->text() != "" && dates.length() > 0);
}

void MainWindow::on_lstListEvents_itemClicked(QListWidgetItem *item) {
  currentEvent = item->text();
  ui->btnListAttendanceNext->setEnabled(true);
}

void MainWindow::on_btnViewAttendanceToggle_clicked() {
  for (int i = 1; i < ui->tableTimeSlots->columnCount(); i++) {
    QTableWidgetItem *item = ui->tableTimeSlots->item(0, i);
    QString time = item->text();
    if (!currentToggleView) {
      item->setText(QDateTime::fromString(time, "MM/dd/yyyy\nhh:mm").toString("MM/dd/yyyy\nh:mm AP"));
    } else {
      item->setText(QDateTime::fromString(time, "MM/dd/yyyy\nh:mm AP").toString("MM/dd/yyyy\nhh:mm"));
    }
  }
  currentToggleView = !currentToggleView;
}

void MainWindow::on_btnAddAttendanceToggle_clicked() {
  QObjectList list = ui->scrollArea_2->widget()->children();
  list.removeFirst();
  for (auto box : list) {
    QCheckBox *item = qobject_cast<QCheckBox *>(box);
    if (!currentToggleAdd) {
      item->setText(QDateTime::fromString(item->text(), "MM/dd/yyyy - hh:mm").toString("MM/dd/yyyy - h:mm AP"));
    } else {
      item->setText(QDateTime::fromString(item->text(), "MM/dd/yyyy - h:mm AP").toString("MM/dd/yyyy - hh:mm"));
    }
  }
  currentToggleAdd = !currentToggleAdd;
}
