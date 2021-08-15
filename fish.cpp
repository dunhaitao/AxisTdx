#include "pch.h"
#include <iostream>
#include <list>
#include "fish.h"
#include "ta_libc.h"

typedef struct tagRunBar {
	TA_Real price = 0;
	TA_Real dif = 0;
	TA_Real dea = 0;
	int cross = 0;
	int day_idx = 0;
	int run_idx = 0;
} RUN_BAR, * P_RUN_BAR;

typedef std::list<P_RUN_BAR> BAR_LIST, * P_BAR_LIST;

void translate(int DataLen, float* pfINa, BAR_LIST& bar_list, int circle);

void progress_0(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
	TA_Initialize();
	BAR_LIST bar_list;
	int circle = (int)roundf(*pfINc);
	translate(DataLen, pfINa, bar_list, circle);
	int bar_count = bar_list.size();
	if (bar_count > 0) {
		int bar_idx = 0;
		TA_Real* price_array = new TA_Real[bar_count];
		//
		TA_Real* dif_short = new TA_Real[bar_count];
		TA_Real* dif_long = new TA_Real[bar_count];
		for (BAR_LIST::iterator iter = bar_list.begin(); iter != bar_list.end(); iter++, bar_idx++) {
			price_array[bar_idx] = ((*iter)->price);
			std::cout << "nbar_idx:" << bar_idx << ", price:" << price_array[bar_idx] << std::endl;
		}
		int outBeginIdx_short = 0, outEBEle_short = 0, outBeginIdx_long = 0, outEBEle_long = 0;
		memset(dif_short, 0, sizeof(TA_Real) * bar_count);
		memset(dif_long, 0, sizeof(TA_Real) * bar_count);
		TA_EMA(0, bar_count - 1, price_array, 12, &outBeginIdx_short, &outEBEle_short, dif_short);
		TA_EMA(0, bar_count - 1, price_array, 26, &outBeginIdx_long, &outEBEle_long, dif_long);
		bar_idx = 0;
		memset(price_array, 0, sizeof(TA_Real) * bar_count);
		for (BAR_LIST::iterator iter = bar_list.begin(); iter != bar_list.end(); iter++, bar_idx++) {
			(*iter)->dif = 0;
			if ((bar_idx < outBeginIdx_short) || (bar_idx < outBeginIdx_long)) {
				continue;
			}
			price_array[bar_idx - outBeginIdx_long] = dif_short[bar_idx - outBeginIdx_short] - dif_long[bar_idx - outBeginIdx_long];
			(*iter)->dif = price_array[bar_idx - outBeginIdx_long];
		}
		memset(dif_short, 0, sizeof(TA_Real) * bar_count);
		memset(dif_long, 0, sizeof(TA_Real) * bar_count);
		TA_EMA(0, bar_idx - outBeginIdx_long - 1, price_array, 9, &outBeginIdx_short, &outEBEle_short, dif_short);
		std::cout << "outBeginIdx_short:" << outBeginIdx_short << std::endl;
		bar_idx = 0;
		int circle_idx = 0, last_day_idx = 0;
		for (BAR_LIST::iterator iter = bar_list.begin(); iter != bar_list.end(); iter++, bar_idx++) {
			(*iter)->cross = 0;
			if (bar_idx < (outBeginIdx_short + outBeginIdx_long)) {
				last_day_idx = (*iter)->run_idx;
				delete (*iter);
				continue;
			}
			dif_long[bar_idx] = dif_short[bar_idx - outBeginIdx_short - outBeginIdx_long];
			(*iter)->dea = dif_long[bar_idx];
			if (circle_idx > 0) {
				for (int iout = last_day_idx + 1; iout < (((*iter)->run_idx + 1)); iout++) {
					pfOUT[iout] = (*iter)->dif;
				}
			}
			last_day_idx = (*iter)->run_idx;
			circle_idx++;
			delete (*iter);
		}
		delete[] price_array;
		delete[] dif_short;
		delete[] dif_long;
	}
	TA_Shutdown();
}

void translate(int DataLen, float* pfINa, BAR_LIST& bar_list, int circle)
{
	int day_idx = 0;
	P_RUN_BAR p = NULL;
	for (int bar_idx = 0; bar_idx < DataLen; bar_idx++) {
		if (0 == (bar_idx % circle)) {
			p = new RUN_BAR;
			p->day_idx = day_idx;
			day_idx++;
			bar_list.push_back(p);
		}
		if (NULL != p) {
			p->price = (*(pfINa + bar_idx));
			p->run_idx = bar_idx;
		}
	}
}
