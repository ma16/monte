# BSD 2-Clause License, see github.com/ma16/monte

# Please refer to: http://doc.qt.io/qt-5/qmake-variable-reference.html

QT += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = monte
TEMPLATE = app

SOURCES += \
	qt/main.cpp \
	qt/AboutWidget.cpp \
	qt/AxisControl.cpp \
	qt/ChartWidget.cpp \
	qt/LogWidget.cpp \
	qt/MainWindow.cpp \
	qt/MonitorWidget.cpp

HEADERS += \
	qt/AboutWidget.h \
	qt/AxisControl.h \
	qt/ChartWidget.h \
	qt/LogWidget.h \
	qt/MainWindow.h \
	qt/MonitorWidget.h

FORMS += \
	qt/LogWidget.ui \
	qt/MainWindow.ui \
	qt/MonitorWidget.ui

SOURCES += \
	Device/Ds18b20/Bang.cc \
	Device/Ds18b20/Recorder.cc \
	Neat/Logger.cc \
	Neat/NotSigned.cc \
	Neat/safe_int.cc \
	Posix/base.cc \
	Posix/Fd.cc \
	Posix/MMap.cc \
	Protocol/OneWire/Bang/Addressing.cc \
	Protocol/OneWire/Bang/Error.cc \
	Protocol/OneWire/Bang/Master.cc \
	Protocol/OneWire/Bang/Signaling.cc \
	Protocol/OneWire/Bang/Timing.cc \
	Rpi/Counter.cc \
	Rpi/Gpio.cc \
	Rpi/Page.cc \
	Rpi/Peripheral.cc \
	RpiExt/BangIo.cc

CONFIG += object_parallel_to_source
# otherwise identic file names (in different paths) overwrite each other

OBJECTS_DIR = build
MOC_DIR     = build
UI_DIR      = build
# if not set we get annoying warnings (if object_parallel_to_source):
# QDir::mkpath: Empty or null file name
# WARNING: Cannot create directory ''

isEmpty(QTSYSDIR){
QTSYSDIR = /opt/qt/5_9_1_static
}

QMAKE_CXXFLAGS += \
	-fno-strict-aliasing \
	-g \
	-I. \
	-isystem $$QTSYSDIR/include \
	-isystem $$QTSYSDIR/include/QtCore \
	-isystem $$QTSYSDIR/include/QtGui \
	-isystem $$QTSYSDIR/include/QtWidgets \
	-O2 \
	-pedantic \
	-std=c++11 \
	-Wall \
	-Wconversion \
	-Wextra \
	-Wsign-conversion \
	-Wstrict-aliasing
