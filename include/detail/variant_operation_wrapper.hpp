#ifndef UTILS_VARIANT_OPERATION_WRAPPER_HPP_INCLUDED
#define UTILS_VARIANT_OPERATION_WRAPPER_HPP_INCLUDED 1

namespace gmb { namespace utils { namespace detail
{
//  using basic_variant_operation_base = variant_operation_base<void>;
//  using basic_vtbl_t = basic_variant_operation_base::vtbl_t;

  template<typename Callable, typename ReturnType, typename ... Args>
  class variant_operation_wrapper : variant_operation_base<ReturnType, Args...>
  {
    template<typename T, typename RTy, typename ... ArgTys>
    friend variant_operation_base<RTy, ArgTys...> * 
      make_operation_wrapper<T, RTy, ArgTys...>(T &&);

    using this_t = variant_operation_wrapper<Callable, ReturnType, Args...>;
    using base_t = variant_operation_base<ReturnType, Args...>;
    using vtbl_t = typename base_t::vtbl_t;

  public:
    ReturnType operator()(Args&& ... args)
    {
      return callable_(std::forward<Args>(args)...);
    }

  private:

    variant_operation_wrapper(Callable &&callable)
      : base_t(&vtbl_),
        callable_(std::forward<Callable>(callable))
    { }

    static ReturnType invoke_impl(base_t *base, Args&& ... args)
    {
      return (*static_cast<this_t *>(base))(std::forward<Args>(args)...);
    }

    static base_t * clone_impl(base_t *base)
    {
      void *p = allocate_operation(sizeof(this_t), 
        &static_cast<this_t *>(base)->callable_);

      return new (p) this_t(*static_cast<this_t *>(base));
    }

    static void destruct_impl(base_t *base)
    {
      auto *p = static_cast<this_t *>(base);
      Callable tmp(p->callable_);
      p->~this_t();
      deallocate_operation(p, sizeof(this_t), &tmp);
    }

    Callable callable_;

    static vtbl_t vtbl_;
  };

  template<typename Callable, typename ReturnType, typename ... Args>
  typename variant_operation_wrapper<Callable, ReturnType, Args...>::vtbl_t 
    variant_operation_wrapper<Callable, ReturnType, Args...>::vtbl_ = {
      &variant_operation_wrapper::clone_impl,
      &variant_operation_wrapper::destruct_impl,
      &variant_operation_wrapper::invoke_impl
    };

}}}
#endif //UTILS_VARIANT_OPERATION_WRAPPER_HPP_INCLUDED
