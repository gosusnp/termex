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
#include <termex.hh>

/*
** Implementation here is about refcounting and wrap result into
** python objects
*/

Termex::Termex():
    lexicon_(),
    extractor_(lexicon_)
{ }

Termex::~Termex()
{ }

PyObject* Termex::add(const PyObject* term)
{
    PyObject* value = PySet_New(0);
    PyObject* result = lexicon_.insert(PyUnicode_AS_UNICODE(term), value);
    if (value != result) {
        Py_DECREF(value);
    }
    Py_INCREF(result);
    return result;
}

PyObject* Termex::get(const PyObject* term) const
{
    PyObject* value = lexicon_.search(PyUnicode_AS_UNICODE(term));
    if (value) {
        Py_INCREF(value);
        return value;
    }
    else
        Py_RETURN_NONE;
}

PyObject* Termex::extract(const PyObject* inputstring) const
{
    extractor_type::result_list_type extractions;
    extractor_.extract(PyUnicode_AS_UNICODE(inputstring), extractions);
    PyObject* results = PyList_New(0);
    for (extractor_type::result_list_type::const_iterator i = extractions.begin(),
                                                          i_end = extractions.end();
        i != i_end; ++i) {
        PyList_Append(results, PyTuple_Pack(3, PyInt_FromSsize_t(i->begin), PyInt_FromSsize_t(i->end), i->value));
    }
    return results;
}
