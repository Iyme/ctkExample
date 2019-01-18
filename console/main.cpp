#include <QCoreApplication>
#include <QtWidgets/QApplication>
#include "ctkPluginFrameworkFactory.h"
#include "ctkPluginFramework.h"
#include "ctkPluginException.h"
#include "ctkPluginContext.h"
#include "ctkPluginFrameworkLauncher.h"
#include <QDebug>
#include <QDir>
#include <QDirIterator>

#include "../includes/imainwindow.h"

void loadPlugins(QString path, ctkPluginContext* context, QList<QSharedPointer<ctkPlugin> >* list)
{
   QDir dir(path);
   if(!dir.exists())
        return;

    QStringList filters;
    filters << "*.dll" << "*.so" << "*.dylib";
    QDirIterator dir_iterator(path, filters,  QDir::Files | QDir::Writable,
                              QDirIterator::Subdirectories);

    while (dir_iterator.hasNext()) {
        dir_iterator.next();
        QFileInfo file = dir_iterator.fileInfo();

        QString file_path = file.absoluteFilePath();
        if(file_path.contains(".1"))
            continue;

        qDebug() << file_path;

        QUrl url = QUrl::fromLocalFile(file_path);

        QSharedPointer<ctkPlugin> plugin;
        try
        {
            plugin = context->installPlugin(url);
        }catch(ctkPluginException e){
            qDebug() << e.message() << e.getType();
            return;
        }
        list->append(plugin);
    }


}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("ctkExample");//给框架创建名称，Linux下没有这步会报错

    QString libPath = QCoreApplication::applicationDirPath();

    ctkPluginFrameworkLauncher::addSearchPath(libPath);
    ctkPluginFrameworkLauncher::start("org.commontk.eventadmin");

    ctkPluginFrameworkFactory factory;//插件工厂类
    QSharedPointer<ctkPluginFramework> framework;
    framework = factory.getFramework();

    try{
        framework->init();
        framework->start();
    }
    catch(const ctkPluginException& e){
        qDebug() << "framework init fail";
        qDebug() << e.message() << e.getType();
    }


    QString dir = QCoreApplication::applicationDirPath() + "/plugins";
    ctkPluginContext* context = framework->getPluginContext();

    QList<QSharedPointer<ctkPlugin> > ctkPluginList;
    loadPlugins(dir, context, &ctkPluginList);


    foreach (QSharedPointer<ctkPlugin> plugin, ctkPluginList) {
        try{
            plugin->start(ctkPlugin::START_TRANSIENT);
        }catch(ctkPluginException e){
            qDebug() << e.message() << e.getType();
        }
    }



    ctkServiceReference ref =context->getServiceReference<iMainWindow>();
    iMainWindow* mainWindow = nullptr;
    if(ref)
        mainWindow = context->getService<iMainWindow>(ref);

    if(mainWindow)
    {
        mainWindow->popMainWindow();
        context->ungetService(ref);
    }

//    ctkPluginFrameworkLauncher::stop("org.commontk.eventadmin");


    return a.exec();
}
