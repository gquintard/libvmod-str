AM_CFLAGS = $(VARNISHAPI_CFLAGS) -Wall -Werror
AM_LDFLAGS = $(VARNISHAPI_LIBS) $(VMOD_LDFLAGS) $(CODE_COVERAGE_LDFLAGS)

vmod_LTLIBRARIES = libvmod_str.la

libvmod_str_la_SOURCES = \
	vmod_str.c

nodist_libvmod_str_la_SOURCES = \
	vcc_str_if.c \
	vcc_str_if.h

dist_man_MANS = vmod_str.3

@BUILD_VMOD_STR@

AM_TESTS_ENVIRONMENT = \
	PATH="$(VMOD_TEST_PATH)" \
	LD_LIBRARY_PATH="$(VARNISH_LIBRARY_PATH)"
TEST_EXTENSIONS = .vtc
VTC_LOG_COMPILER = varnishtest -v
AM_VTC_LOG_FLAGS = -Dvmod_str="$(VMOD_STR)"

TESTS = @VMOD_TESTS@

EXTRA_DIST = \
	vmod_str.vcc \
	$(VMOD_TESTS)
