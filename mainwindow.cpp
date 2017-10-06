#include "mainwindow.h"
#include "attendee.h"
#include "readwrite.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

QVector<QString> dates;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentToggleNew(false), currentToggleAdd(false),
      currentToggleView(false) {
  ui->setupUi(this);
  ReadWrite::read(eventList);
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
  ui->stackedWidget->setCurrentWidget(ui->pageStart);
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
  if (!dates.contains(ui->calendarWidget->selectedDate().toString())) {
    dates.append(ui->calendarWidget->selectedDate().toString("MM/dd/yyyy"));
  }
  ui->btnNewDateNext->setEnabled(ui->eventName->text() != "");
}
void MainWindow::on_btnNewTimeBack_clicked() {
  ui->stackedWidget->setCurrentWidget(ui->pageNewDate);
}

void MainWindow::on_btnNewTimeSave_clicked() {
  QVector<QString> timeSlots;
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
  Event event(ui->eventName->text(), dates, ui->txtName->text(), timeSlots);
  Attendee creator(ui->txtName->text(), timeSlots);
  event.addAttendee(creator);
  eventList.append(event);

  ui->stackedWidget->setCurrentWidget(ui->pageReturn);
}

void MainWindow::on_btnNewTimeToggle_clicked() {
  QWidget *list = ui->scrollArea->widget();
  QObjectList newList = list->children();
  newList.removeFirst();
  foreach (QObject *box, newList) {
    QCheckBox *thatBox = qobject_cast<QCheckBox *>(box);
    QString textToConvert = thatBox->text();
    if (!currentToggleNew) {
      QDateTime time = QDateTime::fromString(textToConvert, "MM/dd/yyyy - hh:mm");
      thatBox->setText(time.toString("MM/dd/yyyy - hh:mm AP"));
    } else {
      QDateTime time = QDateTime::fromString(textToConvert, "MM/dd/yyyy - hh:mm AP");
      thatBox->setText(time.toString("MM/dd/yyyy - hh:mm"));
    }
  }
  currentToggleNew = !currentToggleNew;
}

void MainWindow::on_btnExit_clicked() {
  QCoreApplication::quit();
}

void MainWindow::on_btnReturnToStart_clicked() {
  ui->stackedWidget->setCurrentWidget(ui->pageStart);
}

void MainWindow::on_btnListAttendanceBack_clicked() {
  ui->stackedWidget->setCurrentWidget(ui->pageStart);
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

    ui->stackedWidget->setCurrentWidget(ui->pageAddAttendance);
  } else if (ui->rdView->isChecked()) {
    // Zero Out Table on Page Load.
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);

    // Finds event to be viewed from current event.
    Event currentEventE;
    foreach (Event e, eventList) {
      if (e.getName() == currentEvent) {
        currentEventE = e;
        break;
      }
    }

    // Initialize header
    ui->tableWidget->insertRow(0);
    ui->tableWidget->insertColumn(0);
    ui->tableWidget->insertColumn(1);
    QTableWidgetItem *labelA = new QTableWidgetItem("Attendees");
    QTableWidgetItem *labelT = new QTableWidgetItem("Times");
    ui->tableWidget->setItem(0, 0, labelA);
    ui->tableWidget->setItem(0, 1, labelT);

    // Set Row Count for the amount of attendees, and read everything into the table.
    ui->tableWidget->setRowCount(currentEventE.getAttendees().count() + 1);
    ui->tableWidget->setCurrentCell(1, 0);
    foreach (Attendee a, currentEventE.getAttendees()) {
      QString allSlots;
      QTableWidgetItem *newAtt = new QTableWidgetItem(a.getName());
      for (QString time : a.getSlots()) {
        if (a.getSlots().indexOf(time) != a.getSlots().count() - 1) {
          allSlots.append(time + ",");
        } else {
          allSlots.append(time);
        }
      }
      QTableWidgetItem *newTim = new QTableWidgetItem(allSlots);
      ui->tableWidget->setItem(ui->tableWidget->currentRow(), 0, newAtt);
      ui->tableWidget->setItem(ui->tableWidget->currentRow(), 1, newTim);
      ui->tableWidget->setCurrentCell(ui->tableWidget->currentRow() + 1, 0);
    }

    ui->stackedWidget->setCurrentWidget(ui->pageViewAttendance);
  }
}

void MainWindow::on_btnAddAttendanceBack_clicked() {
  ui->stackedWidget->setCurrentWidget(ui->pageListAttendance);
}

