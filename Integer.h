// --------------------------
// projects/integer/Integer.h
// Copyright (C) 2013
// Glenn P. Downing
// --------------------------

#ifndef Integer_h
#define Integer_h

// --------
// includes
// --------

#include <cassert>   // assert
#include <iostream>  // ostream
#include <stdexcept> // invalid_argument
#include <string>    // string
#include <iterator>  // iterator tags
#include <algorithm> // reverse
#include <math.h>    // pow
// Comparing the performance of these structures
#include <deque>     // deque
#include <list>      // list
#include <vector>    // vector
// Typedefs
typedef std::bidirectional_iterator_tag biTag;
typedef std::input_iterator_tag inTag;
typedef std::random_access_iterator_tag ranTag;

// Helper functions
/** 
 * @param b  an iterator to the beginning of an input  sequence (inclusive)
 * @param e  an iterator to the end       of an input  sequence (exclusive)
 * @return The first non-zero element of the list or the last element if al were zero
 */
template<typename II>
II strip_zeroes(II b, II e) {
	while(b != e && *b == 0)
		++b;
	return b;
}

/**
 * @param b  an iterator to the beginning of an input  sequence (inclusive)
 * @param e  an iterator to the end       of an input  sequence (exclusive)
 * @return The point past the end of the output
 */
template<typename II, typename OI>
OI print_list(II b, II e, OI x) {
	b = strip_zeroes(b, e);
	if (b == e) {
		*x++ = 0;
		return x;
	}

	while (b != e)
		*x++ = *b++;
	return x;
}

/**
 * @param b  an iterator to the beginning of an input  sequence (inclusive)
 * @param e  an iterator to the end       of an input  sequence (exclusive)
 * @param b2 an iterator to the beginning of an input  sequence (inclusive)
 * @param e2 an iterator to the end       of an input  sequence (exclusive)
 * @return 1 if the first number is greater than the original
 * 		   0 if the two numbers are equal
 * 		   -1 if the other number is greater than this one
 */
template<typename II1, typename II2>
int compare(II1 b1, II1 e1, II2 b2, II2 e2) {
	typedef typename std::iterator_traits<II1>::difference_type dType1;
	typedef typename std::iterator_traits<II2>::difference_type dType2;

	b1 = strip_zeroes(b1, e1);
	b2 = strip_zeroes(b2, e2);

	dType1 len1 = std::distance(b1, e1);
	dType2 len2 = std::distance(b2, e2);

	if (len1 > len2)
		return 1;
	if (len1 < len2)
		return -1;

	assert(len1 == len2);

	while (b1 != e1) {
		if (*b1 > *b2)
			return 1;
		if (*b1 < *b2)
			return -1;
		++b1;
		++b2;
	}
	return 0;
}

// Main functions
/**
 * @param b an iterator to the beginning of an input  sequence (inclusive)
 * @param e an iterator to the end       of an input  sequence (exclusive)
 * @param x an iterator to the beginning of an output sequence (inclusive)
 * @return  an iterator to the end       of an output sequence (exclusive)
 * the sequences are of decimal digits
 * output the shift left of the input sequence into the output sequence
 * ([b, e) << n) => x
 */
template<typename II, typename OI>
OI shift_left_digits(II b, II e, int n, OI x) {
	x = print_list(b, e, x);
	while (n-- > 0)
		*x++ = 0;
	return x;
}

// ------------------
// shift_right_digits
// ------------------

// Random iterator
template<typename RI, typename OI>
OI shift_right_digits(RI b, RI e, int n, OI x, ranTag) {
	// Move e over, but not past b
	e = e - b < n ? b : e - n;
	return print_list(b, e, x);
}

// Bidirectional iterator
template<typename BI, typename OI>
OI shift_right_digits(BI b, BI e, int n, OI x, biTag) {
	while (b != e && n-- > 0)
		--e;
	return print_list(b, e, x);
}

