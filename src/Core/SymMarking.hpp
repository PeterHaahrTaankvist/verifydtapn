#ifndef SYMMARKING_HPP_
#define SYMMARKING_HPP_

#include <vector>
#include "DiscretePart.hpp"
#include "TokenMapping.hpp"
#include "boost/smart_ptr.hpp"
#include "dbm/fed.h"

namespace VerifyTAPN {
	namespace TAPN {
		class TimedArcPetriNet;
		class TimedTransition;
		class TimeInterval;
	}

	// Class representing a symbolic marking.
	class SymMarking {
		public: // typedefs
			typedef std::vector<boost::shared_ptr<SymMarking> > Vector;

		public: // construction
			SymMarking(const DiscretePart & dp, const dbm::dbm_t & dbm);
			SymMarking(const SymMarking& marking) : dp(marking.dp), dbm(marking.dbm), mapping(marking.mapping) { };
			virtual ~SymMarking() { };

			SymMarking* clone() const { return new SymMarking(*this); }

		public: // Inspectors
			const DiscretePart* GetDiscretePart() const;
			const int GetTokenPlacement(int tokenIndex) const { return dp.GetTokenPlacement(tokenIndex); }
			const dbm::dbm_t & Zone() const;
			const TokenMapping& GetTokenMapping() const;
			void GenerateDiscreteTransitionSuccessors(const VerifyTAPN::TAPN::TimedArcPetriNet & tapn, std::vector<SymMarking*>& succ) const;

		public: // Modifiers
			void Delay();
			void MoveToken(int tokenIndex, int newPlaceIndex);
			void MoveFirstTokenAtBottomTo(int newPlaceIndex);
			void ResetClock(int clockIndex);
			void AddActiveToken(int nAdditionalTokens);
			void RemoveInactiveToken(int nTokensToRemove);
			void Constrain(const int tokenIndex, const TAPN::TimeInterval& ti);


		private: // Initializers
			void initMapping();


		private: // Helper functions
			void GenerateDiscreteTransitionSuccessorFor(const TAPN::TimedTransition& t, std::vector<SymMarking*>& succ) const;

		private:
			DiscretePart dp;
			dbm::dbm_t dbm;
			TokenMapping mapping;

	};

}

#endif /* SYMMARKING_HPP_ */