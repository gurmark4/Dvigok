#pragma once
#include "search_engine.h"

using namespace N;
using namespace std;


class ConverterJSON {
public:
	ConverterJSON() = default;
	/**
	* ����� ��������� ����������� ������
	* @return ���������� ������ � ���������� ������ ������������� � config.json
	*/
	std::vector<std::string> GetTextDocuments();
	/**
	* ����� ��������� ���� max_responses ��� ����������� �����������
	* ���������� ������� �� ���� ������
	* @return
	*/
int GetResponsesLimit();
int GetTimeUpdate();
	/**
	* ����� ��������� �������� �� ����� requests.json
	* @return ���������� ������ �������� �� ����� requests.json
	*/
	std::vector<std::string> GetRequests();
	/**
	* �������� � ���� answers.json ���������� ��������� ��������
	*/
	void putAnswers(std::vector<std::vector<RelativeIndex>>	answers);
};