// Input iterator
template<typename II, typename OI>
OI shift_right_digits(II b, II e, int n, OI x, inTag) {
	typedef std::vector<typename std::iterator_traits<II>::value_type> result_list;
	typedef typename result_list::iterator result_list_iterator;

	result_list result;
	while (b != e)
		result.push_back(*b++);
	return print_list(result.begin(), result.end(), x);
}

/**
 * @param b an iterator to the beginning of an input  sequence (inclusive)
 * @param e an iterator to the end       of an input  sequence (exclusive)
 * @param x an iterator to the beginning of an output sequence (inclusive)
 * @return  an iterator to the end       of an output sequence (exclusive)
 * the sequences are of decimal digits
 * output the shift right of the input sequence into the output sequence
 * ([b, e) >> n) => x
 *
 * If shifted past the range [b, e], a 0 will be placed
 */
template<typename I, typename OI>
OI shift_right_digits(I b, I e, int n, OI x) {
	typedef typename std::iterator_traits<I>::iterator_category iCat;
	return shift_right_digits(b, e, n, x, iCat());
}

// plus_digits
// bidirection iterators
// O(n)
template<typename BI1, typename BI2, typename OI>
OI plus_digits(BI1 b1, BI1 e1, BI2 b2, BI2 e2, OI x, biTag, biTag) {
	typedef std::deque<typename std::iterator_traits<OI>::value_type> result_list;
	typedef typename result_list::iterator result_list_iterator;
	typedef typename std::iterator_traits<BI1>::difference_type dType1;
	typedef typename std::iterator_traits<BI2>::difference_type dType2;

	result_list result;

	int carry = 0;
	while (b1 != e1 && b2 != e2) {
		--e1;
		--e2;
		carry += *e1 + *e2;
		result.push_front(carry % 10);
		carry /= 10;
	}

	while (b1 != e1) {
		--e1;
		carry += *e1;
		result.push_front(carry % 10);
		carry /= 10;
	}

	while (b2 != e2) {
		--e2;
		carry += *e2;
		result.push_front(carry % 10);
		carry /= 10;
	}
	result.push_front(carry);
	return print_list(result.begin(), result.end(), x);
}

// input iterators
// O(n^2)
template<typename II1, typename II2, typename OI>
OI plus_digits(II1 b1, II1 e1, II2 b2, II2 e2, OI x, inTag, inTag) {
	typedef std::deque<typename std::iterator_traits<OI>::value_type> result_list;
	typedef typename result_list::iterator result_list_iterator;
	typedef typename std::iterator_traits<II1>::difference_type dType1;
	typedef typename std::iterator_traits<II2>::difference_type dType2;

	result_list result;

	// It's necessary to know the length of each list to align
	// the digits correctly
	dType1 len1 = std::distance(b1, e1);
	dType2 len2 = std::distance(b2, e2);

	// Handle leading values
	while (len1 > len2) {
		result.push_back(*b1++);
		--len1;
	}
	while (len2 > len1) {
		result.push_back(*b2++);
		--len2;
	}

	while (b1 != e1) {
		result.push_back(*b1++ + *b2++);

		// Carry the 1 backwards through the array
		result_list_iterator currentDigit = result.end() - 1;
		while (*currentDigit >= 10) {
			*currentDigit -= 10;
			if (currentDigit == result.begin()) {
				result.push_front(1);
				break;
			}
			else {
				--currentDigit;
				*currentDigit += 1;
			}
		}
	}

	return print_list(result.begin(), result.end(), x);
}

/**
 * @param b  an iterator to the beginning of an input  sequence (inclusive)
 * @param e  an iterator to the end       of an input  sequence (exclusive)
 * @param b2 an iterator to the beginning of an input  sequence (inclusive)
 * @param e2 an iterator to the end       of an input  sequence (exclusive)
 * @param x  an iterator to the beginning of an output sequence (inclusive)
 * @return   an iterator to the end       of an output sequence (exclusive)
 * the sequences are of decimal digits
 * output the sum of the two input sequences into the output sequence
 * ([b1, e1) + [b2, e2)) => x
 */
