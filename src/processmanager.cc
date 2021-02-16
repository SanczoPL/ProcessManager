#include "processmanager.h"

constexpr auto PROCESSES{ "Processes" };
auto constexpr PROCESS_STATE_ALIVE{ "Alive" };
auto constexpr PROCESS_STATE_DEAD{ "Dead" };
auto constexpr PROCESS_STATE_UNSTARTED{ "Unstarted" };
auto constexpr PROCESS_STATE_KILLED{ "Killed" };
auto constexpr PROCESS_STATE_FINISHED{ "Finished" };
constexpr auto AUTO_START{ "AutoStart" };

ProcessManager::ProcessManager(QJsonObject const &config)
{
  m_timer = new QTimer(this);
  m_timer->start(1000);
  connect(m_timer, SIGNAL(timeout()), this, SLOT(onUpdate()));
  
  auto const &_processList{ config[PROCESSES].toArray() };
  for (auto const &_processIter : _processList) 
  {
    auto const _process{ new Process{ _processIter.toObject() } };
    m_processes.push_back(_process);
    bool _autostart = _processIter.toObject()[AUTO_START].toBool();
    if (_autostart)
    {
      _process->execute(Process::StartProcess);
    }
  }
}

ProcessManager::~ProcessManager() {}

void ProcessManager::onUpdate()
{
  for (auto const &_process : m_processes) {
    QString processStatus{};

    switch (_process->state()) {
      case Process::ProcessUnstarted: processStatus = PROCESS_STATE_UNSTARTED; break;
      case Process::ProcessAlive: processStatus = PROCESS_STATE_ALIVE; break;
      case Process::ProcessFinished: processStatus = PROCESS_STATE_FINISHED; break;
      case Process::ProcessKilled: processStatus = PROCESS_STATE_KILLED; break;
      case Process::ProcessDead: processStatus = PROCESS_STATE_DEAD; break;
    }

    if (!_process->running() && !_process->killed() && _process->autostart()) _process->execute(Process::StartProcess);

    Logger->debug("Process {} is {}", _process->name().toStdString(), processStatus.toStdString());
  }
}

void ProcessManager::onCase(QJsonObject json)
{

}