// search_engine.cpp: определяет точку входа для приложения.
//

#include "search_engine.h"

#include "thdocs.h"

#include "ConverterJSON.h"

#include "InvertedIndex.h"
#include "SearchServer.h"
//class SearchServer(InvertedIndex);
using namespace N;

using namespace std;
#if defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows
#elif defined(_WIN64)
#define PLATFORM_NAME "windows" // Windows
#elif defined(__CYGWIN__) && !defined(_WIN32)
#define PLATFORM_NAME "windows" // Windows (Cygwin POSIX under Microsoft Window)
#elif defined(__ANDROID__)
#define PLATFORM_NAME "android" // Android (implies Linux, so it must come first)
#elif defined(__linux__)
#define PLATFORM_NAME "linux" // Debian, Ubuntu, Gentoo, Fedora, openSUSE, RedHat, Centos and other
#elif defined(__unix__) || !defined(__APPLE__) && defined(__MACH__)
#include <sys/param.h>
#if defined(BSD)
#define PLATFORM_NAME "bsd" // FreeBSD, NetBSD, OpenBSD, DragonFly BSD
#endif
#elif defined(__hpux)
#define PLATFORM_NAME "hp-ux" // HP-UX
#elif defined(_AIX)
#define PLATFORM_NAME "aix" // IBM AIX
#elif defined(__APPLE__) && defined(__MACH__) // Apple OSX and iOS (Darwin)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#define PLATFORM_NAME "ios" // Apple iOS
#elif TARGET_OS_IPHONE == 1
#define PLATFORM_NAME "ios" // Apple iOS
#elif TARGET_OS_MAC == 1
#define PLATFORM_NAME "osx" // Apple OSX
#endif
#elif defined(__sun) && defined(__SVR4)
#define PLATFORM_NAME "solaris" // Oracle Solaris, Open Indiana
#else
#define PLATFORM_NAME NULL
#endif

// Return a name of platform, if determined, otherwise - an empty string
const char* get_platform_name() {
	return (PLATFORM_NAME == NULL) ? "" : PLATFORM_NAME;
}

/*TEST(sample_test_case, sample_test)
{
	EXPECT_EQ(1, 1);
}*/
void TestInvertedIndexFunctionality(
	const vector<string>& docs,
	const vector<string>& requests,
	const std::vector<vector<Entry>>& expected)
{
	std::vector<std::vector<Entry>> result;
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);
	std::cout << "  TestInvertedIndexFunctionality 000 \n";
	for (auto& request : requests) {
		std::cout << request << "  request \n";
		std::vector<Entry> word_count = idx.GetWordCount(request);
		for (size_t i = 0; i < word_count.size(); i++) cout << word_count[i].doc_id << " " << word_count[i].count << "       word_count\n";
		result.push_back(word_count);
	};
	cout << result.size() << " " << expected.size() << "       result, expected\n";
	EXPECT_EQ(result, expected);
