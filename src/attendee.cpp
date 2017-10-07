#include "attendee.h"

Attendee::Attendee() {
}

Attendee::Attendee(const QString &name, const QVector<QString> &timeSlots, const QVector<QString> &tasks) : name(name), timeSlots(timeSlots), tasks(tasks) {
}

QString Attendee::getName() const {
  return name;
}

QVector<QString> Attendee::getSlots() const {
  return timeSlots;
}

QVector<QString> Attendee::getTasks() const {
  return tasks;
}
