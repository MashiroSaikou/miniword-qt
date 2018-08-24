#include<memoryzero.h>


using namespace std;
struct Pair pp;

int Memory::getEndNumber()
{
    row *p = head;
    while(p->next!=NULL){
        p = p->next;
    }
    return p->num;
}
QStringList* Memory::GetQStringList(){
    QStringList *ArrayList = new QStringList;

    int count = 0;
    row *p = head;
    while(p!=NULL){
        str *r = p->line;
        QString temp;
        while(r!=NULL){
            for(int i=0;i<r->s.size();i++)
            {
               temp.append(r->s[i]);
            }
            r = r->next;
        }
        ArrayList->append(temp);
        p = p->next;
    }
    return ArrayList;
}

QStringList* Memory::stringBetweenLines(int a, int b)
{
    QStringList *arrayList = new QStringList;
    row *p = head;
    int i = 0;
    while(i < a){
        p = p->next;
        i++;
    }
    for(int j = 0; j < b-a+1; j++){
        str *r = p->line;
        QString temp;
        while(r!=NULL){
            for(int i=0;i<r->s.size();i++)
            {
               temp.append(r->s[i]);
            }
            r = r->next;
        }
        arrayList->append(temp);
        p = p->next;
    }
    return arrayList;
}

int Memory::lineCount()
{
    int count = 0;
    row *p = head;
    while(p!=NULL){
        count ++;
        p = p->next;
    }
    return count;
}

void Memory::SetPair(int x,int y){
    pair.x = x;
    pair.y = y;
}

void Memory::SetStart(int x,int y){
    start.x = x;
    start.y = y;
}

void Memory::SetEnd(int x,int y){
    end.x = x;
    end.y = y;
}

void Memory::MovePointer(int ord){
    row *p = head;
    if(ord==1)
    {
        if(pair.x==0)
        {
            if(pair.y>1)
            {
                pair.y--;
                for(int i=1;i<pair.y;i++)
                {
                    p = p->next;
                }
                pair.x = p->num;
            }
            else if(pair.y==1)
            {
                return;
            }
        }
        else
        pair.x--;
    }
    if(ord==2)
    {
        for(int i=1;i<pair.y;i++)
        p = p->next;
        if(pair.x==p->num&&pair.y==memory_capacity)
        {
            return;
        }
        if(pair.x!=p->num)
        pair.x++;
        else
        {
            pair.y++;
            pair.x = 0;
        }
    }
    if(ord==3)
    {
        for(int i=1;i<pair.y-1;i++)
        p = p->next;
        if(pair.x<=p->num){
            pair.y--;
        }
        else{
            pair.y--;
            pair.x = p->num;
        }
    }
    if(ord==4)
    {
        if(pair.y<memory_capacity)
        {
            for(int i=1;i<pair.y+1;i++)
            p = p->next;
            if(pair.x>p->num){
                pair.y++;
                pair.x = p->num;
            }
            else{
                pair.y++;
            }
        }
        else if(pair.y==memory_capacity)
        {
            return;
        }
    }
}

void Memory::Allocation(QString s)
{
    if(memory_capacity==1)
    {
        row *p = head;
        if(p->num==0)
        {
            p->num = s.length();
            str *r = p->line;
            r->prior = NULL;
            for(int i=0;i<s.length();i++)
            {

                if(r->s.size()==CAPACITY)
                {
                    r = inc_line(r);
                    p->row_capacity++;
                    r->s.push_back(s[i]);
                    r->next = NULL;
                }
                else if(r->s.size()<CAPACITY)
                {
                    r->s.push_back(s[i]);
                }
            }
            return;
        }
        else
        {
            row *l = new row;
            l->num = s.length();
            l->row_capacity = 1;
            p->next = l;
            l->prior = p;
            l->next = NULL;
            str *t = new str;
            l->line = t;
            l->line->next = NULL;
            l->line->prior = NULL;
            memory_capacity++;
            str *r = l->line;
            for(int i=0;i<s.length();i++)
            {
                if(r->s.size()==CAPACITY)
                {
                    r = inc_line(r);
                    l->row_capacity++;
                    r->s.push_back(s[i]);
                    r->next = NULL;
                }
                else if(r->s.size()<CAPACITY)
                {
                    r->s.push_back(s[i]);
                }
            }
            return;
        }
    }
    else
    {
        row *p = head;
        while(p->next!=NULL)
        {
            p = p->next;
        }
        row *l = new row;
        l->row_capacity = 1;
        l->num = s.length();
        p->next = l;
        l->prior = p;
        l->next = NULL;
        str *t = new str;
        l->line = t;
        l->line->next = NULL;
        l->line->prior = NULL;
        memory_capacity++;
        str *r = l->line;
        for(int i=0;i<s.length();i++)
        {
            if(r->s.size()==CAPACITY)
            {
                r = inc_line(r);
                l->row_capacity++;
                r->s.push_back(s[i]);
                r->next = NULL;
            }
            else if(r->s.size()<CAPACITY)
            {
                r->s.push_back(s[i]);
            }
        }
        return;
    }

}

