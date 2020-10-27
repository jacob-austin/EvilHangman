#ifndef UNIT_TEST_H
#define UNIT_TEST_H
#include "my_string.h"

Status test_init_default_returns_nonNULL(char* buffer, int length);
Status test_get_size_on_init_default_returns_0(char* buffer, int length);
Status test_jmaustin_get_capacity_on_init_default_returns_non0(char* buffer, int length);
Status test_jmaustin_init_c_string_returns_nonNULL(char* buffer, int length);
Status test_jmaustin_get_capacity_on_init_c_string_returns_non0(char* buffer, int length);
Status test_jmaustin_get_size_on_empty_init_c_string_returns_0(char* buffer, int length);
Status test_jmaustin_compare_right_bigger_returns_lessthan0(char* buffer, int length);
Status test_jmaustin_compare_left_bigger_returns_greaterthan0(char* buffer, int length);
Status test_jmaustin_compare_same_returns_0(char* buffer, int length);
Status test_jmaustin_compare_same_right_bigger_returns_lessthan0(char* buffer, int length);
Status test_jmaustin_compare_same_left_bigger_returns_greaterthan0(char* buffer, int length);
Status test_jmaustin_push_back_increments_size(char* buffer, int length);
Status test_jmaustin_concat_capacity_greaterthan_combined_arg_size(char* buffer, int length);
Status test_jmaustin_concat_size_equalto_combined_arg_size(char* buffer, int length);
Status test_jmaustin_concat_does_not_change_append_string_size(char* buffer, int length);
Status test_jmaustin_concat_does_not_change_append_string_capacity(char* buffer, int length);
Status test_jmaustin_string_empty_on_init_default_returns_1(char* buffer, int length);
Status test_jmaustin_string_empty_on_init_c_string_returns_0(char* buffer, int length);
Status test_jmaustin_pop_back_decrements_size_on_init_c_string(char* buffer, int length);
Status test_jmaustin_pop_back_on_init_default_returns_failure(char* buffer, int length);
Status test_jmaustin_c_str_on_init_c_string_returns_nonNULL(char* buffer, int length);
Status test_jmaustin_string_at_on_negative_index_returns_NULL(char* buffer, int length);
Status test_jmaustin_string_at_on_overflow_index_returns_NULL(char* buffer, int length);
Status test_jmaustin_extraction_returns_success_on_file_read(char* buffer, int length);
Status test_jmaustin_insertion_returns_success_on_writing_to_file(char* buffer, int length);

#endif
