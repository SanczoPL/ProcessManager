#ifndef PROCESS_H
#define PROCESS_H

#include "includespdlog.h"

#include <QJsonObject>
#include <QString>
#include <QProcess>


class Process : public QObject
{
  Q_OBJECT

 public:
  Process(QJsonObject const &Configuration);
  ~Process();

  enum ProcessCommand { StartProcess, RestartProcess, KillProcess };
  enum ProcessState { ProcessUnstarted, ProcessAlive, ProcessFinished, ProcessKilled, ProcessDead };

  QString name() {return m_name;}
  bool running() { return m_isRunning; }
  bool killed() { return m_killed; }
  bool autostart() { return m_autoStart; }
  void execute(ProcessCommand const a_command);
  ProcessState state(); 

 signals:
  void read(QByteArray const a_log);
  
private slots:
  void onProcessStateChanged(QProcess::ProcessState a_state);
  void onRead();

private:
  void start();
  void kill();

 private:

  bool m_killed{};
  bool m_isRunning{};
  bool m_succesFinished{};
  bool m_started{};

  QProcess m_process;

  QString m_name;
  QString m_binaryPath;
  QString m_workingDirectory;
  QString m_arguments;
  bool m_autoStart;
};

#endif // PROCESS_H