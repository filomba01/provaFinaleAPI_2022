#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define ALPH 64  
#define CMML 17
#define RED 0
#define BLACK 1 
#define ZERO 48
#define NOVE 59



#define CATCH '+'
#define NTHRE '|'
#define NTFND '/'

typedef struct rbt_node_s{

    int8_t color;
    struct rbt_node_s * left;
    struct rbt_node_s * right;
    struct rbt_node_s * p;

    char word[];

} rbt_node;

typedef struct list_s {
    struct list_s * next;

    char word[];
} list_t;


typedef struct ist_imp_s{
    short int  val;
    short int max;
} ist_imp;




//RBTREE
rbt_node * insertRB( rbt_node *, char *, int8_t);
rbt_node * rbInsertFixup(rbt_node  *, rbt_node *); 
rbt_node * leftRotate (rbt_node * , rbt_node *);
rbt_node * rightRotate (rbt_node * , rbt_node *);
rbt_node * searchRBT(rbt_node *, char *, int k);
void inOrderVisitRB(rbt_node * );

void freeRBtree( rbt_node *);

//GAME

list_t * game( list_t * , rbt_node *,char * , char *, char * , uint64_t * ,int , int ,  int * , ist_imp *, int8_t *);
int checkEqual( char *, char *, int8_t ,char * , char * );
char * searchChars(char *, char *, int8_t , char * );
int8_t getRightIndex(char );
list_t * firstCompare( rbt_node * , list_t * , char * ,  char * , int , ist_imp *,  int *);
list_t * compareWords( list_t * , char * ,  char * ,  int , ist_imp *, int *);
int8_t checkVincoli(char * , char * , char *  , int, ist_imp * );


//scanf rapida
int string_scanf( char * , int );
void int_scanf( int * n );

int mystrncmp ( const char * , const char *  ,  int ,int );

//liste
//LISTA
list_t * push(list_t * , char * , int , ist_imp * , char * , char *, uint64_t * );
list_t * delete (list_t * , list_t *, list_t * );
void freeList( list_t *  );
void printList( list_t * );

list_t * mergeSortList( list_t * head );
list_t * splitList( list_t * head );
list_t * merge( list_t * head,list_t * s1, list_t * s2);

int main ( int argc, char * argv[]){
    
    //VARIABILI
    rbt_node * root = NULL;
    list_t * head = NULL;
    ist_imp info[ALPH];
    int tnt;
    int res;
    int k, i;
    int max_p_confronto;
    int8_t g = 0;
    int partite = 0;
    int_scanf(&k);

    char word[k+1];
    char answer[k+1];
    char result[k+1];
    uint64_t wrongP[k];


    if(string_scanf(word,k ) != 1) return 0;
    
    while( !(word[0] == '+' && word[1] == 'n') ){
        if(word[0] != '+'){
            root = insertRB( root, word, k);
        }
        if(string_scanf(word, k) != 1) return 0;
    }
    
    g = 0; //flag partita attiva
    tnt = 0;  //tentativi partita   
    do{
        if( g && word[0] != '+'){
         
            head = game(head,root,word,answer, result, wrongP,k,max_p_confronto,  &tnt, info, &g);   
            if(!g){
                freeList( head );
                head = NULL;
                tnt = 0;
            }   

        }
        
        if( word[0] == '+' && word[1] == 'n'){ //nuova partita
                partite++;
                tnt  = 0;
                g=0;

                //resetto informazioni

                if( head != NULL){
                    freeList( head);
                    head = NULL;
                    tnt = 0;
                }

                for( i = 0; i < ALPH; i++){
                    info[i].val = 0;
                    info[i].max = 0;
                }

                for(i = 0; i < k; i++){
                    result[i] = NTFND;
                    wrongP[i] = 0;
                }
                result[i] = '\0';

                if( !string_scanf(answer, k)) return 0;

                while(answer[0] == '+'){

                    if( answer[0] == '+' && answer[1] == 'i'){
                    
                        if( !string_scanf(answer, k)) return 0;

                        while(answer[0] != '+'){
                            root = insertRB( root, answer, k);
                            if( !string_scanf(answer, k) ) return 0;
                        }

                        if( !string_scanf(answer, k)) return 0;

                    }
                
                }

                int_scanf(&max_p_confronto);
                if( !string_scanf(word, k) ) return 0;

                head = game(head,root,word,answer, result, wrongP ,k,max_p_confronto,  &tnt, info, &g); 
                
                if(!g){
                    freeList( head );    
                    head = NULL;
                    tnt = 0;
                }   
                   
        }


        if( word[0] == '+' && word[1] == 'i'){
        
            if( !string_scanf(word, k)) return 0;

            while(word[0] != '+'){
                root = insertRB( root, word, k);
                if(g && tnt > 0){
                    head = push( head, word, k , info, answer, result, wrongP);
                }
                
                if( !string_scanf(word, k) ) return 0;
            }


            
        }
   
        if( word[0] == '+' && word[1] == 's' && g){
            if( tnt > 0) {
                head = mergeSortList( head);
                printList( head );
                //stampa_lista
            } else {
                inOrderVisitRB(root);
            }

        } 

        if( !(word[0]== '+' && word[1] == 'n') ){
            res = string_scanf(word, k);
        }
        
    } while(res != EOF);

    freeList(head);
    freeRBtree( root);
    return 0;
}