void Memory::Copy(){
    cache.clear();
    pair = end;
    if(start.y==end.y){
        double d1 = start.x,d2 = end.x;
        int line_num1 = ceil((d1+1)/CAPACITY),x_loc1 = start.x%CAPACITY;
        int line_num2 = ceil(d2/CAPACITY),x_loc2 = end.x%CAPACITY;

        row *p = head;
        for(int i=1;i<start.y;i++)
        p = p->next;
        str *r = p->line;
        str *s = p->line;
        for(int i=1;i<line_num1;i++)
        r = r->next;
        for(int i=1;i<line_num2;i++)
        s = s->next;
        if(line_num1==line_num2){
            if(x_loc1==0&&x_loc2==0){
                cache.push_back(r->s[r->s.size()-1]);
            }
            if(x_loc1>0){
                if(x_loc2==0){
                    for(int i=x_loc1;i<r->s.size();i++)
                    cache.push_back(r->s[i]);
                }
                else{
                    for(int i=x_loc1;i<x_loc2;i++ )
                    cache.push_back(r->s[i]);
                }
            }
            for(int i=0;i<cache.size();i++)
            {
                qDebug() << cache[i];
            }
            return;
        }
        str *t = r;
        for(int i=x_loc1;i<CAPACITY;i++)
        {
            cache.push_back(t->s[i]);
        }
        t = t->next;
        while(true){
            if(t!=s){
                for(int i=0;i<t->s.size();i++)
                {
                    cache.push_back(t->s[i]);
                }
                t = t->next;
            }
            else{
                if(x_loc2==0){
                    for(int i=0;i<CAPACITY;i++){
                        cache.push_back(t->s[i]);
                    }
                }
                else{
                    for(int i=0;i<x_loc2;i++){
                        cache.push_back(t->s[i]);
                    }
                    break;
                }
            }
        }
        for(int i=0;i<cache.size();i++)
        {
            qDebug() << cache[i];
        }
        return;
    }
    if(end.y>start.y){
        row *p = head;
        row *l = head;
        for(int i=1;i<start.y;i++)
        p = p->next;
        for(int i=1;i<end.y;i++)
        l = l->next;
        double d1 = start.x;
        int line_num1 = ceil((d1+1)/CAPACITY),x_loc1 = start.x%CAPACITY;
        str *r = p->line;
        for(int i=1;i<line_num1;i++)
        r = r->next;
        for(int i=x_loc1;i<r->s.size();i++)
        cache.push_back(r->s[i]);
        r = r->next;
        while(r!=NULL){
            for(int i=0;i<r->s.size();i++){
                cache.push_back(r->s[i]);
            }
            r = r->next;
        }
        cache.push_back('\r');
        for(int i=0;i<cache.size();i++)
        {
            qDebug() << cache[i];
        }
        p = p->next;
        while(true){
            if(p!=l){
                str *t = p->line;
                while(t!=NULL){
                    for(int i=0;i<t->s.capacity();i++){
                        cache.push_back(t->s[i]);
                    }
                    t = t->next;
                }
                cache.push_back('\r');
                p = p->next;
            }
            else{
                double d2 = end.x;
                int line_num2 = ceil(d2/CAPACITY),x_loc2 = end.x%CAPACITY;
                str *t = p->line;
                str *s = p->line;
                for(int i=1;i<line_num2;i++)
                s = s->next;
                while(true){
                    if(t!=s){
                        for(int i=0;i<t->s.size();i++)
                        cache.push_back(t->s[i]);
                        t = t->next;
                    }
                    if(t==s){
                        if(x_loc2==0){
                            for(int i=0;i<t->s.size();i++)
                            cache.push_back(t->s[i]);
                        }
                        else{
                            for(int i=0;i<x_loc2;i++)
                            cache.push_back(t->s[i]);
                        }
                        break;
                    }
                }
                for(int i=0;i<cache.size();i++)
                {
                    if(cache[i]!='\r')
                        qDebug() << cache[i];
                    else
                        qDebug() << 'n';
                }
                return;
            }
        }
    }
}

