#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_DEFAULT_CAP   10
#define VEC_DEFAULT_SCALE 2

typedef struct Line {
	double year;
	char*  region;
	double natural_grow;
	double birth_rate;
	double death_rate;
	double general_demographic_weight;
	double urbanization;
} Line;

typedef struct Vector {
	size_t  count;
	size_t  cap;
	Line**  data;
} Vector;

typedef struct CSV_FILE {
	Vector lines; 
	size_t lines_count;
	size_t errors;
} CSV_FILE;

Vector v_get()
{
	Line** data = (Line**)calloc(VEC_DEFAULT_CAP, sizeof(Line*));
	return (Vector){
		.data  = data, 
		.cap   = VEC_DEFAULT_CAP,
		.count = 0
	};
}

void v_push_ptr(Vector* v, Line* item_ptr)
{
	if(!v) return;
	if(v->cap == v->count){
		v->cap *= VEC_DEFAULT_SCALE;	
		Line** new_data = (Line**)calloc(v->cap*VEC_DEFAULT_SCALE, sizeof(Line*));
		memcpy(new_data, v->data, sizeof(Line*)*count);
		free(v->data);
		v->data = new_data;
	}
	v->data[count++] = item;
}

Line* v_item(Vector* v, size_t idx) { return idx < v->count? v->data[i] : NULL; }
void v_destroy(Vector* v)
{
	for(int i = 0; i < v->count; i++){
		free(v->data[i]->region);
		free(v->data[i]);
	}
	free(v->data);
}
int main()
{
	Vector v = v_get();
	/////////////////////
	Line* line1 = calloc(1, sizeof(Line)); 
	line1->region = calloc(10, sizeof(char));
	strcpy(line1->region, "egyped");
	line1->year = 1987;

	Line* line2 = calloc(1, sizeof(Line)); 
	line2->region = calloc(10, sizeof(char));
	strcpy(line2->region, "Moscow");
	line1->year = 1946;
	/////////////////////
	v_push_ptr(&v, line1);	
	v_push_ptr(&v, line2);	
	for(int i = 0; i < v.count; i++){
		printf("Region: %s, year: %d\n", v_item(&v, i)->region, v_item(&v, i)->year);
	}
	v_destroy(&v);

	return 0;
}
