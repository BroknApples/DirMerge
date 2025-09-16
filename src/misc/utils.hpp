#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <type_traits>


/***********************************************************
******************* beg Random Variables *******************
***********************************************************/


/** @brief Is this executable in DEBUG mode? */
constexpr bool DEBUG = true;


/***********************************************************
******************* end Random Variables *******************
***********************************************************/


/***********************************************************
******************* beg Printing Methods *******************
***********************************************************/


// ----------------------------------------- //
// ---------- Core Implementation ---------- //
// ----------------------------------------- //


/**
 * @brief Traits to check if T can be sent to std::cout
 * NOTE: Def got this from ChatGPT, I don't know about these <type_traits> techniques yet
 */
template <typename T, typename = void>
struct is_streamable : std::false_type {};

template <typename T>
struct is_streamable<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>> : std::true_type {};


/** 
 * Single argument print.
 * 
 * @brief Print a value without a newline
 * @param os: Stream to output on
 * @param val: Value to print
 */
template <typename T>
typename std::enable_if<is_streamable<T>::value>::type
print(std::ostream& os, const T& val) {
  os << val;
}


/**
 * @brief print() fallback for non-streamable types
 * @param os: Stream to output on
 * @tparam T: 
 */ 
template <typename T>
typename std::enable_if<!is_streamable<T>::value>::type
print(std::ostream& os, const T&) {
  os << "[Unsupported Type]";
}


/** 
 * Variadic print
 * 
 * @brief Print some values without a newline
 * @param os: Stream to output on
 * @param first: The first argument to print
 * @param args: Used if a variable number are args are passed for printing
 */
template <typename T, typename... Args>
void print(std::ostream& os, const T& first, const Args&... args) {
  print(os, first);
  if constexpr (sizeof...(args) > 0) {
    print(os, args...);
  }
}


/** 
 * Single argument print.
 * 
 * @brief Print a value along with a newline
 * @param os: Stream to output on
 * @param val: Value to print
 */
template <typename T>
typename std::enable_if<is_streamable<T>::value>::type
println(std::ostream& os, const T& val) {
  os << val << std::endl;
}


/**
 * @brief println() fallback for non-streamable types
 * @param os: Stream to output on
 * @tparam T: 
 */ 
template <typename T>
typename std::enable_if<!is_streamable<T>::value>::type
println(std::ostream& os, const T&) {
  os << "[Unsupported Type]" << std::endl;
}


/** 
 * Variadic print
 * 
 * @brief Print some values along with a newline
 * @param os: Stream to output on
 * @param first: The first argument to print
 * @param args: Used if a variable number are args are passed for printing
 */
template <typename T, typename... Args>
void println(std::ostream& os, const T& first, const Args&... args) {
  print(os, first);
  if constexpr (sizeof...(args) > 0) {
    print(os, args...);
  }

  // Empty println for a newline at the end.
  println(os, "");
}


// ------------------------------------------ //
// ---------- Convenience Wrappers ---------- //
// ------------------------------------------ //


/** 
 * @brief Print some values without a newline
 * @param args: Values to print
 */
template <typename... Args>
void print(const Args&... args) {
  print(std::cout, args...);
}


/** 
 * @brief Print some values along with a newline
 * @param args: Values to print
 */
template <typename... Args>
void println(const Args&... args) {
  println(std::cout, args...);
}


/** 
 * @brief Error print some values without a newline
 * @param args: Values to print
 */
template <typename... Args>
void eprint(const Args&... args) {
  print(std::cerr, args...);
}


/** 
 * @brief Error print some values along with a newline
 * @param args: Values to print
 */
template <typename... Args>
void eprintln(const Args&... args) {
  println(std::cerr, args...);
}


/***********************************************************
******************* end Printing Methods *******************
***********************************************************/


#endif // UTILS_HPP
