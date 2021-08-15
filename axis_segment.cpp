#include "pch.h"
#include "axis_define.h"


void make_axis_gg(PBAR in_array, const int max_count, float* pOut) {
    PAXIS_LIST axis_list;
    if (max_count > 4) {
        PBAR p1 = NULL;
        PBAR p2 = NULL;
        PBAR p3 = NULL;
        PBAR p4 = NULL;
        int seg_idx = 0;
        PAXIS pAxis = new AXIS;
        int b_is_axis = 0; // 0:当前不是中枢; 1或-1:当前中枢延续
        int i_last_axis_end_idx = 0;
        int i_need_jump = 0;
        while (seg_idx < max_count) {
            PBAR p5 = (in_array + seg_idx);
            if (i_need_jump > 0) {
                p1 = p2; p2 = p3; p3 = p4; p4 = p5;
            }
            else {
                if (0 == b_is_axis) {
                    // 当前不是中枢状态
                    if ((seg_idx - i_last_axis_end_idx) > 3) {
                        b_is_axis = axis_is_axis(p1, p2, p3, p4, p5);
                    }/*
                    else if ((3 == (seg_idx - i_last_axis_end_idx)) && (i_last_axis_end_idx > 0)) {
                        b_is_axis = axis_is_axis(p1, p2, p3, p4, p5);
                    }*/
                    if (0 != b_is_axis) {
                        pAxis = new AXIS;
                        axis_list.push_back(pAxis);
                        i_last_axis_end_idx = p5->seg_idx;
                        pAxis->idx_left = p2->idx;
                        pAxis->idx_right = p5->idx;
                        if (p1->peak > 0) {
                            // 第一条线是向下线段
                            if ((p5->point > p2->point) && (p1->point > p4->point)) {
                                pAxis->zg = (p5->high > p3->high) ? p5->high : p3->high;
                                pAxis->gg = (p5->high > p3->high) ? p3->high : p5->high;
                                pAxis->zd = (p4->low < p2->low) ? p4->low : p2->low;
                                pAxis->dd = (p4->low < p2->low) ? p2->low : p4->low;
                            }
                        }
                        else {
                            // 第一条线是向上线段
                            if ((p5->point < p2->point) && (p1->point < p4->point)) {
                                pAxis->zg = (p4->high > p2->high) ? p4->high : p2->high;
                                pAxis->gg = (p4->high > p2->high) ? p2->high : p4->high;
                                pAxis->zd = (p5->low < p3->low) ? p5->low : p3->low;
                                pAxis->dd = (p5->low < p3->low) ? p3->low : p5->low;
                            }
                        }
                    }
                }
                else {
                    // 当前是中枢状态，判断中枢是否延续
                    b_is_axis = axis_in_axis(p4, p5, pAxis);
                    if (0 != b_is_axis) {
                        if (!((p5->point > pAxis->gg) || (p5->point < pAxis->dd))) {
                            pAxis->idx_right = p5->idx;
                            i_last_axis_end_idx = p5->seg_idx;
                        }
                        else {
                            pAxis->idx_right = p4->idx;
                            i_last_axis_end_idx = p4->seg_idx;
                        }
                    }
                }
                p1 = p2; p2 = p3; p3 = p4; p4 = p5;
            }
            seg_idx++;
        }
        size_t axis_count = axis_list.size();
        int idx = 0;
        for (PAXIS_LIST::iterator iter = axis_list.begin(); iter != axis_list.end(); iter++) {
            PAXIS pa = *iter;
            for (idx = pa->idx_left - 1; idx < pa->idx_right; idx++) {
                pOut[idx + 1] = pa->gg;
            }
        }
    }
}

