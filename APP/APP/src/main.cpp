#include <QtGui/QApplication>
#include <QTextCodec>
#include <QDir>
#include "frame.h"

#include "sand_main.h"
#include "global.h"

extern "C"{
void manager(struct seven *EvenInfo)
{
   Q_UNUSED(EvenInfo);
   return ;
}
}



bool copyDirFiles(const QString &SourceDir,const QString &DestDir,bool coverFileIfExist);
/*
created by Dean.Pan on Feb.19th.2013
根据徐宇驰的思路和样例代码做更改
将资源文件中的gif文件夹整体覆盖入app/00104/image路径下
以便LINUXPOS API Os__gif_start再无额外上送图片的工作情况下直接调用
Description:
<1> 如果指定目标目录不存在,则进行创建;
<2> 当为目录时,递归进行复制;
<3> 当允许覆盖,即将旧文件进行删除再进行复制
*/

bool copyDirFiles(const QString &SourceDir,const QString &DestDir,bool coverFileIfExist)
{
    qDebug() << Q_FUNC_INFO;
    QDir srcDir(SourceDir);
    QDir dstDir(DestDir);
    if(!dstDir.exists())
        if(dstDir.mkdir(dstDir.absolutePath())) return false;
    QFileInfoList fileInfoList = srcDir.entryInfoList();
    foreach(QFileInfo fileInfo,fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;
        if(fileInfo.isDir())
        {
            if(!copyDirFiles(fileInfo.filePath(),dstDir.filePath(fileInfo.fileName()),coverFileIfExist))
                return false;
        }
        else
        {
            if(coverFileIfExist && dstDir.exists(fileInfo.fileName()))
                    dstDir.remove(fileInfo.fileName());
            if(!QFile::copy(fileInfo.filePath(),dstDir.filePath(fileInfo.fileName())))
                return false;
        }
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << Q_FUNC_INFO;

    copyDirFiles(":/gif/","./image/",true);
//    copyDirFiles(":/bmp/","./image/bmp/",true);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    Frame frame(argc,argv);
    
    main_entry_gui(argc, argv);
    return a.exec();
}
