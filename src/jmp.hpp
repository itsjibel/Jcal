/* Definitions for JMP class functions   -*- mode: C++ -*-

Copyleft(🄯) All uses of this library are free.
  * This library is for challenging my (or your) C++ knowledge to write the good Multiple Precision Arithmetic Library,
    all advice or help would be appreciated.

Github address: https://github.com/itsjibel/JMP

Authors:
  * Jibel Sadeghi <itsjibel@gmail.com> */

#include <algorithm>
#include <complex>
#include <memory>

class jmp
{
    private:
        typedef unsigned long long int ulli;
        typedef long long int lli;
        typedef std::string str;

        /// The main members of the class
        ulli float_point_index {0};
        lli division_precision {10}, precision {-1};
        str number {"0"};
        bool initialized {false};

        /// The helper functions
        void validation (const str& num);
        bool which_string_number_is_bigger(const str& num1, const str& num2) const;
        void trim_the_number(jmp& j, const bool bigger_number_is_negative);
        str subtract(const str& num1, const str& num2);

        /// Arithmetic functions
        void FFT(std::complex<double>* a, ulli& n, const bool invert);
        str sum(const str& num1, const str& num2,
                        bool first_number_is_bigger, bool first_number_is_negative, bool second_number_is_negative,
                        ulli& sum_obj_float_point_index);
        str multiply(const str& num1, const str& num2);
        str divide(str& num1, str& num2);

    public:
        bool is_negative {false};
        /// Constructors
        jmp() {}
        jmp(const str& num) { initialized = true; validation(num); }
        jmp(const char* num) { initialized = true; validation(num); }
        jmp (const jmp& j)
        {
            number = j.number;
            float_point_index = j.float_point_index;
            is_negative = j.is_negative;
            initialized = true;
        }

        /// Destructor
        ~jmp() { reset(); }

        /// ostream operator
        friend std::ostream& operator<<(std::ostream& o, const jmp& j)
        {
            o<<(j.is_negative ? '-' : '\0')<<j.number;
            return o;
        }

        void reset()
        {
            number = float_point_index = is_negative = initialized = 0;
            precision = -1;
        }

        // Getter functions
        str get_number() const
        {
            return number;
        }

        ulli get_division_precision() const
        {
            return division_precision;
        }

        lli get_precision() const
        {
            return precision;
        }

        // Conversion functions
        jmp internal_conversion_to_int()
        {
            number = number.substr(0, float_point_index);
            float_point_index = 0;
            return *this;
        }

        jmp append (const str& num)
        {
            if (!num.empty() && std::find_if(num.begin(), 
                 num.end(), [](unsigned char c) { return !std::isdigit(c); }) == num.end())
                number.append(num);
            return *this;
        }

        jmp set_precision (lli precision)
        {
            if (precision >= -1)
            {
                this->precision = precision;
                if (float_point_index != 0 && precision != -1)
                    number = number.substr(0, float_point_index + precision + (precision == 0 ? 0 : 1));
                float_point_index = precision == 0 ? 0 : float_point_index;
            }
            return *this;
        }

        void set_division_precision (lli precision)
        {
            division_precision = precision >= 0 ? precision : 10;
        }

        jmp round_precision (lli precision)
        {
            // First, add the required number to number to round it
            if (precision == 0)
            {
                jmp one("1");
                if (float_point_index != 0 && number[float_point_index + 1] >= '5')
                    *this += one;
            } else {
                jmp number_add_after_rounding("0.1"), one_tenth("0.1");
                for (int i=1; i<precision; i++)
                    number_add_after_rounding *= one_tenth;

                if (number[float_point_index + precision + 1] >= '5')
                    *this += number_add_after_rounding;
            }
            /* Now we added or have not added the required number to our JMP number.
               number. It's time just to use the 'set_precision' function to set the number precision. */
            set_precision(precision);
            return *this;
        }

        // About-number functions
        bool is_decimal() const { return float_point_index != 0; }
        bool is_integer() const { return float_point_index == 0; }
        bool is_initialized() const { return initialized; }
        ulli size() const { return number.size(); }
        ulli num_of_decimals() { return (number.size() - float_point_index == number.size() ? 0 : number.size() - float_point_index - 1); };

        /// Summation operators
        jmp operator+();
        jmp operator+(jmp& j);
        jmp operator+=(jmp& j);
        jmp operator++();
        jmp operator++(int);

        /// Subtraction operators
        jmp operator-();
        jmp operator-(jmp& j);
        jmp operator-=(jmp& j);
        jmp operator--(int);
        jmp operator--();

