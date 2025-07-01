#include "imuhealthmonitor.h"
#include "toolbarmanager.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QLabel>
#include <QStatusBar>
#include <QResizeEvent>
#include <QTimer>
#include <QScrollBar>
#include <QGraphicsProxyWidget>
#include <QTransform>
#include <cmath>

// Constants
const double IMUHealthMonitor::MIN_ZOOM_FACTOR = 0.1;
const double IMUHealthMonitor::MAX_ZOOM_FACTOR = 5.0;
const double IMUHealthMonitor::ZOOM_STEP = 0.2;

IMUHealthMonitor::IMUHealthMonitor(QWidget *parent)
    : QMainWindow(parent)
    , m_centralWidget(nullptr)
    , m_mainLayout(nullptr)
    , m_mainView(nullptr)
    , m_scene(nullptr)
    , m_statusLabel(nullptr)
    , m_connectionLabel(nullptr)
    , m_toolbarManager(nullptr)
    , m_handToolEnabled(false)
    , m_isConnected(false)
    , m_currentZoomFactor(1.0)
    , m_updateTimer(nullptr)
{
    setWindowTitle("IMU Health Monitor");
    setMinimumSize(800, 600);
    resize(1200, 800);
    
    setupUI();
    
    // Create toolbar manager
    m_toolbarManager = new ToolbarManager(this);
    m_toolbarManager->createToolbar();
    
    // Setup update timer for IMU data
    m_updateTimer = new QTimer(this);
    connect(m_updateTimer, &QTimer::timeout, this, &IMUHealthMonitor::updateIMUData);
    m_updateTimer->start(100); // 10 Hz update rate
}

IMUHealthMonitor::~IMUHealthMonitor()
{
    if (m_updateTimer) {
        m_updateTimer->stop();
    }
}

void IMUHealthMonitor::setupUI()
{
    // Create central widget
    m_centralWidget = new QWidget(this);
    setCentralWidget(m_centralWidget);
    
    // Create main layout
    m_mainLayout = new QVBoxLayout(m_centralWidget);
    m_mainLayout->setContentsMargins(5, 5, 5, 5);
    m_mainLayout->setSpacing(5);
    
    // Setup main view
    setupMainView();
    
    // Create status bar
    createStatusBar();
}

void IMUHealthMonitor::setupMainView()
{
    // Create graphics scene
    m_scene = new QGraphicsScene(this);
    m_scene->setBackgroundBrush(QBrush(QColor(240, 240, 240)));
    
    // Create graphics view
    m_mainView = new QGraphicsView(m_scene, this);
    m_mainView->setRenderHint(QPainter::Antialiasing);
    m_mainView->setDragMode(QGraphicsView::NoDrag);
    m_mainView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mainView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    
    // Add some sample IMU visualization elements
    QGraphicsEllipseItem *accelIndicator = m_scene->addEllipse(0, 0, 100, 100, 
        QPen(QColor(0, 120, 215), 2), QBrush(QColor(0, 120, 215, 100)));
    accelIndicator->setPos(50, 50);
    
    QGraphicsEllipseItem *gyroIndicator = m_scene->addEllipse(0, 0, 80, 80,
        QPen(QColor(255, 140, 0), 2), QBrush(QColor(255, 140, 0, 100)));
    gyroIndicator->setPos(200, 50);
    
    QGraphicsTextItem *accelLabel = m_scene->addText("Accelerometer", QFont("Arial", 10));
    accelLabel->setPos(60, 160);
    
    QGraphicsTextItem *gyroLabel = m_scene->addText("Gyroscope", QFont("Arial", 10));
    gyroLabel->setPos(215, 140);
    
    // Set scene rect
    m_scene->setSceneRect(0, 0, 400, 300);
    
    // Add to layout
    m_mainLayout->addWidget(m_mainView);
}

