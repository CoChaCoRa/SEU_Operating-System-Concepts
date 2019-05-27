#include"LRU.h"
#include<stdio.h>
#include<fstream>
using namespace std;

int main(){
    vector<RSItem> refStr;
    const char* filename = "latex";
    FILE * file;
    ofstream csv_file;

    file = fopen (filename,"r");
    if(!file){
        cerr<<"Cannot open file!"<<endl;
        return -1;
    }

    csv_file.open("latex_statistics.csv",ios::out);
    if(!csv_file){
        cerr<<"Cannot open file!"<<endl;
        return -1;
    }
    csv_file<<"最大内存帧"<<','<<"LRU_计数器"<<','<<"LRU_栈"<<','<<"额外引用位算法"<<','<<"第二次机会算法"<<endl;

    cout<<"Reading reference string..."<<endl;
    cout<<"Length of refStr is 3000"<<endl;
    unsigned int addr;
    char mode;
    
    for(int i=0;i<300000;i++){
        fscanf(file, "%x %c", &addr, &mode);
        //cout<<"addr:"<<addr<<",mode:"<<mode<<endl;
        RSItem newitem(addr,mode);
        refStr.push_back(newitem);
    }

    int num_Frames[]={5,30,50,100,300,500,1000,3000,5000,10000,50000,100000};
    for(int i=0;i<12;++i){
        cout<<"\nProgress: "<<100*(i*1.0/12)<<"%..."<<endl;
        cout<<"Constructing Instance of LRU..."<<endl;
        LRU lru(num_Frames[i],refStr,20);
        cout<<"Testing LRU_Counter..."<<endl;
        float pageFault_c = (float)lru.LRU_Counter()/300000.0;
        cout<<"Testing LRU_Stack..."<<endl;
        float pageFault_s = (float)lru.LRU_Stack()/300000.0;
        cout<<"Testing Additional_RB..."<<endl;
        float pageFault_arb = (float)lru.AdditionalRB()/300000.0;
        cout<<"Testing SecondChance..."<<endl;
        float pageFault_sc = (float)lru.SecondChance()/300000.0;

        csv_file<<num_Frames[i]<<','<<pageFault_c<<','<<pageFault_s<<','<<pageFault_arb<<','<<pageFault_sc<<endl;
    }

    fclose(file);
    csv_file.close();
    return 0;
}