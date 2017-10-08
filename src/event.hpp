#pragma once

#include "attendee.hpp"
#include <QString>
#include <QVector>

using namespace std;

class Event {
public:
  /*!
   * \brief Event - Constructor
   *\details
   * Pre: None
   *
   * Post: An empty event is created.
   */
  Event();
  /*!
   * \brief Event - Constructor
   * \param name
   * \param dates
   * \param creator
   * \param timeSlots
   * \param tasks
   *\details
   * Pre: None
   *
   * Post: An event is created with multiple parts initialized.
   */
  Event(const QString &name, const QVector<QString> &dates, const QString &creator, const QVector<QString> &timeSlots, const QVector<QString> &tasks);

  /*!
   * \brief Event - Deconstructor
   *
   * Pre: An event is to be deleted.
   *
   * Post: None
   */
  ~Event();
  /*!
   * \brief getName
   * \return name
   *\details
   * Pre: None
   *
   * Post: None
   */
  QString getName() const;

  /*!
   * \brief getDate
   * \return date
   *\details
   * Pre: None
   *
   * Post: None
   */
  QVector<QString> getDates() const;

  /*!
   * \brief getCreator
   * \return creator
   *\details
   * Pre: None
   *
   * Post: None
   */
  QString getCreator() const;

  /*!
   * \brief getSlots
   * \return QVector<QString> timeSlots
   *\details
   * Pre: None
   *
   * Post: None
   */
  QVector<QString> getSlots() const;

  /*!
   * \brief getTasks
   * \return QVector<QString> tasks
   *\details
   * Pre: None
   *
   * Post: None
   */
  QVector<QString> getTasks() const;

  /*!
   * \brief getAttendees
   * \return attendees
   *\details
   * Pre: None
   *
   * Post: None
   */
  QVector<Attendee> getAttendees() const;

  /*!
   * \brief addSlot
   * \param slot
   *\details
   * Pre: a time slot is to be added to the event.
   *
   * Post: a time slot is added to the event.
   */
  void addSlot(const QString &slot);

  /*!
   * \brief removeSlot
   * \param slot
   *\details
   * Pre: A time slot is to be removed from the event.
   *
   * Post: A time slot is removed from the event.
   */
  void removeSlot(const QString &slot);

  /*!
   * \brief addAttendee
   * \param attendee
   *\details
   * Pre: An attendee is to be added to the vector.
   *
   * Post: An attendee is added.
   */
  void addAttendee(const Attendee &attendee);

  /*!
   * \brief getAttendeeNames
   * \return QVector<QString> names
   *
   * Pre: None
   *
   * Post: None
   *
   * Note: names is a local variable declared inside of the function.
   */
  QVector<QString> getAttendeeNames() const;

private:
  QString name;                /*!< The name of the event.*/
  QVector<QString> dates;      /*!< the dates that the event is going to take place at.*/
  QString creator;             /*!< The name of the creator.*/
  QVector<QString> timeSlots;  /*!< The times the event will be taking place at.
                                  Multiple times in stagnated bunches might be
                                  used,  so a vector is used. */
  QVector<QString> tasks;  /*!< The list of tasks to be signed up for */
  QVector<Attendee> attendees; /*!< The list of the attendees to an event.*/
};
