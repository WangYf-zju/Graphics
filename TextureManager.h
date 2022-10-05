#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QString>
#include <QFileInfo>
#include <QImage>
#include <QOpenGLTexture>
#include <QMap>
#include <QVector>

class TextureManager
{
public:
    QString addTexture(QString filepath)
    {
        QImage img;
        if (img.load(filepath))
        {
            QOpenGLTexture * texture = new QOpenGLTexture(img);
            QFileInfo f1(filepath);
            
            for (auto k = texturePath.keyBegin(); k != texturePath.keyEnd(); k++)
            {
                QFileInfo f2(*k);
                if (f1 == f2)
                {
                    unsigned int index = texturePath[*k];
                    auto p = textures[index];
                    textures[index] = texture;
                    delete p;
                    return *k;
                }
            }
            filepath = f1.absoluteFilePath();
            texturePath.insert(filepath, textures.size());
            textures.push_back(texture);
            return filepath;
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
};

#endif // !TEXTUREMANAGER_H
