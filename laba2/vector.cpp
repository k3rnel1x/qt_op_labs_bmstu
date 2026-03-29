#include "vector.h"

Line* get_line()
{
    Line* line = (Line*)calloc(1, sizeof(Line));
    if(!line) return NULL;
    line->by_idx[0] = NULL; // count of all lines
    line->by_idx[1] = &line->year;
    line->by_idx[2] = &line->natural_grow;
    line->by_idx[3] = &line->birth_rate;
    line->by_idx[4] = &line->death_rate;
    line->by_idx[5] = &line->general_demographic_weight;
    line->by_idx[6] = &line->urbanization;
    return line;
}

Vector v_init()
{
    Line** dat = (Line**)calloc(VEC_CAP, sizeof(Line*));
    return (Vector){
        .inited = 1,
        .count  = 0,
        .cap    = (size_t)VEC_CAP,
        .data   = dat,
    };
}

void v_push_ptr(Vector* v, Line* item_ptr)
{

    if(!v || v->inited == 0 || !item_ptr) return;

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
    if(!v || v->inited == 0 || idx >= v->count) return NULL;

    return v->data[idx];
}

void v_destroy(Vector* v)
{
    if(v->inited == 0) return;

    for(int i = 0; i < v->count; i++){
        if (v->data[i]){
            free(v->data[i]->region);
            free(v->data[i]);
            v->data[i] = NULL;
        }
    }
    free(v->data);
    v->inited = 0;
}

// Iterator begin(Vector* v)
// {
// 	if(!v) return (Iterator){0};
// 	
// 	return (Iterator){ 
// 		.curr = v->data[0];
// 		.data = v->data,
// 		.size = v->count,
// 	};
// }
// 
// Iterator end(Vector* v)
// {
// 	if(!v) return (Iterator){ .curr = NULL; };
// 	Iterator iter = begin(v);	
// 	while()
// }
// Line*    get(Iterator* it);
// void next(Iterator* iter)
// {
// 	if(!iter) return;
// 	
// }
// void set(Iterator& it, Book* book);
// int  is_end(Iterator* it, Line* line)
// {
// 	if(!it || !line)
// 		return 1;
// 
// 	return it->curr;
// }


