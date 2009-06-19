Name:			plotutils-hist
Version:		0.0
Release:		1%{?dist}
Summary: 		Histograms!
License:		UNKOWN
Group:     Applications/Productivity
Source:			%{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

%description
hist is to make histograms!

%prep
%setup -q

%build 
%configure
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{_bindir}/hist
