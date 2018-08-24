#ifndef MEMORYZERO_H
#define MEMORYZERO_H
#include<iostream>
#include<vector>
#include<math.h>
#include<QDebug>
#include<QStringList>
#include<QObject>
using namespace std;

struct Pair{
    unsigned int x;
    unsigned int y;
	Pair(){
		x = 0;
		y = 1;
	}
};

typedef struct Buffer{
    int operation_type;
    int operation_num;
    vector<QChar>save;
    Pair record;
    bool DeleteRow;
    int block_size;
    Buffer(){
        block_size = 0;
        operation_num = 1;
    }
}Buffer;

#define ADD 1
#define BACKSPACE 2
#define DELETE 3
#define BLOCKDELETE 4
#define PASTE 5
#define REPLACE 6
#define INSERTROW 7
#define PASTE_PART 8

#define CAPACITY 100

#define UPSEARCH 6
#define DOWNSEARCH 7
#define WHOLESEARCH 8
#define string_length 100

typedef struct str{
    unsigned int str_num;//串序号
    unsigned int ch_num;//串中字数
    vector<QChar>s;
	str *next;
	str *prior;
	str(){
		ch_num = 0;
		str_num = 0;
        s.reserve(5);
	}
}str;

typedef struct row{
    unsigned int num;//行序号
    unsigned int row_capacity;//行中串的个数
	str *line;
	row	*next;
	row *prior;
	row(){
		num = 0;
        row_capacity = 1;
	}
}row;

class Memory
{
    public:
        vector<Buffer>buffer;
        vector<QChar>cache;
        Pair pair;
        Pair start;//块首
        Pair end;//块尾
        row* head;
        int memory_capacity;//块中的行数
        int memory_size;//块中的字数
    public:
        Memory(){
            memory_capacity = 1;
            memory_size = 0;
            head = new row;//初始化有一空行
            head->next = NULL;

            head->row_capacity = 1;//初始化有一空行
            str *s = new str;
            head->line = s;//初始化有一空串
            head->line->next = NULL;
            head->line->prior = NULL;
            head->line->ch_num = 0;//字数为0
            head->line->str_num = 1; //串序号为1
        }
         ~Memory(){};
        void OutCache(){
            for(int i=0;i<cache.size();i++);
        }
        void init_row(row* r){
            r->num = 0;
            str *s = new str;
            r->line = s;
            r->line->ch_num=0;//字数为0
            r->line->str_num=1; //串序号为1
        }
        /*void inc_line(row* p){//拓展某行的串的个数
            str* s=new str;
            str *r = p->line;
            while(r->next!=NULL)
            r= r->next;
            s->prior = r;
            r->next = s;
            r = r->next;
            r->next = NULL;
            s->ch_num=0;//串中字数为零
            s->str_num=s->prior->str_num+1;//串序号加1
        }*/
        str* inc_line(str *r)
        {
            str *s = new str;
            s->prior = r;
            r->next = s;
            r = r->next;
            r->next = NULL;
            return r;
        }
        void inc_row(){
            row* p= head;
            row *r = new row;
            while(p->next!=NULL)
            p = p->next;
            r->prior = p;
            p->next = r;
            p = p->next;
            p->next = NULL;
            init_row(p);//初始化该行的第一个串
            r->row_capacity=1;//行中有一个空串
            r->line->ch_num=0;
            r->line->str_num=1; // 第一个串为空串
            memory_capacity++;
        }

        //完成
        void Input();
        void Output();
        void Add(QChar ch);//添加字符或字符串
        void AddString(QString);
        void BackSpace();//删除字符,当光标在一行的首位，调用函数实现删除行
        void MovePointer(int ord);//移动光标操作（键盘，鼠标）
        void SetPair(int x,int y);//设置光标位置
        void SetStart(int x,int y);
        void SetEnd(int x,int y);
        void InsertRow();//插入行
        void WordCount();//字符统计
        void Time();//显示当前时间
        void Delete();//删除字符，当光标在一行的末尾，调用函数实现删除行
        void Copy();//复制
        void Paste();//黏贴
        QStringList* GetQStringList();
        QStringList* stringBetweenLines(int, int);
        int lineCount();
        void ReceiveString();
        void Allocation(QString s);
        //未完成
        bool Search(int, QString);//搜索操作（这里可以实现从光标位置向前，从光标位置向后，以及全文搜索功能，并且实现下一个
        //操作，即存在多个符合搜索内容的字符或字符串时，光标应该首先移位到以一次出现的字符串之前，点击先一个操作，移动光标
        //到下一个出现的字符串之前
        void cal_next(QString, int*, int);
        bool KMP(QString, int, QString, int, Pair, int);
        void Exchange(QString);
        void selectCursor(int&, int&, int&, int&, int&, int&);
        //块操作
        //void Replace();//替换操作
        void BlockDelete();//块删除
        //void Recover();//撤销操作
        int getEndNumber();
        void UndoAdd(Buffer);
        void UndoDelete(Buffer);
        void UndoBlockDelete(Buffer);
        void UndoBackSpace(Buffer);
        void UndoPaste_part(Buffer);
        void UndoPaste(Buffer);
        void UndoInsertRow(Buffer);
        void Undo();//撤销操作
        void Redo();//还原函数

};

#endif // MEMORYZERO_H
