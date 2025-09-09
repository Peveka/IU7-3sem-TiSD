#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_MANTISSA_LEN 41
#define MAX_LEN 100
#define MAX_MAN_AFTER_MULTY 30

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

void print_lineal(int max_digits) {
    for (int i = 1; i <= max_digits; i++) {
        if (i % 10 == 0) {
            printf("%d", i / 10);
        } else {
            printf(" ");
        }
    }
    printf("\n");

    for (int i = 1; i <= max_digits; i++) {
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

    int dot_count = 0;
    int dot_index = 0;
    int exp_count = 0;
    int exp_index = 99999;
    int len_num = 0;

    int len_int = 0;
    int unknown_detected = 0;
    char* ptr = (s[0] == '+' || s[0] == '-' ? s+1 : s);
    while (*ptr && *ptr == '0')
        ptr++; //пропуск ведущих нулей

    int int_seen = 0;          //была ли ненулевая целая часть
    while (*ptr) {
        if (isdigit(*ptr)) {
            len_num++;
            if (*ptr != '0')
                int_seen = 1;   //нашли целую часть
            len_int++;
        }
        else if (*ptr == 'E' || *ptr == 'e')
        {
            exp_count++;
            exp_index = ptr - s;
        }

        else if (*ptr == '.')
        {
            dot_count++;
            dot_index = ptr - s;
        }
        else if ((*ptr == '-' || *ptr == '+') && (*(ptr-1) == 'E' || *(ptr-1) == 'e'))
        {
            ptr++;
            continue;
        }
        else
            unknown_detected = 1;
        ptr++;
    }

    //если целой части не было, убираем ведущие нули после точки
    if (!int_seen && dot_count){
        ptr = (s[0] == '+' || s[0] == '-' ? s+1 : s) + dot_index + 1; //сразу после точки
        while (*ptr == '0')
        {
            ptr++;
            len_num--;
        }   //не считаем нули значащими, если не было целой части
    }

    if (len_num == 0 || dot_count > 1 || exp_count > 1 || unknown_detected || dot_index > exp_index ||
        len_num > 40)
        return 0;

    if (exp_count == 1 && exp_index) //смотрим дальше после буквы E
    {
        ptr = s + exp_index + 1; //пропускаем букву Е
        int after_exp_count = 0;
        while (*ptr)
        {
            if (*ptr == '+' || *ptr == '-')
            {
                ptr++;
                continue;
            }
            else if (isdigit(*ptr++))
                after_exp_count++;
            else
                unknown_detected = 1;
        }

        if (after_exp_count > 5 || unknown_detected)
            return 0;
    }

    return 1;
}

void norm_double(char *str, char *out)
{
    char *ptr = str;
    char sign = get_sign(str[0]);
    char exp_sign = '+';
    char number[MAX_LEN];
    int idx = 0;

    if (*ptr == '-' || *ptr == '+')
        ptr++;

    //пропуск ведущих нулей
    while (*ptr == '0')
        ptr++;

    int int_len = 0;
    while (isdigit(*ptr)) {
        number[idx++] = *ptr++;
        int_len++;
    }

    int float_len = 0;
    int first_num_flag = 0; //флаг если после точки 0 и целая часть нулевая
    //считывание дробной части
    if (*ptr == '.') {
        ptr++;
        if (*ptr == '0' && int_len == 0)
        {
            exp_sign = '-';
            first_num_flag = 1;
        }
        while (isdigit(*ptr)) {
            if (*ptr == '0' && first_num_flag) {
                float_len++;
                ptr++;
                continue;
            }
            else {
                number[idx++] = *ptr++;
                first_num_flag = 0;
            }
        }
    }
    number[idx] = '\0';

    // если нуль
    if (idx == 0) {
        strcpy(out, "+0.0E+0");
        return;
    }

    snprintf(out, MAX_LEN+20, "%c0.%sE%c%d", sign, number, exp_sign, exp_sign == '+' ? int_len : float_len);
}

void norm_int(char* str, char* out)
{
    char* ptr = str;
    char sign = get_sign(str[0]);

    // Если 1 эл-т - знак
    if (*ptr == '-' || *ptr == '+') {
        ptr++;
    }

    // Если первые эл-ты 0
    while (*ptr == '0') ptr++;

    // Подсчет количества цифр (ptr ведет на начало числа без нулей и знака)
    int len = 0;
    while (isdigit(ptr[len]))
        len++;

    // Пусто - значит нуль
    if (len == 0) {
        strcpy(out, "+0.0E+0");
        return;
    }

    snprintf(out, MAX_LEN+20, "%c0.%sE+%d", sign, ptr, len);
}

void norm_exp(const char *str, char *out)
{
    // бежим до экспоненты
    const char *e = str;
    while (*e && *e != 'E' && *e != 'e')
        ++e;

    // сохраняем всё без экспоненты
    char mant_str[MAX_LEN];
    size_t mlen = (size_t)(e - str);
    memcpy(mant_str, str, mlen);
    mant_str[mlen] = '\0';

    // изначальную экспоненту сохраняем
    int add_exp = atoi(e + 1);

    // норм. мантиссы: если есть . - значит до экспоненты дробное, иначе - целое число
    char mant_norm[MAX_LEN];
    if (strchr(mant_str, '.'))
        norm_double(mant_str, mant_norm);
    else
        norm_int(mant_str, mant_norm);

    // пробегаем первый символ (+ или -) и дальше до E
    char *p = mant_norm + 1;
    while (*p && *p != 'E' && *p != 'e')
        ++p;
    int old_exp = atoi(p + 1);     // получаем старый порядок (экспоненту)

    // высчитываем итоговый порядок
    int new_exp = old_exp + add_exp;

    // обрезаем строку сразу после цифр мантиссы
    *p = '\0';

    snprintf(out, MAX_LEN+20, "%sE%+d", mant_norm, new_exp);
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
            res[i + j] = t % 10; // в клеточку пишем только посл. цифру
            if (res[i + j] != 0)
                len = i + j + 1; //запоминаем последний ненулевой разряд
            remember = t / 10; //запоминаем старший разряд
        }
        res[i + j] = remember; // перенос в самый старший разряд (после умножения)
    }
    return len; //возвращаем длину результирующего массива - то есть номер старшего ненулевого разряда, который запомнили
}