rbt_node * insertRB( rbt_node * root, char * word, int8_t  k){

    int tmp;
    rbt_node * new = NULL;
    rbt_node * curr = NULL;
    rbt_node * prev = NULL;

    
    if((new = malloc(sizeof(rbt_node) + (k+1) ))){

        curr = root;
        prev=NULL;
        
        while(curr != NULL){
            
            prev=curr;
           if (  (tmp = mystrncmp( word,curr->word, 0,k) ) > 0){
                curr = curr->right;
            } else {
                curr = curr->left;
            }   
        }
        
        strcpy(new->word, word);
        //new->valid = 1;
        new->p = prev;
        new->color = RED;
        new->right = NULL;
        new->left = NULL;

        if(prev == NULL){
            root = new;
        }   
        else if( tmp > 0){
            prev->right = new;
        }
        else{
            prev->left = new;
        }
                
        root = rbInsertFixup(root, new);  
  
    }
    else{
        printf("Error in memory allocation \n");
    }
    return root;
}



rbt_node * rbInsertFixup(rbt_node * root, rbt_node * elem){

    rbt_node * uncle = NULL;


    while(  elem->p != NULL && elem->p->color == RED){
        if( elem->p->p != NULL && elem->p == elem->p->p->left ){
            uncle = elem->p->p->right;
            if(uncle != NULL && uncle->color == RED){
                elem->p->color = BLACK;
                uncle->color = BLACK;
                if( elem->p->p != NULL){
                    elem->p->p->color  = RED;  
                }
                elem = elem->p->p;
                
            } else{

                if( elem->p != NULL && elem == elem->p->right){
                    elem = elem->p;
                    root = leftRotate(root, elem);
                }
                if(elem->p != NULL){
                    elem->p->color = BLACK;
                    if(elem->p->p != NULL){
                        elem->p->p->color = RED;
                        root = rightRotate(root, elem->p->p);
                    }
                }

            } 
            
        } else {
            uncle = elem->p->p->left;
            if(uncle != NULL && uncle->color == RED){
                elem->p->color = BLACK;
                uncle->color = BLACK;
                if( elem->p->p != NULL){
                    elem->p->p->color  = RED;  
                }
                elem = elem->p->p;
                
            } else{

                if ( elem->p != NULL && elem == elem->p->left){
                    elem = elem->p;
                    root = rightRotate(root, elem);
                }
                if(elem->p != NULL){
                    elem->p->color = BLACK;
                    if(elem->p->p != NULL){
                        elem->p->p->color = RED;
                        root = leftRotate(root, elem->p->p);
                    }
                }
            }
        }
    }

    root->color = BLACK;
    return root;
} 

rbt_node * leftRotate (rbt_node * root, rbt_node * elem){
    rbt_node * tmp = NULL;
    tmp = elem->right;
    if(tmp != NULL){
        elem->right = tmp->left;
        if(tmp->left != NULL){
            tmp->left->p = elem;
        }
        tmp->p = elem->p;
        if(elem->p == NULL){
            root = tmp;
        } else if(elem->p->left == elem){
            elem->p->left=tmp;
        }
        else {
            elem->p->right=tmp; 
        }
        tmp->left = elem;
        elem->p = tmp;
    }
    return root;
}

