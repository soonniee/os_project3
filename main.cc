#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
using namespace std;
int pageNum, pageFrameNum, windowSize, referLen;
int referString[1000];
int minFrame[20];
int fifoFrame[20];
int lruFrame[20];
int lfuFrame[20];
int lfuCount[1000]={0};
vector<int> wsPage;
queue<int> q;
void hanleInput(){
    FILE *fp;
    fp = fopen("input1.txt","r");
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
        printMIN(index,true);
    }else{
        int replacedIndex = -1;
        // vector<pair<int,int>> referTime;
        int max = -1;
        int tieBreak = pageNum;
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
                if(tieBreak > minFrame[i]){
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
void printFIFO(int index, bool fault){
    // 메모리 상태 변화 과정 //
    
    cout << "Time : " << index + 1 << "  Ref. string : " << referString[index] << '\n';
    cout << "Memory State" << '\n';
    for(int i=0;i<pageFrameNum;i++){
        cout << fifoFrame[i] << ' ' ;
    }
    if(fault) cout << "\nPAGE FAULT";
    cout << "\n----------------------------------\n";

}
void handleFIFO(int pageFaultNum, int index){
    if(pageFaultNum <= pageFrameNum){
        fifoFrame[pageFaultNum - 1] = referString[index];
        // q.push(index);
        printFIFO(index,true);
    }else{
        int replacedIndex = -1;
        replacedIndex = (pageFaultNum-1) % pageFrameNum;
        // for(int i=0;i<pageFrameNum;i++){
        //     if(fifoFrame[i] == q.pop()){
        //         replacedIndex = i;
        //         break;
        //     }
        // }
        fifoFrame[replacedIndex] = referString[index];
        // q.push(index);
        printFIFO(index,true);
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
void printLRU(int index, bool fault){
    // 메모리 상태 변화 과정 //
    
    cout << "Time : " << index + 1 << "  Ref. string : " << referString[index] << '\n';
    cout << "Memory State" << '\n';
    for(int i=0;i<pageFrameNum;i++){
        cout << lruFrame[i] << ' ' ;
    }
    if(fault) cout << "\nPAGE FAULT";
    cout << "\n----------------------------------\n";

}
void handleLRU(int pageFaultNum, int index){
    if(pageFaultNum <= pageFrameNum){
        lruFrame[pageFaultNum - 1] = referString[index];
        // q.push(index);
        printLRU(index,true);
    }else{
        
        int replacedIndex = -1;
        // vector<pair<int,int>> referTime;
        int min = referLen;
        int tieBreak = pageNum;
        int tieBreakIndex = -1;
        // printf("%d\n",index);
        for(int i=0;i<pageFrameNum;i++){
            bool refered = false;
            for(int j=index-1;j>=0;j--){
                if(lruFrame[i] == referString[j]){
                    if(min > j){
                        replacedIndex = i;
                        min = j;
                    } 
                    refered = true;
                    // referTime.push_back(make_pair(i,j));
                    break;
                }
            }
            if(!refered){
                if(tieBreak > minFrame[i]){
                    tieBreak = minFrame[i];
                    tieBreakIndex = i;
                }
                // referTime.push_back(make_pair(i,referLen));
            }
        }
        // sort(referTime.begin(),referTime.end(),cmp);
        // replacedIndex = referTime.back().first;
        
        // cout << "replaced : " << replacedIndex <<"\n";
        if(tieBreakIndex == -1) lruFrame[replacedIndex] = referString[index];
        else lruFrame[tieBreakIndex] = referString[index];
        printLRU(index,true);

    }
}
void LRU(){
    for(int i=0;i<pageFrameNum;i++){
        lfuFrame[i] = -1;
    }
    int pageFaultNum = 0;
    for(int i=0;i<referLen;i++){
        bool pageFault = true;
        for(int j=0;j<pageFrameNum;j++){
            if(referString[i] == lfuFrame[j]){
                pageFault = false;
                printLRU(i,false);
                break;
            }
        }
        if(pageFault){
            pageFaultNum++;
            handleLRU(pageFaultNum, i);
        }
    }
    cout << "Total Page Fault Count : " << pageFaultNum << '\n';
} 
void printLFU(int index, bool fault){
    // 메모리 상태 변화 과정 //
    
    cout << "Time : " << index + 1 << "  Ref. string : " << referString[index] << '\n';
    cout << "Memory State" << '\n';
    for(int i=0;i<pageFrameNum;i++){
        cout << lfuFrame[i] << ' ' ;
    }
    if(fault) cout << "\nPAGE FAULT";
    cout << "\n----------------------------------\n";

}
void handleLFU(int pageFaultNum, int index){
    if(pageFaultNum <= pageFrameNum){
        lfuFrame[pageFaultNum - 1] = referString[index];
        printLFU(index,true);
    }else{
        
        int replacedIndex = -1;
        int min = referLen;
        int max = referLen;
        int tieBreak = pageNum;
        int tieBreakIndex = -1;
        // printf("%d\n",index);
        for(int i=0;i<pageFrameNum;i++){
            int frequency = lfuCount[lfuFrame[i]];
            if(max < frequency) continue;
            else if(max==frequency){
                
                bool refered = false;
                for(int j=index-1;j>=0;j--){
                    if(lfuFrame[i] == referString[j]){
                        if(min > j){
                            replacedIndex = i;
                            min = j;
                        } 
                        refered = true;
                        break;
                    }
                }
                if(!refered){
                    if(tieBreak > lfuFrame[i]){
                        tieBreak = lfuFrame[i];
                        tieBreakIndex = i;
                    }
                    
                }
            }
            else{
                replacedIndex = i;
                max = frequency;
                for(int j=index-1;j>=0;j--){
                    if(lfuFrame[i] == referString[j]){
                        if(min > j){
                            replacedIndex = i;
                            min = j;
                        } 
                        break;
                    }
                }
            }
        }
        if(tieBreakIndex == -1) lfuFrame[replacedIndex] = referString[index];
        else lfuFrame[tieBreakIndex] = referString[index];
        printLFU(index,true);

    }
}
void LFU(){
    for(int i=0;i<pageFrameNum;i++){
        lfuFrame[i] = -1;
    }
    int pageFaultNum = 0;
    for(int i=0;i<referLen;i++){
        bool pageFault = true;
        lfuCount[referString[i]] += 1;
        for(int j=0;j<pageFrameNum;j++){
            if(referString[i] == lfuFrame[j]){
                pageFault = false;
                printLFU(i,false);
                break;
            }
        }
        if(pageFault){
            pageFaultNum++;
            handleLFU(pageFaultNum, i);
        }
    }
    cout << "Total Page Fault Count : " << pageFaultNum << '\n';
} 
void printWS(int index, bool fault){
    // 메모리 상태 변화 과정 //
    
    cout << "Time : " << index + 1 << "  Ref. string : " << referString[index] << '\n';
    cout << "Memory State" << '\n';
    for(int i=0;i<wsPage.size();i++){
        cout << wsPage[i] << ' ' ;
    }
    if(fault) cout << "\nPAGE FAULT";
    cout << "\n----------------------------------\n";

}
void handleWS(int index, bool fault){
    int refered = referString[index];
    vector<int> popArr;
    
    if(index < windowSize){
        for(int i=0;i<wsPage.size();i++){
             bool pop = true;
            for(int j=index-1;j>=0;j--){
                if(wsPage[i] == referString[j] || refered == wsPage[i]){
                    pop = false;
                    break;
                }
            }
            if(pop) popArr.push_back(i);
        }
        
    }else{
        printf("%d\n",index);
        for(int i=0;i<wsPage.size();i++){
            bool pop = true;
            
            for(int j=index-1;j>=index-windowSize;j--){
                if(wsPage[i] == referString[j]|| refered == wsPage[i]){
                    
                    pop = false;
                    break;
                }
            }
            if(pop) popArr.push_back(i);
        }
    }
   
    for(int i=0;i<popArr.size();i++){
        
        wsPage.erase(wsPage.begin() + popArr[i]);
    }
    if(fault) wsPage.push_back(refered);
    printWS(index,fault);
    
}
void WS(){
    // for(int i=0;i<pageNum;i++){
    //     wsPage[i] = -1;
    // }
    int pageFaultNum = 0;
    for(int i=0;i<referLen;i++){
        bool pageFault = true;
        for(int j=0;j<wsPage.size();j++){
            if(referString[i] == wsPage[j]){
                pageFault = false;
                handleWS(i,false);
                break;
            }
        }
        if(pageFault){
            pageFaultNum++;
            handleWS(i,true);
        }
    }
    cout << "Total Page Fault Count : " << pageFaultNum << '\n';
} 
int main(){
    hanleInput();
    // cout << "MIN\n\n";
    // MIN();
    // cout << "\nFIFO\n\n";
    // FIFO();
    // cout << "\nLRU\n\n";
    // LRU();
    // cout << "\nLFU\n\n";
    // LFU();
    cout << "\nWS\n\n";
    WS();
    return 0;
}