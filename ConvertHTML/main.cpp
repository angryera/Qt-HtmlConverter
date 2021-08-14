#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QPrinter>
#include <QPainter>
#include <QTextDocument>
#include <QPdfWriter>
#include <QDir>
#include <QProcess>
#include <windows.h>
#include <QAbstractTextDocumentLayout>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString from = QCoreApplication::arguments().at(1);
    QString to = QCoreApplication::arguments().at(2);

    QFile file(from);
    file.open(QIODevice::ReadOnly);

    QByteArray fileContent = file.readAll();
    file.seek(0);
    QByteArray htmlContent = file.readAll();

    int imageFrom = fileContent.indexOf("/9j/");
    fileContent = fileContent.right(fileContent.length() - imageFrom);

    int imageTo = fileContent.indexOf("</span></P>");

    fileContent = fileContent.left(imageTo - 6);
    fileContent = QByteArray::fromBase64(fileContent);

    // For Image Extract
    QFile imageFile(QDir::currentPath() + "\\Images\\" + to.split(".").first() + ".jpg");
    imageFile.open(QIODevice::ReadWrite);
    imageFile.write(fileContent);
    imageFile.close();

    // For Pdf File
    htmlContent = htmlContent.remove(imageFrom, imageTo);
    int htmlFrom = htmlContent.indexOf("src=") + 5;
    int htmlTo = htmlContent.indexOf("\"", htmlFrom);
    htmlContent = htmlContent.remove(htmlFrom, htmlTo - htmlFrom);
    QString replaceStr = "src=\"Images\\" + to.split(".").first() + ".jpg" + "\"";
    htmlContent.replace("src=\"\"", QByteArray::fromStdString(replaceStr.toStdString()));
//    qDebug() << htmlFrom << htmlTo;
    htmlContent.replace("�", " ");

    QFile htmlFile(to.split(".").first() + ".html");
    htmlFile.open(QIODevice::ReadWrite);
    htmlFile.write(htmlContent);
    htmlFile.close();
    file.close();

    QString wkpath = QDir::currentPath() + "//wkhtmltopdf.exe --enable-local-file-access -q -T 20 -B 20 -R 20 -L 20 " + to.split(".").first() + ".html " + to;
//    qDebug() << wkpath;
    system(wkpath.toStdString().data());
    QFile::copy(QDir::currentPath()+ "//" + to, QDir::currentPath() + "//Result//" + to);
    qDebug() << "Finished";
    exit(true);
}
