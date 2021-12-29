/**
 * @author Alejandro Solozabal
 *
 */

#ifndef CIRCULAR_BUFFER_HPP_
#define CIRCULAR_BUFFER_HPP_

#include <array>
#include <mutex>

template<typename Type, int Size>
class CircularBuffer
{
    public:
        CircularBuffer();
        ~CircularBuffer();
        Type Peek();
        void Advance();
        void Insert(Type value);
        void Modify(Type value);
        Type Max();
        Type Min();
        Type Mean();
    private:
        std::array<Type, Size> m_data = {};
        int m_position = 0;
        std::mutex m_access_mutex;
};

#include "circular_buffer_imp.hpp"

#endif