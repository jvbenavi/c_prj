/* deadband.c */

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* function declarations */
#include "utils.h"

/****************************************************************************/

double deadband(double value, double min, double max)
{
    double new_value;

    if (value < min)
        new_value = value + min;
    else if (value > max)
        new_value = value - max;
    else
        new_value = 0.0;
        
    return(new_value);
}
