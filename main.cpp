#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<limits>
#include<sstream>
#include<map>
#include<algorithm>
using namespace std;

vector<vector<int>> graph;
vector<string>pname;
void get_name();

// data structure for storing the debts that we obtain from minimising the cashflow
// first = debtor; second = creditor; amt = amount owed
struct cashf
{
    int first,second,amt;
};


//---------------------------------------------------------CLEAR DEBTS------------------------------------------------------------------

// class for knowing the maximum debt that can be cleared with a given amount
class personal{
    public:
        vector<pair<string,int>> debts;
        map<string,int> flow;
        int balance;

        // adding all the debts a person owes into the map 'flow' and then sorting them acc to priority
        void insert(int n){
            get_name();
            int flag=0,count=0;
            for(int i=0;i<n;i++){
                flag=0;
                cout<<"\t\t\t\t"<<pname[i]<<"'s Account"<<endl<<endl;
                for(int j=0;j<n;j++){
                    if(graph[i][j]){
                        flag=1;
                        flow.insert(make_pair(pname[j],graph[i][j]));
                        cout<<"\t\t\t\t   "<<pname[j]<<": "<<graph[i][j]<<endl;
                        count++;
                    }
                }
                if(flag==0){
                    cout<<"No debts present"<<endl;
                    string p;
                    cout<<endl<<"Enter any key to continue"<<endl;
                    cin>>p;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(),'\n');
                    system("cls");
                    continue;
                }
                cout<<endl;
                priority(count);
                cout<<"Enter the balance you have left"<<endl;
                cin>>balance;
                disp();
                cout<<endl;
                flow.clear();
                debts.clear();
            }
        }

        void priority(int count){
            int i=0;
            char pr;
            cout<<"Do you want to set priority?"<<endl;
            cin>>pr;
            if(pr=='y'){
                int n=0;
                string name;
                while(true){
                    cout<<"For how many people?"<<endl;
                    cin>>n;
                    if(cin.fail() || n>count){
                        cout<<"Not a valid number"<<endl<<endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        continue;
                    }
                    break;
                }
                cout<<"Enter priority: "<<endl;
                for(i=0;i<n;i++){
                    while(true){
                        cin>>name;
                        if(flow.find(name)==flow.end()){
                            cout<<"No such person exists in the group. Enter again"<<endl;
                            continue;
                        }
                        break;
                    }
                    debts.push_back(make_pair(name,flow[name]));
                    flow.erase(name);
                }
            }
            for(auto i:flow){
                debts.push_back(i);
            }

            quick_sort(debts,i,debts.size()-1);
        }

        void quick_sort(vector<pair<string,int>> &debts,int start,int end){
            if(start>=end) return;
            int pi=start+(rand()%(end-start+1));
            auto pivot=debts[pi];

            debts[pi]=debts[end];
            debts[end]=pivot;
            
            pi=start;
            for(int i=start;i<end;i++){
                if(pivot.second>debts[i].second){
                    auto temp=debts[pi];
                    debts[pi]=debts[i];
                    debts[i]=temp;
                    pi++;
                }
            }

            debts[end]=debts[pi];
            debts[pi]=pivot;
            quick_sort(debts,start,pi-1);
            quick_sort(debts,pi+1,end);
        }

        void disp(){
            cout<<endl;
            if(balance < debts[0].second){
                cout<<"No debt can be cleared"<<endl;
            }
            else{
                int i=0,flag=0;
                cout<<"To clear the maximum debt: "<<endl;
                while(balance>=0 && i<debts.size()){
                    flag= 0;
                    if(debts[i].second<=balance){
                        flag=1;
                        balance=balance-debts[i].second;
                        cout<<" "<<debts[i].first<<": "<<debts[i].second<<endl;
                        i++;
                    }
                    if(flag==0){
                        int p=0;
                        cout<<endl<<"Enter any key to continue"<<endl;
                        cin>>p;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(),'\n');
                        system("cls");
                        return;
                    } 
                }
            }

            int p=0;
            cout<<endl<<"Enter any key to continue"<<endl;
            cin>>p;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            system("cls");
        }
};
personal x;

