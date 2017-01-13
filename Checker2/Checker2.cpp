// Checker2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <fstream>
using std::ios;
using std::ifstream;
#include <exception>
using std::exception;

#include <cstring>
#include <cstdlib>
using std::exit;
using std::memcmp;

#include <string>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <cmath>
using std::fstream;
#include <Windows.h>


//vrne ime fajla za i-ti izhod
std::string vrniOut(int i) {
	std::ostringstream oss2;
	oss2 << "testi/izhod" << i << ".txt";
	std::string outFile = oss2.str();
	return outFile;
}
//vrne ime fajla za i-ti vhod
std::string vrniIn(int i) {
	std::ostringstream oss;
	oss << "inputs/I_" << i << ".txt";
	std::string inFile = oss.str();
	return inFile;
}
//vrne ime fajla za i-to resitev
std::string vrniOutResitev(int i) {
	std::ostringstream oss;
	oss << "outputs/O_" << i << ".txt";
	std::string rez = oss.str();
	return rez;
}
//za i-ti test vrne string OK/ERROR
std::string vrniRez(int i, bool ok) {
	std::ostringstream oss;
	if (ok) {
		oss << i << ". test: OK";
	} else {
		oss << i << ". test: ERROR";
	}
	std::string rez = oss.str();
	return rez;
}
//pove ali se int-a razlikujeta za 1 ali manj
bool zaEnaAliManjRazlike(int a, int b) {
	if (abs(a - b) == 1 || abs(a -b) == 0) {
		return true;
	}
	return false;
}
//preveri da znaka nista newline
bool novaVrsta(char c1, char c2) {
	if (c1 == '\n' && c2 == '\n') {
		return true;
	}
	else if (c1 == '\r\n' && c2 == '\n') {
		return true;
	}
	else if (c1 == '\n' && c2 == '\r\n') {
		return true;
	}
	else if (c1 == '\r\n' && c2 == '\r\n') {
		return true;
	}
	else {
		return false;
	}
}


//preveri ce sta dve datoteki enaki
bool sameFiles(std::string f1, std::string f2) {
	ifstream fin1;
	fin1.open(f1, ios::in);

	ifstream fin2;
	fin2.open(f2, ios::in);

	int len1 = 0;
	int len2 = 0;
	int staEnaka = 1;

	while (!fin1.eof()) {
		char c;
		fin1.get(c);
		len1++;
	}
	while (!fin2.eof()) {
		char c;
		fin2.get(c);
		len2++;
	}

	char c1;
	char c2;

	if (len1 < len2) {
		while (!fin1.eof()) {
			fin1.get(c1);
			fin2.get(c2);
			if (c1 != c2 && !novaVrsta(c1, c2)) {
				staEnaka = 0;
				break;
			}
		}
	}
	else if (len2 < len1) {
		while (!fin2.eof()) {
			fin1.get(c1);
			fin2.get(c2);
			if (c1 != c2 && !novaVrsta(c1, c2)) {
				staEnaka = 0;
				break;
			}
		}
	}
	else {
		while (!fin1.eof()) {
			fin1.get(c1);
			fin2.get(c2);
			if (c1 != c2 && !novaVrsta(c1, c2)) {
				staEnaka = 0;
				break;
			}
		}
	}

	if (staEnaka && zaEnaAliManjRazlike(len1, len2)) {
		return true;
	}
	else {
		return false;
	}
}
//odstrani koncnico file-u
std::string vrniBrezKoncnice(std::string fullname) {
	size_t lastindex = fullname.find_last_of(".");
	std::string rawname = fullname.substr(0, lastindex);
	return rawname;
}

int main(int argc, char* argv[]) {
	//ce je premalo argumentov, vrze error
	if (argc < 2) {
		cerr << "ERROR: missing filename\nCorrect usage: Checker2.exe \"file_name.java\"" << endl;
		system("pause");
		return 1;
	}
	//shrani file in njegovo pot
	std::string datoteka = argv[1];
	std::string dat = vrniBrezKoncnice(datoteka);

	//prevede java program
	cout << "Prevajam..." << endl;
	std::ostringstream oss_compile;
	oss_compile << "javac " << datoteka;
	std::string compileString = oss_compile.str();
	const char * compile_ukaz = compileString.c_str();
	system(compile_ukaz);

	//kreira mapo testi
	std::string dir = "mkdir testi";
	const char * newDir = dir.c_str();
	system(newDir);

	//izvede java program 10x - beres iz inputs mape in piše v mapo testi
	for (int i = 0; i < 10; i++) {

		std::ostringstream oss3;
		oss3 << "java " << dat << " " << vrniIn(i) << " " << vrniOut(i);
		std::string cmd = oss3.str();
		const char * ukaz = cmd.c_str();

		system(ukaz);
	}
	//konec izvajanja

	//zacetek testiranja
	int rez = 0;
	for (int i = 0; i < 10; i++) {
		try {
			if (sameFiles(vrniOut(i), vrniOutResitev(i))) {
				cout << vrniRez(i, true) << endl;
				rez++;
			}
			else {
				cout << vrniRez(i, false) << endl;;
			}

		}
		catch (const exception& ex) {
			cerr << ex.what() << endl;
			exit(-2);
		}
	}	
	//konec testiranja
	cout << "Rezultat: " << rez << "/10" << endl;

	system("pause");
    return 0;
}


