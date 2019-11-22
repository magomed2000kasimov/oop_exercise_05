#pragma once

#include <iterator>
#include <memory>

namespace cntr {

    template<class T>
    class Stack {
    private:
        class StackNode;

    public:
        class ForwardIterator {
        public:
            using value_type = T;
            using reference = T &;
            using pointer = T *;
            using difference_type = ptrdiff_t;
            using iterator_category = std::forward_iterator_tag;

            ForwardIterator(StackNode *node) : Ptr(node) {};

            T &operator*();

            ForwardIterator &operator++();

            ForwardIterator operator++(int);

            bool operator==(const ForwardIterator &it) const;

            bool operator!=(const ForwardIterator &it) const;

        private:
            StackNode *Ptr;

            friend class Stack;
        };

        ForwardIterator begin();

        ForwardIterator end();

        T Top();

        void Insert(const ForwardIterator &it, const T &value);

        void Erase(const ForwardIterator &it);

        void Pop();

        void Push(const T &value);

        size_t Size = 0;
    private:
        class StackNode {
        public:
            T Value;
            std::unique_ptr<StackNode> NextNode = nullptr;

            ForwardIterator next();

            StackNode(const T &value, std::unique_ptr<StackNode> next) : Value(value), NextNode(std::move(next)) {};
        };
        std::unique_ptr<StackNode> Head = nullptr;
        Stack(const Stack&);
        Stack&operator=(const Stack&);
    };


    template<class T>
    typename Stack<T>::ForwardIterator Stack<T>::StackNode::next() {
        return {NextNode.get()};
    }

    template<class T>
    T &Stack<T>::ForwardIterator::operator*() {
        return Ptr->Value;
    }

    template<class T>
    typename Stack<T>::ForwardIterator &Stack<T>::ForwardIterator::operator++() {
        *this = Ptr->next();
        return *this;
    }

    template<class T>
    typename Stack<T>::ForwardIterator Stack<T>::ForwardIterator::operator++(int) {
        ForwardIterator prev = *this;
        ++(*this);
        return prev;
    }

    template<class T>
    bool Stack<T>::ForwardIterator::operator!=(const ForwardIterator &other) const {
        return Ptr != other.Ptr;
    }

    template<class T>
    bool Stack<T>::ForwardIterator::operator==(const ForwardIterator &other) const {
        return Ptr == other.Ptr;
    }

    template<class T>
    typename Stack<T>::ForwardIterator Stack<T>::begin() {
        return Head.get();
    }

    template<class T>
    typename Stack<T>::ForwardIterator Stack<T>::end() {
        return nullptr;
    }

    template<class T>
    void Stack<T>::Insert(const ForwardIterator &it, const T &value) {
        std::unique_ptr<StackNode> newNode(new StackNode(value, nullptr));
        if (it.Ptr == nullptr && Size != 0) {
            throw std::logic_error("Out of bounds");
        }
        if (Size == 0) {
            Head = std::move(newNode);
            ++Size;
        } else {
            newNode->NextNode = std::move(it.Ptr->NextNode);
            it.Ptr->NextNode = std::move(newNode);
            ++Size;
        }
    }

    template<class T>
    void Stack<T>::Push(const T &value) {
        std::unique_ptr<StackNode> newNode(new StackNode(value, nullptr));
        newNode->NextNode = std::move(Head);
        Head = std::move(newNode);
        ++Size;
    }

    template<class T>
    T Stack<T>::Top() {
        if (Head.get()) {
            return Head->Value;
        } else {
            throw std::logic_error("Stack is empty");
        }
    }

    template<class T>
    void Stack<T>::Pop() {
        if (Head.get() == nullptr) {
            throw std::logic_error("Stack is empty");
        }
        Head = std::move(Head->NextNode);
        --Size;
    }

    template<class T>
    void Stack<T>::Erase(const Stack<T>::ForwardIterator &it) {
        if (it.Ptr == nullptr) {
            throw std::logic_error("Invalid iterator");
        }
        if (it == this->begin()) {
            Head = std::move(Head->NextNode);

        } else {
            auto tmp = this->begin();
            while (tmp.Ptr->next() != it.Ptr) {
                ++tmp;
            }
            tmp.Ptr->NextNode = std::move(it.Ptr->NextNode);
        }
    }

}













