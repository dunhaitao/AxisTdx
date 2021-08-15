#pragma once

#include <iostream>
#include <list>

typedef struct tag_bar {
	int idx = 0;
	int seg_idx = 0;
	float high = 0;
	float low = 0;
	short peak = 0;
	float r_high = 0;
	float r_low = 0;
	float point = 0;
	int line_idx = 0;
} BAR, * PBAR;

typedef struct tagLineCalculateResult {
	short code;
	short over;
	float point;
	PBAR p1;
	PBAR p2;
} LineCalculateResult, * LPLineCalculateResult;

typedef struct tagAxis {
	int idx_left;
	int idx_right;
	float gg;
	float dd;
	float zg;
	float zd;
} AXIS, * PAXIS;

typedef std::list< PBAR > PBAR_LIST, * LP_PBAR_LIST;
typedef std::list< PAXIS > PAXIS_LIST, * LP_PAXIS_LIST;

void line_make_line(PBAR in_array, const int max_length, float* pOut);
int line_if_contain(PBAR p1, PBAR p2);
int line_dir(PBAR p1, PBAR p2);
void line_if_peak(PBAR p1, PBAR p2, PBAR p3, LPLineCalculateResult lpResult);
void line_peak_cut(const int tidx, const PBAR p1, const PBAR p2, const PBAR p3, PBAR in_array, const int max_length, LP_PBAR_LIST p_peak_list);
void line_make_it(LP_PBAR_LIST inx_array, float* pOut);
void line_is_line_ver2(PBAR p0, PBAR p1, PBAR p2, PBAR p3, LPLineCalculateResult lpResult);
int line_is_pre_line(PBAR p1, PBAR p2);
void line_is_line(PBAR p1, PBAR p2, PBAR p3, LPLineCalculateResult lpResult);

void make_segment(PBAR in_array, const int max_length, float* pOut);
int segment_wait_check(int layer, int i_dir, PBAR p1, PBAR p3, PBAR p5, PBAR p6);
int segment_is_peak(int layer, int i_dir, PBAR p1, PBAR p2, PBAR p3, PBAR p4, PBAR p5, PBAR p6);
int segment_wait_check_2(int layer, int i_dir, PBAR p3, PBAR p4, PBAR p5, PBAR p6);
int segment_is_contain(int layer, int i_dir, PBAR p1, PBAR p2, PBAR p3, PBAR p4);
void segment_if_segment(int layer, PBAR p1, PBAR p2, PBAR p3, PBAR p4, LPLineCalculateResult lpResult);

void make_axis_gg(PBAR in_array, const int max_count, float* pOut);
void make_axis_dd(PBAR in_array, const int max_count, float* pOut);
int axis_is_axis(PBAR p1, PBAR p2, PBAR p3, PBAR p4, PBAR p5);
int axis_in_axis(PBAR p4, PBAR p5, PAXIS pAxis);

