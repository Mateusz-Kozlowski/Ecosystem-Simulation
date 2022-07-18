#pragma once

/*
The following class is based on an answer to a question from StackOverflow.
The answer: https://stackoverflow.com/a/12262626
*/

namespace Blueberry
{
    class Formatter
    {
    public:
        Formatter() {}
        ~Formatter() {}

        template <typename T>
        Formatter& operator << (const T& value)
        {
            m_stream << value;
            return *this;
        }

        std::string str() const { return m_stream.str(); }
        operator std::string() const { return m_stream.str(); }

        enum class ConvertToString { to_str };

        std::string operator>>(ConvertToString) { return m_stream.str(); }

    private:
        std::stringstream m_stream;
    };
}
