#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

using namespace std;

int main(int argc, char **argv)
{
	ifstream config;
	config.open(argv[1]);

	int m, k;
	config >> m >> k;

	vector<vector<int>> PattHist;
	vector<bool> BraHist;

	config.close();

	for (int i = 0; i < k; i++)
	{
		BraHist.push_back(true);
	}

	for (int i = 0; i < pow(2, m); i++)
	{
		PattHist.push_back(vector<int>());
		for (int j = 0; j < pow(2, k); j++)
			PattHist.back().push_back(3);
	}
	ofstream out;
	string out_file_name = string(argv[2]) + ".out";
	out.open(out_file_name.c_str());
	// out.close();

	ifstream trace;
	trace.open(argv[2]);

	float Miss = 0;
	float Total = 0;
	float missRate = 0;
	bool InitialVal = true;
	while (!trace.eof())
	{
		Total++;
		unsigned long pc;
		bool taken;
		trace >> std::hex >> pc >> taken; //load pc and taken in hex format
		bool prediction;
		int index = 0, power = 1;
		int T_NT;
		int Temp_value;

		for (int i = 0; i < k; i++)
		{

			if (BraHist[i])
			{
				T_NT = 1;
			}
			else
			{
				T_NT = 0;
			}
			index = index + (T_NT)*power;
			// cout << index;
			power = power * 2;
		}

		Temp_value = PattHist[pc % (int)pow(2, m)][index];

		if (Temp_value == 1)
		{
			prediction = false;
			if (prediction != taken)
			{
				Miss++;
				PattHist[pc % (int)pow(2, m)][index] = 2;
			}
			else
			{
				PattHist[pc % (int)pow(2, m)][index] = 0;
			}
		}
		else if (Temp_value == 2)
		{
			prediction = true;
			if (prediction != taken)
			{
				PattHist[pc % (int)pow(2, m)][index] = 1;
				Miss++;
			}
			else
			{
				PattHist[pc % (int)pow(2, m)][index] = 3;
			}
		}
		else if (Temp_value == 3)
		{
			prediction = true;
			if (prediction != taken)
			{
				PattHist[pc % (int)pow(2, m)][index] = 2;
				Miss++;
			}
		}
		else{
			prediction = false;
			if (prediction != taken)
			{
				Miss++;
				PattHist[pc % (int)pow(2, m)][index] = 1;
			}
		}

		for (int i = k - 1; i > 0; i--)
			BraHist[i] = BraHist[i - 1];

		if (k > 0)
			BraHist[0] = taken;
		if (!InitialVal)
			out << endl;

		out << prediction;
		InitialVal = false;
	}
	missRate = Miss / Total;
	cout << endl << missRate << endl;

	trace.close();
	out.close();
}
