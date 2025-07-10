#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPushButton>
#include <QComboBox>
#include <QSerialPortInfo>
#include <numeric>

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent), ui(new Ui::MainWindow), serial(new QSerialPort(this)), plotTimer(new QTimer(this))
// {
//     ui->setupUi(this);
//     updateAvailablePorts();

//     // Initialize UI state
//     ui->StopBtn->setEnabled(false); // Disable stop button initially

//     connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
//     connect(ui->acquisition, &QPushButton::clicked, this, &MainWindow::on_acquisition_clicked);
//     connect(ui->StopBtn, &QPushButton::clicked, this, &MainWindow::on_StopBtn_clicked); // Add this connection
//     connect(ui->PortAvailable, &QComboBox::currentTextChanged, this, &MainWindow::openSelectedPort);

//     // Setup QCustomPlot
//     ui->dataPlot->addGraph();
//     ui->dataPlot->xAxis->setLabel("Time (s)");
//     ui->dataPlot->yAxis->setLabel("ECG (V)");
//     ui->dataPlot->xAxis->setRange(0, 3);
//     ui->dataPlot->yAxis->setRange(-2.5, 2.5);

//     // ECG-style grid setup
//     // Major grid (RED) - every 0.2 seconds
//     QSharedPointer<QCPAxisTickerFixed> xTicker(new QCPAxisTickerFixed);
//     xTicker->setTickStep(0.2); // major ticks every 0.2s
//     ui->dataPlot->xAxis->setTicker(xTicker);

//     QSharedPointer<QCPAxisTickerFixed> yTicker(new QCPAxisTickerFixed);
//     yTicker->setTickStep(0.5); // major ticks every 0.5mV
//     ui->dataPlot->yAxis->setTicker(yTicker);

//     // Grid styling
//     ui->dataPlot->xAxis->grid()->setSubGridVisible(true);
//     ui->dataPlot->yAxis->grid()->setSubGridVisible(true);

//     // Major grid lines (RED) - 0.2s intervals
//     ui->dataPlot->xAxis->grid()->setPen(QPen(Qt::red, 1.0, Qt::SolidLine));
//     ui->dataPlot->yAxis->grid()->setPen(QPen(Qt::red, 1.0, Qt::SolidLine));

//     // Minor grid lines (BLUE) - 0.04s intervals
//     ui->dataPlot->xAxis->grid()->setSubGridPen(QPen(Qt::blue, 0.5, Qt::SolidLine));
//     ui->dataPlot->yAxis->grid()->setSubGridPen(QPen(Qt::blue, 0.5, Qt::SolidLine));

//     // Additional plot settings
//     ui->dataPlot->setNotAntialiasedElement(QCP::aeAll);
//     ui->dataPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
//     ui->dataPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);
//     ui->dataPlot->graph(0)->setPen(QPen(Qt::black, 1.5)); // Make ECG line more visible

//     connect(plotTimer, &QTimer::timeout, this, &MainWindow::updatePlot);
//     plotTimer->start(50); // Faster updates
// }

// MainWindow::MainWindow(QWidget *parent)
//     : QMainWindow(parent), ui(new Ui::MainWindow), serial(new QSerialPort(this)), plotTimer(new QTimer(this))
// {
//     ui->setupUi(this);
//     updateAvailablePorts();

//     // Initialize UI state
//     ui->StopBtn->setEnabled(false);

//     connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
//     connect(ui->acquisition, &QPushButton::clicked, this, &MainWindow::on_acquisition_clicked);
//     connect(ui->StopBtn, &QPushButton::clicked, this, &MainWindow::on_StopBtn_clicked);
//     connect(ui->PortAvailable, &QComboBox::currentTextChanged, this, &MainWindow::openSelectedPort);

//     // Setup QCustomPlot
//     ui->dataPlot->addGraph();
//     ui->dataPlot->xAxis->setLabel("Time (s)");
//     ui->dataPlot->yAxis->setLabel("ECG (V)");
//     ui->dataPlot->xAxis->setRange(0, 3);
//     ui->dataPlot->yAxis->setRange(-2.5, 2.5);

