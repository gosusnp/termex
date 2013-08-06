# distutils: language = c++
# distutils: include_dirs = include
# distutils: sources = src/termex.cc

# The MIT License (MIT)
#
#    Copyright (c) 2013 gosusnp
#
#    Permission is hereby granted, free of charge, to any person obtaining a copy of
#    this software and associated documentation files (the "Software"), to deal in
#    the Software without restriction, including without limitation the rights to
#    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
#    the Software, and to permit persons to whom the Software is furnished to do so,
#    subject to the following conditions:
#
#    The above copyright notice and this permission notice shall be included in all
#    copies or substantial portions of the Software.
#
#    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
#    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
#    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
#    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

cdef extern from "include/termex.hh":
    cdef cppclass Termex:
        Termex()
        list extract(string)
        object add(string)
        object get(string)


cdef class PyTermex:
    cdef Termex *thisptr      # hold a C++ instance which we're wrapping
    def __cinit__(self):
        self.thisptr = new Termex()
    def __dealloc__(self):
        del self.thisptr
    def add(self, term):
        return self.thisptr.add(term)
    def get(self, term):
        return self.thisptr.get(term)
    def extract(self, inputstring):
        return self.thisptr.extract(inputstring)