// getting the names of all the people in the group
void get_name(){
    ifstream file;
    file.open("people.txt");
    string str;
    while(!file.eof()){
        file>>str;
        pname.push_back(str);
        getline(file,str);
    }
    file.close();
}


//----------------------------------------------------- ADD A PERSON ----------------------------------------------------

// updating the number of people owed to while adding a new person; each time a new person is added, the number of people owed to increases by 1
void update(){
    fstream file;
    file.open("people.txt",ios::in);
    string str,detail="";
    while(!file.eof()){
        getline(file,str);
        detail+=str + "0 \n";
    }
    file.close();

    file.open("people.txt",ios::out);
    file<<detail;
    file.close();
}


void add_person(){
    ofstream file;
    file.open("people.txt",ios::app);
    if(file.is_open()){
        string name;
        cout<<"Enter the name of the person: ";
        getline(cin,name);
        file<<name<<" ";
        int num;
        fstream numfile("num.txt",ios::in);
        numfile>>num;
        numfile.close();
        for(int i=0;i<num;i++){
            file<<"0 ";
        }
        file.close();
        update();
        num++;
        numfile.open("num.txt",ios::out);
        numfile<<num;
        numfile.close();
    }

}


//------------------------------------------------------ LISTING PEOPLE AND DEBT ------------------------------------------
void disp_file(){
    ifstream file("people.txt");
    string str;
    cout<<"Listing"<<endl;
    while(!file.eof()){
        getline(file,str);
        cout<<str<<endl;

    }
}


//--------------------------------------------------- UPDATE EXISTING BALANCE -------------------------------------

// getting the index of the creditor and debtors from the file people.txt
int* get_index(string debtor,string creditor){
    int *index=new int[2],i=1,flag=0;
    index[0]=index[1]=0;
    ifstream file;
    file.open("people.txt");
    string str;
    while(!file.eof()){
        file>>str;
        if(str==debtor){
            flag++;
            index[0]=i;
        }
        else if(str==creditor){
            flag++;
            index[1]=i;
        }
        if(flag==2){
            break;
        }
        getline(file,str);
        i++;
    }
    if(flag!=2){
        cout<<"Enter valid names"<<endl;
    }
    file.close();
    return index;
}

// for updating debts owed, in the file people.txt, as entered by the user
void add_amount(){
    string debtor,creditor;
    int amount;
    int* index;
    while(true){
        cout<<"Enter debtor(giver): ";
        getline(cin,debtor);
        cout<<"Enter creditor(receiver): ";
        getline(cin,creditor);
        if(debtor==creditor){
            cout<<"Both can't be the same"<<endl;
            continue;
        }
        index = get_index(debtor,creditor);
        if(index[0]==0 || index[1]==0){
            continue;
        }
        break;
    }

    while(true){
        cout<<"Enter the amount: ";
        cin>>amount;
        if(cin.fail()){
            cout<<"Not a valid number"<<endl<<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(),'\n');
            continue;
        }
        break;
    }

    string str,sp,detail="";
    int i=1;
    fstream file;
    file.open("people.txt",ios::in);
    while(!file.eof()){
        if(i==index[0]){
            for(int j=1;j<=index[1];j++){
                file>>str;
                detail+=str+" ";
            }
            file>>str;
            amount+=stoi(str);
            stringstream ss;
            ss<<amount;
            string amt;
            ss>>amt;
            detail+=amt;
            getline(file,str);
            detail+=str+"\n";
        }
        else{
            getline(file,str);
            if(str.empty()){
                break;
            }
            detail+=str+"\n";
        }
        i++;
    }
    file.close();

    file.open("people.txt",ios::out);
    file<<detail;
    file.close();
}


//---------------------------------------------------- MINIMISE CASHFLOW --------------------------------------------------

// making the graph(adjacency matrix) from the values in the file people.txt
int get_graph(){
    ifstream file("people.txt");
    string str;
    vector<int> val;
    int value;
    while(!file.eof()){
        file>>str;
        if(str.empty()) continue;
        getline(file,str);
        stringstream ss(str);
        while(ss>>value){
            val.push_back(value);
        }
        graph.push_back(val);
        val.clear();
        str.clear();

    }
    file.close();
}


