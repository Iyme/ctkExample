#include "mainwindowplugin.h"
#include <service/event/ctkEventConstants.h>
#include <service/event/ctkEventAdmin.h>

MainWindowPlugin::MainWindowPlugin(ctkPluginContext *context)
    :m_context(context)
{
    m_windowDlg = new MainWindowDlg(context);
    ctkDictionary dic;
    dic.insert(ctkEventConstants::EVENT_TOPIC, "event/registAction");
    context->registerService<ctkEventHandler>(this, dic);
    connect(this, SIGNAL(registAction(ctkEvent)), this, SLOT(onRegistAction(ctkEvent)));

}

void MainWindowPlugin::popMainWindow()
{
    m_windowDlg->show();
}
void MainWindowPlugin::handleEvent(const ctkEvent& event)
{
    if(event.getTopic() == "event/registAction")
        emit registAction(event);

}

void MainWindowPlugin::onRegistAction(const ctkEvent& event)
{
    QString name, id, topic;
    name = event.getProperty("name").toString();
    id = event.getProperty("id").toString();
    topic = event.getProperty("topic").toString();
    m_windowDlg->registAction(id, name, topic);
}
