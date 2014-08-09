TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    levnode.c \
    levtree.c \
    wlevnode.c \
    wlevtree.c \
    levtree_standing.c \

HEADERS += \
    levnode.h \
    levtree.h \
    wlevnode.h \
    wlevtree.h \
    levtree_standing.h \
    common.h

OTHER_FILES += \
    setup.py\
    pylevtree.c\
    pywlevtree.c\
    python3.c\



