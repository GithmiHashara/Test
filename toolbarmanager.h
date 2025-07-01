#ifndef TOOLBARMANAGER_H
#define TOOLBARMANAGER_H

#include <QToolBar>
#include <QAction>
#include <QActionGroup>
#include <QObject>

class IMUHealthMonitor;
class QMainWindow;

class ToolbarManager : public QObject
{
    Q_OBJECT

public:
    explicit ToolbarManager(IMUHealthMonitor *parent);
    ~ToolbarManager();
    
    // Setup methods
    void createToolbar();
    void setupActions();
    void setupToolbarLayout();
    
    // Responsive design methods
    void updateForWindowSize(int width);
    void optimizeIconSizes();
    
    // State management
    void updateConnectionState(bool connected);
    void updateHandToolState(bool enabled);

private slots:
    // Action handlers
    void onZoomIn();
    void onZoomOut();
    void onFitToWindow();
    void onHandToolToggled(bool enabled);
    void onConnectionToggled();

private:
    void createZoomActions();
    void createViewActions();
    void createConnectionActions();
    void addSeparators();
    void loadIcons();
    void setIconSizes(int size);
    
    // Parent reference
    IMUHealthMonitor *m_monitor;
    QMainWindow *m_mainWindow;
    
    // Toolbar
    QToolBar *m_toolbar;
    
    // Action groups
    QActionGroup *m_viewActionGroup;
    
    // Individual actions
    QAction *m_zoomInAction;
    QAction *m_zoomOutAction;
    QAction *m_fitToWindowAction;
    QAction *m_handToolAction;
    QAction *m_connectionAction;
    
    // Icon management
    QString m_iconBasePath;
    int m_currentIconSize;
    bool m_compactMode;
    
    // Layout constants
    static const int DEFAULT_ICON_SIZE;
    static const int COMPACT_ICON_SIZE;
    static const int MIN_WINDOW_WIDTH_FOR_FULL_TOOLBAR;
};

#endif // TOOLBARMANAGER_H