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
** Implementation notes
**
** data is stored on nodes where key_ == 0, this allows to use the
** lokid_ field to store the value since it will never be used.
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
        key_(key)
    { }
    ~Node() {
        delete eqkid_;
        delete hikid_;
        if (key_)          // Only delete lokid_ when key_ is not null
            delete lokid_; // otherwise, it means that lokid_ is a value_type*
    }

    template <typename T>
    inline value_type* insert(T cp, value_type* value, bool replace) {
        node_type* node = this;
        char_type c = *cp;
        while (true) {
            c = *cp;
            if (c > node->key_) {
                if (node->hikid_)
                    node = node->hikid_;
                else
                {
                    node->hikid_ = new Node(c);
                    node = node->hikid_;
                }
            } else if (c < node->key_) {
                if (node->lokid_)
                    node = node->lokid_;
                else
                {
                    node->lokid_ = new Node(c);
                    node = node->lokid_;
                }
            } else {
                if (c) {
                    if (node->eqkid_)
                    {
                        ++cp;
                        node = node->eqkid_;
                    }
                    else
                    {
                        node->eqkid_ = new Node(*++cp);
                        node = node->eqkid_;
                    }
                } else {
                    value_type* v = node->get_value();
                    if (!v || replace)
                        return node->set_value(value);
                    else
                        return v;
                }
            }
        }
    }

    inline value_type* get_value() const
    { // See implementation notes
        return reinterpret_cast<value_type*>(lokid_);
    }
    inline value_type* set_value(value_type* value)
    { // See implementation notes
        lokid_ = reinterpret_cast<node_type*>(value);
        return reinterpret_cast<value_type*>(lokid_);
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
            return np ? np->get_value() : 0;
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
                return get_value();
            }
        }
    }

protected:
    node_type*  lokid_;
    node_type*  eqkid_;
    node_type*  hikid_;
    char_type   key_;

}; // End of class Node

#endif // ! NODE_HH