void Memory::Paste(){
    Buffer note;
    note.operation_type = PASTE;
    note.block_size = cache.size();
    if(start.x==end.x&&start.y == end.y)
    {
        qDebug() << "here";
        /*for(int i=0;i<cache.size();i++)
        {
            if(cache[i]=='\r')
                qDebug() << "n" <<endl;
            else
                qDebug() << cache[i] <<endl;
        }*/
        note.operation_num = 1;
        for(int i=0;i<cache.size();i++)
        {
            if(cache[i]=='\r')
                qDebug() << "n" <<endl;
            else
                qDebug() << cache[i] <<endl;
            QChar ch = cache[i];
            Add(ch);
        }
        buffer.push_back(note);
        return;
    }
    else
    {
        qDebug()<<"fsdkljflsdkjf"<<endl;
        note.operation_num = 2;
        BlockDelete();
        for(int i=0;i<cache.size();i++)
        {
            QChar ch = cache[i];
            Add(ch);
        }
        buffer.push_back(note);
        return;
    }
}


void Memory::BackSpace(){
    Buffer note;
    note.operation_type = BACKSPACE;
    if(pair.y==1&&pair.x==0)
        return;
    if(pair.y>1&&pair.x==0)
    {
        memory_capacity--;
        row *p = head;
        for(int i=1;i<pair.y-1;i++)
        p = p->next;
        int len = p->num;
        note.record.x = p->num;
        note.record.y = pair.y-1;
        note.DeleteRow = true;
        note.save.push_back('\r');
        buffer.push_back(note);
        row *l = p->next;
        str *t = p->line;
        str *u = l->line;
        while(t->next!=NULL)
        t = t->next;
        while(u!=NULL){
            for(int i=0;i<u->s.size();i++){
                p->num++;
                if(t->s.size()<t->s.capacity()){
                    t->s.push_back(u->s[i]);
                }
                else if(t->s.size()==t->s.capacity()){
                    t = inc_line(t);
                    p->row_capacity++;
                    t->s.push_back(u->s[i]);
                }
            }
            u = u->next;
        }
        if(l->next==NULL)
        {
            p->next = NULL;
            pair.x = len;
            pair.y--;
            delete l;
            double x = p->num;
            p->row_capacity = ceil(x/CAPACITY);
            if(p->row_capacity==0)
                p->row_capacity = 1;
            return;
        }
        pair.x =len;
        pair.y--;
        p->next = l->next;
        l->next->prior = p;
        delete l;
        double x = p->num;
        p->row_capacity = ceil(x/CAPACITY);
        if(p->row_capacity==0)
            p->row_capacity = 1;
        return;
    }
    note.record.x = pair.x-1;
    note.record.y = pair.y;
    note.DeleteRow = false;
    //buffer.push_back(note);
    row *p = head;
    for(int i=1;i<pair.y;i++)
        p = p->next;
    str *r = p->line;
    double d = pair.x;
    int line_num = ceil(d/CAPACITY);
    int insert_x = pair.x%CAPACITY;
    for(int i=1;i<line_num;i++)
    r = r->next;
    if(r->next==NULL)
    {
        if(line_num==1)
        {
            if(insert_x==0)
            {
                note.save.push_back((r->s.back()));
                r->s.pop_back();
            }
            else
            {
                note.save.push_back(r->s[insert_x-1]);
                r->s.erase(r->s.begin()+insert_x-1);
            }
            buffer.push_back(note);
            p->num--;
            double x = p->num;
            p->row_capacity = ceil(x/CAPACITY);
            if(p->row_capacity==0)
                p->row_capacity = 1;
            pair.x--;
            return;
        }
        else if(line_num>1)
        {
            str *t = r->prior;
            int len = r->s.size();
            if(insert_x==0)
            {
                note.save.push_back((r->s.back()));
                r->s.pop_back();
            }
            else
            {
                note.save.push_back(r->s[insert_x-1]);
                r->s.erase(r->s.begin()+insert_x-1);
            }
            buffer.push_back(note);
            if(len==1)
            {
                t->next = NULL;
            }
            pair.x--;
            double x = p->num;
            p->row_capacity = ceil(x/CAPACITY);
            if(p->row_capacity==0)
                p->row_capacity = 1;
            p->num--;
            return;
        }
    }
    str *t = r;
    if(insert_x!=0)
    {
        note.save.push_back(r->s[insert_x-1]);
        r->s.erase(r->s.begin()+insert_x-1);
    }
    else
    {
        note.save.push_back(r->s.back());
        r->s.pop_back();
    }
    buffer.push_back(note);
    r = r->next;
    while(r->next!=NULL){
       QChar temp = r->s[0];
       t->s.push_back(temp);
       r->s.erase(r->s.begin());
       t = t->next;
       r = r->next;
    }
    if(r->s.size()==1){
       t->s.push_back(r->s[0]);
       t->next = NULL;
       p->row_capacity--;
    }
    else
    {
        QChar temp = r->s[0];
        t->s.push_back(temp);
        r->s.erase(r->s.begin());
    }
    pair.x--;
    p->num--;
    double x = p->num;
    p->row_capacity = ceil(x/CAPACITY);
    if(p->row_capacity==0)
        p->row_capacity = 1;
    return;
}
void Memory::Delete(){
    Buffer note;
    note.operation_type = DELETE;
    row *l = head;
    for(int i=1;i<memory_capacity;i++)
        l = l->next;
    if(pair.x==l->num&&pair.y==memory_capacity)
    {
        return;
    }
    row *p = head;
    for(int i=1;i<pair.y;i++)
    p = p->next;
    str *r = p->line;
    if(pair.x==p->num&&pair.y<memory_capacity){
        note.DeleteRow = true;
        note.record.x = p->num;
        note.record.y = pair.y;
        buffer.push_back(note);
        memory_capacity--;
        row *l = p->next;
        str *t = p->line;
        if(p->row_capacity==1){
            //r->s.pop_back();
            str *u = l->line;
            while(u!=NULL){
                for(int i=0;i<u->s.size();i++){
                    p->num++;
                    if(t->s.size()<t->s.capacity()){
                        t->s.push_back(u->s[i]);
                    }
                    else if(t->s.size()==t->s.capacity()){
                        t = inc_line(t);
                        p->row_capacity++;
                        t->s.push_back(u->s[i]);
                    }
                }
                u = u->next;
            }
            if(l->next==NULL)
            {
                p->next = NULL;
                delete l;
                double x = p->num;
                p->row_capacity = ceil(x/CAPACITY);
                if(p->row_capacity==0)
                    p->row_capacity = 1;
                pair.x--;
                return;
            }
            p->next = l->next;
            l->next->prior = p;
            delete l;
            double x = p->num;
            p->row_capacity = ceil(x/CAPACITY);
            if(p->row_capacity==0)
                p->row_capacity = 1;
            pair.x--;
            return;
        }
        while(t->next!=NULL){
            t = t->next;
        }
        str *u = l->line;
        while(u!=NULL){
            for(int i=0;i<u->s.size();i++){
                p->num++;
                if(t->s.size()<t->s.capacity()){
                    t->s.push_back(u->s[i]);
                }
                else if(t->s.size()==t->s.capacity()){
                    t = inc_line(t);
                    p->row_capacity++;
                    t->s.push_back(u->s[i]);
                }
            }
            u = u->next;
        }
        if(l->next==NULL)
        {
            p->next = NULL;
            delete l;
            double x = p->num;
            p->row_capacity = ceil(x/CAPACITY);
            if(p->row_capacity==0)
                p->row_capacity = 1;
            pair.x--;
            return;
        }
        p->next = l->next;
        l->next->prior = p;
        double x = p->num;
        p->row_capacity = ceil(x/CAPACITY);
        if(p->row_capacity==0)
            p->row_capacity = 1;
        pair.x--;
        delete l;
        return;
    }
    if(pair.x<p->num){
        p->num--;
        note.DeleteRow = false;
        note.record.x = pair.x;
        note.record.y = pair.y;
        //qDebug()<<"delete"<<endl;
        double d = pair.x;
        int line_num = ceil(d/CAPACITY),x_loc = pair.x%CAPACITY;
        str *r = p->line;
        for(int i=1;i<line_num;i++)
        r = r->next;
        if(x_loc==0){
            //r->s.erase(r->s.begin()+)
            if(pair.x!=0)
            {
                r = r->next;
                note.save.push_back(r->s[0]);
                r->s.erase(r->s.begin());
            }
            else
            {
                note.save.push_back(r->s[0]);
                r->s.erase(r->s.begin());
            }
        }
        else{
            note.save.push_back(r->s[x_loc]);
            r->s.erase(r->s.begin()+x_loc);
        }
        buffer.push_back(note);
        if(r->next==NULL){
            //pair.x--;
            double x = p->num;
            p->row_capacity = ceil(x/CAPACITY);
            if(p->row_capacity==0)
                p->row_capacity = 1;
            pair.x--;
            return;
        }
        while(r->next!=NULL){
            str *t = r->next;
            if(t->s.size()==1){
                r->s.push_back(t->s[0]);
                r->next = NULL;
                //pair.x--;
                double x = p->num;
                p->row_capacity = ceil(x/CAPACITY);
                if(p->row_capacity==0)
                    p->row_capacity = 1;
                pair.x--;
                return;
            }
            r->s.push_back(t->s[0]);
            t->s.erase(t->s.begin());
            r = r->next;
        }
        double x = p->num;
        p->row_capacity = ceil(x/CAPACITY);
        if(p->row_capacity==0)
            p->row_capacity = 1;
        pair.x--;
    }
}