void MainWindow::on_btnAddAttendanceSave_clicked() {
  QVector<QString> timeSlots;
  QWidget *list = ui->scrollArea_2->widget();
  QObjectList newList = list->children();
  newList.removeFirst(); // Removes the Grid from the list.
  foreach (QObject *box, newList) {
    QCheckBox *thatBox = qobject_cast<QCheckBox *>(box);
    if (thatBox->isChecked()) {
      QString time = thatBox->text();
      timeSlots.append(time);
    }
  }
  Attendee attendee(ui->txtName->text(), timeSlots);
  Event CurrentEventE;
  foreach (Event e, eventList) {
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

void MainWindow::on_btnViewAttendanceBack_clicked() {
  ui->stackedWidget->setCurrentWidget(ui->pageListAttendance);
}

void MainWindow::on_btnViewAttendanceReturn_clicked() {
  ui->stackedWidget->setCurrentWidget(ui->pageStart);
}

void MainWindow::on_btnViewAttendanceQuit_clicked() {
  QCoreApplication::quit();
}

void MainWindow::on_txtName_textChanged(/*const QString &arg1*/) {
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

void MainWindow::on_eventName_textChanged(/*const QString &arg1*/) {
  ui->btnNewDateNext->setEnabled(ui->eventName->text() != "" && dates.length() > 0);
}

void MainWindow::on_lstListEvents_itemClicked(QListWidgetItem *item) {
  currentEvent = item->text();
  ui->btnListAttendanceNext->setEnabled(true);
}

void MainWindow::on_btnViewAttendanceToggle_clicked() {
  if (ui->tableWidget->rowCount() != 1) {
    for (int i = 1; i < ui->tableWidget->rowCount(); i++) {
      QTableWidgetItem *item = ui->tableWidget->item(i, 1);
      QList<QString> itemS = item->text().split(",");
      QString newTime;
      foreach (QString time, itemS) {
        if (time.contains("&")) {
          if (time.indexOf("&") == 3) {
            time.replace("&", "");
          }
          if (time.indexOf("&") == 1) {
            time.replace("&", "");
          }
          if (time.indexOf("&") == 0) {
            time.replace("&", "");
          }
        }
        if (!currentToggleView) {
          QDateTime Dtime;
          Dtime.setTime(QTime::fromString(time));
          newTime.append(Dtime.time().toString("hh:mm:ss AP"));
          newTime.append(",");
        } else {
          if (time.contains("AM")) {
            if (time.startsWith("12")) {
              time = "00" + time.left(8).remove(0, 2);
            } else {
              time = time.left(8);
            }
          } else if (time.contains("PM")) {
            if (time.left(2) != QString::number(12)) {
              time = QString::number(time.left(2).toInt() + 12) + time.left(8).remove(0, 2);
            } else {
              time = time.left(8);
            }
          }

          newTime.append(time);
          newTime.append(",");
        }
      }
      newTime.chop(1);
      QTableWidgetItem *newTim = new QTableWidgetItem(newTime);
      ui->tableWidget->setItem(i, 1, newTim);
    }
    currentToggleView = !currentToggleView;
  }
}

void MainWindow::on_btnAddAttendanceToggle_clicked() {
  QWidget *list = ui->scrollArea_2->widget();
  QObjectList newList = list->children();
  newList.removeFirst();
  float timeCounter = 0;
  foreach (QObject *box, newList) {
    QCheckBox *thatBox = qobject_cast<QCheckBox *>(box);
    if (!currentToggleAdd) {
      QString textToConvert = thatBox->text();
      /*Extremely odd behavior where I imagine float to String converstion
       * produces odd & character. These were the locations that they appeared
       * in the string, so they are fixed accordingly.
       */
      if (textToConvert.contains("&")) {
        if (textToConvert.indexOf("&") == 3) {
          textToConvert.replace("&", "");
        }
        if (textToConvert.indexOf("&") == 1) {
          textToConvert.replace("&", "");
        }
        if (textToConvert.indexOf("&") == 0) {
          textToConvert.replace("&", "");
        }
      }
      QDateTime time;
      time.setTime(QTime::fromString(textToConvert));
      thatBox->setText(time.time().toString("hh:mm:ss AP"));
    } else {
      QString timeCounterS = QString::number(timeCounter);
      if (timeCounterS.contains(".5")) {
        QString sTime = QString::number((int)timeCounter) + ":30";
        if (timeCounter < 3) {
          sTime.prepend("0");
        }
        QDateTime time;
        time.setTime(QDateTime::fromString(sTime, "hh:mm").time());

        thatBox->setText(time.time().toString());
      } else {
        QString sTime = QString::number((int)timeCounter) + ":00";
        if (timeCounter < 3) {
          sTime.prepend("0");
        }
        QDateTime time;
        time.setTime(QDateTime::fromString(sTime, "hh:mm").time());
        thatBox->setText(time.time().toString());
      }
      timeCounter += 0.5;
    }
  }
  currentToggleAdd = !currentToggleAdd;
}
