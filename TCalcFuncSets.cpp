#include "pch.h"
#include "framework.h"
#include "TCalcFuncSets.h"
#include "axis_define.h"
#include "fish.h"

//生成的dll及相关依赖dll请拷贝到通达信安装目录的T0002/dlls/下面,再在公式管理器进行绑定
// make line
void TestPlugin1(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
	PBAR bar_array = new BAR[DataLen];
	for (int i = 0; i < DataLen; i++) {
		bar_array[i].idx = i;
		bar_array[i].high = pfINa[i];
		bar_array[i].low = pfINb[i];
		bar_array[i].r_high = pfINa[i];
		bar_array[i].r_low = pfINb[i];
		pfOUT[i] = 0;
	}
	line_make_line(bar_array, DataLen, pfOUT);
	delete[] bar_array;
}

void TestPlugin2(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc)
{
	PBAR bar_array = new BAR[DataLen];
	int max_count = 0;
	for (int i = 0; i < DataLen; i++)
	{
		short peak = pfINc[i];
		pfOUT[i] = 0;
		if (0 != peak) {
			bar_array[max_count].idx = i;
			bar_array[max_count].high = pfINa[i];
			bar_array[max_count].low = pfINb[i];
			bar_array[max_count].r_high = pfINa[i];
			bar_array[max_count].r_low = pfINb[i];
			bar_array[max_count].peak = peak;
			bar_array[max_count].point = (peak > 0) ? pfINa[i] : pfINb[i];
			max_count++;
		}
	}
	make_segment(bar_array, max_count, pfOUT);
	delete[] bar_array;
}

void TestPlugin3(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc) {
	PBAR bar_array = new BAR[DataLen];
	int max_count = 0;
	for (int i = 0; i < DataLen; i++) {
		short peak = pfINc[i];
		//pfOUT[i] = -2;
		if (0 != peak) {
			bar_array[max_count].idx = i;
			bar_array[max_count].high = pfINa[i];
			bar_array[max_count].low = pfINb[i];
			bar_array[max_count].r_high = pfINa[i];
			bar_array[max_count].r_low = pfINb[i];
			bar_array[max_count].peak = peak;
			bar_array[max_count].point = (peak > 0) ? pfINa[i] : pfINb[i];
			max_count++;
		}
	}
	make_axis_gg(bar_array, max_count, pfOUT);
	delete[] bar_array;
}

void TestPlugin4(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc) {
	PBAR bar_array = new BAR[DataLen];
	int max_count = 0;
	for (int i = 0; i < DataLen; i++) {
		short peak = pfINc[i];
		//pfOUT[i] = -1;
		if (0 != peak) {
			bar_array[max_count].idx = i;
			bar_array[max_count].high = pfINa[i];
			bar_array[max_count].low = pfINb[i];
			bar_array[max_count].r_high = pfINa[i];
			bar_array[max_count].r_low = pfINb[i];
			bar_array[max_count].peak = peak;
			bar_array[max_count].point = (peak > 0) ? pfINa[i] : pfINb[i];
			max_count++;
		}
	}
	make_axis_dd(bar_array, max_count, pfOUT);
	delete[] bar_array;
}
void TestPlugin5(int DataLen, float* pfOUT, float* pfINa, float* pfINb, float* pfINc) {
	progress_0(DataLen, pfOUT, pfINa, pfINb, pfINc);
}

//加载的函数
PluginTCalcFuncInfo g_CalcFuncSets[] = 
{
	{1,(pPluginFUNC)&TestPlugin1},
	{2,(pPluginFUNC)&TestPlugin2},
	{3,(pPluginFUNC)&TestPlugin3},
	{4,(pPluginFUNC)&TestPlugin4},
	{5,(pPluginFUNC)&TestPlugin5},
	{0,NULL},
};

//导出给TCalc的注册函数
BOOL RegisterTdxFunc(PluginTCalcFuncInfo** pFun)
{
	if(*pFun==NULL)
	{
		(*pFun)=g_CalcFuncSets;
		return TRUE;
	}
	return FALSE;
}
