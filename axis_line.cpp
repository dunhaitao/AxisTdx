#include "pch.h"
#include "axis_define.h"

void line_make_line(PBAR in_array, const int max_length, float* pOut)
{
    int i_ready = 0;
    PBAR p1 = NULL;
    PBAR p2 = NULL;
    PBAR p3 = NULL;
    int idx = 3;
    if (max_length > 3) {
        int i_contain = 0;
        int i_dir = 0;
        p1 = in_array + 0;
        p2 = in_array + 1;
        p3 = in_array + 2;
        PBAR p4 = NULL;
        // 判断出现方向
        while (idx < max_length) {
            p4 = (in_array + idx);
            i_contain = line_if_contain(p1, p2);
            if (0 == i_contain) {
                break;
            }
            else {
                p1 = p2;
                p2 = p3;
                p3 = p4;
            }
            idx++;
        }
        while (idx < max_length) {
            p4 = (in_array + idx);
            i_contain = line_if_contain(p2, p3);
            if (0 != i_contain) {
                i_dir = line_dir(p1, p2);
                if (i_contain > 0) {
                    if (i_dir > 0) {
                        p2->r_low = p3->r_low;
                    }
                    else if (i_dir < 0) {
                        p2->r_high = p3->r_high;
                    }
                    else {
                        break;
                    }
                }
                else if (i_contain < 0) {
                    if (i_dir > 0) {
                        p3->r_low = p2->r_low;
                    }
                    else if (i_dir < 0) {
                        p3->r_high = p2->r_high;
                    }
                    else {
                        break;
                    }
                    p2 = p3;
                }
                else {
                    break;
                }
                p3 = p4;
            }
            else {
                i_ready = 1;
                break;
            }
            idx++;
        }
    }
    if (i_ready > 0) {
        PBAR_LIST peak_list;
        line_peak_cut(idx, p1, p2, p3, in_array, max_length, &peak_list);
        if (peak_list.size() > 0) {
            line_make_it(&peak_list, pOut);
        }
    }
}

void line_make_it(LP_PBAR_LIST inx_array, float* pOut)
{
    PBAR_LIST::iterator iter = inx_array->begin();
    PBAR_LIST line_dot_list;
    if (inx_array->size() > 2) {
        PBAR p0 = NULL;
        PBAR p1 = *iter;
        iter++;
        PBAR p2 = *iter;
        iter++;
        PBAR p3 = NULL;
        int i_line = 0;
        while (iter!=inx_array->end()) {
            p3 = *iter; iter++;
            i_line = line_is_pre_line(p1, p2);
            if (0 == i_line) {
                p2 = p3;
            }
            else {
                break;
            }
        }
        while (iter != inx_array->end()) {
            p3 = *iter; iter++;
            LineCalculateResult lineCalculateResult;
            line_is_line(p1, p2, p3, &lineCalculateResult);
            if (0 != lineCalculateResult.code) {
                if (0 != lineCalculateResult.over) {
                    p2->peak = 0;
                }
                else {
                    line_dot_list.push_back(p1);
                }
                p1 = lineCalculateResult.p1;
                p2 = lineCalculateResult.p2;
            }
            else {
                size_t line_size = line_dot_list.size();
                if (line_size > 0) {
                    p0 = line_dot_list.back();
                    line_is_line_ver2(p0, p1, p2, p3, &lineCalculateResult);
                    if (0 != lineCalculateResult.code) {
                        if (abs(lineCalculateResult.over) > 1) {
                            p1->peak = 0;
                            p2->peak = 0;
                            p1 = line_dot_list.back();
                            line_dot_list.pop_back();
                            p2 = lineCalculateResult.p2;
                        }
                    }
                }
            }
        }
        line_dot_list.push_back(p1);
        line_dot_list.push_back(p2);
        //
        for (iter = line_dot_list.begin(); iter != line_dot_list.end(); iter++) {
            pOut[(*iter)->idx] = (*iter)->peak;
        }
    }
}

void line_peak_cut(const int tidx, const PBAR p1, const PBAR p2, const PBAR p3, PBAR in_array, const int max_length, LP_PBAR_LIST p_peak_list)
{
    int ridx = 0;
    int i_contain = 0;
    int i_dir = 0;
    PBAR tmp_p1 = p1;
    PBAR tmp_p2 = p2;
    PBAR tmp_p3 = p3;
    int idx = tidx;
    while (idx < max_length) {
        PBAR tmp_p4 = (in_array + idx);
        i_contain = line_if_contain(tmp_p3, tmp_p4);
        if (0 != i_contain) {
            i_dir = line_dir(tmp_p2, tmp_p3);
            if (i_contain > 0) {
                if (i_dir > 0) {
                    tmp_p3->r_low = tmp_p4->r_low;
                }
                else if (i_dir < 0) {
                    tmp_p3->r_high = tmp_p4->r_high;
                }
                else {
                    break;
                }
            }
            else if (i_contain < 0) {
                if (i_dir > 0) {
                    tmp_p4->r_low = tmp_p3->r_low;
                }
                else if (i_dir < 0) {
                    tmp_p4->r_high = tmp_p3->r_high;
                }
                else {
                    break;
                }
                tmp_p3 = tmp_p4;
            }
            else {
                break;
            }
        }
        else {
            LineCalculateResult lineCalculateResult;
            line_if_peak(tmp_p1, tmp_p2, tmp_p3, &lineCalculateResult);
            if (0 != lineCalculateResult.code) {
                tmp_p2->point = lineCalculateResult.point;
                tmp_p2->line_idx = ridx;
                tmp_p2->peak = lineCalculateResult.code;
                p_peak_list->push_back(tmp_p2);
                ridx++;
            }
            tmp_p1 = tmp_p2;
            tmp_p2 = tmp_p3;
            tmp_p3 = tmp_p4;
        }
        idx++;
    }
}