void make_axis_dd(PBAR in_array, const int max_count, float* pOut) {
    PAXIS_LIST axis_list;
    if (max_count > 4) {
        PBAR p1 = NULL;
        PBAR p2 = NULL;
        PBAR p3 = NULL;
        PBAR p4 = NULL;
        int seg_idx = 0;
        PAXIS pAxis = new AXIS;
        int b_is_axis = 0; // 0:当前不是中枢; 1或-1:当前中枢延续
        int i_last_axis_end_idx = 0;
        int i_need_jump = 0;
        while (seg_idx < max_count) {
            PBAR p5 = (in_array + seg_idx);
            if (i_need_jump > 0) {
                p1 = p2; p2 = p3; p3 = p4; p4 = p5;
            }
            else {
                if (0 == b_is_axis) {
                    // 当前不是中枢状态
                    if ((seg_idx - i_last_axis_end_idx) > 3) {
                        b_is_axis = axis_is_axis(p1, p2, p3, p4, p5);
                    }/*
                    else if ((3 == (seg_idx - i_last_axis_end_idx)) && (i_last_axis_end_idx > 0)) {
                        b_is_axis = axis_is_axis(p1, p2, p3, p4, p5);
                    }*/
                    if (0 != b_is_axis) {
                        pAxis = new AXIS;
                        axis_list.push_back(pAxis);
                        i_last_axis_end_idx = p5->seg_idx;
                        pAxis->idx_left = p2->idx;
                        pAxis->idx_right = p5->idx;
                        if (p1->peak > 0) {
                            // 第一条线是向下线段
                            if ((p5->point > p2->point) && (p1->point > p4->point)) {
                                pAxis->zg = (p5->high > p3->high) ? p5->high : p3->high;
                                pAxis->gg = (p5->high > p3->high) ? p3->high : p5->high;
                                pAxis->zd = (p4->low < p2->low) ? p4->low : p2->low;
                                pAxis->dd = (p4->low < p2->low) ? p2->low : p4->low;
                            }
                        }
                        else {
                            // 第一条线是向上线段
                            if ((p5->point < p2->point) && (p1->point < p4->point)) {
                                pAxis->zg = (p4->high > p2->high) ? p4->high : p2->high;
                                pAxis->gg = (p4->high > p2->high) ? p2->high : p4->high;
                                pAxis->zd = (p5->low < p3->low) ? p5->low : p3->low;
                                pAxis->dd = (p5->low < p3->low) ? p3->low : p5->low;
                            }
                        }
                    }
                }
                else {
                    // 当前是中枢状态，判断中枢是否延续
                    b_is_axis = axis_in_axis(p4, p5, pAxis);
                    if (0 != b_is_axis) {
                        if (!((p5->point > pAxis->gg) || (p5->point < pAxis->dd))) {
                            pAxis->idx_right = p5->idx;
                            i_last_axis_end_idx = p5->seg_idx;
                        }
                        else {
                            pAxis->idx_right = p4->idx;
                            i_last_axis_end_idx = p4->seg_idx;
                        }
                    }
                }
                p1 = p2; p2 = p3; p3 = p4; p4 = p5;
            }
            seg_idx++;
        }
        size_t axis_count = axis_list.size();
        int idx = 0;
        for (PAXIS_LIST::iterator iter = axis_list.begin(); iter != axis_list.end(); iter++) {
            PAXIS pa = *iter;
            for (idx = pa->idx_left - 1; idx < pa->idx_right; idx++) {
                pOut[idx + 1] = pa->dd;
            }
        }
    }
}

