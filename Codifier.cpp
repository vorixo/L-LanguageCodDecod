#include <iostream>
#include <vector>
#include <windows.h>
#include <math.h>
#include <conio.h>
#include <algorithm>    // std::count
#include <string>
using namespace std;

struct Instruction {
	int a;  // tag
	int b; 	// instruction type
	int c;	// Variable - 1
};

vector<Instruction> instructions;
vector<int> solutions;
int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271};
string tag_list[] = {"A1", "B1", "C1", "D1", "S1", "A2", "B2", "C2", "D2", "S2", "A3", "B3", "C3", "D3", "S3", "A4", "B4", "C4", "D4", "S4", "A6", "B6", "C6", "D6", "S6", "A7", "B7", "C7", "D7", "S7", "A8", "B8", "C8", "D8", "S8", "A9", "B9", "C9", "D9", "S9"};
string var_list[] = { "Y", "X1", "Z1", "X2", "Z2", "X3", "Z3", "X4", "Z4", "X5", "Z5", "X6", "Z6", "X7", "Z7", "X8", "Z8", "X9", "Z9", "X10", "Z10", "X11", "Z11", "X12", "Z12", "X13", "Z13", "X14", "Z14", "X15", "Z15", "X16", "Z16", "X17", "Z17", "X18", "Z18", "X19", "Z19"};


int solve_y(int a, int b) {
	return ((pow(2, a))*((2*b) + 1)-1);
}

void resolve_phase_one() {
	for(int i = 0; i < instructions.size(); ++i) {
		const int partial_one(solve_y(instructions[i].b, instructions[i].c));
		const int solution(solve_y(instructions[i].a, partial_one));
		solutions.push_back(solution);
		cout << "I#" << i+1 << " = <" << instructions[i].a << ", " << partial_one << "> = " << solution << endl;
	} 
}

void resolve_phase_two() {
	cout << "--------------------------------\n[";
	for(int i = 0; i < solutions.size(); ++i) {
		cout << (i==0 ? "" : ", ") << solutions[i];
	}
	cout << "] - 1" << endl;
	
	// Final result
	cout << "#P = ";
	for(int i = 0; i < solutions.size(); ++i) {		
		cout << primes[i] << "^" << solutions[i] << (i==solutions.size()-1 ? " - 1" : " * ");
	}
}

void codify() {
	int a = -1;
	do {
		cout << "Tag (0: none): ";
		cin >> a;
		if(a != -1) {
			Instruction ins;
			ins.a = a;
			cout << "Instruction type: ";
			cin >> ins.b;
			cout << "Variable (var - 1): ";
			cin >> ins.c;
			instructions.push_back(ins);
			system("cls");
		}
	} while(a != -1);
	system("cls");
	resolve_phase_one();
	getch();
	resolve_phase_two();
}

vector<int> prime_descomposition;
vector<int> ordered_exponents;

void decomposeprimefactors(int n) 
{ 
    // Print the number of 2s that divide n 
    while (n%2 == 0) 
    { 
        prime_descomposition.push_back(2);
        n = n/2; 
    }   
    // n must be odd at this point.  So we can skip  
    // one element (Note i = i +2) 
    for (int i = 3; i <= sqrt(n); i = i+2) 
    { 
        // While i divides n, print i and divide n 
        while (n%i == 0) 
        { 
            prime_descomposition.push_back(i);
            n = n/i; 
        } 
    } 
    // This condition is to handle the case when n  
    // is a prime number greater than 2 
    if (n > 2) 
        prime_descomposition.push_back(n); 
} 

int left_y(int y){
	// l(y) = min(t<=i) [¬2^(t+1) | c+1]
	bool g_end(false);
	int i(0);
	while(!g_end) {
		int twot(pow(2, i+1));
		g_end = !(((y+1)%twot) == 0);
		++i;
	}
	return i-1;
}

int right_y(int y){
	// r(y) = ((c+1)/2^l(c) -1)/2
	int partone((y+1)/pow(2, left_y(y)));
	partone = partone - 1;
	partone = partone/2;
	return partone;
}


void decodify_n(int n) {
	cout << n << " = <a, <b, c>>\n";
	int a(left_y(n));
	cout << "a = L(" << n << ") = " << a << endl; 
	int bc(right_y(n));
	cout << "<b, c> = R(" << n << ") = " << bc << endl;
	int b(left_y(bc));
	cout << "b = L(" << bc << ") = " << b << endl;
	int c(right_y(bc));
	cout << "c = R(" << bc << ") = " << c << endl;
	string fin_instr("<" + std::to_string(a) + ", <" + std::to_string(b) + ", " + std::to_string(c) + ">>");
	Instruction instr_toro;
	instr_toro.a = a;
	instr_toro.b = b;
	instr_toro.c = c;
	instructions.push_back(instr_toro);
	cout << "#I" << instructions.size() << " = " << fin_instr << endl;
	cout << endl;
}

void generate_instruction(Instruction l) {
	if(l.a != 0) {
		cout << "(" << tag_list[l.a-1] << ")\t";
	} else {
		cout << "\t";
	}
	
	if(l.b == 0) {
		cout << var_list[l.c] << "==";	
	} else if(l.b == 1 || l.b == 2) {
		cout << var_list[l.c] << (l.b==1 ? "++" : "--");
	} else if(l.b > 2) {
		cout << "IF " << var_list[l.c] << " != 0 GOTO " << tag_list[(l.b-2)-1];
	}
	cout << endl;
	
}


void rawnm() {
	int program(-1);
	cout << "#P = ";
	cin >> program;
	// #P-1
	program+=1;
	cout << program << " = ";
	decomposeprimefactors(program);
	// at this point the vector is refreshed
	int quantity(1);
	for(int i = 0; i < sizeof(primes)/sizeof(primes[0]) && quantity != program; ++i) {
		int count = std::count(std::begin(prime_descomposition), std::end(prime_descomposition), primes[i]);
		ordered_exponents.push_back(count);
		quantity*=pow(primes[i], count);
		cout << primes[i] << "^" << count << (quantity != program ? " * " : "");
	}
	
	cout << "\n--------------------------------" << endl; 
	getch();
}

void fillquantityarray() {
	cout << "Press -1 when you are ready." << endl;
	int val(0);
	while(val != -1) {
		cout << "Val = ";
		cin >> val;
		if(val != -1)
			ordered_exponents.push_back(val);
	}
	system("cls");
}

void decodify() {
	cout << "Raw number, vector of exponents (0,1): ";
	int opt(-1);
	cin >> opt;
	system("cls");
	switch(opt) {
		case 0:
			rawnm();
			break;
		case 1:
			fillquantityarray();
			break;
		default:
			cout << "ERROR.";
			break;
	}
	
	cout << "#P = [";
	for(int i = 0; i < ordered_exponents.size(); ++i) {
		cout << (i==0 ? "" : ", ") << ordered_exponents[i];
	}
	cout << "] - 1" << endl << endl;
	
	// here we resolve each instruction
	for(int i = 0; i < ordered_exponents.size(); ++i) {
		decodify_n(ordered_exponents[i]);
	}
	
	cout << "GENERATED CODE: " << endl;
	// we generate the instructions here
	for(Instruction l : instructions) {
		generate_instruction(l);
	}
	
}

int main() {
	int op(-1);
	cout << "Codify or decodify [0-1]: ";
	cin >> op;
	system("cls");
	switch(op) {
		case 0:
			codify();
			break;
		case 1:
			decodify();
			break;
		default:
			cout << "ERROR." << endl;
			break;
	}
}



