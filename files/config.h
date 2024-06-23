#ifndef CONFIG_H_
#define CONFIG_H_

#include "bool.h"
#include "todo.h"
#define CONFIG_NAME "./.todo.conf"

bool  Is_There_Config	(void 				);
FILE* Create_Config_File(void 				);
void  Add_To_Config	(Todo_Files* , int		);
void  Load_File		(Todo_Files* , int*, int	);

#endif
