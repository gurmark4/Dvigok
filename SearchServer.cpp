
#include "search_engine.h"
#include "SearchServer.h"

using namespace N;

using namespace std;


InvertedIndex* idx;
InvertedIndex _index;
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)
{
	std::vector<string> nm_requests;
	std::vector<std::vector<Entry>> freq_requests;
	std::vector<std::vector<RelativeIndex>> ans;
	std::vector<RelativeIndex> req_ans;
	RelativeIndex ri;
	std::map<std::string, int> uniq_word; // частотный	словарь
	std::map<size_t, size_t> abs_Index;

	string s;
	for (int i = 0; i < queries_input.size(); i++) {
		nm_requests.push_back("request00" + std::to_string(i + 1));
		//				cout <<  nm_requests[i] << " !!!\n";
		abs_Index.insert(pair<int, int>(i, 0));
		std::istringstream iss(queries_input[i], std::istringstream::in);
		//		cout << queries_input[i].size() << "  " << i << " queries_input[i].size()\n";
		uniq_word.clear();
		while (iss >> s)
		{
			uniq_word.insert(pair<string, int>(s, i));
		}
		//		cout << uniq_word.size() << "  " << i << " uniq_word.size() !\n";
		freq_requests.clear();
		for (map<string, int>::iterator it = uniq_word.begin(); it != uniq_word.end(); ++it) {
			freq_requests.push_back(_index.GetWordCount(it->first));
			//		cout << it->first << "  " << it->second << " uniq_word ===\n";
		}
		//		cout << freq_requests.size() << " freq_requests.size() !!\n";
		abs_Index.clear();
		for (int k = 0; k < freq_requests.size(); k++) {
			//			cout << freq_requests[k].size() << " " << k << " freq_requests[k].size() !!!\n";
			for (int j = 0; j < freq_requests[k].size(); j++) {
				//				cout << freq_requests[k][j].doc_id << " " << freq_requests[k][j].count << " \n";
				int l = -1;
				for (map<size_t, size_t>::iterator it = abs_Index.begin(); it != abs_Index.end(); ++it)
					if (freq_requests[k][j].doc_id == it->first) { it->second += freq_requests[k][j].count;	l = 1; }
				if (l < 0) abs_Index.insert(pair<size_t, size_t>(freq_requests[k][j].doc_id, freq_requests[k][j].count));
			}
		}

		float rmax = 0.;
		for (map<size_t, size_t>::iterator it = abs_Index.begin(); it != abs_Index.end(); ++it)
		{
			//		cout << it->first << " " << it->second << "                        abs_Index\n";
			if (float(it->second) > rmax) rmax = float(it->second);
		}

		//	size_t value_comparer;
		std::vector<std::pair<size_t, size_t>> items;
		items.clear();
		for (map<size_t, size_t>::iterator it = abs_Index.begin(); it != abs_Index.end(); ++it)
		{
			items.push_back(make_pair(it->second, it->first));
		}
		//sort by value using std::sort
		//std::sort(items.begin(), items.end());
		abs_Index.clear();
		for (int k = 0; k < items.size(); k++) {
			abs_Index.insert(pair<size_t, size_t>(items[k]));
		}

		req_ans.clear();
		for (map<size_t, size_t>::iterator it = abs_Index.begin(); it != abs_Index.end(); ++it)
		{
			//	cout << it->first << " " << it->second << "                        abs_Index\n";
			ri.doc_id = it->second;
			if (rmax > 0.1) ri.rank = it->first / rmax;
			req_ans.push_back(ri);
		}
		ans.push_back(req_ans);
		/*		for (int k = 0; k < req_ans.size(); k++) {
					cout << req_ans[k].doc_id << " " << req_ans[k].rank << "   req_ans[k]\n";
				}*/
	}
	return ans;
};