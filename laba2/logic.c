#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_CAP   10
#define VEC_SCALE 2
#define BUFF_SIZE 2048
#define MEM_ERROR errors[errors_count] = curr_line; errors_count++
// #define MEM_ERROR NULL;

typedef struct Line {
	size_t year;
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

// typedef struct CSV_FILE {
// 	Vector lines;
// 	size_t lines_count;
// 	size_t errors;
// } CSV_FILE;

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
	if(!v || !item_ptr) return;
	if(v->cap == v->count){
		v->cap *= VEC_SCALE;	
		Line** new_data = (Line**)calloc(v->cap*VEC_SCALE, sizeof(Line*));
		memcpy(new_data, v->data, sizeof(Line*)*(v->count));
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

// parse funcs
int fill_size_t(size_t* field, char* ptr)
{
	if (!field || !ptr) return 0;

	char* endptr;
	*field = strtoul(ptr, &endptr, 10);
	if (endptr == ptr)
		return 0;
	return 1;
}

int fill_double(double* field, char* ptr)
{
	if (!field || !ptr) return 0;

	char* endptr;
	*field = strtod(ptr, &endptr);
	if (endptr == ptr)
		return 0;
	return 1;
}

char* fill_str(char* ptr)
{
	if (!ptr) return NULL;
	size_t str_len = strchr(ptr, ',') - ptr;
	if (str_len == 0)
		return NULL;

	char* str = (char*)calloc(str_len+1, sizeof(char));
	if (!str) return NULL;

	strncpy(str, ptr, str_len);
	return str;
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
	size_t* errors = calloc(3000, sizeof(size_t));
	size_t errors_count = 0;
	size_t max_name_size = 0;
	size_t total_count = 0;
	while (!feof(f))
	{
		memset(buff, 0, BUFF_SIZE);
		if (!fgets(buff, BUFF_SIZE-1, f))
			continue;
		curr_line++;

		if ((!feof(f) && buff[strlen(buff)-1] != '\n') || char_count(buff, ',') != 6){
			MEM_ERROR;
			continue;
		}
		if (buff[0] == ','){
			MEM_ERROR;
			continue;
		}

		if (strstr(buff, ",,") || strstr(buff, ",\n")){
			MEM_ERROR;
			continue;
		}

		*strchr(buff, '\n') = 0;

		Line* line = (Line*)calloc(1, sizeof(Line));
		if (!line) return -1;
		char* ptr = buff;

		// fill year
		{
			if (!fill_size_t(&line->year, ptr))
			{
				MEM_ERROR;
				free(line);
				continue;
			}
			// move
			ptr = strchr(ptr, ',') + 1;
		}

		// fill region
		{
			line->region = fill_str(ptr);
			if (line->region == NULL) {
				MEM_ERROR;
				free(line);
				continue;
			}
			max_name_size = max_name_size < strlen(line->region)? strlen(line->region) : max_name_size;
			// move
			ptr = strchr(ptr, ',') + 1;
		}
		//1990,Krasnoyarsk Krai,4.5,13.8,9.3,69.33,73.94
		//	                    |

		// fill other fields
		double* fields[] = {
			&line->natural_grow,
			&line->birth_rate,
			&line->death_rate,
			&line->general_demographic_weight,
			&line->urbanization
		};

		for (int i = 0; i < sizeof(fields)/sizeof(double*); i++)
		{
			if (!fill_double(fields[i], ptr))
			{
				MEM_ERROR;
				free(line->region);
				free(line);
				line = NULL;
				break;
			}
			// move
			ptr = strchr(ptr, ',') + 1;
		}

		// printf("line = %p\n", line);
		if (line)
			v_push_ptr(&v, line);
	}
	free(buff);

	// ---- show
	char f_str[100] = {0};
	sprintf(f_str, "%%-010d|%%-010d|%%-0%lus|%%-015f|%%-015f|%%-015f|%%-015f|%%-015f|\n", max_name_size);
	printf("Num       |Year      |Region                        |Natural grow   |Birth rate     |Death rate     |GDW            |Urbanization   |\n");
	printf("--------------------------------------------------------------------------------------------------------------------------\n");
	for(int i = 0; i < v.count; i++){
		printf(f_str,
			i+1,
			v_item(&v, i)->year,
			v_item(&v, i)->region,
			v_item(&v, i)->natural_grow,
			v_item(&v, i)->birth_rate,
			v_item(&v, i)->death_rate,
			v_item(&v, i)->general_demographic_weight,
			v_item(&v, i)->urbanization
			);
	}
	printf("--------------------------------------------------------------------------------------------------------------------------\n");
	printf("INFO:\n");

	printf("total_count   |%6lu|\n", curr_line);
	printf("max_name_size |%6lu|\n", max_name_size);
	printf("errors_count  |%6lu|\n", errors_count);

	// for (int i = 0; i < errors_count; i++)
	// {
	// 	printf("Error in line: %lu\n", errors[i]);
	// }

	// ---- destroy
	v_destroy(&v);

	free(errors);
	return 0;
}
