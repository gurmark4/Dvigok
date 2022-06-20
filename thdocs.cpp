
#include "search_engine.h"
#include "thdocs.h"
#include "InvertedIndex.h"

using namespace N;
using namespace std;

std::mutex freq_dictionary_mtx;

void thdocs(map<string, vector<Entry>>& freq_dictionary,
	const string& iss,
	const size_t i)
{
	std::stringstream s1;
	std::string s;
	Entry ee;
	vector<Entry> vee;
	ee.doc_id = i;
	ee.count = 1;
	vee.clear();
	vee.push_back(ee);
	s1.str(iss);
	while (s1 >> s)
	{
		int k1 = -1;
		for (map<string, vector<Entry>>::iterator it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it) {
			if (s == it->first) {
				k1 = 1;
				int k2 = -1;
				for (size_t j = 0; j < it->second.size(); j++) {
					if (i == it->second[j].doc_id) {
						k2 = 1;
						freq_dictionary_mtx.lock();
						it->second[j].count++;
						freq_dictionary_mtx.unlock();
						break;
					}
				}
				if (k2 < 0) {
					freq_dictionary_mtx.lock();
					it->second.push_back(ee);
					freq_dictionary_mtx.unlock();
				}
				break;
			}
		}
		if (k1 < 0) {
			freq_dictionary_mtx.lock();
			freq_dictionary[s] = vee;
			freq_dictionary_mtx.unlock();
		}
	}
}