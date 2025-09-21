#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MANTISSA_LEN 41
#define MAX_INT_LEN 30
#define MAX_MAN_AFTER_MULTY 30
#define MAX_LEN 100

typedef struct
{
    char mantissa[MAX_MANTISSA_LEN + 10];
    char mantissa_sign;
    int exponent;
    char exponent_sign;
} extended_float;

char get_sign(char letter)
{
    char sign = '+';
    if (!isdigit(letter)) // первый символ - знак
        sign = (letter == '-' ? '-' : '+');
    return sign;
}

void print_lineal(int max_digits) 
{
    for (int i = 1; i <= max_digits; i++) 
    {
        if (i % 10 == 0) 
        {
            printf("%d", i / 10);
        } else 
        {
            printf(" ");
        }
    }
    printf("\n");

    for (int i = 1; i <= max_digits; i++) 
    {
        printf("%d", i % 10);
    }
    printf("\n");
}

void print_num_struct(extended_float* num) {
    printf("%c0.%sE%+d", num->mantissa_sign, num->mantissa, num->exponent);
}

int string_to_arr_num(char *s, int *d)
{
    int len = 0;
    char* ptr = s;
    while (*ptr && *ptr != 'E' && *ptr != 'e' && len < MAX_MAN_AFTER_MULTY*2)
    {
        d[len++] = *ptr++ - '0'; // записываем цифры в прямом порядке
    }

    // Переворачиваем массив, чтобы d[0] был единицами
    for (int i = 0; i < len / 2; ++i)
    {
        int temp = d[i];
        d[i] = d[len - 1 - i];
        d[len - 1 - i] = temp;
    }

    return len;
}

int validate_number(char *s)
{
    if (!s || !*s) 
        return 0;

    int has_digits = 0;
    int has_dot = 0;
    int has_exp = 0;
    int len = 0;

    char *ptr = s;

    // Пропуск знака, если он есть
    if (*ptr == '+' || *ptr == '-') 
    {
        ptr++;
    }

    // Проверка на пустую строку после знака
    if (!*ptr) 
        return 0;
    
    char* temp_ptr = ptr;
    while (*temp_ptr) 
    {
        if (isdigit(*temp_ptr)) 
        {
            has_digits = 1;
        } else if (*temp_ptr == '.') 
        {
            if (has_dot || has_exp) 
                return 0;
            has_dot = 1;
        } else if (*temp_ptr == 'E' || *temp_ptr == 'e') 
        {
            if (has_exp || !has_digits) 
                return 0;
            has_exp = 1;
            temp_ptr++;
            // Проверка знака экспоненты
            if (*temp_ptr == '+' || *temp_ptr == '-') 
            {
                temp_ptr++;
            }
            // Проверка на наличие цифр после E
            if (!*temp_ptr || !isdigit(*temp_ptr)) 
                return 0;
            while (isdigit(*temp_ptr)) 
                temp_ptr++;
            continue;
        } 
        else 
        {
            return 0; // Недопустимый символ
        }
        temp_ptr++;
    }

    // Дополнительные проверки
    if (!has_digits) 
    {
        return 0;
    }

    // Подсчет значащих цифр для проверки длины мантиссы
    ptr = s;
    if (*ptr == '+' || *ptr == '-') 
        ptr++;
    
    char* dot_pos = strchr(ptr, '.');
    char* exp_pos = strchr(ptr, 'E');
    if (exp_pos == NULL) 
        exp_pos = strchr(ptr, 'e');

    // Если есть экспонента, работаем с частью до неё
    char temp_mant[MAX_LEN];
    if (exp_pos) 
    {
        strncpy(temp_mant, ptr, exp_pos - ptr);
        temp_mant[exp_pos - ptr] = '\0';
        ptr = temp_mant;
    }

    // Пропуск ведущих нулей (включая нескольно, напирмер .000)
    if (*ptr == '0' && dot_pos) 
    {
        ptr++;
        if (*ptr == '.') ptr++;
        while (*ptr == '0' && *ptr != '\0') 
            ptr++;
    } 
    else 
    {
        while (*ptr == '0') ptr++;
    }
    
    // Подсчет значащих цифр
    while (*ptr && *ptr != '.') 
    {
        len++;
        ptr++;
    }
    if (dot_pos) 
    {
        ptr = dot_pos + 1;
        while (*ptr) 
        {
            len++;
            ptr++;
        }
    }

    if (len > MAX_MANTISSA_LEN) 
    {
        return 0;
    }
    
    return 1;
}

void norm_res(extended_float* num) 
{
    char* ptr = num->mantissa;
    int indx = 0;
    while (*ptr && *ptr == '0')
    {
        indx++;
        ptr++;
    }
    strcpy(num->mantissa, ptr);
    num->exponent = num->exponent - indx;
    if (strcmp(num->mantissa, "") == 0) 
    {
        strcpy(num->mantissa, "0");
        num->exponent = 0;
    }
}

