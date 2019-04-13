
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
/*
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
        Element *elem;
        size_t length = 0;
    public:

        LinkedList() {
            elem = new Element;
        }

        LinkedList(std::initializer_list<Type> l) {
            Element *prev_elem = nullptr;
            for (auto item: l) {
                auto next_elem = new Element;
                *elem = item;
                elem->change_next(next_elem);
                elem->change_previous(prev_elem);
                prev_elem = elem;
                elem = next_elem;
                length++;
            }
        }

        LinkedList(const LinkedList &other) {
            while (length <= other.length) {
                elem = other.elem;
                auto next_elem = new Element;
                elem = next_elem; // może też prev
                length++;
            }

        }

        LinkedList(LinkedList &&other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        ~LinkedList() {
            delete elem;
        }

        LinkedList &operator=(const LinkedList &other) {
            while (length <= other.length) {
                elem = other.elem;
                auto next_elem = new Element;
                elem = next_elem; // może też prev
                length++;
            }
            return *this;
        }

        LinkedList &operator=(LinkedList &&other) {
            (void) other;
            throw std::runtime_error("TODO");
        }

        bool isEmpty() const {
            return length == 0;
        }

        size_type getSize() const {
            return length;
        }

        void append(const Type &item) {
            auto x = item;
            if (isEmpty()) elem->item = x;
            else {
                while (elem->return_next() != nullptr)
                    elem->go_to_next();
                auto new_elem = new Element;
                new_elem->change_previous(elem);
                elem->change_next(new_elem);
                *new_elem = x;
                elem->go_to_next();
            }

            length++;
        }

        void prepend(const Type &item) {
            auto x = item;
            if (isEmpty()) *elem = x;
            else {
                while (elem->return_previous() != nullptr)
                    elem->go_to_previous();
                auto new_elem = new Element;
                new_elem->change_next(elem);
                elem->change_previous(new_elem);
                *new_elem = x;
                elem->go_to_previous();
            }

            length++;
        }

        void insert(const const_iterator &insertPosition, const Type &item) {
            (void) insertPosition;
            (void) item;
            throw std::runtime_error("TODO");
        }

        Type popFirst() {
            throw std::runtime_error("TODO");
        }

        Type popLast() {
            throw std::runtime_error("TODO");
        }

        void erase(const const_iterator &possition) {
            (void) possition;
            throw std::runtime_error("TODO");
        }

        void erase(const const_iterator &firstIncluded, const const_iterator &lastExcluded) {
            (void) firstIncluded;
            (void) lastExcluded;
            throw std::runtime_error("TODO");
        }

        iterator begin() {
            while (elem->return_previous() != nullptr)
                elem->go_to_previous();
            iterator it(elem);
            return it;


        }

        iterator end() {
            while (elem->return_next() != nullptr)
                elem->go_to_next();
            iterator it(elem);
            return it;

        }

        const_iterator cbegin() const {
            throw std::runtime_error("TODO");
        }

        const_iterator cend() const {
            throw std::runtime_error("TODO");
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
        Element *next;
        Element *previous;
        Type item;

        Element() {
            next = nullptr;
            previous = nullptr;
        }

        ~Element() {
            delete next;
            delete previous;
        }

        Element(Element *x = nullptr, Type &y, Element *z = nullptr) {
            previous = x;
            item = y;
            next = z;
        }


        Element *return_next() {
            return next;
        }

        Element *return_previous() {
            return previous;
        }

        void go_to_next() {
            *this = next;
        }

        void go_to_previous() {
            *this = previous;
        }

        void change_previous(Element *x) {
            *previous = x;
        }

        void change_next(Element *x) {
            *next = x;
        }

        Element &operator=(Type &other) {
            item = other;
            return *this;
        }


        Element &operator=(Element *other) {
            item = other->item;
            previous = other->previous;
            next = other->next;
            return *this;
        }

        bool operator==(Element *other) {
            return this->item == other->item;
        }

        bool operator!=(Element *other) {
            return this->item != other->item;
        }
    };

    template<typename Type>
    class LinkedList<Type>::ConstIterator {
    private:
        Element *elem;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = typename LinkedList::value_type;
        using difference_type = typename LinkedList::difference_type;
        using pointer = typename LinkedList::const_pointer;
        using reference = typename LinkedList::const_reference;

        explicit ConstIterator(Element *x = new Element) {
            elem = x;
        }

        reference operator*() const {
            return (elem->item);
        }

        ConstIterator &operator++() {
            if (elem->return_next() == nullptr) throw std::out_of_range("error");
            elem->go_to_next();
            return *this;
        }

        ConstIterator operator++(int) {
            if (elem->return_next() == nullptr) throw std::out_of_range("error");
            ConstIterator it_elem2;
            it_elem2 = elem;
            elem->go_to_next();
            return it_elem2;
        }

        ConstIterator &operator--() {
            if (elem->return_previous() == nullptr) throw std::out_of_range("error");
            elem->go_to_previous();
            return *this;
        }

        ConstIterator operator--(int) {
            if (elem->return_previous() == nullptr) throw std::out_of_range("error");
            ConstIterator it_elem2;
            it_elem2 = elem;
            elem->go_to_previous();
            return it_elem2;
        }

        ConstIterator operator+(difference_type d) const {
            size_t x = d;
            ConstIterator it;
            while (x != 0) {
                if (elem->return_next() == nullptr) throw std::out_of_range("error");
                elem->go_to_next();
                x--;
            }
            it = elem;
            return it;
        }

        ConstIterator operator-(difference_type d) const {
            ConstIterator it;
            size_t x = d;
            while (x != 0) {
                if (elem->return_previous() == nullptr) throw std::out_of_range("error");
                elem->go_to_previous();
                x--;
            }
            it = elem;
            return it;
        }

        bool operator==(const ConstIterator &other) const {
            return this->elem == other.elem;
        }

        bool operator!=(const ConstIterator &other) const {
            return this->elem == other.elem;
        }

        ConstIterator &operator=(Element *x) {
            elem = x;
            return *this;
        }

    };

    template<typename Type>
    class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator {
    public:
        using pointer = typename LinkedList::pointer;
        using reference = typename LinkedList::reference;

        explicit Iterator(Element *x = nullptr) :
                ConstIterator(x) {}

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
