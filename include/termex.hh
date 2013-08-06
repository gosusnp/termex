/* The MIT License (MIT)
**
**    Copyright (c) 2013 gosusnp
**
**    Permission is hereby granted, free of charge, to any person obtaining a copy of
**    this software and associated documentation files (the "Software"), to deal in
**    the Software without restriction, including without limitation the rights to
**    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
**    the Software, and to permit persons to whom the Software is furnished to do so,
**    subject to the following conditions:
**
**    The above copyright notice and this permission notice shall be included in all
**    copies or substantial portions of the Software.
**
**    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
**    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
**    FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
**    COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
**    IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
**    CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef TERMEX_HH
# define TERMEX_HH

# include <Python.h>

# include "tst.hh"
# include "extractor.hh"

/*
** Exposed Class
**
** Handles lexicons and extractor.
*/
class Termex
{
public:
    typedef TernarySearchTree<Py_UNICODE, PyObject> lexicon_type;
    typedef Extractor<lexicon_type>                 extractor_type;
    Termex();
    ~Termex();

    // Add <term> to the lexicon, returns a set of tags
    PyObject* add(const PyObject* term);
    // returns a set of tags of <term>
    PyObject* get(const PyObject* term) const;
    // Perform extraction returns a list of [(begin, end, set([tags])), ...]
    PyObject* extract(const PyObject* inputstring) const;

protected:
    lexicon_type   lexicon_;
    extractor_type extractor_;

}; // End of class Termex

#endif // ! TERMEX_HH
