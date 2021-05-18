#include<iostream>
#include<vector>
#include<algorithm>
#include<iomanip>
using namespace std;
int pageNum, pageFrameNum, windowSize, referLen;
int *referString;
int *minFrame;
int *fifoFrame;
int *lruFrame;
int *lfuFrame;
int *lfuCount;
vector<int> wsPage;
// pageNum : process가 갖는 page 수
// pageFrameNum : 할당 page frame 수
// windowSize : window size
// referLen : page reference string 길이
// referString : page reference string 저장된 배열
// minFrame: MIN 알고리즘을 위한 page frame 배열
// fifoFrame : FIFO 알고리즘을 위한 page frame 배열
// lruFrame : LRU 알고리즘을 위한 page frame 배열
// lfuFrame : LFU 알고리즘을 위한 page frame 배열
// lfuCount : Page reference count 저장될 배열
// wsPage : WS 알고리즘 Memory State 보여줄 벡터
void hanleInput(){
    FILE *fp;
    fp = fopen("input.txt","r");
    
    fscanf(fp,"%d %d %d %d",&pageNum,&pageFrameNum,&windowSize,&referLen);
    referString = (int*)malloc(sizeof(int)*referLen);
    minFrame = (int*)malloc(sizeof(int)*pageFrameNum);
    fifoFrame = (int*)malloc(sizeof(int)*pageFrameNum);
    lruFrame = (int*)malloc(sizeof(int)*pageFrameNum);
    lfuFrame = (int*)malloc(sizeof(int)*pageFrameNum);
    lfuCount = (int*)malloc(sizeof(int)*pageNum);
    for(int i=0;i<referLen;i++){
        fscanf(fp,"%d ",&referString[i]);
    }
    fclose(fp);
}
void printMIN(int index, bool fault){
    // 메모리 상태 변화 과정 //
    if(fault) cout << "| " << setw(3) << index + 1 << "  | " << setw(5)<< referString[index] << " | " << " F     | " ;
    else cout << "| " <<setw(3) <<  index + 1 << "  | " << setw(5)<< referString[index] << " | " << "       | ";
    for(int i=0;i<pageFrameNum;i++){
        if(minFrame[i]!=-1) cout << minFrame[i] << ' ' ;
    }
    cout << '\n';
}

