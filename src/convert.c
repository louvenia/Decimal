#include "decimal.h"

int s21_from_decimal_to_int(decimal src, int *dst) {
  decimal truncated_src = {0};
  s21_truncate(src, &truncated_src);
  int res = 1;
  int sign = get_Bit(truncated_src, 127);
  *dst = 0;

  if (sign) {
    decimal min = {0};
    char mantissa[] = "00000000000000000002147483648";
    set_val(&min, mantissa, 0, 1);
    if (s21_is_greater_or_equal(truncated_src, min)) {
      res = 0;
      convert_decimal_to_int(truncated_src, dst, 1);
    }
  } else {
    decimal max = {0};
    char mantissa[] = "00000000000000000002147483647";
    set_val(&max, mantissa, 0, 0);
    if (s21_is_less_or_equal(truncated_src, max)) {
      res = 0;
      convert_decimal_to_int(truncated_src, dst, 0);
    }
  }

  return res;
}

void convert_decimal_to_int(decimal value, int *dst, int sign) {
  for (int i = 0; i < 96; i++) {
    if (get_Bit(value, i)) {
      if (!sign) {
        *dst = *dst + (int)pow(2, i);
      } else {
        *dst = *dst - (int)pow(2, i);
      }
    }
  }
}

int s21_from_int_to_decimal(int src, decimal *dst) {
  int sign = 1, min = 0, res = 0;
  if (src < 0) {
    if (src == -2147483648) {
      src += 1;
      min = 1;
    }
    src *= (-1);
  } else {
    sign = 0;
  }
  memset(dst, 0, sizeof(*dst));
  dst->bits[0] = src;
  set_Bit(&(dst->bits[3]), sign, 31);
  if (sign && min) {
    decimal one = {0};
    one.bits[0] = 1;
    s21_sub(*dst, one, dst);
  }
  return res;
}

int s21_from_decimal_to_float(decimal src, float *dst) {
  dec_number float_dec_number = {0};
  make_Dec(src, &float_dec_number);
  char src_f[32];
  src_f[31] = '\0';
  if (float_dec_number.sign == 0) {
    src_f[0] = '+';
  } else {
    src_f[0] = '-';
  }
  int i = 0;
  for (int j = 1; j < 31; j++) {
    if (31 - j == 30 - float_dec_number.exp) {
      src_f[31 - j] = '.';
      continue;
    }
    src_f[31 - j] = float_dec_number.mantissa[i] + '0';
    i++;
  }
  sscanf(src_f, "%f", dst);
  return 0;
}

int s21_from_float_to_decimal(float src, decimal *dst) {
  int res = 0;
  if (isnan(src) || src == INFINITY || src == -INFINITY) {
    res = 1;
  } else {
    char srcs[100] = {'\0'};
    srcs[99] = '\0';
    int sign = 0;
    if (src < 0) {
      sign = 1;
      src *= (-1);
    }
    int a = (int)src;
    int cel = a;
    int y = 1;
    while (cel > 10) {
      cel /= 10;
      y++;
    }
    if (y == 1) {
      sprintf(srcs, "%-+10.6f", src);
    } else if (y == 2) {
      sprintf(srcs, "%-+10.5f", src);
    } else if (y == 3) {
      sprintf(srcs, "%-+10.4f", src);
    } else if (y == 4) {
      sprintf(srcs, "%-+10.3f", src);
    } else if (y == 5) {
      sprintf(srcs, "%-+10.2f", src);
    } else if (y == 6) {
      sprintf(srcs, "%-+10.1f", src);
    }
    dec_number dec_res = {0};
    if (sign == 1) {
      dec_res.sign = 1;
    }
    int index = 0;
    int counter = 0;
    int exp = 0;
    for (int i = 1; i <= 29 && index < 8; i++) {
      if (srcs[i] != '.') {
        dec_res.mantissa[counter++] = srcs[i] - '0';
        index++;
      } else {
        exp = 8 - index - 1;
      }
    }
    int count = 0;
    for (int q = 28; q >= 0; q--) {
      if (dec_res.mantissa[q] == 0) {
        count++;
      } else {
        break;
      }
    }

    for (int q = 28; q - count >= 0; q--) {
      dec_res.mantissa[q] = dec_res.mantissa[q - count];
      dec_res.mantissa[q - count] = 0;
    }
    swap_array(dec_res.mantissa, 28);
    dec_number dzero = {0};
    if (compare(dzero, dec_res) == 0) {
      for (int i = exp + 29; i <= exp + 40; i++) {
        if (srcs[i] != 0) {
          res = 1;
        }
      }
    }
    dec_res.exp = exp;
    convert_from_Dec(dst, dec_res);
  }
  return res;
}
