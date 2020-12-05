#pragma once
#include <iostream>
#include <memory>

template <class T>
class Allocator
{
public:
    using value_type = T;
    using pointer = T*;
    using size_type = size_t;

    pointer allocate(size_type count) {
        pointer new_data = static_cast<pointer>(malloc(sizeof(value_type) * count));
        if (new_data == nullptr) {
            throw std::bad_alloc();
        }
        return new_data;
    }

    void deallocate(pointer ptr, size_type count) noexcept {
        free(ptr);
    }

    template<typename ... Args>
    void construct(pointer xptr, Args&&... args) {
        new(xptr) T(std::forward<Args>(args)...);
    }

    void destroy(pointer xptr) noexcept {
        xptr->~T();
    }
};

template <class T>
class Iterator
{
public:
    using It = Iterator<T>;

    Iterator(T* data) : data_{data} { }

    bool operator==(const It& other) {
        return data_ == other.data_;
    }

    bool operator!=(const It& other) {
        return data_ != other.data_;
    }

    bool operator<(const It& other) {
        return data_ < other.data_;
    }

    bool operator<=(const It& other) {
        return data_ <= other.data_;
    }

    bool operator>(const It& other) {
        return data_ > other.data_;
    }

    bool operator>=(const It& other) {
        return data_ >= other.data_;
    }

    It& operator++() {
        ++data_;
        return *this;
    }

    It operator++(int) {
        It old{*this};
        ++data_;
        return old;
    }

    It& operator--() {
        --data_;
        return *this;
    }

    It operator--(int) {
        It old{*this};
        --data_;
        return old;
    }

    It operator+(int shift) const {
        return {data_ + shift};
    }

    It operator-(int shift) const {
        return {data_ - shift};
    }

    It& operator+=(int shift) {
        data_ += shift;
        return *this;
    }

    It& operator-=(int shift) {
        data_ -= shift;
        return *this;
    }

    T& operator*() {
        return *data_;
    }

    const T& operator*() const  {
        return *data_;
    }

private:
    T* data_;
};

template <class T, class Alloc = Allocator<T>>
class Vector
{
public:
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using size_type = std::size_t;

    Vector()
        : alloc_{}
        , data_{alloc_.allocate(4)}
        , capacity_{4}
        , size_{0}
    {
    }

    Vector(const Vector<T>& other)
        : alloc_{}
        , data_{alloc_.allocate(other.size_)}
        , size_{0}
        , capacity_{other.size_}
    {
        for (; size_ < other.size_; ++size_) {
            alloc_.construct(data_ + size_, other.data_[size_]);
        }
    }

    Vector(Vector<T> && other)
        : alloc_{}
        , data_{other.data_}
        , size_{other.size_}
        , capacity_{other.capacity_}
    {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    Vector(std::initializer_list<T> l)
        : alloc_{}
        , data_{alloc_.allocate(l.size())}
        , size_{0}
        , capacity_{l.size()}
    {
        for (const T& value : l) {
            alloc_.construct(data_ + size_++, std::move(value));
        }
    }

    ~Vector() {
        for (; size_ > 0;) {
            alloc_.destroy(data_ + --size_);
        }
        alloc_.deallocate(data_, capacity_);
    }

    Vector& operator=(const Vector& other){
        T* new_data = alloc_.allocate(other.size_);
        for (size_t i = 0; i < other.size_; ++i) {
            alloc_.construct(new_data + i, other[i]);
        }
        for (size_t i = 0; i < size_; ++i) {
            alloc_.destroy(data_ + i);
        }
        alloc_.deallocate(data_, capacity_);
        data_ = new_data;
        size_ = other.size_;
        capacity_ = other.capacity_;
        return *this;
    }

    Vector& operator=(Vector&& other) {
        data_ = other.data_;
        other.data_ = nullptr;
        size_ = other.size_;
        capacity_ = other.capacity_;
        return *this;
    }

    T& operator[](size_type i) {
        return data_[i];
    }

    const T& operator[](size_type i) const {
        return data_[i];
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            reserve(size_ * 2);
        }
        alloc_.construct(data_ + size_++, value);
    }

    void push_back(T&& value) {
        if (size_ >= capacity_) {
            reserve(size_ * 2);
        }
        alloc_.construct(data_ + size_++, std::move(value));
    }

    void pop_back() {
        alloc_.destroy(data_ + --size_);
    }

    template<class ... Args>
    void emplace_back(Args&& ... args) {
        if (size_ >= capacity_) {
            reserve(size_ * 2);
        }
        alloc_.construct(data_ + size_++, std::forward<Args>(args)...);
    }

    bool empty() const noexcept {
        return size_ == 0;
    }

    size_type size() const noexcept {
        return size_;
    }

    size_type capacity() const noexcept {
        return capacity_;
    }

    void clear() noexcept {
        for (; size_ > 0;) {
            alloc_.destroy(data_ + --size_);
        }
    }

    iterator begin() {
        return {data_};
    }

    iterator end() {
        return {data_ + size_};
    }

    reverse_iterator rbegin() {
        return {end()};
    }

    reverse_iterator rend() {
        return {begin()};
    }

    void resize(size_type new_size) {
        if (new_size != size_) {
            resize(new_size, T());
        }
    }

    // don't look on strong exception guarantee, too difficult
    void resize(size_type new_size, const T & value) {
        if (new_size > size_) {
            reserve(new_size);
            for (; size_ < new_size; ++size_) {
                alloc_.construct(data_ + size_, value);
            }
        } else if (new_size < size_) {
            for (; size_ > new_size;) {
                alloc_.destroy(data_ + --size_);
            }
        }
    }

    // try to strong exception guarantee
    void reserve(size_type new_cap) {
        if (new_cap <= capacity_) {
            return;
        }
        T* new_data = alloc_.allocate(new_cap);

        // if is not nothrow, standard speaks of undefined behavior
        if constexpr (std::is_nothrow_move_constructible<T>::value
                || !std::is_copy_constructible<T>::value) {
            for (size_type i = 0; i < size_; ++i) {
                alloc_.construct(new_data + i, std::move(data_[i]));
            }
        } else {
            size_type i;
            try { // try to copy, if error, free data
                for (i = 0; i < size_; ++i) {
                    alloc_.construct(new_data + i, data_[i]);
                }
            } catch (...) {
                for (; i > 0;) {
                    alloc_.destroy(new_data + --i);
                }
                alloc_.deallocate(new_data, capacity_);
                throw;
            }

            for (; i > 0;) {
                alloc_.destroy(data_ + --i);
            }
        }

        alloc_.deallocate(data_, capacity_);
        data_ = new_data;
        capacity_ = new_cap;
    }

private:
    Alloc alloc_;
    T* data_;
    size_type capacity_;
    size_type size_;
};
