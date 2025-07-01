QT += core widgets

CONFIG += c++17

TARGET = IMUHealthMonitor
TEMPLATE = app

# Source files
SOURCES += \
    main.cpp \
    imuhealthmonitor.cpp \
    toolbarmanager.cpp

# Header files
HEADERS += \
    imuhealthmonitor.h \
    toolbarmanager.h

# Set application icon and properties
win32 {
    RC_ICONS = icon.ico
    VERSION = 1.0.0.0
    QMAKE_TARGET_COMPANY = "IMU Systems"
    QMAKE_TARGET_PRODUCT = "IMU Health Monitor"
    QMAKE_TARGET_DESCRIPTION = "IMU Health Monitoring Application"
}

# Deployment settings
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/IMUHealthMonitor
INSTALLS += target