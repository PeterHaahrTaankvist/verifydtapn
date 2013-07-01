/*
 * NonStrictSearch.cpp
 *
 *  Created on: 26/04/2012
 *      Author: MathiasGS
 */

#include "TimeDartReachabilitySearch.hpp"

namespace VerifyTAPN {
namespace DiscreteVerification {

bool TimeDartReachabilitySearch::verify(){
	// no need to check if trace is set, just as expensive as setting the generated by
	initialMarking.setGeneratedBy(NULL);

	if(addToPW(&initialMarking, NULL, INT_MAX)){
		return true;
	}

	//Main loop
	while(pwList->hasWaitingStates()){
		TimeDartBase& dart = *pwList->getNextUnexplored();
		exploredMarkings++;

		int passed = dart.getPassed();
		dart.setPassed(dart.getWaiting());
		tapn->getTransitions();
                this->tmpdart = NULL;
                if(options.getTrace() == VerificationOptions::SOME_TRACE){
                    this->tmpdart = ((ReachabilityTraceableDart*)&dart)->trace;
                }
		for(TimedTransition::Vector::const_iterator transition_iter = tapn->getTransitions().begin();
				transition_iter != tapn->getTransitions().end(); transition_iter++){
			TimedTransition& transition = **transition_iter;
			pair<int,int> calculatedStart = calculateStart(transition, dart.getBase());
			if(calculatedStart.first == -1){	// Transition cannot be enabled in marking
				continue;
			}
			int start = max(dart.getWaiting(), calculatedStart.first);
			int end = min(passed-1, calculatedStart.second);
                        cout << "start " << start << endl;
                        cout << "end " << *dart.getBase() << endl;

			if(start <= end){

				if(transition.hasUntimedPostset()){
					NonStrictMarkingBase Mpp(*dart.getBase());
					Mpp.incrementAge(start);

                                        this->tmpupper = start;
                                        if(successorGenerator.generateAndInsertSuccessors(Mpp, transition)){
                                            return true;
                                        }
				}else{
					int stop = min(max(start, calculateStop(transition, dart.getBase())), end);
					for(int n = start; n <= stop; n++){
						NonStrictMarkingBase Mpp(*dart.getBase());
						Mpp.incrementAge(n);
                                                this->tmpupper = n;
                                                if(successorGenerator.generateAndInsertSuccessors(Mpp, transition)){
                                                    return true;
                                                }
					}
				}
			}
		}
                deleteBase(dart.getBase());
	}

	return false;
}

bool TimeDartReachabilitySearch::addToPW(NonStrictMarkingBase* marking, WaitingDart* parent, int upper){
        int start = 0;
        if(options.getTrace() == VerificationOptions::SOME_TRACE){
            start = marking->getYoungest();
        }
	int maxDelay = marking->cut();

	unsigned int size = marking->size();

	pwList->setMaxNumTokensIfGreater(size);

	if(size > options.getKBound()) {
                delete marking;
		return false;
	}
        
        int youngest = marking->makeBase(tapn.get());
        
	if(pwList->add(tapn.get(), marking, youngest, parent, upper, start)){
                // very curde way of doing this, integers could be passed as arguments for querychecker instead
                // this is only needed for deadlock-checks, should possibly be contained in if-structure (huge overhead)
                NonStrictMarkingBase maxed = NonStrictMarkingBase(*marking);
                if(maxDelay != std::numeric_limits<int>::max())
                    maxDelay += youngest;
                maxed.incrementAge(maxDelay);        // delay to maximum possible age (for deadlock query)
                maxed.cut();
		QueryVisitor<NonStrictMarkingBase> checker(maxed, *tapn.get());
		boost::any context;
		query->accept(checker, context);
		if(boost::any_cast<bool>(context)) {
                        if (options.getTrace()) {
                            lastMarking = pwList->getLast();
                        }
			return true;
		} else {
                        deleteBase(marking);
			return false;
		}
	}
        deleteBase(marking);
	return false;
}

void TimeDartReachabilitySearch::printStats(){
	std::cout << "  discovered markings:\t" << pwList->discoveredMarkings << std::endl;
	std::cout << "  explored markings:\t" << exploredMarkings << std::endl;
	std::cout << "  stored markings:\t" << pwList->size() << std::endl;
}

TimeDartReachabilitySearch::~TimeDartReachabilitySearch() {
}

} /* namespace DiscreteVerification */
} /* namespace VerifyTAPN */