template<typename I1, typename I2, typename OI>
OI plus_digits(I1 b1, I1 e1, I2 b2, I2 e2, OI x) {
	typedef typename std::iterator_traits<I1>::iterator_category iCat1;
	typedef typename std::iterator_traits<I2>::iterator_category iCat2;

	b1 = strip_zeroes(b1, e1);
	b2 = strip_zeroes(b2, e2);

	// Adding zero
	if (b1 == e1)
		return print_list(b2, e2, x);
	if (b2 == e2)
		return print_list(b1, e1, x);

	return plus_digits(b1, e1, b2, e2, x, iCat1(), iCat2());
}

// minus_digits
// bidirectional iterator
// O(n)
template<typename BI1, typename BI2, typename OI>
OI minus_digits(BI1 b1, BI1 e1, BI2 b2, BI2 e2, OI x, biTag, biTag) {
	typedef std::deque<typename std::iterator_traits<OI>::value_type> result_list;
	typedef typename result_list::iterator result_list_iterator;
	typedef typename std::iterator_traits<BI1>::difference_type dType1;
	typedef typename std::iterator_traits<BI2>::difference_type dType2;

	result_list result;

	bool borrow = false;
	int difference;
	while (b2 != e2) {
		--e1;
		--e2;
		difference = *e1 - *e2;
		if (borrow)
			--difference;
		borrow = difference < 0;
		if (borrow)
			difference += 10;
		result.push_front(difference);
	}

	while (b1 != e1) {
		--e1;
		difference = *e1;
		if (borrow)
			--difference;
		borrow = difference < 0;
		if (borrow)
			difference += 10;
		result.push_front(difference);
	}

	return print_list(result.begin(), result.end(), x);
}

// input iterators
// O(n^2)
template<typename II1, typename II2, typename OI>
OI minus_digits(II1 b1, II1 e1, II2 b2, II2 e2, OI x, inTag, inTag) {
	typedef std::deque<typename std::iterator_traits<OI>::value_type> result_list;
	typedef typename result_list::iterator result_list_iterator;
	typedef typename std::iterator_traits<II1>::difference_type dType1;
	typedef typename std::iterator_traits<II2>::difference_type dType2;

	result_list result;

	// It's necessary to know the length of each list to align
	// the digits correctly
	dType1 len1 = std::distance(b1, e1);
	dType2 len2 = std::distance(b2, e2);

	// Handle leading values
	while (len1 > len2) {
		result.push_back(*b1++);
		--len1;
	}

	while (b1 != e1) {
		result.push_back(*b1++ - *b2++);

		// Carry the 1 backwards through the array
		result_list_iterator currentDigit = result.end() - 1;
		while (*currentDigit < 0) {
			*currentDigit += 10;
			--currentDigit;
			*currentDigit -= 1;
		}
	}

	return print_list(result.begin(), result.end(), x);
}

/**
 * @param b  an iterator to the beginning of an input  sequence (inclusive)
 * @param e  an iterator to the end       of an input  sequence (exclusive)
 * @param b2 an iterator to the beginning of an input  sequence (inclusive)
 * @param e2 an iterator to the end       of an input  sequence (exclusive)
 * @param x  an iterator to the beginning of an output sequence (inclusive)
 * @return   an iterator to the end       of an output sequence (exclusive)
 * the sequences are of decimal digits
 * output the difference of the two input sequences into the output sequence
 * ([b1, e1) - [b2, e2)) => x
 */