void IMUHealthMonitor::createStatusBar()
{
    // Create status labels
    m_statusLabel = new QLabel("Ready", this);
    m_connectionLabel = new QLabel("Disconnected", this);
    m_connectionLabel->setStyleSheet("color: red; font-weight: bold;");
    
    // Add to status bar
    statusBar()->addWidget(m_statusLabel);
    statusBar()->addPermanentWidget(m_connectionLabel);
}

void IMUHealthMonitor::zoomIn()
{
    if (m_currentZoomFactor < MAX_ZOOM_FACTOR) {
        m_currentZoomFactor += ZOOM_STEP;
        m_mainView->resetTransform();
        m_mainView->scale(m_currentZoomFactor, m_currentZoomFactor);
        m_statusLabel->setText(QString("Zoom: %1%").arg(qRound(m_currentZoomFactor * 100)));
    }
}

void IMUHealthMonitor::zoomOut()
{
    if (m_currentZoomFactor > MIN_ZOOM_FACTOR) {
        m_currentZoomFactor -= ZOOM_STEP;
        m_mainView->resetTransform();
        m_mainView->scale(m_currentZoomFactor, m_currentZoomFactor);
        m_statusLabel->setText(QString("Zoom: %1%").arg(qRound(m_currentZoomFactor * 100)));
    }
}

void IMUHealthMonitor::fitToWindow()
{
    if (m_mainView && m_scene) {
        m_mainView->fitInView(m_scene->sceneRect(), Qt::KeepAspectRatio);
        QTransform transform = m_mainView->transform();
        m_currentZoomFactor = transform.m11(); // Get scale factor
        m_statusLabel->setText("Fit to Window");
    }
}

void IMUHealthMonitor::resetZoom()
{
    m_currentZoomFactor = 1.0;
    m_mainView->resetTransform();
    m_statusLabel->setText("Zoom: 100%");
}

void IMUHealthMonitor::setHandToolEnabled(bool enabled)
{
    m_handToolEnabled = enabled;
    if (m_mainView) {
        if (enabled) {
            m_mainView->setDragMode(QGraphicsView::ScrollHandDrag);
            m_mainView->setCursor(Qt::OpenHandCursor);
            m_statusLabel->setText("Hand Tool Active - Click and drag to pan");
        } else {
            m_mainView->setDragMode(QGraphicsView::NoDrag);
            m_mainView->setCursor(Qt::ArrowCursor);
            m_statusLabel->setText("Hand Tool Disabled");
        }
    }
}

void IMUHealthMonitor::toggleConnection()
{
    if (m_isConnected) {
        disconnectFromDevice();
    } else {
        connectToDevice();
    }
}

void IMUHealthMonitor::connectToDevice()
{
    m_isConnected = true;
    m_connectionLabel->setText("Connected");
    m_connectionLabel->setStyleSheet("color: green; font-weight: bold;");
    m_statusLabel->setText("Connected to IMU device");
    
    // Update toolbar state
    if (m_toolbarManager) {
        m_toolbarManager->updateConnectionState(true);
    }
}

void IMUHealthMonitor::disconnectFromDevice()
{
    m_isConnected = false;
    m_connectionLabel->setText("Disconnected");
    m_connectionLabel->setStyleSheet("color: red; font-weight: bold;");
    m_statusLabel->setText("Disconnected from IMU device");
    
    // Update toolbar state
    if (m_toolbarManager) {
        m_toolbarManager->updateConnectionState(false);
    }
}

void IMUHealthMonitor::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    
    // Update toolbar for window size changes
    if (m_toolbarManager) {
        m_toolbarManager->updateForWindowSize(event->size().width());
    }
}

void IMUHealthMonitor::updateIMUData()
{
    // Simulate IMU data updates
    static int counter = 0;
    counter++;
    
    if (m_isConnected) {
        m_statusLabel->setText(QString("IMU Data Update #%1").arg(counter));
        
        // Here you would typically update the visualization with real IMU data
        // For now, we'll just show that the system is active
    }
}

