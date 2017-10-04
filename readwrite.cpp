#include "readwrite.h"
#include "event.h"
#include <QFile>
#include <QMessageBox> //for debug
#include <QTextStream>

ReadWrite::ReadWrite() {
  // empty constructor
}

/* both methods in this class are static.
 * You can make a ReadWrite object if necessary.
 * But static methods can be called without an instance of the class.
 */

void ReadWrite::write(const Event &event) {
  QFile file("eventlist.txt");
  if (file.open(QIODevice::Append | QIODevice::WriteOnly)) {
    QTextStream writeStream(&file);
    writeStream << "[event] " + event.getName() + "\n";
    writeStream << "[creator] " + event.getCreator() + "\n";
    writeStream << "[date] ";
    foreach (QString date, event.getDates()) { writeStream << date + ","; }
    writeStream << "\n";

    writeStream << "[timeSlots] ";
    foreach (QString time, event.getSlots()) { writeStream << time + ","; }
    writeStream << "\n";

    foreach (Attendee att, event.getAttendees()) {
      writeStream << "[attendee] " + att.getName() + "\n";
      writeStream << "[att_timeSlots] ";
      foreach (QString time, att.getSlots()) { writeStream << time + ","; }
      writeStream << "\n";
    }

    writeStream << "\n\n";
  }
  file.close();
}

void ReadWrite::read(QVector<Event> &eventList) {
  QFile file("eventlist.txt");
  QString lines = "test";
  QString eventName;
  QString creatorName;
  QString dateText;
  QVector<QString> dates;
  QString times;
  QVector<QString> timeSlots;

  QString att_name;
  QString att_times;

  if (file.open(QIODevice::ReadOnly)) {
    QTextStream readStream(&file);
    while (!readStream.atEnd()) {
      lines = readStream.readLine();
      if (lines.indexOf("[event]") == 0) {
        eventName = lines.right(lines.size() - 8);
        lines = readStream.readLine();
        creatorName = lines.right(lines.size() - 10);
        lines = readStream.readLine();
        dateText = lines.right(lines.size() - 7);
        foreach (QString date, dateText.split(",")) { dates.append(date); }
        lines = readStream.readLine();
        times = lines.right(lines.size() - 12);
        foreach (QString time, times.split(",")) { timeSlots.append(time); }
        timeSlots.removeLast();
        Event newEvent(eventName, dates, creatorName, timeSlots);
        timeSlots.clear();
        lines = readStream.readLine();
        while (lines.startsWith("[attendee] ")) {

          att_name = lines.right(lines.size() - 11);
          lines = readStream.readLine();
          att_times = lines.right(lines.size() - 16);
          foreach (QString time, times.split(",")) { timeSlots.append(time); }
          timeSlots.removeLast();
          Attendee att(att_name, timeSlots);
          newEvent.addAttendee(att);
          lines = readStream.readLine();
          timeSlots.clear();
        }

        eventList.append(newEvent);
      }
    }
    file.remove();
  }
}