int max(int amount[],int n){
    int max=0;
    for(int i=1;i<n;i++){
        if(amount[max]<amount[i]){
            max=i;
        }
    }
    return max;
}

int min(int amount[],int n){
    int min=0;
    for(int i=1;i<n;i++){
        if(amount[min]>amount[i]){
            min=i;
        }
    }
    return min;
}

void min_cashflow(auto &min_index)
{
    int n = graph.size();
    int amount[n] = {0};

    // getting the net amount each person owes
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            amount[i] += (graph[j][i] - graph[i][j]);
        }
    }

    get_name();
    while(1){
        int cindex = max(amount,n);
        int dindex = min(amount,n);
        if(amount[cindex]==0 && amount[dindex]==0){
            cout<<endl<<"Cashflow has been minimised"<<endl;
            break;
        }
        int min_amt = (amount[cindex]<(-amount[dindex])) ? amount[cindex] : -amount[dindex];
        amount[cindex] -= min_amt;
        amount[dindex] += min_amt;
        cout<<pname[dindex]<<" owes "<<min_amt<<" to "<<pname[cindex]<<endl;

        min_index.push_back({dindex,cindex,min_amt});
    }
}

int comp(cashf a,cashf b){
    if(a.first==b.first) return (a.second)<(b.second);
    return (a.first)<(b.first);
}

// updating the graph file with the updated debts after minimising the cashflow
void update_graph(vector<cashf>min_index){
    sort(min_index.begin(),min_index.end(),comp);

    int k=0;
    for(int i=0;i<graph.size();i++){
        for(int j=0;j<graph[i].size();j++){
            if(i==min_index[k].first && j==min_index[k].second){
                graph[i][j]=min_index[k].amt;
                k++;
            }
            else{
                graph[i][j]=0;
            }
        }
    }
}

// updating the people.txt file with the updated debts after minimising the cashflow
void update_file(){
    get_name();
    fstream file;
    file.open("people.txt",ios::in);
    string info="",str;
    stringstream ss;
    for(int i=0;i<graph.size();i++){
        info+=pname[i]+" ";
        for(int j=0;j<graph[i].size();j++){
            ss<<graph[i][j]<<" ";
            ss>>str;
            info+=str+" ";
        }
        info+="\n";
        ss.clear();
    }
    file.close();
    file.open("people.txt",ios::out);
    file<<info;
    file.close();
}

// min_index stores the indexes of debtors and creditors after minimising the cashflow so that the graph can be updated from the new values
void util_cashflow(){
    vector<cashf> min_index;
    min_cashflow(min_index);

    update_graph(min_index);
    update_file();
}


//--------------------------------------------------------------------------------------------------------------------
void menu(){

    while (true) {
        cout << "\n\n\t\t\tGroup Formation" << endl;
        cout << "\t\t\t   1. Add a person" << endl;
        cout << "\t\t\t   2. Update existing balance" <<endl;
        cout << "\t\t\t   3. List all the people and debts" << endl<<endl<<endl;

        cout<<"\t\t\tFeatures"<<endl;
        cout<<"\t\t\t   4. Minimise Cashflow"<<endl;
        cout<<"\t\t\t   5. Clear Debts"<<endl;
        cout<<"\t\t\t   6. Exit" << endl<<endl;
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        system("cls");

        switch (choice) {
            case 1:
                add_person();
                break;
            case 2:
                add_amount();
                break;
            case 3:
                disp_file();
                break;

            case 4:
                if(graph.size()==0) get_graph();
                util_cashflow();
                break;

            case 5:
                if(graph.size()==0) get_graph();
                x.insert(graph.size());
                break;

            case 6:
                return;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
        int n=0;
        cout<<endl<<"Enter a key to go back to the main menu"<<endl;
        cin>>n;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        system("cls");
    }
}

int main(){
    ifstream numfile;
    numfile.open("num.txt");
    if(!numfile){
        numfile.close();
        int n=0;
        ofstream numfile("num.txt");
        numfile<<n;
    }
    numfile.close();
    
    menu();
    return 0;
}