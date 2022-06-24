#!/bin/sh

rm -rf /tmp/asterixInspector.AppDir
mkdir -p /tmp/asterixInspector.AppDir
mkdir -p /tmp/asterixInspector.AppDir/usr/bin
mkdir -p /tmp/asterixInspector.AppDir/usr/share/asterixInspector
cp asterixInspector /tmp/asterixInspector.AppDir/usr/bin/
cp asterixInspector.desktop /tmp/asterixInspector.AppDir/
cp images/radar.svg /tmp/asterixInspector.AppDir/asterixInspector.svg
cp -r asterixSpecification /tmp/asterixInspector.AppDir/usr/share/asterixInspector/
cp -r examples /tmp/asterixInspector.AppDir/usr/share/asterixInspector/

linuxdeployqt-continuous-x86_64.AppImage /tmp/asterixInspector.AppDir/asterixInspector.desktop  -qmake=/usr/bin/qmake-qt5 -no-translations -appimage
