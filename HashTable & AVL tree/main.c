#include <stdio.h>
#include <stdlib.h>
#define size 50
#include <string.h>
#include <math.h>
typedef struct AVLnode* AVLNode;
struct AVLnode
{
    char County_name[size];
    char county_related_data_file_name [size];
    AVLNode left;
    AVLNode right;
    int height; //Balance information
};

struct HashNode
{
    int ID;
    char name[size];
    int Age;
    char gender;
    int status ;// if node is empty or not
};
typedef struct HashNode A;
typedef struct HashNode *hn;
int  sizeOfTable =0;  //global size to use it in all function
hn hashTable[size];   //global hash table to use it in all function
int read_hash(hn hashTable [sizeOfTable],char filename[size])
{
    char array [50];
    FILE *in;
    in = fopen(filename,"r");
    if(in==NULL)
    {
        printf("file not here try again\n");
        return 0;
    }
    while(fgets(array,50,in) != NULL)
    {
        int i = 0 ;
        char* token[50] ;
        token[0] = strtok(array, "/");
        while (token [i] != 0)
        {
            i++ ;
            token [i] = strtok(NULL,"/");
        }
        creatHash(atoi(token[0]),token[1],atoi(token[2]),token[3],hashTable);
    }
    fclose(in);
    return 1;
}
int computeSize(char filename[size]) // function to find the size of data in file
{
    int i=0;
    FILE *fin;
    fin=fopen(filename,"r");
    char s [1000];
    while(fgets(s, sizeof(s), fin) != NULL)
    {
        i++;
    }
    printf("THE NUMBER OF LINE IN FILE = %d\n",i);
    i=i*2;
    i = nextPrime(i);
    return i;
}
int nextPrime(int num)  // function to find the size of hash table
{
    num++;
    for (int i = 2; i <num; i++)
    {
        if(num%i == 0)
        {
            num++;
            i=2;
        }
        else
        {
            continue;
        }
    }
    return num;
}
void intilize(hn hashTable [sizeOfTable]) // function to initialize the hash table.
{
    for(int i =0; i<sizeOfTable; i++)
    {
        hashTable[i]=(hn )malloc(sizeof(A));
        hashTable[i]->status=0; // all node empty in initialize
    }
}
void creatHash(int ID,char name[size],int age,char gender,hn hashTable[sizeOfTable])  //create new node in hash table
{
    int i=0;
    int index = hashCode(ID,i);
    while(hashTable[index]->status==1)
    {
        i++;
        index = hashCode(ID,i);
    }
    strcpy(hashTable[index]->name,name);
    hashTable[index]->status=1;
    hashTable[index]->ID=ID;
    hashTable[index]->gender=gender;
    hashTable[index]->Age=age;
}
int hashCode(int id,int i)
{
    int index;
    int y=id + pow(i, 2);
    index = (int) y%sizeOfTable;
    return index;
}
AVLNode find_County_name(char County_name [size],AVLNode tree)
{
    AVLNode current = tree;
    while( current && (strcmp(County_name,current->County_name) !=0 ) )
    {
        if( strcmp(County_name,current->County_name) > 0 )
            current = current->right;
        else
            current = current->left;
    }
    return current;
}
void printPreOrder(AVLNode root)
{
    if(root != NULL)
    {
        printf("*%s---",root->County_name);
        printf("%s\n",root->county_related_data_file_name);
        printPreOrder(root->left);
        printPreOrder(root->right);
    }
}
AVLNode minValueNode(AVLNode node)
{
    AVLNode  current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}
