 
#include "../include/akinator.h"

int main(void)
{
   //SetConsoleCP(1251);
   //SetConsoleOutputCP(1251);
   //setlocale(LC_ALL, "Rus");
   //setlocale (LC_ALL, "ru_RU.cp1251");
   //system("chcp 1251");
   
   int akinator_err = StartAkinator();
   ERROR_CHECK(akinator_err, ERROR_AKINATOR);

   return 0;
}
