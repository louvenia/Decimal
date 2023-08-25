#include "decimal.h"

int get_Bit(decimal value, int i) {
  for (; (i % 32) > 0; i--) {
    value.bits[i / 32] >>= 1;
  }
  return value.bits[i / 32] & 1;
}
void set_Bit(int *value, int bit, int i) {
  if (bit) {
    *value |= (bit << i);
  } else {
    *value &= ~((bit + 1) << i);
  }
}

void make_Dec(decimal value, dec_number *dec_val) {
  decimal q = {0}, n = value;
  memset(dec_val, 0, sizeof(*dec_val));
  int get_exp = 0, index_exp = 119;
  while (index_exp >= 112) {
    get_exp *= 2;
    get_exp += get_Bit(value, index_exp);
    index_exp--;
  }
  dec_val->exp = get_exp;
  dec_val->sign = get_Bit(value, 127);
  for (int i = 0; i <= 28; i++) {
    memset(&q, 0, sizeof(q));
    int temp_mant = 0, j = 95;
    while (j >= 0) {
      temp_mant = temp_mant << 1;
      set_Bit(&temp_mant, get_Bit(n, j), 0);
      if (temp_mant >= 10) {
        temp_mant -= 10;
        set_Bit(&(q.bits[j / 32]), 1, j % 32);
      }
      j--;
    }
    dec_val->mantissa[i] = temp_mant;
    n = q;
  }
}

int convert_from_Dec(decimal *value, dec_number dec_val) {
  dec_number my_dec_val = dec_val;
  memset(value, 0, sizeof(*value));
  for (int j = 0; j < 96; j++) {
    set_Bit(&(value->bits[j / 32]), my_dec_val.mantissa[0] % 2, j % 32);
    int counter = 0;
    for (int i = 28; i >= 0; i--) {
      if ((my_dec_val.mantissa[i] + counter) % 2 == 1) {
        my_dec_val.mantissa[i] += counter;
        my_dec_val.mantissa[i] /= 2;
        counter = 10;
      } else {
        my_dec_val.mantissa[i] += counter;
        my_dec_val.mantissa[i] /= 2;
        counter = 0;
      }
    }
  }
  value->bits[3] = my_dec_val.exp << 16;
  set_Bit(&(value->bits[3]), my_dec_val.sign, 31);
  return check_dec_zero(my_dec_val);
}

void shift_dec_right(dec_number *dec_val, int shift) {
  int five = 0, c = 0;
  if (dec_val->mantissa[shift - 1] == 5) {
    five = 1;
    int i = shift - 2;
    while (i >= 0) {
      if (dec_val->mantissa[i] > 0) {
        five = 0;
        c = 1;
      }
      i--;
    }
  } else {
    if (dec_val->mantissa[shift - 1] > 5) {
      c = 1;
    }
  }
  if (five) {
    c = dec_val->mantissa[shift] % 2;
  }
  int j = 0;
  while (j <= 28 - shift) {
    dec_val->mantissa[j] = dec_val->mantissa[j + shift] + c;
    c = 0;
    if (dec_val->mantissa[j] == 10) {
      c = 1;
      dec_val->mantissa[j] = 0;
    }
    j++;
  }
  int n = 28 - shift + 1;
  while (n <= 28) {
    dec_val->mantissa[n] = c;
    c = 0;
    n++;
  }
}

void shift_array_right(int *arr, int shift, int size) {
  int five = 0;
  int c = 0;
  if (arr[shift - 1] == 5) {
    five = 1;
    for (int i = shift - 2; i >= 0; i--) {
      if (arr[i] > 0) {
        c = 1;
        five = 0;
      }
    }
  } else if (arr[shift - 1] > 5) {
    c = 1;
  }
  if (five) {
    c = arr[shift] % 2;
  }
  for (int i = 0; i <= size * 2 - 1 - shift; i++) {
    arr[i] = arr[i + shift] + c;
    c = 0;
    if (arr[i] == 10) {
      arr[i] = 0;
      c = 1;
    }
  }
  for (int i = size * 2 - 1 - shift + 1; i <= size * 2 - 1; i++) {
    arr[i] = c;
    c = 0;
  }
}

int index_nonzero(dec_number dec_val) {
  int index = 0;
  int i = 0;
  while (i <= 28) {
    if (dec_val.mantissa[i] != 0) {
      index = i;
    }
    i++;
  }
  return index;
}

void swap_array(int *arr, int max_index) {
  for (int i = 0; i <= max_index / 2; i++) {
    int temp = arr[i];
    arr[i] = arr[max_index - i];
    arr[max_index - i] = temp;
  }
}

void set_val(decimal *value, const char *mantissa, int exp, int sign) {
  dec_number dec_val = {0};
  dec_val.exp = exp;
  dec_val.sign = sign;
  for (int i = 0; i <= 28; i++) {
    dec_val.mantissa[i] = mantissa[28 - i] - '0';
  }
  convert_from_Dec(value, dec_val);
}

// void show_val(decimal value) {
//   for (int i = 0; i < 128; i++) {
//     if (i % 32 == 0) {
//       printf(" ");
//     }
//     printf("%d", get_Bit(value, i));
//   }
//   printf("\n");
//   dec_number dec_val = {0};
//   make_Dec(value, &dec_val);
//   for (int i = 0; i <= 28; i++) {
//     printf("%d", dec_val.mantissa[28 - i]);
//   }
//   printf(" %d %d\n", dec_val.exp, dec_val.sign);
// }