void Memory::InsertRow(){
    Buffer note;
    note.record.x = 0;
    note.record.y = pair.y+1;
    note.operation_type = INSERTROW;
    buffer.push_back(note);
    if(pair.y==1&&pair.x==0){
        row *p = head;
        memory_capacity++;
        row *l = new row;
        str *s = new str;
        l->num = 0;
        l->line = s;
        l->line->next = NULL;
        l->line->prior = NULL;
        l->next = p;
        p->prior = l;
        head = l;
        pair.y++;
        return;
    }

    if(pair.y>1&&pair.x==0){
        row *p = head;
        memory_capacity++;
        for(int i=1;i<pair.y-1;i++)
        p = p->next;
        row *l = new row;
        str *s = new str;
        l->num = 0;
        l->line = s;
        l->line->next= NULL;
         l->line->prior = NULL;
        l->prior = p;
        l->next = p->next;
        p->next->prior = l;
        p->next = l;
        pair.y++;
        return;
    }
    if(pair.y==memory_capacity){
        memory_capacity++;
        row *v = head;
        for(int i=1;i<pair.y;i++)
        v = v->next;
        qDebug() << "come here" << endl;
        qDebug() << v->num << endl;
        if(pair.x==v->num){
            str *r = v->line;
            while(r->next!=NULL){
                r = r->next;
            }
            row *l = new row;
            str *s = new str;
            l->num = 0;
            l->line = s;
            l->row_capacity = 1;
            //l->line->s.push_back('');
            l->line->next = NULL;
            l->line->prior = NULL;
            l->prior = v;
            v->next = l;
            l->next = NULL;
            pair.x = 0;
            pair.y++;
            return;
        }
        else if(pair.x<v->num&&pair.x>0){
            int dif = v->num-pair.x;
            row *l = new row;
            str *s = new str;
            l->num = 0;
            l->line = s;
            l->next = NULL;
            l->prior = v;
            v->next = l;
            l->line->next = NULL;
            l->line->prior = NULL;
            str *u = l->line;
            double d = pair.x;
            int line_num = ceil(d/CAPACITY),x_loc = pair.x%CAPACITY;
            str *r = v->line;
            for(int i=1;i<line_num;i++)
            r = r->next;
            if(x_loc==0){
                str *t = r->next;
                while(t!=NULL){
                    for(int i=0;i<t->s.size();i++){
                        l->num++;
                        if(u->s.size()<CAPACITY){
                            u->s.push_back(t->s[i]);
                        }
                        else if(u->s.size()==CAPACITY){
                            u = inc_line(u);
                            u->s.push_back(t->s[i]);
                            l->row_capacity++;
                        }
                    }
                    t = t->next;
                }
                r->next = NULL;
                u->next = NULL;
                pair.x = 0;
                pair.y++;
                v->num = v->num-dif;
                double x = v->num;
                v->row_capacity = ceil(x/CAPACITY);
                if(v->row_capacity==0)
                    v->row_capacity = 1;
                return;
            }
            else if(x_loc>0){
                for(int i=x_loc;i<r->s.size();i++){
                    u->s.push_back(r->s[i]);
                    l->num++;
                }
                str *t = r->next;
                int len = r->s.size();
                for(int i=x_loc;i<len;i++){
                    r->s.pop_back();
                }
                v->num = v->num-dif;
                double x = v->num;
                v->row_capacity = ceil(x/CAPACITY);
                if(v->row_capacity==0)
                    v->row_capacity = 1;
                r->next= NULL;
                while(t!=NULL){
                    for(int i=0;i<t->s.size();i++){
                        l->num++;
                        //v->num--;
                        if(u->s.size()<CAPACITY){
                            u->s.push_back(t->s[i]);
                        }
                        else if(u->s.size()==CAPACITY){
                            u = inc_line(u);
                            u->s.push_back(t->s[i]);
                            l->row_capacity++;
                        }
                    }
                    t = t->next;
                }
                u->next = NULL;
                pair.y++;
                pair.x = 0;
                return;
            }
        }
    }
    if(pair.y<memory_capacity&&pair.x>0){

        memory_capacity++;
        row *v = head;
        for(int i=1;i<pair.y;i++)
        v = v->next;
        if(pair.x==v->num){

            row *l = new row;
            str *s = new str;
            l->num = 0;
            l->line = s;
            l->line->prior = NULL;
            str *u = l->line;
            u->next = NULL;
            l->prior = v;
            l->next = v->next;
            v->next->prior = l;
            v->next = l;
            pair.y++;
            pair.x = 0;
            return;
        }
        if(pair.x<v->num){
            qDebug()<< "here here\n";
            int dif = v->num-pair.x;
            v->num = v->num - dif;
            double x = v->num;
            v->row_capacity = ceil(x/CAPACITY);
            if(v->row_capacity==0)
                v->row_capacity = 1;
            row *l = new row;
            str *s = new str;
            l->num = 0;
            l->line = s;
            l->line->prior = NULL;
            str *u = l->line;
            u->next = NULL;
            l->prior = v;
            l->next = v->next;
            v->next->prior = l;
            v->next = l;
            double d = pair.x;
            int line_num = ceil(d/CAPACITY),x_loc = pair.x%CAPACITY;
            str *r = v->line;
            for(int i=1;i<line_num;i++)
            r = r->next;
            if(x_loc==0){
                str *t = r->next;
                while(t!=NULL){
                    for(int i=0;i<t->s.size();i++){
                        l->num++;
                        if(u->s.size()<CAPACITY){
                            u->s.push_back(t->s[i]);
                        }
                        else if(u->s.size()==CAPACITY){
                            u = inc_line(u);
                            u->s.push_back(t->s[i]);
                            l->row_capacity++;
                        }
                    }
                    t = t->next;
                }
                r->next = NULL;
                u->next = NULL;
                pair.y++;
                pair.x = 0;
                return;
            }
            else if(x_loc>0){
                for(int i=x_loc;i<r->s.size();i++){
                    u->s.push_back(r->s[i]);
                    l->num++;
                }
                int len = r->s.size();
                for(int i=x_loc;i<len;i++){
                    r->s.pop_back();
                }
                if(r->next==NULL)
                {
                    u->next = NULL;
                    pair.y++;
                    pair.x = 0;
                    return;
                }
                str *t = r->next;
                while(t!=NULL){
                    for(int i=0;i<t->s.size();i++){
                        l->num++;
                        if(u->s.size()<CAPACITY){
                            u->s.push_back(t->s[i]);
                        }
                        else if(u->s.size()==CAPACITY){
                            u = inc_line(u);
                            u->s.push_back(t->s[i]);
                            l->row_capacity++;
                        }
                    }
                    t = t->next;
                }
                r->next= NULL;
                u->next = NULL;
                pair.y++;
                pair.x = 0;
                return;
            }
        }
    }
}