//     // Matikan grid bawaan, kita akan buat manual
//     ui->dataPlot->xAxis->grid()->setVisible(false);
//     ui->dataPlot->yAxis->grid()->setVisible(false);

//     // Additional plot settings
//     ui->dataPlot->setNotAntialiasedElement(QCP::aeAll);
//     ui->dataPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
//     ui->dataPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);
//     ui->dataPlot->graph(0)->setPen(QPen(Qt::black, 1.5));

//     connect(plotTimer, &QTimer::timeout, this, &MainWindow::updatePlot);
//     plotTimer->start(50);
// }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), serial(new QSerialPort(this)), plotTimer(new QTimer(this))
{
    ui->setupUi(this);
    updateAvailablePorts();

    // Initialize UI state
    ui->StopBtn->setEnabled(false);

    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readSerialData);
    connect(ui->acquisition, &QPushButton::clicked, this, &MainWindow::on_acquisition_clicked);
    connect(ui->StopBtn, &QPushButton::clicked, this, &MainWindow::on_StopBtn_clicked);
    connect(ui->PortAvailable, &QComboBox::currentTextChanged, this, &MainWindow::openSelectedPort);

    // Setup QCustomPlot
    ui->dataPlot->addGraph();
    ui->dataPlot->xAxis->setLabel("Time (s)");
    ui->dataPlot->yAxis->setLabel("ECG (V)");
    ui->dataPlot->xAxis->setRange(0, 3);
    ui->dataPlot->yAxis->setRange(-2.5, 2.5);

    // Matikan grid bawaan, kita akan buat manual
    ui->dataPlot->xAxis->grid()->setVisible(false);
    ui->dataPlot->yAxis->grid()->setVisible(false);

    // Additional plot settings
    ui->dataPlot->setNotAntialiasedElement(QCP::aeAll);
    ui->dataPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->dataPlot->graph(0)->setScatterStyle(QCPScatterStyle::ssNone);
    ui->dataPlot->graph(0)->setPen(QPen(Qt::black, 1.5));

    // ✅ TAMBAHKAN INI: Inisialisasi grid dari awal
    initializeGrid();

    connect(plotTimer, &QTimer::timeout, this, &MainWindow::updatePlot);
    plotTimer->start(50);
}

void MainWindow::initializeGrid()
{
    // Draw initial grid untuk range 0-3 detik
    double xMin = 0.0;
    double xMax = 3.0;

    // Draw X-axis grid lines
    for (double x = 0.0; x <= xMax; x += 0.04) {
        QCPItemLine *lineX = new QCPItemLine(ui->dataPlot);
        lineX->start->setCoords(x, -2.5);
        lineX->end->setCoords(x, 2.5);
        
        // Cek apakah ini kelipatan 0.2 (grid besar) atau bukan (grid kecil)
        double remainder = fmod(x + 0.001, 0.2);
        if (remainder < 0.002 || remainder > 0.198) { // Grid besar setiap 0.2s
            lineX->setPen(QPen(Qt::red, 1.0));   // Merah untuk grid besar
        } else {                    // Grid kecil setiap 0.04s
            lineX->setPen(QPen(Qt::blue, 0.5)); // Biru untuk grid kecil
        }
    }

    // Draw Y-axis grid lines
    for (double y = -2.5; y <= 2.5; y += 0.1) {
        QCPItemLine *lineY = new QCPItemLine(ui->dataPlot);
        lineY->start->setCoords(xMin, y);
        lineY->end->setCoords(xMax, y);
        
        // Cek apakah ini kelipatan 0.5 (grid besar) atau bukan (grid kecil)
        double yAdjusted = y + 2.5;
        double yRemainder = fmod(yAdjusted + 0.001, 0.5);
        if (yRemainder < 0.002 || yRemainder > 0.498) { // Grid besar setiap 0.5V
            lineY->setPen(QPen(Qt::red, 1.0));   // Merah untuk grid besar
        } else {                           // Grid kecil setiap 0.1V
            lineY->setPen(QPen(Qt::blue, 0.5)); // Biru untuk grid kecil
        }
    }

    ui->dataPlot->replot();
}

MainWindow::~MainWindow()
{
    if (serial && serial->isOpen())
    {
        serial->close();
    }
    delete ui;
}

