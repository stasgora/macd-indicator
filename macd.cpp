#include <iostream>
#include <fstream>
#include <iomanip>

#define SampleNum 1000
#define Period1 12
#define Period2 26
#define Period3 9

using namespace std;

void calcMACD(fstream &data, fstream &output);

void calcSignal(fstream &data, fstream &output);

double calcMean(fstream &data, int elNum);

inline double calcMultip(int elNum);

int main() {
	fstream data, macd, signal;
	data.open("data.txt", ios::in);
	macd.open("MACD.txt", ios::out);
	signal.open("Signal.txt", ios::out);
	if (!data.is_open() || !macd.is_open() || !signal.is_open()) {
		cout << "File open error";
		return 0;
	}
	calcMACD(data, macd);
	data.close();
	macd.close();
	macd.open("MACD.txt", ios::in);
	if (!macd.is_open()) {
		cout << "File open error";
		return 0;
	}
	calcSignal(macd, signal);
	macd.close();
	signal.close();

	return 0;
}

void calcMACD(fstream &data, fstream &output) {
	double el2 = calcMean(data, Period2), elem;
	double multi2 = calcMultip(Period2), revMulti2 = 1 - multi2;
	data.clear();
	data.seekg(0, ios::beg);
	double el1 = calcMean(data, Period1);
	double multi1 = calcMultip(Period1), revMulti1 = 1 - multi1;

	for (int i = 0; i < SampleNum - Period1; ++i) {
		data >> elem;
		el1 = multi1 * elem + revMulti1 * el1;
		if(i < Period2 - Period1 - 1)
			continue;
		if(i == Period2 - Period1 - 1) {
			output << el1 - el2  << endl;
			continue;
		}
		el2 = multi2 * elem + revMulti2 * el2;
		output << el1 - el2  << endl;
	}
}

void calcSignal(fstream &data, fstream &output) {
	double multi = calcMultip(Period3);
	double revMulti = 1 - multi, el;
	double last = calcMean(data, Period3);
	output << last  << endl;
	for (int i = 0; i < SampleNum - (Period2 + Period3 - 1); ++i) {
		data >> el;
		last = multi * el + revMulti * last;
		output << last  << endl;
	}
}

double calcMean(fstream &data, int elNum) {
	int index = 0;
	double sum = 0, el;
	while (index++ < elNum) {
		data >> el;
		sum += el;
	}
	return sum / elNum;
}

inline double calcMultip(int elNum) {
	return (double) 2 / (elNum + 1);
}
