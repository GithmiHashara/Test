#include "imuhealthmonitor.h"
#include <QApplication>
#include <QStyleFactory>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("IMU Health Monitor");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("IMU Systems");
    
    // Set a modern style
    app.setStyle(QStyleFactory::create("Fusion"));
    
    // Apply a modern dark theme
    app.setStyleSheet(
        "QMainWindow { background-color: #f0f0f0; }"
        "QToolBar { background-color: #ffffff; border: 1px solid #d0d0d0; }"
        "QStatusBar { background-color: #ffffff; border-top: 1px solid #d0d0d0; }"
        "QGraphicsView { border: 1px solid #d0d0d0; background-color: #ffffff; }"
    );
    
    // Create and show main window
    IMUHealthMonitor monitor;
    monitor.show();
    
    return app.exec();
}