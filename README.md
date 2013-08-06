Termex
======

Lexicon based term extraction with case and space insensitive evaluations.
The internal structure and text evaluation is written in C++.

Installation
------------

Preferably inside a virtualenv.

    pip install cython
    python setup.py install


Getting started
---------------

```python
>>> from termex import PyTermex

>>> tx = PyTermex()
>>> tx.add(u"term1")
set([])
>>> tx.add(u"another term")
set([])
>>> tx.extract(u"I'm term1 and this is another term")
[(4, 9, set([])), (22, 34, set([]))]
```
