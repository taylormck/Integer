// --------------------------------
// projects/integer/TestInteger.c++
// Copyright (C) 2013
// Glenn P. Downing
// --------------------------------

/*
To test the program:
    % ls /usr/include/cppunit/
    ...
    TestFixture.h
    ...
    % locate libcppunit.a
    /usr/lib/libcppunit.a
    % g++ -pedantic -std=c++0x -Wall Integer.c++ TestInteger.c++ -o TestInteger -lcppunit -ldl
    % valgrind TestInteger > TestInteger.out
*/

// --------
// includes
// --------

#include <algorithm> // equal
#include <cstring>   // strcmp
#include <sstream>   // ostringstream
#include <stdexcept> // invalid_argument
#include <string>    // ==

#include "cppunit/extensions/HelperMacros.h" // CPPUNIT_TEST, CPPUNIT_TEST_SUITE, CPPUNIT_TEST_SUITE_END
#include "cppunit/TestFixture.h"             // TestFixture
#include "cppunit/TextTestRunner.h"          // TextTestRunner

#include "Integer.h"

// Useful for debugging
template<typename II>
void print_test_array(std::string testName, II b, II e) {
    std::cout << "\n" << testName << ": {";
    if (b != e)
        std::cout << *b++;
    while (b != e)
        std::cout << ", " << *b++;
    std::cout << "}" << std::endl;
}

// -----------
// TestInteger
// -----------
struct TestInteger : CppUnit::TestFixture {
    // Helper functions
    void test_strip_zeroes_01 () {
        const int a[] = {0, 0, 1};
        const int* c = strip_zeroes(a, a + 3);
        CPPUNIT_ASSERT(c - a == 2);
    }

    void test_strip_zeroes_02 () {
        const int a[] = {1};
        const int* c = strip_zeroes(a, a + 3);
        CPPUNIT_ASSERT(c - a == 0);
    }

    void test_strip_zeroes_03 () {
        const int a[] = {1, 0, 0, 1, 0};
        const int* c = strip_zeroes(a, a + 3);
        CPPUNIT_ASSERT(c - a == 0);
    }

    void test_print_list_01 () {
        const int a[] = {1, 0, 1};
        int c[10];
        int* x = print_list(a, a + 3, c);
        CPPUNIT_ASSERT(x - c == 3);
        CPPUNIT_ASSERT(std::equal(a, a + 3, a));
    }

    void test_print_list_02 () {
        const int a[] = {1};
        int c[10];
        int* x = print_list(a, a + 1, c);
        CPPUNIT_ASSERT(x - c == 1);
        CPPUNIT_ASSERT(std::equal(a, a + 1, c));
    }
    
    void test_print_list_03 () {
        const int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
        int c[10];
        int* x = print_list(a, a + 9, c);
        CPPUNIT_ASSERT(x - c == 9);
        CPPUNIT_ASSERT(std::equal(a, a + 1, c));
    }
    
    void test_print_list_04 () {
        const int a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        int c[10];
        int* x = print_list(a, a + 10, c);
        CPPUNIT_ASSERT((x - c == 9) && "Make sure not to write leading zeroes");
        CPPUNIT_ASSERT(std::equal(a + 1, a + 10, c));
    }

    void test_compare_01 () {
        const int a[] = {1, 3, 2, 6, 7, 8};
        const int b[] = {5, 6, 7};
        const int c = compare(a, a + 6, b, b + 3);
        CPPUNIT_ASSERT(c == 1);
    }

    void test_compare_02 () {
        const int a[] = {1, 3, 2};
        const int b[] = {5, 6, 7};
        const int c = compare(a, a + 3, b, b + 3);
        CPPUNIT_ASSERT(c == -1);
    }

    void test_compare_03 () {
        const int a[] = {5, 6, 7};
        const int b[] = {5, 6, 7};
        const int c = compare(a, a + 3, b, b + 3);
        CPPUNIT_ASSERT(c == 0);
    }

    void test_compare_04 () {
        const int a[] = {7, 7, 7};
        const int b[] = {5, 6, 7};
        const int c = compare(a, a + 3, b, b + 3);
        CPPUNIT_ASSERT(c == 1);
    }

