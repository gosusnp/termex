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

// Specialization of Node destructor to decrement the refcount on
// the stored values.
template <>
Node<Py_UNICODE, PyObject>::~Node()
{
    if (key_)          // Only delete lokid_ when key_ is not null
        delete lokid_; // otherwise, it means that lokid_ is a value_type*
    else
    {
        if (lokid_)
            Py_DECREF(reinterpret_cast<PyObject*>(lokid_));
    }
    delete eqkid_;
    delete hikid_;
}

Termex::Termex():
    lexicon_(),
    extractor_(lexicon_)
{ }

Termex::~Termex()
{ }

PyObject* Termex::add(const PyObject* term, PyObject* value)
{
    PyObject* result = lexicon_.insert(PyUnicode_AS_UNICODE(term), value);
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

struct result_builder_type
{
    result_builder_type() :
        results(PyList_New(0))
    {}
    void operator()(size_t begin, size_t end, PyObject* value) const
    {
        PyList_Append(results,
                PyTuple_Pack(3, PyInt_FromSsize_t(begin), PyInt_FromSsize_t(end), value));
    }
    PyObject* results;
};

PyObject* Termex::extract(const PyObject* inputstring) const
{
    result_builder_type result_builder;
    extractor_.extract(PyUnicode_AS_UNICODE(inputstring), result_builder);
    return result_builder.results;
}
