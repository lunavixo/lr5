#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double f(double x);
void printTable(double *data, unsigned int point_count);
void printTableN(double *data, unsigned int point_count);
void pressEnterToContinue();
void fillTable(double **data,
               unsigned int *point_count,
               double x_start,
               double x_end,
               double delta);

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    double *data = NULL;
    double x_start = 0;
    double x_end = 0;
    double delta = 0;
    unsigned int points_count = 0;
    char *group_code = "\0";
    char *first_name = "\0";
    char *last_name = "\0";

    FILE *settings = fopen("C:\\Users\\Nazar\\Documents\\infka\\lr5\\settings.txt", "r");
    printf("%s", settings == NULL ? "Y" : "N");

    while(true)
    {
        system("cls");
        printf("Лабораторна робота №5 \"Програмні потоки для роботи з файлами\"\n"
               "Шифр групи: %s\n"
               "%s %s\n"
               "Чи є в пам'яті програми точки таблиці? %s\n\n"
               "Введіть номер дії:\n"
               "\t0 - вийти\n"
               "\t1 - зчитати дані з текстового файлу\n"
               "\t2 - зчитати точки з бінарного файлу\n"
               "\t3 - зберегти таблицю в текстовий файл\n"
               "\t4 - зберегти точки в бінарний файл\n"
               "\t5 - вивести таблицю точок в термінал\n"
               ": ", group_code, first_name, last_name,
               data == NULL || points_count == 0 ? "Ні" : "Так");
        unsigned int t;
        scanf("%u", &t);
        switch(t)
        {
        case 0:
            return 0;
        case 1:
            printf("Не реалізовано 1\n");
            pressEnterToContinue();
            break;
        case 2:
            printf("Не реалізовано 2\n");
            pressEnterToContinue();
            break;
        case 3:
            printf("Не реалізовано 3\n");
            pressEnterToContinue();
            break;
        case 4:
            printf("Не реалізовано 4\n");
            pressEnterToContinue();
            break;
        case 5:
            printf("Не реалізовано 5\n");
            pressEnterToContinue();
            break;
        }
    }
    fillTable(&data, &points_count, 1, 2, 0.11);

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

    double *data = NULL;
    double x_start = 0;
    double x_end = 0;
    double delta = 0;
    unsigned int points_count = 0;
    char *group_code = "\0";
    char *first_name = "\0";
    char *last_name = "\0";
// Зчитати налаштування з файлу
void readSettings(char *file_path,
                  double *x_start,
                  double *x_end,
                  double *delta,
                  unsigned int *points_count,
                  char **group_code,
                  char **first_name,
                  char **last_name)
{

}

// Заповнює таблицю за початковим, кінцевим аргументами та на вибір або кількістю точок, або кроком зміни
void fillTable(double **data,
               unsigned int *point_count,
               double x_start,
               double x_end,
               double delta)
{
    if(point_count == NULL
       || data == NULL
       || x_end < x_start
       || (*point_count == 0 && delta <= 0)
       || (*point_count != 0 && delta > 0))
        return;

    unsigned int inside_point_count = *point_count > 0 ? *point_count : ((x_end - x_start) / delta + 1);
    double inside_delta = delta > 0 ? delta : ((x_end - x_start) / (*point_count - 1));

    *data = malloc(inside_point_count * 2 * sizeof(double));

    for(unsigned int i = 0; i < inside_point_count; i++)
    {
        (*data)[i * 2] = x_start + inside_delta * i;
        (*data)[i * 2 + 1] = f((*data)[i * 2]);
    }

    *point_count = inside_point_count;
}
//Функція залежності y від x за умовою варіанту 2
double f(double x){
    return 0.25*pow(x, 5) + 4.0 * pow(x + 20.0, 4) - 2.0 * x;
}

//Виведення таблиці на екран
void printTable(double *data, unsigned int point_count){

    if(point_count == 0 || data == NULL) return;
    printf("╔═══════════════════════╤═══════════════════════╗\n");
    printf("║           x           │           y           ║\n");
    printf("╠═══════════════════════╪═══════════════════════╣\n");
    for(unsigned int i = 0; i < point_count; i++){
        printf("║%23.8lf│%23.8lf║\n", data[i*2], data[i*2+1]);
        if(i == point_count - 1)
            printf("╚═══════════════════════╧═══════════════════════╝\n");
        else
            printf("╟───────────────────────┼───────────────────────╢\n");
    }
}

//Виведення таблиці на екран з номером точки
void printTableN(double *data, unsigned int point_count){

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
