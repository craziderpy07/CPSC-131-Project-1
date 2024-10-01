#include <cmath>      // abs(), pow()
#include <compare>    // weak_ordering
#include <iomanip>    // quoted()
#include <iostream>
#include <string>
#include <type_traits>    // is_floating_point_v, common_type_t
#include <utility>        // move()

#include "Book.hpp"

/*******************************************************************************
**  Implementation of non-member private types, objects, and functions
*******************************************************************************/
namespace    // unnamed, anonymous namespace
{
  // Avoid direct equality comparisons on floating point numbers. Two values are equal if they are "close enough", which is
  // represented by Epsilon.  Usually, this is a pretty small number, but since we are dealing with money (only two, maybe three
  // decimal places) we can be a bit more tolerant.  Two floating point values are considered equal if they are within EPSILON of
  // each other.
  template<typename T, typename U>
  requires std::is_floating_point_v<std::common_type_t<T, U>>
  constexpr bool floating_point_is_equal( T const lhs, U const rhs, double const EPSILON = 1e-4 ) noexcept
  {
    ///  Write the lines of code that compare two floating point numbers.  Return true when the left hand side (lhs) and the right
    ///  hand side (rhs) are within Epsilon, and false otherwise.
    ///
    ///  See: "Floating point equality" in https://www.learncpp.com/cpp-tutorial/relational-operators-and-floating-point-comparisons/
    ///
    ///  Hint:  Avoid writing code that looks like this:
    ///           if( some expression that is true ) return the constant "true"
    ///           else                               return the constant "false"
    ///         for example, avoid:
    ///           if (a < b) return true;
    ///           else       return false;
    ///         do this instead:
    ///           return a < b;

    return std::abs( lhs - rhs ) <= EPSILON;
  }
}    // namespace

/*******************************************************************************
**  Constructors, assignments, and destructor
*******************************************************************************/

// Default and Conversion Constructor
Book::Book( std::string title, std::string author, std::string isbn, double price )
  : _title( std::move( title ) ), _author( std::move( author ) ), _isbn( std::move( isbn ) ), _price( price ) {}

// Copy constructor
Book::Book( Book const & other )
  : _title( other._title ), _author( other._author ), _isbn( other._isbn ), _price( other._price ) {}

// Move constructor
Book::Book( Book && other ) noexcept
  : _title( std::move( other._title ) ), _author( std::move( other._author ) ), _isbn( std::move( other._isbn ) ), _price( other._price ) {}

// Copy Assignment Operator
Book & Book::operator=( Book const & rhs ) &
{
  if( this != &rhs )
  {
    _isbn   = rhs._isbn;
    _title  = rhs._title;
    _author = rhs._author;
    _price  = rhs._price;
  }

  return *this;
}

// Move Assignment Operator
Book & Book::operator=( Book && rhs ) & noexcept
{
  if( this != &rhs )
  {
    _isbn   = std::move( rhs._isbn );
    _title  = std::move( rhs._title );
    _author = std::move( rhs._author );
    _price  = rhs._price;
  }

  return *this;
}

// Destructor
Book::~Book() noexcept {}

/*******************************************************************************
**  Accessors
*******************************************************************************/

// isbn() const
std::string const & Book::isbn() const &
{
  return _isbn;
}

// title() const
std::string const & Book::title() const &
{
  return _title;
}

// author() const
std::string const & Book::author() const &
{
  return _author;
}

// price() const
double Book::price() const &
{
  return _price;
}

// isbn()
std::string Book::isbn() &&
{
  return std::move( _isbn );
}

// title()
std::string Book::title() &&
{
  return std::move( _title );
}

// author()
std::string Book::author() &&
{
  return std::move( _author );
}

/*******************************************************************************
**  Modifiers
*******************************************************************************/

// isbn()
Book & Book::isbn( std::string newIsbn ) &
{
  _isbn = newIsbn;
  return *this;
}

// title()
Book & Book::title( std::string newTitle ) &
{
  _title = newTitle;
  return *this;
}

// author()
Book & Book::author( std::string newAuthor ) &
{
  _author = newAuthor;
  return *this;
}

// price()
Book & Book::price( double newPrice ) &
{
  _price = newPrice;
  return *this;
}

