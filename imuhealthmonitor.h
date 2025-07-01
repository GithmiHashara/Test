#ifndef IMUHEALTHMONITOR_H
#define IMUHEALTHMONITOR_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>

class ToolbarManager;

class IMUHealthMonitor : public QMainWindow
{
    Q_OBJECT

public:
    IMUHealthMonitor(QWidget *parent = nullptr);
    ~IMUHealthMonitor();

    // Zoom and view control methods
    void zoomIn();
    void zoomOut();
    void fitToWindow();
    void resetZoom();
    
    // Pan/Hand tool methods
    void setHandToolEnabled(bool enabled);
    bool isHandToolEnabled() const { return m_handToolEnabled; }
    
    // Connection methods
    void toggleConnection();
    bool isConnected() const { return m_isConnected; }
    void connectToDevice();
    void disconnectFromDevice();
    
    // View management
    QGraphicsView* getMainView() const { return m_mainView; }

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateIMUData();

private:
    void setupUI();
    void setupMainView();
    void createStatusBar();
    
    // UI Components
    QWidget *m_centralWidget;
    QVBoxLayout *m_mainLayout;
    QGraphicsView *m_mainView;
    QGraphicsScene *m_scene;
    QLabel *m_statusLabel;
    QLabel *m_connectionLabel;
    
    // Toolbar
    ToolbarManager *m_toolbarManager;
    
    // State variables
    bool m_handToolEnabled;
    bool m_isConnected;
    double m_currentZoomFactor;
    QTimer *m_updateTimer;
    
    // Constants
    static const double MIN_ZOOM_FACTOR;
    static const double MAX_ZOOM_FACTOR;
    static const double ZOOM_STEP;
};

#endif // IMUHEALTHMONITOR_H