int max(int a, int b)
{
    return (a > b)? a : b;
}
int height(AVLNode N)
{
    if (N == NULL)
        return 0;
    return N->height;
}
AVLNode rightRotate(AVLNode y)
{
    struct AVLnode *x = y->left;
    struct AVLnode *T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right))+1;
    x->height = max(height(x->left), height(x->right))+1;

    return x;
}
AVLNode leftRotate(AVLNode x)
{
    struct AVLnode *y = x->right;
    struct AVLnode *T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right))+1;
    y->height = max(height(y->left), height(y->right))+1;

    return y;
}
int getBalanceFactor(struct AVLnode *N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}
AVLNode newNode(char County_name [size],char county_related_data_file_name [size])
{
    AVLNode node = (struct AVLnode*)malloc(sizeof(struct AVLnode));
    strcpy(node->County_name,County_name);
    strcpy(node->county_related_data_file_name,county_related_data_file_name);
    node->left = NULL;
    node->right = NULL;
    node->height = 0;
    return(node);
}
AVLNode insertNode(AVLNode node,char County_name [size],char county_related_data_file_name [size])
{
    if (node == NULL)
        return(newNode(County_name,county_related_data_file_name));

    if (strcmp(County_name,node->County_name) < 0)
        node->left = insertNode(node->left,County_name,county_related_data_file_name);
    else if (strcmp(County_name,node->County_name) > 0)
        node->right = insertNode(node->right,County_name,county_related_data_file_name);
    else
        return node;

    node->height = 1 + max(height(node->left),height(node->right));

    int balance = getBalanceFactor(node);

    // If this node becomes unbalanced, then
    // there are 4 cases

    // Right Right Case
    if (balance > 1 && strcmp(County_name,node->left->County_name) < 0 )
        return rightRotate(node);

    // Left Left Case
    if (balance < -1 && strcmp(County_name,node->right->County_name) > 0 )
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && strcmp(County_name,node->left->County_name) > 0 )
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && strcmp(County_name,node->right->County_name) < 0 )
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}
AVLNode read(AVLNode a)
{
    char array [50];
    FILE *in;
    in = fopen("county information.txt","r");
    while(fgets(array,50,in) != NULL)
    {
        int i = 0 ;
        char* token[50] ;
        token[0] = strtok(array, "/");
        while (token [i] != 0)
        {
            i++ ;
            token [i] = strtok(NULL,"/");
        }
        a = insertNode(a,token[0],token[1]);
    }
    return a ;
    fclose(in);
}
void print_hash_table(hn hashTable [sizeOfTable])
{
    printf("#INDEX\t\tNAME\t\tID\n");
    printf("----------------------------------------------\n");
    for(int i=0; i<sizeOfTable; i++)
    {
        if(hashTable[i]->status ==0)
        {
            printf("%d\t\tEMPTY\t\tEMPTY\n",i);
            printf("___________________________________________\n");
        }
        else if(hashTable[i]->status ==-1)
        {
            printf("%d\t\t%s\t%d    (DELETED)\n",i,hashTable[i]->name,hashTable[i]->ID);
            printf("___________________________________________\n");
        }
        else
        {
            printf("%d\t\t%s\t%d\n",i,hashTable[i]->name,hashTable[i]->ID);
            printf("___________________________________________\n");
        }
    }
    printf("----------------------------------------------\n");

}
AVLNode deleteNode(AVLNode root,char County_name [size])
{
    if (root == NULL)
        return root;

    if (strcmp(County_name,root->County_name) < 0)
        root->left = deleteNode(root->left,County_name);

    else if (strcmp(County_name,root->County_name) > 0)
        root->right = deleteNode(root->right,County_name);

    else
    {
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct AVLnode *temp = root->left ? root->left :
                                       root->right;

            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else
                *root = *temp;

            free(temp);
        }
        else
        {
            struct AVLnode* temp = minValueNode(root->right);

            strcpy(root->County_name,temp->County_name) ;

            root->right = deleteNode(root->right,temp->County_name);
        }
    }

    if (root == NULL)
        return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = max(height(root->left), height(root->right)) + 1;

    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to check whether
    // this node became unbalanced)
    int balance = getBalanceFactor(root);

    // If this node becomes unbalanced, then there are 4 cases
    // Left Left Case
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}
void menu()
{
    printf("                            please choose\n");
    printf("------------------------------------------------------------------------------\n");
    printf("                      1-Print out counties sorted\n");
    printf("                      2-Search for a specific county\n");
    printf("                      3-Insert a new county\n");
    printf("                      4-Delete a specific county.\n");
    printf("                      5-Calculate tree height.\n");
    printf("                      6-Go to hash for resident data.\n");
    printf("------------------------------------------------------------------------------\n");
}
void menu1()
{
    printf("                            please choose\n");
    printf("------------------------------------------------------------------------------\n");
    printf("                      1-Print hashed table (including empty spots).\n");
    printf("                      2-Print out table size\n");
    printf("                      3-Print out used hash function.\n");
    printf("                      4-Insert a new record to hash table.\n");
    printf("                      5-Search for a specific record\n");
    printf("                      6-Delete a specific record.\n");
    printf("                      7-Save hash table back to file\n");
    printf("------------------------------------------------------------------------------\n");
}
void show_inf(AVLNode node)
{
    printf("the county name is :%s\n",node->County_name);
    printf("the county related data file name is : %s\n\n",node->county_related_data_file_name);

}
int get_hight_tree(AVLNode node)
{
    if(node==NULL)
        return 0;
    else
        return 1+ max(height(node->left),height(node->right)) ;
}
int insert_in_table(hn hashTable[sizeOfTable])
{
    int ID;
    char name[size];
    int age;
    char gender;
    printf("please enter the ID:\n");
    scanf("%d",&ID);
    for(int i=0; i<sizeOfTable; i++)
    {
        if(hashTable[i]->ID==ID)
        {
            printf("the key :%d in table please try again with new value\n\n",ID);
            return 0;
        }
    }
    printf("please enter the NAME:\n");
    gets(name);
    gets(name);
    printf("please enter the AGE:\n");
    scanf("%d",&age);
    printf("please enter the gender(M/F):\n");
    scanf(" %c", &gender);
    creatHash(ID,name,age,gender,hashTable);
    printf("ADD is done\n");
    return 1;
}
int search_in_table(hn hashTable[sizeOfTable])
{
    int ID;
    printf("please enter the key:\n");
    scanf("%d",&ID);

    for(int i=0; i<sizeOfTable; i++)
    {
        if(hashTable[i]->ID==ID)
        {
            printf("information of key:[name->%s ||| index->%d]\n",hashTable[i]->name,i);
            return 0;
        }
    }
    printf("sorry record not in table.\n");
    return 1;

}
int delete_form_table(hn hashTable[sizeOfTable])
{
    int ID;
    printf("please enter the key:\n");
    scanf("%d",&ID);
    for(int i=0; i<sizeOfTable; i++)
    {
        if(hashTable[i]->ID==ID&&hashTable[i]->status==1)
        {
            hashTable[i]->status=-1; //status =-1 means is deleted
            printf("DELETE IS DONE.\n\n");
            return 0;
        }
    }
    printf("The Record Not Here Or Deleted \n\n");
    return 1;
}
void print_in_file(hn hashTable[sizeOfTable],char filename[size])
{
    char array [50];
    FILE *in;
    in = fopen(filename,"a");
    fprintf(in,"\n         -------HASHTABLE--------\n");

    fprintf(in,"#INDEX\t\tNAME\t\tID\n");
    fprintf(in,"----------------------------------------------\n");
    for(int i=0; i<sizeOfTable; i++)
    {
        if(hashTable[i]->status !=1)
        {
            fprintf(in,"%d\t\tEMPTY\t\tEMPTY\n",i);
            fprintf(in,"___________________________________________\n");
        }
        else
        {
            fprintf(in,"%d\t\t%s\t%d\n",i,hashTable[i]->name,hashTable[i]->ID);
            fprintf(in,"___________________________________________\n");
        }
    }

    fclose(in);
}


