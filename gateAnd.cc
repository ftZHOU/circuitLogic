#include <iostream>
#include "gate.hpp"


int main(int agrc, char* argv[]) {

  auto agenda = gate::agenda();

  auto a      = gate::wire();
  auto b      = gate::wire();
  auto c      = gate::wire();

  auto pa     = gate::probe(agenda,a,"a");
  auto pb     = gate::probe(agenda,b,"b");
  auto pc     = gate::probe(agenda,c,"c");

  auto g      = gate::And(agenda,a,b,c);

  gate::notify(agenda, a, true,  2.0);
  gate::notify(agenda, b, true,  4.0);
  gate::notify(agenda, b, false, 6.0);
  gate::notify(agenda, a, false, 8.0);
  gate::____();
  gate::flush(agenda);
  gate::____();

  return 0;
}

