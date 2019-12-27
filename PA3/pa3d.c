int s;                          // semaphore
{
        /* modify or add code any way you wish */
        if(semtab[s].value == 0){
                addProc(p, s);
                //DPrintf("Just Blocked  car %d, because of semaphore %d\n", p,s);
                Block(p);
        }
        else{
                semtab[s].value --;
        }
}

/*      MySignal (p, s) is called by the kernel whenever the system call
 *      Signal (s) is called.
 */

void MySignal (p,s)
        int p;                          // process
        int s;                          // semaphore
{
        /* modify or add code any way you wish */
        if(isListEmpty(s) == 0){
                p = removeProc(p,s);
                //DPrintf("Just unBlocked car %d\n",p);
                Unblock(p);
        }
        else{
                semtab[s].value++;
        }
}

void addProc(int p,int s){
        int tail = semtab[s].procTail;
        semtab[s].procList[tail] = p;
        semtab[s].procTail = (tail + 1)% MAXPROCS;
        semtab[s].procSize += 1;
}

int removeProc(int p,int s){
        int head = semtab[s].procHead;
        int tempP = semtab[s].procList[head];
        semtab[s].procList[head] = 0;
        semtab[s].procHead = (head + 1)% MAXPROCS;
        semtab[s].procSize -= 1;
        return tempP;
}      

int isListEmpty(int s){
        if(semtab[s].procSize == 0){
                return (1);
        }
        return (0);
}