void round_struct(int *d, int *len)
{
    if (*len <= MAX_MAN_AFTER_MULTY) //не нужно округлять
        return;

    if (d[MAX_MAN_AFTER_MULTY] >= 5) //проверка последней цифры
    {
        int rem = 1;
        for (int i = 0; i < MAX_MAN_AFTER_MULTY+1 && rem; ++i)
        { //добавляем к младшему единицу и проверяем, не переполнили ли какую-то ячейку
            int t = d[i] + rem;
            d[i] = t % 10;
            rem = t / 10;
        }
        if (rem) { // появился 31-й разряд
            d[MAX_MAN_AFTER_MULTY] = 1;
            *len = MAX_MAN_AFTER_MULTY;
            return;
        }
    }
    *len = MAX_MAN_AFTER_MULTY; //Если последняя меньше 5 - просто отбрасываем
}

void mul_mant_strings(char *a, char *b, char *out)
{
    printf("Умножаем мантиссы: '%s' * '%s'\n", a, b); // Добавить эту строку

    int num1[MAX_MAN_AFTER_MULTY*2], num2[MAX_MAN_AFTER_MULTY*2], res_num[MAX_MAN_AFTER_MULTY*2] = {0};

    int len_num1 = string_to_arr_num(a, num1);
    int len_num2 = string_to_arr_num(b, num2);

    printf("Длины массивов: %d * %d\n", len_num1, len_num2); // Добавить эту строку

    int res_num_len = mult_arr(num1, len_num1, num2, len_num2, res_num);

    printf("Длина результата до округления: %d\n", res_num_len); // Добавить эту строку

    round_struct(res_num, &res_num_len);

    // Проверяем длину мантиссы после округления
    if (res_num_len > MAX_MAN_AFTER_MULTY) {
        printf("Ошибка: мантисса превышает 30 символов после округления\n");
        return;
    }

    // перевод результата обратно в строчку (с конца)
    int i = res_num_len - 1, idx = 0;
    while (i >= 0)
        out[idx++] = '0' + res_num[i--];
    out[idx] = '\0';

    printf("Результат умножения мантисс: '%s'\n", out); // Добавить эту строку
}

