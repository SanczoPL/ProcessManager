#include "process.h"

constexpr auto PROCESS_NAME{ "ProcessName" };
constexpr auto BINARY_PATH{ "BinaryPath" };
constexpr auto WOKING_DIRECTORY{ "WorkingDirectory" };
constexpr auto ARGUMENTS{ "Arguments" };
constexpr auto AUTO_START{ "AutoStart" };


Process::Process(QJsonObject const &Configuration)
  : m_killed{ false }
  , m_isRunning{ false }
  , m_process{ this }
  , m_succesFinished{ false }
  , m_started{ false }
  , m_name{ Configuration[PROCESS_NAME].toString() }
  , m_binaryPath{ Configuration[BINARY_PATH].toString() }
  , m_workingDirectory{ Configuration[WOKING_DIRECTORY].toString() }
  , m_arguments{ Configuration[ARGUMENTS].toString() }
  , m_autoStart{ Configuration[AUTO_START].toBool() }
{
  connect(&m_process, &QProcess::readyRead, this, &Process::onRead);
  connect(&m_process, &QProcess::stateChanged, this, &Process::onProcessStateChanged);
}

Process::~Process() {}

void Process::execute(ProcessCommand const a_command)
{
  switch (a_command) {
    case KillProcess:
      m_killed = true;
      Process::kill();
      break;
    case RestartProcess:
      m_killed = false;
      Process::kill();
      Process::start();
      break;
    case StartProcess:
      m_killed = false;
      Process::start();
      break;
  }
}

void Process::start()
{
  Logger->debug("Process:{} from:{}", (m_binaryPath + m_name).toStdString(),m_workingDirectory.toStdString());
  m_process.setWorkingDirectory(m_workingDirectory);
  QStringList arguments{ m_arguments };
  m_process.start(m_binaryPath + m_name, arguments);
  m_started = true;
}

void Process::kill()
{
  Logger->debug("Signal to kill process: {}", m_name.toStdString());
  m_process.kill();
}

void Process::onRead()
{
  QByteArray data{ m_process.readAll() };
  Logger->debug("Info from {} process: {}", m_name.toStdString(), data.toStdString());
  emit(read(data));
}

void Process::onProcessStateChanged(QProcess::ProcessState a_state)
{
  switch (a_state) {
    case QProcess::ProcessState::Starting:
      Logger->debug("Process {} is starting", m_name.toStdString());
      m_isRunning = true;
      break;
    case QProcess::ProcessState::Running:
      Logger->debug("Process {} is running", m_name.toStdString());
      m_isRunning = true;
      break;
    case QProcess::ProcessState::NotRunning:
      m_isRunning = false;
      switch (m_process.exitStatus()) {
        case QProcess::ExitStatus::NormalExit:
          Logger->debug("Process {} normal exit code: {}", m_name.toStdString(), m_process.exitCode());
          m_succesFinished = true;
          break;
        case QProcess::ExitStatus::CrashExit:
          Logger->debug("Process {} exit code -1", m_name.toStdString());
          m_succesFinished = false;
          break;
      }
      break;
  }
}

Process::ProcessState Process::state()
{
  ProcessState state{};

  if (m_isRunning) {
    state = ProcessAlive;
  } else if (m_killed) {
    state = ProcessKilled;
  } else if (!m_started) {
    state = ProcessUnstarted;
  } else if (m_succesFinished) {
    state = ProcessFinished;
  } else {
    state = ProcessDead;
  }

  return state;
}