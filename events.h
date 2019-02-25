#pragma once

#include <string>
using namespace std;

enum APPEVT {
	EVT_OUTPUT_LOG = EVT_EXTERNAL_BEGIN +10,
};

SEVENT_BEGIN(EventOutputLog, EVT_OUTPUT_LOG)
	int level;
	string filter;
	string log;
	unsigned __int64 time_;
SEVENT_END()
