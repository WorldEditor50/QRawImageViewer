#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <QSettings>

struct ImageConfig {
    int width;
    int height;
    int format;
    QString path;
};

class Configuration
{
private:
    ImageConfig imageConfig;
public:
    Configuration();
    int load(const QString &configFile);
    void save(const QString &configFile);
    int getWidth() const {return imageConfig.width;}
    void setWidth(int w) {imageConfig.width = w;}
    int getHeight() const {return imageConfig.height;}
    void setHeight(int h) {imageConfig.height = h;}
    int getFormat() const {return imageConfig.format;}
    void setFormat(int format_) {imageConfig.format = format_;}
    QString getPath() const {return imageConfig.path;}
    void setPath(const QString &path_) {imageConfig.path = path_;}
};

#endif // CONFIGURATION_H
