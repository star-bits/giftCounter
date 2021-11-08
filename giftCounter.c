#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct node* node_pointer;
typedef struct node
{
    char name[50];
    int count;
    node_pointer link;
}node;

typedef struct day_node* day_node_pointer;
typedef struct day_node
{
    char date[11];
    int day_number;
    char day_of_week;
    int gift_hour[24];
    int gift_day_total;
    day_node_pointer link;
}day_node;

void getName(char* s, char* temp_name);
void storeName(node_pointer *initial_pointer, char* temp_name);
void bubbleSort(node_pointer *initial_pointer);
void isoToRaw(char* start_date_iso, char* start_date_raw, int sizeof_start_date_raw);
void isoNextDay(char* end_date_iso, char* end_date_iso_next_day, int sizeof_end_date_iso_next_day);
void rawToIso(char* s, char* gift_date_iso, int sizeof_gift_date_iso);

int main(void) //곰재주 엄마돈 단톡방 선착순 카운터. 자바칩 프라푸치노 기프티콘을 보내주고 싶다? https://open.kakao.com/me/swji621
{
    FILE* fp;
    fp=fopen("a.txt", "r");
    
    char s[200];
    char temp_name[50];
    char giver_pattern[]="선착순 선물 게임을 시작합니다! 기회는 단 10분간, 선착순";
    char receiver_pattern[]="축하합니다. 선착순 선물에 당첨되었어요!";

    //(시작일)과 (종료일의 다음날)에 채팅이 있는 경우에만 가능함.
    
    printf("시작일(YYYY-MM-DD): ");
    char start_date_iso[11];
    scanf("%s", start_date_iso);
    //
    char start_date_raw[33];
    memset(start_date_raw, 0, sizeof(start_date_raw));
    int sizeof_start_date_raw=sizeof(start_date_raw);
    //printf("%d\n", sizeof_start_date_raw);
    isoToRaw(start_date_iso, start_date_raw, sizeof_start_date_raw);
    //printf("%s\n", start_date_raw);

    printf("시작일의 요일(M/T/W/R/F/S/U): ");
    char start_date_day_of_week;
    scanf(" %c", &start_date_day_of_week);
    
    printf("종료일(YYYY-MM-DD): ");
    char end_date_iso[11];
    scanf("%s", end_date_iso);
    //
    char end_date_iso_next_day[11];
    memset(end_date_iso_next_day, 0, sizeof(end_date_iso_next_day));
    int sizeof_end_date_iso_next_day=sizeof(end_date_iso_next_day);
    //printf("%d\n", sizeof_end_date_iso_next_day);
    isoNextDay(end_date_iso, end_date_iso_next_day, sizeof_end_date_iso_next_day);
    //printf("%s\n", end_date_iso_next_day);
    char end_date_raw_next_day[33];
    memset(end_date_raw_next_day, 0, sizeof(end_date_raw_next_day));
    int sizeof_end_date_raw_next_day=sizeof(end_date_raw_next_day);
    //printf("%d\n", sizeof_end_date_raw_next_day);
    isoToRaw(end_date_iso_next_day, end_date_raw_next_day, sizeof_end_date_raw_next_day);
    //printf("%s\n", end_date_raw_next_day);

    printf("\n");
    
    char day_of_week_array[13]={'M', 'T', 'W', 'R', 'F', 'S', 'U', 'M', 'T', 'W', 'R', 'F', 'S'};
    int start_date_day_of_week_number;
    for(int i=0; ; i++)
    {
        if(start_date_day_of_week==day_of_week_array[i])
        {
            start_date_day_of_week_number=i;
            break;
        }
    }
    
    day_node_pointer head=(day_node_pointer)malloc(sizeof(day_node));
    head->link=NULL;
    
    int day_count=0;
    char temp_date_iso[11];
    memset(temp_date_iso, 0, sizeof(temp_date_iso));
    strcpy(temp_date_iso, start_date_iso);
    for(day_node_pointer day_temp=head; ; day_temp=day_temp->link)
    {
        //save temp_date_iso to current_date_iso.
        char current_date_iso[11];
        memset(current_date_iso, 0, sizeof(current_date_iso));
        strcpy(current_date_iso, temp_date_iso);
        
        //new node
        day_node_pointer new=(day_node_pointer)malloc(sizeof(day_node));

        //new->date
        strcpy(new->date, temp_date_iso);
        //update temp_date_iso.
        char next_date_iso[11];
        memset(next_date_iso, 0, sizeof(next_date_iso));
        isoNextDay(temp_date_iso, next_date_iso, sizeof(next_date_iso));
        strcpy(temp_date_iso, next_date_iso);
        
        //new->day_of_week
        new->day_of_week=day_of_week_array[start_date_day_of_week_number+(day_count)%7];

        //new->day_number
        new->day_number=day_count;
        day_count=day_count+1;
        
        //to print list.
        new->link=NULL;
        
        //connecting nodes.
        day_temp->link=new;
        
        //break if match.
        char* end_date_match=strstr(current_date_iso, end_date_iso);
        if(end_date_match!=NULL)
            break;
    }

    day_node_pointer day_temp=head->link;
    do
    {
        for(int i=0; i<24; i++)
            day_temp->gift_hour[i]=0;
        day_temp->gift_day_total=0;
        day_temp=day_temp->link;
    }
    while(day_temp!=NULL);

    node_pointer giver_pointer=(node_pointer)malloc(sizeof(node));
    giver_pointer->link=NULL;
    node_pointer receiver_pointer=(node_pointer)malloc(sizeof(node));
    receiver_pointer->link=NULL;

    char s_date[33];
    bool start_date_found=false, end_date_next_day_found=false;
    int giver_count=0, receiver_count=0;
    while(fgets(s, sizeof(s), fp)!=NULL)
    {
        memset(s_date, 0, sizeof(s_date));
        strncpy(s_date, s, sizeof(s_date));

        //until find start date.
        if(start_date_found==false)
        {
            char* start_date_match=strstr(s_date, start_date_raw);
            if(start_date_match!=NULL)
                start_date_found=true;
        }
        
        //until find end date. if so, break.
        if(end_date_next_day_found==false)
        {
            char* end_date_next_day_match=strstr(s_date, end_date_raw_next_day);
            if(end_date_next_day_match!=NULL)
                end_date_next_day_found=true;
        }
        if(end_date_next_day_found==true)
            break;
        
        //true after finding start date.
        if(start_date_found==true)
        {
            
            char* giver_match=strstr(s, giver_pattern);
            if(giver_match!=NULL)
            {
                giver_count++;
                getName(s, temp_name);
                storeName(&giver_pointer, temp_name);
                      
                //extract date.
                char gift_date_iso[11];
                memset(gift_date_iso, 0, sizeof(gift_date_iso));
                int sizeof_gift_date_iso=sizeof(gift_date_iso);
                //
                rawToIso(s, gift_date_iso, sizeof_gift_date_iso);
                //printf("%s\n", gift_date_iso);

                //search for the extracted date.
                for(day_node_pointer day_temp=head; ; day_temp=day_temp->link)
                {
                    char* saved_date_match=strstr(gift_date_iso, day_temp->date);
                    if(saved_date_match!=NULL)
                    {
                        //increase gift_day_total.
                        day_temp->gift_day_total++;

                        //increase gift_hour[x];
                        int i, j, k, l;
                        
                        for(i=0; ; i++)
                        {
                            if(strncmp(&s[i], " ", 1)==0)
                                break;
                        }
                        for(j=i+1; ; j++)
                        {
                            if(strncmp(&s[j], " ", 1)==0)
                                break;
                        }
                        for(k=j+1; ; k++)
                        {
                            if(strncmp(&s[k], " ", 1)==0)
                                break;
                        }
                        for(l=k+1; ; l++)
                        {
                            if(strncmp(&s[l], ":", 1)==0)
                                break;
                        }
                        
                        char month_identifier[10];
                        memset(month_identifier, 0, sizeof(month_identifier));
                        
                        if(l-k==10)
                        {
                            char hour_identifier[10];
                            memset(hour_identifier, 0, sizeof(hour_identifier));
                            
                            strncpy(hour_identifier, s+k+1, 9);
                            
                            char hour_0[10]="오전 12";
                            if(strncmp(hour_0, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[0]++;
                            
                            char hour_10[10]="오전 10";
                            if(strncmp(hour_10, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[10]++;
                            
                            char hour_11[10]="오전 11";
                            if(strncmp(hour_11, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[11]++;
                            
                            char hour_12[10]="오후 12";
                            if(strncmp(hour_12, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[12]++;
                            
                            char hour_22[10]="오후 10";
                            if(strncmp(hour_22, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[22]++;
                            
                            char hour_23[10]="오후 11";
                            if(strncmp(hour_23, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[23]++;
                        }
                        if(l-k==9)
                        {
                            char hour_identifier[9];
                            memset(hour_identifier, 0, sizeof(hour_identifier));
                            
                            strncpy(hour_identifier, s+k+1, 8);
                            
                            char hour_1[10]="오전 1";
                            if(strncmp(hour_1, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[1]++;
                            
                            char hour_2[10]="오전 2";
                            if(strncmp(hour_2, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[2]++;
                            
                            char hour_3[10]="오전 3";
                            if(strncmp(hour_3, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[3]++;
                            
                            char hour_4[10]="오전 4";
                            if(strncmp(hour_4, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[4]++;
                            
                            char hour_5[10]="오전 5";
                            if(strncmp(hour_5, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[5]++;
                            
                            char hour_6[10]="오전 6";
                            if(strncmp(hour_6, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[6]++;
                            
                            char hour_7[10]="오전 7";
                            if(strncmp(hour_7, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[7]++;
                            
                            char hour_8[10]="오전 8";
                            if(strncmp(hour_8, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[8]++;
                            
                            char hour_9[10]="오전 9";
                            if(strncmp(hour_9, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[9]++;
                            
                            char hour_13[10]="오후 1";
                            if(strncmp(hour_13, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[13]++;
                            
                            char hour_14[10]="오후 2";
                            if(strncmp(hour_14, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[14]++;
                            
                            char hour_15[10]="오후 3";
                            if(strncmp(hour_15, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[15]++;
                            
                            char hour_16[10]="오후 4";
                            if(strncmp(hour_16, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[16]++;
                            
                            char hour_17[10]="오후 5";
                            if(strncmp(hour_17, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[17]++;
                            
                            char hour_18[10]="오후 6";
                            if(strncmp(hour_18, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[18]++;
                            
                            char hour_19[10]="오후 7";
                            if(strncmp(hour_19, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[19]++;
                            
                            char hour_20[10]="오후 8";
                            if(strncmp(hour_20, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[20]++;
                            
                            char hour_21[10]="오후 9";
                            if(strncmp(hour_21, hour_identifier, sizeof(hour_identifier))==0)
                                day_temp->gift_hour[21]++;
                        }
                    }
                    if(day_temp->link==NULL)
                        break;
                }
            }
            
            char* receiver_match=strstr(s, receiver_pattern);
            if(receiver_match!=NULL)
            {
                receiver_count++;
                getName(s, temp_name);
                storeName(&receiver_pointer, temp_name);
            }

            memset(s, '\0', sizeof(s));
            memset(temp_name, '\0', sizeof(temp_name));
        }
    }
        
    int m_hourly_counter[24];
    int t_hourly_counter[24];
    int w_hourly_counter[24];
    int r_hourly_counter[24];
    int f_hourly_counter[24];
    int s_hourly_counter[24];
    int u_hourly_counter[24];
    
    for(int i; i<24; i++)
    {
        m_hourly_counter[i]=0;
        t_hourly_counter[i]=0;
        w_hourly_counter[i]=0;
        r_hourly_counter[i]=0;
        f_hourly_counter[i]=0;
        s_hourly_counter[i]=0;
        u_hourly_counter[i]=0;
    }

    for(day_node_pointer day_temp=head->link; ; day_temp=day_temp->link)
    {
        if(day_temp->day_of_week=='M')
            for(int i=0; i<24; i++)
                m_hourly_counter[i]=m_hourly_counter[i]+day_temp->gift_hour[i];
        if(day_temp->day_of_week=='T')
            for(int i=0; i<24; i++)
                t_hourly_counter[i]=t_hourly_counter[i]+day_temp->gift_hour[i];
        if(day_temp->day_of_week=='W')
            for(int i=0; i<24; i++)
                w_hourly_counter[i]=w_hourly_counter[i]+day_temp->gift_hour[i];
        if(day_temp->day_of_week=='R')
            for(int i=0; i<24; i++)
                r_hourly_counter[i]=r_hourly_counter[i]+day_temp->gift_hour[i];
        if(day_temp->day_of_week=='F')
            for(int i=0; i<24; i++)
                f_hourly_counter[i]=f_hourly_counter[i]+day_temp->gift_hour[i];
        if(day_temp->day_of_week=='S')
            for(int i=0; i<24; i++)
                s_hourly_counter[i]=s_hourly_counter[i]+day_temp->gift_hour[i];
        if(day_temp->day_of_week=='U')
            for(int i=0; i<24; i++)
                u_hourly_counter[i]=u_hourly_counter[i]+day_temp->gift_hour[i];
        if(day_temp->link==NULL)
            break;
    }
    
    bubbleSort(&giver_pointer);
    bubbleSort(&receiver_pointer);
    
    
    
    //선착순 쏜 횟수
    printf("선착순 쏜 횟수: %d\n", giver_count);
    
    //선착순 쏜 사람 수
    node_pointer temp=giver_pointer->link;
    int giver_number=0;
    do
    {
        temp=temp->link;
        giver_number++;
    }
    while(temp!=NULL);
    printf("선착순 쏜 사람 수: %d\n\n", giver_number);
    
    //선착순 쏜 사람
    temp=giver_pointer->link;
    do
    {
        printf("%s: %d\n", temp->name, temp->count);
        temp=temp->link;
    }
    while(temp!=NULL);
    
    //선착순 받은 횟수
    printf("\n선착순 받은 횟수: %d\n", receiver_count);
    
    //선착순 받은 사람 수
    temp=receiver_pointer->link;
    int receiver_number=0;
    do
    {
        temp=temp->link;
        receiver_number++;
    }
    while(temp!=NULL);
    printf("선착순 받은 사람 수: %d\n\n", receiver_number);

    //선착순 받은 사람
    temp=receiver_pointer->link;
    do
    {
        printf("%s: %d\n", temp->name, temp->count);
        temp=temp->link;
    }
    while(temp!=NULL);
    
    printf("\n");


    
    //Excel friendly output

    printf("선착순 쏜 횟수: %d\n", giver_count);
    
    temp=giver_pointer->link;
    giver_number=0;
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
        printf("%s\n", temp->name);
        temp=temp->link;
    }
    while(temp!=NULL);
    temp=giver_pointer->link;
    do
    {
        printf("%d\n", temp->count);
        temp=temp->link;
    }
    while(temp!=NULL);
    
    printf("\n선착순 받은 횟수: %d\n", receiver_count);
    
    temp=receiver_pointer->link;
    receiver_number=0;
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
        printf("%s\n", temp->name);
        temp=temp->link;
    }
    while(temp!=NULL);
    temp=receiver_pointer->link;
    do
    {
        printf("%d\n", temp->count);
        temp=temp->link;
    }
    while(temp!=NULL);
    
    printf("\n");
    
    
    
    //Raw data
    day_temp=head->link;
    do
    {
        printf("%s %c", day_temp->date, day_temp->day_of_week);
        for(int i=0; i<24; i++)
            printf(" %2d", day_temp->gift_hour[i]);
        printf(" %2d", day_temp->gift_day_total);
        printf("\n");
        day_temp=day_temp->link;
    }
    while(day_temp!=NULL);
    
    printf("\n");
    
    
    
    //일별 - Excel friendly output
    day_temp=head->link;
    do
    {
        printf("%s", day_temp->date);
        printf("\n");
        day_temp=day_temp->link;
    }
    while(day_temp!=NULL);
    day_temp=head->link;
    do
    {
        printf("%d", day_temp->gift_day_total);
        printf("\n");
        day_temp=day_temp->link;
    }
    while(day_temp!=NULL);

    printf("\n");
    
    
    
    //요일별 시간당
    printf("H");
    for(int i=0; i<24; i++)
        printf("%3d", i);
    printf("\n");
    printf("M");
    for(int i=0; i<24; i++)
        printf("%3d", m_hourly_counter[i]);
    printf("\n");
    printf("T");
    for(int i=0; i<24; i++)
        printf("%3d", t_hourly_counter[i]);
    printf("\n");
    printf("W");
    for(int i=0; i<24; i++)
        printf("%3d", w_hourly_counter[i]);
    printf("\n");
    printf("R");
    for(int i=0; i<24; i++)
        printf("%3d", r_hourly_counter[i]);
    printf("\n");
    printf("F");
    for(int i=0; i<24; i++)
        printf("%3d", f_hourly_counter[i]);
    printf("\n");
    printf("S");
    for(int i=0; i<24; i++)
        printf("%3d", s_hourly_counter[i]);
    printf("\n");
    printf("U");
    for(int i=0; i<24; i++)
        printf("%3d", u_hourly_counter[i]);
    printf("\n");
    
    printf("\n");

    
    
    //요일별 시간당 - Excel friendly output
    printf("Monday:\n");
    for(int i=0; i<24; i++)
        printf("%d\n", m_hourly_counter[i]);
    printf("Thursday:\n");
    for(int i=0; i<24; i++)
        printf("%d\n", t_hourly_counter[i]);
    printf("Wednesday:\n");
    for(int i=0; i<24; i++)
        printf("%d\n", w_hourly_counter[i]);
    printf("thuRsday:\n");
    for(int i=0; i<24; i++)
        printf("%d\n", r_hourly_counter[i]);
    printf("Friday:\n");
    for(int i=0; i<24; i++)
        printf("%d\n", f_hourly_counter[i]);
    printf("Saturday:\n");
    for(int i=0; i<24; i++)
        printf("%d\n", s_hourly_counter[i]);
    printf("sUnday:\n");
    for(int i=0; i<24; i++)
        printf("%d\n", u_hourly_counter[i]);
    
    printf("\n");
    
    
    
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

void isoToRaw(char* start_date_iso, char* start_date_raw, int sizeof_start_date_raw)
{
    char start_date_year[5];
    memset(start_date_year, 0, sizeof(start_date_year));
    char start_date_month[3];
    memset(start_date_month, 0, sizeof(start_date_month));
    char start_date_day[3];
    memset(start_date_day, 0, sizeof(start_date_day));
    strncpy(start_date_year, start_date_iso, 4);
    strncpy(start_date_month, start_date_iso+5, 2);
    strncpy(start_date_day, start_date_iso+8, 2);
    //printf("%lu\n", sizeof(start_date_raw));
    
    if(48==start_date_month[0] && 48==start_date_day[0])
        snprintf(start_date_raw, sizeof_start_date_raw, "%s%s%c%s%c%s", start_date_year, "년 ", start_date_month[1], "월 ", start_date_day[1], "일");
    if(48==start_date_month[0] && 48!=start_date_day[0])
        snprintf(start_date_raw, sizeof_start_date_raw, "%s%s%c%s%s%s", start_date_year, "년 ", start_date_month[1], "월 ", start_date_day, "일");
    if(48!=start_date_month[0] && 48==start_date_day[0])
        snprintf(start_date_raw, sizeof_start_date_raw, "%s%s%s%s%c%s", start_date_year, "년 ", start_date_month, "월 ", start_date_day[1], "일");
    if(48!=start_date_month[0] && 48!=start_date_day[0])
        snprintf(start_date_raw, sizeof_start_date_raw, "%s%s%s%s%s%s", start_date_year, "년 ", start_date_month, "월 ", start_date_day, "일");
    //printf("%s\n", start_date_raw);
}

void isoNextDay(char* end_date_iso, char* end_date_iso_next_day, int sizeof_end_date_iso_next_day)
{
    char year_temp[5];
    memset(year_temp, 0, sizeof(year_temp));
    char month_day_temp[6];
    memset(month_day_temp, 0, sizeof(month_day_temp));
    strncpy(year_temp, end_date_iso, 4);
    strncpy(month_day_temp, end_date_iso+5, 5);
    //printf("%lu\n", sizeof(end_date_iso_next_day));
    
    int int_leap_year_test=atoi(year_temp);
    if(int_leap_year_test%4==0)
    {
        char jan_last[]="01-31";
        char* jan_month_day_cmp=strstr(month_day_temp, jan_last);
        if(jan_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "02-01");

        char feb_last[]="02-29";
        char* feb_month_day_cmp=strstr(month_day_temp, feb_last);
        if(feb_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "03-01");
        
        char mar_last[]="03-31";
        char* mar_month_day_cmp=strstr(month_day_temp, mar_last);
        if(mar_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "04-01");
        
        char apr_last[]="04-30";
        char* apr_month_day_cmp=strstr(month_day_temp, apr_last);
        if(apr_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "05-01");
        
        char may_last[]="05-31";
        char* may_month_day_cmp=strstr(month_day_temp, may_last);
        if(may_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "06-01");
        
        char jun_last[]="06-30";
        char* jun_month_day_cmp=strstr(month_day_temp, jun_last);
        if(jun_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "07-01");
        
        char jul_last[]="07-31";
        char* jul_month_day_cmp=strstr(month_day_temp, jul_last);
        if(jul_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "08-01");
        
        char aug_last[]="08-31";
        char* aug_month_day_cmp=strstr(month_day_temp, aug_last);
        if(aug_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "09-01");
        
        char sep_last[]="09-30";
        char* sep_month_day_cmp=strstr(month_day_temp, sep_last);
        if(sep_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "10-01");
        
        char oct_last[]="10-31";
        char* oct_month_day_cmp=strstr(month_day_temp, oct_last);
        if(oct_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "11-01");
        
        char nov_last[]="11-30";
        char* nov_month_day_cmp=strstr(month_day_temp, nov_last);
        if(nov_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "12-01");
        
        char dec_last[]="12-31";
        char* dec_month_day_cmp=strstr(month_day_temp, dec_last);
        if(dec_month_day_cmp!=NULL)
        {
            int int_year=atoi(year_temp);
            int_year=int_year+1;
            sprintf(year_temp, "%d", int_year);
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "01-01");
        }
        
        if(jan_month_day_cmp==NULL && feb_month_day_cmp==NULL && mar_month_day_cmp==NULL && apr_month_day_cmp==NULL && may_month_day_cmp==NULL && jun_month_day_cmp==NULL && jul_month_day_cmp==NULL && aug_month_day_cmp==NULL && sep_month_day_cmp==NULL && oct_month_day_cmp==NULL && nov_month_day_cmp==NULL && dec_month_day_cmp==NULL)
        {
            char day_temp[3];
            memset(day_temp, 0, sizeof(day_temp));
            char month_temp[3];
            memset(month_temp, 0, sizeof(month_temp));
            strncpy(month_temp, month_day_temp, 2);
            strncpy(day_temp, month_day_temp+3, 2);
            
            int int_day=atoi(day_temp);
            int_day=int_day+1;
            sprintf(day_temp, "%d", int_day);
            
            if(int_day<10)
            {
                char day_temp_double_digit[3];
                memset(day_temp_double_digit, 0, sizeof(day_temp_double_digit));
                snprintf(day_temp_double_digit, sizeof(day_temp_double_digit), "%s%s", "0", day_temp);
                snprintf(month_day_temp, sizeof(month_day_temp), "%s%s%s", month_temp, "-", day_temp_double_digit);
            }
            else
                snprintf(month_day_temp, sizeof(month_day_temp), "%s%s%s", month_temp, "-", day_temp);
            
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", month_day_temp);
        }
    }
    else
    {
        char jan_last[]="01-31";
        char* jan_month_day_cmp=strstr(month_day_temp, jan_last);
        if(jan_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "02-01");
        
        char feb_last[]="02-28";
        char* feb_month_day_cmp=strstr(month_day_temp, feb_last);
        if(feb_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "03-01");
        
        char mar_last[]="03-31";
        char* mar_month_day_cmp=strstr(month_day_temp, mar_last);
        if(mar_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "04-01");
        
        char apr_last[]="04-30";
        char* apr_month_day_cmp=strstr(month_day_temp, apr_last);
        if(apr_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "05-01");
        
        char may_last[]="05-31";
        char* may_month_day_cmp=strstr(month_day_temp, may_last);
        if(may_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "06-01");
        
        char jun_last[]="06-30";
        char* jun_month_day_cmp=strstr(month_day_temp, jun_last);
        if(jun_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "07-01");
        
        char jul_last[]="07-31";
        char* jul_month_day_cmp=strstr(month_day_temp, jul_last);
        if(jul_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "08-01");
        
        char aug_last[]="08-31";
        char* aug_month_day_cmp=strstr(month_day_temp, aug_last);
        if(aug_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "09-01");
        
        char sep_last[]="09-30";
        char* sep_month_day_cmp=strstr(month_day_temp, sep_last);
        if(sep_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "10-01");
        
        char oct_last[]="10-31";
        char* oct_month_day_cmp=strstr(month_day_temp, oct_last);
        if(oct_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "11-01");
        
        char nov_last[]="11-30";
        char* nov_month_day_cmp=strstr(month_day_temp, nov_last);
        if(nov_month_day_cmp!=NULL)
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "12-01");
        
        char dec_last[]="12-31";
        char* dec_month_day_cmp=strstr(month_day_temp, dec_last);
        if(dec_month_day_cmp!=NULL)
        {
            int int_year=atoi(year_temp);
            int_year=int_year+1;
            sprintf(year_temp, "%d", int_year);
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", "01-01");
        }
        
        if(jan_month_day_cmp==NULL && feb_month_day_cmp==NULL && mar_month_day_cmp==NULL && apr_month_day_cmp==NULL && may_month_day_cmp==NULL && jun_month_day_cmp==NULL && jul_month_day_cmp==NULL && aug_month_day_cmp==NULL && sep_month_day_cmp==NULL && oct_month_day_cmp==NULL && nov_month_day_cmp==NULL && dec_month_day_cmp==NULL)
        {
            char day_temp[3];
            memset(day_temp, 0, sizeof(day_temp));
            char month_temp[3];
            memset(month_temp, 0, sizeof(month_temp));
            strncpy(month_temp, month_day_temp, 2);
            strncpy(day_temp, month_day_temp+3, 2);
            
            int int_day=atoi(day_temp);
            int_day=int_day+1;
            sprintf(day_temp, "%d", int_day);
            
            if(int_day<10)
            {
                char day_temp_double_digit[3];
                memset(day_temp_double_digit, 0, sizeof(day_temp_double_digit));
                snprintf(day_temp_double_digit, sizeof(day_temp_double_digit), "%s%s", "0", day_temp);
                snprintf(month_day_temp, sizeof(month_day_temp), "%s%s%s", month_temp, "-", day_temp_double_digit);
            }
            else
                snprintf(month_day_temp, sizeof(month_day_temp), "%s%s%s", month_temp, "-", day_temp);
            
            snprintf(end_date_iso_next_day, sizeof_end_date_iso_next_day, "%s%s%s", year_temp, "-", month_day_temp);
        }
    }
    //printf("%s\n", end_date_iso_next_day);
}

void rawToIso(char* s, char* gift_date_iso, int sizeof_gift_date_iso)
{
    int i, j, k;
    for(i=0; ; i++)
    {
        if(strncmp(&s[i], " ", 1)==0)
            break;
    }
    for(j=i+1; ; j++)
    {
        if(strncmp(&s[j], " ", 1)==0)
            break;
    }
    for(k=j+1; ; k++)
    {
        if(strncmp(&s[k], " ", 1)==0)
            break;
    }
    //printf("%d %d", j-i, k-j);
    
    char year_temp[5];
    memset(year_temp, 0, sizeof(year_temp));
    char month_temp[3];
    memset(month_temp, 0, sizeof(month_temp));
    char day_temp[3];
    memset(day_temp, 0, sizeof(day_temp));
    
    strncpy(year_temp, s, 4);
    
    if(j-i==5)
    {
        char month_temp_second[2];
        strncpy(month_temp_second, s+i+1, 1);
        snprintf(month_temp, sizeof(month_temp), "%s%s", "0", month_temp_second);
    }
    if(j-i==6)
    {
        strncpy(month_temp, s+i+1, 2);
    }
    //printf("%s", month_temp);
    
    if(k-j==5)
    {
        char day_temp_second[2];
        strncpy(day_temp_second, s+j+1, 1);
        snprintf(day_temp, sizeof(day_temp), "%s%s", "0", day_temp_second);
    }
    if(k-j==6)
    {
        strncpy(day_temp, s+j+1, 2);
    }
    //printf("%s", day_temp);
    
    snprintf(gift_date_iso, sizeof_gift_date_iso, "%s%s%s%s%s", year_temp, "-", month_temp, "-", day_temp);
    //printf("%s\n", gift_date_iso);
}
