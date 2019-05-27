#include<vector>
#include<iostream>
using namespace std;

//引用串item
struct RSItem{
    unsigned int pageid;
    char mode;
    RSItem(){
        pageid = -1;
        mode = NULL;
    }
    RSItem(int id,char c){
        pageid = id;
        mode = c;
    }
};

//counter实现的页表项
struct TabItem_Counter{
    RSItem Item;
    //int pageid;
    int counter;
    TabItem_Counter(RSItem it,int c){
        Item = it;
        counter = c;
    }
};

//stack实现的页表项
struct TabItem_Stack{
    RSItem Item;
    //int pageid;
    TabItem_Stack *front,*next;
    TabItem_Stack(RSItem it){
        Item = it;
        front = NULL,next = NULL;
    }
};

//stack实现的双向链表
struct PageTab_Stack{
    TabItem_Stack *phead,*ptail;
    int maxSize;
    int size;

    PageTab_Stack(int num_Frames){
        maxSize = num_Frames;
        size = 0;
        phead = NULL,ptail = NULL;
    }

    //函数：将新页表项置于栈顶
    bool push(TabItem_Stack &newitem){
        if(size>=maxSize) return false;

        newitem.next = phead;

        if(phead != NULL){
            phead->front = &newitem;
        }
        if(ptail == NULL){
            ptail = &newitem;
        }
        phead = &newitem;
        //phead->front = NULL;
        size++;
        return true;
    }
    //函数：将栈内页表项置于栈顶
    bool move_front(int id){
        bool find = false;
        TabItem_Stack *ItemtoMove = NULL;
        for(TabItem_Stack *iter = phead;iter!=NULL;iter=iter->next){
            if(iter->Item.pageid==id){
                find = true;
                ItemtoMove = iter;
            } 
        }
        if(find==false) return find;

        //如果已经在栈顶则不需要移动
        if(ItemtoMove==phead){
            return true;
        }

        //最坏情况下需要修改6个指针
        if(ItemtoMove->front != NULL){
            ItemtoMove->front->next = ItemtoMove->next;
        }
        if(ItemtoMove->next != NULL){
            ItemtoMove->next->front = ItemtoMove->front;
        }
        else if(ItemtoMove->next==NULL){
            ptail = ItemtoMove->front;
        }
        phead->front = ItemtoMove;
        ItemtoMove->front = NULL;
        ItemtoMove->next = phead;
        phead = ItemtoMove;

        return true;
    }
    //函数：删除栈底元素并将新页表项置于栈顶
    bool replace_end(TabItem_Stack &item){

        ptail = ptail->front;
        if(ptail!=NULL){
            ptail->next = NULL;
        }
        size--;

        if(push(item)) return true;
        else return false;
    }
};

//额外引用位算法、二次机会算法页表项
struct TabItem_RB{
    RSItem Item;
    //int pageid;
    bool refBit;
    unsigned char hisbits;//8位历史位

    TabItem_RB(RSItem it,bool c){
        Item = it;
        refBit = c;
        hisbits = 0;
    }
};

class LRU{
public:
    LRU(int num_memFrames, vector<RSItem> RefStr, int time = 10);

    int LRU_Counter();//LRU计数器实现
    int LRU_Stack();//LRU栈实现
    int AdditionalRB();//额外引用位实现
    int SecondChance();//二次机会实现
private:
    int maxNum_Frames;//最大内存帧数
    vector<RSItem> refStr;//引用串
    int len_RS;//引用串长度，即测试页面数
    int timeUnit;//ARB算法的时间单元
};

LRU::LRU(int num_memFrames, vector<RSItem> RefStr, int time){
    maxNum_Frames = num_memFrames;
    refStr = RefStr;
    len_RS = RefStr.size();
    timeUnit = time;
}

int LRU::LRU_Counter(){
    RSItem empty_item;
    vector<TabItem_Counter>pageTab(maxNum_Frames,TabItem_Counter(empty_item,65535));//页表
    int pageFault = 0;//缺页错误数
    //int currentCnt = 0;//当前计数器大小

    for(const auto RSit: refStr){
        bool hit = false;
        int it_idx = 0;
        int toReplaceIndex = -1;
        int maxCnt = numeric_limits<int>::min();
        //搜索页表
        for(auto it: pageTab){
            //如果已经在内存中，则设置该帧counter为0后结束
            if(it.Item.pageid==RSit.pageid){
                pageTab[it_idx].counter = 0;
                hit = true;
                break;
            }
            //否则找出最大计数值对应index
            else if(it.counter > maxCnt){
                toReplaceIndex = it_idx;
				maxCnt = it.counter;
            }
            it_idx++;
        }
        //如果miss则替换page
        if(hit == false){
            ++pageFault;
            // cout<<"Page "<<pageTab[toReplaceIndex].Item.pageid<<" swapped, ";
            // cout<<"replaced by "<<RSit.pageid<<endl;
            pageTab[toReplaceIndex] = TabItem_Counter(RSit,0);
        }
        for(int i = 0;i<maxNum_Frames;i++){
            pageTab[i].counter++;
        }
    }

    return pageFault;
}


