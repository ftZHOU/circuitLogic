#include <iostream>
#include "gate.hpp"


int main(int agrc, char* argv[]) {

  auto agenda = gate::agenda();
  auto a      = gate::wire();
  auto p      = gate::probe(agenda,a,"a");

  
  std::cout << "Current time : " << gate::now(agenda) << std::endl;

  gate::____(); // displays a line.
  gate::set(a,true);
  gate::set(a,false);
  gate::flush(agenda);
  gate::____();

  std::cout << "Current time : " << gate::now(agenda) << std::endl;
  
  gate::notify(agenda, a, true,  1.0);
  gate::notify(agenda, a, false, 2.0);
  gate::____();
  gate::flush(agenda);
  gate::____();

  std::cout << "Current time : " << gate::now(agenda) << std::endl;

  gate::notify(agenda, a, true,  1.0);
  gate::notify(agenda, a, false, 2.0);
  gate::notify(agenda, a, true,  3.0);
  gate::____();
  gate::flush(agenda);
  gate::____();

  std::cout << "Current time : " << gate::now(agenda) << std::endl;

  gate::notify(agenda, a, true,  1.0);
  gate::notify(agenda, a, false, 2.0);
  gate::____();
  gate::flush(agenda);
  gate::____();

  std::cout << "Current time : " << gate::now(agenda) << std::endl;

  return 0;
}