rbt_node * rightRotate (rbt_node * root, rbt_node * elem){
    rbt_node * tmp = NULL;
    tmp = elem->left;
    if(tmp != NULL){
        elem->left = tmp->right;
        if(tmp->right != NULL){
            tmp->right->p = elem;
        }
        tmp->p = elem->p;
        if(elem->p == NULL){
            root = tmp;
        } else if(elem->p->right == elem){
            elem->p->right=tmp;
        }
        else {
            elem->p->left=tmp; 
        }
        tmp->right = elem;
        elem->p = tmp;
    }
    return root;
}

rbt_node * searchRBT(rbt_node * root, char * word,  int k){
    int tmp;
    if(root != NULL){
        
        if(( tmp = mystrncmp( word, root->word, 0, k) ) == 0){
            return root;
        } 
        if( tmp > 0){   
            return searchRBT(root->right, word, k);
        } 

        return searchRBT(root->left, word, k);        
    }
    return NULL;
}


void inOrderVisitRB(rbt_node * root){
    if(root != NULL){
        inOrderVisitRB(root->left);
        
        printf("%s\n", root->word);  
        
        inOrderVisitRB(root->right);        
    }
    return;
}
void freeRBtree( rbt_node * root){
    if(root == NULL) return;
    freeRBtree(root->left);
    freeRBtree(root->right);
    free(root);
    return;
}


list_t * game( list_t * head, rbt_node * dizionario, char * word, char * answer, char * defresult, uint64_t * wrongP, int k, int max_p_confronto,  int * i, ist_imp * info, int8_t * g){
    int8_t found;
    uint64_t tmpbit;
    short int    j;
    int pvalide;
    char * result = NULL;
    ist_imp wIst[ALPH];
    uint8_t val;
    
    result = malloc(sizeof(char) * (k + 1));

    found = 0;
 
    while( *i < max_p_confronto && word[0] != '+' ){
          
           
            if(  searchRBT( dizionario, word, k) != NULL ){
               
                if (checkEqual(word, answer, k, result, defresult) ){
                    found = 1;
                    break;
                }
                
                result = searchChars(word, answer, k , result);
            
                //AGGIORNO INFO CON LE NUOVE INFORMAjIONI RACCOLTE
                for(j = 0; j < k; j++){
                    val = getRightIndex(word[j]);
                    wIst[val].val = 0;
                    wIst[val].max = 0;

                }

                for( j = 0; j < k; j++){
                    val = getRightIndex(word[j]);
                    if( (result[j] == NTHRE || result[j] == CATCH)){
                            wIst[val].val++;
                            
                    }
                    if(result[j] != CATCH){
                                
                        tmpbit = 1;
                        tmpbit = tmpbit<<val;
                        wrongP[j] = (tmpbit | wrongP[j] );
                    }   

                }    
                
                for(j = 0; j < k; j++){
                        
                        val = getRightIndex(word[j]);
                        if (result[j] == NTFND && wIst[val].val > 0){
                            wIst[val].max = 1;
                        } else if (result[j] == NTFND){
                            info[val].val = -1;
                            info[val].max = 1;

                            wIst[val].max = 1;
                        }
                    
                }

                for(j = 0;j < k; j++){
                    //aggiorno l'istogramma
                    val = getRightIndex(word[j]);

                    if( wIst[val].val > info[val].val && !info[val].max){
                        info[val].val = wIst[val].val;
                        info[val].max = wIst[val].max;
                    }        
                
                }
                if( *i == 0){
                    pvalide = 0;
                    head = firstCompare( dizionario, head, word,  result, k,  wIst, &pvalide);
                    
                } else {
                
                    pvalide = 0;
                    head = compareWords( head,  word,  result, k, wIst, &pvalide);
                    

                }
                
                
                printf("%s\n%d\n", result, pvalide);
                /* SE LE PAROLE SONO UGUALI STAMPO OK E TERMINA LA PARTITA*/
                *i = *i + 1;
                if(*i == max_p_confronto){
                    printf("ko\n");
                    free(result);
                    *g = 0;
                    return head;
                } 
            } else {
                    printf("not_exists\n");    
            }
            
            if( !string_scanf(word, k) ) return head;

        }
    /* returning value is 1 the game is active, when return value is 0 game finishes*/
    if(found){
        /*indovinato*/
        printf("ok\n");
        free(result);
        *g = 0;
        return head;
    }
    /*  game is not finished yet */
    free(result);
    *g = 1;
    return head;
   
}

