#!/usr/bin/env python
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf8 :

import unittest

from termex import PyTermex

class TestPyTermex(unittest.TestCase):

    def test_termex(self):
        tx = PyTermex()
        tags = tx.add(u'value1')
        tags.add('tag1')
        tx.add(u'value2')
        tx.add(u'an other string')
        self.assertEqual(tx.add(u'value1'), tags)
        self.assertEqual(tx.get(u'value1'), tags)
        self.assertEqual(tx.get(u'value1'), tags) # ensure refcounting is good
        self.assertEqual(tx.get(u'value1'), tags) # ensure refcounting is good

    def test_termex_get_non_existant(self):
        tx = PyTermex()
        self.assertIsNone(tx.get(u'haa'))
        tx.add(u'oo')
        tx.add(u'eau')
        self.assertIsNone(tx.get(u'haa'))

    def test_termex_extract(self):
        tx = PyTermex()
        tx.add(u'test').add('tag1')
        self.assertEqual(tx.extract(u'aa test aeu test!'),
                [(3, 7, set(['tag1'])), (12, 16, set(['tag1']))])

        self.assertEqual(tx.extract(u'aa Test aeu teste'),
                [(3, 7, set(['tag1']))])