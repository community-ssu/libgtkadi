AM_CFLAGS = -I$(top_srcdir)/include $(CHECK_CFLAGS) $(GTK_CFLAGS) $(HILDON_CFLAGS) $(COVERAGE_CFLAGS)
AM_LDFLAGS = $(CHECK_LIBS) $(GTK_LIBS) $(HILDON_LIBS) $(COVERAGE_LIBS)
LDADD = $(top_builddir)/src/libgtkadi.la
#TESTS_ENVIRONMENT = CK_VERBOSITY=verbose $(LIBTOOL) --mode=execute

check_adi_SOURCES = runner.c check_adi.c
check_menu_SOURCES = runner.c check_menu.c
check_toolbar_SOURCES = runner.c check_toolbar.c

check_PROGRAMS = check_adi \
				 check_menu \
				 check_toolbar
#TESTS=$(check_PROGRAMS)

check-local: $(check_PROGRAMS)
	af-sb-init.sh start
	for x in $(check_PROGRAMS) ; do \
		$(LCOV) -q -d $(top_builddir) -z; \
		./$$x $$x.xml; \
		$(LCOV) -q -d $(top_builddir) -o $$x.info -t $$x -c; \
		{ lcov -l $$x.info | grep -v "`cd $(top_builddir) && pwd`"; \
		} | cut -d: -f1 | xargs lcov -r $$x.info > $$x.lcov; \
	done
	$(GENHTML) --num-spaces=2 --show-details -k -t "GtkAdi" -o coverage *.lcov
	af-sb-init.sh stop

clean-local:
	rm -rf $(check_PROGRAMS) *.o *.lo *.la *.i
	rm -rf .debs .libs
	rm -rf coverage *.gcno *.gcda *.lcov
	rm -rf *.log