template<typename I1, typename I2, typename OI>
OI minus_digits(I1 b1, I1 e1, I2 b2, I2 e2, OI x) {
	typedef typename std::iterator_traits<I1>::iterator_category iCat1;
	typedef typename std::iterator_traits<I2>::iterator_category iCat2;

	b1 = strip_zeroes(b1, e1);
	b2 = strip_zeroes(b2, e2);
	int cmp = compare(b1, e1, b2, e2);
	assert(( cmp >= 0) && "minus_digits requires the first number to be equal or larger than the second");

	// Number minus itself
	if (cmp == 0) {
		*x++ = 0;
		return x;
	}
	// Number minus zero
	if (b2 == e2) {
		return print_list(b1, e1, x);
	}

	return minus_digits(b1, e1, b2, e2, x, iCat1(), iCat2());
}

// Multiples
// bidirectional iterators
// O(n^2)
template<typename BI1, typename BI2, typename OI>
OI multiplies_digits(BI1 b1, BI1 e1, BI2 b2, BI2 e2, OI x, biTag, biTag) {
	typedef std::vector<typename std::iterator_traits<OI>::value_type> result_list;
	typedef typename result_list::iterator result_list_iterator;
	typedef typename std::iterator_traits<BI1>::difference_type dType1;
	typedef typename std::iterator_traits<BI2>::difference_type dType2;

	dType1 len1 = std::distance(b1, e1);
	dType2 len2 = std::distance(b2, e2);

	result_list result(len1 + len2, 0);
	result_list_iterator startIndex = result.end() - 1;
	const BI2 end_e2 = e2;
	int carry;

	while (b1 != e1) {
		--e1;
		result_list_iterator index = startIndex;
		--startIndex;
		e2 = end_e2;
		carry = 0;

		while (b2 != e2) {
			--e2;
			carry += *index + (*e1 * *e2);
			*index = carry % 10;
			carry /= 10;
			--index;
		}
		*index = carry;
	}

	return print_list(result.begin(), result.end(), x);
}

// input iterators
// O(n^3)
template<typename II1, typename II2, typename OI>
OI multiplies_digits(II1 b1, II1 e1, II2 b2, II2 e2, OI x, inTag, inTag) {
	typedef std::vector<typename std::iterator_traits<OI>::value_type> result_list;
	typedef typename result_list::iterator result_list_iterator;
	typedef typename std::iterator_traits<II1>::difference_type dType1;
	typedef typename std::iterator_traits<II2>::difference_type dType2;

	dType1 len1 = std::distance(b1, e1);
	dType2 len2 = std::distance(b2, e2);

	// For any two integers of lengths n, m digits,
	// the product will never be more than n + m digits
	result_list result(len1 + len2, 0);

	// Start at the second most significant digit
	// The most significant digit is
	result_list_iterator startIndex = result.begin() + 1;
	const II2 start_b2 = b2;
	int carry;

	while (b1 != e1) {
		result_list_iterator index = startIndex;
		b2 = start_b2;
		while (b2 != e2) {
			// Add the current product to this digit
			*index += *b1 * *b2++;

			// Carry back the excess values
			carry = 0;
			result_list_iterator backTrack = index++;
			while (*backTrack >= 10) {
				assert(backTrack != result.begin());
				carry = *backTrack / 10;
				*backTrack-- %= 10;
				*backTrack += carry;
			}
		}
		++startIndex;
		++b1;
	}

	return print_list(result.begin(), result.end(), x);
}

/**
 * @param b  an iterator to the beginning of an input  sequence (inclusive)
 * @param e  an iterator to the end       of an input  sequence (exclusive)
 * @param b2 an iterator to the beginning of an input  sequence (inclusive)
 * @param e2 an iterator to the end       of an input  sequence (exclusive)
 * @param x  an iterator to the beginning of an output sequence (inclusive)
 * @return   an iterator to the end       of an output sequence (exclusive)
 * the sequences are of decimal digits
 * output the product of the two input sequences into the output sequence
 * ([b1, e1) * [b2, e2)) => x
 */
