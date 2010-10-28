#ifndef SEARCHSTRATEGY_HPP_
#define SEARCHSTRATEGY_HPP_

#include "WaitingList.hpp"
#include "PWList.hpp"
#include "../Core/QueryChecker.hpp"
#include "../Core/VerificationOptions.hpp"

namespace VerifyTAPN
{
	class PassedWaitingList;
	class SymMarking;

	namespace TAPN{
		class TimedArcPetriNet;
	}

	namespace AST{
		class Query;
	}

	class SearchStrategy
	{
	public:
		virtual ~SearchStrategy() { };
		virtual bool Verify() = 0;
		virtual Stats GetStats() const = 0;
	};



	class DFS : public SearchStrategy
	{
	public:
		DFS(
			const VerifyTAPN::TAPN::TimedArcPetriNet& tapn,
			SymMarking* initialMarking,
			const AST::Query* query,
			int kBound
		);
		virtual ~DFS() { delete pwList; delete[] maxConstantsArray; };
		virtual bool Verify();
		virtual Stats GetStats() const;
	private:
		virtual bool CheckQuery(const SymMarking& marking) const;
		void PrintDiagnostics(size_t successors) const;
	private:
		PassedWaitingList* pwList;
		const VerifyTAPN::TAPN::TimedArcPetriNet& tapn;
		SymMarking* initialMarking;
		const QueryChecker checker;
		int kBound;
		int* maxConstantsArray;
	};
}

#endif /* SEARCHSTRATEGY_HPP_ */
