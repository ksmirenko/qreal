QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin

DESTDIR = $$PWD/../../../../../bin/plugins/tools/kitPlugins/
TARGET = robots-nxt-osek-c-generator

MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

LIBS += -L../../../../../bin -lqrkernel -lqslog -lqrutils -lqrrepo -lrobots-generator-base

INCLUDEPATH += \
	$$PWD/../../generatorBase/include/ \
	$$PWD/../../../interpreters/interpreterBase/include \
	$$PWD/../../../../../ \
	$$PWD/../../../../../qrgui \
	$$PWD/../../../../../qrtext/include \

# workaround for http://bugreports.qt.nokia.com/browse/QTBUG-8110
# when fixed it would become possible to use QMAKE_LFLAGS_RPATH
!macx {
	QMAKE_LFLAGS += -Wl,-O1,-rpath,$$PWD/../../../../../bin/
	QMAKE_LFLAGS += -Wl,-rpath,$$PWD/../../../../../bin/plugins/
}

TRANSLATIONS = $$PWD/../../../../../qrtranslations/ru/plugins/robots/nxtGenerator_ru.ts

HEADERS += \
	$$PWD/nxtOsekCGeneratorPlugin.h \
	$$PWD/nxtOsekCMasterGenerator.h \
	$$PWD/nxtOsekCGeneratorCustomizer.h \
	$$PWD/nxtOsekCGeneratorFactory.h \
	$$PWD/nxtFlashTool.h \
	$$PWD/converters/nxtStringPropertyConverter.h \

SOURCES += \
	$$PWD/nxtOsekCGeneratorPlugin.cpp \
	$$PWD/nxtOsekCMasterGenerator.cpp \
	$$PWD/nxtOsekCGeneratorCustomizer.cpp \
	$$PWD/nxtOsekCGeneratorFactory.cpp \
	$$PWD/nxtFlashTool.cpp \
	$$PWD/converters/nxtStringPropertyConverter.cpp \

RESOURCES = \
	$$PWD/nxtOsekCGenerator.qrc \
	$$PWD/templates.qrc \
