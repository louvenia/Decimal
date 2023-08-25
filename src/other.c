#include "decimal.h"

int s21_truncate(decimal value, decimal *result) {
  dec_number dec_val_1 = {0};
  make_Dec(value, &dec_val_1);
  if (dec_val_1.exp != 0) {
    int i = 0;
    while (i < dec_val_1.exp) {
      dec_val_1.mantissa[i] = 0;
      i++;
    }
    shift_dec_right(&dec_val_1, dec_val_1.exp);
    dec_val_1.exp = 0;
  }
  convert_from_Dec(result, dec_val_1);
  return 0;
}

int s21_round(decimal value, decimal *result) {
  dec_number dec_val = {0};
  make_Dec(value, &dec_val);
  shift_dec_right(&dec_val, dec_val.exp);
  if (dec_val.exp < 0 || dec_val.exp > 28) {
    return 1;
  }
  dec_val.exp = 0;
  convert_from_Dec(result, dec_val);
  return 0;
}
int s21_negate(decimal value, decimal *result) {
  set_Bit(&value.bits[3], !get_Bit(value, 127), 31);
  *result = value;
  return 0;
}

int s21_floor(decimal value, decimal *result) {
  decimal zero = {0}, one = {0};
  s21_from_int_to_decimal(1, &one);
  s21_truncate(value, result);
  if (!s21_is_greater_or_equal(value, zero)) {
    s21_sub(*result, one, result);
  }
  return 0;
}