template<typename I1, typename I2, typename OI>
OI multiplies_digits(I1 b1, I1 e1, I2 b2, I2 e2, OI x) {
	typedef typename std::iterator_traits<OI>::value_type result_type;
	typedef typename std::iterator_traits<I1>::iterator_category iCat1;
	typedef typename std::iterator_traits<I2>::iterator_category iCat2;

	b1 = strip_zeroes(b1, e1);
	b2 = strip_zeroes(b2, e2);
	// Multiply by 0
	if (b1 == e1 || b2 == e2) {
		*x++ = 0;
		return x;
	}

	// Multiply by
	const result_type ONE[] = {1};
	if ((compare(b1, e1, ONE, ONE + 1) == 0))
		return print_list(b2, e2, x);
	if ((compare(b2, e2, ONE, ONE + 1) == 0))
		return print_list(b1, e1, x);

	return multiplies_digits(b1, e1, b2, e2, x, iCat1(), iCat2());
}

// Divides
// Only need input iterators
/**
 * @param b  an iterator to the beginning of an input  sequence (inclusive)
 * @param e  an iterator to the end       of an input  sequence (exclusive)
 * @param b2 an iterator to the beginning of an input  sequence (inclusive)
 * @param e2 an iterator to the end       of an input  sequence (exclusive)
 * @param x  an iterator to the beginning of an output sequence (inclusive)
 * @return   an iterator to the end       of an output sequence (exclusive)
 * the sequences are of decimal digits
 * output the division of the two input sequences into the output sequence
 * ([b1, e1) / [b2, e2)) => x
 */
template<typename II1, typename II2, typename OI>
OI divides_digits(II1 b1, II1 e1, II2 b2, II2 e2, OI x) {
	typedef typename std::iterator_traits<OI>::value_type result_type;
	typedef std::deque<result_type> result_list;
	typedef typename result_list::iterator result_list_iterator;
	typedef typename std::iterator_traits<II1>::difference_type dType1;
	typedef typename std::iterator_traits<II2>::difference_type dType2;
	typedef typename std::iterator_traits<result_list_iterator>::difference_type result_diff;

	b1 = strip_zeroes(b1, e1);
	b2 = strip_zeroes(b2, e2);

	// zero / anything is zero
	if (b1 == e1) {
		*x++ = 0;
		return x;
	}
	assert((b2 != e2) && "can't divide by zero");

	int cmp = compare(b1, e1, b2, e2);
	// Quotient is smaller than dividend
	if (cmp < 0) {
		*x = 0;
		return ++x;
	}
	// Quotient is equal to dividend
	else if (cmp == 0) {
		*x = 1;
		return ++x;
	}
	const result_type ONE[] = {1};
	// Dividend is one
	if (compare(b2, e2, ONE, ONE + 1) == 0){
		while (b1 != e1)
			*x++ = *b1++;
		return x;
	}

	// General case
	dType1 qlen = std::distance(b1, e1);
	dType2 dlen = std::distance(b2, e2);
	result_list result(1, 0);
	result_list addSpace(qlen + 1, 0);
	result_list quotientSpace(qlen, 0);
	result_list_iterator addEnd;

	// Copy the dividend so we don't overwrite it
	std::copy(b1, e1, quotientSpace.begin());

	// Keep track of the current section of the quotient we're working with
	result_list_iterator quotientStart = quotientSpace.begin();
	result_list_iterator quotientEnd = quotientSpace.begin() + dlen - 1;

	// This lines up the dividend with the front of the quotient and performs
	// good old fashioned long division
	int counter;
	while (quotientEnd != quotientSpace.end()) {
		++quotientEnd;
		counter = 0;
		while (compare(quotientStart, quotientEnd, b2, e2) >= 0) {
			++counter;
			// Subtract dividend out of quotient
			addEnd = minus_digits(quotientStart, quotientEnd, b2, e2, addSpace.begin());

			// Store the result back into quotient
			// shifting over when necessary
			result_list_iterator placeHolder = quotientEnd;
			while(addEnd != addSpace.begin()) {
				--addEnd;
				--placeHolder;
				*placeHolder = *addEnd;
			}
			quotientStart = strip_zeroes(placeHolder, quotientEnd);
		}
		result.push_back(counter);
	}

	return print_list(result.begin(), result.end(), x);
}

