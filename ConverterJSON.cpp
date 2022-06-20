
#include "search_engine.h"
#include "ConverterJSON.h"

using namespace N;
using namespace std;


		/**
		* Метод получения содержимого файлов
		* @return Возвращает список с содержимым файлов перечисленных в config.json
		*/
		std::vector<std::string> ConverterJSON::GetTextDocuments()
		{
			vector<string>  fnames, docs;
			nlohmann::json dict;
			std::stringstream s1;
			//	std::string path, s;
			std::ifstream file_config("config.json");
			if (file_config.fail())
			{
				cout << "config file is missing.\n";
				exit(1);
			}
			file_config >> dict;
			file_config.close();
			//========================================================
			std::string path = "", version, prj_path, s;
			size_t i, k;

	    	char current_work_dir[FILENAME_MAX];
			if (_getcwd(current_work_dir, sizeof(current_work_dir)) != NULL) {
				printf("Current working directory : %s\n", current_work_dir);
			}
			else {
				perror("getcwd() error");
				//return 1;
			}

			for (i = 0; i < FILENAME_MAX; i++) {
				if (current_work_dir[i] == 92) current_work_dir[i] = 47;
				if (current_work_dir[i] > 0 && current_work_dir[i] < 256) path += current_work_dir[i];
			}


			std::cout << path << "\n";

			string prj_name = dict["config"]["name"];
			if (prj_name == "")
			{
				cout << "config file is empty\n";
				exit(1);
			}
			else cout << "Project name: " << prj_name << "\n";
			version = dict["config"]["version"];
			//	cout << "Version:   " << version << " \n";
			k = path.rfind(prj_name);
			for (i = 0; i < (k + prj_name.length()); i++) 		prj_path += path[i];
			prj_path += 47;
			prj_path += "CMakeLists.txt";
			cout << "Project path: " << prj_path << "\n";

			file_config.open(prj_path);
			if (!file_config)
			{
				std::cout << "Файл CMakeLists.txt не открыт\n\n";
			}
			//		else    std::cout << "Ok! File open\n\n";
			while (!file_config.eof()) {
				getline(file_config, s);
				if (s.rfind("project") >= 0) {
					if (s.rfind(version) < 0) cout << "Version: " << version << " config.json has incorrect file version\n";
					else cout << "Version: " << version << " config.json has correct file version\n";
					break;
				}
			}
			file_config.close();
			//=============================================================

			for (auto n : dict["files"]) {
				//		cout << n << " \n";
				fnames.push_back(n);
			}

			std::ifstream fl;
			for (int i = 0; i < fnames.size(); i++) {
				s1 = std::stringstream();
				//			cout << fnames[i] << " \n";
				path = fnames[i];
				fl.open(path);
				if (!fl)
				{
					std::cout << "Файл не открыт\n\n";
				}
				//		else    std::cout << "Ok! File open\n\n";
				while (!fl.eof()) {
					getline(fl, s);
					s1 << s << " ";
				}
				fl.close();
				s = s1.str();
				//	std::cout << s << "  i= " << i << "  \n";
				docs.push_back(s);
			}
			//=============================
			return docs;
		};
		/**
		* Метод считывает поле max_responses для определения предельного
		* количества ответов на один запрос
		* @return
		*/
		int ConverterJSON::GetResponsesLimit()
		{
			nlohmann::json dict;
			std::ifstream file_config("config.json");
			if (file_config.fail())
			{
				cout << "config file is missing.\n";
				exit(1);
			}
			file_config >> dict;
			file_config.close();;

			return dict["config"]["max_responses"];
			//	return atoi(dict["config"]["max_responses"].c_str());
			//	return atoi(val_config[0].c_str());
		}
		int ConverterJSON::GetTimeUpdate()
		{
			nlohmann::json dict;
			std::ifstream file_config("config.json");
			if (file_config.fail())
			{
				cout << "config file is missing.\n";
				exit(1);
			}
			file_config >> dict;
			file_config.close();

			return dict["config"]["time_update"];
		}
		/**
		* Метод получения запросов из файла requests.json
		* @return возвращает список запросов из файла requests.json
		*/
		std::vector<std::string> ConverterJSON::GetRequests()
		{
			vector<string>  requests;
			//	std::stringstream s1;
			//	std::string path, s;
			nlohmann::json dict;
			std::ifstream file_request("requests.json");
			if (file_request.fail())
			{
				cout << "request file is missing.\n";
				exit(1);
			}
			else cout << "request file is open\n";
			file_request >> dict;
			file_request.close();
			for (auto n : dict["requests"]) {
				//		cout << n << " \n";
				requests.push_back(n);
			}
			return requests;
		};
		/**
		* Положить в файл answers.json результаты поисковых запросов
		*/
		void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>>	answers)
		{
			nlohmann::json dict, dict1;
			std::string path = "";
			ofstream fljs("answers.json");
			dict = "answers";
			fljs << "{" << endl << "     " << dict << ":{" << endl;
			int max_responses = GetResponsesLimit();
			size_t m;
			for (size_t i = 0; i < answers.size(); i++) {
				path = "request00" + to_string(i + 1);
				dict = path;
				fljs << endl << "        " << dict << ":{" << endl;
				if (answers[i].size() == 0) {
					dict = "result";
					dict1 = "false";
					fljs << "             " << dict << ":" << dict1 << endl << "}" << endl;
				}
				else {
					dict = "result";
					dict1 = "true";
					fljs << "             " << dict << ":" << dict1 << "," << endl;
					dict = "relevance";
					fljs << "             " << dict << ":{" << endl;
					if (max_responses < answers[i].size()) m = answers[i].size() - max_responses;
					else m = 0;
					dict = "docid";
					dict1 = "rank";
					size_t j = answers[i].size();
				mmm: j--;

					//for (size_t j = answers[i].size() - 1; j >= m; j--) {
					//	std::cout << j << "   " << m << "   j   m\n";
						if (j > m) fljs << "             " << dict << ":" << answers[i][j].doc_id << ", " << dict1 << ":" << answers[i][j].rank << "," << endl;
						else fljs << "             " << dict << ":" << answers[i][j].doc_id << ", " << dict1 << ":" << answers[i][j].rank << endl;
					//}
						if (j > m) goto mmm;
					fljs << "              }" << endl;
				}
				if (i < answers.size() - 1) fljs << "     }," << endl;
				else fljs << "     }" << endl;
			}
			fljs << "}" << endl;
			fljs.close();
		};