        /// Multiplication operators
        jmp operator*(jmp& j);
        jmp operator*=(jmp& j);

        /// Divideation operators
        jmp operator/(jmp& j);
        jmp operator/=(jmp& j);

        /// Modulus operators
        jmp operator%(jmp& j);
        jmp operator%=(jmp& j);

        /// Conditional operators
        bool operator==(jmp& j);
        bool operator!=(jmp& j);
        bool operator<(jmp& j);
        bool operator<=(jmp& j);
        bool operator>(jmp& j);
        bool operator>=(jmp& j);

        // Conversion operator
        operator str() const
        {
            return (is_negative ? "-" : "") + number;
        }
};

namespace JMP
{
    jmp abs(jmp j)
    {
        j.is_negative = false;
        return j;
    }

    jmp fact(jmp& j)
    {
        jmp zero("0");
        if (j.is_decimal() || j < zero)
            std::__throw_logic_error("JMP::fact: Factorial is only defined for natural numbers.");

        jmp result("1");
        for (jmp i="1"; i<=j; i++)
            result *= i;

        return result;
    }

    jmp mod(jmp& dividend, jmp& divisor)
    {
        jmp quotient((dividend / divisor).round_precision(0));
        jmp a((quotient * divisor)), zero("0");
        jmp remainder (dividend - a);
        if (remainder < zero)
            remainder += divisor;
        return remainder;
    }

    jmp gcd(jmp a, jmp b)
    {
        if (a.is_decimal() || b.is_decimal())
            std::__throw_logic_error("JMP::gcd: Can not calculate gcd of two decimal jmp numbers.");
        jmp zero("0");
        return b == zero ? a : gcd(b, a % b);
    }

    jmp sqrt(jmp& j)
    {
        jmp x(j), y("1"), sqrt_precision("1"), two("2"), ten("10");

        x.set_division_precision(j.get_division_precision());
        y.set_division_precision(j.get_division_precision());
        sqrt_precision.set_division_precision(j.get_division_precision());

        for (unsigned long long int i{0}; i<j.get_division_precision(); i++)
            sqrt_precision /= ten;

        // Iterate until the difference between x and y is less than the sqrt precision value
        while (x - y > sqrt_precision)
        {
            x = (x + y) / two;
            y = j / x;
        }

        return x;
    }

    jmp pow(jmp& base, jmp exponent)
    {
        if (exponent.is_decimal())
            std::__throw_logic_error("JMP::pow: The number cannot be raised to a decimal power.");

        jmp result("1"), negative_one("-1");
        if (exponent.is_negative)
            while (exponent++ <= negative_one)
                result /= base;
        else
            while (--exponent > negative_one)
                result *= base;
        return result;
    }

    long long int to_int(const jmp& j) { return atoi(j.get_number().c_str()); }
    long double to_double(const jmp& j) { return std::stold(j.get_number()); }
    std::string to_string(const jmp& j) { return j.get_number(); }
    jmp sqr(jmp& j) { return j*j; }
    jmp cube(jmp& j) { return j*j*j; }
};

