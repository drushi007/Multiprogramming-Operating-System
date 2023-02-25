#include<bits/stdc++.h>
#include<fstream>   
using namespace std;

class Opsys{
private:
    vector<vector<char>> M;
    vector<char>IR,R;
    char buffer[40];
    bool C;
    int m; //to keep track of pc
    int IC,counter,SI; //si is for cases in gd, pd .....

public:
    void LOAD();
    void INIT();
    void STARTEXECUTION();
    void EXECUTEUSERPROGRAM();
    void MOS();
    void TERMINATE();
    void READ();
    void WRITE();
    void displaymemory();
    fstream read;
    fstream write;
    
};

void Opsys :: displaymemory()
{
    cout<<"printing memory:"<<endl;
    for(auto x: M)
    {
        for(auto y : x)
            cout<<y;
        cout<<endl;
    }
}

void Opsys :: MOS()
{
    cout<<"in MOS......."<<endl;
    switch(SI)
    {
        case 1:
            READ();
            break;
        case 2:
            WRITE();
            break;
        case 3:
            TERMINATE();
            break;
    }
}

void Opsys :: INIT()
{
    cout<<"in INIT......."<<endl;
    M.resize(100 , vector<char>(4));
    for(int i = 0 ; i < 100 ; i++)
    {
        for( int j = 0 ; j<4 ; j++ )
        {
            M[i][j] = '\0';
        }
    }
    IC = 0;
    IR.resize(4);
    for( int i = 0 ; i<4 ; i++ ) IR[i] = '\0';
    R.resize(4);
    for( int i = 0 ; i<4 ; i++ ) R[i] = '\0';
    m = 0;
    C=false;
    counter = 0; 
}

void Opsys :: EXECUTEUSERPROGRAM()
{
    cout<<"in EXEcuteuserprogram......."<<endl;
    while(true)
    {
        for(int i = 0 ; i<4 ; i++ )
        {
            IR[i]=M[IC][i];
        }
        IC++;
        
        if( IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            MOS();
        }

        else if(IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();
        }

        else if(IR[0] == 'L' && IR[1] == 'R')
        {
            int index = (IR[2] - '0')*10 + (IR[3] - '0');

            for(int i = 0 ; i<4 ; i++)
            {
                R[i] = M[index][i];
            }
        }

        else if(IR[0] == 'S' && IR[1] == 'R')
        {
            int index = (IR[2] - '0')*10 + (IR[3] - '0');
            for(int i = 0 ; i<4 ; i++)
            {
                M[index][i] = R[i] ;
            }
            cout<<"After SR: "<<endl;
        }

        else if(IR[0] == 'C' && IR[1] == 'R')
        {
            int index = (IR[2] - '0')*10 + (IR[3] - '0');
            int count = 0 ;
            for(int i = 0 ; i<4 ; i++)
            {
                if( R[i] == M[index][i])
                {
                    count++;
                }
            }
            if(count == 4) C = true;
            else C = false; 
            cout<<"value of C is : "<<C<<endl;
        }

        else if(IR[0] == 'B' && IR[1] == 'T')
        {
            int index = (IR[2] - '0')*10 + (IR[3] - '0');
            if( C == true )
            {
                IC = index ;
            }
        }

        else if(IR[0] == 'H')
        {
            SI = 3;
            MOS();
            break;
        }
    }
}

void Opsys :: STARTEXECUTION()
{
    cout<<"in startexecution......."<<endl;
    IC = 0;
    EXECUTEUSERPROGRAM();  
}

void Opsys :: LOAD()
{
    cout<<"in LOAD......."<<endl;
    
    
    if( read.is_open() == 0 )
    {
        cout<<"File Not Opened Properly....."<<endl;
        return;
    }
    else
    {
        cout<<"Reading input.txt......."<<endl;
    }
     
    while(read.eof() == 0)
    {
        for(int i = 0 ; i<40 ; i++)
        {
            buffer[i] = '\0';
        }
       
        read.getline(buffer, 41);
    
        cout<<endl;
        if(buffer[0]=='$' && buffer[1]=='A' && buffer[2]=='M' && buffer[3]=='J')
        {
            cout<<"in AMJ ............"<<endl;
            INIT();
        }
        else if(buffer[0]=='$' && buffer[1]=='D' && buffer[2]=='T' && buffer[3]=='A')
        {
            cout<<"in DTA ............"<<endl;
            STARTEXECUTION();
        }
        else if(buffer[0]=='$' && buffer[1]=='E' && buffer[2]=='N' && buffer[3]=='D')
        {
            cout<<"in END.............."<<endl;
        }
        else
        {
            cout<<"reading instructions"<<endl;
            //putting instructions in memory--
            int k = 0;
            cout<<"printing buffer "<<buffer<<endl;
            for( int i = m ; i< m+10 ; i++ )
            {
                for( int j = 0 ; j<4 ; j++)
                {
                    
                    M[i][j] = buffer[k];
                    if(buffer[k] == 'H')
                    {
                        k++;
                        break;
                    }
                    k++;
                }
            }
            
            m += 10;
            displaymemory();
        }
    }
}

void Opsys :: READ()
{
    cout<<"in read......."<<endl;
    for(int i = 0 ; i<40 ; i++)
    {
        buffer[i] = '\0';
    }

    read.getline(buffer, 41);
    int blockptr = (IR[2]-'0')*10;
    int k = 0 ;
    cout<<"buffer after reading data is: "<<buffer<<endl;
    for( int i = 0 ; i<10 ; i++)
    {
        for(int j = 0 ; j<4 ; j++)
        {
            M[blockptr][j] = buffer[k];
            k++;
        }
        if(k == 40) break; //buffer exhausted;
        blockptr++;
    }
    cout<<"printing memory:"<<endl;
    for(auto x: M)
    {
        for(auto y : x)
            cout<<y;
        cout<<endl;
    }
    SI = 0 ;
}

void Opsys :: WRITE()
{
    
    cout<<"in write......."<<endl;
    for(int i = 0 ; i<40 ; i++)
    {
        buffer[i] = '\0';
    }

    int blockptr = (IR[2]-'0')*10;
    int k = 0 ;
   
    for( int i = 0 ; i<10 ; i++)
    {
        for(int j = 0 ; j<4 ; j++)
        {
            buffer[k] = M[blockptr][j] ;
            write<<buffer[k];
            k++;
        }
        
        if(k == 40) break; //buffer exhausted;
        blockptr++;
    }
    write<<"\n";

    cout<<"buffer for writing in output.txt is"<<buffer<<endl;
    SI = 0 ;
}

void Opsys :: TERMINATE()
{
    cout<<"in terminate......."<<endl;
    cout<<endl<<"Jobs Completed Successfully!!!"<<endl;
    SI = 0;
    string empty;
    write<<"\n";
    write<<"\n";
}

int main()
{ 
    Opsys os;
    os.read.open("input.txt",ios::in);
    os.write.open("output.txt" , ios :: app);
    
    os.LOAD();
    
    os.write.close();
    os.read.close();
    return 0;
}