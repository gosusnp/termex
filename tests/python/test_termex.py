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

    def test_termex_default_value_override(self):
        tx = PyTermex(default=list)
        tags = tx.add(u'value1')
        tags.append('tag1')
        tx.add(u'value2')
        tx.add(u'an other string')
        self.assertEqual(tx.add(u'value1'), tags)
        self.assertEqual(tx.get(u'value1'), tags)
        self.assertEqual(tx.get(u'value1'), tags) # ensure refcounting is good
        self.assertEqual(tx.get(u'value1'), tags) # ensure refcounting is good
        self.assertEqual(tx.get(u'value1'), ['tag1'])

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

    def test_termex_with_invalid_data(self):
        tx = PyTermex()
        with self.assertRaises(TypeError):
            tx.add('fail')
        with self.assertRaises(TypeError):
            tx.add({})
        with self.assertRaises(TypeError):
            tx.get('fail')
        with self.assertRaises(TypeError):
            tx.extract('fail')

    def test_bug(self):
        tx = PyTermex(default=list)
        tx.add(u'France').append({'name': 'France'})
        tx.add(u'United States').append({'name': 'United States'})
        self.assertEqual(tx.extract(u'I am moving from France to the United States'),
                [(17, 23, [{'name': 'France'}]), (31, 44, [{'name': 'United States'}])])
        self.assertEqual(tx.extract(u'I am moving from france to the United  States.'),
                [(17, 23, [{'name': 'France'}]), (31, 45, [{'name': 'United States'}])])

    def test_diacritics(self):
        tx = PyTermex(default=list)
        tx.add(u'té').append({'name': u'té'})
        self.assertEqual(tx.extract(u"té! C'est l'été! otéo"),
                [(0, 2, [{'name': u'té'}])])

    def test_numbers(self):
        tx = PyTermex(default=list)
        tx.add(u'13').append({'name': '13'})
        self.assertEqual(tx.extract(u"2013 a13 13a 13 01310"),
                [(13, 15, [{'name': '13'}])])
