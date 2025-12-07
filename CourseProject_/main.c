#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>

#define _CRT_SECURE_NO_WARNINGS

typedef struct {
    double xmin, ymin;
    double xmax, ymax;
} Extrema;

double f(double x);
double derivative(double x);
Extrema find_extrema(double a, double b, int steps);
void table_of_values(double a, double b, double step);
double find_x(double a, double b, double Y);
double input_double(const char* msg);

int main() {
    setlocale(LC_CTYPE, "RUS");
    int choice;

    printf("=== Консольное меню ===\n");

    while (1) {
        printf("\nВыберите операцию:\n");
        printf("1 - Значение f(x)\n");
        printf("2 - Таблица значений\n");
        printf("3 - Минимум и максимум\n");
        printf("4 - Поиск x, где f(x) = Y\n");
        printf("5 - Производная f'(x)\n");
        printf("0 - Выход\n> ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода.\n");
            while (getchar() != '\n');
            continue;
        }
        if (choice == 0) break;

        double x, a, b, step, Y;

        switch (choice) {
        case 1:
            x = input_double("Введите x: ");
            printf("f(%.6lf) = %.8lf\n", x, f(x));
            break;

        case 2:
            a = input_double("Введите a: ");

            b = input_double("Введите b: ");
            step = input_double("Введите шаг: ");
            if (step <= 0 || a >= b) {
                printf("Ошибка: неправильный интервал.\n");
                break;
            }
            table_of_values(a, b, step);
            break;

        case 3: {
            a = input_double("Введите a: ");
            b = input_double("Введите b: ");
            if (a >= b) {
                printf("Ошибка: a < b обязательно.\n");
                break;
            }
            Extrema e = find_extrema(a, b, 1000);
            printf("Минимум: f(%.6lf) = %.8lf\n", e.xmin, e.ymin);
            printf("Максимум f(%.6lf) = %.8lf\n", e.xmax, e.ymax);
            break;
        }

        case 4:
            a = input_double("Введите a: ");
            b = input_double("Введите b: ");
            Y = input_double("Введите Y: ");
            x = find_x(a, b, Y);
            printf("x = %.6lf, f(x)=%.8lf\n", x, f(x));
            break;

        case 5:
            x = input_double("Введите x: ");
            printf("f'(%.6lf) = %.8lf\n", x, derivative(x));
            break;

        default:
            printf("Неверный выбор.\n");
        }
    }

    printf("Выход из программы.\n");
    return 0;
}
double f(double x) {
    if (x < 0) {
        double denom = log(3 + fabs(x));
        return sin(x * x) / denom;
    }
    else if (x < 5) {
        return atan(x) * exp(cos(x)) / cbrt(1 + x * x);
    }
    else {
        double sum = 0.0;
        for (int n = 0; n <= 7; n++) {
            double denom = tgamma(n + 1) * log(n + 4);
            sum += pow(-1, n) * pow(x, n) / denom;
        }
        return sum;
    }
}

double derivative(double x) {
    double h = 1e-6;
    return (f(x + h) - f(x - h)) / (2 * h);
}

Extrema find_extrema(double a, double b, int steps) {
    Extrema e;

    double step = (b - a) / steps;

    e.xmin = a;
    e.xmax = a;
    e.ymin = f(a);
    e.ymax = f(a);

    for (int i = 0; i <= steps; i++) {
        double x = a + i * step;
        double y = f(x);

        if (y < e.ymin) { e.ymin = y; e.xmin = x; }
        if (y > e.ymax) { e.ymax = y; e.xmax = x; }
    }
    return e;
}

void table_of_values(double a, double b, double step) {
    printf("\n=============================================\n");
    printf("|     x      |         f(x)                 |\n");
    printf("=============================================\n");

    for (double x = a; x <= b + step / 2; x += step)
        printf("| %10.4lf | %25.8lf    |\n", x, f(x));

    printf("=============================================\n\n");
}

double find_x(double a, double b, double Y) {
    double mid, fm;
    for (int i = 0; i < 100; i++) {
        mid = (a + b) / 2;
        fm = f(mid);
        if (fabs(fm - Y) < 1e-6) break;
        if ((f(a) - Y) * (fm - Y) < 0)
            b = mid;
        else
            a = mid;
    }
    return mid;
}

double input_double(const char* msg) {
    double x;
    printf("%s", msg);
    while (scanf("%lf", &x) != 1) {
        printf("Ошибка ввода. Повторите: ");
        while (getchar() != '\n');
    }
    return x;
}