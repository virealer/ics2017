#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32
#define MAX_LEN 128

static WP wp_pool[NR_WP];
static WP *head, *free_;

uint32_t expr(char *e, bool *success);

void init_wp_pool() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_pool[i].NO = i;
		wp_pool[i].next = &wp_pool[i + 1];
	}
	wp_pool[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_pool;
}

WP* new_wp(){
	WP* tmp=NULL;
	if(free_){
		tmp = free_;
		free_ = free_->next;
				
	}
	else{
		assert(0);
	}
	return tmp;
}

int add_wp(char* args){
	WP* next = new_wp();
	if(!head){
			head = next;
			strncpy(next->expr, args, MAX_LEN);
			bool* success = false;
			next->pre_val = expr(args, success);
	}
	else if(!next){
//		next.expr
		strncpy(next->expr, args, MAX_LEN);
		bool* success = false;
		next->pre_val = expr(args, success);
		next->next = head->next;
		head->next = next;
	}
	printf("%s\n", next->expr);
	return 0;
}

bool check_wp(){
	WP* tem = head;
	int new_val = 0;
	bool* success = false;
//	do {
//		new_val = expr(tem->expr, success);
//		if(new_val != tem->pre_val){
//			printf("pre:%d\nnew:%d\n", tem->pre_val, new_val);
//			tem->pre_val = new_val;
//			return false;
//		} 
//		tem = tem->next;
//	}while(tem);
	new_val = expr(tem->expr, success);
	if(new_val != tem->pre_val){
		printf("pre:%d\nnew:%d\n", tem->pre_val, new_val);
		tem->pre_val = new_val;
		return false;
	} 
	tem = tem->next;
	printf("new:%d\n", new_val);
	return true;
}


/* TODO: Implement the functionality of watchpoint */


