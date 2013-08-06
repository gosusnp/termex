CXX=clang++
top_srcdir=../..

check: tst
	./tst

tst: tst.cc $(top_srcdir)/include/*hh
	$(CXX) -W -Wall -I$(top_srcdir)/include tst.cc -o tst
