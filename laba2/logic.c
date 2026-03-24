#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_CAP   10
#define VEC_SCALE 2
#define BUFF_SIZE 1024

typedef struct Line {
	int	   year;
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

Vector v_init()
{
	Line** data = (Line**)calloc(VEC_CAP, sizeof(Line*));
	return (Vector){
		.data  = data, 
		.cap   = VEC_CAP,
		.count = 0
	};
}

void v_push_ptr(Vector* v, Line* item_ptr)
{
	if(!v) return;
	if(v->cap == v->count){
		v->cap *= VEC_SCALE;	
		Line** new_data = (Line**)calloc(v->cap*VEC_SCALE, sizeof(Line*));
		memcpy(new_data, v->data, sizeof(Line*)*v->count);
		free(v->data);
		v->data = new_data;
	}
	v->data[v->count++] = item_ptr;
}

Line* v_item(Vector* v, size_t idx)
{
	return idx < v->count? v->data[idx] : NULL;
}

void v_destroy(Vector* v)
{
	for(int i = 0; i < v->count; i++){
		if (v->data[i]){
			free(v->data[i]->region);
			free(v->data[i]);
			v->data[i] = NULL;
		}
	}
	free(v->data);
}

size_t char_count(char* str, char c)
{
	if (!str) return 0;
	size_t count = 0;
	for (char* ptr = str; *ptr; ptr++) if (*ptr == c) count++;
	return count;
}

int main()
{
	FILE* f = fopen("russian_demography.csv", "r");
	if (!f) return -1;
	size_t curr_line = 0;

	char* buff = calloc(BUFF_SIZE, sizeof(char));
	if (!buff) return -1;
	// fgets(buff, BUFF_SIZE-1, f);
	// curr_line++;

	Vector v = v_init();
	size_t errors[1000] = {0};
	size_t errors_count = 0;
	size_t max_name_size = 0;
	while (!feof(f))
	{
		memset(buff, 0, BUFF_SIZE);
		fgets(buff, BUFF_SIZE-1, f);
		curr_line++;
		if ((!feof(f) && buff[strlen(buff)-1] != '\n') || char_count(buff, ',') != 6){
			errors[errors_count] = curr_line;
			errors_count++;
			continue;
		}


		Line* line = (Line*)calloc(1, sizeof(Line));
		if (!line) return -1;
		char* ptr = buff;

		{
			// get year
			line->year = atoi(ptr);
			while (*ptr != ',') ptr++;
			ptr++;
		}

		{
			// get name
			size_t name_len = 1;
			char *p = ptr + 1;
			for (; *(p - 1) != ','; p++) name_len++;
			*(p - 1) = 0;
			// printf("ptr = |%s|\n", ptr);
			// printf("name_len = %u\n", name_len);
			char *name = (char *) calloc(name_len + 1, sizeof(char));
			if (!name) return -1;
			strcpy(name, ptr);
			while (*ptr != ',') ptr++;
			ptr++;
			line->region = name;
		}

		{
			// get natural_grow
			char* a = ptr-1;
			char** end = &a;
			line->natural_grow = strtod(ptr, end);
			if (*end == ptr){
				errors[errors_count] = curr_line;
				errors_count++;
				printf("%lu:natural_grow error\n", curr_line);
				continue;
			}
			while (*ptr != ',') ptr++; ptr++;
		}

		{
			// get birth_rate
			char* a = ptr-1;
			char** end = &a;
			line->birth_rate = strtod(ptr, end);
			if (*end == ptr){
				errors[errors_count] = curr_line;
				errors_count++;
				printf("%lu:birth_rate error\n", curr_line);
				continue;
			}
			while (*ptr != ',') ptr++; ptr++;
		}

		{
			// get death_rate
			char* a = ptr-1;
			char** end = &a;
			line->death_rate = strtod(ptr, end);
			if (*end == ptr)
			{
				errors[errors_count] = curr_line;
				errors_count++;
				printf("%lu:death_rate error\n", curr_line);
				continue;
			}
			while (*ptr != ',') ptr++; ptr++;
		}

		{
			// get general_demographic_weight
			char* a = ptr-1;
			char** end = &a;
			line->general_demographic_weight = strtod(ptr, end);
			if (*end == ptr)
			{
				errors[errors_count] = curr_line;
				errors_count++;
				printf("%lu:general_demographic_weight error\n", curr_line);
				continue;
			}
			while (*ptr != ',') ptr++; ptr++;
		}

		{
			printf("|\n\n\n\n\n%s\n\n\n\n\n|\n", ptr);
			// get urbanization
			char* a = ptr-1;
			char** end = &a;
			line->urbanization = strtod(ptr, end);
			if (*end == ptr)
			{
				errors[errors_count] = curr_line;
				errors_count++;
				printf("%lu:urbanization error\n", curr_line);
				continue;
			}
			while (*ptr != ',') ptr++; ptr++;
		}

		// if (max_name_size < strlen(line->region))
		// {
			// printf("line->region = |%s|\n", line->region);
		// }
		max_name_size = max_name_size < (strlen(line->region))? strlen(line->region) : max_name_size;
		v_push_ptr(&v, line);
	}
	free(buff);

	// double year;
	// char*  region;
	// double natural_grow;
	// double birth_rate;
	// double death_rate;
	// double general_demographic_weight;
	// double urbanization;
	// ---- show
	// char f_str[100] = {0};
	// sprintf(f_str, "%%-010d|%%-010d|%%-0%lus|%%-015f|%%-015f|%%-015f|%%-015f|%%-015f|\n", max_name_size);
	// printf("Num       |Year      |Region                        |Natural grow   |Birth rate     |Death rate     |GDW            |Urbanization   |\n");
	// printf("--------------------------------------------------------------------------------------------------------------------------\n");
	// for(int i = 0; i < v.count; i++){
	//
	// 	printf(f_str,
	// 		i+1,
	// 		v_item(&v, i)->year,
	// 		v_item(&v, i)->region,
	// 		v_item(&v, i)->natural_grow,
	// 		v_item(&v, i)->birth_rate,
	// 		v_item(&v, i)->death_rate,
	// 		v_item(&v, i)->general_demographic_weight,
	// 		v_item(&v, i)->urbanization
	// 		);
	// }
	printf("max_name_size = %lu\n", max_name_size);
	printf("errors_count  = %lu\n", errors_count);
	// for (int i = 0; i < errors_count; i++)
	// {
	// 	printf("Error in line: %d\n", errors[i]);
	// }

	// ---- destroy
	v_destroy(&v);
	return 0;
}
