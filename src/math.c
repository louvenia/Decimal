#include "decimal.h"

int s21_add(decimal value_1, decimal value_2, decimal *result) {
  dec_number dec_val_1 = {0}, dec_val_2 = {0}, dec_res = {0};
  make_Dec(value_1, &dec_val_1);
  make_Dec(value_2, &dec_val_2);
  int res = 0;
  if (dec_val_1.sign != dec_val_2.sign) {
    dec_val_1.sign = !dec_val_1.sign;
    convert_from_Dec(&value_1, dec_val_1);
    s21_sub(value_1, value_2, result);
    set_Bit(&result->bits[3], !get_Bit(*result, 127), 31);
    return res;
  }
  int difference_exp = dec_val_1.exp - dec_val_2.exp;
  if (difference_exp < 0) {
    dec_number temp = dec_val_1;
    dec_val_1 = dec_val_2;
    dec_val_2 = temp;
    difference_exp *= -1;
  }

  int counter = 0;
  for (int i = 0; i <= 28; i++) {
    dec_res.mantissa[i] = dec_val_1.mantissa[i] + counter;
    if (difference_exp <= i) {
      dec_res.mantissa[i] += dec_val_2.mantissa[i - difference_exp];
    }
    if (dec_res.mantissa[i] >= 10) {
      dec_res.mantissa[i] -= 10;
      counter = 1;
    } else {
      counter = 0;
    }
  }
  dec_res.exp = dec_val_1.exp;
  int shift = difference_exp - 28 + index_nonzero(dec_val_2);
  if (shift > 0) {
    shift_dec_right(&dec_res, shift);
    dec_res.exp -= shift;
    for (int i = 29 - shift; i <= 28; i++) {
      dec_res.mantissa[i] =
          dec_res.mantissa[i] +
          dec_val_2.mantissa[i - 28 + index_nonzero(dec_val_2)] + counter;
      if (dec_res.mantissa[i] >= 10) {
        dec_res.mantissa[i] -= 10;
        counter = 1;
      } else {
        counter = 0;
      }
    }
  }
  if (counter) {
    if (dec_res.exp == 0) {
      res = 1;
    }
    shift_dec_right(&dec_res, 1);
    dec_res.mantissa[28] += counter;
    dec_res.exp -= 1;
  }
  if (!res) {
    res = convert_from_Dec(result, dec_res);
    if (res && (dec_res.exp > 0)) {
      shift_dec_right(&dec_res, 1);
      dec_res.exp -= 1;
      convert_from_Dec(result, dec_res);
      res = 0;
    }
  }
  if (dec_val_1.sign == 1) {
    res += res;
    set_Bit(&result->bits[3], 1, 31);
  }
  return res;
}

