
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <libswiftnav/set.h>

#include "check_utils.h"

#define LEN(x) (sizeof(x) / sizeof(x[0]))

void test_map_f(void *context, u32 n, const void *a_, const void *b_)
{
  s32 *a = (s32 *)a_;
  s32 *b = (s32 *)b_;
  s32 *c = (s32 *)context;

  fail_unless(context != NULL);
  fail_unless(*a == *b, "Intersection items not equal");
  c[n] = *a;
}

#define TEST_INTERSECTION(a, b, c) {                                           \
  s32 c_result[LEN(c)];                                                        \
                                                                               \
  qsort(a, LEN(a), sizeof(a[0]), cmp_s32_s32);                                 \
  qsort(b, LEN(b), sizeof(b[0]), cmp_s32_s32);                                 \
  qsort(c, LEN(c), sizeof(c[0]), cmp_s32_s32);                                 \
                                                                               \
  s32 ret = intersection_map(LEN(a), sizeof(a[0]), a,                          \
                             LEN(b), sizeof(b[0]), b,                          \
                             cmp_s32_s32, c_result, test_map_f);               \
                                                                               \
  fail_unless(ret == LEN(c),                                                   \
      "Intersection length does not match test data");                         \
                                                                               \
  fail_unless(memcmp(c, c_result, sizeof(c)) == 0,                             \
      "Output of intersection does not match test data");                      \
                                                                               \
  ret = intersection(LEN(a), sizeof(a[0]), a, c_result,                        \
                     LEN(b), sizeof(b[0]), b, NULL,                            \
                     cmp_s32_s32);                                             \
                                                                               \
  fail_unless(ret == LEN(c),                                                   \
      "Intersection length does not match test data (2)");                     \
                                                                               \
  fail_unless(memcmp(c, c_result, sizeof(c)) == 0,                             \
      "Output of intersection does not match test data (2)");                  \
                                                                               \
  ret = intersection(LEN(a), sizeof(a[0]), a, NULL,                            \
                     LEN(b), sizeof(b[0]), b, c_result,                        \
                     cmp_s32_s32);                                             \
                                                                               \
  fail_unless(ret == LEN(c),                                                   \
      "Intersection length does not match test data (3)");                     \
                                                                               \
  fail_unless(memcmp(c, c_result, sizeof(c)) == 0,                             \
      "Output of intersection does not match test data (3)");                  \
}

START_TEST(test_intersection_map_1)
{
  /* Empty first set */
  s32 a[] = {};
  s32 b[] = {1, 2, 3};
  s32 c[] = {};
  TEST_INTERSECTION(a, b, c)
}
END_TEST

START_TEST(test_intersection_map_2)
{
  /* Empty second set */
  s32 a[] = {1, 2, 3};
  s32 b[] = {};
  s32 c[] = {};
  TEST_INTERSECTION(a, b, c)
}
END_TEST

START_TEST(test_intersection_map_3)
{
  /* Beginning intersects */
  s32 a[] = {1, 2, 3, 4, 5, 6, 7};
  s32 b[] = {1, 2, 3};
  s32 c[] = {1, 2, 3};
  TEST_INTERSECTION(a, b, c)
}
END_TEST

START_TEST(test_intersection_map_4)
{
  /* End intersects */
  s32 a[] = {1, 2, 3, 4, 5, 6, 7};
  s32 b[] = {5, 6, 7};
  s32 c[] = {5, 6, 7};
  TEST_INTERSECTION(a, b, c)
}
END_TEST

START_TEST(test_intersection_map_5)
{
  /* Same set */
  s32 a[] = {1, 2, 3, 4, 5, 6, 7};
  s32 b[] = {1, 2, 3, 4, 5, 6, 7};
  s32 c[] = {1, 2, 3, 4, 5, 6, 7};
  TEST_INTERSECTION(a, b, c)
}
END_TEST

START_TEST(test_intersection_map_6)
{
  /* Disjoint */
  s32 a[] = {1, 2, 3, 4};
  s32 b[] = {5, 6, 7, 8};
  s32 c[] = {};
  TEST_INTERSECTION(a, b, c)
}
END_TEST

