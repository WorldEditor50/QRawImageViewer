#include "configuration.h"
#include <QFile>
#include <QSettings>

Configuration::Configuration()
{

}

int Configuration::load(const QString &configFile)
{
    if (!QFile::exists(configFile)) {
        return -1;
    }
    QSettings config(configFile, QSettings::IniFormat);

    imageConfig.width = config.value("/ImageConfig/width").toInt();
    imageConfig.height = config.value("/ImageConfig/height").toInt();
    imageConfig.format = config.value("/ImageConfig/format").toInt();
    imageConfig.path = config.value("/ImageConfig/path").toString();
    return 0;
}

void Configuration::save(const QString &configFile)
{
    QSettings config(configFile, QSettings::IniFormat);

    config.setValue("/ImageConfig/width", imageConfig.width);
    config.setValue("/ImageConfig/height", imageConfig.height);
    config.setValue("/ImageConfig/format", imageConfig.format);
    config.setValue("/ImageConfig/path", imageConfig.path);
    return;
}
