#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
using namespace std;
int pageNum, pageFrameNum, windowSize, referLen;
int referString[1000];
int minFrame[20];
int fifoFrame[20];
queue<int> q;
void hanleInput(){
    FILE *fp;
    fp = fopen("input.txt","r");
    fscanf(fp,"%d %d %d %d",&pageNum,&pageFrameNum,&windowSize,&referLen);
    for(int i=0;i<referLen;i++){
        fscanf(fp,"%d ",&referString[i]);
    }
}
void printMIN(int index, bool fault){
    // 메모리 상태 변화 과정 //
    cout << "Time : " << index + 1 << "  Ref. string : " << referString[index] << '\n';
    cout << "Memory State" << '\n';
    for(int i=0;i<pageFrameNum;i++){
        cout << minFrame[i] << ' ' ;
    }
    if(fault) cout << "\nPAGE FAULT";
    cout << "\n----------------------------------\n";

}
bool cmp(const pair<int, int> &a, const pair<int, int> &b)
{
    return a.second < b.second;
}

void handleMIN(int pageFaultNum, int index){
    if(pageFaultNum <= pageFrameNum){
        minFrame[pageFaultNum - 1] = referString[index];
        q.push(index);
        printMIN(index,true);
    }else{
        int replacedIndex = -1;
        // vector<pair<int,int>> referTime;
        int max = -1;
        int tieBreak = -1;
        int tieBreakIndex = -1;
        // printf("%d\n",index);
        for(int i=0;i<pageFrameNum;i++){
            bool refered = false;
            for(int j=index+1;j<referLen;j++){
                if(minFrame[i] == referString[j]){
                    if(max < j){
                        replacedIndex = i;
                        max = j;
                    } 
                    refered = true;
                    // referTime.push_back(make_pair(i,j));
                    break;
                }
            }
            if(!refered){
                if(tieBreak < minFrame[i]){
                    tieBreak = minFrame[i];
                    tieBreakIndex = i;
                }
                // referTime.push_back(make_pair(i,referLen));
            }
        }
        // sort(referTime.begin(),referTime.end(),cmp);
        // replacedIndex = referTime.back().first;
        
        // cout << "replaced : " << replacedIndex <<"\n";
        if(tieBreakIndex == -1) minFrame[replacedIndex] = referString[index];
        else minFrame[tieBreakIndex] = referString[index];
        printMIN(index,true);
    }
}
void MIN(){
    for(int i=0;i<pageFrameNum;i++){
        minFrame[i] = -1;
    }
    int pageFaultNum = 0;
    for(int i=0;i<referLen;i++){
        bool pageFault = true;
        for(int j=0;j<pageFrameNum;j++){
            if(referString[i] == minFrame[j]){
                pageFault = false;
                printMIN(i,false);
                break;
            }
        }
        if(pageFault){
            pageFaultNum++;
            handleMIN(pageFaultNum, i);
        }
    }
    cout << "Total Page Fault Count : " << pageFaultNum << '\n';
} 
void handleFIFO(int pageFaultNum, int index){
    if(pageFaultNum <= pageFrameNum){
        fifoFrame[pageFaultNum - 1] = referString[index];
        printFIFO(index,true);
    }else{
        int replacedIndex = -1;
        // vector<pair<int,int>> referTime;
        int max = -1;
        int tieBreak = -1;
        int tieBreakIndex = -1;
        // printf("%d\n",index);
        for(int i=0;i<pageFrameNum;i++){
            bool refered = false;
            for(int j=index+1;j<referLen;j++){
                if(minFrame[i] == referString[j]){
                    if(max < j){
                        replacedIndex = i;
                        max = j;
                    } 
                    refered = true;
                    // referTime.push_back(make_pair(i,j));
                    break;
                }
            }
            if(!refered){
                if(tieBreak < minFrame[i]){
                    tieBreak = minFrame[i];
                    tieBreakIndex = i;
                }
                // referTime.push_back(make_pair(i,referLen));
            }
        }
        // sort(referTime.begin(),referTime.end(),cmp);
        // replacedIndex = referTime.back().first;
        
        // cout << "replaced : " << replacedIndex <<"\n";
        if(tieBreakIndex == -1) minFrame[replacedIndex] = referString[index];
        else minFrame[tieBreakIndex] = referString[index];
        printMIN(index,true);
    }
}
void FIFO(){
    for(int i=0;i<pageFrameNum;i++){
        fifoFrame[i] = -1;
    }
    int pageFaultNum = 0;
    for(int i=0;i<referLen;i++){
        bool pageFault = true;
        for(int j=0;j<pageFrameNum;j++){
            if(referString[i] == fifoFrame[j]){
                pageFault = false;
                printFIFO(i,false);
                break;
            }
        }
        if(pageFault){
            pageFaultNum++;
            handleFIFO(pageFaultNum, i);
        }
    }
    cout << "Total Page Fault Count : " << pageFaultNum << '\n';
} 
int main(){
    hanleInput();
    MIN();
    FIFO();
    return 0;
}