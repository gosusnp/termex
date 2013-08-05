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
#ifndef TST_HH
# define TST_HH

# include "node.hh"
# include "normalizer.hh"

/*
** TernarySearchTree
**
** User end class
*/
template <typename charT, typename valueT>
class TernarySearchTree
{
public:
    typedef charT                        char_type;
    typedef valueT                       value_type;
    typedef Node<charT, valueT>          node_type;
    typedef typename node_type::Searcher searcher_type;

    TernarySearchTree() :
        root_(new node_type(0))
    { }
    ~TernarySearchTree()
    {
        delete root_;
    }

    void insert(const char_type* str, value_type* value)
    {
        root_->insert(Normalizer<char_type>(str), value);
    }

    value_type* search(const char_type* str) const
    {
        return root_->search(Normalizer<char_type>(str));
    }

    typename node_type::Searcher searcher() const
    {
        return root_->searcher();
    }

protected:
    node_type* root_;

}; // End of class TernarySearchTree

#endif // ! TST_HH
