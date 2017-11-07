# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application


TEMPLATE = app

TARGET = harbour-vaievem
CONFIG += c++11 sailfishapp
QT += qml quick sql positioning

SOURCES = src/main.cpp \
    src/timetable.cpp \
    src/database.cpp

HEADERS = \
    src/timetable.h \
    src/database.h


DISTFILES += qml/harbour-vaievem.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    rpm/harbour-vaievem.changes.in \
    rpm/harbour-vaievem.changes.run.in \
    rpm/harbour-vaievem.spec \
    rpm/harbour-vaievem.yaml \
    translations/*.ts \
    harbour-vaievem.desktop \
    qml/pages/utils.js \
    qml/pages/Python.qml \
    qml/pages/Tile.qml \
    poor/storage.py \
    poor/geocoder.py \
    poor/polysimp.py \
    poor/__init__.py \
    poor/tilesource.py \
    poor/i18n.py \
    poor/paths.py \
    poor/cache.py \
    poor/util.py \
    poor/config.py \
    poor/tilecollection.py \
    poor/http.py \
    poor/attrdict.py \
    poor/history.py \
    poor/guide.py \
    poor/router.py \
    poor/narrative.py \
    poor/application.py \
    tilesources/slippy.py \
    tilesources/openstreetmap_@1x.json \
    qml/pages/StateButton.qml \
    qml/pages/Selection.qml \
    qml/pages/Alpha.qml \
    qml/pages/Omega.qml


SAILFISHAPP_ICONS = 86x86 108x108 128x128 256x256

# to disable building translations every time, comment out the
# following CONFIG line
# CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
# TRANSLATIONS += translations/harbour-vaievem-de.ts
