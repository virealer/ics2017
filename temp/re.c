//
// Created by 曹朋辉 on 2017/3/11.
//
#include <stdio.h>
int main(){
    printf("\\d+\n");
}


static int eval(p, q) {
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
            puts("wrong ...");
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
        int op = 5;
        int val1 = eval(p, op - 1);
        int val2 = eval(op + 1, q);

        switch(op) {
            case '+': return val1 + val2;
            case '-': return val1 - val2;
            case '*': return val1 * val2;
            case '/': return val1 / val2;
            default: assert(0);
        }
    }
}
