AM_CPPFLAGS = @VMOD_INCLUDES@ -Wall -Werror

vmoddir = @VMOD_DIR@
vmod_LTLIBRARIES = libvmod_str.la

libvmod_str_la_LDFLAGS = -module -export-dynamic -avoid-version -shared

libvmod_str_la_SOURCES = \
	vmod_str.c

nodist_libvmod_str_la_SOURCES = \
	vcc_if.c \
	vcc_if.h

vmod_str.lo: vcc_if.c vcc_if.h

vcc_if.c: vcc_if.h

vcc_if.h: @VMODTOOL@ $(top_srcdir)/src/vmod_str.vcc
	@VMODTOOL@ $(top_srcdir)/src/vmod_str.vcc

VMOD_TESTS = $(top_srcdir)/src/tests/*.vtc
.PHONY: $(VMOD_TESTS)

$(top_srcdir)/src/tests/*.vtc: libvmod_str.la
	@VARNISHTEST@ -Dvarnishd=@VARNISHD@ -Dvmod_topbuild=$(abs_top_builddir) $@

check: $(VMOD_TESTS)

EXTRA_DIST = \
	vmod_str.vcc \
	$(VMOD_TESTS)

CLEANFILES = \
	$(builddir)/vcc_if.c \
	$(builddir)/vcc_if.h \
	$(builddir)/vmod_str.rst \
	$(builddir)/vmod_str.man.rst
