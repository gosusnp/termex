#!/usr/bin/env python
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf8 :

from distutils.core import setup
from Cython.Build import cythonize

setup(
    name='termex',
    version='0.0.1',
    description='termex',
    ext_modules=cythonize(
        "termex.pyx",
        language='c++',
    )
)
