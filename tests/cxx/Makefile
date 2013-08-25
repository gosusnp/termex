CXX=clang++
top_srcdir=../..

check: tst tst_d
	./tst
	valgrind ./tst_d

tst_d: tst.cc $(top_srcdir)/include/*hh
	$(CXX) -W -Wall -DDEBUG -g -g3 -I$(top_srcdir)/include tst.cc -o tst_d

tst: tst.cc $(top_srcdir)/include/*hh
	$(CXX) -W -Wall -I$(top_srcdir)/include tst.cc -o tst