template<typename T, typename C = std::vector<T> >
class Integer {
		/**
		 * Returns content equality
		 */
		friend bool operator ==(const Integer& lhs, const Integer& rhs) {
			return lhs.positive == rhs.positive && lhs.data.size() == rhs.data.size()
			       && std::equal(lhs.data.begin(), lhs.data.end(), rhs.data.begin());
		}

		/**
		 * Returns the negation of content equality
		 */
		friend bool operator !=(const Integer& lhs, const Integer& rhs) {
			return !(lhs == rhs);
		}

		/**
		 * Returns true if the value of lhs is less than rhs
		 * False otherwise
		 */
		friend bool operator <(const Integer& lhs, const Integer& rhs) {
			// Easy +/- testing
			if (lhs.positive != rhs.positive)
				return !lhs.positive;

			int cmp = compare(lhs.data.begin(), lhs.data.end(), rhs.data.begin(), rhs.data.end());
			if (lhs.positive) {
				assert(rhs.positive);
				return cmp < 0;
			}
			else {
				return cmp > 0;
			}
		}

		/**
		 * Returns true if the value of lhs is less than or equal to rhs
		 * false otherwise
		 */
		friend bool operator <=(const Integer& lhs, const Integer& rhs) {
			return !(rhs < lhs);
		}

		/**
		 * Returns true if the value of lhs is greater than rhs
		 * false otherwise
		 */
		friend bool operator >(const Integer& lhs, const Integer& rhs) {
			return (rhs < lhs);
		}

		/**
		 * Returns true if lhs is greater than or equal to rhs
		 */
		friend bool operator >=(const Integer& lhs, const Integer& rhs) {
			return !(lhs < rhs);
		}

		/**
		 * Returns a new Integer with the sum of lhs and rhs
		 */
		friend Integer operator +(Integer lhs, const Integer& rhs) {
			return lhs += rhs;
		}

		/**
		 * Returns a new Integer wth teh value of rhs subtracted from rhs
		 */
		friend Integer operator -(Integer lhs, const Integer& rhs) {
			return lhs -= rhs;
		}

		/**
		 * Returns a new Integer with the product of lhs and rhs
		 */
		friend Integer operator *(Integer lhs, const Integer& rhs) {
			return lhs *= rhs;
		}

		/**
		 * Returns a new Integer with the value of lhs divided by rhs
		 *
		 * @throws invalid_argument if (rhs == 0)
		 */
		friend Integer operator /(Integer lhs, const Integer& rhs) {
			return lhs /= rhs;
		}

		/**
		 * Returns the remainder of lhs divided by rhs
		 *
		 * @throws invalid_argument if (rhs <= 0)
		 */
		friend Integer operator %(Integer lhs, const Integer& rhs) {
			return lhs %= rhs;
		}

		/**
		 * Returns a new Integer with the value of lhs shifted rhs bits left
		 *
		 * @throws invalid_argument if (rhs < 0)
		 */
		friend Integer operator <<(Integer lhs, int rhs) {
			return lhs <<= rhs;
		}

		/**
		 * Returns a new Integer with the value of lhs shifted rhs bits right
		 *
		 * @throws invalid_argument if (rhs < 0)
		 */
		friend Integer operator >>(Integer lhs, int rhs) {
			return lhs >>= rhs;
		}

		/**
		 * Prints this Integer to lhs
		 */
		friend std::ostream& operator <<(std::ostream& lhs, const Integer& rhs) {
			if (!rhs.positive)
				lhs << "-";
			for (C_const_iter i = rhs.data.begin(); i != rhs.data.end(); ++i)
				lhs << *i;
			return lhs;
		}

		/**
		 * Returns a new Integer with the absolute value of x
		 */
		friend Integer abs(Integer x) {
			return x.abs();
		}