/*******************************************************************************
**  Relational Operators
*******************************************************************************/
// operator<=>
std::weak_ordering Book::operator<=>( const Book & rhs ) const noexcept
{
  // Design decision:  A very simple and convenient defaulted 3-way comparison operator
  //                         auto operator<=>( const Book & ) const = default;
  //                   in the class definition (header file) would get very close to what is needed and would allow both the <=> and
  //                   the == operators defined here to be skipped.  The physical ordering of the attributes in the class definition
  //                   would have to be changed (easy enough in this case) but the default directly compares floating point types
  //                   (price) for equality, and that should be avoided, in general. For example, if x and y are of type double,
  //                   then x < y is okay but x == y is not.  So these (operator<=> and operator==) explicit definitions are
  //                   provided.
  //
  //                   Also, many ordering (sorting) algorithms, like those used in std::map and std::set, require at least a weak
  //                   ordering of elements. operator<=> provides only a partial ordering when comparing floating point numbers.
  //
  // Weak order:       Objects that compare equal but are not substitutable (identical).  For example, since _price can be within
  //                   EPSILON, Book("Title", "Author", "ISBN", 9.99999) and Book("Title", "Author", "ISBN", 10.00001) are equal but
  //                   they are not identical.  If you ignore case when comparing strings, as another example, Book("Title") and
  //                   Book("title") are equal but they are not identical.
  //
  // See std::weak_ordering    at https://en.cppreference.com/w/cpp/utility/compare/weak_ordering and
  //     std::partial_ordering at https://en.cppreference.com/w/cpp/utility/compare/partial_ordering
  //     The Three-Way Comparison Operator at  http://modernescpp.com/index.php/c-20-the-three-way-comparison-operator
  //     Spaceship (Three way comparison) Operator Demystified https://youtu.be/S9ShnAFmiWM
  //
  //
  // Books are equal if all attributes are equal (or within Epsilon for floating point numbers, like price). Books are ordered
  // (sorted) by ISBN, author, title, then price.
  // Compare ISBNs

  // auto result;

  // Compare ISBNs
  if( auto result = _isbn <=> rhs._isbn; result != 0 )
  {
    return result;
  }

  // Compare titles
  if(auto result = _title <=> rhs._title; result != 0 )
  {
    return result;
  }

  // Compare authors
  if(auto result = _author <=> rhs._author; result != 0 )
  {
    return result;
  }

  if( floating_point_is_equal( _price, rhs._price ) )
  {
    return std::weak_ordering::equivalent;
  }
  return _price < rhs._price ? std::weak_ordering::less : std::weak_ordering::greater;
}

// operator==
bool Book::operator==( const Book & rhs ) const noexcept
{
  // All attributes must be equal for the two books to be equal to the other.  This can be done in any order, so put the quickest
  // and then the most likely to be different first.

  return ( _title == rhs._title ) && ( _author == rhs._author ) && ( _isbn == rhs._isbn ) && ( _price == rhs._price );
}

/*******************************************************************************
**  Insertion and Extraction Operators
*******************************************************************************/

// operator>>
std::istream & operator>>( std::istream & stream, Book & book )
{
  Book bookIn;
  char spacer = ',';

  stream >> std::quoted( bookIn._isbn ) >> spacer
      >> std::quoted( bookIn._title ) >> spacer
      >> std::quoted( bookIn._author ) >> spacer
      >> bookIn._price;

  if( stream ) book = std::move( bookIn );

  return stream;

  /// A lot can go wrong when reading from streams - no permission, wrong types, end of file, etc. Minimal exception guarantee says
  /// there should be no side affects if an error or exception occurs, so let's do our work in a local object and move it into place
  /// at the end if all goes well.
  ///
  /// This function should be symmetrical with operator<< below.  Read what your write, and write what you read
  ///
  ///
  /// Assume fields are separated by commas and string attributes are enclosed with double quotes.  For example:
  ///    ISBN             | Title                 | Author             | Price
  ///    -----------------+-----------------------+--------------------+-----
  ///    "9789998287532",   "Over in the Meadow",   "Ezra Jack Keats",   91.11
  ///
  ///
  /// Hint:  Use std::quoted to read and write quoted strings.  See
  ///        1) https://en.cppreference.com/w/cpp/io/manip/quoted
  ///        2) https://www.youtube.com/watch?v=Mu-GUZuU31A
}

// operator<<
std::ostream & operator<<( std::ostream & stream, const Book & book )
{
  /// This function should be symmetrical with operator>> above.  Read what your write, and write what you read
  const std::string spacer = ", ";

  stream << std::quoted( book.isbn() ) << spacer
         << std::quoted( book.title() ) << spacer
         << std::quoted( book.author() ) << spacer
         << book.price();

  return stream;
}
