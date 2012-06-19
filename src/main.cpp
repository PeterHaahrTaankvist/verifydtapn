#include <iostream>
#include "boost/smart_ptr.hpp"
#include "Core/TAPNParser/TAPNXmlParser.hpp"
#include "Core/VerificationOptions.hpp"
#include "Core/ArgsParser.hpp"
#include "Core/QueryParser/UpwardClosedVisitor.hpp"
#include "Core/QueryParser/TAPNQueryParser.hpp"
#include "Core/QueryParser/NormalizationVisitor.hpp"
#include "Core/QueryParser/ToStringVisitor.hpp"
#include "Core/QueryParser/BadPlaceVisitor.hpp"
#include "Core/TAPN/TimedPlace.hpp"

#include "ReachabilityChecker/Trace/trace_exception.hpp"

#include "DiscreteVerification/DiscreteVerification.hpp"

using namespace std;
using namespace VerifyTAPN;
using namespace VerifyTAPN::TAPN;
using namespace boost;

int main(int argc, char* argv[])
{
	srand ( time(NULL) );

	ArgsParser parser;
	VerificationOptions options = parser.Parse(argc, argv);

	TAPNXmlParser modelParser;
	boost::shared_ptr<TAPN::TimedArcPetriNet> tapn;

	try{
		tapn = modelParser.Parse(options.GetInputFile());
	}catch(const std::string& e){
		std::cout << "There was an error parsing the model file: " << e << std::endl;
		return 1;
	}

	tapn->Initialize(options.GetUntimedPlacesEnabled(), options.GetGlobalMaxConstantsEnabled());

	std::vector<int> initialPlacement(modelParser.ParseMarking(options.GetInputFile(), *tapn));

	AST::Query* query;
	try{
		TAPNQueryParser queryParser(*tapn);
		queryParser.parse(options.QueryFile());
		query = queryParser.GetAST();
	}catch(...){
		std::cout << "There was an error parsing the query file." << std::endl;
		return 1;
	}

	if(query->GetQuantifier() == AST::EF || query->GetQuantifier() == AST::AG){
		tapn->removeOrphantedTransitions();
	}

	tapn->updatePlaceTypes(query);

	return DiscreteVerification::DiscreteVerification::run(tapn, initialPlacement, query, options);
}


