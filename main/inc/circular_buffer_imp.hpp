
#include "circular_buffer.hpp"

template<typename Type, int Size> CircularBuffer<Type, Size>::CircularBuffer()
{
}

template<typename Type, int Size> CircularBuffer<Type, Size>::~CircularBuffer()
{
}

template<typename Type, int Size> Type CircularBuffer<Type, Size>::Peek()
{
    const std::lock_guard<std::mutex> lock(m_access_mutex);
    return m_data[m_position];
}

template<typename Type, int Size> void CircularBuffer<Type, Size>::Advance()
{
    const std::lock_guard<std::mutex> lock(m_access_mutex);
    if((++m_position) >= Size) 
    {
        m_position = 0;
    }
}

template<typename Type, int Size> void CircularBuffer<Type, Size>::Modify(Type value)
{
    const std::lock_guard<std::mutex> lock(m_access_mutex);
    m_data[m_position] = value;
}

template<typename Type, int Size> void CircularBuffer<Type, Size>::Insert(Type value)
{
    const std::lock_guard<std::mutex> lock(m_access_mutex);
    if((++m_position) >= Size) 
    {
        m_position = 0;
    }
    m_data[m_position] = value;
}

template<typename Type, int Size> Type CircularBuffer<Type, Size>::Max()
{
    const std::lock_guard<std::mutex> lock(m_access_mutex);
    Type max_value = 0;
    for(auto& value : m_data)
    {
        if(value > max_value)
        {
            max_value = value;
        }
    }
    return max_value;
}

template<typename Type, int Size> Type CircularBuffer<Type, Size>::Min()
{
    const std::lock_guard<std::mutex> lock(m_access_mutex);
    Type min_value = 0;
    for(auto& value : m_data)
    {
        if(value < min_value)
        {
            min_value = value;
        }
    }
    return min_value;
}

template<typename Type, int Size> Type CircularBuffer<Type, Size>::Mean()
{
    const std::lock_guard<std::mutex> lock(m_access_mutex);
    Type sum = 0;
    for(auto& value : m_data)
    {
        sum += value;
    }
    return sum/Size;
}