int segment_cut(int layer, size_t tidx, size_t max_line_points, int i_segment, PBAR p1, PBAR p2, PBAR p3, PBAR p4, PBAR line_points, LP_PBAR_LIST p_parent_points)
{
    PBAR last_peak = NULL;
    PBAR last_append = NULL;
    size_t idx = tidx;
    int result_count = 0;
    if ((max_line_points - (idx + 2)) > 0) {
        PBAR p5 = line_points + idx + 1;
        idx += 2;
        int now_status = 0;
        int i_need_jump = 0;
        int i_peak_type_2 = 0;
        PBAR p1_wait_check = NULL;
        PBAR p2_wait_check = NULL;
        PBAR p3_wait_check = NULL;
        PBAR p4_wait_check = NULL;
        while (idx < max_line_points) {
            PBAR p6 = line_points + idx;
            if (1 == now_status) {
                if (i_need_jump > 0) {
                    p1 = p2;
                    p2 = p3;
                    p3 = p4;
                    p4 = p5;
                    p5 = p6;
                    // int i_contain_b = segment_is_contain(layer, i_segment, p3, p4, p5, p6);
                    // if (i_contain_b > 0) {
                    //     p4 = p6;
                    //     i_need_jump += 2;
                    // }
                }
                else {
                    // 分型成功，判断是否穿透，是否成新线，或第二种情况
                    int i_wait_check = segment_wait_check(layer, i_segment, p1_wait_check, p3_wait_check, p5, p6);
                    if (abs(i_wait_check) > 1) {
                        p1 = p2;
                        p2 = p3;
                        p3 = p4;
                        p4 = p5;
                        p5 = p6;
                        now_status = 0;
                    }
                    else if (abs(i_wait_check) > 0) {
                        i_segment = 0 - i_segment;
                        p1 = p3;
                        p2 = p4;
                        p3 = p5;
                        p4 = p6;
                        i_need_jump = 2;
                        now_status = 0;
                        p_parent_points->push_back(p3_wait_check);
                        result_count++;
                        last_append = p3_wait_check;
                    }
                    else {
                        i_peak_type_2 += 1;
                        if (i_peak_type_2 > 1) {
                            PBAR_LIST tmp_k_list;
                            int tmp_points_count = segment_cut(layer + 1, idx - 2, max_line_points, 0 - i_segment, p1, p2, p3, p4, line_points, &tmp_k_list);
                            if (tmp_points_count > 0) {
                                now_status = 0;
                                i_segment = 0 - i_segment;
                                p_parent_points->push_back(p3_wait_check);
                                result_count++;
                                last_append = p3_wait_check;
                                idx -= 1;
                            }
                        }
                        else {
                            p1 = p2;
                            p2 = p3;
                            p3 = p4;
                            p4 = p5;
                            p5 = p6;
                            now_status = 1;
                            i_need_jump = 2;
                        }
                    }
                }
            }
            else if (2 == now_status) {
                // 前两有右包含，没有生成新线
                if (i_need_jump > 0) {
                    p1 = p2;
                    p2 = p3;
                    p3 = p4;
                    p4 = p5;
                    p5 = p6;
                }
                else {
                    int i_wait_check_2 = segment_wait_check_2(layer, i_segment, p3_wait_check, p4_wait_check, p5, p6);
                    if (abs(i_wait_check_2) > 0) {
                        if (abs(i_wait_check_2) > 1) {
                            i_need_jump = 2;
                            now_status = 0;
                            p1 = p3;
                            p2 = p4;
                            p3 = p5;
                            p4 = p6;
                        }
                        else if (abs(i_wait_check_2) > 0) {
                            now_status = 1;
                            p1 = p2_wait_check;
                            p2 = p3_wait_check;
                            p3 = p4_wait_check;
                            p4 = p5;
                            p5 = p6;
                        }
                    }
                    else {
                        i_need_jump = 2;
                    }
                }
            }
            else if (3 == now_status) {
                // 前两个有右包含，已生成新线
                int i_wait_check_2 = segment_wait_check(layer, i_segment, p1_wait_check, p3_wait_check, p5, p6);
                if (abs(i_wait_check_2) > 1) {
                    p1 = p2;
                    p2 = p3;
                    p3 = p4;
                    p4 = p5;
                    p5 = p6;
                    now_status = 0;
                }
                else {
                    i_segment = 0 - i_segment;
                    p1 = p3;
                    p2 = p4;
                    p3 = p5;
                    p4 = p6;
                    i_need_jump = 2;
                    now_status = 0;
                    p_parent_points->push_back(p3_wait_check);
                    result_count++;
                    last_append = p3_wait_check;
                }
            }
            else {
                if (i_need_jump > 0) {
                    p5 = p6;
                }
                else {
                    int i_contain_a = segment_is_contain(layer, i_segment, p1, p2, p3, p4);
                    if (i_contain_a < 0) {
                        LineCalculateResult i_tmp_segment;
                        segment_if_segment(layer, p3, p4, p5, p6, &i_tmp_segment);
                        if (abs(i_segment - i_tmp_segment.code) > 1) {
                            if (layer > 0) {
                                last_peak = p3;
                                break;
                            }
                            now_status = 3;
                            p1_wait_check = p1;
                            p2_wait_check = p2;
                            p3_wait_check = p3;
                            p4_wait_check = p4;
                            p1 = p2;
                            p2 = p3;
                            p3 = p4;
                            p4 = p5;
                            p5 = p6;
                        }
                        else {
                            if (abs(i_tmp_segment.over) > 1) {
                                p1 = p3;
                                p4->point = p2->point;
                                p2 = p4;
                                p3 = p5;
                                p4 = p6;
                                i_need_jump = 2;
                            }
                            else {
                                now_status = 2;
                                p1_wait_check = p1;
                                p2_wait_check = p2;
                                p3_wait_check = p3;
                                p4_wait_check = p4;
                                p1 = p2;
                                p2 = p3;
                                p3 = p4;
                                p4 = p5;
                                p5 = p6;
                                i_need_jump = 2;
                            }
                        }
                    }
                    else {
                        int i_contain_b = segment_is_contain(layer, i_segment, p3, p4, p5, p6);
                        if (i_contain_b > 0) {
                            p4 = p6;
                            i_need_jump = 2;
                        }
                        else {
                            int i_peak = segment_is_peak(layer, i_segment, p1, p2, p3, p4, p5, p6);
                            if (0 != i_peak) {
                                now_status = 1;
                                i_peak_type_2 = 0;
                                p1_wait_check = p1;
                                p2_wait_check = p2;
                                p3_wait_check = p3;
                                p4_wait_check = p4;
                                last_peak = p3;
                                if (layer > 0) {
                                    break;
                                }
                                p1 = p2;
                                p2 = p3;
                                p3 = p4;
                                p4 = p5;
                                p5 = p6;
                            }
                            else {
                                p1 = p3;
                                p2 = p4;
                                p3 = p5;
                                p4 = p6;
                                i_need_jump = 2;
                            }
                        }
                    }
                }
            }
            idx += 1;
            if (i_need_jump > 0) {
                i_need_jump -= 1;
            }
        }
    }
    if (NULL != last_peak) {
        if (NULL != last_append) {
            if (last_append->line_idx < last_peak->line_idx) {
                p_parent_points->push_back(last_peak);
                result_count++;
            }
        }
        else {
            p_parent_points->push_back(last_peak);
            result_count++;
        }
    }
    return result_count;
}