void Memory::AddString(QString text)
{
    for(int i = 0; i < text.length(); i++)
        Add(text[i]);
}

void Memory::Add(QChar ch){
    if(ch=='\r'){
        InsertRow();
        return;
    }
    Buffer note;
    note.record.x = pair.x+1;
    note.record.y = pair.y;
    note.DeleteRow = false;
    note.operation_type = ADD;
    row *p = head;
    for(int i=1;i<pair.y;i++)
    p = p->next;
    str *r = p->line;
    if(pair.x==p->num){
        while(r->next!=NULL)
        r = r->next;
        if(r->s.size()<CAPACITY){
            r->s.push_back(ch);
            p->num++;
            pair.x++;
            buffer.push_back(note);
            //UndoDeBug();
            return;
        }
        else if(r->s.size()==CAPACITY){
            r = inc_line(r);
            p->row_capacity++;
            r->s.push_back(ch);
            p->num++;
            pair.x++;
            buffer.push_back(note);
            //UndoDeBug();
            return;
        }
    }

    double d = pair.x;
    int line_num = ceil((d+1)/CAPACITY),insert_x = pair.x%CAPACITY;
    for(int i=1;i<line_num;i++)
    r = r->next;
    if(r->next==NULL){
        if(r->s.size()<CAPACITY){
            r->s.insert(r->s.begin()+insert_x,ch);
            pair.x++;
            p->num++;
            buffer.push_back(note);
            return;
        }
        if(r->s.size()==CAPACITY){
            QChar temp = r->s.back();
            r->s.pop_back();
            r->s.insert(r->s.begin()+insert_x,ch);
            r = inc_line(r);
            p->row_capacity++;
            r->s.push_back(temp);
            p->num++;
            pair.x++;
            buffer.push_back(note);
            return;
        }
    }
    QChar temp = r->s.back();
    r->s.pop_back();
    r->s.insert(r->s.begin()+insert_x,ch);
    line_num += 1;
    r = r->next;
    for(int i=line_num;i<p->row_capacity;i++)
    {

        QChar t = temp;
        temp = r->s.back();
        r->s.pop_back();
        r->s.insert(r->s.begin(),t);
        r = r->next;
    }

    if(r->s.size()<CAPACITY){
        r->s.insert(r->s.begin(),temp);
    }
    else if(r->s.size()==CAPACITY){
        QChar pis = r->s[CAPACITY-1];
        r->s.pop_back();
        r->s.insert(r->s.begin(),temp);
        r = inc_line(r);
        p->row_capacity++;
        r->s.push_back(pis);
    }
    p->num++;
    pair.x++;
    buffer.push_back(note);
    return;
}

