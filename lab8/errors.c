#include "errors.h"

const char* error_message(error_t err) {
    switch (err) {
        case OK: 
            return "Успешно";
        case ERR_NOT_ENOUGH_DATA: 
            return "Недостаточно данных";
        case ERR_INVALID_DATA: 
            return "Неверные данные";
        case ERR_MEM_ALLOC: 
            return "Ошибка выделения памяти";
        case ERR_FILE_OPEN: 
            return "Ошибка открытия файла";
        case ERR_GRAPH_EMPTY: 
            return "Граф пуст";
        case ERR_VERTEX_NOT_FOUND: 
            return "Вершина не найдена";
        case ERR_EDGE_EXISTS: 
            return "Ребро уже существует";
        case ERR_EDGE_NOT_FOUND: 
            return "Ребро не найдено";
        case ERR_NEGATIVE_CYCLE:
            return "Обнаружен отрицательный цикл";
        default: 
            return "Неизвестная ошибка";
    }
}