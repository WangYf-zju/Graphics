#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QString>
#include <QFileInfo>
#include <QImage>
#include <QOpenGLTexture>
#include <QMap>
#include <QVector>
#include <QDir>

class TextureManager
{
public:
    void setCurrentDir(QDir &&dir) { this->_curDir = dir; }
    QString getOriginPath(int index) 
    {
        if (index >= 0 && index < originalPath.size())
            return originalPath[index]; 
        return "";
    }
    QString addTexture(QString filepath, bool forceUpdate = false)
    {
        QImage img;
        bool flag = QFileInfo(filepath).isAbsolute();
        QString path = filepath;
        if (!flag) path = _curDir.absoluteFilePath(filepath);
        if (!forceUpdate)
        {
            QFileInfo f1(path);
            for (auto k = texturePath.keyBegin(); k != texturePath.keyEnd(); k++)
            {
                QFileInfo f2(*k);
                if (f1 == f2) return *k;
            }
        }
        if (img.load(path))
        {
            QOpenGLTexture * texture = new QOpenGLTexture(img);
            QFileInfo f1(path);
            for (auto k = texturePath.keyBegin(); k != texturePath.keyEnd(); k++)
            {
                QFileInfo f2(*k);
                if (f1 == f2)
                {
                    if (forceUpdate)
                    {
                        unsigned int index = texturePath[*k];
                        auto p = textures[index];
                        textures[index] = texture;
                        delete p;
                    }
                    return *k;
                }
            }
            path = f1.absoluteFilePath();
            texturePath.insert(path, textures.size());
            textures.push_back(texture);
            originalPath.push_back(filepath);
            return path;
        }
        else return "";
    }
    void bind(QString filepath)
    {
        int index = getTextureIndex(filepath);
        if (index >= 0 && index < textures.size())
            textures[index]->bind(0);
    }
    void bind(int index)
    {
        if (index >= 0 && index < textures.size())
            textures[index]->bind(0);
    }
    int getTextureIndex(QString filepath) 
    {  
        filepath = QFileInfo(filepath).absoluteFilePath();
        auto it = texturePath.find(filepath);
        if (it != texturePath.end())
        {
            return texturePath[filepath];
        }
        return -1;
    }
private:
    QMap<QString, unsigned int> texturePath;
    QVector<QOpenGLTexture*> textures;
    QVector<QString> originalPath;
    QDir _curDir;
};

#endif // !TEXTUREMANAGER_H
