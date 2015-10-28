#ifndef UTILS_ALLOCATION_HELPERS_HPP_INCLUDED
#define UTILS_ALLOCATION_HELPERS_HPP_INCLUDED 1

#include <new>

namespace gmb { namespace utils { namespace detail
{
  inline void * allocate_operation(size_t n, ...)
  {
    return ::operator new(n);
  }

  inline void deallocate_operation(void *p, size_t n, ...)
  {
    ::operator delete(p, n);
  }

  template<typename T, typename ReturnType, typename ... Args>
  inline variant_operation_base<ReturnType, Args...> * make_operation_wrapper(T &&callable)
  {
    void *p = allocate_operation(
      sizeof(variant_operation_wrapper<T, ReturnType, Args...>::this_t), &callable);

    return new (p) variant_operation_wrapper<T, ReturnType, Args...>::this_t(
      std::forward<T>(callable));
  }

}}}

#endif //UTILS_ALLOCATION_HELPERS_HPP_INCLUDED
