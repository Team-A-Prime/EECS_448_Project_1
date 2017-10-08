#pragma once

#include "event.hpp"
#include <QFile>
#include <QMessageBox> //for debug
#include <QString>
#include <QTextStream>

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
