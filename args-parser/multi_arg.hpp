
/*
    SPDX-FileCopyrightText: 2013-2024 Igor Mironchik <igor.mironchik@gmail.com>
    SPDX-License-Identifier: MIT
*/

#ifndef ARGS__MULTI_ARG_HPP__INCLUDED
#define ARGS__MULTI_ARG_HPP__INCLUDED

// Args include.
#include "arg.hpp"
#include "context.hpp"
#include "exceptions.hpp"
#include "types.hpp"
#include "utils.hpp"
#include "value_utils.hpp"

// C++ include.
#include <utility>

namespace Args
{

//
// MultiArg
//

/*!
    MultiArg is a class that presents argument in the command
    line taht can be presented more than once or can have more
    than one value.
*/
class MultiArg : public Arg
{
public:
    //! Construct argument with flag and name.
    template<typename T>
    MultiArg(
        //! Flag for this argument.
        Char flag,
        //! Name for this argument.
        T &&name,
        //! Is this argument with value?
        bool isWithValue = false,
        //! Is this argument required?
        bool isRequired = false);

    //! Construct argument only with flag, without name.
    explicit MultiArg(
        //! Flag for this argument.
        Char flag,
        //! Is this argument with value?
        bool isWithValue = false,
        //! Is this argument required?
        bool isRequired = false);

    //! Construct argument only with name, without flag.
    template<typename T>
    explicit MultiArg(
        //! Name for this argument.
        T &&name,
        //! Is this argument with value?
        bool isWithValue = false,
        //! Is this argument required?
        bool isRequired = false);

    virtual ~MultiArg();

    //! \return Type of the argument.
    ArgType type() const override
    {
        return ArgType::MultiArg;
    }

    //! \return First value of this argument.
    const String &value() const override;
    //! Set value. \note Value will be pushed back to the list of values.
    void setValue(const String &v) override;

    //! \return All values for this argument.
    virtual const StringList &values() const;

    /*!
        \return Count of times that this argument was presented
        in the command line if argument without value or count of values
        if argument is with values.
    */
    size_t count() const;

    //! \return Default value.
    const String &defaultValue() const override
    {
        if (!m_defaultValues.empty()) {
            return m_defaultValues.front();
        } else {
            return details::DummyString<>::c_string;
        }
    }

    //! Set default value. \note Value will be pushed back to the list
    //! of default values.
    void setDefaultValue(const String &v) override
    {
        m_defaultValues.push_back(v);
    }

    //! \return Default values.
    const StringList &defaultValues() const
    {
        return m_defaultValues;
    }

    //! Set default values.
    void setDefaultValues(const StringList &v)
    {
        m_defaultValues = v;
    }

    //! Clear state of the argument.
    void clear() override
    {
        setDefined(false);

        m_values.clear();
    }

protected:
    /*!
        Process argument's staff, for example take values from
        context. This method invokes exactly at that moment when
        parser has found this argument.
    */
    void process(
        //! Context of the command line.
        Context &context) override;

private:
    DISABLE_COPY(MultiArg)

    //! Values of this argument.
    StringList m_values;
    //! Counter.
    size_t m_count;
    //! Default values.
    StringList m_defaultValues;
}; // class MultiArg

//
// MultiArg
//

template<typename T>
MultiArg::MultiArg(Char flag,
                   T &&name,
                   bool isWithValue,
                   bool isRequired)
    : Arg(flag,
          std::forward<T>(name),
          isWithValue,
          isRequired)
    , m_count(0)
{
}

inline MultiArg::MultiArg(Char flag,
                          bool isWithValue,
                          bool isRequired)
    : Arg(flag,
          isWithValue,
          isRequired)
    , m_count(0)
{
}

template<typename T>
MultiArg::MultiArg(T &&name,
                   bool isWithValue,
                   bool isRequired)
    : Arg(std::forward<T>(name),
          isWithValue,
          isRequired)
    , m_count(0)
{
}

inline MultiArg::~MultiArg()
{
}

inline const String &MultiArg::value() const
{
    if (!m_values.empty()) {
        return m_values.front();
    } else if (!m_defaultValues.empty()) {
        return m_defaultValues.front();
    } else {
        return details::DummyString<>::c_string;
    }
}

inline void MultiArg::setValue(const String &v)
{
    m_values.push_back(v);
}

inline const StringList &MultiArg::values() const
{
    if (!m_values.empty()) {
        return m_values;
    } else {
        return m_defaultValues;
    }
}

inline size_t MultiArg::count() const
{
    if (!isWithValue()) {
        return m_count;
    } else if (!m_values.empty()) {
        return m_values.size();
    } else {
        return m_defaultValues.size();
    }
}

inline void MultiArg::process(Context &context)
{
    if (isWithValue()) {
        setDefined(eatValues(context,
                             m_values,
                             String(SL("Argument \"")) + name() + SL("\" requires value that wasn't presented."),
                             cmdLine()));
    } else {
        setDefined(true);

        ++m_count;
    }
}

} /* namespace Args */

#endif // ARGS__MULTI_ARG_HPP__INCLUDED
