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

void print_lineal(int max_digits) // вывод линейки
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
} //вывод структуры с числом в виде строки в норм виде

int string_to_arr_num(char *s, int *d) //перевод строки в массив чисел
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

    int dot_count = 0;
    int exp_count = 0;
    int exp_index = -1;
    int dot_index = -1;

    char* ptr = s;
    if (*ptr == '+' || *ptr == '-') 
    {
        ptr++;
    }

    // Проверка на пустую строку после знака
    if (!*ptr) {
        return 0;
    }

    while (*ptr && *ptr != 'E' && *ptr != 'e') 
    {
        if (isdigit(*ptr)) 
        {
            // Просто пропускаем, потом посчитаем значащие
        } else if (*ptr == '.') 
        {
            dot_count++;
            dot_index = ptr - s;
        } else 
        {
            return 0; // Недопустимый символ в мантиссе
        }
        ptr++;
    }
    
    // Проверка на E
    if (*ptr) 
    {
        exp_count++;
        exp_index = ptr - s;
        ptr++; // Пропускаем 'E'
        if (*ptr == '+' || *ptr == '-') 
        {
            ptr++;
        }
        // Проверка экспоненты
        int exp_len = 0;
        if (!*ptr) return 0; // Знак экспоненты без цифр
        while (*ptr) 
        {
            if (!isdigit(*ptr)) 
            {
                return 0; // Недопустимый символ в экспоненте
            }
            exp_len++;
            ptr++;
        }
        if (exp_len >= 6) 
        {
            return 0; // Экспонента слишком длинная
        }
    }
    
    // Подсчет значащих цифр в мантиссе
    int mantissa_len = 0;
    char *mantissa_start = s;
    if (*mantissa_start == '+' || *mantissa_start == '-') {
        mantissa_start++;
    }
    
    // Если есть точка, смотрим, где начинаются значащие цифры
    if (dot_count == 1) 
    {
        char *p_after_dot = strchr(mantissa_start, '.') + 1;
        
        // Пропускаем ведущие нули после точки
        char *temp_ptr = p_after_dot;
        while(*temp_ptr == '0' && isdigit(*(temp_ptr + 1))) 
        {
            temp_ptr++;
        }
        
        // Считаем все остальные цифры до 'E'
        while(*temp_ptr && *temp_ptr != 'E' && *temp_ptr != 'e' && isdigit(*temp_ptr)) 
        {
            mantissa_len++;
            temp_ptr++;
        }
        
    } else 
    { // Если нет точки, считаем все цифры
        while (*mantissa_start && *mantissa_start != 'E' && *mantissa_start != 'e')
        {
            if(isdigit(*mantissa_start)){
                 mantissa_len++;
            }
            mantissa_start++;
        }
    }
    
    // Если число равно 0
    if (mantissa_len == 0 && (strcmp(s, "0") == 0 || strcmp(s, "+0") == 0 || strcmp(s, "-0") == 0 || strcmp(s, "0.0") == 0)) 
    {
        return 1;
    }

    if (dot_count > 1 || exp_count > 1 || mantissa_len > 40 || (dot_index > exp_index && exp_index != -1)) 
    {
        return 0;
    }
    
    return 1;
}

void norm_res(extended_float* num) //нормализация результата (при необходимости)
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

void norm_double(char *str, char *out) //нормализация дробного числа
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

void norm_int(char* str, char* out) //норм целого числа
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

void norm_exp(const char *str, char *out) //нормализация числа в экспоненциальном виде 
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

void move_num_to_struct(extended_float *num_struct, char *number) //запись строки в структуру
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

int parse_num(extended_float *num) //обработчик ввода числа
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
        norm_exp(scanned_str, str_for_num); //если есть E - число в эксп. форме
    else if (strchr(scanned_str, '.'))
        norm_double(scanned_str, str_for_num); // если есть . - дробная
    else
        norm_int(scanned_str, str_for_num); //иначе целое

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
            remember = t / 10; //запоминаем перенос
        }

        if (remember != 0) 
        {
            res[i + j] = remember;
            len = i + j + 1;
        } //обновление длины числа после последнего умножения
    }

    return len;
}

void mul_mant_strings(char *a, char *b, char *out) //перемножение мантисс и переворот числа
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

//функция для округления
void round_mantissa(char *mantissa, int *exponent) {
    int len = strlen(mantissa);
    if (len <= 30) 
    {
        return;
    } //округлять не надо

    char rounded[31];
    strncpy(rounded, mantissa, 30);
    rounded[30] = '\0'; 

    //проверка последней цифры
    if (mantissa[30] >= '5')
    {
        int i = 29;
        while (i >= 0 && rounded[i] == '9') 
        { //все 9ки, идущие после числа большего 5, заменяем на 0 
            rounded[i] = '0';
            i--;
        }
        //когда вышли из цикла (либо кончились девятки, либо дошли до конца числа)
        if (i >= 0)  //случай 1: кончились девятки
        {
            rounded[i]++;
        } else //случай 2: дошли до конца числа, значит все число было из девяток 
        //в таком случае нужно старший символ заменить на 2, а остальные - 0, и учесть порядок
        //пример: 36 девяток будут округлены до 1 и порядка 37
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


// обработчик для функций умножения и занесения строки в структуру
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