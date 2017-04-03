#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <fstream>

using namespace std;

class card
{
    static int nr;
    int val;//0,1,2,3=as 4,5,6,7=2 ... 48,49,50,51
public:
    void getnrobiecte(){cout<<nr;};
    card(int i=0){nr++;val=i;};
    void setval(int i){val=i;};
    int getval(){return val;};
    bool operator == (card x){if (x.val/4==val/4 || x.val%4==val%4) return true;return false;};
    friend ostream & operator << (ostream &,card);
};

int card::nr;

ostream & operator << (ostream & out, card x)
{
    int nr=x.val/4+1,simbol=x.val%4+3;
    if (nr>=11) nr++;
    if (nr==1) out<<"AS"<<(char)simbol<<"\n";
    else if (nr==12) out<<"J"<<(char)simbol<<"\n";
    else if (nr==13) out<<"Q"<<(char)simbol<<"\n";
    else if (nr==14) out<<"K"<<(char)simbol<<"\n";
    else out<<nr<<(char)simbol<<"\n";
    return out;
}

class card_package
{
    card v[52];
public:
    int nr(){int nr=0;for (int i=0;i<52;i++) if (v[i].getval()!=-1) nr++; return nr;}
    bool empty(){if (nr()==0) return true; return false;};
    card_package(){for (int i=0;i<52;i++) v[i].setval(i);};
    card get_card(){srand (time(NULL));int x;do{x=rand()%52;}while(v[x].getval()==-1);card j=v[x];v[x].setval(-1);return j;};
    void add_card(card x){v[x.getval()]=x;};
};

class mana
{
    card *v;
    int n;
public:
    mana(int i=0){v=new card[i];n=i;};
    void get(card_package &x){for (int i=0;i<n;i++) v[i]=x.get_card();}
    card draw(int i){card x=v[i];for (int j=i;j<n;j++) v[j]=v[j+1];n--;delete (v+n);return x;};
    void pick(card_package &);
    void pick(card);
    bool find(int val){for (int i=0;i<n;i++) if (v[i].getval()/4+1==val) return true;return false;};
    int size(){return n;};
    void add_to_package(card_package &x){while (n>1) { card j=draw(0); x.add_card(j);}};
    friend ostream & operator << (ostream &,mana);
};

void mana::pick(card x)
{
    card *cop=new card[n];
    for (int i=0;i<n;i++) cop[i]=v[i];
    n++;
    v=new card[n];
    for (int i=0;i<n-1;i++) v[i]=cop[i];
    v[n-1]=x;
}

void mana::pick(card_package &x)
{
    card *cop=new card[n];
    for (int i=0;i<n;i++) cop[i]=v[i];
    n++;
    v=new card[n];
    for (int i=0;i<n-1;i++) v[i]=cop[i];
    v[n-1]=x.get_card();
}

ostream & operator << (ostream &out,mana x)
{
    for (int i=0;i<x.n;i++) out<<"Cartea "<<i<<"="<<x.v[i];
    return out;
}

