#include<bits/stdc++.h>
#include <fstream>
using namespace std;

class PCB{
public:
    int jobid;
    int TTL;
    int TLL;
    int TTC;
    int TLC;

    PCB(int id, int ttl, int tll)
    {
        jobid = id;
        TTL = ttl;
        TLL = tll;
        TTC = 0;
        TLC = 0;
    }
};

class Opsys {
private:
    vector<vector<char>>M;  //main memory
    vector<char>IR;
    int IC, ptr, SI, PI, TI, EM, flag; //flag is used to denote if data card is encountered
    vector<char>R;
    bool C;
    char buffer[40];
    char PTR[4]; 
    unordered_map<int,int> map; //in this map we are storing the page addresses which are
    PCB *pcb;                   //currently in use

public:
    void LOAD();
    void INIT();
    void displaymemory();
    void ALLOCATE();
    int AddressMap(int VA );
    void STARTEXECUTION();
    void EXECUTEUSERPROGRAM();
    void MOS();
    int returnrand();
    void READ();
    void WRITE();
    void TERMINATE(int EM);
    void SIMULATION();
    fstream read;
    fstream write;
};

void Opsys :: displaymemory()
{
    cout<<"displaying memory: "<<endl;
    cout<<"--------------------------"<<endl;
    for( auto x: M )
    {
        for(auto y:x)
        {
            cout<<y;
        }
        cout<<endl;
    }
    cout<<"---------------------------"<<endl;
}

void Opsys :: TERMINATE(int EM)
{
    cout<<"terminate called"<<endl;
    switch(EM)
    {
        case 0:
            cout<< "No errors.."<<endl;
            write<<"No errors..\n";
            break;
        case 1:
            cout<<"Out of data error."<<endl;
            write<<"Out of data error.\n";
            break;
        case 2:
            cout<<"Line limit exceeded."<<endl;
            write<<"Line limit exceeded.\n";
            break;
        case 3:
            cout<<"Time limit exceeded."<<endl;
            write<<"Time limit exceeded.\n";
            break;
        case 4:
            cout<<"Operation code error."<<endl;
            write<<"Operation code error.\n";
            break;
        case 5:
            cout<<"Oprand error."<<endl;
            write<<"Oprand error.\n";
            break;
        case 6:
            cout<<"Invalid Page fault."<<endl;
            write<<"Invalid Page fault.\n";
            break;
    }
    SI=0;
    write<<"jobid-"<<pcb->jobid<<" TTL-"<<pcb->TTL<<" TLL-"<<pcb->TLL<<" TLC-"<<pcb->TLC<<" TTC-"<<pcb->TTC<<" SI-"<<SI<<" PI-"<<PI<<" TI-"<<TI;
    write<<"\n";
    write<<"\n";
    write<<"\n";

}

void Opsys :: READ()
{
    cout<<"reading the dta... "<<endl;
    for( int i = 0 ; i< 40 ; i++ )
    {
        buffer[i] = '\0';
    }
    read.getline(buffer, 41);
    if(buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D'){
        EM = 1;
        TERMINATE(EM);
        return;
    }

    cout<<"address in IR is :"<<IR[2]<<IR[3]<<endl;
    int VA = ((IR[2] - '0')*10) + (IR[3] - '0');
    int RA = AddressMap(VA);
    RA = (RA*10)/10;
    cout<<"RA is to put on data in gd-"<<RA<<endl;
    displaymemory();

    int k = 0 ;
    for( int i = RA ; i< RA+10 ; i++ )
    {
        for( int j = 0 ; j < 4 ; j++ )
        {
            M[i][j] = buffer[k++];
        }
        if(k == 40) break; //buffer exhausted;
    }
    SI = 0 ;
    cout<<"READ ends.."<<endl;
}

