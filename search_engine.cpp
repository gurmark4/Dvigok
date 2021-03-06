// search_engine.cpp: определяет точку входа для приложения.
//

#include "search_engine.h"

#include "thdocs.h"

#include "ConverterJSON.h"

#include "InvertedIndex.h"
#include "SearchServer.h"
#include "mTEST.h"

using namespace N;

using namespace std;
#if defined(_WIN64)
#define PLATFORM_NAME "x64" // Windows
#elif defined(_WIN32)
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
