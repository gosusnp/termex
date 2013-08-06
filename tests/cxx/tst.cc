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
#include <cassert>
#include <iostream>

#include <tst.hh>
#include <extractor.hh>

void test_normalizer() {
    {
        Normalizer<char> n("A");
        assert(*n == 'a');
        assert(n.offset() == 0);
    }
    {
        Normalizer<char> n(" A");
        assert(*n == 'a');
        assert(n.offset() == 1);
    }
    {
        Normalizer<char> n(" A   b");
        assert(*n == 'a');
        assert(n.offset() == 1);
        ++n;
        assert(*n == ' ');
        assert(n.offset() == 2);
        ++n;
        assert(*n == 'b');
        assert(n.offset() == 5);
    }
    {
        Normalizer<char> n("  A   ");
        assert(*n == 'a');
        assert(n.offset() == 2);
        ++n;
        assert(!*n);
    }
}

void test_ternary_search_tree() {
    TernarySearchTree<char, int> tree;

    int string1 = 1;
    int test = 2;
    int string2 = 3;
    int tests = 4;
    int te_s = 5;
    int oae = 6;
    int lead = 7;

    assert(!tree.search("test"));

    assert(tree.insert("string1", &string1) == &string1);
    tree.insert("test", &test);
    tree.insert("string2", &string2);
    tree.insert("tests", &tests);
    tree.insert("te    s", &te_s);
    tree.insert("oae    ", &oae);
    tree.insert("   lead  ", &lead);
    assert(tree.insert("string1", &string2) == &string1);
    assert(tree.insert("string1", &string2, true) == &string2);
    assert(tree.search("string1") == &string2);
    assert(tree.insert("string1", &string1, true) == &string1);

    assert(!tree.search("aoeuastha"));
    assert(tree.search("test") == &test);
    assert(tree.search("tests") == &tests);
    assert(tree.search("string1") == &string1);
    assert(tree.search("string2") == &string2);
    assert(tree.search("TEST") == &test);
    assert(tree.search("tesTs") == &tests);
    assert(tree.search("stRing1") == &string1);
    assert(tree.search("strinG2") == &string2);
    assert(!tree.search("not found"));
    assert(!tree.search("test!"));
    assert(tree.search("te   s") == &te_s);
    assert(tree.search("te s") == &te_s);
    assert(tree.search("   te   s  ") == &te_s);
    assert(!tree.search("tes"));
    assert(tree.search("oae") == &oae);
    assert(tree.search("lead") == &lead);
}

void test_searcher() {
    TernarySearchTree<char, int> tree;
    int test = 1;
    int test_test = 2;
    tree.insert("test", &test);
    tree.insert("test test", &test_test);

    assert(tree.search("test") == &test);
    assert(tree.search("test test") == &test_test);
    assert(!tree.search("tes"));
    assert(!tree.search("teste"));

    TernarySearchTree<char, int>::searcher_type searcher = tree.searcher();

    assert(searcher.search('t'));
    assert(searcher.search('e'));
    assert(!searcher.search('r'));

    searcher.reset();
    assert(searcher.search('t'));
    assert(!searcher.value());
    assert(searcher.search('e'));
    assert(!searcher.value());
    assert(searcher.search('s'));
    assert(!searcher.value());
    assert(searcher.search('t'));
    assert(searcher.value() == &test);
    assert(searcher.search(' '));
    assert(searcher.search('t'));
    assert(searcher.search('e'));
    assert(searcher.search('s'));
    assert(searcher.search('t'));
    assert(searcher.value() == &test_test);
}

void test_extractor() {
    TernarySearchTree<char, int> tree;
    int test = 1;
    int test_test = 2;
    tree.insert("test", &test);
    tree.insert("test test", &test_test);

    typedef Extractor< TernarySearchTree<char, int> > extractor_type;
    extractor_type extractor(tree);
    extractor_type::result_list_type results;

    extractor.extract(std::string("Test   aoeu test e tes e test   test  o test"), results);
    assert(results.size() == 6);
    extractor_type::result_list_type::iterator i = results.begin();
    assert(i->begin == 0);
    assert(i->end == 4);
    assert(i->value == &test);
    ++i;
    assert(i->begin == 12);
    assert(i->end == 16);
    assert(i->value == &test);
    ++i;
    assert(i->begin == 25);
    assert(i->end == 29);
    assert(i->value == &test);
    ++i;
    assert(i->begin == 25);
    assert(i->end == 36);
    assert(i->value == &test_test);
    ++i;
    assert(i->begin == 32);
    assert(i->end == 36);
    assert(i->value == &test);
    ++i;
    assert(i->begin == 40);
    assert(i->end == 44);
    assert(i->value == &test);
}

int main()
{
    test_normalizer();
    test_ternary_search_tree();
    test_searcher();
    test_extractor();
}