void Memory::BlockDelete(){
    Buffer note;
    note.operation_type = BLOCKDELETE;
    note.record.x = start.x;
    note.record.y = start.y;
    pair = end;
    if(start.y==end.y){
        int len = end.x - start.x;
        note.block_size = len;
        for(int i=1;i<=len;i++)
        {
            BackSpace();
        }
        buffer.push_back(note);
        return;
    }
    if(end.y>start.y){
        row *p = head;
        int len = 0;
        for(int i=1;i<start.y;i++){
            p = p->next;
        }
        len = p->num-start.x + end.x;
        int dif = end.y - start.y;
        len = len + dif;
        for(int i=1;i <dif;i++){
            p = p->next;
            len = len + p->num ;
        }
        note.block_size = len;
        for(int i=1;i<=len;i++)
        {
            BackSpace();
        }
        buffer.push_back(note);
        return;
    }
}

void Memory::cal_next(QString str, int *next, int len)
{
    next[0] = -1;
    int k = -1;
    for (int q = 1; q <= len-1; q++)
    {
        while (k > -1 && str[k + 1] != str[q])
        {
            k = next[k];
        }
        if (str[k + 1] == str[q])
        {
            k = k + 1;
        }
        next[q] = k;
    }
}
bool Memory::KMP(QString str, int slen, QString ptr, int plen,Pair p,int search_type)
{
    int *next = new int[plen];
    cal_next(ptr, next, plen);
    int k = -1;
    for (int i = 0; i < slen; i++)
    {
        while (k >-1&& ptr[k + 1] != str[i])
            k = next[k];
        if (ptr[k + 1] == str[i])
            k = k + 1;
        if (k == plen-1)
        {
            if(search_type!=UPSEARCH){
                p.x+=i+1;
                //pair.x+=i+1;//////////////////////////////////////////////////////////////////////////////
                pair=p;
                end=pair;
                start.y=pair.y;
                start.x=pair.x-plen;
                qDebug()<<start.x<<' '<<start.y<<' '<<end.x<<' '<<end.y<<endl;
                qDebug()<<str<<endl;
                return true;
            }
            else{
                p.x-=i+1;
                pair=p;
                start=pair;
                end.y=pair.y;
                end.x=pair.x+plen;
                qDebug()<<plen<<"----"<<endl;
                return true;
            }
        }
    }
    return false;
}