void norm_double(char *str, char *out)
{
    char *ptr = str;
    char sign = get_sign(str[0]);
    
    if (*ptr == '-' || *ptr == '+')
        ptr++;
    
    char number[MAX_LEN];
    int idx = 0;
    int exponent = 0;

    char* dot_pos = strchr(ptr, '.');
    char* first_digit = ptr;

    if (dot_pos) 
    {
        // Подсчет экспоненты для чисел вида X.XX
        exponent = dot_pos - first_digit;
    } else 
    {
        // Подсчет экспоненты для чисел вида XXX
        exponent = strlen(ptr);
    }

    // Извлечение мантиссы (без точки)
    char* p = ptr;
    while (*p) 
    {
        if (isdigit(*p)) 
        {
            number[idx++] = *p;
        }
        p++;
    }
    number[idx] = '\0';
    
    // Пропуск ведущих нулей в мантиссе
    char *num_ptr = number;
    while (*num_ptr == '0' && *(num_ptr + 1) != '\0') 
    {
        num_ptr++;
    }

    // Если число меньше 1, корректируем экспоненту
    if (dot_pos && (dot_pos == first_digit + 1 && *first_digit == '0')) 
    {
        char* p_after_dot = dot_pos + 1;
        int leading_zeros_after_dot = 0;
        while (*p_after_dot == '0') 
        {
            leading_zeros_after_dot++;
            p_after_dot++;
        }
        exponent = -leading_zeros_after_dot;
    }

    // Если мантисса состоит только из нулей
    if (strcmp(num_ptr, "0") == 0) 
    {
        strcpy(out, "+0.0E+0");
        return;
    }
    
    snprintf(out, MAX_LEN+20, "%c0.%sE%+d", sign, num_ptr, exponent);
}

void norm_int(char* str, char* out)
{
    char* ptr = str;
    char sign = get_sign(str[0]);

    if (*ptr == '-' || *ptr == '+') 
    {
        ptr++;
    }

    // Если первые эл-ты 0
    while (*ptr == '0') 
        ptr++;

    // Подсчет количества цифр (без нулей и знака)
    int len = 0;
    while (isdigit(ptr[len]))
        len++;

    // Пусто - значит нуль
    if (len == 0) 
    {
        strcpy(out, "+0.0E+0");
        return;
    }

    snprintf(out, MAX_LEN+20, "%c0.%sE+%d", sign, ptr, len);
}

void norm_exp(const char *str, char *out)
{
    const char *e = str;
    while (*e && *e != 'E' && *e != 'e')
        ++e; // бежим до E

    char mant_str[MAX_LEN];
    size_t mlen = (size_t)(e - str);
    memcpy(mant_str, str, mlen);
    mant_str[mlen] = '\0';

    int add_exp = atoi(e + 1); //Все что после E (e+1) это исходная экспонента

    char mant_norm[MAX_LEN];
    if (strchr(mant_str, '.'))
        norm_double(mant_str, mant_norm);
    else
        norm_int(mant_str, mant_norm);

    char *p = mant_norm + 1; // пропуск знака у уже обработанного нормализованного числа
    while (*p && *p != 'E' && *p != 'e')
        ++p; //Ищем E, которую получили после нормализации числа в norm int/double
    int old_exp = atoi(p + 1);

    int new_exp = old_exp + add_exp;

    *p = '\0';  

    snprintf(out, MAX_LEN+20, "%sE%+d", mant_norm, new_exp);
}

void move_num_to_struct(extended_float *num_struct, char *number)
{
    if (*number == '+' || *number == '-') {
        num_struct->mantissa_sign = *number;
        ++number;
    } 
    else 
    {
        num_struct->mantissa_sign = '+';
    }

    number += 2; // пропуск нуля и точки

    char *p = number; // начало мантиссы (только цифры)
    while (*p && *p != 'E' && *p != 'e')
        ++p; // доходим до экспоненты

    size_t mlen = (size_t)(p - number);
    memcpy(num_struct->mantissa, number, mlen); // записываем только цифры мантиссы
    num_struct->mantissa[mlen] = '\0';

    if (*p) 
    {
        num_struct->exponent_sign = *(p + 1); // сдвигаем знак после E
        num_struct->exponent = atoi(p + 2); // смещаемся на 2: на E и знак
    } else 
    {
        num_struct->exponent_sign = '+';
        num_struct->exponent = 0;
    }
}

int parse_num(extended_float *num)
{
    char scanned_str[MAX_LEN + 20];
    char str_for_num[MAX_LEN + 20];

    if (fgets(scanned_str, sizeof(scanned_str), stdin) == NULL)
        return -1;
    
    scanned_str[strcspn(scanned_str, "\n")] = 0;

    if (!validate_number(scanned_str))
        return 1;

    num->mantissa_sign = get_sign(scanned_str[0]);

    if (strchr(scanned_str, 'E') || strchr(scanned_str, 'e'))
        norm_exp(scanned_str, str_for_num);
    else if (strchr(scanned_str, '.'))
        norm_double(scanned_str, str_for_num);
    else
        norm_int(scanned_str, str_for_num);

    move_num_to_struct(num, str_for_num);
    return 0;
}

