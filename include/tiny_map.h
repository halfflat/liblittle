#ifndef TINY_MAP_H
#define TINY_MAP_H

#include <algorithm>
#include <iterator>
#include <type_traits>
#include <vector>

/** Classes for handling small size maps with a linear search implementation.
 *
 * Implements the C++ UnorderedAssociativeContainer concept with the following exceptions:
 * 1. No equal_range() method
 * 2. No hash_function() method
 * 3. No emplace_hint() method
 * 4. No get_allocator() method for tiny_map.
 */

/** Vector-backed small map */

template <typename Key,typename Value,class KeyEqual=std::equal_to<Key>,class Allocator=std::allocator<std::pair<Key,Value>>>
struct small_map {
    typedef Key key_type;
    typedef Value mapped_type;
    typedef std::pair<key_type,mapped_type> value_type;
    typedef KeyEqual key_equal;

    typedef const value_type &const_reference;
    typedef const_reference reference;

    typedef Allocator allocator_type;
private:
    typedef std::vector<value_type,allocator_type> store_type;

public:
    typedef typename store_type::size_type size_type;
    typedef typename store_type::difference_type difference_type;

    typedef typename store_type::const_iterator const_iterator;
    typedef const_iterator iterator;

    small_map(const small_map &) =default;
    small_map(small_map &&) =default;

    explicit small_map(const KeyEqual &eq_=KeyEqual(),
        const Allocator &alloc_=Allocator()): eq(eq_),v(alloc_) {}

    explicit small_map(const Allocator &alloc_): v(alloc_) {}

    small_map(const small_map &other,const Allocator &alloc_)
        : v(other.v,alloc_) {}
    small_map(small_map &&other,const Allocator &alloc_)
        : v(std::move(other.v),alloc_) {}

    template <typename I>
    small_map(I b,I e,const KeyEqual &eq_=KeyEqual(),
        const Allocator &alloc_=Allocator()): eq(eq_),v(alloc_) { insert(b,e); }

    small_map(std::initializer_list<value_type> ilist,
        const KeyEqual &eq_=KeyEqual(),const Allocator &alloc_=Allocator())
        : eq(eq_),v(alloc_) { insert(ilist); }

    small_map &operator=(const small_map &) =default;
    small_map &operator=(small_map &&) =default;

    const_iterator begin() const { return cbegin(); }
    const_iterator cbegin() const { return v.cbegin(); }

    const_iterator end() const { return cend(); }
    const_iterator cend() const { return v.cend(); }
    
    bool empty() const { return v.empty(); }
    size_type size() const { return v.size(); }
    size_type max_size() const { return v.max_size(); }

    void clear() { v.clear(); }

    iterator insert(const value_type &value) {
        auto where=find_in_store(value.first);
        if (where==v.end()) {
            v.push_back(value);
            return std::prev(v.end());
        }
        else {
            *where=value;
            return where;
        }
    }
    
    iterator insert(value_type &&value) {
        auto where=find_in_store(value.first);
        if (where==v.end()) {
            v.push_back(std::move(value));
            return std::prev(v.end());
        }
        else {
            *where=std::move(value);
            return where;
        }
    }

    template <typename I>
    void insert(I b,I e) {
        while (b!=e) insert(*b++);
    }
    
    void insert(std::initializer_list<value_type> ilist) {
        for (const auto &x: ilist) insert(x);
    }

    template <typename... Args>
    std::pair<iterator,bool> emplace(Args &&... args) {
        value_type kv(std::forward<Args>(args)...);
        auto where=find(kv.first);
        if (where!=end()) return std::make_pair(where,false);

        v.push_back(std::move(kv));
        return std::make_pair(std::prev(v.end()),true);
    }

    iterator erase(const_iterator pos) {
        return v.erase(pos);
    }

    size_type erase(const key_type &key) {
        auto where=find(key);
        if (where==end()) return 0;

        erase(where);
        return 1;
    }

