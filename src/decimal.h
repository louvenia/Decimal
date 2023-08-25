#ifndef SRC_DECIMAL_H_
#define SRC_DECIMAL_H_

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int bits[4];
} decimal;

typedef struct {
  int mantissa[29];
  int exp;
  int sign;
} dec_number;

typedef enum { LOW, MID, HIGH, SCALE } dec_values;

typedef enum { CONVERT_ERROR, CONVERT_OK } convert_ok;

void show_dec(dec_number dec_val);
void set_val(decimal *value, const char *mantissa, int exponent, int sign);
void show_val(decimal value);

int getbit_1(int a, int i);
int get_Bit(decimal value, int i);
void set_Bit(int *value, int bit, int i);
void make_Dec(decimal value, dec_number *dec_val);
int convert_from_Dec(decimal *value, dec_number dec_val);
void shift_dec_right(dec_number *dec_val, int shift);
void shift_array_right(int *arr, int shift, int size);
void swap_array(int *arr, int max_index);
int index_nonzero(dec_number dec_val);
int check_dec_zero(dec_number dec_val);
int compare_conv(decimal value_1, decimal value_2);
int compare(dec_number dec_val_1, dec_number dec_val_2);
int compare_equal(decimal value_1, decimal value_2);

int s21_is_less(decimal value_1, decimal value_2);
int s21_is_less_or_equal(decimal value_1, decimal value_2);
int s21_is_greater(decimal value_1, decimal value_2);
int s21_is_greater_or_equal(decimal value_1, decimal value_2);
int s21_is_equal(decimal value_1, decimal value_2);
int s21_is_not_equal(decimal value_1, decimal value_2);

int s21_add(decimal value_1, decimal value_2, decimal *result);
void subtraction_dec(dec_number dec_val_1, dec_number dec_val_2,
                     dec_number *dec_res);
int s21_sub(decimal value_1, decimal value_2, decimal *result);
void multiplication_dec(dec_number dec_val_1, dec_number dec_val_2,
                        dec_number *dec_res);
int s21_mul(decimal value_1, decimal value_2, decimal *result);
int for_division(dec_number *dec_val_1, dec_number dec_val_2);
int s21_div(decimal value_1, decimal value_2, decimal *result);
int s21_mod(decimal value_1, decimal value_2, decimal *result);

void convert_decimal_to_int(decimal value, int *dst, int sign);
int s21_from_decimal_to_int(decimal src, int *dst);
int s21_from_int_to_decimal(int src, decimal *dst);
int s21_from_float_to_decimal(float src, decimal *dst);
int s21_from_decimal_to_float(decimal src, float *dst);

int s21_truncate(decimal value, decimal *result);
int s21_round(decimal value, decimal *result);
int s21_floor(decimal value, decimal *result);
int s21_negate(decimal value, decimal *result);

#endif // SRC_DECIMAL_H_
