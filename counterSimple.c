#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node* node_pointer;
typedef struct node
{
    char name[50];
    int count;
    node_pointer link;
}node;

void getName(char* s, char* temp_name);
void storeName(node_pointer *initial_pointer, char* temp_name);
void bubbleSort(node_pointer *initial_pointer);

int main(void) //곰재주 엄마돈 단톡방 선착순 카운터. 자바칩 프라푸치노 기프티콘을 보내주고 싶다? https://open.kakao.com/me/swji621
{
    FILE* fp;
    fp=fopen("a.txt", "r");
    
    char s[200];
    char temp_name[50];
    char giver_pattern[]="선착순 선물 게임을 시작합니다! 기회는 단 10분간, 선착순";
    char receiver_pattern[]="축하합니다. 선착순 선물에 당첨되었어요!";

    node_pointer giver_pointer=(node_pointer)malloc(sizeof(node));
    giver_pointer->link=NULL;
    node_pointer receiver_pointer=(node_pointer)malloc(sizeof(node));
    receiver_pointer->link=NULL;

    int giver_count=0, receiver_count=0;
    while(fgets(s, sizeof(s), fp)!=NULL)
    {
        char* giver_match=strstr(s, giver_pattern);
        if(giver_match!=NULL)
        {
            giver_count++;
            getName(s, temp_name);
            storeName(&giver_pointer, temp_name);
        }
        
        char* receiver_match=strstr(s, receiver_pattern);
        if(receiver_match!=NULL)
        {
            receiver_count++;
            getName(s, temp_name);
            storeName(&receiver_pointer, temp_name);
        }

        memset(s, '\0', sizeof s);
        memset(temp_name, '\0', sizeof temp_name);
    }
    
    bubbleSort(&giver_pointer);
    bubbleSort(&receiver_pointer);
    
    printf("선착순 쏜 횟수: %d\n", giver_count);
    
    node_pointer temp=giver_pointer->link;
    int giver_number=0;
    do
    {
        temp=temp->link;
        giver_number++;
    }
    while(temp!=NULL);
    printf("선착순 쏜 사람 수: %d\n\n", giver_number);
    
    temp=giver_pointer->link;
    do
    {
        printf("%s: %d\n", temp->name, temp->count);
        temp=temp->link;
    }
    while(temp!=NULL);
    
    printf("\n선착순 받은 횟수: %d\n", receiver_count);
    
    temp=receiver_pointer->link;
    int receiver_number=0;
    do
    {
        temp=temp->link;
        receiver_number++;
    }
    while(temp!=NULL);
    printf("선착순 받은 사람 수: %d\n\n", receiver_number);

    temp=receiver_pointer->link;
    do
    {
        printf("%s: %d\n", temp->name, temp->count);
        temp=temp->link;
    }
    while(temp!=NULL);
    
    return 0;
}

void getName(char* s, char* temp_name)
{
    int i, j, k;
    int l=0;
    
    for(i=0; ; i++)
    {
        if(strncmp(&s[i], ":", 1)==0)
            break;
    }
    for(j=i; ; j++)
    {
        if(strncmp(&s[j], ",", 1)==0)
            break;
    }
    for(k=j+2; ; k++)
    {
        temp_name[l]=s[k];
        l++;
        if(strncmp(&s[k+2], ":", 1)==0)
            break;
    }
}

void storeName(node_pointer *initial_pointer, char* t_name)
{
    node_pointer temp;
    char temp_name[50];
    strcpy(temp_name, t_name);
    
    for(temp=*initial_pointer; ; temp=temp->link)
    {
        if(strncmp(temp->name, temp_name, sizeof(temp_name))==0)
        {
            //printf("Name found. Increase the counter.\n");
            (temp->count)++;
            break;
        }
        if(temp->link==NULL)
        {
            //printf("Name not found in any node. Create a new node.\n");
            node_pointer new=(node_pointer)malloc(sizeof(node));
            strcpy(new->name, temp_name);
            //printf("%s\n", new->name);
            new->count=1;
            new->link=NULL;
            
            temp->link=new;
            temp=new;
            break;
        }
        //printf("Name not found on this node. Move to the next node.\n");
    }
}

void bubbleSort(node_pointer *initial_pointer)
{
    int temp_count;
    char temp_name[50];
    
    node_pointer temp1, temp2;
    for(temp1=(*initial_pointer)->link; temp1->link!=NULL; temp1=temp1->link)
    {
        for(temp2=temp1->link; temp2!=NULL; temp2=temp2->link)
        {
            if(temp1->count<temp2->count)
            {
                temp_count=temp1->count;
                temp1->count=temp2->count;
                temp2->count=temp_count;
                
                strcpy(temp_name, temp1->name);
                strcpy(temp1->name, temp2->name);
                strcpy(temp2->name, temp_name);
            }
        }
    }
}