void handleMIN(int pageFaultNum, int index){
    // page fault 발생하였지만 page frame 내 empty 존재하는 경우.
    if(pageFaultNum <= pageFrameNum){
        minFrame[pageFaultNum - 1] = referString[index];
        printMIN(index,true);
    }else{
        // page fault 발생하였고 page frame 내 empty 존재하지 않는 경우.
        int replacedIndex = -1;
        int max = -1;
        int tieBreak = pageNum;
        int tieBreakIndex = -1;
        
        for(int i=0;i<pageFrameNum;i++){
            bool refered = false;
            for(int j=index+1;j<referLen;j++){
                if(minFrame[i] == referString[j]){
                    if(max < j){
                        replacedIndex = i;
                        max = j;
                    } 
                    refered = true;
                    
                    break;
                }
            }
            if(!refered){
                // 미래에 refered 되지 않는 경우 따로 처리.
                // 두 개 이상의 page 존재 시 tie break rule 적용.
                if(tieBreak > minFrame[i]){
                    tieBreak = minFrame[i];
                    tieBreakIndex = i;
                }
                
            }
        }
        
        if(tieBreakIndex == -1) minFrame[replacedIndex] = referString[index];
        else minFrame[tieBreakIndex] = referString[index];
        printMIN(index,true);
    }
}
void MIN(){
    //minFrame 배열 -1로 초기화. -> -1은 empty 함을 뜻함.
    for(int i=0;i<pageFrameNum;i++){
        minFrame[i] = -1;
    }
    int pageFaultNum = 0;
    for(int i=0;i<referLen;i++){
        bool pageFault = true;
        for(int j=0;j<pageFrameNum;j++){
            //Page fault 아닐 경우 -> printMin 함수 호출.
            if(referString[i] == minFrame[j]){
                pageFault = false;
                printMIN(i,false); // Memory State 출력 함수.
                break;
            }
        }
        //Page fault 발생할 경우 -> handleMIN 함수 호출.
        if(pageFault){
            pageFaultNum++;
            handleMIN(pageFaultNum, i); // MIN 알고리즘 수행 함수.
        }
    }
    // MIN 알고리즘 수행 후 총 Page fault 갯수 출력.
    cout << "Total Page Fault Count : " << pageFaultNum << '\n' << '\n';
} 
void printFIFO(int index, bool fault){
    // 메모리 상태 변화 과정 //
    if(fault) cout << "| " << setw(3) << index + 1 << "  | " << setw(5)<< referString[index] << " | " << " F     | " ;
    else cout << "| " <<setw(3) <<  index + 1 << "  | " << setw(5)<< referString[index] << " | " << "       | ";
    for(int i=0;i<pageFrameNum;i++){
        if(fifoFrame[i]!=-1) cout << fifoFrame[i] << ' ' ;
    }
    cout << '\n';
}
void handleFIFO(int pageFaultNum, int index){
    // page fault 발생하였지만 page frame 내 empty 존재하는 경우.
    if(pageFaultNum <= pageFrameNum){
        fifoFrame[pageFaultNum - 1] = referString[index];
        printFIFO(index,true);
    }else{
        // page fault 발생하였고 page frame 내 empty 존재하지 않는 경우.
        int replacedIndex = -1;
        replacedIndex = (pageFaultNum-1) % pageFrameNum;
        
        fifoFrame[replacedIndex] = referString[index];
        
        printFIFO(index,true);
    }
}
void FIFO(){
    //fifoFrame 배열 -1로 초기화. -> -1은 empty 함을 뜻함.
    for(int i=0;i<pageFrameNum;i++){
        fifoFrame[i] = -1;
    }
    int pageFaultNum = 0;
    for(int i=0;i<referLen;i++){
        bool pageFault = true;
        for(int j=0;j<pageFrameNum;j++){
            //Page fault 아닐 경우 -> printFIFO 함수 호출.
            if(referString[i] == fifoFrame[j]){
                pageFault = false;
                printFIFO(i,false); // Memory State 출력 함수.
                break;
            }
        }
        //Page fault 발생할 경우 -> handleFIFO 함수 호출.
        if(pageFault){
            pageFaultNum++;
            handleFIFO(pageFaultNum, i); // FIFO 알고리즘 수행 함수.
        }
    }
    cout << "Total Page Fault Count : " << pageFaultNum << '\n' << '\n';
} 
void printLRU(int index, bool fault){

    // 메모리 상태 변화 과정 //
    if(fault) cout << "| " << setw(3) << index + 1 << "  | " << setw(5)<< referString[index] << " | " << " F     | " ;
    else cout << "| " <<setw(3) <<  index + 1 << "  | " << setw(5)<< referString[index] << " | " << "       | ";
    for(int i=0;i<pageFrameNum;i++){
        if(lruFrame[i]!=-1) cout << lruFrame[i] << ' ' ;
    }
    cout << '\n';
    

}
void handleLRU(int pageFaultNum, int index){
    // page fault 발생하였지만 page frame 내 empty 존재하는 경우.
    if(pageFaultNum <= pageFrameNum){
        lruFrame[pageFaultNum - 1] = referString[index];
        // q.push(index);
        printLRU(index,true);
    }else{
        // page fault 발생하였고 page frame 내 empty 존재하지 않는 경우.
        int replacedIndex = -1;
        int min = referLen;
        int tieBreak = pageNum;
        int tieBreakIndex = -1;
        for(int i=0;i<pageFrameNum;i++){
            bool refered = false;
            for(int j=index-1;j>=0;j--){
                if(lruFrame[i] == referString[j]){
                    if(min > j){
                        replacedIndex = i;
                        min = j;
                    } 
                    refered = true;
                    
                    break;
                }
            }
            if(!refered){
                // 이전에 refered 되지 않은 경우 따로 처리.
                // 두 개 이상의 page 존재 시 tie break rule 적용.
                if(tieBreak > minFrame[i]){
                    tieBreak = minFrame[i];
                    tieBreakIndex = i;
                }
                
            }
        }
        
        if(tieBreakIndex == -1) lruFrame[replacedIndex] = referString[index];
        else lruFrame[tieBreakIndex] = referString[index];
        printLRU(index,true);

    }
}
void LRU(){
    //lruFrame 배열 -1로 초기화. -> -1은 empty 함을 뜻함.
    for(int i=0;i<pageFrameNum;i++){
        lruFrame[i] = -1;
    }
    int pageFaultNum = 0;
    for(int i=0;i<referLen;i++){
        bool pageFault = true;
        for(int j=0;j<pageFrameNum;j++){
            //Page fault 아닐 경우 -> printLRU 함수 호출.
            if(referString[i] == lruFrame[j]){
                pageFault = false;
                printLRU(i,false);
                break;
            }
        }
        //Page fault 발생할 경우 -> handleLRU 함수 호출.
        if(pageFault){
            pageFaultNum++;
            handleLRU(pageFaultNum, i); // LRU 알고리즘 수행 함수.
        }
    }
    cout << "Total Page Fault Count : " << pageFaultNum << '\n' << '\n';
} 
void printLFU(int index, bool fault){
    // 메모리 상태 변화 과정 //
    if(fault) cout << "| " << setw(3) << index + 1 << "  | " << setw(5)<< referString[index] << " | " << " F     | " ;
    else cout << "| " <<setw(3) <<  index + 1 << "  | " << setw(5)<< referString[index] << " | " << "       | ";
    for(int i=0;i<pageFrameNum;i++){
        if(lfuFrame[i]!=-1) cout << lfuFrame[i] << ' ' ;
    }
    cout << '\n';

}
void handleLFU(int pageFaultNum, int index){
    // page fault 발생하였지만 page frame 내 empty 존재하는 경우.
    if(pageFaultNum <= pageFrameNum){
        lfuFrame[pageFaultNum - 1] = referString[index];
        printLFU(index,true);
    }else{
        // page fault 발생하였고 page frame 내 empty 존재하지 않는 경우.
        int replacedIndex = -1;
        int min = referLen;
        int max = referLen;
        int tieBreak = pageNum;
        int tieBreakIndex = -1;
        for(int i=0;i<pageFrameNum;i++){
            int frequency = lfuCount[lfuFrame[i]];
            if(max < frequency) continue;
            else if(max==frequency){
                // page reference count 수 가장 적은 page가 여러 개 존재 -> Tie-breaking rule 적용
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
    //lfuFrame 배열 -1로 초기화. -> -1은 empty 함을 뜻함.
    for(int i=0;i<pageFrameNum;i++){
        lfuFrame[i] = -1;
    }
     //lfuCount 배열 0로 초기화.
    for(int i=0;i<pageNum;i++){
        lfuCount[i] = 0;
    }
    int pageFaultNum = 0;
    for(int i=0;i<referLen;i++){
        bool pageFault = true;
        lfuCount[referString[i]] += 1;
        for(int j=0;j<pageFrameNum;j++){
            //Page fault 아닐 경우 -> printLFU 함수 호출.
            if(referString[i] == lfuFrame[j]){
                pageFault = false;
                printLFU(i,false);
                break;
            }
        }
        //Page fault 발생할 경우 -> handleLFU 함수 호출.
        if(pageFault){
            pageFaultNum++;
            handleLFU(pageFaultNum, i); // LFU 알고리즘 수행 함수.
        }
    }
    cout << "Total Page Fault Count : " << pageFaultNum << '\n' << '\n';
} 
void printWS(int index, bool fault){
    // 메모리 상태 변화 과정 //
    if(fault) cout << "| " << setw(3) << index + 1 << "  | " << setw(5)<< referString[index] << " | " << " F     | " ;
    else cout << "| " <<setw(3) <<  index + 1 << "  | " << setw(5)<< referString[index] << " | " << "       | ";
    for(int i=0;i<wsPage.size();i++){
        cout << wsPage[i] << ' ' ;
    }
    cout <<'\n';   
}
void handleWS(int index, bool fault){
    int refered = referString[index];
    vector<int> popArr; //replace될 page 저장할 벡터.
    if(index < windowSize){
        // wsPage : WS 알고리즘 Memory State 저장할 벡터
        for(int i=0;i<wsPage.size();i++){
             bool pop = true;
            for(int j=index-1;j>=0;j--){
                if(wsPage[i] == referString[j] || refered == wsPage[i]){
                    pop = false;
                    break;
                }
            }
            if(pop) popArr.push_back(i); //replace될 page를 popArr에 push.
        }
        
    }else{
        for(int i=0;i<wsPage.size();i++){
            bool pop = true;
            for(int j=index-1;j>=index-windowSize;j--){
                if(wsPage[i] == referString[j]|| refered == wsPage[i]){
                    pop = false;
                    break;
                }
            }
            if(pop) popArr.push_back(i); //replace될 page push
        }
    }
    for(int i=0;i<popArr.size();i++){
        wsPage.erase(wsPage.begin() + popArr[i]); //replace될 page를 wsPage에서 삭제.
    }
    if(fault) wsPage.push_back(refered); //page fault 발생 -> 해당 page를 wsPage에 push.
    printWS(index,fault);
}
void WS(){
    int pageFaultNum = 0;
    for(int i=0;i<referLen;i++){
        bool pageFault = true;
        for(int j=0;j<wsPage.size();j++){
            //Page fault 아닐 경우 -> handleWS 함수 호출.
            if(referString[i] == wsPage[j]){
                pageFault = false;
                handleWS(i,false);
                break;
            }
        }
        //Page fault 경우 -> handleWS 함수 호출.
        if(pageFault){
            pageFaultNum++;
            handleWS(i,true);
        }
    }
    cout << "Total Page Fault Count : " << pageFaultNum << '\n' << '\n';
}
void printTop(){
    cout << "+------+-------+--------+-------------\n";
    cout << "| Time | Ref.S | Page.F | Memory State\n";
    cout << "+------+-------+--------+-------------\n";
} 
int main(){
    hanleInput();
    cout << "MIN\n";
    printTop();
    MIN();
    cout << "\nFIFO\n";
    printTop();
    FIFO();
    cout << "\nLRU\n";
    printTop();
    LRU();
    cout << "\nLFU\n";
    printTop();
    LFU();
    cout << "\nWS\n";
    printTop();
    WS();
    free(referString);
    free(minFrame);
    free(fifoFrame);
    free(lruFrame);
    free(lfuFrame);
    free(lfuCount);
    return 0;
}