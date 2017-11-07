#pragma once

#include <utility>
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <initializer_list>


#define gateAND_DELAY 1.0
#define gateNOT_DELAY 0.5
#define gateOR_DELAY  1.0



namespace gate {
	typedef std::function<void ()>              Action;
	typedef std::list<std::weak_ptr<Action>>    Actions;
	typedef std::pair<bool,Actions>             Wire;
	typedef std::shared_ptr<Wire>               refWire;
	
	inline refWire wire() {return std::make_shared<Wire>(false,Actions());}
	inline bool&   value(refWire w) {return w -> first;}
	
	typedef std::pair<std::shared_ptr<Wire>,bool>    SetRequest;
	typedef std::multimap<double,SetRequest>         Requests;
	typedef std::pair<double, Requests>              Agenda;
	
	typedef std::shared_ptr<Action>                      refGate;
    typedef std::shared_ptr<Agenda>                      refAgenda;
     
     // Get the wire's action list 
inline Actions& actions(refWire w)                                        {return w -> second;}

// Build a shared-pointer handling a function. 
inline refGate action(Action a)         {return std::make_shared<Action>(a);}


// Execute the function handled by a
inline void execute(refGate a)                                            {(*a)();}

// Allocates an agenda
inline refAgenda agenda()                                                 {return std::make_shared<Agenda>(0,Requests());}

// Get the agenda's elements.
inline double& now(refAgenda agenda)                                      {return agenda->first;}
inline Requests& reqs(refAgenda agenda)                                   {return agenda->second;}

// Add an action in the wire
inline void link(refWire w,refGate a)                                     {actions(w).push_back(a);}

// Set the value of the wire. If the value is actually changed, call
// all the wire actions. As they are weak pointers, the action
// existence is to be checked. Weak pointers to dead actions should be
// removed while iterating on actions.
inline void set(refWire wire, bool val)                                   {
	if(val != value(wire)) {
      value(wire) = val;
      auto iter = actions(wire).begin();
      while(iter != actions(wire).end()) {
	auto sp = iter->lock();
	if(sp) {
	  execute(sp);
	  ++iter;
	}
	else  // (*iter) has expired
	  iter = actions(wire).erase(iter);
      }
    }

}

// Add a new pending event, i.e. register the setting of 'value' to
// the wire 'w' at time now+delay.
inline void notify(refAgenda agenda, refWire w, bool value, double delay) {
	reqs(agenda).insert(std::make_pair(now(agenda)+delay,SetRequest(w,value)));
}

// Execute next pending event (if any). Returns false if all the
// pending events have been processed. This corresponds to a single
// simulation step.
inline bool next(refAgenda agenda)                                        {
	if(reqs(agenda).empty()){return false;}
	else{
		now(agenda)=reqs(agenda).begin()->first;
		auto set_req=reqs(agenda).begin()->second;
		reqs(agenda).erase(reqs(agenda).begin());
		set(set_req.first,set_req.second);
		return true;
		
	}
	
}

// Run the simulation until the end (may loop).
inline void flush(refAgenda agenda)                                       {
	while(next(agenda));
		
}

// Run the simulation for at most nb_steps_max steps.
inline void flush(refAgenda agenda, unsigned int nb_steps_max)            {
	unsigned int i;
	for(i=1;i<nb_steps_max && next(agenda);i++)	;
}

//this gate simply print out the wire value
inline refGate probe(refAgenda agenda, refWire w, const std::string& name){
	auto update = action([agenda,w,name]() -> void {
//#ifndef gateTRACE_MEMORY
	std::cout << "|" << std::setw(15) << now(agenda) << " | "
		  << std::setw(5) << name << " | ";
	if(value(w))
	  std::cout << "on  |" << std::endl;
	else
	  std::cout << "off |" << std::endl;
//#endif
      });
    
    // We register the action into the wire.
    link(w,update);
    return update;
}

inline refGate And(refAgenda agenda, refWire a, refWire b, refWire c){
	auto update = action([agenda,a,b,c]() -> void {
		notify(agenda,c,value(a)and value(b),gateAND_DELAY);});
	link(a,update);
	link(b,update);
	execute(update);
	return update;
	
}

inline refGate Not(refAgenda agenda, refWire a, refWire b){
	auto update = action([agenda,a,b]() -> void {
		notify(agenda,b,!value(a),gateNOT_DELAY);});
		link(a,update);
		execute(update);
		return update;
	}

inline refGate Or(refAgenda agenda, refWire a, refWire b,refWire c){
	auto update = action([agenda,a,b,c]() -> void{
		notify(agenda,c,value(a) or value(b),gateOR_DELAY);});
		link(a,update);
		link(b,update);
		execute(update);
		return update;
	}
	
inline refGate Xor(refAgenda agenda, refWire a, refWire b, refWire c){
	auto update = action([agenda,a,b,c]() -> void {
	auto na = wire();
	auto nb = wire();
	auto n1 = Not(agenda,a,na);
	auto n2 = Not(agenda,b,nb);
	auto o1 = wire();
	auto o2 = wire();
	auto an1 = And(agenda,na,b,o1);
	auto an2 = And(agenda,a,nb,o2);
	auto orp = Or(agenda,o1,o2,c);});
	return update;
	}


inline void ____() {
#ifndef gateTRACE_MEMORY
    std::cout << "+----------------+-------+-----+" << std::endl;
#endif
  }

class Diagram{
private:
std::string title;
std::string fileName;
std::string fileType;

public:

	
};

}