class player
{
    mana m;
    int waiting;
public:
    player(card_package &x):m(5){waiting=0;m.get(x);}
    bool empty(){if (m.size()==0) return true;return false;};
    void afisare(){if (waiting>0) cout<<"You have to wait "<<waiting<<" turns!";else cout<<m;};
    void play(card_package &x,mana &y,int &,int&);
};
//situatie-1-umflaturi 2-stai ture
void player::play(card_package &x,mana &y,int &simbol,int &actiune)
{
    if (waiting==0)
    {
    card aux;
    int situatie=0,s=0;
    if (actiune==1)
    {
    mana auxx;
    aux=y.draw(y.size()-1);
    auxx.pick(aux);
    /* Cercetare daca jucatorul trebuie sa stea ture sau sa umfle carti */
    if (aux.getval()/4+1==2 || aux.getval()/4+1==3)
    {
        situatie=1;
        s=aux.getval()/4+1;
        card aux2;
        do
        {
            aux2=y.draw(y.size()-1);
            if (aux2.getval()/4+1==2 || aux2.getval()/4+1==3) s+=aux2.getval()/4+1;
            auxx.pick(aux2);
        }while (aux2.getval()/4+1==2 || aux2.getval()/4+1==3 || y.size()>0);
    }
    if (aux.getval()/4+1==4)
    {
        situatie=2;
        s=1;
        card aux2;
        do
        {
            aux2=y.draw(y.size()-1);
            if (aux2.getval()/4+1==4) s++;
            auxx.pick(aux2);
        }while (aux2.getval()/4+1==4 || y.size()>0);
    }
    /*Refacere carti de pe tabla*/
    while (auxx.size()>0)
    {
        aux=auxx.draw(auxx.size()-1);
        y.pick(aux);
    }
    }
    int nr=0,u;
    /* Randul jucatorului */
    do
    {
        afisare();
        cout<<"Jos:\n";
        cout<<y;
        cout<<"Simbol necesar:"<<(char)(simbol+3)<<"\n";
        cout<<"Draw a card(enter the card id) or stop enter -1";
        cin>>u;
        if (u==-1)
        {
            /* cazul in care jucatorul nu a dat nimic-se verifica cate carti trebuie sa traga */
            if (nr==0)
            {
                if (situatie==1)
                {
                    while (s>0 && !x.empty())
                    {
                        s--;
                        m.pick(x);
                    }
                    if (x.empty() && s>0)
                    {
                        y.add_to_package(x);
                        while (s>0)
                        {
                            s--;
                            m.pick(x);
                        }
                    }
                    actiune=0;
                }
                    else if (situatie==2)
                    {
                        actiune=0;
                        waiting=s-1;
                    }
                        else m.pick(x);
            }
        }
            else if (u<0 || u>=m.size()) cout<<"Indice gresit!\n"; //daca u nu se afla in intervalul de carti pe care il detine jucatorul
                else
                {
                    //cazul in care jucatorul a ales o carte testeaza daca cartea este acceptabila
                    int copiee=nr;
                    aux=m.draw(u);
                    if (situatie==1 && (aux.getval()/4+1==2 || aux.getval()/4+1==3))
                    {
                        nr++;
                        simbol=aux.getval()%4;
                    }
                     else if (situatie==2 && aux.getval()/4+1==4)
                     {
                         nr++;
                         simbol=aux.getval()%4;
                     }
                      else if (situatie==0 && nr>0)
                      {
                          card aux3=y.draw(y.size()-1);
                          y.pick(aux3);
                          if (aux.getval()/4+1==aux3.getval()/4+1)
                          {
                                if (aux.getval()/4+1==1)
                                {
                                    cout<<"In ce schimbati?(0-"<<(char)3<<"1-"<<char(4)<<"2-"<<char(5)<<"3-"<<char(6)<<")\n";
                                    cin>>simbol;
                                    nr++;
                                }
                                else
                                {
                                    nr++;
                                    simbol=aux.getval()%4;
                                }
                          }
                      }
                        else if (situatie==0 && aux.getval()/4+1==1)
                        {
                            cout<<"In ce schimbati?(0-"<<(char)3<<"1-"<<char(4)<<"2-"<<char(5)<<"3-"<<char(6)<<")\n";
                            cin>>simbol;
                            nr++;
                        }
                        else if (situatie==0)
                        {
                            card aux3=y.draw(y.size()-1);
                            y.pick(aux3);
                            if (aux.getval()/4+1==aux3.getval()/4+1 || aux.getval()%4==simbol)
                            {
                                nr++;
                                simbol=aux.getval()%4;
                            }
                        }
                    if (nr==copiee)
                    {
                        cout<<"Actiune nepermisa!\n";
                        m.pick(aux);
                    }
                        else
                        {
                            actiune=1;
                            y.pick(aux);
                        }
                }
    } while (u!=-1);
    cout<<"Tura terminata!\n";
    /*aux=y.draw(y.size()-1);
    y.pick(aux);
    simbol=aux.getval()%4;
    */
    }
    else
    {
        waiting--;
        cout<<"Waiting for "<<waiting<<" more turns!";
    }
}
