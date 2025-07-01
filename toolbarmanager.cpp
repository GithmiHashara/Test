#include "toolbarmanager.h"
#include "imuhealthmonitor.h"
#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QIcon>
#include <QStyle>
#include <QApplication>
#include <QFileInfo>
#include <QDir>

// Constants
const int ToolbarManager::DEFAULT_ICON_SIZE = 24;
const int ToolbarManager::COMPACT_ICON_SIZE = 16;
const int ToolbarManager::MIN_WINDOW_WIDTH_FOR_FULL_TOOLBAR = 800;

ToolbarManager::ToolbarManager(IMUHealthMonitor *parent)
    : QObject(parent)
    , m_monitor(parent)
    , m_mainWindow(qobject_cast<QMainWindow*>(parent))
    , m_toolbar(nullptr)
    , m_viewActionGroup(nullptr)
    , m_zoomInAction(nullptr)
    , m_zoomOutAction(nullptr)
    , m_fitToWindowAction(nullptr)
    , m_handToolAction(nullptr)
    , m_connectionAction(nullptr)
    , m_iconBasePath("D:/1-A Excel Tech/AAA UI/NEW_IMU_V01/Images/")
    , m_currentIconSize(DEFAULT_ICON_SIZE)
    , m_compactMode(false)
{
    // Initialize action group
    m_viewActionGroup = new QActionGroup(this);
}

ToolbarManager::~ToolbarManager()
{
}

void ToolbarManager::createToolbar()
{
    if (!m_mainWindow) return;
    
    // Create toolbar
    m_toolbar = m_mainWindow->addToolBar("Main Toolbar");
    m_toolbar->setObjectName("MainToolbar");
    m_toolbar->setMovable(false);
    m_toolbar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    
    // Setup actions and layout
    setupActions();
    setupToolbarLayout();
    loadIcons();
    optimizeIconSizes();
}

void ToolbarManager::setupActions()
{
    createZoomActions();
    createViewActions();
    createConnectionActions();
}

void ToolbarManager::createZoomActions()
{
    // Zoom In action
    m_zoomInAction = new QAction("Zoom In", this);
    m_zoomInAction->setToolTip("Zoom In (Ctrl++)");
    m_zoomInAction->setShortcut(QKeySequence::ZoomIn);
    connect(m_zoomInAction, &QAction::triggered, this, &ToolbarManager::onZoomIn);
    
    // Zoom Out action
    m_zoomOutAction = new QAction("Zoom Out", this);
    m_zoomOutAction->setToolTip("Zoom Out (Ctrl+-)");
    m_zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    connect(m_zoomOutAction, &QAction::triggered, this, &ToolbarManager::onZoomOut);
    
    // Fit to Window action
    m_fitToWindowAction = new QAction("Fit to Window", this);
    m_fitToWindowAction->setToolTip("Fit to Window (Ctrl+0)");
    m_fitToWindowAction->setShortcut(QKeySequence("Ctrl+0"));
    connect(m_fitToWindowAction, &QAction::triggered, this, &ToolbarManager::onFitToWindow);
}

void ToolbarManager::createViewActions()
{
    // Hand Tool action
    m_handToolAction = new QAction("Hand Tool", this);
    m_handToolAction->setToolTip("Hand Tool - Click and drag to pan (H)");
    m_handToolAction->setShortcut(QKeySequence("H"));
    m_handToolAction->setCheckable(true);
    m_handToolAction->setChecked(false);
    connect(m_handToolAction, &QAction::toggled, this, &ToolbarManager::onHandToolToggled);
    
    // Add to view action group
    m_viewActionGroup->addAction(m_handToolAction);
}

void ToolbarManager::createConnectionActions()
{
    // Connection toggle action
    m_connectionAction = new QAction("Connect", this);
    m_connectionAction->setToolTip("Connect/Disconnect to IMU Device (Ctrl+C)");
    m_connectionAction->setShortcut(QKeySequence("Ctrl+C"));
    m_connectionAction->setCheckable(true);
    m_connectionAction->setChecked(false);
    connect(m_connectionAction, &QAction::triggered, this, &ToolbarManager::onConnectionToggled);
}

void ToolbarManager::setupToolbarLayout()
{
    if (!m_toolbar) return;
    
    // Add zoom actions
    m_toolbar->addAction(m_zoomInAction);
    m_toolbar->addAction(m_zoomOutAction);
    m_toolbar->addAction(m_fitToWindowAction);
    
    // Add separator
    m_toolbar->addSeparator();
    
    // Add view actions
    m_toolbar->addAction(m_handToolAction);
    
    // Add separator
    m_toolbar->addSeparator();
    
    // Add connection action
    m_toolbar->addAction(m_connectionAction);
    
    // Add stretchable space at the end
    QWidget *spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_toolbar->addWidget(spacer);
}

