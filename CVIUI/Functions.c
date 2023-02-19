#include "ExtraVar.h" 
#include <ansi_c.h>


void ExitPoint(void)
{
	if(saveBuf != NULL){free(saveBuf);saveBuf = NULL;}
}