void make_segment(PBAR in_array, const int max_length, float* pOut)
{
    int idx = 0;
    idx = 3;
    PBAR_LIST segment_bar_list;
    if ((max_length - idx) > 0) {
        PBAR p1 = (in_array + 0);
        PBAR p2 = (in_array + 1);
        PBAR p3 = (in_array + 2);
        PBAR p4 = (in_array + 3);
        LineCalculateResult lineCalculateResult;
        while (idx < max_length) {
            p4 = in_array + idx;
            segment_if_segment(0, p1, p2, p3, p4, &lineCalculateResult);
            if (0 != lineCalculateResult.code) {
                break;
            }
            p1 = p2;
            p2 = p3;
            p3 = p4;
            idx += 1;
        }
        // p0 = p1 # 线段的起点
        p1 = p2;
        p2 = p3;
        p3 = p4;
        idx += 1;
        int i_need_jump = 0;
        while (idx < max_length) {
            p4 = (in_array + idx);
            if (i_need_jump > 0) {
                p3 = p4;
            }
            else {
                int i_contain = segment_is_contain(0, lineCalculateResult.code, p1, p2, p3, p4);
                if (i_contain > 0) {
                    p2 = p4;
                    i_need_jump = 2;
                }
                else {
                    break;
                }
            }
            idx += 1;
            if (i_need_jump > 0) {
                i_need_jump -= 1;
            }
        }
        int i_sgement_count = segment_cut(0, idx, max_length, lineCalculateResult.code, p1, p2, p3, p4, in_array, &segment_bar_list);
        
        for (PBAR_LIST::iterator iter = segment_bar_list.begin(); iter != segment_bar_list.end(); iter++) {
            pOut[(*iter)->idx] = (*iter)->peak;
        }
    }
}

