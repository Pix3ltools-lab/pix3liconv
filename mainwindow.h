#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QPixmap>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>

class QLabel;
class QLineEdit;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void selectInputFile();
    void convertToIco();
    void exitApplication();
    void showAboutDialog();
    void showAboutQt();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QLabel *inputLabel;
    QLineEdit *inputPathEdit;
    QPushButton *browseButton;
    QPushButton *convertButton;

    QLabel *previewLabel;
    QWidget *placeholderWidget;
    QLabel *placeholderIconLabel;
    QLabel *placeholderTextLabel;
    QPixmap currentPixmap;

    QString inputFilePath;

    void setupMenuBar();
    void updatePreview();
    bool saveMultiSizeIco(const QList<QImage> &images, const QString &icoPath);
    QPixmap createPlaceholderIcon(int size);
};

#endif // MAINWINDOW_H
