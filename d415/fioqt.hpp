#pragma once

#include <QDir>
#include <QString>
static void SetFolder(const std::string &fop)
{
	QString fp(fop.c_str());
	if(!QDir(fp).exists())
		QDir().mkdir(fp);
}

static void ListFileName(const std::string &folder, std::vector<std::string> &fileName)
{
    QStringList files=QDir(folder.c_str()).entryList(QDir::Files);
    fileName.assign(files.size(),"");
    for(size_t i=0;i<fileName.size();i++)
        fileName[i]=files[i].toStdString();
}