int segment_wait_check(int layer, int i_dir, PBAR p1, PBAR p3, PBAR p5, PBAR p6)
{
    int result = 0;
    if (i_dir > 0) {
        if (p6->point > p3->point) {
            result = 2;
        }
        else if (p5->point < p1->point) {
            result = -1;
        }
    }
    else if (i_dir < 0) {
        if (p6->point < p3->point) {
            result = -2;
        }
        else if (p5->point > p1->point) {
            result = 1;
        }
    }
    return result;
}

int segment_is_peak(int layer, int i_dir, PBAR p1, PBAR p2, PBAR p3, PBAR p4, PBAR p5, PBAR p6)
{
    int result = 0;
    if (i_dir > 0) {
        if (p3->peak > 0) {
            if (p3->point > p1->point && p4->point > p2->point && p3->point > p5->point && p4->point > p6->point) {
                result = 1;
            }
        }
    }
    if (i_dir < 0) {
        if (p3->peak < 0) {
            if (p3->point < p1->point && p4->point < p2->point && p3->point < p5->point && p4->point < p6->point) {
                result = -1;
            }
        }
    }
    return result;
}

int segment_wait_check_2(int layer, int i_dir, PBAR p3, PBAR p4, PBAR p5, PBAR p6)
{
    int result = 0;
    if (i_dir > 0) {
        if (p5->point > p3->point) {
            result = 2;
        }
        else if (p6->point < p4->point) {
            result = 1;
        }
    }
    else if (i_dir < 0) {
        if (p5->point < p3->point) {
            result = -2;
        }
        else if (p6->point > p4->point) {
            result = -1;
        }
    }
    return result;
}

int segment_is_contain(int layer, int i_dir, PBAR p1, PBAR p2, PBAR p3, PBAR p4)
{
    int result = 0;
    if (p1->peak > 0) {
        if (p1->point >= p3->point && p2->point <= p4->point) {
            result = 1;
        }
        else if (p1->point <= p3->point && p2->point >= p4->point) {
            result = -1;
        }
    }
    else if (p1->peak < 0) {
        if (p1->point >= p3->point && p2->point <= p4->point) {
            result = -1;
        }
        else if (p1->point <= p3->point && p2->point >= p4->point) {
            result = 1;
        }
    }
    return result;
}

void segment_if_segment(int layer, PBAR p1, PBAR p2, PBAR p3, PBAR p4, LPLineCalculateResult lpResult)
{
    lpResult->code = 0;
    lpResult->over = 0;
    if (p1->peak < 0) {
        if (p1->point < p2->point && p2->point > p3->point && p3->point > p1->point && p2->point < p4->point && p3->point < p4->point) {
            lpResult->code = 1;
        }
    }
    else if (p1->peak > 0) {
        if (p1->point > p2->point && p2->point < p3->point && p3->point < p1->point && p2->point > p4->point && p3->point > p4->point) {
            lpResult->code = -1;
        }
    }
    if (0 == lpResult->code) {
        if (p1->peak < 0) {
            if (p3->point < p1->point) {
                lpResult->over = -2;
            }
        }
        else if (p1->peak > 0) {
            if (p3->point > p1->point) {
                lpResult->over = 2;
            }
        }
    }
}

int axis_is_axis(PBAR p1, PBAR p2, PBAR p3, PBAR p4, PBAR p5)
{
    int result = 0;
    if (p1->peak > 0) {
        // 第一条线是向下线段
        if ((p5->point > p2->point) && (p1->point > p4->point)) {
            result = -1;
        }
    }
    else {
        // 第一条线是向上线段
        if ((p5->point < p2->point) && (p1->point < p4->point)) {
            result = 1;
        }
    }
    return result;
}

int axis_in_axis(PBAR p4, PBAR p5, PAXIS pAxis)
{
    int result = 1;
    if (p4->peak > 0) {
        if ((p4->high > pAxis->gg) && (p5->low > pAxis->gg)) {
            result = 0;
        }
    }
    else {
        if ((p4->low < pAxis->dd) && (p5->high < pAxis->dd)) {
            result = 0;
        }
    }
    return result;
}
