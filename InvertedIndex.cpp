
#include "search_engine.h"

#include "thdocs.h"

#include "InvertedIndex.h"

//using namespace N;

using namespace std;

	void InvertedIndex::UpdateDocumentBase(std::vector<std::string> input_docs)
	{
		vector<string>  docs;
		std::string s;
		std::vector<std::thread> fls;

		docs = input_docs;
		freq_dictionary.clear();
		for (size_t i = 0; i < docs.size(); i++) {
			std::istringstream iss(docs[i], std::istringstream::in);
			s = iss.str();
			fls.push_back(thread(thdocs, ref(freq_dictionary), s, i));
			if (fls[i].joinable()) fls[i].join();

			//	std::reverse(begin(wordsVector), end(wordsVector));
		}
	};
	/* ����� ���������� ���������� ��������� ����� word � ����������� ���� ����������
* @param word �����, ������� ��������� �������� ���������� ����������
* @return ���������� �������������� ������ � �������� ����
*/
	std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word)
	{
		std::vector<Entry> freq_requests; // ������ ��������
		for (map<string, vector<Entry>>::iterator it = freq_dictionary.begin(); it != freq_dictionary.end(); ++it)
			if (word == it->first) freq_requests = it->second;
		return freq_requests;
	};
//private:
//	std::vector<std::string> docs; // ������ ����������� ����������
//	std::map<std::string, std::vector<Entry>> freq_dictionary; // ���������	�������
//};