    void swap(small_map &other) {
        std::swap(v,other.v);
    }

    size_type count(const key_type &key) const {
        return find(key)!=end();
    }

    iterator find(const key_type &key) const {
        return find_in_store(key);
    }

    mapped_type &operator[](const Key &key) {
        auto where=find_in_store(key);
        if (where!=v.end()) return where->second;
        v.emplace_back(std::piecewise_construct,std::forward_as_tuple(key),std::tuple<>());
        return std::prev(v.end())->second;
    }

    mapped_type &operator[](Key &&key) {
        auto where=find_in_store(key);
        if (where!=v.end()) return where->second;
        v.emplace_back(std::piecewise_construct,std::forward_as_tuple(std::move(key)),std::tuple<>());
        return std::prev(v.end())->second;
    }

    mapped_type &at(const Key &key) {
        auto where=find_in_store(key);
        if (where!=v.end()) return where->second;
        throw std::out_of_range("missing key");
    }

    const mapped_type &at(const Key &key) const {
        auto where=find_in_store(key);
        if (where!=v.end()) return where->second;
        throw std::out_of_range("missing key");
    }

    KeyEqual key_eq() const { return eq; }
    Allocator get_allocator() const { return v.get_allocator(); }

    friend bool operator==(const small_map &a,const small_map &b) {
        if (a.size()!=b.size()) return false;
        auto bend=b.end();
        for (const auto &e: a) {
            auto bi=b.find(e.first);
            if (bi==bend || e.second!=bi->second) return false;
        }

        return true;
    }

    friend bool operator!=(const small_map &a,const small_map &b) {
        return !(a==b);
    }

private:
    store_type v;
    KeyEqual eq;

    typename store_type::const_iterator find_in_store(const key_type &key) const {
        auto b=v.begin();
        auto e=v.end();
        while (b!=e) if (eq(b->first,key)) break; else ++b;
        return b;
    }

    typename store_type::iterator find_in_store(const key_type &key) {
        auto b=v.begin();
        auto e=v.end();
        while (b!=e) if (eq(b->first,key)) break; else ++b;
        return b;
    }
};

/** Array-backed small map with fixed max capacity.
 *
 * Note that capcity is not checked when inserting elements.
 */

namespace impl {
    // common functionality across tiny_map classes with trivial
    // and non-trivial value types.

    template <typename Key,typename Value,std::size_t N,class KeyEqual>
    struct tiny_map_common {
        typedef Key key_type;
        typedef std::pair<Key,Value> value_type;
        typedef Value mapped_type;
        typedef KeyEqual key_equal;

        typedef const value_type &const_reference;
        typedef const_reference reference;

        typedef std::size_t size_type;
        typedef std::ptrdiff_t difference_type;

        typedef const value_type *const_iterator;
        typedef const_iterator iterator;

        tiny_map_common() =default;
        tiny_map_common(const KeyEqual &eq_): eq(eq_) {}

        key_equal key_eq() const { return eq; }

        const_iterator begin() const { return cbegin(); }
        const_iterator cbegin() const { return get(); }

        const_iterator end() const { return cend(); }
        const_iterator cend() const { return get()+n; }

        bool empty() const { return n==0; }
        size_type size() const { return n; }
        size_type max_size() const { return N; }

        size_type count(const key_type &key) const {
            return find(key)!=end();
        }

        iterator find(const key_type &key) const {
            return find_(key);
        }

        template <typename... Args>
        std::pair<iterator,bool> emplace(Args &&... args) {
            value_type kv(std::forward<Args>(args)...);
            auto where=find(kv.first);
            if (where!=end()) return std::make_pair(where,false);

            ::new(get(n)) value_type(std::move(kv));
            return std::make_pair(begin()+n++,true);
        }

        iterator insert(const value_type &value) {
            auto where=find_(value.first);
            if (where==end()) {
                ::new(get(n)) value_type(value);
                return begin()+n++;
            }
            else {
                *where=value;
                return where;
            }
        }

