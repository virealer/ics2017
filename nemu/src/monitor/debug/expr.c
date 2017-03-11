#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include <stdlib.h>

enum {
	NOTYPE = 256, EQ, NUM, LB, RB

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"\\+", '+'},					// plus
	{"==", EQ},						// equal
	{"-", '-'},						// minus
	{"\\*", '*'},					// multiply
	{"/", '/'},						// devide
	{"[0-9]+", NUM},   				// int
	{"\\(", LB}, 					// left bracket
	{"\\)", RB},						// right bracket
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */

void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array `tokens'. For certain types
				 * of tokens, some extra actions should be performed.
				 */

				switch(rules[i].token_type) {
					case NOTYPE:
						break;
					default:
						tokens[nr_token].type = rules[i].token_type;
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						nr_token++;
						break;
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true;
}

static bool check_parentheses(p, q){
	if((tokens[p].type != LB) || (tokens[q].type != RB))
		return false;
	int i = 0;
	while (p<q) {
		if (tokens[p].type == LB) {
			i++;
			p++;
		}
		else if (tokens[p].type == RB) {
			i--;
			p++;
			if(i<1) return false;
		}
		else
			p++;
	}
	return true;
}

static int get_priority(int type){
	switch(type){
		case '+':
		case '-':
			return 0;
		case '*':
		case '/':
			return 1;
		case 9999:
			return 9999;
		default:
			return 1000;
	}
	return 1000;
}

static int get_dominant_op(int p, int q){
	int pos=p;
	bool in_parentheses = false;
	int stack=0;
	Token tem = {9999, "init"};
	bool loop = true;
	while(p<=q){
//		if(tokens[p].type == LB)
		
		switch(tokens[p].type){
			case NUM:
				p++;
				loop = false;
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				if(in_parentheses)
					p++;
					break;
				if (get_priority(tokens[p].type)>get_priority(tem.type)) {
					p++;
					break;
				}
				tem = tokens[p];
				pos = p;
				p++;
				break;
			case LB:
				stack+=1;
				in_parentheses=true;
				p++;
				break;
			case RB:
				stack-=1;
				if(stack==0)
					in_parentheses=false;
				p++;
				break;
			default:
				p++;
				break;
		}
		if(!loop)
			break;
	}
	return pos;
}

static int eval(int p, int q) {
	if(p > q) {
		/* Bad expression */
		return 0;
	}
	else if(p == q) { 
		/* Single token.
		 * For now this token should be a number. 
		 * Return the value of the number.
		 */ 
		if(tokens[p].type != NUM){
			assert(0);
			return 0;
		}
		int n = atoi(tokens[p].str);
		return n;
	}
	else if(check_parentheses(p, q) == true) {
		/* The expression is surrounded by a matched pair of parentheses. 
		 * If that is the case, just throw away the parentheses.
		 */
		return eval(p + 1, q - 1); 
	}
	else {
		/* We should do more things here. */
//		op = the position of dominant operator in the token expression;
		int op = get_dominant_op(p, q);
		int val1 = eval(p, op - 1);
		int val2 = eval(op + 1, q);

		switch(tokens[op].type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			default: assert(0);
		}
	}
}


uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
	puts("hello");
	printf("%d\n", get_dominant_op(0, nr_token-1));
	/* TODO: Insert codes to evaluate the expression. */
	printf("%d\n", eval(0, nr_token-1));
//	TODO();
//	printf("%s\n", check_parentheses(0, nr_token-1)?"True":"False");
	return 0;
}
