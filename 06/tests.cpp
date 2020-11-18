#include <iostream>
#include <string>
#include <cassert>

#include "format.h"

#define check_err(str, ...) {bool err = false; try {format(str, __VA_ARGS__);} catch(std::runtime_error) {err = true;} assert(err);}

int main()
{
    assert(format("kek!!!", 1, 2) == "kek!!!");
    assert(format("{1}opa chirik {0} {1}", 5, 10) == ("10opa chirik 5 10"));
    assert(format("Kupil {0} shlyapu, {1} ona {2} kak {3}", "mujik", "a", "emu", "ras") == 
                  "Kupil mujik shlyapu, a ona emu kak ras");

    assert(format("{1}{0}{2}{4}", false, true, 2, 3, "444") == ("102444"));
    assert(format("mail.ru") == ("mail.ru"));
    assert(format("{0}", "{0}") == ("{0}"));
    
    check_err("ras dva{4r}", 5);
    check_err("Chill bez krinja{}", 0);
    check_err("okay boomer}", 2);
    check_err("{0}{2020}{1}", 1, "lel");
    check_err("{{0}}", 1);
    check_err("Big brother is watching {0", "you");

    std::cout << "TESTS OK" << std::endl;
    return 0;
}