void subtraction_dec(dec_number dec_val_1, dec_number dec_val_2,
                     dec_number *dec_res) {
  memset(dec_res, 0, sizeof(*dec_res));
  int difference_exp = dec_val_1.exp - dec_val_2.exp;
  int counter = 0;
  if (difference_exp >= 0) {
    dec_res->exp = dec_val_1.exp;
    for (int i = 0; i <= 28; i++) {
      if (i < difference_exp) {
        dec_res->mantissa[i] = dec_val_1.mantissa[i];
      } else {
        dec_res->mantissa[i] = dec_val_1.mantissa[i] -
                               dec_val_2.mantissa[i - difference_exp] - counter;
        if (dec_res->mantissa[i] < 0) {
          dec_res->mantissa[i] += 10;
          counter = 1;
        } else {
          counter = 0;
        }
      }
    }
  } else {
    for (int i = 0; i <= 28; i++) {
      if (i <= 28 + difference_exp) {
        dec_res->mantissa[i - difference_exp] = dec_val_1.mantissa[i];
      }
      dec_res->mantissa[i] =
          dec_res->mantissa[i] - dec_val_2.mantissa[i] - counter;
      if (dec_res->mantissa[i] < 0) {
        dec_res->mantissa[i] += 10;
        counter = 1;
      } else {
        counter = 0;
      }
    }
    dec_res->exp = dec_val_2.exp;
    int shift = index_nonzero(dec_val_1) - 28 - difference_exp;
    if (shift > 0) {
      dec_res->exp -= shift;
      shift_dec_right(dec_res, shift);
      for (int i = 29 - shift; i <= 28; i++) {
        dec_res->mantissa[i] +=
            dec_val_1.mantissa[index_nonzero(dec_val_1) - 28 + i] - counter;
        if (dec_res->mantissa[i] < 0) {
          dec_res->mantissa[i] += 10;
          counter = 1;
        } else {
          counter = 0;
        }
      }
    }
  }
}
int s21_sub(decimal value_1, decimal value_2, decimal *result) {
  int swap = 1;
  dec_number dec_val_1 = {0}, dec_val_2 = {0}, dec_res = {0};
  make_Dec(value_1, &dec_val_1);
  make_Dec(value_2, &dec_val_2);
  if (dec_val_1.sign != dec_val_2.sign) {
    dec_val_1.sign = !dec_val_1.sign;
    convert_from_Dec(&value_1, dec_val_1);
    int res = 0;
    res = s21_add(value_1, value_2, result);
    set_Bit(&result->bits[3], !get_Bit(*result, 127), 31);
    return res == 0 ? 0 : !(res - 1) + 1;
  }
  if (dec_val_1.sign == 1) {
    swap *= -1;
    set_Bit(&value_1.bits[3], 0, 31);
    set_Bit(&value_2.bits[3], 0, 31);
  }
  if (s21_is_greater(value_2, value_1)) {
    swap *= -1;
    dec_number temp = dec_val_1;
    dec_val_1 = dec_val_2;
    dec_val_2 = temp;
  }
  subtraction_dec(dec_val_1, dec_val_2, &dec_res);
  int over = convert_from_Dec(result, dec_res);
  if (over) {
    shift_dec_right(&dec_res, 1);
    dec_res.exp -= 1;
    convert_from_Dec(result, dec_res);
  }
  int bit = get_Bit(*result, 127);
  if (swap == -1) {
    bit = !bit;
  }
  set_Bit(&result->bits[3], bit, 31);
  return 0;
}

void multiplication_dec(dec_number dec_val_1, dec_number dec_val_2,
                        dec_number *dec_res) {
  memset(dec_res, 0, sizeof(*dec_res));
  // int *temp = (int*)malloc(sizeof(int) * 58);
  int temp[58] = {0};
  for (int i = 0; i < 58; i++) {
    temp[i] = 0;
  }
  for (int i = 0; i <= 28; i++) {
    int carry = 0;
    for (int j = 0; j <= 28; j++) {
      temp[j + i] += carry + dec_val_2.mantissa[j] * dec_val_1.mantissa[i];
      carry = temp[i + j] / 10;
      temp[i + j] = temp[i + j] % 10;
    }
    temp[i + 29] = carry;
  }
  int last_nonzero = 28;
  for (int i = 28; i < 58; i++) {
    if (temp[i] != 0) {
      last_nonzero = i;
    }
  }
  if (last_nonzero > 28) {
    shift_array_right(temp, last_nonzero - 28, 29);
  }
  for (int i = 0; i <= 28; i++) {
    dec_res->mantissa[i] = temp[i];
  }
  dec_res->exp = dec_val_1.exp + dec_val_2.exp - last_nonzero + 28;
  dec_res->sign = (dec_val_1.sign == dec_val_2.sign) ? 0 : 1;
  // free(temp);
}

int s21_mul(decimal value_1, decimal value_2, decimal *result) {
  dec_number dec_val_1 = {0}, dec_val_2 = {0}, dec_res = {0};
  make_Dec(value_1, &dec_val_1);
  make_Dec(value_2, &dec_val_2);
  multiplication_dec(dec_val_1, dec_val_2, &dec_res);
  if (convert_from_Dec(result, dec_res)) {
    shift_dec_right(&dec_res, 1);
    dec_res.exp -= 1;
    convert_from_Dec(result, dec_res);
  }
  int res = 0;
  if (dec_res.exp < 0 && dec_res.sign == 0) {
    res = 1;
  } else if (dec_res.exp < 0 && dec_res.sign == 1) {
    res = 2;
  }
  return res;
}

