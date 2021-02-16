#include "includespdlog.h"
#include "configreader.h"

#include "processmanager.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

constexpr auto GENERAL{ "General" };
constexpr auto LOG_LEVEL{ "LogLevel" };

void intro();

int main(int argc, char *argv[])
{
  QCoreApplication application(argc, argv);
  
  
  Logger->set_pattern("[%Y-%m-%d] [%H:%M:%S.%e] [%t] [%^%l%$] %v");
  Logger->set_level(static_cast<spdlog::level::level_enum>(0));
  
  //ConfigReader *configReader = new ConfigReader(); 
  std::unique_ptr<ConfigReader> configReader = std::make_unique<ConfigReader>();
  
  QString configName{ "config.json" };
  QJsonObject jObject;
  if(!configReader->readConfig(configName,jObject))
  {
    Logger->error("Read {} failed", configName.toStdString());
    return -1;
  }
  
  QJsonObject jGeneral{jObject[GENERAL].toObject()};
  auto messageLevel{ jGeneral[LOG_LEVEL].toInt() };
  Logger->set_level(static_cast<spdlog::level::level_enum>(messageLevel));

  intro();
  QStringList args = QCoreApplication::instance()->arguments();
  Logger->info("args:{}",args.takeFirst().toStdString()); 
  qDebug() << "args:" << args;

  ProcessManager o_processManager{ jObject };
  return application.exec();
}

void intro()
{
	Logger->info(
    "\n\n\t\033[1;31mQtProcessManager v1.0 \033[0m\n"
    "\tAuthor: Grzegorz Matczak\n"
    "\t22.03.2020\n"
  );
}