bool Memory::Search(int search_type,QString search){
    int tlen,slen,str_num,ch_num;
    bool key;
    Pair p;
    QString temp;
    row *r;
    str *st;
    switch(search_type){
        case DOWNSEARCH:
            p=pair;
            slen=search.length();
            r=head;
            for(int i=1;i<pair.y;i++)//定位行
            r=r->next;
            st=r->line;
            str_num=pair.x/string_length;
            for(int i=0;i<str_num;i++)//定位串
            st=st->next;
            ch_num=pair.x%string_length;
            for(int i=ch_num;i<st->s.size();i++)//读取该串剩余字符
            temp+=st->s[i];
            if(st->next!=NULL){
                st=st->next;
                for(int i=str_num+1;i<r->row_capacity;i++){//读取该行剩余的串
                    for(int j=0;j<st->s.size();j++)
                    temp+=st->s[j];
                    st=st->next;
                }
            }
            tlen=temp.length();
            key=KMP(temp,tlen,search,slen,p,search_type);
            if(key==true)
            return key;
            temp.clear();
            r=r->next;
            while(r!=NULL){
                p.y++;
                p.x=0;
                st=r->line;
                while(st!=NULL){
                    for(int j=0;j<st->s.size();j++){
                        temp+=st->s[j];
                    }
                    st=st->next;
                }
                tlen=temp.length();
                key=KMP(temp,tlen,search,slen,p,search_type);
                if(key==true)
                return key;
                temp.clear();
                r=r->next;
            }
            return false;
            break;
        case UPSEARCH:
            p=pair;
            slen=search.length();
            reverse(search.begin(),search.end());
            r=head;
            for(int i=1;i<pair.y;i++)//定位行
            r=r->next;
            st=r->line;
            str_num=pair.x/string_length+1;//光标所在的串号
            ch_num=pair.x%string_length;//光标在串中的位置
            for(int i=1;i<str_num;i++)//定位串
            st=st->next;
            for(int i=ch_num-1;i>=0;i--)
            temp+=st->s[i];
            st=st->prior;
            while(st!=NULL){
                for(int j=st->s.size()-1;j>=0;j--)
                temp+=st->s[j];
                st=st->prior;
            }
            tlen=temp.length();
            key=KMP(temp,tlen,search,slen,p,search_type);
            if(key==true)
            return key;
            r=r->prior;
            temp.clear();
            while(r!=NULL){
                p.y--;
                p.x=r->num;
                st=r->line;
                while(st->next!=NULL)
                st=st->next;
                for(int i=st->s.size()-1;i>=0;i--)
                temp+=st->s[i];
                st=st->prior;
                while(st!=NULL){
                    for(int j=st->s.size()-1;j>=0;j--)
                    temp+=st->s[j];
                    st=st->prior;
                }
                tlen=temp.length();
                key=KMP(temp,tlen,search,slen,p,search_type);
                if(key==true)
                return key;
                temp.clear();
                r=r->prior;
            }
            return false;
            break;
        case WHOLESEARCH:
            p=pp;
            slen=search.length();
            r=head;
            for(int i=1;i<p.y;i++)//定位行
            r=r->next;
            st=r->line;
            str_num=p.x/string_length;
            for(int i=0;i<str_num;i++)//定位串
            st=st->next;
            ch_num=p.x%string_length;
            for(int i=ch_num;i<st->s.size();i++)//读取该串剩余字符
            temp+=st->s[i];
            if(st->next!=NULL){
                st=st->next;
                for(int i=str_num+1;i<r->row_capacity;i++){//读取该行剩余的串
                    for(int j=0;j<st->s.size();j++)
                    temp+=st->s[j];
                    st=st->next;
                }
            }
            tlen=temp.length();
            key=KMP(temp,tlen,search,slen,p,search_type);
            if(key==true){
                pp=pair;
                return key;
            }
            temp.clear();
            r=r->next;
            while(r!=NULL){
                p.y++;
                p.x=0;
                st=r->line;
                while(st!=NULL){
                    for(int j=0;j<st->s.size();j++){
                        temp+=st->s[j];
                    }
                    st=st->next;
                }
                tlen=temp.length();
                key=KMP(temp,tlen,search,slen,p,search_type);
                if(key==true){
                    pp=pair;
                    return key;
                }
                temp.clear();
                r=r->next;
            }
            pp.x=0;
            pp.y=1;
            return false;
            break;
        }
}