int main()
{
    AVLNode root = NULL ;
    char t [15] ;
    root= read(root);
    while(2)
    {
        menu();
        gets(t);
        int k = atoi(t) ;
        switch(k)
        {
        case 1:
        {
            printf("The list of county name and file name:\n");
            printf("------------------------\n");
            printPreOrder( root);
            printf("------------------------\n\n");
        }
        break;
        case 2:
        {
            char name [size] ;
            printf("please enter the county name(the first letter must be capital):\n") ;
            gets(name) ;
            AVLNode o = find_County_name(name,root) ;
            if(o==NULL)
                printf("the county not here please try again\n");
            else
            {
                printf("the information of the county:\n") ;
                show_inf(o) ;
            }
        }
        break;
        case 3:
        {
            char name_county [size] ;
            printf("please enter the county name(the first letter must be capital):\n") ;
            gets(name_county);
            char name_file [size] ;
            printf("please enter the county name file(the first letter must be capital):\n") ;
            gets(name_file);
            root=insertNode(root,name_county,name_file);
            printf("the insert done.\n");
        }
        break;
        case 4:
        {
            char name_county_DE [size];
            printf("please enter the county name(the first letter must be capital):\n") ;
            gets(name_county_DE);
            AVLNode h = find_County_name(name_county_DE,root) ;
            if(h==NULL)
                printf("the county not here please try again\n");
            else
            {
                deleteNode(root,name_county_DE);
                printf("Delete Done.\n");
            }
        }
        break;
        case 5:
        {
            printf("The Hight of Tree is :%d\n",get_hight_tree(root));
        }
        break;
        case 6:
        {
            char filename [size];
            printf("please inter the file name EX(hebron.txt)\n");
            gets(filename);
            sizeOfTable = computeSize(filename);
            intilize(hashTable);
            if(read_hash(hashTable,filename)==0)
                break;
            else
            {
                while(2)
                {
                    char d [15] ;
                    menu1();
                    gets(d);
                    int c = atoi(d) ;
                    switch(c)
                    {
                    case 1:
                        print_hash_table(hashTable);
                        break;
                    case 2:
                    {
                        printf("------------------------\n");
                        printf("The Size Of Table : %d\n",sizeOfTable);
                        printf("------------------------\n");
                    }
                    break;
                    case 3:
                        printf("\n-->Quadratic Hashing\n-->g(x) = (function + i^2) %% (size of hash table)\n\n");
                        break;
                    case 4:
                        insert_in_table(hashTable);
                        break;
                    case 5:
                        search_in_table(hashTable);
                        break;
                    case 6:
                        delete_form_table(hashTable);
                        break;
                    case 7:
                        print_in_file(hashTable,filename);
                        break;
                    }
                }
            }
        }
        }
    }
    return 0;
}
