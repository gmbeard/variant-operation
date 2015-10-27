#ifndef UTILS_VARIANT_OPERATION_BASE_HPP_INCLUDED
#define UTILS_VARIANT_OPERATION_BASE_HPP_INCLUDED 1

#include <utility>

namespace gmb { namespace utils { namespace detail {

  template<typename ReturnType, typename ... Args>
  struct variant_operation_base;

  template<typename ReturnType, typename ... Args>
  struct variant_operation_vtable
  {
    using base_t      = variant_operation_base<ReturnType, Args...>;
    using invoke_t    = ReturnType (*)(base_t *, Args&& ...);
    using clone_t     = base_t * (*)(base_t *);
    using destruct_t  = void (*)(base_t *);

    clone_t     clone;
    destruct_t  destruct;
    invoke_t    invoke;
  };

  template<typename ReturnType, typename ... Args>
  struct variant_operation_base
  {
    using vtbl_t = variant_operation_vtable<ReturnType, Args...>;

    variant_operation_base(vtbl_t *vtbl)
      : vtbl_(vtbl)
    { }

    ReturnType invoke(Args&& ... args)
    {
      return vtbl_->invoke(this, std::forward<Args>(args)...);
    }

    variant_operation_base * clone()
    {
      return vtbl_->clone(this);
    }

    void destruct()
    {
      vtbl_->destruct(this);
    }

  private:
    vtbl_t *vtbl_;
  };

}}}

#endif //UTILS_VARIANT_OPERATION_BASE_IMPL_HPP_INCLUDED
