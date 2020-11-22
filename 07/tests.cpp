#include <iostream>
#include <vector>
#include <cassert>

#include "Vector.h"

template<typename T>
void assert_equals(const Vector<T>& my_vec, const std::vector<T> vec) {
    assert (my_vec.size() == vec.size());

    for (size_t i = 0; i < vec.size(); ++i) {
        assert(vec[i] == my_vec[i]);
    }
}

int main()
{
    Vector<int> my_vec = {1, 2, 3};
    std::vector<int> vec = {1, 2, 3};
    assert(!my_vec.empty());
    assert_equals(my_vec, vec);

    my_vec.pop_back(); vec.pop_back();
    my_vec.push_back(4); vec.push_back(4);
    assert_equals(my_vec, vec);

    my_vec.clear(); vec.clear();
    assert(my_vec.size() == vec.size());
    assert(my_vec.empty());

    my_vec.push_back(1); vec.push_back(1);
    my_vec.push_back(1); vec.push_back(1);
    my_vec.emplace_back(1); vec.emplace_back(1);
    assert(my_vec.size() == vec.size());

    {
        int counter = 0;
        for (auto it = my_vec.begin(); it != my_vec.end(); ++it, ++counter) {
            assert(*it == 1);
        }
        assert(counter == 3);
    }

    std::cout << "TESTS OK" << std::endl;
    return 0;
}
