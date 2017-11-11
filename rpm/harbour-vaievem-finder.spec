# Prevent brp-python-bytecompile from running.
%define __os_install_post %{___build_post}

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

Name: harbour-vaievem-finder

Summary:    VaieVem@Portimão bus schedule finder
Version:    0.007
Release: 67
Group:      Qt/Qt
License:    GPLv3
URL:        https://github.com/jusirkka/vaievem-finder
Source0:    %{name}-%{version}.tar.bz2
Requires: pyotherside-qml-plugin-python3-qt5 >= 1.2
Requires: qt5-plugin-geoservices-here
Requires: qt5-qtdeclarative-import-location
Requires: qt5-qtdeclarative-import-positioning >= 5.2
Requires: sailfishsilica-qt5
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.3
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5Sql)


%description
Find Vai e Vem bus schedules around Portimão, Portugal by pointing start and endpoints of the journey in the map.

%prep
%setup -q -n %{name}-%{version}

%build

mkdir -p rpmbuilddir-%{_arch}
cd rpmbuilddir-%{_arch} && %{qtc_qmake5} ../%{name}.pro
cd ..
make -C rpmbuilddir-%{_arch} VERBOSE=1


%install
make -f sailfishos.make DESTDIR=%{buildroot} VERSION=%{version} PREFIX=/usr BUILDDIR=rpmbuilddir-%{_arch} install

%files
%defattr(-,root,root,-)
%{_bindir}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
