#include <stdio.h>
#include "unit_test.h"

int main(int argc, char* argv[])
{
  Status (*tests[])(char*, int)=
  {
    test_init_default_returns_nonNULL,                            //tests init_default
    test_get_size_on_init_default_returns_0,                      //tests get_size
    test_jmaustin_get_capacity_on_init_default_returns_non0,      //tests get_capacity
    test_jmaustin_init_c_string_returns_nonNULL,                  //tests init_c_string
    test_jmaustin_get_size_on_empty_init_c_string_returns_0,
    test_jmaustin_compare_right_bigger_returns_lessthan0,         //tests string_compare
    test_jmaustin_compare_left_bigger_returns_greaterthan0,
    test_jmaustin_compare_same_right_bigger_returns_lessthan0,
    test_jmaustin_compare_same_left_bigger_returns_greaterthan0,
    test_jmaustin_compare_same_returns_0,
    test_jmaustin_push_back_increments_size,                      //tests push_back
    test_jmaustin_concat_capacity_greaterthan_combined_arg_size,  //tests concat
    test_jmaustin_concat_size_equalto_combined_arg_size,
    test_jmaustin_concat_does_not_change_append_string_size,
    test_jmaustin_concat_does_not_change_append_string_capacity,
    test_jmaustin_string_empty_on_init_default_returns_1,         //tests string_empty
    test_jmaustin_string_empty_on_init_c_string_returns_0,
    test_jmaustin_pop_back_decrements_size_on_init_c_string,      //tests pop_back
    test_jmaustin_pop_back_on_init_default_returns_failure,
    test_jmaustin_c_str_on_init_c_string_returns_nonNULL,         //tests c_str
    test_jmaustin_string_at_on_negative_index_returns_NULL,       //tests string_at
    test_jmaustin_string_at_on_overflow_index_returns_NULL,
    test_jmaustin_get_capacity_on_init_c_string_returns_non0,
    test_jmaustin_extraction_returns_success_on_file_read,        //tests string_extraction
    test_jmaustin_insertion_returns_success_on_writing_to_file    //tests string_insertion
  };
  
  int number_of_functions = sizeof(tests) / sizeof(tests[0]);
  int i;
  char buffer[500];
  int success_count = 0;
  int failure_count = 0;

  for(i=0; i < number_of_functions; i++)
  {
    if(tests[i](buffer,500) == FAILURE)
    {
      printf("FAILED: Test %d failed\n", i);
      printf("\t%s\n", buffer);
      failure_count++;
    }
    else
    {
      printf("PASS: Test %d passed\n", i);
      printf("\t%s\n", buffer);
      success_count++;
    }
  }
  
  printf("Total number of tests:%d\n", number_of_functions);
  printf("%d/%d Pass, %d/%d Fail\n", success_count, number_of_functions,
	 failure_count, number_of_functions);
  
  return 0;
}
