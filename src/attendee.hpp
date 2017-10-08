#pragma once

#include <QObject>
#include <QVector>

class Attendee {
public:
  /*!
   * \brief Attendee
   *
   * Pre: None
   *
   * Post: An empty Attendee is created.
   */
  Attendee();

  /*!
   * \brief Attendee
   * \param name
   * \param timeSlots
   * \param tasks
   *
   * Pre: None
   *
   * Post: An Attendee is created with variables initialized.
   */
  Attendee(const QString &name, const QVector<QString> &timeSlots, const QVector<QString> &tasks);

  /*!
   * \brief getName
   * \return name
   *
   * Pre: None
   *
   * Post: None
   */
  QString getName() const;

  /*!
   * \brief getSlots
   * \return  QVector<QString> timeSlots
   *
   * Pre: None
   *
   * Post: None
   */
  QVector<QString> getSlots() const;

  /*!
   * \brief getTasks
   * \return  QVector<QString> taskss
   *
   * Pre: None
   *
   * Post: None
   */
  QVector<QString> getTasks() const;

private:
  QString name;               /*!< Name of the attendee */
  QVector<QString> timeSlots; /*!< The times they are attending the associated event. */
  QVector<QString> tasks;     /*!< The event tasks they are responsible for. */
};