void Memory::Exchange(QString ex){
    int len=ex.length();
    BlockDelete();
    start.x = end.x = pair.x;
    start.y = end.y = pair.y;
    for(int i=0;i<len;i++){
        Add(ex[i]);
        end.x ++;
    }
}

void Memory::selectCursor(int &nl, int &np, int &sl, int &sp, int &el, int &ep)
{
    nl = pair.y-1;
    np = pair.x;

    sl = start.y-1;
    sp = start.x;
    el = end.y-1 ;
    ep = end.x;
}


void Memory::UndoBlockDelete(Buffer note)
{
    buffer.pop_back();
    int len = note.block_size;
    for(int i=1;i<=len;i++)
    {
        note = buffer.back();
        UndoBackSpace(note);
    }
    return;
}

void Memory::UndoBackSpace(Buffer note)
{
    buffer.pop_back();

    if(note.DeleteRow==true)
    {
        pair.x = note.record.x;
        pair.y = note.record.y;
        Add('\r');
        buffer.pop_back();
        return;
    }
    else
    {
        pair.x = note.record.x;
        pair.y = note.record.y;
        QChar ch = note.save[0];
        Add(ch);
        buffer.pop_back();
        return;
    }
}

void Memory::UndoDelete(Buffer note)
{
    buffer.pop_back();
    if(note.DeleteRow==true)
    {

        pair.x = note.record.x;
        pair.y = note.record.y;
        Add('\r');
        buffer.pop_back();
        return;
    }
    else
    {
        pair.x = note.record.x;
        pair.y = note.record.y;
        QChar ch = note.save[0];
        Add(ch);
        buffer.pop_back();
        return;
    }
}

void Memory::UndoAdd(Buffer note)
{
    buffer.pop_back();
    pair.x = note.record.x;
    pair.y = note.record.y;
    BackSpace();
    buffer.pop_back();
    return;
}

void Memory::UndoPaste_part(Buffer note)
{
    buffer.pop_back();
    int len = note.block_size;
    for(int i=1;i<=len;i++)
    {
        note = buffer.back();
        UndoAdd(note);
    }
}

void Memory::UndoPaste(Buffer note)
{
    if(note.operation_num==1)
    {
        UndoPaste_part(note);
        return;
    }
    else
    {
        note = buffer.back();
        UndoPaste_part(note);
        note = buffer.back();
        UndoBlockDelete(note);
        return;
    }
}

void Memory::UndoInsertRow(Buffer note)
{
    BackSpace();
    buffer.pop_back();
    return;
}

void Memory::Undo()
{

    if(buffer.size()==0)
     return;
    Buffer note;
    note = buffer.back();
    if(note.operation_type==ADD)
    {

        UndoAdd(note);
    }
    if(note.operation_type==BACKSPACE)
    {
       UndoBackSpace(note);
    }
    if(note.operation_type==DELETE)
    {
        UndoDelete(note);
    }
    if(note.operation_type==BLOCKDELETE)
    {
        UndoBlockDelete(note);
    }
    if(note.operation_type==PASTE)
    {
        UndoPaste(note);
        return;
    }
    if(note.operation_type==INSERTROW)
    {
        UndoInsertRow(note);
        return;
    }
}

