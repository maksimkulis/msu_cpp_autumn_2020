#include <iostream>
#include <vector>
#include <cassert>

#include "Vector.h"

template<typename T1, typename T2>
void assert_equals(const T1& lhs, const T2& rhs) {
    assert (lhs.size() == rhs.size());

    for (size_t i = 0; i < lhs.size(); ++i) {
        assert(lhs[i] == rhs[i]);
    }
}

int main()
{
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
    }

    {
        Vector<int> vec1 = {2, 3, 4};
        Vector<int> vec2 = {1, 2, 3};
        assert_equals(vec1, Vector<int>{2, 3, 4});
        vec1 = vec2;
        assert_equals(vec1, vec2);
    }
    std::cout << "TESTS OK" << std::endl;
    return 0;
}