void Opsys :: WRITE()
{
    pcb->TLC = pcb->TLC + 1;
    if( pcb->TLC > pcb->TLL )
    {
        EM = 2;
        TERMINATE(2);
        return;
    }
    cout<<"write function called..."<<endl;
    for( int i = 0 ; i< 40 ; i++ )
    {
        buffer[i] = '\0';
    }

    int VA = ((IR[2] - '0')*10) + (IR[3] - '0');
    int RA = AddressMap(VA);
    RA = (RA*10)/10;

    int k = 0 ;
    for( int i = RA ; i< RA+10 ; i++ )
    {
        for( int j = 0 ; j < 4 ; j++ )
        {
            buffer[k] = M[i][j]  ;
            write<<buffer[k];
            k++;
        }
        if(k == 40) break; //buffer exhausted;
    }
    write<<"\n";
    cout<<"buffer to put in op.txt is  "<<buffer<<endl;
    SI = 0;
}

//shift the execution in master mode
void Opsys :: MOS()
{
    cout<<"in MOS --"<<endl;
    if(TI == 0){
        if(SI == 1){
            READ();
            SI = 0;
        }

        else if(SI == 2){
            WRITE();
            SI = 0;
        }

        else if(SI == 3){
            EM = 0 ;
            TERMINATE(0);
            SI = 0;
        }

        else if(PI == 1){
            EM = 4 ;
            TERMINATE(4);
        }

        else if(PI == 2){
            EM = 5;
            TERMINATE(5);
        }

        else if(PI == 3){
            EM = 6;
            TERMINATE(6);
        }
    }

    else if(TI == 2){

         if(SI == 1){
            EM = 3;
            TERMINATE(3);
        }

        else if(SI == 2){
            WRITE();
            EM = 3;
            TERMINATE(3);
        }

        else if(SI == 3){
            EM = 0;
            TERMINATE(0);
        }

        else if(PI == 1){
            EM = 3; 
            TERMINATE(3);
            TERMINATE(4);
        }

        else if(PI == 2){
            EM = 3; 
            TERMINATE(3);
            TERMINATE(5);
        }

        else if(PI == 3){
            EM = 3; 
            TERMINATE(3);
        }
    }
}

//this function will check if the page no is already in use or not 
//and return the unique no which is not in use
int Opsys :: returnrand()
{
    int index; 
    while(true)
    {
        index = rand()%29;
        if( map[index] == 0 ) break;
    }
    map[index]++;
    return index;
}

//to increase the time counter 
//and to set the time limit exceeded error if occurs
void Opsys :: SIMULATION()
{
    pcb->TTC = pcb->TTC + 1;
    if( pcb->TTC == pcb->TTL ) 
    {
        TI = 2;
        cout<<"ti set to 2..."<<endl;
    }
}

//function will return the RA from given input VA
//this will set all the PI interrupts if required
int Opsys :: AddressMap( int VA )
{
    cout<<"in addressmap..........."<<endl;
    //formla to calculate page table entry
    int pte = ptr + VA/10;
    int pte_val = ( M[pte][2] - '0' )*10 + M[pte][3] - '0';
    int RA = ( pte_val )*10 + VA%10;
    cout<<"pte is "<<pte<<" value at pte is "<<pte_val<<" VA is "<< VA<<" RA is "<<RA<<endl;

    //oprand error
    cout<<"IR is"<<IR[0]<<IR[1]<<IR[2]<<IR[3]<<endl;
    if(( IR[0] == 'G' && IR[1] == 'D') || (IR[0] == 'P' && IR[1] == 'D') ||
        (IR[0] == 'L' && IR[1] == 'R') || (IR[0] == 'S' && IR[1] == 'R') ||
        (IR[0] == 'C' && IR[1] == 'R') || (IR[0] == 'B' && IR[1] == 'T') )
        {
            if( (IR[2] < '0' || IR[2] >'9')  || (IR[3] < '0' || IR[3] >'9')) 
            {
                PI = 2;
                cout<<"Oprand error operand out of memory index..........."<<endl;
                cout<<"PI is set to 2......."<<endl;
                return RA;
            }
        }
    if( VA>99 || VA<0 )
    {
        PI = 2;
        cout<<"Oprand error operand out of memory index..........."<<endl;
        cout<<"PI is set to 2......."<<endl;
        return RA;
    }

    //page fault
    if( (M[pte][2] == '*') && (M[pte][3] == '*') )
    {
        if( (IR[0] == 'G' && IR[1] == 'D' ) || ( IR[0] == 'S' && IR[1] == 'R' ) )
        {
            //valid page fault
            PI = 0;
            SIMULATION();
            cout<<"valid page fault..."<<endl;
            
            int index = returnrand();
            
            cout<<"unique index got is "<<index<<endl;
            cout<<"gd entry"<<" is on "<<pte<<" and is "<<index<<endl;
            M[pte][0] = '0';
            M[pte][1] = '0';
            M[pte][3] = '0' + (index %10);
            M[pte][2] = '0' + ((index/10)%10);
            return index*10;
        }
        else 
        {
            //invalid page fault
            PI = 3;
            cout<<"Invalid Page fault occurs.........."<<endl;
            cout<<"PI set to 3......."<<endl;
        }
    }

    cout<<"function ends with value of pi "<<PI<<endl;
    cout<<endl;
    return RA;
}

