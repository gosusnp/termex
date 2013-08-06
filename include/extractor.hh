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
#ifndef EXTRACTOR_HH
# define EXTRACTOR_HH

# include <list>

# include "normalizer.hh"

/*
** Entity extraction module
**
** Detects all occurences of data of the given lexicon.
*/

template <typename T>
class Extractor
{
public:
    typedef typename T::value_type                                value_type;

    struct result_type
    {
        result_type(size_t b, size_t e, value_type* v) :
            begin(b),
            end(e),
            value(v)
        {}
        size_t begin;
        size_t end;
        value_type*   value;
    };

    typedef std::list<result_type>                                result_list_type;
    typedef typename T::char_type                                 char_type;
    typedef typename T::searcher_type                             searcher_type;

    struct searcher_list_element_type
    {
        searcher_list_element_type(searcher_type s, size_t b) :
            searcher(s),
            begin(b)
        {}
        searcher_type searcher;
        size_t begin;
    };
    typedef std::list<searcher_list_element_type>                 searcher_list_type;

    Extractor(const T& lexicon) :
        lexicon_(lexicon)
    { }
    ~Extractor() {}

    bool extract(const char_type* input, result_list_type& results) const
    {
        searcher_list_type    searchers;
        Normalizer<char_type> reader(input);
        int                   last_seen = 0;
        int                   current_type_of_char = 0;

        while (*reader)
        {
            char_type c = *reader;

            current_type_of_char = get_type_of_char_(c);

            // See if we can start a new match
            if (current_type_of_char != 3 &&
                    (current_type_of_char != last_seen || current_type_of_char == 2)) {
                searchers.push_back(searcher_list_element_type(lexicon_.searcher(), reader.offset()));
            }

            for (typename searcher_list_type::iterator i = searchers.begin(),
                                                       i_end = searchers.end();
                    i != i_end; ) {
                keep_match_(current_type_of_char, reader.offset(), i, results);

                if (i->searcher.search(c)) {
                    ++i;
                } else {
                    i = searchers.erase(i);
                }
            }

            last_seen = current_type_of_char;
            ++reader;
        }

        // Do not forget that the last char might trigger some final matches
        current_type_of_char = get_type_of_char_(*reader);
        for (typename searcher_list_type::iterator i = searchers.begin(),
                                                   i_end = searchers.end();
                i != i_end; ++i) {
            keep_match_(current_type_of_char, reader.offset(), i, results);
        }

        return true;
    }

    bool extract(const std::basic_string<char_type>& input, result_list_type& results) const
    {
        return extract(input.c_str(), results);
    }

protected:
    int get_type_of_char_(char_type c) const
    {
        if (isalpha(c))
            return 1;
        else if (isspace(c))
            return 3;
        else
            return 2;
    }

    void keep_match_(int current_type_of_char,
                     size_t current_offset,
                     typename searcher_list_type::iterator& i,
                     result_list_type& results) const
    {
        if (current_type_of_char > 1) { // Match can occur if current is > 1
            value_type* value = i->searcher.value();
            if (value) {
                results.push_back(result_type(
                            i->begin,
                            current_offset,
                            value));
            }
        }
    }

    const T& lexicon_;

}; // End of class Extractor


#endif // ! EXTRACTOR_HH
