#ifndef SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_
#define SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_
#include "../PQL/MatcherPQL.h"
#include <iterator>
#include <sstream>
#include <map>
#include <utility>

#include "../PKB/API/PKBApi.h"
#ifndef _GLIBCXX_VECTOR
#include <vector>
#endif
#ifndef SRC_TREE_FIELD_H_
#include "../PQL/Field.h"
#endif
#ifndef SRC_PQL_PQLTREE_H_
#include "../PQL/PQLTree.h"
#endif

namespace std {
class QueryEvaluator {
public:
	QueryEvaluator();
	virtual ~QueryEvaluator();
	vector<string> getWynik(PQLTree* tree);
	vector<int> getModifiesWynik(Field* field1, Field* field2,
			vector<int> lines, string selectValue);
	vector<int> getUsesWynik(Field* field1, Field* field2, vector<int> lines,
			string selectValue);
	vector<int> getFollowsWynik(Field* field1, Field* field2,
			vector<int> lines, string selectValue);
	vector<int> getParentWynik(Field* field1, Field* field2, vector<int> lines,
			string selectValue);
	vector<int> getUsesSWynik(Field* field1, Field* field2, vector<int> lines,
			string selectValue);
	vector<int> getFollowsSWynik(Field* field1, Field* field2,
			vector<int> lines, string selectValue);
	vector<int> getParentSWynik(Field* field1, Field* field2,
			vector<int> lines, string selectValue);
	void getWithWynik(Field* field1, Field* field2,
				vector<int> lines);
private:
	PKBApi *pkbApi = new PKBApi();
	map<string,vector<int>> withMap;
	set<int> cutSetLines(string fieldValue, set<int> setLines);
	string wynikType;

	set<std::pair<string,int>> usesPairs;
	bool pierwszyUses;

	void cutUsesPairs(vector<int> lines);
};
} /* namespace std */

#endif /* SRC_QUERYEVALUATOR_QUERYEVALUATOR_H_ */
