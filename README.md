# variant-operation
A library for creating deferred operations with variable arguments / return type, and custom allocation support
## Usage
The most basic use is to create with lamdas / functors with empty return / params...
```c++
using gmb::utils::variant_operation;

variant_operation op([]{std::cout << "Hello, World!\n";});

//  ... Store for future use ...

op();
```
... However, the return type and params can be explicitly specified ...
```c++
using gmb::utils::basic_variant_operation;

//  Alias to an operation taking 2 ints and returning int...
using my_operation_t = 
  basic_variant_operation<int /* rettype */, int /* arg */, int /* arg */>;

my_operation_t op([](int a, int b){ return a + b; });
//  ... Store for future use ...

std::cout << "Result: " << op(1, 2);
```
Memory allocation can also be explicitly controlled. This is acheived by providing your own overloads for `allocate_operation` and `deallocate_operation`. Your overloads will then be resolved using ADL instead of the default implementations. This is useful if you're implementing an async completion queue, for example, and you want to be in complete control of the memory the queue uses.
```c++
struct my_op
{
  void operator()() { std::cout << "Hello, Custom allocation!\n"; }
};

void * allocate_operation(size_t n, my_op *)
{
  //  This will be called instead of the default
  //  implementation. Allocate how ever we want...

  return ::operator new(n);
}

void deallocate_operation(void *p, size_t n, my_op *)
{
  //  You MUST then also provide a custom deallocator
  ::operator delete(p, n);
}

int main(int, char const *[]) 
{
  using gmb::utils::variant_operation;

  //  Creating operations with my_op types
  //  will now invoke custom (de)allocation...
  variant_operation op((my_op()));

  ...
}
```
