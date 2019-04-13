#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <boost/concept_check.hpp>

namespace aisdi {

    template<typename Type>
    class Vector {
    public:
        using difference_type = std::ptrdiff_t;
        using size_type = std::size_t;
        using value_type = Type;
        using pointer = Type *;
        using reference = Type &;
        using const_pointer = const Type *;
        using const_reference = const Type &;

        class ConstIterator;

        class Iterator;

        using iterator = Iterator;
        using const_iterator = ConstIterator;

    private:
        pointer arr;
        size_t vector_size = 0;
        size_t reserved_size = 10;

        void get_more_space() {
            reserved_size *= 2;
            auto temp = new Type[reserved_size];
            for (size_t i = 0; i < vector_size; i++) {
                temp[i] = arr[i];
            }
            delete[] arr;
            arr = temp;
        }

    public:

        Vector() {
            arr = new Type[reserved_size];
        }

        Vector(std::initializer_list<Type>
               l) {
            reserved_size = l.size();
            arr = new Type[reserved_size];
            for (auto &item: l)
                arr[vector_size++] = item;
        }

        Vector(const Vector &other) {
            reserved_size = other.reserved_size;
            arr = new Type[reserved_size];
            vector_size = other.vector_size;
            size_t x = 0;
            while (x < other.vector_size) {
                arr[x] = other.arr[x];
                x++;
            }
        }


        Vector(Vector &&other) {
            vector_size = other.vector_size;
            reserved_size = other.reserved_size;
            arr = other.arr;
            other.reserved_size = 0;
            other.vector_size = 0;
            other.arr = nullptr;
        }

        ~Vector() {
            delete[] arr;
        }

        Vector &operator=(const Vector &other) {
            if (this == &other)
                return *this;
            delete[] arr;
            vector_size = other.vector_size;
            reserved_size = other.reserved_size;
            arr = new Type[reserved_size];
            for (size_t i = 0; i < reserved_size; ++i) {
                arr[i] = other.arr[i];
            }
            return *this;
        }

        Vector &operator=(Vector &&other) {
            vector_size = other.vector_size;
            reserved_size = other.reserved_size;
            delete[] arr;
            arr = other.arr;
            other.reserved_size = 0;
            other.vector_size = 0;
            other.arr = nullptr;
            return *this;
        }

        bool isEmpty() const {
            return vector_size == 0;
        }

        size_type getSize() const {
            return vector_size;
        }

        void append(const Type &item) {
            if (vector_size == reserved_size) get_more_space();
            arr[vector_size++] = item;
        }

        void prepend(const Type &item) {
            if (vector_size == reserved_size) get_more_space();

            for (size_type i = vector_size; i > 0; i--)
                arr[i] = arr[i - 1];

            arr[0] = item;
            vector_size++;
        }

        void insert(const const_iterator &insertPosition, const Type &item) {

            if (insertPosition == begin()) {
                prepend(item);
                return;
            }
            if (insertPosition == end()) {
                append(item);
                return;
            }
            iterator it2 = begin();

            size_t i = 0;
            auto temp = new Type[reserved_size];
            while (it2 != insertPosition) {
                temp[i] = arr[i];
                i++;
                it2++;
            }
            temp[i] = item;

            i++;
            while (it2 != end()) {
                temp[i] = arr[i-1];
                i++;
                it2++;
            }
            vector_size++;
            delete [] arr;
            arr = temp;
            if (reserved_size == vector_size) get_more_space();

        }

        Type popFirst() {
            if (isEmpty()) throw std::out_of_range("error");
            vector_size--;
            Type item = arr[0];
            for (size_t i = 0; i < vector_size; i++) {
                arr[i] = arr[i + 1];
            }
            arr[vector_size].~Type();
            return item;
        }

        Type popLast() {
            if (isEmpty()) throw std::out_of_range("error");
            vector_size--;
            Type item = arr[vector_size];
            arr[vector_size].~Type();
            return item;

        }

