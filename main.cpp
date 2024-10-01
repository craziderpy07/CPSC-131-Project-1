#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Book.hpp"

int main()
{
  std::cout << std::showpoint << std::fixed << std::setprecision( 2 )
            << "Welcome to Forgotten Books, a book store filled with books from all nations. Place books into your shopping cart by entering each book's information.\n"
            << " enclose string entries in quotes, separate fields with comas\n"
            << " Enter CTL-Z (Windows) or CTL-D (Linux) to quit\n\n";

  std::vector<std::unique_ptr<Book>> shoppingCart;

  Book                               book;

  while( std::cout << "Enter ISBN, Title, Author, and Price\n", std::cin >> book )
  {
    shoppingCart.emplace_back( std::make_unique<Book>( std::move( book ) ) );
    std::cout << "Item added to shopping cart: " << *shoppingCart.back() << "\n\n";
  }

  for( auto i = shoppingCart.crbegin(); i < shoppingCart.crend(); ++i )
  {
    std::cout << **i << '\n';
  }

  return 0;
}
