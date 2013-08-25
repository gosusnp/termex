#ifndef ALLOCATOR_HH
# define ALLOCATOR_HH

template <typename T, int Block, int Size>
class PoolAllocator
{
    struct Chunk {
        Chunk() :
            occupied(0),
            freed(0),
            next(0),
            previous(0)
        {}

        size_t occupied;
        size_t freed;
        Chunk* next;
        Chunk* previous;
        char data[Size * Block];
    }; // struct Chunk

public:
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef size_t difference_type;

    typedef Chunk chunk_type;

    PoolAllocator() {}
    ~PoolAllocator() {}

    inline pointer allocate(size_type n, const_pointer hint = 0) {
        (void)hint; // silence unused var warning

        n /= sizeof (T);

        chunk_type* chunks = chunks_();
        if (chunks->occupied + n > Block)
            chunks = chunks_(new chunk_type());
        chunks->occupied += n;
        return reinterpret_cast<pointer>(chunks->data + Size * (chunks->occupied - n));
    }

    inline void deallocate(void* p) {
        for (chunk_type* c = chunks_(); c; c = c->next) {
            if (p > c && p < c->data + Size * Block) {
                ++c->freed;
                if (c->freed == c->occupied)
                {
                    if (c->previous)
                        c->previous->next = c->next;
                    if (c->next)
                        c->next->previous = c->previous;
                    delete c;
                    if (c == chunks_())
                        chunks_() = 0;
                }
                break;
            }
        }
    }

protected:
    chunk_type*& chunks_(chunk_type* c = 0) {
        static chunk_type* chunks_ = 0;
        if (!chunks_)
            chunks_ = new chunk_type();
        if (c) {
            c->next = chunks_;
            chunks_->previous = c;
            chunks_ = c;
        }
        return chunks_;
    }

}; // End of class PoolAllocator

#endif // ! ALLOCATOR_HH
