#include "postgres.h"

#include "fmgr.h"
#include "libpq/pqformat.h"             /* needed for send/recv functions */

PG_MODULE_MAGIC;

typedef struct Complex
{
        int32           x;
        char            data[FLEXIBLE_ARRAY_MEMBER];
}                       Complex;


PG_FUNCTION_INFO_V1(complex_in);

Datum
complex_in(PG_FUNCTION_ARGS)
{
        char    *str = PG_GETARG_CSTRING(0);
        int     length = strlen(str);
        int     num[length];
        int     i,r,v,n;
        i = 0;

        while(1){
                r = sscanf(str,"%d%n",&v,&n);
        if (1 == r){
                num[i++] = v;
                //printf("%d %d\n", v,n);
            str += n;
        }
        else if (0 == r){
            str ++;
        }
        else{
            break;
        }
        }
    if (i > 1){
        int p,q;
        for(p = 0; p < i; p ++){
            for(q = 0; q < i-1-p; q++){
                if (num[q] > num[q+1]){
                    int t = num[q];
                    num[q] = num[q+1];
                    num[q+1] = t;
                }
            }

        }
    }
    int counter = i;
    char *out_s = (char *) malloc (length);
    sprintf(out_s,"{");
    for(n = 0; n < i; n ++){
        if(n!=0){
                if(num[n] == num[n-1]){
                        counter -= 1;
                        continue;
                }
                sprintf(out_s,"%s,",out_s);
        }
        sprintf(out_s,"%s%d",out_s,num[n]);
    }
    sprintf(out_s,"%s}",out_s);

    length = strlen(out_s);

    Complex *result = (Complex *) palloc (VARHDRSZ + length);
    SET_VARSIZE(result, VARHDRSZ + length);

    result -> x = length;
    memcpy(result->data, out_s, length);
    PG_RETURN_POINTER(result);
}

PG_FUNCTION_INFO_V1(complex_out);

Datum
complex_out(PG_FUNCTION_ARGS)
{
        Complex    *complex = (Complex *) PG_GETARG_POINTER(0);
        char    *result = (char *) malloc (complex->x);
        char    *str = (char *) malloc (complex->x);
        memcpy(str, complex->data, complex->x);

        int i,r,v,n;
        int num[complex->x];
        i = 0;

        while(1){
                r = sscanf(str,"%d%n",&v,&n);
                if (1 == r){
                        num[i++] = v;
                //printf("%d %d\n", v,n);
                        str += n;
                }
                else if (0 == r){
                        str ++;
                }
                else{
                        break;
                }
        }

        sprintf(result,"{");
        for (n = 0; n < i; n ++){
                if (n != 0){
                        sprintf(result,"%s,",result);
                }
                sprintf(result,"%s%d",result,num[n]);
        }
        sprintf(result,"%s}",result);

        PG_RETURN_CSTRING(result);
}


PG_FUNCTION_INFO_V1(complex_intersection);

Datum
complex_intersection(PG_FUNCTION_ARGS)
{
    Complex *a = (Complex *) PG_GETARG_POINTER(0);
    Complex *b = (Complex *) PG_GETARG_POINTER(1);
    
    char    *str1 = (char *) malloc (a->x);
    memcpy(str1, a->data, a->x);
    int i1,r,v,n;
    int num1[a->x];
    i1 = 0;
    while(1){
        r = sscanf(str1,"%d%n",&v,&n);
        if (1 == r){
            num1[i1++] = v;
            str1 += n;
        }
        else if (0 == r){
            str1 ++;
        }
        else{
            break;
        }
    }

    char    *str2 = (char *) malloc (b->x);
    memcpy(str2, b->data, b->x);
    int i2 = 0;
    int num2[b->x];
    while(1){
        r = sscanf(str2,"%d%n",&v,&n);
        if (1 == r){
            num2[i2++] = v;
            str2 += n;
        }
        else if (0 == r){
            str2 ++;
        }
        else{
            break;
        }
    }

    int length = b -> x;
    if ((a->x) < (b->x))
        length = a -> x;
    int out_n[length];
    int counter = 0;

    int p = 0,q = 0;
    while(1)
    {
        if(p >= i1)
            break;
        while(1)
        {
            if( q >= i2)
                break;
            if(num1[p] == num2[q]){
                out_n[counter] = num1[p];
                counter += 1;
                break;
            }
            q += 1;
        }
        p += 1;
	q = 0;
    }
    
    char    *result = (char *) malloc (length);
    sprintf(result,"{");
    for (n = 0; n < counter; n ++){
        if (n != 0){
            sprintf(result,"%s,",result);
        }
        sprintf(result,"%s%d",result,out_n[n]);
    }
    sprintf(result,"%s}",result);

    length = strlen(result);

    Complex *complex_result = (Complex *) palloc (VARHDRSZ + length);
    SET_VARSIZE(complex_result, VARHDRSZ + length);
    complex_result -> x = length;
    memcpy(complex_result->data,result, length);

    PG_RETURN_POINTER(complex_result);    
}


PG_FUNCTION_INFO_V1(complex_union);
    
Datum

complex_union(PG_FUNCTION_ARGS)
{
    Complex *a = (Complex *) PG_GETARG_POINTER(0);
    Complex *b = (Complex *) PG_GETARG_POINTER(1);
    
    char    *str1 = (char *) malloc (a->x);
    memcpy(str1, a->data, a->x);
    int i1,r,v,n;
    int num1[a->x];
    i1 = 0;
    while(1){
        r = sscanf(str1,"%d%n",&v,&n);
        if (1 == r){
            num1[i1++] = v;
            str1 += n;
        }
        else if (0 == r){
            str1 ++;
        }
        else{
            break;
        }
    }

    char    *str2 = (char *) malloc (b->x);
    memcpy(str2, b->data, b->x);
    int i2 = 0;
    int num2[b->x];
    while(1){
        r = sscanf(str2,"%d%n",&v,&n);
        if (1 == r){
            num2[i2++] = v;
            str2 += n;
        }
        else if (0 == r){
            str2 ++;
        }
        else{
            break;
        }
    }

    int length = b->x + a->x;
    int out_n[length];
    int counter = a->x;
    memcpy(out_n,num1,i1);
    int p = 0, q = 0; 

    while(1)
    {
 	if(p >= i1)
	    break;
	while(1)
	{
	    if(q >= i2)
		break;
	    if(num2[q] >= num1 [p])
		break;
	    if(num2[q] < num1[p])
	    {
		out_n[counter] = num2[q];
		counter += 1;
	    }
	    q += 1;
	}
	p += 1;
	q = 0;
    }

    q = 0;
    while(1)
    {
	if(q >= i2)
	    break;
	if(num2[q] > num1[i1-1]){
	    out_n[counter] = num2[q];
	    counter += 1;
	}
	q += 1;
    }
    
    char    *result = (char *) malloc (length);
    sprintf(result,"{");
    for (n = 0; n < counter; n ++){
        if (n != 0){
            sprintf(result,"%s,",result);
        }
        sprintf(result,"%s%d",result,out_n[n]);
    }
    sprintf(result,"%s}",result);

    length = strlen(result);

    Complex *complex_result = (Complex *) palloc (VARHDRSZ + length);
    SET_VARSIZE(complex_result, VARHDRSZ + length);
    complex_result -> x = length;
    memcpy(complex_result->data,result, length);

    PG_RETURN_POINTER(complex_result);    
}