int multiply_long(extended_float *a, extended_float *b, extended_float *res)
{
    // перемнож мантиссы
    char mantissa_res[31];
    mul_mant_strings(a->mantissa, b->mantissa, mantissa_res);

    // Округляем мантиссу
    int mant_len = strlen(mantissa_res);
    if (mant_len > MAX_MAN_AFTER_MULTY) {
        // Порядок увеличивается на 1
        res->exponent = a->exponent + b->exponent + 1;
        mantissa_res[MAX_MAN_AFTER_MULTY] = '\0';
    } else {
        res->exponent = a->exponent + b->exponent;
    }

    // Копируем результат мантиссы в структуру
    strcpy(res->mantissa, mantissa_res);

    // Определяем знак результата
    res->mantissa_sign = (a->mantissa_sign == b->mantissa_sign) ? '+' : '-';

    // Проверяем длину порядка
    if (res->exponent > 99999 || res->exponent < -99999) {
        printf("Ошибка: порядок выходит за пределы допустимого диапазона [-99999, 99999]\n");
        return 1;
    }

    return 0;
}

void move_num_to_struct(extended_float *num_struct, char *number)
{
    // Определяем знак числа
    if (*number == '+' || *number == '-') {
        num_struct->mantissa_sign = *number;
        ++number;
    } else {
        num_struct->mantissa_sign = '+';
    }

    number += 2; // пропуск нуля и точки

    char *p = number; // начало мантиссы (только цифры)
    while (*p && *p != 'E' && *p != 'e')
        ++p; // доходим до экспоненты

    size_t mlen = (size_t)(p - number);
    memcpy(num_struct->mantissa, number, mlen); // записываем только цифры мантиссы
    num_struct->mantissa[mlen] = '\0';

    num_struct->exponent_sign = *(p + 1); // сдвигаем знак после E
    num_struct->exponent = atoi(p + 2); // смещаемся на 2: на E и знак
}

int parse_num(extended_float *num)
{
    char scanned_str[MAX_LEN+20];
    char str_for_num[MAX_LEN+20];

    if (scanf("%s", scanned_str) != 1)
        return -1; //ошибка ввода

    if (!validate_number(scanned_str)) //проверка числа
        return 1;

    num->mantissa_sign = get_sign(scanned_str[0]);

    if (strchr(scanned_str, 'E') || strchr(scanned_str, 'e')) //запись эксп
        norm_exp(scanned_str, str_for_num);
    else if (strchr(scanned_str, '.')) //дробная запись
        norm_double(scanned_str, str_for_num);
    else
        norm_int(scanned_str, str_for_num);

    move_num_to_struct(num, str_for_num);
    return 0;
}

int main() {
    extended_float num1, num2, result;

    printf("Input float num (max 40 digit in mant, E should be UPPER):\n");
    print_lineal(40);
    if (parse_num(&num1) != 0) {
        printf("Input error (float num).\n");
        return 1;
    }

    printf("Input int num (max 30 digits):\n");
    print_lineal(40);
    if (parse_num(&num2) != 0 || num2.exponent != strlen(num2.mantissa) || strlen(num2.mantissa) > 30) {
        printf("Input error (int num).\n");
        return 1;
    }

    multiply_long(&num1, &num2, &result);

    printf("Result:\n");
    print_num_struct(&result);

    return 0;
}