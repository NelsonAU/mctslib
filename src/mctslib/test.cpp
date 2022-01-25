#include <iostream>


template<class T, class A>
class Test {
public:
    Test () {
        std::cout << "Who knows what the type is" << std::endl;
    }
};

template<class A>
using C = Test<int, A>;

int main() {
    C<int> c;
}
