#!/usr/bin/env python
# -*- coding: utf-8 -*-
# vim: set fileencoding=utf8 :

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

from termex import PyTermex

TOPPINGS = {
    u"Vegetables": [
        u"Alfalfa Sprouts",
        u"Artichoke hearts",
        u"Avocado",
        u"Baby leeks",
        u"Beetroot",
        u"Black Beans",
        u"Broccoli",
        u"Capers",
        u"Capicolla",
        u"Carrot",
        u"Cherry tomatoes",
        u"Dried tomatoes",
        u"Eggplant",
        u"Fungi",
        u"Fungi carciofi",
        u"Green peppers",
        u"Kalamata olives",
        u"Lettuce",
        u"Mushrooms",
        u"Onions",
        u"Olives",
        u"Peas",
        u"Porcini mushrooms",
        u"Portobello Mushrooms",
        u"Red beans",
        u"Red onions",
        u"Red peppers",
        u"Roast cauliflower",
        u"Roasted eggplant",
        u"Roasted Garlic",
        u"Roasted peppers",
        u"scallions",
        u"Shallots",
        u"Snow peas",
        u"Spinach",
        u"Sun dried tomatoes",
        u"Sweet corn",
        u"Tomatoes",
        u"Watercress",
        u"Wild mushrooms",
        u"Yellow peppers",
        u"Yellow squash",
        u"Zucchini",
    ],
    u"Nuts": [
        u"Almonds",
        u"Peanuts",
        u"Pistachios",
        u"Pecans",
        u"Pine Nuts",
        u"Walnuts",
    ],
    u"Herbs and Spîces": [
        u"Basil",
        u"Bay Leaf",
        u"Cardamon",
        u"Chili Dried or Fresh",
        u"Chives",
        u"Cilantro",
        u"Coriander",
        u"Cumin",
        u"Dill",
        u"Garlic",
        u"Jalapeno Peppers",
        u"Laurel",
        u"Marjoram",
        u"Methi Leaves",
        u"Fenugreek",
        u"Oregano",
        u"Parley",
        u"Pepper",
        u"Rosemary",
        u"Basil",
        u"Bay Leaf",
        u"Cardamon",
    ],
    u"Sea Food": [
        u"Anchovies",
        u"Cajun Prawn",
        u"Crayfish",
        u"Lobster",
        u"Oysters",
        u"Prawns",
        u"Salmon",
        u"Shrimps",
        u"Smoked Salmon",
        u"Squid",
        u"Tuna",
        u"Whitebait",
    ],
    u"Cheese": [
        u"Blue Cheese",
        u"Brie",
        u"Camembert",
        u"Cheddar",
        u"Colby",
        u"Feta",
        u"Goat Cheese",
        u"Gorgonzola",
        u"Limburger",
        u"Manchego",
        u"Monterey Jack",
        u"Parmesan",
        u"Mozzarella",
        u"Muenster",
        u"Port de Salut",
        u"Provolone",
        u"Ricota",
        u"Romano",
        u"Roquefort",
        u"Smoked Gouda",
    ],
    u"Meats": [
        u"Bacon",
        u"BBQ Chicken",
        u"Beef",
        u"Cajun Chicken",
        u"Chicken Masala",
        u"Chicken Tikka",
        u"Chorizo",
        u"Duck",
        u"Egg",
        u"Ham",
        u"Honey Cured Ham",
        u"Meatballs",
        u"Pepperoni",
        u"Proscuitto",
        u"Salami",
        u"Sausage",
        u"Serrano Ham",
        u"Turkey",
        u"Venison",
    ],
}

class ToppingExtractor(object):
    def __init__(self):
        self.tx = PyTermex()
        for tag, values in TOPPINGS.iteritems():
            for v in values:
                self.tx.add(v).add(tag)

    def extract(self, inputstr):
        extractions = self.tx.extract(inputstr)
        for begin, end, tags in extractions:
            print (u'%s: %s' % (
                u', '.join(tags),
                inputstr[begin:end])).encode('utf-8')

if __name__ == '__main__':

    test_str = u"""6 slices bacon, cooked
    Pizza dough for 12" Pizza (try Thin Crust recipe)
    2 plum tomatoes, sliced 1/4" thick
    1 1/2 cups grated Cheddar cheese
    1/2 teas. ground black pepper
    2 eggs
    """

    te = ToppingExtractor()
    te.extract(test_str)