void MainWindow::updateAvailablePorts()
{
    ui->PortAvailable->clear();
    const auto serialPortInfos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &portInfo : serialPortInfos)
    {
        ui->PortAvailable->addItem(portInfo.portName());
        qDebug() << "Available port:" << portInfo.portName()
                 << "Description:" << portInfo.description();
    }

    if (ui->PortAvailable->count() == 0)
    {
        qDebug() << "No serial ports found";
    }
}

// void MainWindow::updatePlot()
// {
//     if (xData.isEmpty())
//         return;

//     ui->dataPlot->graph(0)->setData(xData, yData);

//     // Update X-axis range to show last 10 seconds
//     double xMin = currentTime - 10.0;
//     double xMax = currentTime;
//     ui->dataPlot->xAxis->setRange(xMin, xMax);

//     // Ensure grid aligns properly by setting range to multiples of 0.2
//     double adjustedXMin = floor(xMin / 0.2) * 0.2;
//     double adjustedXMax = ceil(xMax / 0.2) * 0.2;
//     ui->dataPlot->xAxis->setRange(adjustedXMin, adjustedXMax);

//     ui->dataPlot->replot(QCustomPlot::rpQueuedReplot);
// }

// void MainWindow::updatePlot()
// {
//     if (xData.isEmpty())
//         return;

//     ui->dataPlot->graph(0)->setData(xData, yData);

//     // Update X-axis range to show last 5 seconds (ubah dari 10.0 ke 5.0)
//     double xMin = currentTime - 3.0;
//     double xMax = currentTime;
//     ui->dataPlot->xAxis->setRange(xMin, xMax);

//     // Ensure grid aligns properly by setting range to multiples of 0.2
//     double adjustedXMin = floor(xMin / 0.2) * 0.2;
//     double adjustedXMax = ceil(xMax / 0.2) * 0.2;
//     ui->dataPlot->xAxis->setRange(adjustedXMin, adjustedXMax);

//     ui->dataPlot->replot(QCustomPlot::rpQueuedReplot);
// }
void MainWindow::updatePlot()
{
    if (xData.isEmpty())
        return;

    ui->dataPlot->graph(0)->setData(xData, yData);

    double xMin = currentTime - 3.0;
    double xMax = currentTime;
    ui->dataPlot->xAxis->setRange(xMin, xMax);

    // Align grid to proper intervals
    double adjustedXMin = floor(xMin / 0.04) * 0.04;
    double adjustedXMax = ceil(xMax / 0.04) * 0.04;
    ui->dataPlot->xAxis->setRange(adjustedXMin, adjustedXMax);

    // Clear existing manual grid lines
    ui->dataPlot->clearItems();

    // Draw X-axis grid lines dengan warna berbeda
    for (double x = ceil(adjustedXMin / 0.04) * 0.04; x <= adjustedXMax; x += 0.04) {
        QCPItemLine *lineX = new QCPItemLine(ui->dataPlot);
        lineX->start->setCoords(x, -2.5);
        lineX->end->setCoords(x, 2.5);
        
        // Perbaiki pengecekan untuk grid merah
        double remainder = fmod(x + 0.001, 0.2); // Tambah offset kecil untuk floating point precision
        if (remainder < 0.002 || remainder > 0.198) { // Grid besar setiap 0.2s
            lineX->setPen(QPen(Qt::red, 1.0));   // Merah untuk grid besar
        } else {                    // Grid kecil setiap 0.04s
            lineX->setPen(QPen(Qt::blue, 0.5)); // Biru untuk grid kecil
        }
    }

    // Draw Y-axis grid lines dengan warna berbeda
    for (double y = -2.5; y <= 2.5; y += 0.1) {
        QCPItemLine *lineY = new QCPItemLine(ui->dataPlot);
        lineY->start->setCoords(adjustedXMin, y);
        lineY->end->setCoords(adjustedXMax, y);
        
        // Perbaiki pengecekan untuk grid merah Y
        double yAdjusted = y + 2.5; // Shift to positive range
        double yRemainder = fmod(yAdjusted + 0.001, 0.5);
        if (yRemainder < 0.002 || yRemainder > 0.498) { // Grid besar setiap 0.5V
            lineY->setPen(QPen(Qt::red, 1.0));   // Merah untuk grid besar
        } else {                           // Grid kecil setiap 0.1V
            lineY->setPen(QPen(Qt::blue, 0.5)); // Biru untuk grid kecil
        }
    }

    ui->dataPlot->replot(QCustomPlot::rpQueuedReplot);
}

