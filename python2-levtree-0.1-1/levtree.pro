TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    levnode.c \
    levtree.c \
    wlevnode.c \
    wlevtree.c

HEADERS += \
    levnode.h \
    levtree.h \
    wlevnode.h \
    wlevtree.h \
    levtree_standing.h

OTHER_FILES += \
    pymodule.c\
    setup.py



