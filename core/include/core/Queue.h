#pragma once

#include <list>

namespace core {

template<typename T>
class Queue {
public:
    typedef typename std::list<T>::iterator iterator;

    typedef typename std::list<T>::const_iterator const_iterator;

    iterator begin() { return m_list.begin(); }

    const_iterator begin() const { return m_list.begin(); }

    iterator end() { return m_list.end(); }

    const_iterator end() const { return m_list.end(); }

public:
    bool empty() const { return m_list.empty(); }

    T const& front() const { return m_list.front(); }

    T& front() { return m_list.front(); }

    T const& back() const { return m_list.back(); }

    T& back() { return m_list.back(); }

    void push(T const& t) { m_list.push_back(t); }

    void pop_front() { if (!empty()) m_list.pop_front(); }

    void pop_back() { if (!empty()) m_list.pop_back(); }

    size_t size() { return m_list.size(); }

private:
    std::list<T> m_list;
};

}