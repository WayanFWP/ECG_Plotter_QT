#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QComboBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), serial(new QSerialPort(this)), plotTimer(new QTimer(this))
{
    ui->setupUi(this);
    updateAvailablePorts();

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    connect(ui->acquisition, &QPushButton::clicked, this, &MainWindow::on_acquisition_clicked);
    connect(ui->PortAvailable, &QComboBox::currentTextChanged, this, &MainWindow::openSelectedPort);

    // Setup QCustomPlot with optimizations
    ui->dataPlot->addGraph();
    ui->dataPlot->xAxis->setLabel("Time (s)");
    ui->dataPlot->yAxis->setLabel("ECG ADC");
    ui->dataPlot->xAxis->setRange(0, 10);
    ui->dataPlot->yAxis->setRange(-10, 10);
    
    // Performance optimizations
    ui->dataPlot->setNotAntialiasedElement(QCP::aeAll);
    ui->dataPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->dataPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);
    
    // Reduce plot update frequency - only update when new data arrives
    connect(plotTimer, &QTimer::timeout, this, &MainWindow::updatePlot);
    plotTimer->start(100); // 10 FPS instead of 30 FPS
}

void MainWindow::readSerialData()
{
    static QByteArray buffer;
    buffer.append(serial->readAll());

    const int sampleSize = 2; // 2 bytes per sample
    bool newDataAdded = false;

    while (buffer.size() >= sampleSize) {
        uint16_t value = (uint8_t(buffer[0]) & 0xFF) | ((uint8_t(buffer[1]) & 0xFF) << 8);
        float voltage = 5.0f * (value / 1023.0); // Convert ADC value to voltage
        buffer.remove(0, sampleSize);

        currentTime += 0.01; // 1 ms = 1 kHz sampling rate
        xData.append(currentTime);
        yData.append(voltage);
        newDataAdded = true;

        // Keep only last 10 seconds of data
        while (!xData.isEmpty() && xData.first() < currentTime - 10.0) {
            xData.removeFirst();
            yData.removeFirst();
        }
    }
    
    // Only update plot if new data was added
    if (newDataAdded) {
        // Immediate plot update for real-time feel
        updatePlot();
    }
}

void MainWindow::updatePlot()
{
    if (xData.isEmpty()) return; // Don't plot if no data
    
    ui->dataPlot->graph(0)->setData(xData, yData);
    ui->dataPlot->xAxis->setRange(currentTime - 10.0, currentTime);
    ui->dataPlot->replot(QCustomPlot::rpQueuedReplot); // Use queued replot for better performance
}

MainWindow::~MainWindow()
{
    if (serial->isOpen())
        serial->close();
    delete ui;
}

void MainWindow::updateAvailablePorts()
{
    ui->PortAvailable->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports) {
        // Only add Arduino and USB serial ports, skip generic ttyS* ports
        if (port.portName().startsWith("ttyACM") || 
            port.portName().startsWith("ttyUSB") ||
            port.manufacturer().contains("Arduino", Qt::CaseInsensitive)) {
            
            // Store full device path in combo box
            QString fullPath = "/dev/" + port.portName();
            ui->PortAvailable->addItem(fullPath);
            qDebug() << "Available port:" << fullPath
                     << "Description:" << port.description()
                     << "Manufacturer:" << port.manufacturer();
        }
    }
}

// ...existing code...
void MainWindow::openSelectedPort()
{
    if (serial->isOpen())
        serial->close();

    QString selectedPort = ui->PortAvailable->currentText();
    if (selectedPort.isEmpty())
    {
        qDebug() << "No port selected.";
        return;
    }

    qDebug() << "Attempting to open port:" << selectedPort;
    
    serial->setPortName(selectedPort);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "Successfully opened port:" << selectedPort;
    }
    else
    {
        qDebug() << "Failed to open port:" << selectedPort;
        qDebug() << "Error:" << serial->errorString();
        qDebug() << "Error code:" << serial->error();
    }
}

void MainWindow::on_acquisition_clicked()
{
    qDebug() << "Acquisition button clicked. Port open status:" << serial->isOpen();
    
    if (serial->isOpen())
    {
        QByteArray command = "1";
        qint64 bytesWritten = serial->write(command);
        qDebug() << "Sent command:" << command << "Bytes written:" << bytesWritten;
    }
    else
    {
        qDebug() << "Serial port not open. Attempting to open selected port.";
        openSelectedPort(); // Try to open the port
        
        if (serial->isOpen()) {
            QByteArray command = "1";
            qint64 bytesWritten = serial->write(command);
            qDebug() << "Sent command:" << command << "Bytes written:" << bytesWritten;
        } else {
            qDebug() << "Still unable to open serial port.";
        }
    }
}
