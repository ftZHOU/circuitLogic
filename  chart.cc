#include <iostream>
#include "gate.hpp"

int main(int agrc, char* argv[]) {

  auto agenda = gate::agenda();

  auto a      = gate::wire();
  auto b      = gate::wire();
  auto c      = gate::wire();
  auto g      = gate::And(agenda,a,b,c);
  
  gate::Diagram time_diagram = {"And gate", "chart.jpeg", "chart",
				agenda,
				{gate::plot("a",a),
				 gate::plot("b",b),
				 gate::plot("c",c)}};
  
  gate::notify(agenda, a, true,  2.0);
  gate::notify(agenda, b, true,  4.0);
  gate::notify(agenda, b, false, 6.0);
  gate::notify(agenda, a, false, 8.0);
  gate::flush(agenda);

  return 0;
}