    void test_compare_05 () {
        const int a[] = {1};
        const int b[] = {5};
        const int c = compare(a, a + 1, b, b + 1);
        CPPUNIT_ASSERT(c == -1);
    }

    // -----------------
    // shift_left_digits
    // -----------------

    void test_shift_left_digits () {
        const int a[] = {2, 3, 4};
        const int b[] = {2, 3, 4, 0, 0};
        int x[10];
        const int* p = shift_left_digits(a, a + 3, 2, x);
        CPPUNIT_ASSERT((p - x) == 5);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, b));
    }

    void test_shift_left_digits_01 () {
        const int a[] = {1};
        const int b[] = {1, 0, 0};
        int x[10];
        const int* p = shift_left_digits(a, a + 1, 2, x);
        CPPUNIT_ASSERT((p - x) == 3);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, b));
    }

    void test_shift_left_digits_02 () {
        const int a[] = {2, 3, 4, 5, 6};
        const int b[] = {2, 3, 4, 0, 0};
        int x[10];
        const int* p = shift_left_digits(a, a + 3, 2, x);
        CPPUNIT_ASSERT((p - x) == 5);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, b));
    }

    void test_shift_left_digits_03 () {
        const int a[] = {2, 3, 4};
        const int b[] = {2, 3, 4};
        int x[10];
        const int* p = shift_left_digits(a, a + 3, 0, x);
        CPPUNIT_ASSERT((p - x) == 3);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, b));
    }

    void test_shift_left_digits_04 () {
        const int a[] = {0, 2, 3, 4};
        const int b[] = {2, 3, 4, 0};
        int x[10];
        const int* p = shift_left_digits(a, a + 4, 1, x);
        CPPUNIT_ASSERT(((p - x) == 4) && "Remember to remove leading zeroes");
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, b));
    }

    // ------------------
    // shift_right_digits
    // ------------------

    void test_shift_right_digits () {
        const int a[] = {2, 3, 4};
        const int b[] = {2};
        int x[10];
        const int* p = shift_right_digits(a, a + 3, 2, x);
        CPPUNIT_ASSERT((p - x) == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, b));
    }

    // Puts 0 if you shift past the end
    void test_shift_right_digits_01 () {
        const int a[] = {2};
        const int b[] = {0};
        int x[10];
        const int* p = shift_right_digits(a, a + 1, 2, x);
        CPPUNIT_ASSERT((p - x) == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, b));
    }

    void test_shift_right_digits_02 () {
        const int a[] = {1};
        const int b[] = {0};
        int x[10];
        const int* p = shift_right_digits(a, a, 1, x);
        CPPUNIT_ASSERT((p - x) == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, b));
    }

    void test_shift_right_digits_03 () {
        const int a[] = {2, 3, 4};
        const int b[] = {2, 3, 4};
        int x[10];
        const int* p = shift_right_digits(a, a + 3, 0, x);
        CPPUNIT_ASSERT((p - x) == 3);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, b));
    }

    // -----------
    // plus_digits
    // -----------

    void test_plus_digits () {
        const int a[] = {2, 3, 4};
        const int b[] = {5, 6, 7};
        const int c[] = {8, 0, 1};
        int x[10];
        const int* p = plus_digits(a, a + 3, b, b + 3, x);
        CPPUNIT_ASSERT(p - x == 3);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_plus_digits_01 () {
        const int a[] = {1};
        const int b[] = {2};
        const int c[] = {3};
        int x[10];
        const int* p = plus_digits(a, a + 1, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_plus_digits_02 () {
        const int a[] = {0, 0, 0};
        const int b[] = {1, 2, 3};
        const int c[] = {2, 3};
        int x[10];
        const int* p = plus_digits(a, a + 3, b + 1, b + 3, x);
        CPPUNIT_ASSERT(p - x == 2);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_plus_digits_03 () {
        const int a[] = {0, 0, 0};
        const int b[] = {1, 2, 3};
        const int c[] = {3};
        int x[10];
        const int* p = plus_digits(a, a + 3, b + 2, b + 3, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    // Make sure to carry past the length of the shorter list
    void test_plus_digits_04 () {
        const int a[] = {1, 5};
        const int b[] = {7};
        const int c[] = {2, 2};
        int x[10];
        const int* p = plus_digits(a, a + 2, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 2);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    // Make sure to add digits when necessary
    void test_plus_digits_05 () {
        const int a[] = {9, 9};
        const int b[] = {1};
        const int c[] = {1, 0, 0};
        int x[10];
        const int* p = plus_digits(a, a + 2, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 3);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    // ------------
    // minus_digits
    // ------------

    void test_minus_digits () {
        const int a[] = {8, 0, 1};
        const int b[] = {5, 6, 7};
        const int c[] = {2, 3, 4};
        int x[10];
        const int* p = minus_digits(a, a + 3, b, b + 3, x);
        CPPUNIT_ASSERT(p - x == 3);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_minus_digits_01 () {
        const int a[] = {8};
        const int b[] = {5};
        const int c[] = {3};
        int x[10];
        const int* p = minus_digits(a, a + 1, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_minus_digits_02 () {
        const int a[] = {1, 0};
        const int b[] = {9};
        const int c[] = {1};
        int x[10];
        const int* p = minus_digits(a, a + 2, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_minus_digits_03 () {
        const int a[] = {1, 2};
        const int b[] = {1, 1};
        const int c[] = {1};
        int x[10];
        const int* p = minus_digits(a, a + 2, b, b + 2, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_minus_digits_04 () {
        const int a[] = {1};
        const int c[] = {0};
        int x[10];
        const int* p = minus_digits(a, a + 1, a, a + 1, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    // -----------------
    // multiplies_digits
    // -----------------

    void test_multiplies_digits () {
        const int a[] = {2, 3, 4};
        const int b[] = {5, 6, 7};
        const int c[] = {1, 3, 2, 6, 7, 8};
        int x[10];
        const int* p = multiplies_digits(a, a + 3, b, b + 3, x);
        CPPUNIT_ASSERT(p - x == 6);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_multiplies_digits_01 () {
        const int a[] = {2};
        const int b[] = {3};
        const int c[] = {6};
        int x[10];
        const int* p = multiplies_digits(a, a + 1, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_multiplies_digits_02 () {
        const int a[] = {1, 0};
        const int b[] = {2};
        const int c[] = {2, 0};
        int x[10];
        const int* p = multiplies_digits(a, a + 2, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 2);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_multiplies_digits_03 () {
        const int a[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        const int c[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        int x[20];
        const int* p = multiplies_digits(a, a + 9, a, a + 9, x);
        CPPUNIT_ASSERT(p - x == 17);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_multiplies_digits_04 () {
        const int a[] = {2, 3, 6};
        const int b[] = {4, 5};
        const int c[] = {1, 0, 6, 2, 0};
        int x[20];
        const int* p = multiplies_digits(a, a + 3, b, b + 2, x);
        CPPUNIT_ASSERT(p - x == 5);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_multiplies_digits_05 () {
        const int a[] = {9, 0, 7};
        const int b[] = {3, 0, 8};
        const int c[] = {2, 7, 9, 3, 5, 6};
        int x[20];
        const int* p = multiplies_digits(a, a + 3, b, b + 3, x);
        CPPUNIT_ASSERT(p - x == 6);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    // --------------
    // divides_digits
    // --------------

    void test_divides_digits () {
        const int a[] = {1, 3, 2, 6, 7, 8};
        const int b[] = {5, 6, 7};
        const int c[] = {2, 3, 4};
        int x[10];
        const int* p = divides_digits(a, a + 6, b, b + 3, x);
        CPPUNIT_ASSERT(p - x == 3);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_01 () {
        const int a[] = {4};
        const int b[] = {2};
        const int c[] = {2};
        int x[10];
        const int* p = divides_digits(a, a + 1, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_02 () {
        const int a[] = {2, 0};
        const int b[] = {1, 0};
        const int c[] = {2};
        int x[10];
        const int* p = divides_digits(a, a + 2, b, b + 2, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_03 () {
        const int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 8, 7, 6, 5, 4, 3, 2, 1};
        const int b[] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
        int x[10];
        const int* p = divides_digits(a, a + 17, b, b + 9, x);
        CPPUNIT_ASSERT(p - x == 9);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, b));
    }

    void test_divides_digits_04 () {
        const int a[] = {1, 0, 0};
        const int b[] = {1};
        const int c[] = {1, 0, 0};
        int x[10];
        const int* p = divides_digits(a, a + 3, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 3);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_05 () {
        const int a[] = {0};
        const int b[] = {1};
        const int c[] = {0};
        int x[10];
        const int* p = divides_digits(a, a + 1, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_06 () {
        const int a[] = {1, 0, 0};
        const int b[] = {2};
        const int c[] = {5, 0};
        int x[10];
        const int* p = divides_digits(a, a + 3, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 2);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_07 () {
        const int a[] = {1, 2, 1};
        const int b[] = {1, 1};
        const int c[] = {1, 1};
        int x[10];
        const int* p = divides_digits(a, a + 3, b, b + 2, x);
        CPPUNIT_ASSERT(p - x == 2);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_08 () {
        const int a[] = {1, 4, 4};
        const int b[] = {2};
        const int c[] = {7, 2};
        int x[10];
        const int* p = divides_digits(a, a + 3, b, b + 1, x);

        CPPUNIT_ASSERT(p - x == 2);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_09 () {
        const int a[] = {1, 4, 4};
        const int b[] = {1, 2};
        const int c[] = {1, 2};
        int x[10];
        const int* p = divides_digits(a, a + 3, b, b + 2, x);
        CPPUNIT_ASSERT(p - x == 2);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_10 () {
        const int a[] = {8};
        const int b[] = {1};
        const int c[] = {8};
        int x[10];
        const int* p = divides_digits(a, a + 1, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_11 () {
        const int a[] = {8};
        const int b[] = {8};
        const int c[] = {1};
        int x[10];
        const int* p = divides_digits(a, a + 1, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_12 () {
        const int a[] = {8};
        const int b[] = {8, 8};
        const int c[] = {0};
        int x[10];
        const int* p = divides_digits(a, a + 1, b, b + 2, x);
        CPPUNIT_ASSERT(p - x == 1);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_13 () {
        const int a[] = {1, 0, 0, 0};
        const int b[] = {1, 0};
        const int c[] = {1, 0, 0};
        int x[10];
        const int* p = divides_digits(a, a + 4, b, b + 2, x);
        CPPUNIT_ASSERT(p - x == 3);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    void test_divides_digits_14 () {
        const int a[] = {1, 0, 0};
        const int b[] = {3};
        const int c[] = {3, 3};
        int x[10];
        const int* p = divides_digits(a, a + 3, b, b + 1, x);
        CPPUNIT_ASSERT(p - x == 2);
        CPPUNIT_ASSERT(std::equal(const_cast<const int*>(x), p, c));
    }

    // -----------
    // constructor
    // -----------

    void test_constructor_1 () {
        try {
            const Integer<int> x("abc");
            CPPUNIT_ASSERT(false);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(strcmp(e.what(), "Integer()"));
        }
    }

    void test_constructor_2 () {
        try {
            const Integer<int> x("2");
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_constructor_3 () {
        try {
            const Integer<int> x(2);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    // ---
    // abs
    // ---

    void test_abs_1 () {
        try {
            Integer<int>  x = -98765;
            Integer<int>& y = x.abs();
            CPPUNIT_ASSERT( x == 98765);
            CPPUNIT_ASSERT(&x == &y);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_abs_2 () {
        try {
            const Integer<int> x = -98765;
            const Integer<int> y = abs(x);
            CPPUNIT_ASSERT(x == -98765);
            CPPUNIT_ASSERT(y ==  98765);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    // --------
    // equal_to
    // --------

    void test_equal_to () {
        try {
            const Integer<int> x = 98765;
            const Integer<int> y = 98765;
            CPPUNIT_ASSERT(      x == y);
            CPPUNIT_ASSERT(      x == 98765);
            CPPUNIT_ASSERT(  98765 == x);
            CPPUNIT_ASSERT(    !(x != y));
            CPPUNIT_ASSERT(    !(x != 98765));
            CPPUNIT_ASSERT(!(98765 != y));
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_equal_to_01 () {
        try {
            const Integer<int> x = 98765;
            const Integer<int> y = 98765;
            CPPUNIT_ASSERT(      x == y);
            CPPUNIT_ASSERT(      x == 98765);
            CPPUNIT_ASSERT(  98765 == x);
            CPPUNIT_ASSERT(    !(x != y));
            CPPUNIT_ASSERT(    !(x != 98765));
            CPPUNIT_ASSERT(!(98765 != y));
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_equal_to_02 () {
        try {
            const Integer<int> x = 98765;
            const Integer<int> y = 98765;
            CPPUNIT_ASSERT(      x == y);
            CPPUNIT_ASSERT(      x == 98765);
            CPPUNIT_ASSERT(  98765 == x);
            CPPUNIT_ASSERT(    !(x != y));
            CPPUNIT_ASSERT(    !(x != 98765));
            CPPUNIT_ASSERT(!(98765 != y));
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_equal_to_03 () {
        try {
            const Integer<int> x = 98765;
            const Integer<int> y = 98765;
            CPPUNIT_ASSERT(      x == y);
            CPPUNIT_ASSERT(      x == 98765);
            CPPUNIT_ASSERT(  98765 == x);
            CPPUNIT_ASSERT(    !(x != y));
            CPPUNIT_ASSERT(    !(x != 98765));
            CPPUNIT_ASSERT(!(98765 != y));
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    // ---------
    // less than
    // ---------

    void test_lt_01 () {
        try {
            const Integer<int> x = 1;
            const Integer<int> y = 2;
            CPPUNIT_ASSERT(x < y);
            CPPUNIT_ASSERT(!(y < x));
        } catch(std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    // Test positive/negative
    void test_lt_02 () {
        try {
            const Integer<int> x = 1;
            const Integer<int> y = -2;
            CPPUNIT_ASSERT(y < x);
            CPPUNIT_ASSERT(!(x < y));
        } catch(std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_lt_03 () {
        try {
            const Integer<int> x = -3;
            const Integer<int> y = 2;
            CPPUNIT_ASSERT(x < y);
            CPPUNIT_ASSERT(!(y < x));
        } catch(std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_lt_06 () {
        try {
            const Integer<int> x ("111111111");
            const Integer<int> y ("12345678987654321");
            CPPUNIT_ASSERT(x < y);
            CPPUNIT_ASSERT(!(y < x));
        } catch(std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_lt_07 () {
        try {
            const Integer<int> x ("111111111");
            const Integer<int> y ("-12345678987654321");
            CPPUNIT_ASSERT(!(x < y));
            CPPUNIT_ASSERT(y < x);
        } catch(std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    // --------
    // negation
    // --------

    void test_negation () {
        try {
            const Integer<int> x = 98765;
            const Integer<int> y = -x;
            CPPUNIT_ASSERT(x ==  98765);
            CPPUNIT_ASSERT(y == -98765);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_negation_01 () {
        try {
            const Integer<int> x = -98765;
            const Integer<int> y = -x;
            CPPUNIT_ASSERT(x == -98765);
            CPPUNIT_ASSERT(y ==  98765);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    // ------
    // output
    // ------

    void test_output () {
        try {
            const Integer<int> x = 98765;
            std::ostringstream out;
            out << x;
            CPPUNIT_ASSERT(out.str() == "98765");
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_output_1 () {
        try {
            const Integer<int> x = 111111111;
            std::ostringstream out;
            out << x;
            CPPUNIT_ASSERT(out.str() == "111111111");
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_output_2 () {
        try {
            const Integer<int> x ("12345678987654321");
            std::ostringstream out;
            out << x;
            CPPUNIT_ASSERT(out.str() == "12345678987654321");
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }


    // ---
    // pow
    // ---

    void test_pow_1 () {
    	std::cout << "\nAttempting test_pow1" << std::endl;
        try {
            Integer<int>       x = 98765;
            const int          e =  9867;
            Integer<int>&      y = x.pow(e);
            CPPUNIT_ASSERT( e == 9867);

            // x ~ 5.6.. * (10 ^ 49281)
            // Not putting that in here
            // CPPUNIT_ASSERT( x ==    0);
            CPPUNIT_ASSERT(&x ==   &y);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_pow_2 () {
    	std::cout << "\nAttempting test_pow2" << std::endl;
        try {
            const Integer<int> x = 98765;
            const int          e =  9867;
            const Integer<int> y = pow(x, e);
            CPPUNIT_ASSERT(x == 98765);
            CPPUNIT_ASSERT(e ==  9867);
            CPPUNIT_ASSERT(&x != &y);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_pow_3 () {
        try {
            const Integer<int> x =    1;
            const int          e = 9867;
            const Integer<int> y = pow(x, e);
            CPPUNIT_ASSERT(x ==    1);
            CPPUNIT_ASSERT(y ==    1);
            CPPUNIT_ASSERT(e == 9867);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_pow_4 () {
        try {
            const Integer<int> x = 98765;
            const int          e =  1;
            const Integer<int> y = pow(x, e);
            CPPUNIT_ASSERT(x == 98765);
            CPPUNIT_ASSERT(y == 98765);
            CPPUNIT_ASSERT(e ==     1);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_pow_5 () {
        try {
            const Integer<int> x = 2;
            const int          e = 8;
            const Integer<int> y = pow(x, e);
            CPPUNIT_ASSERT(x == 2);
            CPPUNIT_ASSERT(y == 256);
            CPPUNIT_ASSERT(e == 8);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    void test_pow_6 () {
        try {
            const Integer<int> x = 111111111;
            const int          e = 2;
            const Integer<int> y = pow(x, e);
            CPPUNIT_ASSERT(x == 111111111);
            CPPUNIT_ASSERT(y == Integer<int>("12345678987654321"));
            CPPUNIT_ASSERT(e == 2);
        }
        catch (std::invalid_argument& e) {
            CPPUNIT_ASSERT(false);
        }
    }

    // ---------
    // operators
    // ---------

    void test_plus_equal_01 () {
        Integer<int> x = 1;
        x += Integer<int>(1);
        CPPUNIT_ASSERT(x == Integer<int>(2));
    }

    void test_plus_equal_02 () {
        Integer<int> x = 1;
        x += 1;
        CPPUNIT_ASSERT(x == 2);
    }

    void test_plus_equal_03 () {
        Integer<int> x ("12345678987654321");
        x += 1;
        CPPUNIT_ASSERT(x == Integer<int>("12345678987654322"));
    }

    void test_plus_equal_04 () {
    	Integer<int> x ("-1");
    	x += 1;
        CPPUNIT_ASSERT(x == Integer<int>("0"));
    }

    void test_plus_equal_05 () {
    	Integer<int> x ("1");
    	x += -1;
        CPPUNIT_ASSERT(x == Integer<int>("0"));
    }

    void test_minus_equal_01 () {
        Integer<int> x = 1;
        x -= Integer<int> (1);
        CPPUNIT_ASSERT(x == Integer<int>(0));
    }

    void test_minus_equal_02 () {
        Integer<int> x = 1;
        x -= 1;
        CPPUNIT_ASSERT(x == 0);
    }

    void test_minus_equal_03 () {
        Integer<int> x = 1;
        x -= 2;
        CPPUNIT_ASSERT(x == -1);
    }

    void test_minus_equal_04 () {
        Integer<int> x = -1;
        x -= 2;
        CPPUNIT_ASSERT(x == -3);
    }

    void test_minus_equal_05 () {
        Integer<int> x = 1;
        x -= -2;
        CPPUNIT_ASSERT(x == 3);
    }

    void test_multiply_equal_01 () {
        Integer<int> x = 2;
        x *= 4;
        CPPUNIT_ASSERT(x == 8);
    }

    void test_multiply_equal_02 () {
        Integer<int> x = 2;
        x *= -1;
        CPPUNIT_ASSERT(x == -2);
    }

    void test_multiply_equal_03 () {
        Integer<int> x ("111111111");
        x *= x;
        CPPUNIT_ASSERT(x == Integer<int>("12345678987654321"));
    }

    void test_multiply_equal_04 () {
        Integer<int> x = -2;
        x *= 1;
        CPPUNIT_ASSERT(x == -2);
    }

    void test_multiply_equal_05 () {
        Integer<int> x = -2;
        x *= -1;
        CPPUNIT_ASSERT(x == 2);
    }

    void test_divide_equal_01 () {
        Integer<int> x = 10;
        x /= 5;
        CPPUNIT_ASSERT(x == 2);
    }

    void test_divide_equal_02 () {
        Integer<int> x = 10;
        x /= 4;
        CPPUNIT_ASSERT(x == 2);
    }

    void test_divide_equal_03 () {
        Integer<int> x ("12345678987654321");
        x /= Integer<int> ("111111111");
        CPPUNIT_ASSERT(x == Integer<int>("111111111"));
    }

    void test_divide_equal_04 () {
        Integer<int> x = 10;
        x /= -4;
        CPPUNIT_ASSERT(x == -2);
    }

    void test_divide_equal_05 () {
        Integer<int> x = -10;
        x /= 4;
        CPPUNIT_ASSERT(x == -2);
    }

    void test_divide_equal_06 () {
        Integer<int> x = -10;
        x /= -4;
        CPPUNIT_ASSERT(x == 2);
    }

    void test_mod_equal_01 () {
        Integer<int> x = 10;
        x %= 5;
        CPPUNIT_ASSERT(x == 0);
    }

    void test_mod_equal_02 () {
        Integer<int> x = 10;
        x %= 4;
        CPPUNIT_ASSERT(x == 2);
    }

    void test_mod_equal_03 () {
        Integer<int> x = 100;
        x %= 98;
        CPPUNIT_ASSERT(x == 2);
    }

    void test_shift_left_equal_01 () {
        Integer<int> x = 10;
        x <<= 1;
        CPPUNIT_ASSERT(x == 100);
    }

    void test_shift_left_equal_02 () {
        Integer<int> x = 12345;
        x <<= 2;
        CPPUNIT_ASSERT(x == 1234500);
    }

    void test_shift_left_equal_03 () {
        Integer<int> x = 0;
        x <<= 2;
        CPPUNIT_ASSERT(x == 0);
    }

    void test_shift_right_equal_01 () {
        Integer<int> x = 10;
        x >>= 1;
        CPPUNIT_ASSERT(x == 1);
    }

    void test_shift_right_equal_02 () {
        Integer<int> x = 10;
        x >>= 2;
        CPPUNIT_ASSERT(x == 0);
    }
    
    void test_shift_right_equal_03 () {
        Integer<int> x = 12345;
        x >>= 2;
        CPPUNIT_ASSERT(x == 123);
    }
    
    // -----
    // suite
    // -----

    CPPUNIT_TEST_SUITE(TestInteger);

    CPPUNIT_TEST(test_strip_zeroes_01);
    CPPUNIT_TEST(test_strip_zeroes_02);
    CPPUNIT_TEST(test_strip_zeroes_03);

    CPPUNIT_TEST(test_print_list_01);
    CPPUNIT_TEST(test_print_list_02);
    CPPUNIT_TEST(test_print_list_03);
    CPPUNIT_TEST(test_print_list_04);

    CPPUNIT_TEST(test_compare_01);
    CPPUNIT_TEST(test_compare_02);
    CPPUNIT_TEST(test_compare_03);
    CPPUNIT_TEST(test_compare_04);
    CPPUNIT_TEST(test_compare_05);

    CPPUNIT_TEST(test_shift_left_digits);
    CPPUNIT_TEST(test_shift_left_digits_01);
    CPPUNIT_TEST(test_shift_left_digits_02);
    CPPUNIT_TEST(test_shift_left_digits_03);
    CPPUNIT_TEST(test_shift_right_digits);
    CPPUNIT_TEST(test_shift_right_digits_01);
    CPPUNIT_TEST(test_shift_right_digits_02);
    CPPUNIT_TEST(test_shift_right_digits_03);
    CPPUNIT_TEST(test_plus_digits);
    CPPUNIT_TEST(test_plus_digits_01);
    CPPUNIT_TEST(test_plus_digits_02);
    CPPUNIT_TEST(test_plus_digits_03);
    CPPUNIT_TEST(test_plus_digits_04);
    CPPUNIT_TEST(test_plus_digits_05);
    CPPUNIT_TEST(test_minus_digits);
    CPPUNIT_TEST(test_minus_digits_01);
    CPPUNIT_TEST(test_minus_digits_02);
    CPPUNIT_TEST(test_minus_digits_03);
    CPPUNIT_TEST(test_minus_digits_04);
    CPPUNIT_TEST(test_multiplies_digits);
    CPPUNIT_TEST(test_multiplies_digits_01);
    CPPUNIT_TEST(test_multiplies_digits_02);
    CPPUNIT_TEST(test_multiplies_digits_03);
    CPPUNIT_TEST(test_multiplies_digits_04);
    CPPUNIT_TEST(test_multiplies_digits_05);
    CPPUNIT_TEST(test_divides_digits);
    CPPUNIT_TEST(test_divides_digits_01);
    CPPUNIT_TEST(test_divides_digits_02);
    CPPUNIT_TEST(test_divides_digits_03);
    CPPUNIT_TEST(test_divides_digits_04);
    CPPUNIT_TEST(test_divides_digits_05);
    CPPUNIT_TEST(test_divides_digits_06);
    CPPUNIT_TEST(test_divides_digits_07);
    CPPUNIT_TEST(test_divides_digits_08);
    CPPUNIT_TEST(test_divides_digits_09);
    CPPUNIT_TEST(test_divides_digits_10);
    CPPUNIT_TEST(test_divides_digits_11);
    CPPUNIT_TEST(test_divides_digits_12);
    CPPUNIT_TEST(test_divides_digits_13);
    CPPUNIT_TEST(test_divides_digits_14);
    CPPUNIT_TEST(test_constructor_1);
    CPPUNIT_TEST(test_constructor_2);
    CPPUNIT_TEST(test_constructor_3);
    CPPUNIT_TEST(test_abs_1);
    CPPUNIT_TEST(test_abs_2);
    CPPUNIT_TEST(test_lt_01);
    CPPUNIT_TEST(test_lt_02);
    CPPUNIT_TEST(test_lt_03);
    CPPUNIT_TEST(test_lt_06);
    CPPUNIT_TEST(test_lt_07);
    CPPUNIT_TEST(test_negation);
    CPPUNIT_TEST(test_negation_01);
    CPPUNIT_TEST(test_output);
    CPPUNIT_TEST(test_output_1);
    CPPUNIT_TEST(test_output_2);

    // They take a while, but pass
   // CPPUNIT_TEST(test_pow_1);
   // CPPUNIT_TEST(test_pow_2);

    CPPUNIT_TEST(test_pow_3);
    CPPUNIT_TEST(test_pow_4);
    CPPUNIT_TEST(test_pow_5);
    CPPUNIT_TEST(test_pow_6);
    CPPUNIT_TEST(test_plus_equal_01);
    CPPUNIT_TEST(test_plus_equal_02);
    CPPUNIT_TEST(test_plus_equal_03);
    CPPUNIT_TEST(test_plus_equal_04);
    CPPUNIT_TEST(test_plus_equal_05);
    CPPUNIT_TEST(test_minus_equal_01);
    CPPUNIT_TEST(test_minus_equal_02);
    CPPUNIT_TEST(test_minus_equal_03);
    CPPUNIT_TEST(test_minus_equal_04);
    CPPUNIT_TEST(test_minus_equal_05);
    CPPUNIT_TEST(test_multiply_equal_01);
    CPPUNIT_TEST(test_multiply_equal_02);
    CPPUNIT_TEST(test_multiply_equal_03);
    CPPUNIT_TEST(test_multiply_equal_04);
    CPPUNIT_TEST(test_multiply_equal_05);
    CPPUNIT_TEST(test_divide_equal_01);
    CPPUNIT_TEST(test_divide_equal_02);
    CPPUNIT_TEST(test_divide_equal_03);
    CPPUNIT_TEST(test_divide_equal_04);
    CPPUNIT_TEST(test_divide_equal_05);
    CPPUNIT_TEST(test_mod_equal_01);
    CPPUNIT_TEST(test_mod_equal_02);
    CPPUNIT_TEST(test_mod_equal_03);
    CPPUNIT_TEST(test_shift_left_equal_01);
    CPPUNIT_TEST(test_shift_left_equal_02);
    CPPUNIT_TEST(test_shift_left_equal_03);
    CPPUNIT_TEST(test_shift_right_equal_01);
    CPPUNIT_TEST(test_shift_right_equal_02);
    CPPUNIT_TEST(test_shift_right_equal_03);
    CPPUNIT_TEST_SUITE_END();
};

// ----
// main
// ----

int main () {
    using namespace std;
    cout << "TestInteger.c++" << endl;

    CppUnit::TextTestRunner tr;
    tr.addTest(TestInteger::suite());
    tr.run();

    cout << "Done." << endl;
    return 0;
}
