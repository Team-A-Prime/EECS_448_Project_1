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
   *
   * Pre: None
   *
   * Post: An Attendee is created with variables initialized.
   */
  Attendee(const QString &name, const QVector<QString> &timeSlots);

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

private:
  QString name;               /*!< Name of the attendee */
  QVector<QString> timeSlots; /*!< The times they are attending the associated event. */
};
