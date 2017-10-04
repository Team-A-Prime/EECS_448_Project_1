#include "attendee.h"

Attendee::Attendee() {
}

Attendee::Attendee(const QString &name, const QVector<QString> &timeSlots)
    : name(name), timeSlots(timeSlots) {
}

QString Attendee::getName() const {
  return name;
}

QVector<QString> Attendee::getSlots() const {
  return timeSlots;
}