//	ASSERT_EQ(result, expected);
	//ASSERT_EQ(1, 1);
}
TEST(TestCaseInvertedIndex, TestBasic) {
	const vector<string> docs = {
	"london is the capital of great britain",
	"big ben is the nickname for the Great bell of the striking clock"
	};
	const vector<string> requests = { "london", "the" };
	const vector<vector<Entry>> expected = {
{
{0, 1}
}, {
{0, 1}, {1, 3}
}
	};
	std::cout << "  TestCaseInvertedIndex 111 \n";
	TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
	const vector<string> docs = {
	"milk milk milk milk water water water",
	"milk water water",
	"milk milk milk milk milk water water water water water",
	"Americano Cappuccino"
	};
	const vector<string> requests = { "milk", "water", "Cappuccino" };
	const vector<vector<Entry>> expected = {
	{
	{0, 4}, {1, 1}, {2, 5}
	}, {
	{0, 3}, {1, 2}, {2, 5}
	}, {
	{3, 1}
	}
	};
	std::cout << "  TestCaseInvertedIndex 222 \n";
	TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
	const vector<string> docs = {
	"a b c d e f g h i j k l",
	"statement"
	};
	const vector<string> requests = { "m", "statement" };
	const vector<vector<Entry>> expected = {
{
{}
}, {
{1, 1}
}
	};
	std::cout << "  TestCaseInvertedIndex 333 \n";
	TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseSearchServer, TestSimple) {
	const vector<string> docs = {
	"milk milk milk milk water water water",
	"milk water water",
	"milk milk milk milk milk water water water water water",
	"Americano Cappuccino"
	};
	const vector<string> request = { "milk water", "sugar" };
	const std::vector<vector<RelativeIndex>> expected = {
	{
	{2, 1},
	{0, 0.7},
	{1, 0.3}
	},
	{
	{}
	}
	};
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);
	SearchServer srv(idx);
	std::cout << " TestCaseSearchServer, TestSimple 444 \n";
	std::vector<vector<RelativeIndex>> result = srv.search(request);
	//	ConverterJSON* cjs = new ConverterJSON();
	//	cjs->putAnswers(result);
	ASSERT_EQ(result, expected);
}
TEST(TestCaseSearchServer, TestTop5) {
	const vector<string> docs = {
	"london is the capital of great britain",
	"paris is the capital of france",
	"berlin is the capital of germany",
	"rome is the capital of italy",
	"madrid is the capital of spain",
	"lisboa is the capital of portugal",
	"bern is the capital of switzerland",
	"moscow is the capital of russia",
	"kiev is the capital of ukraine",
	"minsk is the capital of belarus",
	"astana is the capital of kazakhstan",
	"beijing is the capital of china",
	"tokyo is the capital of japan",
	"bangkok is the capital of thailand",
	"welcome to moscow the capital of russia the third rome",
	"amsterdam is the capital of netherlands",
	"helsinki is the capital of finland",
	"oslo is the capital of norway",
	"stockholm is the capital of sweden",
"riga is the capital of latvia",
"tallinn is the capital of estonia",
"warsaw is the capital of poland"
	};
	const vector<string> request = { "moscow is the capital of russia" };
	const std::vector<vector<RelativeIndex>> expected = {
	{
	{7, 1},
	{14, 1},
	{0, 0.4},
	{1, 0.4},
	{2, 0.4}
	}
	};
	InvertedIndex idx;
	idx.UpdateDocumentBase(docs);
	SearchServer srv(idx);
	std::cout << " TestCaseSearchServer, TestTop5 555 \n";
	std::vector<vector<RelativeIndex>> result = srv.search(request);
	ASSERT_EQ(result, expected);
};

int main()
{
	ConverterJSON* cjs = new ConverterJSON();
	InvertedIndex* idx = new InvertedIndex();
	string s, prj_path = "", version;
	size_t max_responses;
	int time_update;
	time_t l, l0, n, min, sec;
	vector<string> docs, requests;
	std::vector<Entry> freq_requests;;
	std::vector<std::vector<RelativeIndex>> ans;
	setlocale(LC_ALL, "Russian");

	std::cout << (get_platform_name()) << "  \n";

	docs = cjs->GetTextDocuments();
	//	for (int i = 0; i < docs.size(); i++) std::cout << docs[i] << "  i= " << i << "  \n";
	max_responses = cjs->GetResponsesLimit();
	time_update = cjs->GetTimeUpdate();
	std::cout << "  max_responses: " << max_responses << "  \n";
	std::cout << "  time_update: " << time_update << "  \n";
	requests = cjs->GetRequests();
	//	for (int i = 0; i < requests.size(); i++) std::cout << requests[i] << "  i= " << i << "  requests\n";

	idx->UpdateDocumentBase(docs);
	SearchServer* ssrv = new SearchServer(*idx);
	ans = ssrv->search(requests);
	std::cout << "  ans.size():  " << ans.size() << " \n";
	cjs->putAnswers(ans);
	std::cout << "  ans.size():  " << ans.size() << " \n";
		testing::InitGoogleTest();
		int i = RUN_ALL_TESTS();

	l0 = time(nullptr);
	n = time_update;
	while (true) {
		l = time(nullptr);
		if (l > l0) {
			n += l0 - l;
			sec = n % 60;
			min = n / 60;
			l0 = l;
			if (min > 0) std::cout << min << " min " << sec << " sec \n";
			else std::cout << sec << " sec \n";
			if (n <= 0) {
				std::cout << " Update! \n";
				idx->UpdateDocumentBase(docs);
				SearchServer* ssrv = new SearchServer(*idx);
				ans = ssrv->search(requests);
				cjs->putAnswers(ans);
				n = time_update;
			}
		}
	}
	l = time(nullptr);

	return 0;
}
