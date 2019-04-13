#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <boost/concept_check.hpp>

namespace aisdi {

    template<typename Type>
    class LinkedList {
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

        class Element;

    private:
        Element *first;
        Element *last;
        size_t length = 0;
    public:

        LinkedList() {
            first = new Element();
            last = new Element(first);
            first->next = last;
        }

        LinkedList(std::initializer_list<Type> l) {
            first = new Element();
            last = new Element(first);
            first->next = last;

            for (auto it = l.begin(); it != l.end(); it++) append(*it);

        }

        LinkedList(const LinkedList &other) {
            first = new Element();
            last = new Element(first);
            first->next = last;

            for (auto it = other.begin(); it != other.end(); it++) append(*it);
        }

        LinkedList(LinkedList &&other) {
            first = other.first;
            last = other.last;
            length = other.length;
            other.first = nullptr;
            other.last = nullptr;
            other.length = 0;
        }

        ~LinkedList() {
            if (length != 0) {
                iterator it = begin();
                while (first->next != last) {
                    first->next = it.current->next;
                    auto it2 = it;
                    it.current = it.current->next;
                    delete it2.current;
                }
            }
            length = 0;
            delete first;
            delete last;
        }

        LinkedList &operator=(const LinkedList &other) {
            if (this == &other) return *this;

            auto it = begin();
            while (it != end()) {
                auto del = it;
                it.current = it.current->next;
                delete del.current;
            }

            if (isEmpty()) {
                first = new Element();
                last = new Element(first);
            }
            first->next = last;
            last->previous = first;

            for (auto it = other.begin(); it != other.end(); it++) append(*it);
            return *this;
        }

        LinkedList &operator=(LinkedList &&other) {
            if (this == &other)return *this;

            auto it = begin();
            while (it != end()) {
                auto temp = it;
                it.current = it.current->next;
                delete temp.current;
            }
            delete first;
            delete last;
            first = other.first;
            last = other.last;
            length = other.length;
            other.first = nullptr;
            other.last = nullptr;
            other.length = 0;

            return *this;
        }

        bool isEmpty() const {
            return first->next == last;
        }

        size_type getSize() const {
            return length;
        }

        void append(const Type &item) {

            auto temp = new Element(item, last->previous, last);
            last->previous->next = temp;
            last->previous = temp;
            length++;
        }

        void prepend(const Type &item) {
            auto temp = new Element(item, first, first->next);
            first->next->previous = temp;
            first->next = temp;
            length++;
        }

        void insert(const const_iterator &insertPosition, const Type &item) {
            if (insertPosition == end()) {
                append(item);
            } else if (insertPosition == begin()) {
                prepend(item);
            } else {
                auto temp = new Element(item, insertPosition.current->previous, insertPosition.current);
                insertPosition.current->previous->next = temp;
                insertPosition.current->previous = temp;
                length++;
            }
        }


        Type popFirst() {
            if (isEmpty()) throw std::out_of_range("error");
            auto temp = begin();
            first->next = temp.current->next;
            temp.current->next->previous = first;
            temp.current->next = nullptr;
            temp.current->previous = nullptr;
            length--;
            auto x = temp.current->item;
            delete temp.current;
            return x;
        }

        Type popLast() {
            if (isEmpty()) throw std::out_of_range("error");
            auto temp = last->previous;
            last->previous = temp->previous;
            temp->previous->next = last;
            temp->next = nullptr;
            temp->previous = nullptr;
            length--;
            auto x = temp->item;
            delete temp;
            return x;
        }

        void erase(const const_iterator &possition) {
            if (isEmpty()) throw std::out_of_range("error");
            if (possition == end()) throw std::out_of_range("error");
            possition.current->next->previous = possition.current->previous;
            possition.current->previous->next = possition.current->next;
            possition.current->next = nullptr;
            possition.current->previous = nullptr;
            delete possition.current;
            length--;
        }

        void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded) {
            firstIncluded.current->previous->next = lastExcluded.current;
            lastExcluded.current->previous = firstIncluded.current->previous;
            Element *temp;
            for (auto it = firstIncluded; it != lastExcluded;) {
                if (it == end()) throw std::out_of_range("error");
                temp = it.current;
                it.current = it.current->next;
                delete temp;
                length--;
            }
        }

        iterator begin() {
            return iterator(first->next, this);
        }

        iterator end() {
            return iterator(last, this);
        }

        const_iterator cbegin() const {
            return const_iterator(first->next, this);
        }

        const_iterator cend() const {
            return const_iterator(last, this);
        }

        const_iterator begin() const {
            return cbegin();
        }

        const_iterator end() const {
            return cend();
        }
    };

    template<typename Type>
    class LinkedList<Type>::Element {
    public:

        Element *previous;
        Element *next;
        Type item;


        explicit Element(Element *x = nullptr, Element *y = nullptr) : previous(x), next(y) {};

        explicit Element(const Type &z, Element *x = nullptr, Element *y = nullptr) : previous(x), next(y), item(z) {};

        ~Element() = default;

    };

    template<typename Type>
    class LinkedList<Type>::ConstIterator {

    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        Element *current;
        const LinkedList<Type> *this_list;

        ConstIterator() = default;

        explicit ConstIterator(Element *x, const LinkedList<Type> *y) {
            current = x;
            this_list = y;
        }

        reference operator*() const {
            if (current == this_list->last) throw std::out_of_range("error");
            return current->item;
        }

        ConstIterator &operator++() {
            if (*this == this_list->end()) throw std::out_of_range("error");
            current = current->next;
            return *this;

        }

        ConstIterator operator++(int) {
            if (*this == this_list->end()) throw std::out_of_range("error");
            auto temp = *this;
            current = current->next;
            return temp;
        }

        ConstIterator &operator--() {
            if (*this == this_list->begin()) throw std::out_of_range("error");
            current = current->previous;
            return *this;
        }

        ConstIterator operator--(int) {
            if (*this == this_list->begin()) throw std::out_of_range("error");
            auto temp = *this;
            current = current->previous;
            return temp;
        }

        ConstIterator operator+(difference_type d) const {
            auto temp = *this;
            while (d != 0) {
                if (temp == this_list->end()) throw std::out_of_range("error");
                temp.current = temp.current->next;
                d--;
            }
            return temp;
        }

        ConstIterator operator-(difference_type d) const {
            auto temp = *this;
            while (d != 0) {
                if (temp == this_list->begin()) throw std::out_of_range("error");
                temp.current = temp.current->previous;
                d--;
            }
            return temp;
        }

        bool operator==(const ConstIterator &other) const {
            return current == other.current;
        }

        bool operator!=(const ConstIterator &other) const {
            return current != other.current;
        }
    };

    template<typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;


        explicit Iterator() {}

        Iterator(Element *x, LinkedList<Type> *y) : ConstIterator(x, y) {}

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

#endif // AISDI_LINEAR_LINKEDLIST_H
