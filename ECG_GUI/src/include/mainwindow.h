#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateAvailablePorts();
    void openSelectedPort();
    void readSerialData();
    void on_acquisition_clicked();
    void updatePlot();

    void on_StopBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QVector<double> xData, yData;
    QTimer *plotTimer;
    double currentTime = 0.0;
    QVector<double> baselineWindow;
    const int baselineWindowSize = 20;
    bool isAcquiring = false;

    void initializeGrid();


};

#endif // MAINWINDOW_H