int for_division(dec_number *dec_val_1, dec_number dec_val_2) {
  dec_number temp = {0}, dec_val_2_mul = {0};
  if (compare(*dec_val_1, dec_val_2) == -1) {
    return 0;
  }
  int i;
  for (i = 0; i <= 10; i++) {
    memset(&temp, 0, sizeof(temp));
    temp.mantissa[0] = i;
    memset(&dec_val_2_mul, 0, sizeof(dec_val_2_mul));
    multiplication_dec(temp, dec_val_2, &dec_val_2_mul);
    if (compare(*dec_val_1, dec_val_2_mul) == -1) {
      i--;
      break;
    } else if (compare(*dec_val_1, dec_val_2_mul) == 0) {
      break;
    }
  }
  temp.mantissa[0] = i;
  memset(&dec_val_2_mul, 0, sizeof(dec_val_2_mul));
  multiplication_dec(temp, dec_val_2, &dec_val_2_mul);
  memset(&temp, 0, sizeof(temp));
  subtraction_dec(*dec_val_1, dec_val_2_mul, &temp);
  *dec_val_1 = temp;
  return i;
}

int s21_div(decimal value_1, decimal value_2, decimal *result) {
  dec_number dec_val_1 = {0}, dec_val_2 = {0}, dec_res = {0}, dzero = {0},
             temp = {0};
  decimal ten = {0};
  s21_from_int_to_decimal(10, &ten);
  dec_number dten = {0};
  make_Dec(ten, &dten);
  make_Dec(value_1, &dec_val_1);
  make_Dec(value_2, &dec_val_2);
  if (compare(dec_val_2, dzero) == 0) {
    return 3;
  }
  dec_res.sign = (dec_val_1.sign == dec_val_2.sign) ? 0 : 1;
  dec_val_1.sign = 0;
  dec_val_2.sign = 0;
  int exp = dec_val_1.exp - dec_val_2.exp + index_nonzero(dec_val_2) -
            index_nonzero(dec_val_1);
  dec_val_1.exp = index_nonzero(dec_val_1);
  dec_val_2.exp = index_nonzero(dec_val_2);
  int ans[40];
  for (int j = 0; j < 40; j++) {
    ans[j] = 0;
  }
  int i = 0;
  while (i < 40) {
    int counter = for_division(&dec_val_1, dec_val_2);
    ans[i] = counter;
    memset(&temp, 0, sizeof(temp));
    multiplication_dec(dec_val_1, dten, &temp);
    dec_val_1 = temp;
    i++;
  }
  int exp2 = 0;
  for (int j = 0; j < 40; j++) {
    if (ans[j] != 0) {
      exp2 = j;
      break;
    }
  }
  int end = exp2 + 28;
  if (end > 39) {
    end = 39;
  }
  if (end < 39) {
    swap_array(ans, 39);
    shift_array_right(ans, 39 - end, 20);
    swap_array(ans, 39);
  }
  for (int j = 39; j >= 11; j--) {
    dec_res.mantissa[39 - j] = ans[j];
  }
  dec_res.exp = exp + 28 + exp2;
  int res = 0;
  if (dec_res.exp > 28) {
    shift_dec_right(&dec_res, dec_res.exp - 28);
    dec_res.exp = 28;
  }
  if (convert_from_Dec(result, dec_res)) {
    shift_dec_right(&dec_res, 1);
    dec_res.exp -= 1;
    convert_from_Dec(result, dec_res);
  }
  memset(result, 0, sizeof(*result));
  convert_from_Dec(result, dec_res);
  if (dec_res.exp < 0) {
    res = 1 + dec_res.sign;
  }
  return res;
}

int s21_mod(decimal value_1, decimal value_2, decimal *result) {
  int res = 0;
  decimal quotient = {0};
  int over = s21_div(value_1, value_2, &quotient);
  if (over != 0) {
    memset(&quotient, 0, sizeof(quotient));
    *result = quotient;
    return res;
  }
  s21_truncate(quotient, &quotient);
  s21_mul(value_2, quotient, &quotient);
  res = s21_sub(value_1, quotient, result);
  if (get_Bit(*result, 127) == 1) {
    if (get_Bit(value_2, 127) == 1) {
      s21_sub(value_2, *result, result);
    } else {
      s21_add(value_2, *result, result);
    }
    set_Bit(&result->bits[3], 0, 31);
  }
  return res;
}
