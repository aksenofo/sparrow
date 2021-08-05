/*
 *      Author: Alexander Ksenofontov
 *      License: MIT
 *      All right reserved
 */

#pragma once
#include <memory>
#include <mutex>

namespace sparrow
{


/*
*
*/
template<typename Type>
class Singletone
{
public:
    Singletone() = delete;
    Singletone(const Singletone& that) = delete;
    static Type& Instance();

private:
    static void Create();

    static std::once_flag m_flag;
    static std::unique_ptr<Type> m_instance;
};


template<typename Type>
std::once_flag Singletone<Type>::m_flag;
template<typename Type>
std::unique_ptr<Type> Singletone<Type>::m_instance;

template<typename Type>
inline Type& Singletone<Type>::Instance()
{
    std::call_once(m_flag, &Singletone<Type>::Create);
    return *m_instance;
}

template<typename Type>
inline void Singletone<Type>::Create()
{
    m_instance = std::make_unique<Type>();
}

} // namespace sparrow