        iterator insert(value_type &&value) {
            auto where=find_(value.first);
            if (where==end()) {
                ::new(get(n)) value_type(std::move(value));
                return begin()+n++;
            }
            else {
                *where=value;
                return where;
            }
        }
    
        template <typename I>
        void insert(I b,I e) {
            while (b!=e) insert(*b++);
        }
        
        void insert(std::initializer_list<value_type> ilist) {
            insert(ilist.begin(),ilist.end());
        }

        mapped_type &operator[](const Key &key) {
            auto where=find_(key);
            if (where!=end()) return where->second;
            ::new(get(n)) value_type(std::piecewise_construct,std::forward_as_tuple(key),std::tuple<>());
            return get(n++)->second;
        }

        mapped_type &operator[](Key &&key) {
            auto where=find_(key);
            if (where!=end()) return where->second;
            ::new(get(n)) value_type(std::piecewise_construct,std::forward_as_tuple(std::move(key)),std::tuple<>());
            return get(n++)->second;
        }

        mapped_type &at(const Key &key) {
            auto where=find_(key);
            if (where!=end()) return where->second;
            throw std::out_of_range("missing key");
        }

        const mapped_type &at(const Key &key) const {
            auto where=find_(key);
            if (where!=end()) return where->second;
            throw std::out_of_range("missing key");
        }

        bool operator==(const tiny_map_common &b) const {
            if (size()!=b.size()) return false;
    
            auto bend=b.end();
            for (const auto &e: *this) {
                auto bi=b.find(e.first);
                if (bi==bend || e.second!=bi->second) return false;
            }

            return true;
        }

        bool operator!=(const tiny_map_common &b) const {
            return !(*this==b);
        }

    protected:
        typename std::aligned_storage<sizeof(value_type),alignof(value_type)>::type data[N];
        size_t n=0;
        KeyEqual eq;

        value_type *get(std::ptrdiff_t i=0) { return reinterpret_cast<value_type *>(data+i); }
        const value_type *get(std::ptrdiff_t i=0) const { return reinterpret_cast<const value_type *>(data+i); }

        const value_type *find_(const Key &key) const {
            for (std::ptrdiff_t i=0;i<n;++i) if (eq(get(i)->first,key)) return get(i);
            return get(n);
        }

        value_type *find_(const Key &key) {
            for (std::ptrdiff_t i=0;i<n;++i) if (eq(get(i)->first,key)) return get(i);
            return get(n);
        }
    };

} // namesapce impl

// tiny_map with trivial value type
template <typename Key,typename Value,std::size_t N,class KeyEqual=std::equal_to<Key>,bool trivial=std::is_trivially_copyable<Key>::value>
struct tiny_map: public impl::tiny_map_common<Key,Value,N,KeyEqual> {
private:
    using common=impl::tiny_map_common<Key,Value,N,KeyEqual>;
    using common::get;
    using common::find_;
    using common::data;
    using common::n;
    using common::eq;

public:
    using key_type=typename common::key_type;
    using value_type=typename common::value_type;
    using key_equal=typename common::key_equal;
    using reference=typename common::reference;
    using const_reference=typename common::const_reference;
    using size_type=typename common::size_type;
    using difference_type=typename common::difference_type;
    using iterator=typename common::iterator;
    using const_iterator=typename common::const_iterator;

    using common::begin;
    using common::end;
    using common::insert;
    using common::emplace;
    
    tiny_map() {}
    explicit tiny_map(const KeyEqual &eq_): common(eq_) {}

    template <typename I>
    tiny_map(I b,I e,const KeyEqual &eq_=KeyEqual()): common(eq_) {
        insert(b,e);
    }

    tiny_map(std::initializer_list<value_type> ilist,
        const KeyEqual &eq_=KeyEqual()) : common(eq_)
    {
        insert(ilist);
    }

