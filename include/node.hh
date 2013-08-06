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
#ifndef NODE_HH
# define NODE_HH

/*
** Node implementation for a TernarySearchTree
**
** Lot of optimisations can be done.
** Memory usage is far from optimal.
*/
template <typename charT, typename valueT>
class Node
{
public:
    typedef charT               char_type;
    typedef valueT              value_type;
    typedef Node<charT, valueT> node_type;

    Node(char_type key) :
        lokid_(0),
        eqkid_(0),
        hikid_(0),
        value_(0),
        key_(key)
    { }
    ~Node() {
        delete lokid_;
        delete eqkid_;
        delete hikid_;
    }

    template <typename T>
    value_type* insert(T cp, value_type* value, bool replace) {
        if (*cp < key_) {
            if (!this->lokid_)
                this->lokid_ = new Node(*cp);//, cp, value, replace);
            return lokid_->insert(cp, value, replace);
        } else if (*cp > key_) {
            if (!hikid_)
                hikid_ = new Node(*cp);//, cp, value, replace);
            return hikid_->insert(cp, value, replace);
        } else {
            if (*cp) {
                if (!eqkid_)
                    eqkid_ = new Node(*cp);//, ++cp, value, replace);
                return eqkid_->insert(++cp, value, replace);
            } else {
                if (value_ && !replace)
                    return value_;
                return value_ = value;
            }
        }
    }

    /*
    ** Act as a char by char iterator
    ** Not really an iterator though as it does not iterate over
    ** the elements stored in the tree but instead allows searching
    ** through the tree.
    */
    class Searcher
    {
    public:
        Searcher(const node_type* root) :
            root_(root),
            np_(root)
        {}
        ~Searcher() {}

        bool search(char_type c) {
            np_ = search_node_(c);
            return !!np_;
        }

        void reset() {
            np_ = root_;
        }

        /*
        ** get the value of the current node.
        ** NOTE: call entails processing that is not cached.
        **       use sparingly
        */
        value_type* value() const
        {
            const node_type* np = search_node_(0);
            return np ? np->value_ : 0;
        }

    protected:
        // Find the next node
        const node_type* search_node_(char_type c) const
        {
            const node_type* np = np_;
            while (np) {
                if (c < np->key_) {
                    np = np->lokid_;
                } else if (c > np->key_) {
                    np = np->hikid_;
                } else {
                    if (c)
                        np = np->eqkid_;
                    return np;
                }
            }
            return np;
        }

        const node_type* root_;
        const node_type* np_;

    }; // End of class Searcher

    Searcher searcher() const
    {
        return Searcher(this);
    }


    template <typename T>
    value_type* search(T cp) const
    {
        if (*cp < key_) {
            return lokid_ ? lokid_->search(cp) : 0;
        } else if (*cp > key_) {
            return hikid_ ? hikid_->search(cp) : 0;
        } else {
            if (*cp) {
                return eqkid_ ? eqkid_->search(++cp) : 0;
            } else {
                return value_;
            }
        }
    }

protected:
    node_type*  lokid_;
    node_type*  eqkid_;
    node_type*  hikid_;
    value_type* value_;
    char_type   key_;

}; // End of class Node

#endif // ! NODE_HH