int checkEqual( char * word, char *  answer, int8_t  k,char * result, char * defresult){
    int i;
    int8_t equals;
    i=0;
    equals = 1;
    while(i < k){
        if(word[i] == answer[i]){
            result[i] = CATCH;
            defresult[i] = CATCH;
        } else {
            result[i] = NTFND;
            equals = 0;
        }
        i++;
    } 
    result[i] = '\0';
    return equals;
}


char * searchChars(char * word, char * answer, int8_t k , char * result){
    int i;
    short int ist[ALPH];
    int8_t val;
    for(i=0; i<k;i++){
        ist[ getRightIndex(answer[i])] = 0;
        ist[ getRightIndex(word[i])] = 0;
    }
    for(i=0;i<k;i++){
        if( result[i] != CATCH){
             ist[getRightIndex(answer[i])]++;
        }
       
    }
    for(i = 0; i<k; i++){
        val = getRightIndex(word[i]);
        if( ist[val] > 0 && result[i] != CATCH){
                result[i] = NTHRE;
                ist[val]--;
        }
    }
    return result;
}  

int8_t getRightIndex(char c){
    if( c >= 'a' && c <= 'z' )
        return (c - 'a' + 38);
    if( c >= 'A' && c <= 'Z' )
        return (c - 'A' + 11);  
    if( c == '-')
        return 0;
    if( c == '_')
        return 37;
    if( c >= '0' && c <= '9')
        return ((c + 1) - '0');
    return -1;
}


list_t * firstCompare( rbt_node * root, list_t * head, char * word,  char * result,  int k, ist_imp * wIst,  int * sum){

    if(root == NULL){

        return head;
    }

    if(checkVincoli(root->word, result, word, k, wIst)){
        head = push( head, root->word, k, NULL, NULL, NULL, NULL);
        *sum = *sum + 1;

    } 
    head = firstCompare( root->left, head,  word,  result,  k, wIst, sum); 
    head = firstCompare( root->right, head,  word,  result,k,  wIst, sum);


    return head;
    
}

list_t * compareWords( list_t * head, char * word,  char * result, int k, ist_imp * wIst, int * sum){
    list_t * papa, * x, *tmp;

    x = head;
    papa = NULL;
    while(x != NULL){

        if(checkVincoli(x->word, result,  word, k, wIst)){
            *sum = *sum + 1;
            papa = x;
            x = x->next;
        } else {
            if( papa != NULL){
                tmp = x->next;
                head = delete(head, papa,x);
                x = tmp;
                
            } else {
                head = delete(head, papa,x);  
                x = head;
                papa = NULL;
                
            }
            
            
        }
         
    }

    
    return head;
    
}


int8_t checkVincoli(char * elem, char * result,  char * word , int k, ist_imp * wIst){

    int8_t i;
    short int bIst[ALPH];
    int8_t val;

    for(i = 0; i < k; i++){
        
        if( ( word[i] !=  elem[i] && result[i] == CATCH ) ||  (word[i] ==  elem[i] && result[i] != CATCH ) ){
                
            return 0;
        }
        bIst[getRightIndex(elem[i])] = 0;
        bIst[getRightIndex(word[i])] = 0;
    }

    for( i = 0; i < k; i++){
        bIst[getRightIndex(elem[i])]++;
    }

    for(i = 0; i < k; i++){
        
        val = getRightIndex(word[i]);
        // sotto il minimo
        if(bIst[val] < wIst[val].val){

            return 0;
        } 
        // sopra il massimo
        if ( bIst[val] > wIst[val].val && wIst[val].max){

            return 0;
        }
 
    }
    return 1;
}


int string_scanf( char * word, int buff){
    int i;
    char c;
    i = 0;
    
    while((word[i]= getchar_unlocked()) != '\n' && i < buff ){
        
        if (word[i] == EOF){
           return word[i]; 
        }
        i++;
    }
    c = word[i];
    while(c != '\n'){
        c = getchar_unlocked();
    }
    word[i] ='\0';
    return 1;
}

void int_scanf( int * n ){
    char c;
    c  = getchar_unlocked();
    *n = 0;
    while (c >= ZERO && c <= NOVE) {
        *n = *n * 10 + c - ZERO;
        c  = getchar_unlocked();
    }

    return;
}


int mystrncmp ( const char * s1, const char * s2 ,  int start,int k){
    int i;
    int tmp;
    for( i = start; i < k ; i++){
        if( (tmp = s1[i] - s2[i] ) != 0 )
            return tmp;
    }

    return tmp;
}