void ToolbarManager::loadIcons()
{
    // Load custom icons if available, otherwise use system icons
    
    // Zoom In icon
    QString zoomInPath = m_iconBasePath + "zoomin.png";
    if (QFileInfo::exists(zoomInPath)) {
        m_zoomInAction->setIcon(QIcon(zoomInPath));
    } else {
        m_zoomInAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowUp));
    }
    
    // Zoom Out icon
    QString zoomOutPath = m_iconBasePath + "zoomout.png";
    if (QFileInfo::exists(zoomOutPath)) {
        m_zoomOutAction->setIcon(QIcon(zoomOutPath));
    } else {
        m_zoomOutAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowDown));
    }
    
    // Fit to Window icon
    QString fitToWindowPath = m_iconBasePath + "fittowindow.png";
    if (QFileInfo::exists(fitToWindowPath)) {
        m_fitToWindowAction->setIcon(QIcon(fitToWindowPath));
    } else {
        m_fitToWindowAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    }
    
    // Hand Tool icon
    QString handToolPath = m_iconBasePath + "handtool.png";
    if (QFileInfo::exists(handToolPath)) {
        m_handToolAction->setIcon(QIcon(handToolPath));
    } else {
        m_handToolAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_ArrowRight));
    }
    
    // Connection icon
    QString connectPath = m_iconBasePath + "connect.png";
    if (QFileInfo::exists(connectPath)) {
        m_connectionAction->setIcon(QIcon(connectPath));
    } else {
        m_connectionAction->setIcon(QApplication::style()->standardIcon(QStyle::SP_ComputerIcon));
    }
}

void ToolbarManager::setIconSizes(int size)
{
    if (m_toolbar) {
        m_toolbar->setIconSize(QSize(size, size));
        m_currentIconSize = size;
    }
}

void ToolbarManager::optimizeIconSizes()
{
    // Start with default icon size
    setIconSizes(DEFAULT_ICON_SIZE);
    
    // Reduce spacing and margins for better space utilization
    if (m_toolbar) {
        m_toolbar->setContentsMargins(2, 2, 2, 2);
        
        // Set stylesheet for compact button appearance
        m_toolbar->setStyleSheet(
            "QToolBar { spacing: 2px; border: 1px solid #c0c0c0; background: #f0f0f0; }"
            "QToolButton { border: 1px solid transparent; border-radius: 3px; margin: 1px; padding: 2px; }"
            "QToolButton:hover { border: 1px solid #0078d4; background: #e5f1fb; }"
            "QToolButton:pressed { border: 1px solid #005a9e; background: #cce4f7; }"
            "QToolButton:checked { border: 1px solid #0078d4; background: #cce4f7; }"
        );
    }
}

void ToolbarManager::updateForWindowSize(int width)
{
    if (width < MIN_WINDOW_WIDTH_FOR_FULL_TOOLBAR && !m_compactMode) {
        // Switch to compact mode
        m_compactMode = true;
        setIconSizes(COMPACT_ICON_SIZE);
        
        // Update tooltips to be more descriptive in compact mode
        m_zoomInAction->setToolTip("Zoom In (Ctrl++)");
        m_zoomOutAction->setToolTip("Zoom Out (Ctrl+-)");
        m_fitToWindowAction->setToolTip("Fit to Window (Ctrl+0)");
        m_handToolAction->setToolTip("Hand Tool (H)");
        m_connectionAction->setToolTip("Connect/Disconnect (Ctrl+C)");
        
    } else if (width >= MIN_WINDOW_WIDTH_FOR_FULL_TOOLBAR && m_compactMode) {
        // Switch back to full mode
        m_compactMode = false;
        setIconSizes(DEFAULT_ICON_SIZE);
    }
}

void ToolbarManager::updateConnectionState(bool connected)
{
    if (m_connectionAction) {
        m_connectionAction->setChecked(connected);
        m_connectionAction->setText(connected ? "Disconnect" : "Connect");
        m_connectionAction->setToolTip(connected ? 
            "Disconnect from IMU Device (Ctrl+C)" : 
            "Connect to IMU Device (Ctrl+C)");
    }
}

void ToolbarManager::updateHandToolState(bool enabled)
{
    if (m_handToolAction) {
        m_handToolAction->setChecked(enabled);
    }
}

// Action handlers
void ToolbarManager::onZoomIn()
{
    if (m_monitor) {
        m_monitor->zoomIn();
    }
}

void ToolbarManager::onZoomOut()
{
    if (m_monitor) {
        m_monitor->zoomOut();
    }
}

void ToolbarManager::onFitToWindow()
{
    if (m_monitor) {
        m_monitor->fitToWindow();
    }
}

void ToolbarManager::onHandToolToggled(bool enabled)
{
    if (m_monitor) {
        m_monitor->setHandToolEnabled(enabled);
    }
}

void ToolbarManager::onConnectionToggled()
{
    if (m_monitor) {
        m_monitor->toggleConnection();
    }
}

