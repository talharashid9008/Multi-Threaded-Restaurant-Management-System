#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<sys/wait.h>
#include<vector>
#include<pthread.h>
using namespace std;
double totalsales=0 ;
double waitersale=0;
struct Item{
public:
    int time;
    double price;
    string name;
    Item(){

        time=0;
        price=0;
        name="";
       
    }

};
struct order{
    public:
    int cid;
    vector<Item> dishes;

    order()
    {
        cid=-1;
    }
    order( int c ,vector<Item> d)
    {
        cid=c;
        dishes=d;
    }
    double get_price(){
        double t=0 ;
        for(int i=0 ; i<dishes.size() ; i++){
            t+=dishes[i].price;
        }
        return t ;
    }
};

struct waiter{
    int wid ;
    string wname ;
    double sales ;
    waiter(){
        wid=-1 ;
        wname = "" ;
        sales=0 ;
    }
    waiter(int id , string n){
        wid=id ;
        wname = n ;
        sales=0 ;
    }

};
struct manager{
    public:
    int mid ;
    string mname ;
    vector<order> O;
    manager(){
        mid=123 ;
        mname="Walter White";
    }
    manager(int i, string n){
        mid=i ;
        mname=n;
    }
    void take(order o){
        O.push_back(o);
    }
};
class Customer{

public:
    string name;
    int id;

    Customer(string n,int ID){

        name =n;
        id=ID;
    }
    Customer(){

        name="";
        id=0;
    }

};
class Menu{

public:
    Item num[5];

    Menu(){

        num[0].name="Burger";num[0].price=210;num[0].time=5;
        num[1].name="Pizza";num[1].price=1200;num[1].time=10;
        num[2].name="Sandwich";num[2].price=180;num[2].time=2;
        num[3].name="Fries";num[3].price=140;num[3].time=3;
        num[4].name="Pasta";num[4].price=130;num[4].time=2;

    }
    void displayMenu(){
        cout <<"                     Menu                               " << endl;
        cout<<"Item******************Price*****************Delivery Time"<<endl;
        cout<<"****************"<<endl;
        for(int i =0;i<5;i++){

            cout<<i<<"- "<<num[i].name<<"          "<<num[i].price<<"                    "<<num[i].time<<endl;
        }
    }
};
void *waiter(void *arg)
{
	order o=*(order*) arg;
	cout<<"waiter:\n";
	cout<<"Customer with id "<<o.cid <<" can take his order!!\n";
	waitersale+=o.get_price();
	cout<<"Daily Waiter Sales="<<waitersale<<endl;	
pthread_exit(NULL);
}
void *cook(void *arg)
{
	order o =*(order*)arg;
    order odr = o ;
    cout << "Cooking Process of Customer# " << o.cid << " in Progress\n" ;
    for(int i=0 ; i<o.dishes.size();i++){
        cout << "*************\n" ;
       cout << "Cooking : " <<  o.dishes[i].name << "       ->(PLease wait!  It will take  " << o.dishes[i].time << " ...seconds...to cook)<-\n";
       cout << "*************\n" ;
       sleep(o.dishes[i].time);
       cout << o.dishes[i].name << "Cooked!" << endl ;
    }
	pthread_t thread1;
	pthread_create(&thread1,NULL,waiter,(void*)&odr);
	pthread_exit(NULL);
}
int main(){
    char buffer[30];
    int fd[2],fd1[2];
    int temp[6][2];
    pipe(fd);
    pipe(fd1);
    int pid=fork();
    int s=0 ;
    if(pid>0){
        close(fd[1]);//close write end.
        Customer c1 ;
        manager m ;
        char buffer2[20];
        string name="" ;
        string str = "WELCOME!!!\nI am Manager : " + m.mname + "  with id : " +  to_string(m.mid) + "\nPlease provide the requested information\n";
        wait(NULL);
        read(fd[0],buffer2,sizeof(buffer2));
        for(int i=0 , j=0 ;i<sizeof(buffer2);i++ ){
            if(buffer2[i]=='_' && j==0){
                c1.id=stoi(name);
                name="" ;
                j++;
            }
            else if(buffer2[i]=='_' && j==1){
                c1.name=name ;
                break;
            }
            else{
                name=name+buffer2[i];
            }
        }
        Menu m1 ;
        int pid2=fork();

        if(pid2>0){
            m1.displayMenu();
            char buffer4[20];
            int ch ;
            wait(NULL);
            int s1 ;
            string str = "" ;
            read(fd1[0],buffer4,sizeof(buffer4));
            vector<Item> d;
            for(int i=0 ;i<sizeof(buffer4);i++ ){
                    if(buffer4[i]=='_'){
                        break;
                    }
                    else if(buffer4[i]==','){
                        s1=stoi(str);
                        d.push_back(m1.num[s1]);
                        str="";
                    }
                    else{
                        str=str+buffer4[i];
                    }
            }
            order o(c1.id,d);
            cout<<"Thank you for Ordering Sir!\n" ;
            cout << "Your bill is RS: ";
            cout << o.get_price() << ".\n";
            totalsales+=o.get_price();
            pthread_t c_thread;
            pthread_create(&c_thread,NULL,cook,(void*)&o);
            pthread_join(c_thread, NULL);
            pthread_exit(NULL);

        }
        else if(pid2==0){
            char buffer3[20];
            int choice ;
            string ch="" ;
            do{
                    cout << "*****************************************************\n";
                    cout<<"Press 0 to Purchase Burger\nPress 1 to Purchase Pizza\nPress 2 to Purchase Sandwich\nPress 3 to Purchase Fries\nPress 4 to Purchase Pasta\nPress 5 to Complete!!!\n"<<endl;
                    do{
                        cin >> choice ;
                    }while(choice<0 || choice>5);
                    if(choice!=5){
                       ch=ch+to_string(choice)+",";
                    }
            }while(choice!=5);
            ch=ch+"_";
            for(int i=0 ; i<ch.length() ; i++){
                buffer3[i]=ch[i];
            }
            write(fd1[1],buffer3,ch.length());
            exit(0);
        }
        
       
    }
    else if (pid==0){
        close(fd[0]);//close read end.
        char buffer1[20];
        cout<<"Name: ";
        string n;
        int idx = 0;
        getline(cin,n);
        cout<<"ID: ";
        cin>>idx;
        n=to_string(idx)+"_"+n+"_";
        s=n.length();
        for(int i=0 ; i<n.length() ; i++){
                buffer1[i]=n[i];
        }
        //Customer guest(n,idx);
        write(fd[1],buffer1,n.length());
        exit(0);
       
    }
}