    tiny_map(const tiny_map &) =default;
    tiny_map(tiny_map &&) =default;
    tiny_map &operator=(const tiny_map &) =default;
    tiny_map &operator=(tiny_map &&) =default;
    
    void clear() { n=0; }

    iterator erase(const_iterator pos) {
        value_type *x=get(pos-begin());
        value_type *last=get(n-1);

        // trivial copy-ctor and dtor on Key
        *x=*last;
        --n;
        return pos;
    }

    size_type erase(const key_type &key) {
        auto where=find_(key);
        if (where==end()) return 0;

        erase(where);
        return 1;
    }
    
    void swap(tiny_map &other) {
        std::swap(data,other.data);
        std::swap(n,other.n);
    }
};

// tiny_map with non-trivial value type
template <typename Key,typename Value,std::size_t N,class KeyEqual>
struct tiny_map<Key,Value,N,KeyEqual,false>: public impl::tiny_map_common<Key,Value,N,KeyEqual>  {
private:
    using common=impl::tiny_map_common<Key,Value,N,KeyEqual>;
    using common::get;
    using common::find_;
    using common::data;
    using common::n;
    using common::eq;

public:
    using key_type=typename common::key_type;
    using value_type=typename common::value_type;
    using key_equal=typename common::key_equal;
    using reference=typename common::reference;
    using const_reference=typename common::const_reference;
    using size_type=typename common::size_type;
    using difference_type=typename common::difference_type;
    using iterator=typename common::iterator;
    using const_iterator=typename common::const_iterator;

    using common::begin;
    using common::end;
    using common::insert;
    using common::emplace;

    tiny_map() {}
    explicit tiny_map(const KeyEqual &eq_): common(eq_) {}

    template <typename I>
    tiny_map(I b,I e,const KeyEqual &eq_=KeyEqual()): common(eq_) {
        insert(b,e);
    }

    tiny_map(std::initializer_list<value_type> ilist,
        const KeyEqual &eq_=KeyEqual()) : common(eq_)
    {
        insert(ilist);
    }

    tiny_map(const tiny_map &other): common(other.eq) {
        for (const auto &x: other) ::new(get(n++)) value_type(x);
    }

    tiny_map(tiny_map &&other) {
        for (auto &x: other) ::new(get(n++)) value_type(std::move(x));
    }

    tiny_map &operator=(const tiny_map &other) {
        if (this!=&other) {
            clear();
            for (const auto &x: other) ::new(get(n++)) value_type(x);
        }
        return *this;
    }

    tiny_map &operator=(tiny_map &&other) {
        if (this!=&other) {
            clear();
            for (auto &x: other) ::new(get(n++)) value_type(std::move(x));
        }
        return *this;
    }

    ~tiny_map() { clear(); }

    void clear() {
        for (size_t i=0;i<n;++i) get(i)->~value_type();
        n=0;
    }

    iterator erase(const_iterator pos) {
        value_type *x=get(pos-begin());
        value_type *last=get(n-1);

        std::swap(*x,*last);
        last->~value_type();
        --n;
        return pos;
    }

    size_type erase(const key_type &key) {
        auto where=find_(key);
        if (where==end()) return 0;

        erase(where);
        return 1;
    }
    
    void swap(tiny_map &other) {
        std::ptrdiff_t i=0;
        std::ptrdiff_t nmin=std::min(n,other.n);
        for (std::ptrdiff_t i=0;i<nmin;++i) std::swap(*get(i),*(other.get(i)));
        for (std::ptrdiff_t i=nmin;i<n;++i) {
            ::new(other.get(i)) value_type(std::move(*get(i)));
            get(i)->~value_type();
        }
        for (std::ptrdiff_t i=nmin;i<other.n;++i) {
            ::new(get(i)) value_type(std::move(*other.get(i)));
            other.get(i)->~value_type();
        }
        std::swap(n,other.n);
    }
};

#endif // ndef TINY_MAP_H