		/**
		 * Returns a new Integer with the value of x raised to the e power
		 *
		 * @throws invalid_argument if (x == 0) && (e == 0)
		 * @throws invalid_argument if (e < 0)
		 */
		friend Integer pow(Integer x, int e) {
			return x.pow(e);
		}

	private:
		// Typedefs
		typedef typename C::iterator C_iter;
		typedef typename C::const_iterator C_const_iter;
		typedef typename std::iterator_traits<C_iter>::difference_type C_diff_t;
		typedef T* pointer;

		// data
		C data;
		bool positive;

	private:

		bool valid() const {
			bool result = true;
			result &= (data.size() > 0);
			for (C_const_iter i = data.begin(); i != data.end(); ++i)
				result &= (*i >= 0 && *i <= 9);
			return result;
		}

	public:

		/**
		 * Constructs a new Integer using a built-in int
		 */
		Integer(int value) :
				positive(value >= 0) {
			value = std::abs(value);
			if (value == 0)
				data.push_back(0);
			else {
				while (value > 0) {
					data.push_back(value % 10);
					value /= 10;
				}
				std::reverse(data.begin(), data.end());
			}
			assert(valid());
		}

		/**
		 * Constructs a new Integer object using a valid string
		 *
		 * @throws invalid_argument if value is not a valid representation of an Integer
		 */
		explicit Integer(const std::string& value) :
				positive(value[0] != '-') {
			int startIndex;
			if (positive)
				startIndex = 0;
			else
				startIndex = 1;

			for (std::string::const_iterator i = value.begin() + startIndex; i != value.end(); ++i)
				data.push_back(*i - '0');

			if (!valid())
				throw std::invalid_argument("Integer::Integer()");
		}

		/**
		 * Returns a new Integer with the negated value of this Integer
		 */
		Integer operator -() const {
			Integer result(*this);
			result.positive = !positive;
			assert(result.valid());
			return result;
		}

		/**
		 * Increments this Integer and returns the new value
		 */
		Integer& operator ++() {
			*this += 1;
			return *this;
		}

		/**
		 * Increments this Integer and returns the original value
		 */
		Integer operator ++(int) {
			Integer x = *this;
			++(*this);
			return x;
		}

		/**
		 * Decrements the value in this Integer and returns the new value
		 */
		Integer& operator --() {
			*this -= 1;
			return *this;
		}

		/**
		 * Decrements the value in this Integer and returns the original value
		 */
		Integer operator --(int) {
			Integer x = *this;
			--(*this);
			return x;
		}

		/**
		 * Adds the value of rhs to this Integer, taking care of negative values as well
		 *
		 * @param rhs an Integer object
		 */
		Integer& operator +=(const Integer& rhs) {
			// Adding 0
			if (rhs == 0)
				return *this;
			if (*this == 0)
				return *this = rhs;

			C_diff_t bufferSize = std::max(data.size(), rhs.data.size()) + 1;
			C buffer(bufferSize);
			C_iter endOfNumber;
			if (positive == rhs.positive) {
				endOfNumber = plus_digits(data.begin(), data.end(), rhs.data.begin(), rhs.data.end(), buffer.begin());
			}
			else {
				int cmp = compare(data.begin(), data.end(), rhs.data.begin(), rhs.data.end());
				if (positive) {
					assert(!rhs.positive);
					if (cmp < 0) {
						endOfNumber = minus_digits(rhs.data.begin(), rhs.data.end(), data.begin(), data.end(), buffer.begin());
						positive = false;
					}
					else {
						assert(cmp >= 0);
						endOfNumber = minus_digits(data.begin(), data.end(), rhs.data.begin(), rhs.data.end(), buffer.begin());
					}
				}
				else {
					assert(!positive);
					if (cmp > 0) {
						endOfNumber = minus_digits(data.begin(), data.end(), rhs.data.begin(), rhs.data.end(), buffer.begin());
					}
					else {
						assert(cmp <= 0);
						endOfNumber = minus_digits(rhs.data.begin(), rhs.data.end(), data.begin(), data.end(), buffer.begin());
						positive = true;
					}
				}
			}

			data.assign(buffer.begin(), endOfNumber);
			assert(valid());
			return *this;
		}

