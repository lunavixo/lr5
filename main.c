#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SETTINGS_PATH "settings.txt"
#define TABLE_OTPUT_PATH "table.txt"
#define BINARY_OUTPUT_PATH "data.bin"

double f(double x);
void printTable(double *data, unsigned int point_count);
void printTableToFile(double *data, unsigned int point_count, const char *file_path);
void pressEnterToContinue();
void fillTable(double **data,
               unsigned int *point_count,
               double x_start,
               double x_end,
               double *delta);
void readSettings(const char *file_path,
                  double *x_start,
                  double *x_end,
                  double *delta,
                  unsigned int *points_count,
                  char group_code[50],
                  char first_name[50],
                  char last_name[50]);
void saveBinary(const char *file_path, double *data, unsigned int points_count);
void readBinary(const char *file_path, double **data, unsigned int *points_count);


int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    double *data = NULL;
    double x_start = 0;
    double x_end = 0;
    double delta = 0;
    unsigned int points_count = 0;
    char group_code[50] = "\0";
    char first_name[50] = "\0";
    char last_name[50] = "\0";

    while(true)
    {
        system("cls");
        printf("Лабораторна робота №5 \"Програмні потоки для роботи з файлами\"\n"
               "Шифр групи: %s\n"
               "%s %s\n"
               "Чи є в пам'яті програми точки таблиці? %s\n"
               "delta = %lf, x_0 = %lf, x = %lf, points_count = %u\n"
               "Введіть номер дії:\n"
               "\t0 - вийти\n"
               "\t1 - зчитати дані з текстового файлу\n"
               "\t2 - зчитати точки з бінарного файлу\n"
               "\t3 - зберегти таблицю в текстовий файл\n"
               "\t4 - зберегти точки в бінарний файл\n"
               "\t5 - вивести таблицю точок в термінал\n"
               "\t6 - створити точки таблиці з налаштувань\n"
               ": ", group_code, first_name, last_name,
               data == NULL || points_count == 0 ? "Ні" : "Так",
               delta, x_start, x_end, points_count);
        unsigned int t;
        scanf("%u", &t);
        switch(t)
        {
        case 0:
            return 0;
        case 1:
            readSettings(SETTINGS_PATH,
                         &x_start,
                         &x_end,
                         &delta,
                         &points_count,
                         group_code,
                         first_name,
                         last_name);
            pressEnterToContinue();
            break;
        case 2:
            free(data);
            data = NULL;
            readBinary(BINARY_OUTPUT_PATH, &data, &points_count);
            pressEnterToContinue();
            break;
        case 3:
            printTableToFile(data, points_count, TABLE_OTPUT_PATH);
            pressEnterToContinue();
            break;
        case 4:
            saveBinary(BINARY_OUTPUT_PATH, data, points_count);
            pressEnterToContinue();
            break;
        case 5:
            system("cls");
            printf("Таблиця точок\n");
            printTable(data, points_count);
            pressEnterToContinue();
            break;
        case 6:
            free(data);
            data = NULL;
            fillTable(&data, &points_count, x_start, x_end, &delta);
            pressEnterToContinue();
            break;
        }
    }

    //printTable(data, points_count);
    //printTableN(data, points_count);

    // Зчитати вхідні дані з файлу
    // Створити на їх основі таблицю
    // Запис оформленої таблиці та ім'я прізвища в текстовий файл
    // Зберегти таблицю в бінарний файл
    // Зчитати бінарний файл та витягти з неї таблицю
    // Вивести таблицю з номерами точок
    return 0;
}

// Зчитує бінарний файл з точками кількістю points_count за шляхом file_path в масив data
void readBinary(const char *file_path, double **data, unsigned int *points_count)
{
    if(points_count == NULL)
    {
        printf("Помилка points_count NULL\n");
    }
    FILE *fp = fopen(file_path, "rb");
    if(fp == NULL)
    {
        printf("Помилка відкриття файла\n");
        return;
    }

    if(fread(points_count, sizeof(unsigned int), 1, fp) != 1)
    {
        if(feof(fp))
        {
            printf("Досягнуто кінець передчасно 1\n");
            fclose(fp);
            return;
        }
        else
        {
            printf("Помилка зчитування файлу 1\n");
            fclose(fp);
            return;
        }
    }

    if(points_count == 0)
    {
        printf("В бінарному файлі збережених точок немає\n");
        fclose(fp);
        return;

    }

    *data = malloc(sizeof(double) * 2 * (size_t)*points_count);
    if(*data == NULL)
    {
        printf("Помилка виділення пам'яті data\n");
        fclose(fp);
        return;
    }

    if(fread(*data, sizeof(double), *points_count * 2, fp) != (size_t)*points_count * 2)
    {
        if(feof(fp))
        {
            printf("Досягнуто кінець передчасно 2\n");
            free(*data);
            fclose(fp);
            return;
        }
        else
        {
            printf("Помилка зчитування файлу 2\n");
            free(*data);
            fclose(fp);
            return;
        }
    }

    fclose(fp);
    printf("Дані успішно зчитано\n");
}