int mult_arr(int *a, int len_a, int *b, int len_b, int *res)
{
    int i, j, remember, t;
    int len = 0;
    for (j = 0; j < len_b; ++j) // перебор множителей
    {
        remember = 0;
        for (i = 0; i < len_a; ++i)//на каждую цифру одного множителя умножаем другого
        {
            t = a[i] * b[j] + res[i + j] + remember;
            res[i + j] = t % 10;
            if (res[i + j] != 0 || remember != 0)
                len = i + j + 1; 
                //len указывает длину числа (позицию последней правой ненулевой цифры)
            remember = t / 10;
        }

        if (remember != 0) 
        {
            res[i + j] = remember;
            len = i + j + 1;
        }
    }

    return len;
}

void mul_mant_strings(char *a, char *b, char *out)
{
    int num1[MAX_MANTISSA_LEN-1], num2[MAX_INT_LEN], res_num[MAX_MANTISSA_LEN+MAX_INT_LEN+1] = {0};

    int len_num1 = string_to_arr_num(a, num1);
    int len_num2 = string_to_arr_num(b, num2);

    int res_num_len = mult_arr(num1, len_num1, num2, len_num2, res_num);

    int i = res_num_len - 1, idx = 0;
    while (i >= 0) //переворачиваем обратно число
        out[idx++] = '0' + res_num[i--];
    out[idx] = '\0';
}

void round_mantissa(char *mantissa, int *exponent) {
    int len = strlen(mantissa);
    if (len <= 30) 
    {
        return;
    }

    char rounded[31];
    strncpy(rounded, mantissa, 30);
    rounded[30] = '\0';

    if (mantissa[30] >= '5')
    {
        int i = 29;
        while (i >= 0 && rounded[i] == '9') 
        {
            rounded[i] = '0';
            i--;
        }
        if (i >= 0) 
        {
            rounded[i]++;
        } else 
        {
            rounded[0] = '1';
            for (int j = 1; j < 30; j++) 
            {
                rounded[j] = '0';
            }
            (*exponent)++;
        }
    }
    strcpy(mantissa, rounded);
}

int multiply_long(extended_float *a, extended_float *b, extended_float *res) {
    char mantissa_res[70] = {0};
    mul_mant_strings(a->mantissa, b->mantissa, mantissa_res);

    int L_a = strlen(a->mantissa);
    int L_b = strlen(b->mantissa);
    int L_product = strlen(mantissa_res);

    int exp_a = a->exponent;
    if (a->exponent_sign == '-') 
    {
        exp_a = -exp_a;
    }
    int exp_b = b->exponent;
    if (b->exponent_sign == '-') 
    {
        exp_b = -exp_b;
    }

    res->exponent = exp_a + exp_b + L_product - L_a - L_b;
    round_mantissa(mantissa_res, &res->exponent);
    strcpy(res->mantissa, mantissa_res);

    res->mantissa_sign = (a->mantissa_sign == b->mantissa_sign) ? '+' : '-';

    if (res->exponent > 99999 || res->exponent < -99999) 
    {
        printf("Error: exponent value out of range [-99999, 99999]\n");
        return 1;
    }

    return 0;
}

int main() {
    extended_float num1 = {.mantissa_sign = '+', .mantissa = "0", .exponent_sign = '+', .exponent = 0};
    extended_float num2 = {.mantissa_sign = '+', .mantissa = "0", .exponent_sign = '+', .exponent = 0};
    extended_float result = {.mantissa_sign = '+', .mantissa = "0", .exponent_sign = '+', .exponent = 0};

    printf("Input float num (max 40 digit in mant, E should be UPPER):\n");
    print_lineal(40);
    if (parse_num(&num1) != 0) 
    {
        printf("Input error (float num).\n");
        return 1;
    }

    printf("Input int num (max 30 digits):\n");
    print_lineal(40);
    if (parse_num(&num2) != 0) 
    {
        printf("Input error (int num).\n");
        return 1;
    }

    //printf("Debug: num1_mantissa: %s, num1_exponent: %c%d\n", num1.mantissa, num1.exponent_sign, num1.exponent);
    //printf("Debug: num2_mantissa: %s, num2_exponent: %c%d\n", num2.mantissa, num2.exponent_sign, num2.exponent);

    if (strcmp(num1.mantissa, "0") == 0 || strcmp(num2.mantissa, "0") == 0)
    {
        printf("0");
        return 0;
    }

    if (num2.exponent != strlen(num2.mantissa) || strlen(num2.mantissa) > 30) {
        printf("Input error (int num).\n");
        return 1;
    }

    int rc = multiply_long(&num1, &num2, &result);
    if (!rc)
    {
        printf("Result:\n");
        norm_res(&result);
        print_num_struct(&result);
    }
    return 0;
}