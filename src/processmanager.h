#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QVector>
#include "process.h"


class ProcessManager : public QObject
{
  Q_OBJECT

 public:
  ProcessManager(QJsonObject const &Configuration);
  ~ProcessManager();

public slots:
  void onCase(QJsonObject json);
  void onUpdate();

 private:
 QTimer *m_timer;
 QVector<Process *> m_processes{};
};

#endif // PROCESS_MANAGER_H