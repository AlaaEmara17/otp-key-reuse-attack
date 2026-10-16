//
// Created by alaa-emara on 10/12/26.
//
#include<bits/stdc++.h>
using namespace std;

struct space_location {
    int index;
    int cipher1;
    int cipher2;

};
vector<string>ciphers;  //store cipher txt c1->c8
vector<space_location>v;// to store the possibility of spaces between ciphers
int key [74];
//first we need to identify locations of spaces
// we need to xor c1 with c2 check bit 6
//space in hexadecimal 0x20 xor it with a-z or A-Z
// if output of xor = 1 in bit 6 and remaining are zeros  -> one of them is space
void xor_operation(int c1 , int c2 ) {

    for (int i =0 ; i<148; i=i+2) {
             string l1 , l2 ;
             l1= ciphers[c1].substr(i , 2 ) ; //take each 2 characters from string (1 byte)
             l2 = ciphers[c2].substr(i,2);
         // cout<< l <<" "<<l2;
         int x = stoi(l1, nullptr , 16) ;
         int y = stoi(l2, nullptr , 16);
         int z= x^y ;
        if (z & 0x40) {
           // cout<<"YES"<<endl;
            v.push_back({i , c1 , c2 });
        }
    }
}
bool is_line_or_space(int c) {
    // is ch in range a-z or A-Z or space
    return c == 32 || (c >= 65 && c <= 90) || (c >= 97 && c <= 122);

}
void majority_to_find_space() {
   while(!v.empty()) {
        int vote1 =0, vote2 =0  ;
        space_location s = v.back();
        v.pop_back();
        string hexa_a , c , hexa_b ;
        for (int i =0 ; i< 8 ; i++) {
            if (i== s.cipher1 || i == s.cipher2) //check if it's valid combination
                continue;
            else {
                hexa_a=  ciphers[s.cipher1].substr(s.index , 2);
                hexa_b = ciphers[s.cipher2].substr(s.index , 2);
                c =  ciphers[i].substr(s.index , 2);
                int voter_one= stoi(hexa_a , nullptr , 16)^stoi(c , nullptr ,16);
                int voter_two = stoi(hexa_b , nullptr , 16)^stoi(c , nullptr ,16);

                if (voter_one & 0x40)
                    vote1++;
                if (voter_two &0x40)
                    vote2++;

            }
        }
        int ch =-10;
        if (vote2 > vote1 ) {
             ch = stoi(hexa_b , nullptr , 16) ^ 0x20;

        }
        else if (vote2<vote1) {
           ch= stoi(hexa_a , nullptr , 16) ^ 0x20;
        }
       else
           continue;
       bool flag = true;
       for (int i =0 ; i<8 ; i++) {
           int res= stoi(ciphers[i].substr(s.index ,2) ,nullptr , 16)^ch;
           if ( is_line_or_space(res))
               continue;

           else {
               flag = false;
               break;
               }
           }
       if (flag )
           key[s.index/2]=ch;
       }
}
void set_key_guess( int c, int start, string word) {
    for (int i = 0; i < (int)word.size(); i++) {
        int idx = start + i;
        int cbyte = stoi(ciphers[c].substr(2 * idx, 2), nullptr, 16);
        key[idx] = cbyte ^ word[i];
    }
}
void print_decrypted_msg() {
    for (int i =0 ; i<8 ; i++) {
        for (int j =0 ; j<148 ; j =j+2) {
            if (key[j/2]!=-1) {
                char temp = stoi(ciphers[i].substr(j, 2 ) , nullptr , 16)^key[j/2];
                cout<< temp;
            }
            else
                cout<<"*";
        }
        cout<<endl;
    }
}
int main() {

    memset(key , -1 , sizeof(key)); // marking every key byte unkown

    ifstream inputfile("/home/alaa-emara/security_lab2/Asg2_ciphertexts.txt");
    if (!inputfile) {
        cerr << "Error: Could not open the file!"<<endl;
        return 0;
    }
    //file reading
    string curr_line;
    while (getline(inputfile , curr_line)) {
        ciphers.push_back(curr_line);
        cout<<curr_line << " # of char." << curr_line.size() <<endl;

    }
    for (int i =0 ; i<8 ; i++) {
        for (int j =i+1 ; j<8 ; j++) {
            xor_operation( i ,j );
            majority_to_find_space();
        }
    }
    cout<< "Output before any guesses"<<endl;
    print_decrypted_msg();
    // after seeing the reveled letters some vocabulary are very easy to guess
    // sys*** s*ft*are -> system software        sof*wa*e-> software c1
    // mess**** ->messages unre**g**z*d->unrecognized
    // impor***t ->important     fre****tly->frequently
    //***mi*si*ns ->permissions
    //safe*ua*d -> safeguard    pers***l->personal
    //  o*line ->online  co*pl*te**->complete
    cout<<"---------------------------------------------------"<<endl;
    cout<<"Output after guesses"<<endl;
    set_key_guess( 1, 10  ,"tem");
    set_key_guess( 0 , 0 , "Acti");
    set_key_guess( 7 ,18,"n" );
    set_key_guess( 0 , 24 , "cation");
    set_key_guess( 1 , 14 ,"software");
    set_key_guess(7 ,51,"completely" );
    set_key_guess(1 ,59, "vulnerabilities" );
    set_key_guess(2 , 37 , "unrecognized");

    print_decrypted_msg();



}
