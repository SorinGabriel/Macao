#include <iostream>
#include "card.h"

using namespace std;

int menu()
{
    int opt;
    cout<<"1.Alege o carte(0=Prima carte de sus,1=A doua...)\n2.Trage o carte\n";
    cin>>opt;
    return opt;
}

int main()
{
    card_package x;
    mana m5(1);//,m2(5),m3(5),m4(5),m5(1);
    m5.get(x);
    player unu(x),doi(x),trei(x);
    int simbol,actiune=1;
    card aux=m5.draw(m5.size()-1);
    m5.pick(aux);
    simbol=aux.getval()%4;
    do
    {
        cout<<"Numar carti pachet:"<<x.nr()<<"\n";
        cout<<"PLAYER 1:\n";
        unu.play(x,m5,simbol,actiune);
        if (unu.empty() || doi.empty() || trei.empty()) break;
        if (m5.size()>8 || x.empty()) m5.add_to_package(x);
        cout<<"Numar carti pachet:"<<x.nr()<<"\n";
        cout<<"PLAYER 2:\n";
        doi.play(x,m5,simbol,actiune);
        if (unu.empty() || doi.empty() || trei.empty()) break;
        if (m5.size()>8 || x.empty()) m5.add_to_package(x);
        cout<<"Numar carti pachet:"<<x.nr()<<"\n";
        cout<<"PLAYER 3:\n";
        trei.play(x,m5,simbol,actiune);
        if (unu.empty() || doi.empty() || trei.empty()) break;
        if (m5.size()>8 || x.empty()) m5.add_to_package(x);
    } while (!unu.empty() && !doi.empty() && !trei.empty());
    if (unu.empty()) cout<<"Player 1 win!";
    if (doi.empty()) cout<<"Player 2 win!";
    if (trei.empty()) cout<<"Player 3 win!";
    return 0;
}
