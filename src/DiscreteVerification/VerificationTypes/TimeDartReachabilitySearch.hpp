/*
 * NonStrictSearch.hpp
 *
 *  Created on: 26/04/2012
 *      Author: MathiasGS
 */

#ifndef TIMEDARTREACHABILITYSEARCH_HPP_
#define TIMEDARTREACHABILITYSEARCH_HPP_

#include "../DataStructures/TimeDart.hpp"
#include "../DataStructures/TimeDartPWList.hpp"
#include "boost/smart_ptr.hpp"
#include "boost/numeric/interval.hpp"
#include "../../Core/TAPN/TAPN.hpp"
#include "../../Core/QueryParser/AST.hpp"
#include "../../Core/VerificationOptions.hpp"
#include "../../Core/TAPN/TimedPlace.hpp"
#include "../../Core/TAPN/TimedTransition.hpp"
#include "../../Core/TAPN/TimedInputArc.hpp"
#include "../../Core/TAPN/TransportArc.hpp"
#include "../../Core/TAPN/InhibitorArc.hpp"
#include "../../Core/TAPN/OutputArc.hpp"
#include "../TimeDartSuccessorGenerator.hpp"
#include "../QueryVisitor.hpp"
#include "boost/any.hpp"
#include "../DataStructures/NonStrictMarking.hpp"
#include <stack>
#include "Verification.hpp"
#include "../DataStructures/TimeDart.hpp"
#include "../Util/IntervalOps.hpp"

namespace VerifyTAPN {
namespace DiscreteVerification {

using namespace rapidxml;

class TimeDartReachabilitySearch : public Verification{
public:
	typedef pair<NonStrictMarking*, int> TraceList;
public:
	TimeDartReachabilitySearch(boost::shared_ptr<TAPN::TimedArcPetriNet>& tapn, NonStrictMarking& initialMarking, AST::Query* query, VerificationOptions options, WaitingList<TimeDart>* waiting_list);
	virtual ~TimeDartReachabilitySearch();
	bool Verify();
	NonStrictMarking* GetLastMarking() { return lastMarking; }
	inline unsigned int MaxUsedTokens(){ return pwList.maxNumTokensInAnyMarking; };
	void PrintTransitionStatistics() const { successorGenerator.PrintTransitionStatistics(std::cout); }

protected:
	vector<NonStrictMarking*> getPossibleNextMarkings(NonStrictMarking& marking, const TimedTransition& transition);
	bool addToPW(NonStrictMarking* marking);
	pair<int,int> calculateStart(const TimedTransition& transition, NonStrictMarking* marking);
	int calculateEnd(const TimedTransition& transition, NonStrictMarking* marking);
	int calculateStop(const TimedTransition& transition, NonStrictMarking* marking);
	void addToTrace(NonStrictMarking* marking, NonStrictMarking* parent, int d);
	void PrintXMLTrace(NonStrictMarking* m, std::stack<TraceList*>& stack);
	xml_node<>* generateTransitionNode(NonStrictMarking* from, NonStrictMarking* to, xml_document<> doc);
	xml_node<>* generateDelayNode(int delay, xml_document<> doc);

protected:
	int validChildren;
	TimeDartPWList pwList;
	boost::shared_ptr<TAPN::TimedArcPetriNet>& tapn;
	NonStrictMarking& initialMarking;
	AST::Query* query;
	VerificationOptions options;
	TimeDartSuccessorGenerator successorGenerator;
	vector<const TAPN::TimedTransition*> allwaysEnabled;
	int exploredMarkings;
public:
	void printStats();
	void GetTrace();
private:
	NonStrictMarking* lastMarking;
	google::sparse_hash_map<NonStrictMarking*, TraceList > trace;
};

} /* namespace DiscreteVerification */
} /* namespace VerifyTAPN */
#endif /* NONSTRICTSEARCH_HPP_ */
