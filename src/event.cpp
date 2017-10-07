#include "event.h"

Event::Event() {
}

Event::Event(const QString &name, const QVector<QString> &dates, const QString &creator,
             const QVector<QString> &timeSlots, const QVector<QString> &tasks)
    : name(name), dates(dates), creator(creator), timeSlots(timeSlots), tasks(tasks) {
}

Event::~Event() {
}

QString Event::getName() const {
  return name;
}

QVector<QString> Event::getDates() const {
  return dates;
}

QString Event::getCreator() const {
  return creator;
}

QVector<QString> Event::getSlots() const {
  return timeSlots;
}

QVector<QString> Event::getTasks() const {
  return tasks;
}

QVector<Attendee> Event::getAttendees() const {
  return attendees;
}

QVector<QString> Event::getAttendeeNames() const {
  QVector<QString> names;
  foreach (Attendee attendee, attendees) { names.append(attendee.getName()); }
  return names;
}

void Event::addSlot(const QString &slot) {
  this->timeSlots.push_back(slot);
}

void Event::removeSlot(const QString &slot) {
  this->timeSlots.erase(remove(this->timeSlots.begin(), this->timeSlots.end(), slot), this->timeSlots.end());
}

void Event::addAttendee(const Attendee &attendee) {
  this->attendees.append(attendee);
}