//execute instructions in user mode
//else will set the interrupts to appropriate values
void Opsys :: EXECUTEUSERPROGRAM()
{
    cout<<"in execute user program.........."<<endl;
    while(true)
    {
        cout<<"getting instruction to load in instruction register..."<<endl;
        int RA = AddressMap(IC);

        if( PI != 0 )
        {
            cout<<"breaking execute program loop coz of PI error while fetching instruction."<<endl; 
            MOS();
            break;
        }

        for( int i = 0 ; i< 4 ; i++ )
        {
            IR[i] = M[RA][i]; //ir = gd10
        }
        
        IC += 1;

        int oprand_VA;
        int oprand_RA;
        if( IR[0] != 'H' )
        {
            oprand_VA = ( IR[2] - '0' )*10 + ( IR[3] - '0');
            oprand_RA = AddressMap(oprand_VA);
        }

        if( PI != 0 )
        {
            
            cout<<"breaking execute program loop coz of PI error while executing instruction."<<endl; 
            MOS();
            break;
        }

        if( IR[0] == 'G' && IR[1] == 'D' )
        {
            
            cout<<"instruction is "<<"GD"<<oprand_VA<<endl;
            cout<<"to check oprand of instruction from instruction register.."<<endl;
            SI = 1;
            SIMULATION();
        }
        else if( IR[0] == 'P' && IR[1] == 'D' )
        {
            cout<<"instruction is "<<"PD"<<oprand_VA<<endl;
            cout<<"to check oprand of instruction from instruction register.."<<endl;
            SI = 2;
            SIMULATION();
        }
        else if( IR[0] == 'L' && IR[1] == 'R' )
        {
            for( int i = 0 ; i<4; i++ )
            {
                R[i] = M[oprand_RA][i];
            }
        }
        else if( IR[0] == 'S' && IR[1] == 'R' )
        {
            for( int i = 0 ; i<4; i++ )
            {
                M[oprand_RA][i] = R[i];
            }
        }
        else if( IR[0] == 'C' && IR[1] == 'R' )
        {
            int count = 0 ;
            for(int i = 0 ; i<4 ; i++)
            {
                if( R[i] == M[oprand_RA][i])
                {
                    count++;
                }
            }
            if(count == 4) C = true;
            else C = false; 
            cout<<"value of C is : "<<C<<endl;
        }
        else if( IR[0] == 'B' && IR[1] == 'T' )
        {
            if( C == true )
            {
                IC = oprand_VA ;
            }
        }
        else if( IR[0] == 'H')
        {
            cout<<"instruction is H"<<endl;
            SI = 3;
            SIMULATION();
            MOS();
            break;
        }
        else 
        {
            cout<<"opcode error"<<endl;
            PI = 1 ;
        }
        
        if(SI != 0 || TI != 0 || PI != 0 ) 
        {
            MOS();
            if( EM != 0 ) break;
        }
        else 
        {
            continue;
        }
    }
    cout<<"execute user program exits.....with values....."<<endl;
    cout<<"jobid-"<<pcb->jobid<<" TTL-"<<pcb->TTL<<" TLL-"<<pcb->TLL<<" TLC-"<<pcb->TLC<<" TTC-"<<pcb->TTC<<endl;
    cout<<"SI-"<<SI<<" PI-"<<PI<<" TI-"<<TI<<endl;
}

void Opsys :: STARTEXECUTION()
{
    cout<<"in startexecution.........."<<endl;
    IC = 0;
    EXECUTEUSERPROGRAM();
}