		/**
		 * Subtracts the value in rhs from this Integer
		 *
		 * @param rhs an Integer object
		 */
		Integer& operator -=(const Integer& rhs) {
			return *this += -rhs;
		}

		/**
		 * Multiplies the value of this Integer object by the value in rhs
		 *
		 * @param rhs an Integer object
		 */
		Integer& operator *=(const Integer& rhs) {
			// Multiply by 0
			if (*this == 0)
				return *this;
			if (rhs == 0) {
				*this = 0;
				return *this;
			}
			// Multiply by 1
			if (rhs == 1)
				return *this;
			if (*this == 1) {
				*this = rhs;
				return *this;
			}

			// Multiply by -1
			if (*this == -1) {
				*this = -rhs;
				return *this;
			}
			if (rhs == -1) {
				positive = !positive;
				return *this;
			}

			C_diff_t bufferSize = data.size() + rhs.data.size();
			C buffer(bufferSize);
			C_iter endOfNumber = multiplies_digits(data.begin(), data.end(), rhs.data.begin(), rhs.data.end(), buffer.begin());
			data.assign(buffer.begin(), endOfNumber);
			positive = positive == rhs.positive;

			assert(valid());
			return *this;
		}

		/**
		 * Divides the value of this Integer object by the value in rhs
		 *
		 * @param rhs an Integer object
		 *
		 * @throws invalid_argument if (rhs == 0)
		 */
		Integer& operator /=(const Integer& rhs) {
			C buffer(data.size(), 0);
			C_iter endOfNumber = divides_digits(data.begin(), data.end(), rhs.data.begin(), rhs.data.end(), buffer.begin());
			data.assign(buffer.begin(), endOfNumber);
			positive = positive == rhs.positive;

			assert(valid());
			return *this;
		}

		/**
		 * Divides the value of this Integer object the the value in rhs,
		 * stores the remainder in this Integer
		 *
		 * @param rhs an Integer object
		 *
		 * @throws invalid_argument if (rhs <= 0)
		 */
		Integer& operator %=(const Integer& rhs) {
			Integer lowerValue = *this;
			lowerValue /= rhs;
			lowerValue *= rhs;
			return *this -= lowerValue;
		}

		/**
		 * Shifts the value in this Integer left by n digits, filling in 0's
		 */
		Integer& operator <<=(int n) {
			if (*this != 0) {
				while (n-- > 0)
					data.push_back(0);
			}
			return *this;
		}

		/**
		 * Shifts the value in this Integer right by n digits
		 * If shifted past the end of the Integer, this Integer will be set to
		 * positive 0
		 */
		Integer& operator >>=(int n) {
			if (data.size() <= static_cast<unsigned int>(n)) {
				data.clear();
				data.push_back(0);
			}
			else {
				data.erase(data.end() - n, data.end());
			}
			return *this;
		}

		/**
		 * Stores the absolute value of this Integer back into this Integer
		 */
		Integer & abs() {
			positive = true;
			return *this;
		}

		/**
		 * power
		 *
		 * Raises the value of this Integer to the e power
		 *
		 * @throws invalid_argument if (this == 0) && (e == 0)
		 * @throws invalid_argument if (e < 0)
		 */
		Integer& pow(int e) {
			if ((*this == 0 && e == 0) || e < 0)
				throw std::invalid_argument("Integer::pow()");
			if (*this == 1 || *this == 0)
				return *this;
			if (e == 0) {
				*this = 1;
				return *this;
			}

			// Learned this trick from Java's BigInteger
			Integer originalValue = *this;
			*this = 1;
			while (e != 0) {
				if ((e & 1) == 1)
					*this *= originalValue;
				if ((e >>= 1) != 0)
					originalValue *= originalValue;
			}
			return *this;
		}
};

#endif // Integer_h