int LRU::LRU_Stack(){
    int pageFault = 0;//缺页错误数
    PageTab_Stack pageTab(maxNum_Frames);//页表

    for(const auto &RSit: refStr){
        bool hit = false;

        for(TabItem_Stack *pItem = pageTab.phead;pItem != NULL;pItem = pItem->next){
            if(RSit.pageid==pItem->Item.pageid){
                hit = true;
                pageTab.move_front(RSit.pageid);
            }
        }

        if(hit == false){
            TabItem_Stack *it = new TabItem_Stack(RSit);
            if(pageTab.size<pageTab.maxSize){
                // cout<<"Page swapped, ";
                // cout<<"replaced by "<<RSit.pageid<<endl;
                pageFault++;
                pageTab.push(*it);
            }
            else{
                pageFault++;
                // cout<<"Page "<<pageTab.ptail->Item.pageid<<" swapped, ";
                // cout<<"replaced by "<<RSit.pageid<<endl;
                pageTab.replace_end(*it);
            }
        }
    }

    return pageFault;
}


int LRU::AdditionalRB(){
    RSItem empty_item;
    vector<TabItem_RB>pageTab(maxNum_Frames,TabItem_RB(empty_item,false));//页表
    int pageFault = 0;//缺页错误数
    int timeCnt = 0;//通过计数器模拟时钟中断
    int nextpos = 0;//用循环队列实现FIFO

    for(const auto RSit: refStr){
        bool hit = false;
        int it_idx = 0;
        int toReplaceIndex = -1;
        int minhis = numeric_limits<int>::max();

        //每个时间单元更新历史位
        if(timeCnt==timeUnit){

            timeCnt = 0;
			for(vector<TabItem_RB>::iterator it = pageTab.begin(); it != pageTab.end();++it){
				it->hisbits = it->hisbits >> 1;
				it->hisbits = it->refBit == true ? it->hisbits|0x80 : it->hisbits;
				it->refBit = false;
			}
        }

        for(auto it: pageTab){
            //如果已经在内存中，则设置该页面refBit为1后结束
            if(it.Item.pageid==RSit.pageid){
                pageTab[it_idx].refBit = true;
                hit = true;
                break;
            }
            it_idx++;
        }
        //如果miss则替换最小历史位的页
        if(hit == false){
            ++pageFault;
            for( int i = 0;i<maxNum_Frames;i++){
				int cur = (nextpos+i)%maxNum_Frames;
				//用九位比较
				if ( (int)pageTab[cur].hisbits + pageTab[cur].refBit * 256 < minhis ){
					toReplaceIndex = cur;
					minhis = (int)pageTab[cur].hisbits + pageTab[cur].refBit * 256;
				}
			}
            nextpos = (toReplaceIndex + 1)%maxNum_Frames;

            // cout<<"Page "<<pageTab[toReplaceIndex].Item.pageid<<" swapped, ";
            // cout<<"replaced by "<<RSit.pageid<<endl;

			pageTab[toReplaceIndex].Item = RSit;
			pageTab[toReplaceIndex].refBit = true;
            //pageTab[toReplaceIndex].hisbits = 0;
        }
        ++timeCnt;
    }

    return pageFault;
}


int LRU::SecondChance(){
    RSItem empty_item;
    vector<TabItem_RB>pageTab(maxNum_Frames,TabItem_RB(empty_item,false));//页表
    int pageFault = 0;//缺页错误数
    int nextpos = 0;


    for(const auto RSit: refStr){
        bool hit = false;
        int it_idx = 0;

        for(auto it: pageTab){
            //如果已经在内存中，则设置该帧refBit为1后结束
            if(it.Item.pageid==RSit.pageid){
                pageTab[it_idx].refBit = true;
                hit = true;
                break;
            }
        }
        //如果miss则替换
        if(hit == false){
            ++pageFault;

            while(pageTab[nextpos % maxNum_Frames].refBit == true){
                pageTab[nextpos % maxNum_Frames].refBit = false;
				nextpos = (nextpos+1) % maxNum_Frames; 
			}

            // cout<<"Page "<<pageTab[nextpos].Item.pageid<<" swapped, ";
            // cout<<"replaced by "<<RSit.pageid<<endl;

            pageTab[nextpos] = TabItem_RB(RSit,true);
			nextpos = (nextpos+1) % maxNum_Frames; 

        }
    }

    return pageFault;
}