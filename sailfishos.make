# -*- coding: us-ascii-unix -*-

NAME       = harbour-vaievem-finder
VERSION    =

DESTDIR    =
BUILDDIR   =
PREFIX     = /usr
DATADIR    = $(DESTDIR)$(PREFIX)/share/$(NAME)
DESKTOPDIR = $(DESTDIR)$(PREFIX)/share/applications
ICONDIR    = $(DESTDIR)$(PREFIX)/share/icons/hicolor
BINDIR     = $(DESTDIR)/$(PREFIX)/bin

MAKETHIS = $(MAKE) -f sailfishos.make

ICONS = data/vaievem-finder-86x86.png \
	data/vaievem-finder-108x108.png \
	data/vaievem-finder-128x128.png \
	data/vaievem-finder-256x256.png \
	qml/icons/alpha-100.png \
	qml/icons/alpha-125.png \
	qml/icons/alpha-150.png \
	qml/icons/alpha-175.png \
	qml/icons/alpha-200.png \
	qml/icons/busstop-100.png \
	qml/icons/busstop-125.png \
	qml/icons/busstop-150.png \
	qml/icons/busstop-175.png \
	qml/icons/busstop-200.png \
	qml/icons/crosshairs-100.png \
	qml/icons/crosshairs-125.png \
	qml/icons/crosshairs-150.png \
	qml/icons/crosshairs-175.png \
	qml/icons/crosshairs-200.png \
	qml/icons/first-100.png \
	qml/icons/first-125.png \
	qml/icons/first-150.png \
	qml/icons/first-175.png \
	qml/icons/first-200.png \
	qml/icons/go-100.png \
	qml/icons/go-125.png \
	qml/icons/go-150.png \
	qml/icons/go-175.png \
	qml/icons/go-200.png \
	qml/icons/omega-100.png \
	qml/icons/omega-125.png \
	qml/icons/omega-150.png \
	qml/icons/omega-175.png \
	qml/icons/omega-200.png \
	qml/icons/runner-100.png \
	qml/icons/runner-125.png \
	qml/icons/runner-150.png \
	qml/icons/runner-175.png \
	qml/icons/runner-200.png \
	qml/icons/second-100.png \
	qml/icons/second-125.png \
	qml/icons/second-150.png \
	qml/icons/second-175.png \
	qml/icons/second-200.png

clean:
	rm -rf dist
	rm -rf __pycache__ */__pycache__ */*/__pycache__
	rm -rf .cache */.cache */*/.cache
	rm -f rpm/*.rpm
	rm -rf qml/icons
	rm -f data/*.png

dist:
	$(MAKETHIS) clean
	mkdir -p dist/$(NAME)-$(VERSION)
	cp --parents `git ls-files` dist/$(NAME)-$(VERSION)
	tar -C dist -cjf dist/$(NAME)-$(VERSION).tar.bz2 $(NAME)-$(VERSION)


install: $(ICONS)
	@echo "Installing Python files..."
	mkdir -p $(DATADIR)/poor
	cp poor/*.py $(DATADIR)/poor

	@echo "Installing QML files..."
	mkdir -p $(DATADIR)/qml
	cp qml/vaievem-finder.qml $(DATADIR)/qml/$(NAME).qml
	cp qml/[ABCDEFGHIJKLMNOPQRSTUVXYZ]*.qml $(DATADIR)/qml
	cp qml/*.js $(DATADIR)/qml
	mkdir -p $(DATADIR)/qml/icons
	cp qml/icons/*.png $(DATADIR)/qml/icons
	cp qml/*.log $(DATADIR)/qml

	@echo "Installing tilesources..."
	mkdir -p $(DATADIR)/tilesources
	cp tilesources/*.json $(DATADIR)/tilesources
	cp tilesources/[!_]*.py $(DATADIR)/tilesources

	@echo "Installing timetables..."
	cp data/timetable.db $(DATADIR)

	@echo "Installing desktop file..."
	mkdir -p $(DESKTOPDIR)
	cp data/$(NAME).desktop $(DESKTOPDIR)

	@echo "Installing icons..."
	mkdir -p $(ICONDIR)/86x86/apps
	mkdir -p $(ICONDIR)/108x108/apps
	mkdir -p $(ICONDIR)/128x128/apps
	mkdir -p $(ICONDIR)/256x256/apps
	cp data/vaievem-finder-86x86.png  $(ICONDIR)/86x86/apps/$(NAME).png
	cp data/vaievem-finder-108x108.png $(ICONDIR)/108x108/apps/$(NAME).png
	cp data/vaievem-finder-128x128.png $(ICONDIR)/128x128/apps/$(NAME).png
	cp data/vaievem-finder-256x256.png $(ICONDIR)/256x256/apps/$(NAME).png

	@echo "Installing binary..."
	mkdir -p $(BINDIR)
	cp $(BUILDDIR)/harbour-vaievem-finder $(BINDIR)
	chmod 755 $(BINDIR)/harbour-vaievem-finder


rpm:
	$(MAKETHIS) dist
	mkdir -p $$HOME/rpmbuild/SOURCES
	cp dist/$(NAME)-$(VERSION).tar.bz2 $$HOME/rpmbuild/SOURCES
	rm -rf $$HOME/rpmbuild/BUILD/$(NAME)-$(VERSION)
	rpmbuild -ba --nodeps rpm/$(NAME).spec
	cp $$HOME/rpmbuild/RPMS/noarch/$(NAME)-$(VERSION)-*.rpm rpm
	cp $$HOME/rpmbuild/SRPMS/$(NAME)-$(VERSION)-*.rpm rpm


%.png:
	svg/svg2png $@

icons: $(ICONS)


.PHONY: dist install rpm clean icons

