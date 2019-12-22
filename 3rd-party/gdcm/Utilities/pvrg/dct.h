/*************************************************************
Copyright (C) 1990, 1991, 1993 Andy C. Hung, all rights reserved.
PUBLIC DOMAIN LICENSE: Stanford University Portable Video Research
Group. If you use this software, you agree to the following: This
program package is purely experimental, and is licensed "as is".
Permission is granted to use, modify, and distribute this program
without charge for any purpose, provided this license/ disclaimer
notice appears in the copies.  No warranty or maintenance is given,
either expressed or implied.  In no event shall the author(s) be
liable to you or a third party for any special, incidental,
consequential, or other damages, arising out of the use or inability
to use the program for any purpose (or the loss of data), even if we
have been advised of such possibilities.  Any public reference or
advertisement of this source code should refer to it as the Portable
Video Research Group (PVRG) code, and not by any author(s) (or
Stanford University) name.
*************************************************************/
/*
************************************************************
dct.h

This file contains the basic DCT matrix values for the reference DCT
transform.  It doesn't matter if it is slow because it is rarely used.
A butterfly would be much faster.

************************************************************
*/

#ifndef DCT_DONE
#define DCT_DONE

#define DC_QUARTER 1.41421356237309504880
#define DC_THREE_EIGHTHS 0.76536686473017954348
#define DC_ONE_EIGHTH 1.84775906502257351226

/* Bitshifted Twelve */

#define IC_QUARTER 2896
#define IC_THREE_EIGHTHS 1567
#define IC_ONE_EIGHTH 3784


double DctMatrix[] = {
0.3535533905932737,
0.3535533905932737,
0.3535533905932737,
0.3535533905932737,
0.3535533905932737,
0.3535533905932737,
0.3535533905932737,
0.3535533905932737,
0.4903926402016152,
0.4157348061512727,
0.2777851165098011,
0.0975451610080642,
-0.0975451610080641,
-0.2777851165098010,
-0.4157348061512727,
-0.4903926402016152,
0.4619397662556434,
0.1913417161825449,
-0.1913417161825449,
-0.4619397662556434,
-0.4619397662556434,
-0.1913417161825452,
0.1913417161825450,
0.4619397662556432,
0.4157348061512727,
-0.0975451610080641,
-0.4903926402016152,
-0.2777851165098011,
0.2777851165098009,
0.4903926402016153,
0.0975451610080640,
-0.4157348061512721,
0.3535533905932738,
-0.3535533905932737,
-0.3535533905932738,
0.3535533905932737,
0.3535533905932738,
-0.3535533905932733,
-0.3535533905932736,
0.3535533905932733,
0.2777851165098011,
-0.4903926402016152,
0.0975451610080641,
0.4157348061512728,
-0.4157348061512726,
-0.0975451610080649,
0.4903926402016152,
-0.2777851165098008,
0.1913417161825449,
-0.4619397662556434,
0.4619397662556432,
-0.1913417161825449,
-0.1913417161825453,
0.4619397662556437,
-0.4619397662556435,
0.1913417161825431,
0.0975451610080642,
-0.2777851165098011,
0.4157348061512728,
-0.4903926402016153,
0.4903926402016152,
-0.4157348061512720,
0.2777851165098022,
-0.0975451610080625};
static double IDctMatrix[] = {
0.3535533905932737,
0.4903926402016152,
0.4619397662556434,
0.4157348061512727,
0.3535533905932738,
0.2777851165098011,
0.1913417161825449,
0.0975451610080642,
0.3535533905932737,
0.4157348061512727,
0.1913417161825449,
-0.0975451610080641,
-0.3535533905932737,
-0.4903926402016152,
-0.4619397662556434,
-0.2777851165098011,
0.3535533905932737,
0.2777851165098011,
-0.1913417161825449,
-0.4903926402016152,
-0.3535533905932738,
0.0975451610080641,
0.4619397662556432,
0.4157348061512728,
0.3535533905932737,
0.0975451610080642,
-0.4619397662556434,
-0.2777851165098011,
0.3535533905932737,
0.4157348061512728,
-0.1913417161825449,
-0.4903926402016153,
0.3535533905932737,
-0.0975451610080641,
-0.4619397662556434,
0.2777851165098009,
0.3535533905932738,
-0.4157348061512726,
-0.1913417161825453,
0.4903926402016152,
0.3535533905932737,
-0.2777851165098010,
-0.1913417161825452,
0.4903926402016153,
-0.3535533905932733,
-0.0975451610080649,
0.4619397662556437,
-0.4157348061512720,
0.3535533905932737,
-0.4157348061512727,
0.1913417161825450,
0.0975451610080640,
-0.3535533905932736,
0.4903926402016152,
-0.4619397662556435,
0.2777851165098022,
0.3535533905932737,
-0.4903926402016152,
0.4619397662556432,
-0.4157348061512721,
0.3535533905932733,
-0.2777851165098008,
0.1913417161825431,
-0.0975451610080625};

#endif