void MainWindow::openSelectedPort()
{
    if (serial->isOpen())
        serial->close();

    QString selectedPort = ui->PortAvailable->currentText();
    if (selectedPort.isEmpty())
    {
        qDebug() << "No port selected";
        return;
    }

    serial->setPortName(selectedPort);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (!serial->open(QIODevice::ReadWrite))
    {
        qDebug() << "Failed to open port:" << serial->errorString();
    }
    else
    {
        qDebug() << "Successfully opened port:" << selectedPort;
    }
}

void MainWindow::readSerialData()
{
    // Only process data if we're actively acquiring
    if (!isAcquiring)
    {
        return;
    }

    qDebug() << "readSerialData called"; // Add debugging

    while (serial->canReadLine())
    {
        QByteArray line = serial->readLine().trimmed();
        qDebug() << "Received line:" << line; // Add debugging

        bool ok;
        double raw_value = line.toDouble(&ok);

        if (ok)
        {
            // Fix: Use raw_value instead of undefined 'value'
            float raw_voltage = raw_value / 256.0 * 3.0;

            qDebug() << "Raw value:" << raw_value << "Raw voltage:" << raw_voltage; // Add debugging

            // Update baseline window
            baselineWindow.append(raw_voltage);
            if (baselineWindow.size() > baselineWindowSize)
                baselineWindow.removeFirst();

            // Compute baseline
            double baseline = std::accumulate(baselineWindow.begin(), baselineWindow.end(), 0.0) / baselineWindow.size();

            // Center around baseline
            float voltage = raw_voltage - baseline;

            currentTime += 0.01;
            xData.append(currentTime);
            yData.append(voltage);

            // Keep only last 5 seconds of data (ubah dari 10.0 ke 5.0)
            while (!xData.isEmpty() && xData.first() < currentTime - 3.0)
            {
                xData.removeFirst();
                yData.removeFirst();
            }

            qDebug() << "Added point - Time:" << currentTime << "Voltage:" << voltage; // Add debugging
        }
        else
        {
            qDebug() << "Invalid line:" << line;
        }
    }
}

void MainWindow::on_acquisition_clicked()
{
    qDebug() << "Acquisition button clicked";

    if (!isAcquiring)
    {
        // Start acquisition
        if (serial->isOpen())
        {
            QByteArray command = "1";
            qDebug() << "Sending command to start acquisition";
            serial->write(command);
            isAcquiring = true;
            ui->acquisition->setText("Running...");
            ui->acquisition->setEnabled(false); // Disable while running
            ui->StopBtn->setEnabled(true);      // Enable stop button
        }
        else
        {
            qDebug() << "Port not open, attempting to open";
            openSelectedPort();
            if (serial->isOpen())
            {
                QByteArray command = "1";
                qDebug() << "Port opened, sending command";
                serial->write(command);
                isAcquiring = true;
                ui->acquisition->setText("Running...");
                ui->acquisition->setEnabled(false);
                ui->StopBtn->setEnabled(true);
            }
            else
            {
                qDebug() << "Failed to open port for acquisition";
            }
        }
    }
}

void MainWindow::on_StopBtn_clicked()
{
    qDebug() << "Stop button clicked";

    if (isAcquiring)
    {
        // Stop acquisition
        isAcquiring = false;

        // Send stop command to device
        if (serial->isOpen())
        {
            QByteArray command = "0"; // Assuming "0" stops the device
            qDebug() << "Sending stop command to device";
            serial->write(command);
        }

        // Update UI
        ui->acquisition->setText("proceed");
        ui->acquisition->setEnabled(true); // Re-enable start button
        ui->StopBtn->setEnabled(false);    // Disable stop button

        qDebug() << "Data acquisition stopped. Total points collected:" << xData.size();
    }
}
