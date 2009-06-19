#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

static int doublecmp(const double * d1, const double * d2) {
	if(*d1 > *d2) return 1;
	if(*d1 < *d2) return -1;
	if(*d1 == *d2) return 0;
}
#define insert(STACK, value) _insert(& STACK, & STACK ## _length, & STACK ## _size, value)
int _insert(double ** stack, int * stack_length, int * stack_size, const double value) {
	if(*stack_size == 0) {
		*stack = (double*)realloc(*stack, 8 * sizeof(double));
		*stack_size = 8;
	}
	if(*stack_length == *stack_size) {
		*stack_size = *stack_size * 2;
		*stack = (double*) realloc(*stack, *stack_size);
	}
	int i = 0;
	for(i = 0; i < *stack_length; i++) {
		int cmp = doublecmp(&(*stack)[i], &value);
		if(cmp == 0) return i;
		if(cmp > 0 ) break;
	}
	memmove(&(*stack)[i + 1], &(*stack)[i], sizeof(double) * (*stack_length - i));
	(*stack)[i] = value;
	(*stack_length) ++;
	return i;
	
}
#define find(STACK, value) _find(STACK, STACK ## _length, value)
int _find(double * stack, int stack_length, double value) {
	double * ptr = (double*) bsearch(&value, stack, stack_length, sizeof(double), (void*) doublecmp);
	if(ptr == NULL) return -1;
	return ptr - stack;
}
#define Z(xi, yi) _Z[Y_length * xi + yi]

double * X = NULL;
int X_length = 0;
int X_size = 0;

double * Y = NULL;
int Y_length = 0;
int Y_size = 0;

double * C = NULL;
int C_length = 0;
int C_size = 0;
double * _Z = NULL;

static void draw_contour(const double c) {
	
}
int main(int argc, char* argv[]) {
	char* tempfile = strdup("/tmp/cntXXXXXX");
	int fd = mkstemp(tempfile);
	FILE * tempfp = fdopen(fd, "r+");
	
	double x, y, z;
	int xi, yi;
	int i;

	if(tempfp == NULL) {
		fprintf(stderr, "Can't open temp file\n");
		return -1;
	}
	/* The coutour levels*/
	for(i = 1; i < argc; i++) {
		insert(C, atof(argv[i]));
	}

	/* The grid */
	while(scanf("%le %le %le", &x, &y, &z) == 3) {
		if((xi = find(X, x)) == -1) insert(X, x);
		if((yi = find(Y, y)) == -1) insert(Y, y);
		fprintf(tempfp, "%le %le %le\n", x, y, z);
	}
	rewind(tempfp);

	/* Load the Z values */
	_Z = calloc(X_length * Y_length, sizeof(double));
	while(fscanf(tempfp, "%le %le %le", &x, &y, &z) == 3) {
		xi = find(X, x);
		yi = find(Y, y);
		Z(xi, yi) = z;
	}
	
	for(i = 0; i < C_length; i++) {
		draw_contour(C[i]);
	}
	for(xi = 0; xi < X_length; xi++) {
		printf("X = %le ", X[xi]);
	}
	printf("\n");
	for(yi = 0; yi < Y_length; yi++) {
		printf("Y = %le ", Y[yi]);
	}
	printf("\n");
	for(xi = 0; xi < X_length; xi++) {
		for(yi = 0; yi < Y_length; yi++) {
			printf("%le ", Z(xi, yi));
		}
		printf("\n");
	}
	fclose(tempfp);
	unlink(tmpfile);
}