        void erase(const const_iterator &possition) {
            if (isEmpty()) throw std::out_of_range("error");
            if (possition == this->end()) throw std::out_of_range("error");
            iterator it = possition;
            while (it != this->end() - 1) {
                *it = *(it + 1);
                it++;
            }
            vector_size--;
        }

        void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded) {
            if (this->isEmpty()) throw std::out_of_range("error");
            if (firstIncluded == begin() && lastExcluded == end()) {
                vector_size = 0;
                return;
            }

            auto it = begin();
            size_t i = 0;
            size_t j = 0;
            while (it != firstIncluded) {
                i++;
                it++;

            }
            j = i;
            while (it != lastExcluded) {
                it++;
                j++;
            }

            auto size = reserved_size;
            vector_size -= (j - i);
            do {
                arr[i] = arr[j];
                arr[j].~Type();
                i++;
                j++;
            } while (j < size);

        }


        iterator begin() {
            iterator it(arr, this);
            return it;
        }

        iterator end() {
            iterator it(arr + vector_size, this);
            return it;
        }

        const_iterator cbegin() const {
            const_iterator it(arr, this);
            return it;
        }

        const_iterator cend() const {
            const_iterator it(arr + vector_size, this);
            return it;
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }

    };

    template<typename Type>
    class Vector<Type>::ConstIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename Vector::value_type;
        using difference_type = typename Vector::difference_type;
        using pointer = typename Vector::const_pointer;
        using reference = typename Vector::const_reference;
    private:
        pointer p;
        const Vector<Type> *vector_ptr;
    public:
        explicit ConstIterator() {}

        explicit ConstIterator(pointer x, const Vector<Type> *z) {
            p = x;
            vector_ptr = z;
        }

        reference operator*() const {
            if (*this == vector_ptr->end()) throw std::out_of_range("error");
            return *p;
        }

        ConstIterator &operator++() {
            if (*this == vector_ptr->end()) throw std::out_of_range("error");
            p++;
            return *this;
        }

        ConstIterator operator++(int) {
            if (*this == vector_ptr->end()) throw std::out_of_range("error");
            auto result = *this;
            p++;
            return result;
        }

        ConstIterator &operator--() {
            if (*this == vector_ptr->begin()) throw std::out_of_range("error");
            p--;
            return *this;
        }

        ConstIterator operator--(int) {
            if (*this == vector_ptr->begin()) throw std::out_of_range("error");
            ConstIterator result = *this;
            p--;
            return result;
        }

        ConstIterator operator+(difference_type d) const {
            ConstIterator result = *this;
            result.p += d;
            return result;
        }

        ConstIterator operator-(difference_type d) const {
            ConstIterator result = *this;
            result.p -= d;
            return result;
        }

        bool operator==(const ConstIterator &other) const {
            return (p == other.p);

        }

        bool operator!=(const ConstIterator &other) const {
            return p != other.p;
        }
    };

    template<typename Type>
    class Vector<Type>::Iterator : public Vector<Type>::ConstIterator {
    public:
        using pointer = typename Vector::pointer;
        using reference = typename Vector::reference;

        explicit Iterator(pointer p1, Vector<Type> *x)
                : ConstIterator(p1, x) {}

        Iterator(const ConstIterator &other)
                : ConstIterator(other) {}

        Iterator &operator++() {
            ConstIterator::operator++();
            return *this;
        }

        Iterator operator++(int) {
            auto result = *this;
            ConstIterator::operator++();
            return result;
        }

        Iterator &operator--() {
            ConstIterator::operator--();
            return *this;
        }

        Iterator operator--(int) {
            auto result = *this;
            ConstIterator::operator--();
            return result;
        }

        Iterator operator+(difference_type d) const {
            return ConstIterator::operator+(d);
        }

        Iterator operator-(difference_type d) const {
            return ConstIterator::operator-(d);
        }

        reference operator*() const {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }
    };
}

#endif // AISDI_LINEAR_VECTOR_H
/*
Vector(std::initializer_list<Type>
l) {
size = 0;
x = new Type[l.size()];
preallocated = l.size();
auto itt = l.begin();
while (itt != l.

end()

) {
x[size] = *
itt;
++
size;
itt++;
}
} */