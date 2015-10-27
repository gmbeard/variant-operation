#ifndef UTILS_VARIANT_OPERATION_HPP_INCLUDED
#define UTILS_VARIANT_OPERATION_HPP_INCLUDED 1

#include <utility>
#include <type_traits>

#include "detail/variant_operation_base.hpp"
#include "detail/allocation_helpers.hpp"
#include "detail/variant_operation_wrapper.hpp"

namespace gmb { namespace utils
{
  template<typename ReturnType, typename ... Args>
  class basic_variant_operation
  {
  public:
    template<typename Callable>
    explicit basic_variant_operation(Callable &&callable)
      : base_(detail::make_operation_wrapper<Callable, ReturnType, Args...>(
          std::forward<Callable>(callable)))
    { }

    basic_variant_operation(basic_variant_operation const &other)
      : base_(other.base_->clone())
    { }

    ~basic_variant_operation()
    {
      base_->destruct();
    }

    basic_variant_operation & operator=(basic_variant_operation const &rhs)
    {
      basic_variant_operation tmp(rhs);
      swap(*this, tmp);
      return *this;
    }

    template<typename OtherCallable>
    typename std::enable_if<!std::is_same<basic_variant_operation, OtherCallable>::value, 
      basic_variant_operation>::type & operator=(OtherCallable &&rhs)
    {
      basic_variant_operation tmp(rhs);
      swap(*this, tmp);
      return *this;
    }

    friend void swap(basic_variant_operation &lhs, basic_variant_operation &rhs)
    {
      using std::swap;
      swap(lhs.base_, rhs.base_);
    }

    //template<typename ... Args>
    ReturnType operator()(Args&& ... args)
    {
      return base_->invoke(std::forward<Args>(args)...);
    }

  private:
    detail::variant_operation_base<ReturnType, Args...> *base_;
  };

  using variant_operation = basic_variant_operation<void>;
}}

#endif //UTILS_VARIANT_OPERATION_HPP_INCLUDED