START_TEST(test_intersection_map_7)
{
  /* Middle overlaps */
  s32 a[] = {1, 2, 3, 4, 5, 6, 7};
  s32 b[] = {5, 6};
  s32 c[] = {5, 6};
  TEST_INTERSECTION(a, b, c)
}
END_TEST

START_TEST(test_intersection_map_8)
{
  /* Overlapping but not subset */
  s32 a[] = {1, 2, 3, 4, 5};
  s32 b[] = {4, 5, 6, 7, 8};
  s32 c[] = {4, 5};
  TEST_INTERSECTION(a, b, c)
}
END_TEST

START_TEST(test_intersection_map_9)
{
  /* Alternating disjoint */
  s32 a[] = {2, 4, 6, 8, 10};
  s32 b[] = {1, 3, 7, 9, 11};
  s32 c[] = {};
  TEST_INTERSECTION(a, b, c)
}
END_TEST

START_TEST(test_intersection_map_10)
{
  /* Alternating with overlap */
  s32 a[] = {2, 4, 6, 8, 9, 10};
  s32 b[] = {1, 3, 7, 8, 9, 11};
  s32 c[] = {8, 9};
  TEST_INTERSECTION(a, b, c)
}
END_TEST

START_TEST(test_is_prn_set)
{

#define TEST_IS_SET(set, result) \
  fail_unless(is_sid_set(sizeof(set)/sizeof(set[0]), set) == result, \
              "is_sid_set(" #set ") != " #result);

  /* Normal set. */
  gnss_signal_t prns1[] = {{.sat = 0}, {.sat = 1}, {.sat = 2}, {.sat = 33}, {.sat = 44}, {.sat = 200}};
  TEST_IS_SET(prns1, true);

  /* Empty set. */
  fail_unless(is_sid_set(0, prns1) == true);

  /* Single element set. */
  gnss_signal_t prns2[] = {{.sat = 22}};
  TEST_IS_SET(prns2, true);

  /* Repeated elements. */

  gnss_signal_t prns3[] = {{.sat = 22}, {.sat = 22}};
  TEST_IS_SET(prns3, false);

  gnss_signal_t prns4[] = {{.sat = 0}, {.sat = 1}, {.sat = 2}, {.sat = 3}, {.sat = 3}};
  TEST_IS_SET(prns4, false);

  gnss_signal_t prns5[] = {{.sat = 1}, {.sat = 1}, {.sat = 2}, {.sat = 3}, {.sat = 4}};
  TEST_IS_SET(prns5, false);

  /* Incorrectly sorted. */

  gnss_signal_t prns6[] = {{.sat = 22}, {.sat = 1}, {.sat = 2}, {.sat = 3}, {.sat = 4}};
  TEST_IS_SET(prns6, false);

  gnss_signal_t prns7[] = {{.sat = 0}, {.sat = 1}, {.sat = 2}, {.sat = 3}, {.sat = 1}};
  TEST_IS_SET(prns7, false);

  gnss_signal_t prns8[] = {{.sat = 0}, {.sat = 1}, {.sat = 22}, {.sat = 3}, {.sat = 4}};
  TEST_IS_SET(prns8, false);
}
END_TEST

Suite* set_suite(void)
{
  Suite *s = suite_create("Set");

  TCase *tc_intersection = tcase_create("Intersection");
  tcase_add_test(tc_intersection, test_intersection_map_1);
  tcase_add_test(tc_intersection, test_intersection_map_2);
  tcase_add_test(tc_intersection, test_intersection_map_3);
  tcase_add_test(tc_intersection, test_intersection_map_4);
  tcase_add_test(tc_intersection, test_intersection_map_5);
  tcase_add_test(tc_intersection, test_intersection_map_6);
  tcase_add_test(tc_intersection, test_intersection_map_7);
  tcase_add_test(tc_intersection, test_intersection_map_8);
  tcase_add_test(tc_intersection, test_intersection_map_9);
  tcase_add_test(tc_intersection, test_intersection_map_10);
  TCase *tc_set = tcase_create("Set");
  tcase_add_test(tc_set, test_is_prn_set);
  suite_add_tcase(s, tc_intersection);
  suite_add_tcase(s, tc_set);

  return s;
}
