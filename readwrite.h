#ifndef READWRITE_H
#define READWRITE_H

#include "event.h"
#include <QString>

class ReadWrite {
public:
  /*!
   * \brief ReadWrite
   *
   * Pre: None
   *
   * Post: None
   */
  ReadWrite();
  /*!
   * \brief write
   * \param event
   *
   * Pre: None
   *
   * Post: An Event is written to a file.
   */
  static void write(const Event &event);

  /*!
   * \brief read
   * \param eventList
   *
   * Pre: None
   *
   * Post: A file is read. For this specific purpose, it is used to read events.
   */
  static void read(QVector<Event> &eventList);

private:
};

#endif