int line_if_contain(PBAR p1, PBAR p2)
{
    int result = 0;
    if (p1->r_high <= p2->r_high && p1->r_low >= p2->r_low) {
        result = -1;
    }
    else if (p1->r_high >= p2->r_high && p1->r_low <= p2->r_low) {
        result = 1;
    }
    return result;
}

int line_dir(PBAR p1, PBAR p2)
{
    int result = 0;
    if (p1->r_high > p2->r_high && p1->r_low > p2->r_low) {
        result = -1;
    }
    else if (p1->r_high < p2->r_high && p1->r_low < p2->r_low) {
        result = 1;
    }
    return result;
}

void line_if_peak(PBAR p1, PBAR p2, PBAR p3, LPLineCalculateResult lpResult)
{
    lpResult->code = 0;
    lpResult->point = 0;
    if (p1->r_high > p2->r_high && p3->r_high > p2->r_high && p1->r_low > p2->r_low && p3->r_low > p2->r_low) {
        lpResult->code = -1;
        lpResult->point = p2->r_low;
    }
    else if (p1->r_high < p2->r_high && p3->r_high < p2->r_high && p1->r_low < p2->r_low && p3->r_low < p2->r_low) {
        lpResult->code = 1;
        lpResult->point = p2->r_high;
    }
}

void line_is_line_ver2(PBAR p0, PBAR p1, PBAR p2, PBAR p3, LPLineCalculateResult lpResult)
{
    lpResult->code = 0;
    lpResult->over = 0;
    if ((p2->peak > 0 && p3->peak < 0) && (p2->r_high > p3->r_high && p2->r_low > p3->r_low)) {
        if ((p3->idx - p2->idx) > 3) {
            lpResult->code = -1;
            lpResult->p1 = p2;
            lpResult->p2 = p3;
        }
        else {
            if ((p3->r_low < p1->r_low) && (p2->r_high < p0->r_high)) {
                lpResult->code = -1;
                lpResult->p1 = p0;
                lpResult->p2 = p3;
                lpResult->over = -2;
            }
        }
    }
    else if ((p2->peak < 0 && p3->peak > 0) && (p2->r_high < p3->r_high && p2->r_low < p3->r_low)) {
        if ((p3->idx - p2->idx) > 3) {
            lpResult->code = 1;
            lpResult->p1 = p2;
            lpResult->p2 = p3;
        }
        else {
            if ((p3->r_high > p1->r_high) && (p2->r_low > p0->r_low)) {
                lpResult->code = 1;
                lpResult->p1 = p0;
                lpResult->p2 = p3;
                lpResult->over = 2;
            }
        }
    }
    else if ((p2->peak > 0 && p3->peak > 0) && (p2->r_high < p3->r_high)) {
        lpResult->code = 1;
        lpResult->over = 1;
        lpResult->p1 = p1;
        lpResult->p2 = p3;
    }
    else if ((p2->peak < 0 && p3->peak < 0) && (p2->r_low > p3->r_low)) {
        lpResult->code = -1;
        lpResult->over = -1;
        lpResult->p1 = p1;
        lpResult->p2 = p3;
    }
}

int line_is_pre_line(PBAR p1, PBAR p2)
{
    int result = 0;
    if (p2->peak > 0 && p1->peak  < 0 && p2->r_high > p1->r_high && p2->r_low > p1->r_low) {
        if ((p2->idx - p1->idx) > 3) {
            result = 1;
        }
    }
    else if (p2->peak < 0 && p1->peak > 0 && p2->r_high < p1->r_high && p2->r_low < p1->r_low) {
        if ((p2->idx - p1->idx) > 3) {
            result = -1;
        }
    }
    return result;
}

void line_is_line(PBAR p1, PBAR p2, PBAR p3, LPLineCalculateResult lpResult)
{
    lpResult->code = 0;
    lpResult->over = 0;
    if ((p2->peak > 0 && p3->peak < 0) && (p2->r_high > p3->r_high && p2->r_low > p3->r_low)) {
        if ((p3->idx - p2->idx) > 3) {
            lpResult->code = -1;
            lpResult->p1 = p2;
            lpResult->p2 = p3;
        }
    }
    else if ((p2->peak < 0 && p3->peak > 0) && (p2->r_high < p3->r_high && p2->r_low < p3->r_low)) {
        if ((p3->idx - p2->idx) > 3) {
            lpResult->code = 1;
            lpResult->p1 = p2;
            lpResult->p2 = p3;
        }
    }
    else if ((p2->peak > 0 && p3->peak > 0) && (p2->r_high < p3->r_high)) {
        lpResult->code = 1;
        lpResult->over = 1;
        lpResult->p1 = p1;
        lpResult->p2 = p3;
    }
    else if ((p2->peak < 0 && p3->peak < 0) && (p2->r_low > p3->r_low)) {
        lpResult->code = -1;
        lpResult->over = -1;
        lpResult->p1 = p1;
        lpResult->p2 = p3;
    }
}
