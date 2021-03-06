#ifndef SIMPPL_DETAIL_IS_VECTOR_H
#define SIMPPL_DETAIL_IS_VECTOR_H


#include <vector>


namespace simppl
{
   
namespace dbus
{

namespace detail
{
   
template<typename T>
struct is_vector
{
   enum { value = false };
};


template<typename T>
struct is_vector<std::vector<T> >
{
   enum { value = true };
};

}   // namespace detail

}   // namespace dbus

}   // namespace simppl


#endif   // SIMPPL_DETAIL_IS_VECTOR_H