void jmp::FFT(std::complex<double>* a, ulli& n, const bool invert)
{
    const double angleMultiplier = (invert ? -2.0 : 2.0) * M_PI / n;

    // Bit-reversal permutation
    for (ulli i = 1, j = 0; i < n; ++i)
    {
        ulli bit = n >> 1;
        while (j & bit)
        {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;

        if (i < j)
            std::swap(a[i], a[j]);
    }

    // Precompute twiddle factors and perform butterfly operations
    for (ulli len = 2; len <= n; len <<= 1)
    {
        const double deltaAngle = angleMultiplier * (n / len);
        const std::complex<double> omegaBase = std::polar(1.0, deltaAngle);
        std::complex<double> omega = 1.0;

        for (ulli i = 0; i < len / 2; ++i)
        {
            for (ulli j = i; j < n; j += len)
            {
                std::complex<double> u = a[j];
                std::complex<double> t = omega * a[j + len / 2];
                a[j] = u + t;
                a[j + len / 2] = u - t;
            }

            omega *= omegaBase;
        }
    }

    // Normalize the output if invert flag is set
    if (invert)
    {
        const double scale = 1.0 / n;
        for (ulli i = 0; i < n; ++i)
            a[i] *= scale;
    }
}

std::string jmp::multiply(const str& num1, const str& num2)
{
    const ulli size1 {num1.size()}, size2 {num2.size()};
    ulli n {1};
    while (n < size1 + size2)
        n <<= 1;
    std::unique_ptr<std::complex<double>[]> a = std::make_unique<std::complex<double>[]>(n);
    std::unique_ptr<std::complex<double>[]> b = std::make_unique<std::complex<double>[]>(n);

    // Convert input strings to complex number arrays
    for (ulli i{0}; i<size1; ++i)
        a[i] = num1[size1 - i - 1] - '0';
    for (ulli i{0}; i < size2; ++i)
        b[i] = num2[size2 - i - 1] - '0';

    // Perform FFT on both arrays
    FFT(a.get(), n, false);
    FFT(b.get(), n, false);

    // Multiply the transformed arrays element-wise
    for (ulli i{0}; i<n; ++i)
        a[i] *= b[i];

    // Perform inverse FFT to obtain the convolution result
    FFT(a.get(), n, true);

    ulli carry {0};
    str result;

    // Construct the result str by rounding the real parts and performing carry propagation
    for (ulli i{0}; i<n; ++i)
    {
        ulli digit {static_cast<ulli>(a[i].real() + 0.5) + carry};
        result += '0' + (digit % 10);
        carry = digit / 10;
    }

    // Remove trailing zeros from the result string
    while (result.size() > 1 && result.back() == '0')
        result.pop_back();

    // Efficiently reverse the result string
    ulli start {0}, end {result.size() - 1};
    while (start < end)
    {
        std::swap(result[start], result[end]);
        ++start;
        --end;
    }

    return result;
}

void jmp::validation (const str& num)
{
    number = num;
    // If number is empty so is not valid
    if (number.empty())
    {
        number = "0";
        return;
    }
    // We erase the number sign to just check that this number str indexes are number or aren't a number
    if (number[0] == '-')
    {
        number.erase(number.begin());
        is_negative = true;
    } else if (number[0] == '+')
        number.erase(number.begin());

    bool valid {true}, float_point_found {false};
    for (ulli i{0}; i<number.size(); i++)
    {
        /* If the number character is '.', so we set float_point_found true to if another one detected
           we know number is not valid because we already have found the float point,
           then update the float_point_index to know where is the point index in the number */
        if (number[i] == '.')
        {
            if (float_point_found == 1)
            {
                valid = false;
                break;
            }
            float_point_found = true;
            float_point_index = i;
        } else if (number[i] < '0' || number[i] > '9') {
            /* If the number character is not in the range 0-9 and the number character is not '.'-
               then this character is invalid */
            valid = false;
            break;
        }
    }

    // "If conditions" for falsing the validity of the number string
    if ((number[0] == '0' && number[1] != '.') ||
        number.front() == '.')
        valid = false;

    if (!valid)
        number = 48 + (valid = is_negative = float_point_index = false);

    /* If the last index of the number is a '.' character so the user meant that this number is-
       a float number so we do consider this for the JMP number. */
    if (number.back() == '.') number.push_back('0');

    /* If the number is valid, we match the decimal precision of the number-
       that we are going to put in our number member with the decimal precision of the class itself. */
    if (valid && precision != -1 && number.size() - 1 > float_point_index + precision)
        number = number.substr(0, float_point_index + precision + (precision != 0 ? 1 : 0));
}

bool jmp::which_string_number_is_bigger(const str& num1, const str& num2) const
{
    // We should check digit by digit to understand which number is bigger
    unsigned long int num1size {num1.size()}, num2size {num2.size()};
    if (num1size > num2size)
        return 0;
    else if (num1size < num2size)
        return 1;
    else
        for (unsigned long int counter {0}; counter < num1size; counter++)
            if (num1[counter] > num2[counter])
                return 0;
            else if (num2[counter] > num1[counter])
                return 1;
    return 0;
}

std::string jmp::sum (const str& num1, const str& num2,
                      bool first_number_is_bigger, bool first_number_is_negative, bool second_number_is_negative,
                      ulli& sum_obj_float_point_index)
{
    str result = first_number_is_bigger ? num1 : num2;

    if (first_number_is_bigger && (first_number_is_negative == second_number_is_negative))
    {
        /* If the numbers have the same sign (both are positive or negative),
           and this number is bigger, so we sum the second number to this number like this: */
        auto difference_size_of_two_numbers {num1.size() - num2.size()};
        for (auto i{num1.size() - 1}; i>0; i--)
        {
            /* We start from the last digit of this number and move forwards to the second digit,
               and if the current digit is greater than the difference between the two digits,
               we add the current digit of this number to the second digit. */
            if (i >= difference_size_of_two_numbers)
                result[i] += num2[i - difference_size_of_two_numbers] - '0';

            /* If the current digit gets more than '9',
               then we add 1 to the previous digit and subtract 10 from the current digit */
            if (result[i] > '9')
            {
                result[i - 1] += 1;
                result[i] -= 10;
            }
        }

        // If the difference between two numbers is zero, we add the first digits of the two numbers together
        if (0 == difference_size_of_two_numbers)
            result[0] += num2[0] - '0';

        // And again do the previous condition for the first digit
        if (result[0] > '9')
        {
            result[0] -= 10;
            result = '1' + result;
            sum_obj_float_point_index = sum_obj_float_point_index != 0 ? sum_obj_float_point_index + 1 : 0;
        }
    }
    else if (!first_number_is_bigger && (first_number_is_negative == second_number_is_negative))
    {
        // This is the same. Just for the second number instead of this number.
        auto difference_size_of_two_numbers {num2.size() - num1.size()};
        for (auto i{(num2.size() == 1 ? 1 : num2.size() - 1)}; i>0; i--)
        {
            if (i >= difference_size_of_two_numbers)
                result[i] += num1[i - (difference_size_of_two_numbers)] - '0';
            if (i != 0 && result[i] > '9')
            {
                result[i - 1] += 1;
                result[i] -= 10;
            }
        }

        if (0 == difference_size_of_two_numbers)
            result[0] += num1[0] - '0';

        if (result[0] > '9')
        {
            result[0] -= 10;
            result = '1' + result;
            sum_obj_float_point_index = sum_obj_float_point_index != 0 ? sum_obj_float_point_index + 1 : 0;
        }
    }
    else if (first_number_is_bigger && (first_number_is_negative != second_number_is_negative))
    {
        /* If the number signs are not equal together and this number is bigger,
          so we subtract the second number from this number */
        auto difference_size_of_two_numbers {num1.size() - num2.size()};
        for (auto i{num1.size() - 1}; i>0; i--)
        {
            /* We start from the last digit of this number and move forwards to the second digit,
               and if the current digit is greater than the difference between the two digits,
               we subtract second number digit from the current digit of this number. */
            if (i >= difference_size_of_two_numbers)
                result[i] -= num2[i - difference_size_of_two_numbers] - '0';

            /* If the current digit gets less than '0',
               then we subtract 1 to the previous digit and add 10 to the current digit */
            if (result[i] < '0')
            {
                result[i] += 10;
                result[i - 1]--;
            }
        }
        /* If the difference size of two numbers is 0,
           then in the last action we subtract the first digit of the second number from this number.
         */
        if (0 == difference_size_of_two_numbers)
            result[0] -= num2[0] - '0';
    }
    else if (!first_number_is_bigger && (is_negative != second_number_is_negative))
    {
        // This is the same. Just for the second number instead of this number.
        auto difference_size_of_two_numbers {num2.size() - num1.size()};
        for (auto i{num2.size() - 1}; i>0; i--)
        {
            if (i >= difference_size_of_two_numbers)
                result[i] -= num1[i - difference_size_of_two_numbers] - '0';

            if (result[i] < '0')
            {
                result[i] += 10;
                result[i - 1]--;
            }
        }

        if (0 == difference_size_of_two_numbers)
            result[0] -= num1[0] - '0';
    }
    return std::move(result);
}

std::string jmp::divide(str& dividend, str& divisor)
{
    auto how_much_forward {(dividend.length() >= divisor.length() ? divisor.length() : dividend.length())};
    ulli how_many_decimals_calculated {0};
    str quotient, hypothetical_number {dividend}, detached_part;
    bool can_add_fpi = true;

    while (hypothetical_number.size() < divisor.size())
    {
        can_add_fpi = false;
        hypothetical_number.push_back('0');
        quotient.push_back('0');
    }

    if (!can_add_fpi)
    {
        quotient.insert(quotient.begin() + 1, '.');
        how_many_decimals_calculated += quotient.size() - 2;
    }

    for (int i{0}; i<divisor.size(); i++)
        detached_part.push_back(hypothetical_number.at(i));

    while (how_many_decimals_calculated <= division_precision && detached_part != "0")
    {
        char num {0};
        while (which_string_number_is_bigger(detached_part, divisor) != 1)
        {
            num++;
            detached_part = subtract(detached_part, divisor);
        }

        if ((quotient.empty() && num == 0) == false)
            quotient.push_back('0' + num);

        if (detached_part != "0")
        {
            if (how_much_forward < dividend.size())
                detached_part.push_back(dividend.at(how_much_forward));
            else {
                if (can_add_fpi)
                {
                    if (quotient.empty())
                        quotient.push_back('0');
                    quotient.push_back('.');
                    can_add_fpi = false;
                }
                detached_part.push_back('0');
            }
        }

        if (!can_add_fpi)
            how_many_decimals_calculated++;

        how_much_forward++;
    }

    return quotient;
}

void jmp::trim_the_number(jmp& j, const bool bigger_number_is_negative)
{
    // Erase the useless beginning zeros after subtraction
    while (j.number[0] == '0')
    {
        j.number.erase(j.number.begin());
        if (j.float_point_index > 0)
        {
            j.float_point_index--;
            if (j.float_point_index == 0)
            {
                j.float_point_index++;
                j.number = "0" + j.number;
                break;
            }
        }
    }

    // Remove the ending useless zeros after subtraction
    while (j.number[j.number.size() - 1] == '0' && j.float_point_index != 0 && j.float_point_index != j.number.size())
        j.number.erase(j.number.begin() + j.number.size() - 1);

    if (j.number.empty())
        j.number = "0";
    else if (bigger_number_is_negative)
        j.is_negative = true;

    // If the float index is the last index of the number so we push back '0' to the number to have the valid number.
    if (j.float_point_index != 0 && j.float_point_index == j.number.size())
        j.number.append("0");
}

std::string jmp::subtract(const str& num1, const str& num2)
{
    str result;
    result.reserve(std::max(num1.size(), num2.size()));
    long int n = num1.size() - 1, j = num2.size() - 1;
    bool borrow {0};

    while (n >= 0 || j >= 0 || borrow)
    {
        char digit = (n >= 0 ? num1[n--] - 48 : 0) - (j >= 0 ? num2[j--] - 48 : 0) - borrow;
        if (digit < 0)
        {
            digit += 10;
            borrow = 1;
        } else
            borrow = 0;

        result.push_back(digit + 48);
    }

    std::reverse(result.begin(), result.end());

    // Remove leading zeros from the result
    result.erase(0, std::min(result.find_first_not_of(48), result.size() - 1));

    return result;
}

jmp jmp::operator+()
{
    return JMP::abs(*this);
}

jmp jmp::operator+(jmp& j)
{
    // Erase the float point from numbers to start the calculation
    if (j.float_point_index != 0)
        j.number.erase(j.number.begin() + j.float_point_index);
    if (float_point_index != 0)
        number.erase(number.begin() + float_point_index);

    jmp sum_obj;
    ulli temp_number_size {number.size()}, temp_second_number_size {j.number.size()},
         how_many_0_added_this_num {0}, how_many_0_added_j_num {0};

    // If two numbers do not have the same decimals, we add '0' decimals to the end of that number that has lower decimals.
    if (j.float_point_index != 0 && float_point_index != 0 && (number.size() - float_point_index) > (j.number.size() - j.float_point_index))
    {
        // It means this number has more decimals than second number
        // Now we need to know how many '0' decimals we should push back to the second number.
        while (j.num_of_decimals() < num_of_decimals())
        {
            j.number.push_back('0');
            how_many_0_added_j_num++;
        }
    } else if (j.float_point_index != 0 && float_point_index != 0 && (number.size() - float_point_index) < (j.number.size() - j.float_point_index)) {
        while (num_of_decimals() < j.num_of_decimals())
        {
            number.push_back('0');
            how_many_0_added_this_num++;
        }
    } else if (j.float_point_index != 0 && float_point_index == 0) {
        while (how_many_0_added_this_num < j.number.size() - j.float_point_index)
        {
            number.push_back('0');
            how_many_0_added_this_num++;
        }
    } else if (j.float_point_index == 0 && float_point_index != 0) {
        while (how_many_0_added_j_num < number.size() - float_point_index)
        {
            j.number.push_back('0');
            how_many_0_added_j_num++;
        }
    } 

    // Check which number is bigger, and we equal the sum object number to the biggest number
    bool this_number_is_bigger {false}, second_number_is_bigger {false};
    if (which_string_number_is_bigger(number, j.number) == 0)
    {
        if (float_point_index != 0)
            sum_obj.float_point_index = float_point_index;
        else if (j.float_point_index != 0)
            sum_obj.float_point_index = temp_number_size;
        this_number_is_bigger = true;
    } else {
        if (j.float_point_index != 0)
            sum_obj.float_point_index = j.float_point_index;
        else if (float_point_index != 0)
            sum_obj.float_point_index = temp_second_number_size;
        second_number_is_bigger = true;
    }
    // Get sum of the two filtered strings
    sum_obj.number = sum(number, j.number, this_number_is_bigger, is_negative, j.is_negative, sum_obj.float_point_index);

    /* Trim the number means if we have a number such as '000012.32400' after the sum of two numbers, we trim the number to '12.324'
     * (-999900 + 999912) = 000012  ---> 12
     * (-0.2222 + 12.2222) = 12.0000 ---> 12
     * (-9900.22 + 9912.22) = 0012.00 ---> 12 */
    trim_the_number(sum_obj, (this_number_is_bigger && is_negative) || (second_number_is_bigger && j.is_negative));

    // Remove added zeros from the tail of the numbers
    while (how_many_0_added_this_num-- > 0)
        number.pop_back();
    while (how_many_0_added_j_num-- > 0)
        j.number.pop_back();

    // Add float point to numbers
    if (float_point_index != 0)
        number.insert(number.begin() + float_point_index, '.');
    if (j.float_point_index != 0)
        j.number.insert(j.number.begin() + j.float_point_index, '.');
    if (sum_obj.float_point_index != 0)
        sum_obj.number.insert(sum_obj.number.begin() + sum_obj.float_point_index, '.');

    sum_obj.is_negative = (j.is_negative && second_number_is_bigger) || (is_negative && this_number_is_bigger);
    sum_obj.division_precision = division_precision;

    return sum_obj;
}

jmp jmp::operator*(jmp& j)
{
    // Erase the float point from numbers to start the calculation
    if (j.float_point_index != 0)
        j.number.erase(j.number.begin() + j.float_point_index);
    if (float_point_index != 0)
        number.erase(number.begin() + float_point_index);

    ulli number_of_removed_zeros_this_number {0}, number_of_removed_zeros_second_number {0};
    while(j.number[0] == '0')
    {
        j.number.erase(j.number.begin());
        number_of_removed_zeros_second_number++;
    }

    while(number[0] == '0')
    {
        number.erase(number.begin());
        number_of_removed_zeros_this_number++;
    }

    jmp mul_obj;
    if (j.number == "1")
        mul_obj = jmp(number);
    else if (number == "1")
        mul_obj = jmp(j.number);
    else
        mul_obj.number = multiply(number, j.number);

    for (auto i=0; i<number_of_removed_zeros_second_number; i++)
        j.number.insert(j.number.begin(), '0');
    for (auto i=0; i<number_of_removed_zeros_this_number; i++)
        number.insert(number.begin(), '0');

    ulli sum_of_decimals_of_two_numbers {
        (number.size() - (float_point_index == 0 ? number.size() : float_point_index)) +
        (j.number.size() - (j.float_point_index == 0 ? j.number.size() : j.float_point_index))};

    /* When we have two numbers like 0.0001, and 0.00002,
       mul_obj gets 2 because 1 * 2 is 2, and sum_of_decimals_of_two_numbers gets 9-
       because we have 4 decimals in 0.0001 and 5 decimals in 0.00002,
       so if we want to get correct multiplication,
       we have to add '0' sum_of_decimals_of_two_numbers - mul_obj.number.size() times-
       to begin of the number to get the correct multiplication result, which is 0.000000003.
       (8 zeros to the beginning of '3' result give '000000003' and we add one '0' for the decimal number to have 0.0000...) */
    while (mul_obj.number.size() <= sum_of_decimals_of_two_numbers)
        mul_obj.number = "0" + mul_obj.number;

    mul_obj.float_point_index = mul_obj.number.size() - sum_of_decimals_of_two_numbers;

    /* Determining the negativity of a number from the simple principle of-
       negative in positive negative and positive in negative negative */
    mul_obj.is_negative = (is_negative == true && j.is_negative == false) ||
                          (is_negative == false && j.is_negative == true);

    // Add float point to numbers
    if (float_point_index != 0)
        number.insert(number.begin() + float_point_index, '.');
    if (j.float_point_index != 0)
        j.number.insert(j.number.begin() + j.float_point_index, '.');

    if (mul_obj.float_point_index != 0 && mul_obj.float_point_index != mul_obj.number.size())
        mul_obj.number.insert(mul_obj.number.begin() + mul_obj.float_point_index, '.');
    if (mul_obj.float_point_index == mul_obj.number.size())
        mul_obj.float_point_index = 0;

    // Remove the ending useless zeros after multiplication
    while (mul_obj.float_point_index != 0 && mul_obj.number[mul_obj.number.size() - 1] == '0' &&
           (mul_obj.float_point_index <= mul_obj.number.size() - 1))
        mul_obj.number.erase(mul_obj.number.begin() + mul_obj.number.size()  - 1);

    if (mul_obj.number.back() == '.')
    {
        mul_obj.number.pop_back();
        mul_obj.float_point_index = 0;
    }
    mul_obj.division_precision = division_precision;

    return mul_obj;
}

jmp jmp::operator-()
{
    jmp negative(*this);
    negative.is_negative = true;
    return negative;
}

jmp jmp::operator-(jmp& j)
{
    jmp negative(j);
    negative.is_negative = true;
    return *this + negative;
}

jmp jmp::operator/(jmp& j)
{
    if (j.number == "0.0" || j.number == "0")
        std::__throw_logic_error("jmp::operator/: Can not do division operation for zero.");

    /// Make ready the numbers for division
    /* Erase the float point from numbers to start the calculation
       and if the numbers for division is decimal we convert them to integer. */
    if (j.float_point_index != 0)
        j.number.erase(j.number.begin() + j.float_point_index);
    if (float_point_index != 0)
        number.erase(number.begin() + float_point_index);

    // Delete the beginning zeros from number that have '0' in the beginning
    ulli number_of_deleted_zeros {0}, number_of_deleted_zeros_j {0};
    while (number[0] == '0')
    {
        number.erase(number.begin());
        number_of_deleted_zeros++;
    }

    while (j.number[0] == '0')
    {
        j.number.erase(j.number.begin());
        number_of_deleted_zeros_j++;
    }

    // Note: fpi is 'float point index'
    bool could_reset_fpi=false, could_reset_jfpi=false;
    lli how_many_zero_added_this_num {0}, how_many_zero_added_sec_num {0};
    if (num_of_decimals() + (float_point_index == 0 ? 0 : 1) < j.num_of_decimals() + (j.float_point_index == 0 ? 0 : 1))
    {
        if (float_point_index == 0)
        {
            float_point_index = number.size();
            could_reset_fpi = true;
        }

        while (number.size() - float_point_index < j.number.size() - j.float_point_index)
        {
            number.push_back('0');
            how_many_zero_added_this_num++;
        }
    } else if (j.num_of_decimals() + (j.float_point_index == 0 ? 0 : 1) < num_of_decimals() + (float_point_index == 0 ? 0 : 1)) {    
        if (j.float_point_index == 0)
        {
            j.float_point_index = j.number.size();
            could_reset_jfpi = true;
        }

        while (j.number.size() - j.float_point_index < number.size() - float_point_index)
        {
            j.number.push_back('0');
            how_many_zero_added_sec_num++;
        }
    }

    /* Set the float point index to zero to don't make a mistake in the calculation,
       and save them for restoring the numbers to default. */
    ulli temp_float_point_index = could_reset_fpi ? 0 : float_point_index,
         j_temp_float_point_index = could_reset_jfpi ? 0 : j.float_point_index;
    float_point_index = j.float_point_index = 0;

    /* If the number will divide by 1 when we are sure the result will be the number we have,
       so we equal the 'div_obj' to the number, if the number will not divide by 1, we do the division */
    jmp div_obj;

    if (j.number == "1" || j.number == "1.0")
        div_obj = jmp(number);
    else
        div_obj = divide(number, j.number);

    for (ulli i=0; i<how_many_zero_added_sec_num; i++)
        j.number.pop_back();
    if (j.number.size() - 1 == j.float_point_index)
        j.float_point_index = 0;

    for (ulli i=0; i<how_many_zero_added_this_num; i++)
        number.pop_back();
    if (number.size() - 1 == float_point_index)
        float_point_index = 0;

    // Add the removed zeros from the beginning of the numbers
    for (ulli i=0; i<number_of_deleted_zeros; i++)
        number.insert(number.begin(), '0');
    for (ulli i=0; i<number_of_deleted_zeros_j; i++)
        j.number.insert(j.number.begin(), '0');

    // Add float point to numbers
    float_point_index = temp_float_point_index;
    j.float_point_index = j_temp_float_point_index;
    if (float_point_index != 0)
        number.insert(number.begin() + float_point_index, '.');
    if (j.float_point_index != 0)
        j.number.insert(j.number.begin() + j.float_point_index, '.');

    // Set the negativity of the division product
    div_obj.is_negative = (is_negative == true && j.is_negative == false) ||
                          (is_negative == false && j.is_negative == true);

    // Set the decimal of the division product
    jmp one_thenth("0.1");
    for (ulli i {0}; i<abs(number_of_deleted_zeros - number_of_deleted_zeros_j); i++)
        div_obj *= one_thenth;

    while (div_obj.is_decimal() && div_obj.number.back() == '0')
        div_obj.number.pop_back();

    if (div_obj.number[div_obj.number.size() - 1] == '.')
    {
        div_obj.number.pop_back();
        div_obj.float_point_index = 0;
    }

    div_obj.number = div_obj.number.substr(0, div_obj.number.find('.') + division_precision + 1);
    div_obj.division_precision = division_precision;

    return div_obj;
}

jmp jmp::operator%(jmp& j)
{
    return JMP::mod(*this, j);
}

jmp jmp::operator++()
{
    jmp one("1");
    *this += one;
    return *this;
}

jmp jmp::operator--()
{
    jmp one("1");
    *this -= one;
    return *this;
}

jmp jmp::operator++(int)
{
    jmp temp(*this), one("1");
    *this += one;
    return temp;
}

jmp jmp::operator--(int)
{
    jmp temp = *this, one("1");
    *this -= one;
    return temp;
}

jmp jmp::operator+=(jmp& num2_str)
{
    *this = *this + num2_str;
    return *this;
}

jmp jmp::operator*=(jmp& num2_str)
{
    *this = *this * num2_str;
    return *this;
}

jmp jmp::operator/=(jmp& num2_str)
{
    *this = *this / num2_str;
    return *this;
}

jmp jmp::operator%=(jmp& num2_str)
{
    *this = *this % num2_str;
    return *this;
}

jmp jmp::operator-=(jmp& j)
{
    *this = *this - j;
    return *this;
}

bool jmp::operator==(jmp& j)
{
    bool result = false, tail_zeros_removed_from_this = false, tail_zeros_removed_from_j = false;
    // Remove unnecessary tail zeros for comparison numbers
    if (number.back() == '0' && is_decimal())
    {
        // Remove zero and float point index
        number.erase(number.end() - 2, number.end());
        tail_zeros_removed_from_this = true;
    }
    if (j.number.back() == '0' && j.is_decimal())
    {
        j.number.erase(j.number.end() - 2, j.number.end());
        tail_zeros_removed_from_j = true;
    }
    // Save the result
    result = (is_negative ? "-" : "") + number == (j.is_negative ? "-" : "") + j.number;
    // Back numbers to the default
    if (tail_zeros_removed_from_this)
        number.append(".0");
    if (tail_zeros_removed_from_j)
        j.number.append(".0");
    return result;
}

bool jmp::operator!=(jmp& j)
{
    return (is_negative ? "-" : "") + number != (j.is_negative ? "-" : "") + j.number ? true : false;
}

bool jmp::operator<(jmp& j)
{
    if (*this == j)
        return false;

    if (is_negative && j.is_negative == false)
        return true;
    else if (is_negative == false && j.is_negative)
        return false;

    if (float_point_index != 0)
        number.erase(number.begin() + float_point_index);
    if (j.float_point_index != 0)
        j.number.erase(j.number.begin() + j.float_point_index);

    lli how_many_zeros_removed {0}, j_how_many_zeros_removed {0};
    while (number[0] == '0')
    {
        how_many_zeros_removed++;
        number.erase(number.begin());
    }
    while (j.number[0] == '0')
    {
        j_how_many_zeros_removed++;
        j.number.erase(j.number.begin());
    }

    bool result = which_string_number_is_bigger(number, j.number);

    while (how_many_zeros_removed-- > 0)
        number.insert(number.begin(), '0');
    while (j_how_many_zeros_removed-- > 0)
        j.number.insert(j.number.begin(), '0');

    if (float_point_index != 0)
        number.insert(number.begin() + float_point_index, '.');
    if (j.float_point_index != 0)
        j.number.insert(j.number.begin() + j.float_point_index, '.');

    if (result == 0)
    {
        if (is_negative && j.is_negative)
            return true;
        else
            return false;
    } else {
        if (is_negative && j.is_negative)
            return false;
        else
            return true;
    }
}

bool jmp::operator<=(jmp& j)
{
    return *this < j || *this == j ? true : false;
}

bool jmp::operator>(jmp& j)
{
    return *this < j == false && *this != j ? true : false;
}

bool jmp::operator>=(jmp& j)
{
    return *this > j || *this == j ? true : false;
}