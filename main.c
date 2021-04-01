#include <stdio.h>
#include <stdlib.h>
#define infinite 65536
typedef struct {
    int weight;
    int ID;
    struct link* next;
} link;
typedef struct {
    int dist[8];
    int sign[8];
    link * head;
    link * memory;
} Graph;
typedef struct {
    int array[10];
    int top;
}Stack;
void push(Stack * stack,int CityID){
    stack->array[stack->top++] = CityID;
}
void pop(Stack * stack){
    stack->top--;
}
// 初始化链表
void init(Graph *G,int CityID,int TargetCityID,int weight){
    link * p = (link*)malloc(sizeof(link));//创建一个头结点
    if (G[CityID].head == NULL){
        link * head = (link*)malloc(sizeof(link));
        printf("succes --- 初始化表头%d\n",CityID);
        G[CityID].head = head;
        G[CityID].memory = head;
    }else{
        //printf("warming --- 初始化表头%d已创建 目标地址为%p\n",CityID,G[CityID].head);
    }
    G[CityID].memory->next = p;
    G[CityID].memory = G[CityID].memory->next;
    p->ID = TargetCityID;
    p->weight = weight;
    p->next = NULL;

}
// 获取数据并初始化链表
void getData(Graph* G){
    FILE *fp = NULL;
    int CityID,TargetCityID,weight;
    if((fp=fopen("/Users/kying-star/Documents/文档/大学/大二下/移动通信/实验课2-通信网/lab_2/data","r")) == NULL)
    {
        printf("文件data.txt打开失败,请检查路径\n");
        exit(-1);
    }
    while (fscanf(fp, "%d %d %d", &CityID,&TargetCityID,&weight) != EOF){
        //printf("%d==>%d, weight is %d\n", CityID,TargetCityID,weight);
        G[CityID-1].dist[TargetCityID-1] = weight;
        G[TargetCityID-1].dist[CityID-1] = weight;
        init(G,CityID-1,TargetCityID-1,weight);
        init(G,TargetCityID-1,CityID-1,weight);
    }
    fclose(fp);
}
// 展示链表
void ShowGraphDist(Graph *G){
    for (int i = 0; i < 8; ++i) {
        printf("\t\t City%d",i+1);
    }
    printf("\n");
    for (int i = 0; i < 8; ++i) {
        printf("City%d\t",i+1);
        for (int j = 0; j < 8; ++j) {
            if(G[i].dist[j] == infinite){
                printf("XXXXXX\t\t");
            }else{
                printf("%6d\t\t",G[i].dist[j]);
            }
        }
        printf("\n");
    }
}
void initDist(Graph *G){
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            G[i].dist[j] = infinite;
            G[i].sign[j] = 0;
            G[i].head = NULL;
            if (i == j){
                G[i].dist[j] = 0;
            }
        }
    }
}
void ShowGraph(Graph *G){
    for (int i = 0; i < 8; ++i) {
        link* temp = G[i].head->next;
        printf("节点%d--|\n ",i+1);
        while (temp){
            printf("\t  |--%d-->节点%d\n ",temp->weight,temp->ID+1);
            temp = temp->next;
        }
        printf("\n");
    }
    printf("\n");
}
int full(Graph *G,int CityId){
    int flag = 0;
    for (int i = 0; i < 8; ++i) {
        if(G[CityId].dist[i] != infinite){
            flag++;
        }
    }
    return flag;
}
int isOK(int targetCityID, int * S,int len){
    int flag = 1;
    for (int i = 0; i < len; ++i) {
        if (S[i] == targetCityID){
            flag = 0;
        }
    }
    return flag;
}
int stackIsOk(Graph* G,Stack* stack,int CityID){
    int len = 0;
    link* head = G[CityID].head->next;
    for (int i = 1; i < stack->top; ++i) {
        while (head){
            int num1 = head->ID;
            int num2 = stack->array[i];
            if( head->ID == stack->array[i] ){
                len+=head->weight;
                head = G[stack->array[i]].head;
                break;
            }else{
                head = head->next;
            }
        }
    }
    return len;
}
void djistra(Graph *G,int CityID){
    int ID[8],i = 0,count = 0,signCount = 0;
    int len = 0,flag[8];
    int nowCity = CityID;
    while(count<16){
        link* temp = G[nowCity].head->next;
        // 获取当前节点最短路径
        int min = infinite;
        while (temp){
            int city = nowCity + 1;
            int weight = temp->weight;
            int tempID = temp->ID + 1;
            int num = temp->weight + G[CityID].dist[nowCity];
            int num2 = G[CityID].dist[temp->ID];
            if(G[CityID].dist[nowCity] + temp->weight < G[CityID].dist[temp->ID]){
                G[CityID].dist[temp->ID] = G[CityID].dist[nowCity] + temp->weight;
                Graph testG = G[CityID];
            }
            Graph testG = G[CityID];
            if ( temp->weight < min && isOK(temp->ID,flag,count)) {
                min = temp->weight;
            }
            temp = temp->next;
        }
        // 全局比较
        link* mid = G[nowCity].head->next;
        while (mid){
            if (G[nowCity].dist[mid->ID] + G[CityID].dist[mid->ID] < G[CityID].dist[nowCity]) {
                G[CityID].dist[nowCity] = G[nowCity].dist[mid->ID] + G[CityID].dist[mid->ID];
            }
            mid = mid->next;
        }
        link* Ids = G[nowCity].head->next;
        // 获取当前距离当前节点最近的城市
        while (Ids){
            if(Ids->weight == min && isOK(Ids->ID,flag,count)){
                min = Ids->weight;
                ID[i] = Ids->ID;
                i++;
            }
            Ids = Ids->next;
        }
        if (min == infinite){
            flag[count] = nowCity;
            nowCity = flag[count-1];
            count++;
        }else{
            flag[count] = nowCity;
            count++;
            nowCity = ID[0];
        }

        i = 0;// 清空存储节点ID
    }
}
void dps(Graph* G,link* node,Stack* stack,int CityID,int targetCityID){
    if(stackIsOk(G,stack,CityID) == G[CityID].dist[targetCityID] && stack->array[0] == CityID){
        for (int i = 0; i < stack->top; ++i) {
            printf("%d-->",stack->array[i] + 1);
        }
        printf("路径长度%d",G[CityID].dist[targetCityID]);
        printf("\n");
    }
    link* head = G[node->ID].head->next;
    while (head){
        if(isOK(head->ID,stack->array,stack->top)){
            push(stack,head->ID);
            dps(G,head,stack,CityID,targetCityID);
        }
        head = head->next;
    }
    pop(stack);
}
void findPath(Graph *G,int CityID,int targetCityID,Stack* stack){
    stack->top = 0;
    link * nowNode = G[CityID].head->next;
    push(stack,CityID);
    while (nowNode){
        push(stack,nowNode->ID);
        dps(G,nowNode,stack,CityID,targetCityID);
        pop(stack);
        nowNode = nowNode->next;
    }
}

void main(){
    Graph G[8];
    Stack stack;
    initDist(G);
    getData(G);
    //ShowGraphDist(G);
    // ShowGraph(G);
    for (int i = 0; i < 8; ++i) {
        djistra(G,i);
    }
    ShowGraphDist(G);
//    for (int i = 0; i < 8; ++i) {
//        for (int j = 0; j < 8; ++j) {
//            if(i!=j){
//                findPath(G,i,j,&stack);
//                printf("\n");
//            }
//        }
//    }
    for (int i = 0; i < 8; ++i) {
        findPath(G,2,i,&stack);
        printf("\n");
    }
}