void Opsys :: ALLOCATE()
{
    cout<<"in allocate......"<<endl;
    ptr = returnrand()*10;

    cout<<"ptr is- "<<ptr<<endl;

    PTR[3] = '0';
    PTR[2] = '0' + (ptr%10);
    PTR[1] = '0' + ((ptr/10)%10);

    for( int i = ptr ; i < ptr + 10 ; i++ )
    {
        for ( int j = 0 ; j<4 ; j++)
        {
            M[i][j] = '*';
        }
    }

    cout<<"Page table initialized ...."<<endl;
}

void Opsys :: INIT()
{
    cout<<"in init function......."<<endl;
    M.resize(300, vector<char>(4));

    for( int i = 0 ; i<300 ; i++ )
    {
        for( int j = 0 ; j<4 ; j++ )
        {
            M[i][j] = '\0';
        }
    }
    
    flag = 0;
    IR.resize(4);
    for( int i = 0 ; i<4 ; i++ ) IR[i] = '\0';
    IC = 0;
    R.resize(4);
    for( int i = 0 ; i<4 ; i++ ) R[i] = '\0';

    C = false;

    PI = SI = TI = EM = 0;

    for( auto x: PTR ) x ='0'; 
    ptr = 0;
    for( auto x : map )
    {
        map[x.first] = 0;
    }
    string s; //$AMJ000100030001

    for( auto x: buffer ) s = s+x;
    string job = s.substr(4,4);
    string TTL = s.substr(8,4);
    string TLL = s.substr(12,4);

    cout<<"control card details---"<<endl;
    cout<<job<<endl<<TTL<<endl<<TLL;
    int jobid = stoi(job);
    int ttl = stoi(TTL);
    int tll = stoi(TLL);

    pcb = new PCB(jobid, ttl, tll);
   
    ALLOCATE();
}

//this will load the control card and program card
//also will redirect to function STARTXECUTION() once $DTA is encountered 
void Opsys :: LOAD()
{
    cout<<"in LOAD......"<<endl;
    while( read.eof() == 0 )
    {
        for(int i = 0 ; i<40 ; i++)
        {
            buffer[i] = '\0';
        }
       
        read.getline(buffer, 41);

        //will initialize all the variables to its default values
        if(buffer[0]=='$' && buffer[1]=='A' && buffer[2]=='M' && buffer[3]=='J')
        {
            cout<<"in AMJ ............"<<endl;
            INIT();
        }
        else if(buffer[0]=='$' && buffer[1]=='D' && buffer[2]=='T' && buffer[3]=='A')
        {
            displaymemory();
            STARTEXECUTION();
            flag = 1; 
        }
        else if (buffer[0]=='$' && buffer[1]=='E' && buffer[2]=='N' && buffer[3]=='D')
        {
            cout<<"Job ended.."<<endl;
        }
        else if ( flag == 0 ) //-------------------------------------------------------------------------------------------
        {
            
            cout<<"reading pc....pc in buffer is-----"<<buffer<<endl;

            int index = returnrand();
            cout<<"unique index got is "<<index<<endl;

            int pte;
            for( int i = ptr ; i< ptr+10 ; i++ )
            {
                if ( M[i][2] == '*' && M[i][3] == '*' )
                {
                     pte = i;
                     break;
                }
            }

            cout<<"Page table entry for"<<" pc"<<(pte%10)+1<<" is on "<<pte<<" and is "<<index<<endl;
            M[pte][0] = '0';
            M[pte][1] = '0';
            M[pte][3] = '0' + (index %10);
            M[pte][2] = '0' + ((index/10)%10);

            int k = 0;
            for( int i = index*10 ; i< (index*10)+10 ; i++ )
            {
                for( int j = 0 ; j<4 ; j++ )
                {
                    M[i][j] = buffer[k++];
                    if( M[i][j] == 'H' )
                    {
                        break;
                    }
                }
            }
        }
    }
}

int main()
{
    Opsys os;
    os.read.open("input.txt",ios::in);
    os.write.open("output.txt", ios::app);
    os.LOAD();
    os.write.close();
    os.read.close();
   
    cout<<"Main memory after complete execution is -----------------------------------"<<endl;
    os.displaymemory();
    return 0;
}