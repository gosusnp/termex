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
#ifndef NORMALIZER_HH
# define NORMALIZER_HH

# include <cctype>

/*
** Character normalization
**
** can be specialized with uchar to handle diactrics removal
*/
template <typename charT>
charT normalize(charT c) {
    return tolower(c);
}

/*
** Provide an iterator with the following normalizations:
**  - skip leading and trailing whitespaces
**  - treat consecutive spaces as one
**  - use normalized form of alpha (lower case)
**
** when giving offset of whitespaces, it gives the offset of
** the first encountered whitespace.
*/
template <typename charT>
class Normalizer
{
public:
    Normalizer(const charT* cp) :
        offset_(0),
        skipped_whitespaces_(0),
        cp_(0),
        c_(0)
    {
        while (cp && isspace(*cp)) {
            ++cp;
            ++offset_;
        }
        if (cp) {
            c_ = normalize(*cp);
        }
        cp_ = cp;
    }
    ~Normalizer() {}

    inline operator bool() const { return cp_; }

    inline charT operator*() const { return c_; }

    inline Normalizer<charT>& operator++()
    {
        // step forward
        ++cp_;
        ++offset_;
        skipped_whitespaces_ = 0;

        if ((c_ = *cp_) && isspace(c_)) {
            do { // While we have a space, skip it
                ++cp_;
                ++offset_;
                ++skipped_whitespaces_;
            } while ((c_ = *cp_) && isspace(c_));

            if (c_) { // it means that !isspace(*cp_) and that we went
                     // too far ahead
                --cp_;
                --offset_;
                --skipped_whitespaces_;
                c_ = ' ';
            }
            else
                c_ = 0;
        } else {
            c_ = normalize(c_);
        }
        return *this;
    }

    inline size_t offset() const
    {
        return offset_ - skipped_whitespaces_;
    }

protected:
    size_t       offset_;
    size_t       skipped_whitespaces_;
    const charT* cp_;
    charT        c_;

}; // End of class Normalizer

#endif // ! NORMALIZER_HH