//LISTE

list_t * push (list_t * h, char * word, int k, ist_imp * info, char * answer, char * result , uint64_t * wrongP){
    list_t * temp;
    int valid = 1;
    int i;
    int tmp;
    short int wIst[ALPH];

    uint64_t tmpbit;

    
    if(info != NULL && answer != NULL){
            //confronta tutti i vincoli
            for( i = 0; i < ALPH; i++){
                wIst[i] = 0;
            }
            i = 0;
            while( i < k && valid ){
                tmp = getRightIndex(word[i]);
                wIst[tmp]++;
                if(answer[i] != word[i] && result[i] == CATCH){
                    
                    valid = 0;
                } 
                
                if (result[i] != CATCH){
                    
                    tmpbit = 1;
                    tmpbit = (tmpbit & ( wrongP[i] >> tmp) );

                    if(tmpbit){
                        
                        valid = 0;
                    }
                }
                i++;
            }

            i = 0;
            while(i < ALPH && valid){
                
                
                
                if( (wIst[i] > info[i].val && info[i].max && info[i].val >= 0) || (wIst[i] < info[i].val ) || (wIst[i] > 0 && info[i].val == -1) ){
                    valid = 0;
                }
                i++;
            }

    }

	if( valid  && (temp = malloc(sizeof(list_t) + (k+1)) )){ /* 0 */
		strcpy(temp->word, word); 			 /* 1 */
		temp->next = h; 				/* A: la testa sa chi è il primo elemento della lista*/
		h = temp; 					/* B */
	}

	return h;
}

list_t * delete (list_t * h, list_t * papa, list_t * x){
	list_t  * del;
    if(h == NULL) return h;

    if( papa == NULL){ /* devo controllare sempre che h non punti a NULL*/
		del = h;
		h = h->next; /* la testa guarda al successivo elemento */
		free(del); /* libero la memoria a cui sta guardando del*/
	} else {
        papa->next = x->next;   
        free(x); 
    }


	return h;
}

void freeList( list_t * head ){

    if ( head == NULL){
        return;
    } 
    freeList( head->next );
    free(head);
    return;
}


void printList( list_t * head){
    if( head == NULL){
        return;
    }
    printf("%s\n", head->word);
    printList(head->next);
    return;
}

list_t * mergeSortList( list_t * head ){
    
    list_t * s1, * s2, *tmp;
    
    if( head == NULL || head->next == NULL){
        return head;
    }

    if(head->next->next != NULL){
        s2 = splitList(  head );
        s1 = head;
        s1 = mergeSortList(s1);
        s2 = mergeSortList(s2);

        head = merge(head,s1,s2);
    } else {
        if( strcmp(head->word, head->next->word) > 0){
            tmp = head;
            head = head->next;
            tmp->next = head->next;
            head->next = tmp;
        }
    }
    
    return head;

}

list_t * splitList( list_t * head ){

    list_t * elem, *  prec;
    list_t * veloce;

    elem = head;
    prec = head;
    veloce = head;

    while(veloce != NULL){
        
        if(veloce->next != NULL && veloce->next->next != NULL){
            prec = elem;
            elem = elem->next;
            veloce = veloce->next->next;
        } else {
            break;
        }
    }

    veloce = prec->next;
    prec->next = NULL;

    return veloce;
}

list_t * merge( list_t * head, list_t * h1, list_t * h2){
    
    list_t * elem1, * elem2, * el;

    elem1 = h1;
    elem2 = h2;

    if( elem1 != NULL && elem2 != NULL && strcmp(elem1->word, elem2->word) < 0){
        head = elem1;  
        elem1 = elem1->next; 
    }  else {
        head = elem2;
        elem2 = elem2->next;
    }
    
    el = head;

    while(elem1 != NULL && elem2 != NULL){
        
        if( strcmp(elem1->word, elem2->word) < 0){
            el->next = elem1;
            el = el->next;
            elem1 = elem1->next; 
        } else {
            el->next = elem2;
            el = el->next;
            elem2 = elem2->next; 
        }
    }

    while( elem1 != NULL){
            el->next = elem1;
            el = el->next;
            
            elem1 = elem1->next; 
    }

    while( elem2 != NULL){
            el->next = elem2;
            el = el->next;
            elem2 = elem2->next; 
    }

    return head;
}