// Зберігає дані data точок кількістю points_count в бінарний файл за шляхом file_path
void saveBinary(const char *file_path, double *data, unsigned int points_count)
{
    FILE *fp = fopen(file_path, "wb");
    if(fp == NULL)
    {
        printf("Помилка відкриття файла\n");
        return;
    }

    if(fwrite(&points_count, sizeof(unsigned int), 1, fp) != 1)
    {
        printf("Помилка під час збереження бінарного файлу\n");
        fclose(fp);
        return;
    }
    if(fwrite(data, sizeof(double), points_count * 2, fp) != points_count * 2)
    {
        printf("Помилка під час збереження бінарного файлу\n");
        fclose(fp);
        return;
    }

    fclose(fp);
    printf("Бінарний файл збережено\n");
}

// Зчитати налаштування з файлу
void readSettings(const char *file_path,
                  double *x_start,
                  double *x_end,
                  double *delta,
                  unsigned int *points_count,
                  char *group_code,
                  char *first_name,
                  char *last_name)
{
    FILE *fp = fopen(file_path, "r");

    if(fp == NULL)
    {
        printf("Помилка відкриття файла\n");
        return;
    }

    fscanf(fp, "x_start=%lf\n", x_start);
    fscanf(fp, "x_end=%lf\n", x_end);
    fscanf(fp, "delta=%lf\n", delta);
    fscanf(fp, "points_count=%u\n", points_count);
    fscanf(fp, "group_code=%49s\n", group_code);
    fscanf(fp, "first_name=%49s\n", first_name);
    fscanf(fp, "last_name=%49s\n", last_name);

    printf("Параметри зчитано\n");

    fclose(fp);
}

// Заповнює таблицю за початковим, кінцевим аргументами та на вибір або кількістю точок, або кроком зміни
void fillTable(double **data,
               unsigned int *point_count,
               double x_start,
               double x_end,
               double *delta)
{
    if(point_count == NULL
       || x_end < x_start
       || (*point_count == 0 && *delta <= 0)
       || (*point_count != 0 && *delta > 0))
       {
           printf("Помилка аргументу функції\n");
            return;
       }

    unsigned int inside_point_count = *point_count > 0 ? *point_count : ((x_end - x_start) / *delta + 1);
    double inside_delta = *delta > 0 ? *delta : ((x_end - x_start) / (*point_count - 1));

    *data = malloc(inside_point_count * 2 * sizeof(double));
    if(*data == NULL)
    {
        printf("Помилка відкриття файла\n");
        return;
    }

    for(unsigned int i = 0; i < inside_point_count; i++)
    {
        (*data)[i * 2] = x_start + inside_delta * i;
        (*data)[i * 2 + 1] = f((*data)[i * 2]);
    }

    *point_count = inside_point_count;

    printf("Таблицю заповнено\n");
}
//Функція залежності y від x за умовою варіанту 2
double f(double x){
    return 0.25*pow(x, 5) + 4.0 * pow(x + 20.0, 4) - 2.0 * x;
}

//Виведення таблиці на екран
void printTableToFile(double *data, unsigned int point_count, const char *file_path){

    if(point_count == 0 || data == NULL  || file_path == NULL) return;

    FILE *fp = fopen(file_path, "w");
    if(fp == NULL)
    {
        printf("Помилка відкриття файла\n");
        return;
    }

    fprintf(fp, "╔═══════════════════════╤═══════════════════════╗\n");
    fprintf(fp, "║           x           │           y           ║\n");
    fprintf(fp, "╠═══════════════════════╪═══════════════════════╣\n");
    for(unsigned int i = 0; i < point_count; i++){
        fprintf(fp, "║%23.8lf│%23.8lf║\n", data[i*2], data[i*2+1]);
        if(i == point_count - 1)
            fprintf(fp, "╚═══════════════════════╧═══════════════════════╝\n");
        else
            fprintf(fp, "╟───────────────────────┼───────────────────────╢\n");
    }

    fclose(fp);
    printf("Таблицю збережено в текстовий файл за шляхом: %s", file_path);
}

//Виведення таблиці на екран з номером точки
void printTable(double *data, unsigned int point_count)
{
    if(point_count == 0 || data == NULL) return;
    printf("╔═══════════════════════╤═══════════════════════╤═══════════════════════╗\n");
    printf("║           N           │           x           │           y           ║\n");
    printf("╠═══════════════════════╪═══════════════════════╪═══════════════════════╣\n");
    for(unsigned int i = 0; i < point_count; i++){
        printf("║%23u│%23.8lf│%23.8lf║\n", i, data[i*2], data[i*2+1]);
        if(i == point_count - 1)
            printf("╚═══════════════════════╧═══════════════════════╧═══════════════════════╝\n");
        else
            printf("╟───────────────────────┼───────────────────────┼───────────────────────╢\n");
    }
}

//Очікує Enter від користувача щоб продовжити
void pressEnterToContinue() {
    printf("\nНатисніть Enter щоб продовжити...");

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    getchar();
}
