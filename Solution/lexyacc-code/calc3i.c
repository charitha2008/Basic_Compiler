#include <stdio.h>
#include "calc3.h"
#include "y.tab.h"

static int lbl;

int ex(nodeType *p) {
    int lbl1, lbl2;

    if (!p) return 0;
    switch(p->type) {
    case typeCon:

        printf("\tmovabsq\t\t$%d, %%rax\n", p->con.value); 	// either move absolute 64 bit value to register or load value from 64 bit address.
        printf("\tpushq\t\t%%rax\n");  
        break;

    case typeId:        
	
	printf("\tmovq\t%d(%%rbp),\t%%rax\n", (p->id.i + 1) * 8);    // local variables will be stored from 16(%rbp), thus added 1 to p->id.i
	printf("\tpushq\t%%rax\n"); 
        break;

    case typeOpr:
        switch(p->opr.oper) {
        case WHILE:
            printf("L%03d:\n", lbl1 = lbl++);
            ex(p->opr.op[0]);
            printf("\tjz\t\tL%03d\n", lbl2 = lbl++);
            ex(p->opr.op[1]);
            printf("\tjmp\t\tL%03d\n", lbl1);
            printf("L%03d:\n", lbl2);
            break;
        case IF:
            ex(p->opr.op[0]);
            if (p->opr.nops > 2) {
                /* if else */
		printf("\tjz\t\tL%03d\n", lbl1 = lbl++);
                ex(p->opr.op[1]);
                printf("\tjmp\t\tL%03d\n", lbl2 = lbl++);
                printf("L%03d:\n", lbl1);
                ex(p->opr.op[2]);
                printf("L%03d:\n", lbl2);
            } else {
                /* if */
                printf("\tjz\t\tL%03d\n", lbl1 = lbl++);
                ex(p->opr.op[1]);
                printf("L%03d:\n", lbl1);
            }
            break;

        case PRINT:     
            ex(p->opr.op[0]);

            printf("\n\tpopq\t%%rax\n");
            printf("\tpushq\t%%rdi\n");
            printf("\tpushq\t%%rsi\n");
            printf("\tmovq\t$format, %%rdi\n");
            printf("\tmovq\t%%rax, %%rsi\n");
            printf("\tmovq\t$1, %%rax\n");
            printf("\tcall\tprintf\n");
            printf("\tpopq\t%%rsi\n");
            printf("\tpopq\t%%rdi\n");

            break;

        case '=':       
            ex(p->opr.op[1]);

            printf("\tpopq\t%d(%%rbp)\n", (p->opr.op[0]->id.i+1) * 8);

            break;
        case UMINUS:    
            ex(p->opr.op[0]);
		printf("\tpopq\t%%rax\n");
	    	printf("\tnegq\t%%rax\n");
	    	printf("\tpushq\t%%rax\n");

            break;

	case FACT:
  	    ex(p->opr.op[0]);
		
	    // %rdi​,​ %rsi​, ​%rdx​, ​%rcx​, ​%r8​, and ​%r9​ are used to pass the first six integeror pointer parameters to called functions
	    printf("\tpopq\t%%rdi\n");				// Set parameter
	    printf("\tcall\tfact\n");				// call function
	    printf("\tpushq\t%%rax\n");				// push return value to stack
	    break;

	case LNTWO:
	    ex(p->opr.op[0]);
	    printf("\tpopq\t%%rdi\n");
	    printf("\tcall\tlntwo\n");
	    printf("\tpushq\t%%rax\n");
	    break;

        default:
            ex(p->opr.op[0]);
            ex(p->opr.op[1]);

	 	switch(p->opr.oper) 
	 	{
		    	case GCD:   
				
				//printf("\tgcd\n"); 
				printf("\tpopq\t%%rdi\n");	// set both rdi and rsi to pass parameters to gcd method
				printf("\tpopq\t%%rsi\n");
				printf("\tcall\tgcd\n");
	    			printf("\tpushq\t%%rax\n");
				break;

	            	case '+':   

				//printf("\tadd\n"); 
				printf("\tpopq\t%%rbx\n");
				printf("\tpopq\t%%rax\n");
				printf("\tadd\t%%rbx,\t%%rax\n");	// rax = rax + rbx
				printf("\tpushq\t%%rax\n");
				break;

	            	case '-': 

				//printf("\tsub\n"); 
				printf("\tpopq\t%%rbx\n");
				printf("\tpopq\t%%rax\n");
				printf("\tsub\t%%rbx,\t%%rax\n");	// rax = rax - rbx
				printf("\tpushq\t%%rax\n");
				break; 

	            	case '*':   

				//printf("\tmul\n"); 
				printf("\tpopq\t%%rbx\n");
				printf("\tpopq\t%%rax\n");
				printf("\timul\t%%rbx,\t%%rax\n");	// rax = rax * rbx
				printf("\tpushq\t%%rax\n");				
				break;

	            	case '/':

				//printf("\tdiv\n"); 
				printf("\tpopq\t%%rcx\n");
				printf("\tpopq\t%%rax\n");
				printf("\tcqto\n");
				printf("\tidivq\t%%rcx\n");
				printf("\tpushq\t%%rax\n");		// Quotient is stored in rax
				break;
	            	
			case '<':

				printf("\n\tpopq\t%%rbx\n");		
				printf("\tpopq\t%%rax\n");		
				printf("\tmovq\t$0, %%rcx\n");		// set rcx to 0
				printf("\tcmp\t%%rbx,\t%%rax\n");
				printf("\tsetl\t%%cl\n");		// set result
				printf("\txorb\t$0, %%cl\n");		// set all other bits to 0
			    	break;

	            	case '>':
			
				printf("\n\tpopq\t%%rbx\n");		
				printf("\tpopq\t%%rax\n");		
				printf("\tmovq\t$0, %%rcx\n");		// set rcx to 0
				printf("\tcmp\t%%rbx,\t%%rax\n");
				printf("\tsetg\t%%cl\n");		// set result
				printf("\txorb\t$0, %%cl\n");		// set all other bits to 0
			    	break;

	            	case GE:

				printf("\n\tpopq\t%%rbx\n");		
				printf("\tpopq\t%%rax\n");		
				printf("\tmovq\t$0, %%rcx\n");		// set rcx to 0
				printf("\tcmp\t%%rbx,\t%%rax\n");
				printf("\tsetge\t%%cl\n");		// set result
				printf("\txorb\t$0, %%cl\n");		// set all other bits to 0
			    	break;

        	    	case LE:

				printf("\n\tpopq\t%%rbx\n");		
				printf("\tpopq\t%%rax\n");		
				printf("\tmovq\t$0, %%rcx\n");		// set rcx to 0
				printf("\tcmp\t%%rbx,\t%%rax\n");
				printf("\tsetle\t%%cl\n");		// set result
				printf("\txorb\t$0, %%cl\n");		// set all other bits to 0
			    	break;
        	    	
			case NE:

				printf("\n\tpopq\t%%rbx\n");		
				printf("\tpopq\t%%rax\n");		
				printf("\tmovq\t$0, %%rcx\n");		// set rcx to 0
				printf("\tcmp\t%%rbx,\t%%rax\n");
				printf("\tsetne\t%%cl\n");		// set result
				printf("\txorb\t$0, %%cl\n");		// set all other bits to 0
			    	break;

            		case EQ:

				printf("\n\tpopq\t%%rbx\n");		
				printf("\tpopq\t%%rax\n");		
				printf("\tmovq\t$0, %%rcx\n");		// set rcx to 0
				printf("\tcmp\t%%rbx,\t%%rax\n");
				printf("\tsete\t%%cl\n");		// set result
				printf("\txorb\t$0, %%cl\n");		// set all other bits to 0
			    	break;
	 	}
	}
    }
    return 0;
}
