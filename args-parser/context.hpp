
/*
    SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef ARGS__CONTEXT_HPP__INCLUDED
#define ARGS__CONTEXT_HPP__INCLUDED

// C++ include.
#include <utility>

// Args include.
#include "types.hpp"
#include "utils.hpp"

namespace Args
{

//
// ContextInternal
//

//! What Context actually is.
typedef StringList ContextInternal;

//
// Context
//

/*!
    Context is a list of words in the command line that user
    presented with interface for interacting with them.
*/
class Context final
{
public:
    Context()
        : m_it(m_context.begin())
    {
    }

    explicit Context(ContextInternal items);

    Context &operator=(ContextInternal &&items)
    {
        m_context = std::move(items);

        m_it = m_context.begin();

        return *this;
    }

    //! \return Iterator to the first item in the context.
    ContextInternal::iterator begin();

    //! \return Iterator to the last item in the context.
    ContextInternal::iterator end();

    //! \return Is context at end?
    bool atEnd();

    //! \return Iterator to the next item in the context.
    ContextInternal::iterator next();

    //! Put back last taken item.
    void putBack();

    //! Prepend context with new item.
    void prepend(const String &what);

private:
    DISABLE_COPY(Context)

    //! Internal representation of the context.
    ContextInternal m_context;
    //! Iterator to the current item in the context.
    ContextInternal::iterator m_it;
}; // class Context

//
// Context
//

inline Context::Context(ContextInternal items)
    : m_context(std::move(items))
    , m_it(m_context.begin())
{
}

inline ContextInternal::iterator Context::begin()
{
    return m_it;
}

inline ContextInternal::iterator Context::end()
{
    return m_context.end();
}

inline bool Context::atEnd()
{
    return (begin() == end());
}

inline ContextInternal::iterator Context::next()
{
    if (atEnd()) {
        return end();
    } else {
        return m_it++;
    }
}

inline void Context::putBack()
{
    if (begin() != m_context.begin()) {
        --m_it;
    }
}

inline void Context::prepend(const String &what)
{
    m_it = m_context.insert(m_it, what);
}

} /* namespace Args */

#endif // ARGS__CONTEXT_HPP__INCLUDED
