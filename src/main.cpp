 
#include "../include/akinator.h"

int main(void)
{
   struct Akinator *akn = AkinatorCtor();
   ERROR_CHECK(akn == NULL, ERROR_AKINATOR_CTOR);
   
   int akinator_err = RunAkinator(akn);
   ERROR_CHECK(akinator_err, ERROR_AKINATOR);

   return 0;
}
