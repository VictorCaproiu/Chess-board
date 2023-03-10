#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

struct piece{ //structura pieselor de sah ce vor fi stocate in matrice
    char symbol; //caracteristicile piesei de sah
    int color;
};

string players[2] = {"White","Black"};

piece board[8][8]; //tabla de sah

void display_board(int player_nr,int turn){ //afiseaza tabla de joc
    HANDLE std_output = GetStdHandle(STD_OUTPUT_HANDLE);

    cout<<"\n\n\n\n\n";
    SetConsoleTextAttribute(std_output,63);
    cout<<"Player:"<<players[player_nr]<<" Turn:"<<turn<<"\n";  //afiseaza tabla de sah in starea sa curenta

    for(int i=0;i<8;i++){
        SetConsoleTextAttribute(std_output,63);
        cout<<8-i<<" ";
        for(int j=0;j<8;j++){
            SetConsoleTextAttribute(std_output,board[i][j].color);
            cout<<board[i][j].symbol<<" ";
        }
        SetConsoleTextAttribute(std_output,63);
        cout<<"\n";
    }

    SetConsoleTextAttribute(std_output,63);
    cout<<"  a b c d e f g h\n";
    cout<<"\n\n\n\n\n\n";
}
//-------------------------------------------------------------------------------------------------------------------------------------------
void help(){  //informatii pentru utilizator
    cout<<"Chess pieces:\n";
    cout<<"p = pawn\n";
    cout<<"R = rook\n";
    cout<<"h = knight\n";   //simbolurile pieselor de sah
    cout<<"B = bishop\n";
    cout<<"Q = queen\n";
    cout<<"K = king\n";

}
//---------------------------------------------------------------------------------------------------------------------------------------------
void help_interface(){  //informatii pentru utilizator
    cout<<"Controls:\n";
    cout<<"1-New game\n";
    cout<<"2-Help\n";
    cout<<"3-Select piece\n";  //interfata utilizatorului
    cout<<"9-Exit\n";
}
//------------------------------------------------------------------------------------------------------------------------------------------
int mod(int x){   //functie pentru modulul unui numar
    if(x<0)
        x=x*-1;
    return x;
}
//------------------------------------------------------------------------------------------------------------------------------------------
void reset(){   //pregateste tabla pentru un joc nou

    board[0][0].symbol='R';board[0][7].symbol='R';
    board[0][1].symbol='h';board[0][6].symbol='h';  //muta toate piesele in pozitia de start
    board[0][2].symbol='B';board[0][5].symbol='B';
    board[0][3].symbol='K';board[0][4].symbol='Q';
    for(int i=0;i<=7;i++)
        board[1][i].symbol='p';
//----------------------------------------------------
    for(int i=0;i<2;i++){
        for(int j=0;j<8;j++){
            if((i+j)%2==0)
                board[i][j].color=112; //culoarea pieselor negre
            else
                board[i][j].color=128;
        }
    }
//----------------------------------------------------
    for(int i=6;i<8;i++){
        for(int j=0;j<8;j++){
            if((i+j)%2==0)
                board[i][j].color=127;  //culoarea pieselor albe
            else
                board[i][j].color=143;
        }
    }
//----------------------------------------------------
    for(int i=2;i<6;i++)
        for(int j=0;j<8;j++){
            if((i+j)%2==0){
                board[i][j].color=119;
                board[i][j].symbol='O';  //simbolul si culoarea patratelor libere
            }
            else{
                board[i][j].color=136;
                board[i][j].symbol='O';
            }
        }
//-------------------------------------------------------

    board[7][0].symbol='R';board[7][7].symbol='R';
    board[7][1].symbol='h';board[7][6].symbol='h'; //piesele albe
    board[7][2].symbol='B';board[7][5].symbol='B';
    board[7][3].symbol='K';board[7][4].symbol='Q';
    for(int i=0;i<=7;i++)
        board[6][i].symbol='p';

}
//---------------------------------------------------------------------------------------------------------------------------------------
int select_piece(int player_nr){  //asigura ca jucatorul selegteaza o piesa ce ii apartine
    cout<<"Select piece:\n";
    int row,col,invalid_piece=0;
    char col_char;

    do{
        do{
            cout<<"row:";
            cin>>row;           //input sanitation necesar AICI <~~~~~~~~~
        }while(row<1 || row>8);
        row=8-row;


        do{
            cout<<"column:";
            cin>>col_char;     //ne asiguram ca inputul sa fie corect
        }while(col_char!='a' && col_char!='b' && col_char!='c' && col_char!='d' && col_char!='e' && col_char!='f' && col_char!='g' && col_char!='h');

        switch(col_char){
            case 'a':
                col=0;
                break;
            case 'b':
                col=1;
                break;
            case 'c':
                col=2;
                break;
            case 'd':
                col=3;
                break;   //transforma litera coloanei in int
            case 'e':
                col=4;
                break;
            case 'f':
                col=5;
                break;
            case 'g':
                col=6;
                break;
            case 'h':
                col=7;
                break;
            default:
                cout<<"Invalid piece!\n";
        }

        invalid_piece=0;
        if(player_nr==0 && board[row][col].color!=127 && board[row][col].color!=143){
            cout<<"Invalid piece!\n";  invalid_piece=1;
        }

        if(player_nr==1 && board[row][col].color!=112 && board[row][col].color!=128){
            cout<<"Invalid piece!\n";  invalid_piece=1;
        }

    }while(invalid_piece==1);
//-------------------------------------------------------------------------
    switch(board[row][col].color){
        case 112:
            board[row][col].color=192;
            break;

        case 128:
            board[row][col].color=192;    //evidentiaza cu rosu piesa selectata
            break;

        case 127:
            board[row][col].color=207;
            break;

        case 143:
            board[row][col].color=207;
            break;
    }

    return row*10+col;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------
void move_piece(int row,int col,int row2,int col2){    //asigura schimbul adecvat de culoare a piesei, a fundalului piesei si de simbol in urma mutarii (mare durere de cap)

    if(board[row2][col2].color==192){ //piesa mutata e neagra
        if(board[row][col].color==127 || board[row][col].color==119){//zona pe care e mutata e gri deschis
            board[row][col].symbol=board[row2][col2].symbol;//piesa e mutata
            board[row2][col2].symbol='O';//plecarea e curatata de simbol
            board[row][col].color=112;
            if((row2+col2)%2==0)//fundalu pornire e dechis
                board[row2][col2].color=119;
            else                 //fundal pornire e inchis
                board[row2][col2].color=136;
        }
        if(board[row][col].color==143 || board[row][col].color==136){//zona pe care e mutata e gri inchis
            board[row][col].symbol=board[row2][col2].symbol;
            board[row2][col2].symbol='O';
            board[row][col].color=128;
            if((row2+col2)%2==0)//fundalu pornire e dechis
                board[row2][col2].color=119;
            else               //fundal pornire e inchis
                board[row2][col2].color=136;
        }

    }
    else{                                                            //piesa mutata e alba
        if(board[row][col].color==112 || board[row][col].color==119){//zona pe care e mutata e gri deschis
            board[row][col].symbol=board[row2][col2].symbol;
            board[row2][col2].symbol='O';
            board[row][col].color=127;
            if((row2+col2)%2==0)//fundalu pornire e dechis
                board[row2][col2].color=119;
            else                //fundal pornire e inchis
                board[row2][col2].color=136;
        }
        if(board[row][col].color==128 || board[row][col].color==136){//zona pe care e mutata e gri inchis
            board[row][col].symbol=board[row2][col2].symbol;
            board[row2][col2].symbol='O';
            board[row][col].color=143;
            if((row2+col2)%2==0)//fundal pornire e dechis
                board[row2][col2].color=119;
            else   //fundal pornire e inchis
                board[row2][col2].color=136;
        }
    }
    cout<<board[row2][col2].color<<" "<<board[row][col].color<<" "<<board[row2][col2].symbol<<" "<<board[row][col].symbol;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
void move_piece_check(int x){  //primeste informatii despre mutare si verifica daca este legala

    int row2,col2,row,col;
    bool invalid_move;
    char col_char;
    col2=x%10;
    row2=x/10;

    cout<<"Select destination:\n";

    do{
        invalid_move=0;
        do{
            cout<<"row:";
            cin>>row;           //input sanitation necesar AICI <~~~~~~~~~
        }while(row<1 || row>8);
        row=8-row;


        do{
            cout<<"column:";
            cin>>col_char;     //ne asiguram ca inputul sa fie corect
        }while(col_char!='a' && col_char!='b' && col_char!='c' && col_char!='d' && col_char!='e' && col_char!='f' && col_char!='g' && col_char!='h');

        switch(col_char){
            case 'a':
                col=0;
                break;
            case 'b':
                col=1;
                break;
            case 'c':
                col=2;
                break;
            case 'd':
                col=3;
                break;   //transforma litera coloanei in int
            case 'e':
                col=4;
                break;
            case 'f':
                col=5;
                break;
            case 'g':
                col=6;
                break;
            case 'h':
                col=7;
                break;
            default:
                cout<<"Invalid destination!\n";
        }

//----------------------------------------------------------------------------------------------------------------------------------------------------
        if(board[row2][col2].color==207){ //piesa alba

            switch(board[row2][col2].symbol){
                case 'p':

                    if(board[row][col].symbol=='O' && row==row2-1 && col2==col){
                        move_piece(row,col,row2,col2);             //mutare in fata
                        break;
                    }

                    if(board[row][col].symbol!='K' && (board[row][col].color==112 || board[row][col].color==128) && (col2==col+1 || col2==col-1) &&(row==row2+1)){
                        move_piece(row,col,row2,col2);           //atac pe diagonala
                        break;
                    }
                    else{
                        invalid_move=1;
                        cout<<"Invalid move!\n";
                        break;
                    }
//---------------------------------------------------------------------------------------------------------------------------------------------------------
                case 'R':
                    if((col==col2 && row==row2) || !(col==col2 || row==row2)){
                        invalid_move=1;
                        cout<<"Invalid move!\n";     //ne asiguram ca mutarea e pe orizontala sau verticala
                        break;
                    }
                    if(col==col2)
                        if(row<row2)
                            for(int i=row;i<row2;i++)
                                if(board[i][col].symbol!='O'){
                                    invalid_move=1;
                                    cout<<"Invalid move!\n";
                                    break;
                                }
                        else
                            for(int i=row;i>row2;i--)
                                if(board[i][col].symbol!='O'){
                                    invalid_move=1;
                                    cout<<"Invalid move!\n";
                                    break;
                                }                               //ne asiguram ca drumul e liber
                    if(row==row2)
                        if(col<col2)
                            for(int i=col;i<col2;i++)
                                if(board[i][col].symbol!='O'){
                                    invalid_move=1;
                                    cout<<"Invalid move!\n";
                                    break;
                                }
                        else
                            for(int i=col;i>col2;i--)
                                if(board[i][col].symbol!='O'){
                                    invalid_move=1;
                                    cout<<"Invalid move!\n";
                                    break;
                                }

                    if(board[row][col].color!=112 && board[row][col].color!=128 && board[row][col].symbol!='K')
                            move_piece(row,col,row2,col2);
                    break;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
                case 'h':
                    if(((row==row2+2 || row==row2-2) && (col==col2+1 || col==col2-1)) || ((col==col2+2 || col==col2-2) && (row==row2+1 || row==row2-1))){
                        if(board[row][col].color!=127 && board[row][col].color!=143 && board[row][col].symbol!='K'){
                            move_piece(row,col,row2,col2);
                        }
                        else{
                            invalid_move=1;
                            cout<<"Invalid move!\n";
                            break;
                        }
                    }
                    else{
                        invalid_move=1;
                        cout<<"Invalid move!\n";
                        break;
                    }
                    break;

//---------------------------------------------------------------------------------------------------------------------------------------------------------
                case 'B':
                    if(mod(row-row2)!=mod(col-col2)){
                        invalid_move=1;
                        cout<<"Invalid move!\n";  //asigura ca miscarea e pe diagonala
                        break;
                    }
                    if(row>row2 && col>col2){
                        for(int i=1;i<=row-row2;i++)
                            if(board[row2+i][col2+i].symbol!='O'){  //dreapta-jos
                                invalid_move=1;
                                cout<<"Invalid move!\n";
                                break;
                            }
                        move_piece(row,col,row2,col2);
                    }
                    if(row<row2 && col>col2){
                        for(int i=1;i<=col-col2;i++)
                            if(board[row2-i][col2+i].symbol!='O'){  //dreapta-sus
                                invalid_move=1;
                                cout<<"Invalid move!\n";
                                break;
                            }
                        move_piece(row,col,row2,col2);
                    }
                    if(row<row2 && col<col2){
                        for(int i=1;i>=row-row2;i--)
                            if(board[row2-i][col2-i].symbol!='O'){  //stanga-sus
                                invalid_move=1;
                                cout<<"Invalid move!\n";
                                break;
                            }
                        move_piece(row,col,row2,col2);
                    }
                    if(row>row2 && col<col2){
                        for(int i=1;i<=row-row2;i++)
                            if(board[row2+i][col2-i].symbol!='O'){ //stanga-jos
                                invalid_move=1;
                                cout<<"Invalid move!\n";
                                break;
                            }
                        move_piece(row,col,row2,col2);
                    }

//---------------------------------------------------------------------------------------------------------------------------------------------------------

                case 'K':
                if((col==col2+1 || col==col2-1) && (row=row2+1 || row==row2-1) && board[row][col].color!=112 && board[row][col].color!=128 && board[row][col].symbol!='K'){
                        move_piece(row,col,row2,col2);
                        break;
                    }
                    else{
                        invalid_move=1;
                        cout<<"Invalid move!\n";
                        break;
                    }

                case 'Q':
                break;
            }
        }
        else{

            switch(board[row2][col2].symbol){  //piesa neagra
                case'p':
                if(board[row][col].symbol=='O' && row==row2+1 && col2==col &&(row==row2+1)){
                        move_piece(row,col,row2,col2);             //mutare in fata
                        break;
                    }

                    if(board[row][col].symbol!='K' && (board[row][col].color==127 || board[row][col].color==143) && (col2==col+1 || col2==col-1) &&(row==row2+1)){
                        move_piece(row,col,row2,col2);           //atac pe diagonala
                        break;
                    }
                    else{
                        invalid_move=1;
                        cout<<"Invalid move!\n";
                        break;
                    }
//---------------------------------------------------------------------------------------------------------------------------------------------------------
                case 'R':
                if((col==col2 && row==row2) || !(col==col2 || row==row2)){
                        invalid_move=1;
                        cout<<"Invalid move!\n";     //ne asiguram ca mutarea e pe orizontala sau verticala
                        break;
                    }
                    if(col==col2)
                        if(row<row2)
                            for(int i=row;i<row2;i++)
                                if(board[i][col].symbol!='O'){
                                    invalid_move=1;
                                    cout<<"Invalid move!\n";
                                    break;
                                }
                        else
                            for(int i=row;i>row2;i--)
                                if(board[i][col].symbol!='O'){
                                    invalid_move=1;
                                    cout<<"Invalid move!\n";
                                    break;
                                }                               //ne asiguram ca drumul e liber
                    if(row==row2)
                        if(col<col2)
                            for(int i=col;i<col2;i++)
                                if(board[i][col].symbol!='O'){
                                    invalid_move=1;
                                    cout<<"Invalid move!\n";
                                    break;
                                }
                        else
                            for(int i=col;i>col2;i--)
                                if(board[i][col].symbol!='O'){
                                    invalid_move=1;
                                    cout<<"Invalid move!\n";
                                    break;
                                }
                    if(board[row][col].color!=112 && board[row][col].color!=128 && board[row][col].symbol!='K')
                            move_piece(row,col,row2,col2);
                    break;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
                case 'h':
                    if(((row==row2+2 || row==row2-2) && (col==col2+1 || col==col2-1)) || ((col==col2+2 || col==col2-2) && (row==row2+1 || row==row2-1)))
                        if(board[row][col].color!=112 && board[row][col].color!=128 && board[row][col].symbol!='K')
                            move_piece(row,col,row2,col2);
                            else{
                                cout<<"Invalid move!\n";
                                invalid_move=1;
                                break;
                            }
                    else{
                        cout<<"Invalid move!\n";
                        invalid_move=1;
                        break;
                    }
                    break;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
                case 'B':
                    if(mod(row-row2)!=mod(col-col2)){
                        cout<<"Invalid move!\n";  //asigura ca miscarea e pe diagonala
                        invalid_move=1;
                        break;
                    }
                    if(row>row2 && col>col2)
                        for(int i=1;i<=row-row2;i++)
                            if(board[row2+i][col2+i].symbol!='O'){  //dreapta-jos
                                cout<<"Invalid move!\n";
                                invalid_move=1;
                                break;
                            }

                    if(row<row2 && col>col2)
                        for(int i=1;i<=col-col2;i++)
                            if(board[row2-i][col2+i].symbol!='O'){  //dreapta-sus
                                cout<<"Invalid move!\n";
                                invalid_move=1;
                                break;
                            }
                    if(row<row2 && col<col2)
                        for(int i=1;i>=row-row2;i--)
                            if(board[row2-i][col2-i].symbol!='O'){  //stanga-sus
                                cout<<"Invalid move!\n";
                                invalid_move=1;
                                break;
                            }
                    if(row>row2 && col<col2)
                        for(int i=1;i<=row-row2;i++)
                            if(board[row2+i][col2-i].symbol!='O'){ //stanga-jos
                                cout<<"Invalid move!\n";
                                invalid_move=1;
                                break;
                            }

//---------------------------------------------------------------------------------------------------------------------------------------------------------
                case 'K':
                    if((col==col2+1 || col==col2-1) && (row=row2+1 || row==row2-1) && board[row][col].color!=112 && board[row][col].color!=128 && board[row][col].symbol!='K'){
                        move_piece(row,col,row2,col2);
                        break;
                    }
                    else{
                        invalid_move=1;
                        cout<<"Invalid move!\n";
                        break;
                    }

                case 'Q':
                break;
            }
        }

    }while(invalid_move==1);

}
//---------------------------------------------------------------------------------------------------------------------------------------------
void queen_movement(int row,int col,int row2,int col2){

}

int main(){
    HANDLE std_output = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(std_output,63);
    system ( "color 30" );
    
    cout << "Chess!\n" << endl;
    
    bool exit=FALSE,player_nr=0;
    int in,turn=0,x;
    help_interface();
    reset();
    player_nr=0; turn=0;
//--------------------------------------------------------------------
    do{
        display_board(player_nr,turn);
        cout<<"What to do? ";
        cin>>in;
        while(in<1 || in>9){
            cout<<"Invalid input!\n";
            cout<<"What to do? "; cin>>in;
        }

        switch(in){
            case 1:
                cout<<"\n\n\n\n\n\n\nStart game!\n";
                player_nr=0; turn=0;
                reset();

                break;
//-----------------------------------------------------------------
            case 2:
                cout<<"1-controls\n2-pieces\n";
                cin>>in;
                if(in==1)
                    help_interface();  //meniuri de ajutor
                else
                    help();
                break;
//----------------------------------------------------------------
            case 3:
                x=select_piece(player_nr);//selectarea piesei
                display_board(player_nr,turn);

                cout<<"1-Continue\n2-Back\n"; cin>>in;

                if(in!=1)
                    break;

                move_piece_check(x); //mutarea piesei

                if(player_nr==1)
                    player_nr=0;   //schimba jucatorul
                else
                    player_nr=1;

                turn++;
                break;
//------------------------------------------------------------------
            case 9:
                cout<<"Good game!\n";
                exit=TRUE;           //comanda de iesire
                break;

        }
    }while(exit==0);
    return 0;
}

/*
    Coduri de culori:

    112- negru pe gri deschis
    128- negru pe gri inchis

    127- alb pe gri deschis
    143- alb pe gri inchis

    136- gri inchis pe gri inchis
    119- gri deschis pe gri deschis

    192-negru pe rosu
    207-alb pe rosu

    63- alb pe cyan
    color 30- fundal cyan

    224-negru pe galben
    238-galben pe galben
    239-alb pe galben (nu se prea vede)

*/

