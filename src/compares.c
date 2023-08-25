#include "decimal.h"

int check_dec_zero(dec_number dec_val) {
  int res = 0;
  int i = 0;
  while (i <= 28) {
    if (dec_val.mantissa[i] != 0) {
      res = 1;
    }
    i++;
  }
  return res;
}

int compare(dec_number dec_val_1, dec_number dec_val_2) {
  int swap = 1, shift = 0, result = 0, flag_return = 1, res = 0, flag = 0;
  if (check_dec_zero(dec_val_1) == 0 && check_dec_zero(dec_val_2) == 0) {
    flag_return = 0;
  }
  if (dec_val_1.sign != dec_val_2.sign && flag_return != 0) {
    flag_return = dec_val_2.sign - dec_val_1.sign;
    flag = 1;
  } else if (dec_val_1.sign == 1 && flag_return != 0) {
    swap *= (-1);
  }
  shift = dec_val_1.exp - dec_val_2.exp;
  if (shift < 0) {
    dec_number temp = dec_val_1;
    dec_val_1 = dec_val_2;
    dec_val_2 = temp;
    shift *= (-1);
    swap *= (-1);
  }

  for (int i = 28 + shift; i >= 0; i--) {
    int index = 0;
    index = i - shift;
    if (i > 28) {
      if (dec_val_2.mantissa[index] != 0) {
        result = -1;
        break;
      }
    } else if (i >= shift) {
      if (dec_val_2.mantissa[index] < dec_val_1.mantissa[i]) {
        result = 1;
        break;
      } else if (dec_val_2.mantissa[index] > dec_val_1.mantissa[i]) {
        result = -1;
        break;
      }
    } else {
      if (dec_val_1.mantissa[i] != 0) {
        result = 1;
        break;
      }
    }
  }
  if (flag_return == 1 && flag == 0) {
    res = result * swap;
  } else if (flag_return == 0) {
    res = flag_return;
  } else {
    res = flag_return;
  }

  return res;
}

int compare_conv(decimal value_1, decimal value_2) {
  dec_number dec_val_1 = {0}, dec_val_2 = {0};
  make_Dec(value_1, &dec_val_1);
  make_Dec(value_2, &dec_val_2);
  return compare(dec_val_1, dec_val_2);
}

int s21_is_less(decimal value_1, decimal value_2) {
  int res = CONVERT_ERROR;
  if (compare_conv(value_1, value_2) == -1) {
    res = CONVERT_OK;
  }
  return res;
}

int s21_is_less_or_equal(decimal value_1, decimal value_2) {
  int res = CONVERT_OK;
  if (compare_conv(value_1, value_2) == 1) {
    res = CONVERT_ERROR;
  }
  return res;
}

int s21_is_greater(decimal value_1, decimal value_2) {
  int res = CONVERT_ERROR;
  if (compare_conv(value_1, value_2) == 1) {
    res = CONVERT_OK;
  }
  return res;
}
int s21_is_greater_or_equal(decimal value_1, decimal value_2) {
  int res = CONVERT_OK;
  if (compare_conv(value_1, value_2) == -1) {
    res = CONVERT_ERROR;
  }
  return res;
}

int compare_equal(decimal value_1, decimal value_2) {
  int res = CONVERT_OK, flag = 0;
  for (int i = 0; i < 96; i++) {
    int rank = i / 32;
    if (value_1.bits[rank] == value_2.bits[rank] && value_1.bits[rank] == 0) {
      flag = 1;
    }
    if (value_1.bits[rank] != value_2.bits[rank]) {
      flag = 0;
      res = CONVERT_ERROR;
      break;
    }
  }
  for (int i = 96; i < 128; i++) {
    if (value_1.bits[3] != value_2.bits[3]) {
      if (i == 127 && flag == 1) {
        res = CONVERT_OK;
        break;
      }
      res = CONVERT_ERROR;
      break;
    }
  }
  return res;
}

int s21_is_equal(decimal value_1, decimal value_2) {
  int res = CONVERT_ERROR;
  if (compare_equal(value_1, value_2) == 1 ||
      compare_conv(value_1, value_2) == 0) {
    res = CONVERT_OK;
  }
  return res;
}
int s21_is_not_equal(decimal value_1, decimal value_2) {
  int res = CONVERT_ERROR;
  if (compare_equal(value_1, value_2) == 0 &&
      compare_conv(value_1, value_2) != 0) {
    res = CONVERT_OK;
  }
  return res;
}
