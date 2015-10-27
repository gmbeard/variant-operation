#include <iostream>
#include <variant_operation.hpp>

struct my_operation
{
  void operator()()
  {
    std::cout << "Hello, World from my_operation!" << std::endl;
  }
};

void * allocate_operation(size_t n, my_operation *)
{
  std::cout << "Allocating using " 
    << __FUNCSIG__ << std::endl;

  return ::operator new(n);
}

void deallocate_operation(void *p, size_t n, my_operation *)
{
  std::cout << "Deallocating using "
    << __FUNCSIG__ << std::endl;

  return ::operator delete(p, n);
}

int main(int, char const *[])
{
  using std::cout;
  using std::endl;
  using gmb::utils::basic_variant_operation;
  using gmb::utils::variant_operation;

  variant_operation v([&]{ cout << "Hello, World!" << endl; });
  v();

  v = []{cout << "Goodbye, World!" << endl;};
  v();

  variant_operation v3 = v;

  variant_operation v2((my_operation()));
  v2();

  basic_variant_operation<int, int, int> 
    v4([](int a, int b) { return a + b; });

  cout << v4(1, 2) << endl;

//  variant_operation v5(42); // Won't compile

  return 0;
}
