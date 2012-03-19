/*
 * NonStrictMarking.cpp
 *
 *  Created on: 29/02/2012
 *      Author: MathiasGS
 */

#include "NonStrictMarking.hpp"
#include <iostream>

using namespace std;

namespace VerifyTAPN {
namespace DiscreteVerification {

NonStrictMarking::NonStrictMarking() {
	// TODO Auto-generated constructor stub

}

NonStrictMarking::NonStrictMarking(const std::vector<int>& v){
	int prevPlaceId = -1;
	for(std::vector<int>::const_iterator iter = v.begin(); iter != v.end(); iter++){
		if(*iter == prevPlaceId){
			Place& p = places.back();
			if(p.tokens.size() == 0){
				Token t(0,1);
				p.tokens.push_back(t);
			}else{
				p.tokens.begin()->add(1);
			}
		}else{
			Place p(*iter);
			Token t(0,1);
			p.tokens.push_back(t);
			places.push_back(p);
		}
		prevPlaceId = *iter;
	}
}

unsigned int NonStrictMarking::size(){
	int count = 0;
	for(PlaceList::iterator iter = places.begin(); iter != places.end(); iter++){
		for(TokenList::iterator it = iter->tokens.begin(); it != iter->tokens.end(); it++){
			count += it->getCount();
		}
	}
	return count;
}

int NonStrictMarking::NumberOfTokensInPlace(Place& place) const{
	int count = 0;
	for(TokenList::iterator it = place.tokens.begin(); it != place.tokens.end(); it++){
		count = count + it->getCount();
	}
	return count;
}

bool NonStrictMarking::RemoveToken(Place& place, Token& token){
	assert(token.getCount() > 0);
	if(token.getCount() > 1){
		token.remove(1);
		return true;
	}else{
		for(TokenList::iterator iter = place.tokens.begin(); iter != place.tokens.end(); iter++){
			if(iter->getAge() == token.getAge()){
				place.tokens.erase(iter);
				return true;
			}
		}
	}
	return false;
}

void NonStrictMarking::AddTokenInPlace(Place& place, Token& token){
	for(TokenList::iterator iter = place.tokens.begin(); iter != place.tokens.end(); iter++){
		if(iter->getAge() == token.getAge()){
			iter->add(token.getCount());
			return;
		}
	}
	place.tokens.push_back(token);
}

NonStrictMarking::~NonStrictMarking() {
	// TODO: Should we destruct something here? (places)
}

bool NonStrictMarking::equals(const NonStrictMarking &m1){
	if(m1.places.size() == 0) return false;
	if(m1.places.size() != places.size())	return false;

	// TODO: extensive equals - should we test more?

	return true;
}

std::ostream& operator<<(std::ostream& out, NonStrictMarking& x ) {
	for(PlaceList::iterator iter = x.places.begin(); iter != x.places.end(); iter++){
		out << "Place: " << iter->id << " - has tokens (age, count): ";
		for(TokenList::iterator it = iter->tokens.begin(); it != iter->tokens.end(); it++){
			out << "(" << it->getAge() << ", " << it->getCount() << ") ";
		}
		out << endl;
	}

	return out;
}

} /* namespace DiscreteVerification */
} /* namespace VerifyTAPN */