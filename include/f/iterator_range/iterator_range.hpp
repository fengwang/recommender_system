#ifndef YFGIBOPKHTUSPSBADAXCMJSJOVNDSUDCWGSKTHEDCNSTFGOLYXHBTMXVIAPPUHGDRGAVMTLQA
#define YFGIBOPKHTUSPSBADAXCMJSJOVNDSUDCWGSKTHEDCNSTFGOLYXHBTMXVIAPPUHGDRGAVMTLQA

#include <cstddef>

namespace f
{
    template<class Forward_Traversal_Iterator>
    struct iterator_range
    {
        typedef Forward_Traversal_Iterator              iterator;
        typedef Forward_Traversal_Iterator              const_iterator;
        typedef std::size_t                             size_type;
        typedef std::ptrdiff_t                          difference_type;

        template<class Forward_Traversal_Iterator2>
        iterator_range( Forward_Traversal_Iterator2 begin_, Forward_Traversal_Iterator2 end_ ) : begin_itor( begin_ ), end_itor( end_ ) {}

        iterator begin() const { return begin_itor; }
        iterator end() const { return end_itor; }

        auto& front() const { return *begin_itor; }
        auto& back() const { return *( end_itor - 1 ); }

        bool      empty() const { return begin_itor == end_itor; }

        auto& operator[]( difference_type at_ ) const { return *( begin_itor + at_ ); }
        auto operator()( difference_type at_ ) const { return *( begin_itor + at_ ); }
        size_type size() const { return end_itor - begin_itor; }

        iterator begin_itor;
        iterator end_itor;
    };

    // external construction
    template<class Forward_Traversal_Iterator>
    auto make_iterator_range( Forward_Traversal_Iterator begin_, Forward_Traversal_Iterator end_ )
    {
        return iterator_range<Forward_Traversal_Iterator> { begin_, end_ };
    }

}//namespace f

#endif//YFGIBOPKHTUSPSBADAXCMJSJOVNDSUDCWGSKTHEDCNSTFGOLYXHBTMXVIAPPUHGDRGAVMTLQA

