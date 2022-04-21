#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string.h>
#include <queue>
#include <algorithm>
#include <memory>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"
#include <pthread.h>
#include <time.h>
 #define MAX 100000

#define INFINITY 200000

using namespace std;
using namespace Minisat;


vector<int> vertexCover(int, vector<int>, int);
void CNF_SAT_VC(int, vector<int>);
void APPROCX_VC_1(int, vector<int>);
void APPROCX_VC_2(int, vector<int>);

//int pthread_getcpuclockid(pthread_t thread,clockid_t *clock_id);
vector<int> V;
vector<int> E;
int n_vertices;

vector<int> output_0;
vector<int> output_1;
vector<int> output_2;



bool element_parser(vector<int> v, int element) {
	vector<int>::iterator it;

	it = find(v.begin(), v.end(), element);
	if (it != v.end()) {
		return true;
	}

	else {
		return false;
	}
}

void* APPROX_VC_1handler(void* arg) {
	//cout << "1" << endl;

	//clock_t start, finish;
	//double duration;

	//start = clock();
	APPROCX_VC_1(std::ref(n_vertices), std::ref(E));
	//finish = clock();

	//duration = (double)(finish - start) / CLOCKS_PER_SEC;
	//cout << "the duration of APPROX-VC-1 is: " << "VC-1" << duration << endl;
	// clockid_t cid;
	// int s;
    // s = pthread_getcpuclockid(pthread_self(), &cid);
	// if (s != 0)
	// 	handleErrorEn(s, "pthread_getcpuclockid");
	// pclock("APPROX-VC-1 Time:", cid);
	// cout<<endl;
	return NULL;

}


void APPROCX_VC_1(int n_vertices, vector<int>edge) {
	int cnt[n_vertices];
	int edge_num = edge.size();
	int max = 0;
	std::vector<int> new_edge = edge;
	//std::vector<int> output_1;

	while (!edge.empty()) {
		for (int i = 0; i < n_vertices; i++) {
			cnt[i] = 0;
		}
		for (int i = 0; i < edge.size(); i++) {
			if (edge[i] != INFINITY)
				cnt[edge[i]]++;

		}

		max = max_element(cnt, cnt + n_vertices) - cnt;
		output_1.push_back(max);
		new_edge.clear();


		for (int i = 0; i < edge.size(); i++) {
			if (edge[i] == max || edge[i + 1] == max) {
				//continue;
				edge[i] = INFINITY;
				edge[i + 1] = INFINITY;
				//edge_num -= 2;
			}
			else {
				new_edge.push_back(edge[i]);
				new_edge.push_back(edge[i + 1]);
			}
			i++;
		}
		edge = new_edge;
	}

	sort(output_1.begin(), output_1.end());
}

void* APPROX_VC_2handler(void* arg) {
	//clock_t start, finish;
	//double duration;

	//start = clock();
	APPROCX_VC_2(ref(n_vertices), ref(E));
	//finish = clock();

	//duration = (double)(finish - start)/CLOCKS_PER_SEC;
	//cout << "the duration of APPROX-VC-2 is: " << duration << endl;

	// clockid_t cid;
	// int s;
    // s = pthread_getcpuclockid(pthread_self(), &cid);
	// if (s != 0)
	// 	handleErrorEn(s, "pthread_getcpuclockid");
	// pclock("APPROX-VC-2 Time:", cid);
	// cout<<endl;
	return NULL;

}

void APPROCX_VC_2(int n_vertices, vector<int>edge) {

	//std::vector<int> output_2;
	vector<int> new_edge = edge;


	while (!edge.empty()) {

		output_2.push_back(edge[0]);
		output_2.push_back(edge[1]);

		new_edge.clear();
		for (unsigned i = 0; i < edge.size() - 1; ++i) {
			if (i % 2 == 0) {
				if (edge[i] == edge[0] || edge[i + 1] == edge[1] || edge[i] == edge[1] || edge[i + 1] == edge[0]) {
					continue;
				}
				else {
					new_edge.push_back(edge[i]);
					new_edge.push_back(edge[i + 1]);
				}
			}
		}
		edge = new_edge;
	}
	sort(output_2.begin(), output_2.end());
}

void* CNF_SAT_VChandler(void* arg) {

	//CNF_SAT_VC(std::ref(n_vertices), std::ref(E));
	//clock_t start, finish;
	//double duration;

	//start = clock();
	CNF_SAT_VC(std::ref(n_vertices), std::ref(E));
	//finish = clock();

	//duration = (double)(finish - start) / CLOCKS_PER_SEC;
	//cout << "the duration of CNF_SAT is: " << duration << endl;

	// clockid_t cid;
	// int s;
    // s = pthread_getcpuclockid(pthread_self(), &cid);
	// if (s != 0)
	// 	handleErrorEn(s, "pthread_getcpuclockid");
	// pclock("CNF-SAT Time:", cid);
	// cout<<endl;
	return NULL;

}


void CNF_SAT_VC(int n_vertices, vector<int>edge) {
	Solver solver;

	vector<int> result;
	//vector<int> output_0;

	int num = n_vertices;

	vector<int> Edge = edge;

	int hi = n_vertices;
	int low = 1;
	//int mid;
	vector<int> tmp = {-1};

	while (hi >= low) {
		int mid = (hi + low) / 2;

		result = vertexCover(num, Edge, mid);

		if(result.empty())
		{
			cerr << "Empty Vector1" <<endl;
		}
		else if (tmp.empty())
		{
			/* code */
			cerr << "Empty Vector2" <<endl;
		}
		
		else
		{

		bool nosat = std::equal(result.begin(), result.end(), tmp.begin());
		if (not nosat) {
			hi = mid - 1;
			output_0.clear();
			output_0 = result;
		}
		else {
			low = mid + 1;
		}

		}

	}
	if (Edge.empty()) {
		output_0.clear();
	}
	sort(output_0.begin(), output_0.end());


}

std::vector<int> vertexCover(int n_vertices, vector<int>edge, int k) {

	Solver solver;

	Var propositions[n_vertices][k];

	for (int i = 0; i < n_vertices; i++) {
		for (int j = 0; j < k; j++) {
			propositions[i][j] = solver.newVar();
		}
	}

	// clause 1 : At least (exactly only one) one vertex is the ith vertex in the vertex cover, i in [1,k]

	for (int i = 0; i < k; i++) {
		vec<Lit> clause1;

		for (int j = 0; j < n_vertices; j++) {
			clause1.push(mkLit(propositions[j][i]));
		}

		solver.addClause(clause1);
		clause1.clear();

	}

	// clause 2 : No one vertex can appear twice in a vertex cover

	for (int m = 0; m < n_vertices; m++) {
		for (int p = 0; p < k - 1; p++) {
			for (int q = p + 1; q < k; q++) {
				solver.addClause(mkLit(propositions[m][p], true), mkLit(propositions[m][q], true));
			}
		}
	}
	// clause 3 : No more than one vertex appears in the mth position of the vertex cover.

	for (int m = 0; m < k; m++) {
		for (int p = 0; p < n_vertices - 1; p++) {
			for (int q = p + 1; q < n_vertices; q++) {
				solver.addClause(mkLit(propositions[p][m], true), mkLit(propositions[q][m], true));
			}
		}
	}

	// clause 4 : Every edge is incident to at least one vertex in the vertex cover
	for (int i = 0; i < edge.size(); i++) {
		vec<Lit> clause4;
		for (int m = 0; m < k; m++) {
			clause4.push(mkLit(propositions[edge[i]][m]));

			clause4.push(mkLit(propositions[edge[i + 1]][m]));
		}
		solver.addClause(clause4);
		clause4.clear();
		i++;
	}

	std::vector<int> vertex_cover;


	if (solver.solve()) {

		for (int i = 0; i < n_vertices; i++) {
			for (int j = 0; j < k; j++) {
				if (toInt(solver.modelValue(propositions[i][j])) == 0) {
					vertex_cover.push_back(i);
				}
			}
		}

		return vertex_cover;
	}
	else {
		return { -1 };
	}
}

void print_output() {
	//double ratio_0, ratio_1, ratio_2;

	cout << "CNF-SAT-VC: ";
	for (int i = 0; i < output_0.size(); ++i) {
		if (i < output_0.size()-1) {
			cout << output_0[i]+ 1<< ",";
		}
		else {
			cout << output_0[i]+ 1;
		}
	}
	cout << "\n";
	//cout << "CNF-SAT Ratio: " << output_0.size()/output_0.size() << endl;

	cout << "APPROX-VC-1: ";
	for (int i = 0; i < output_1.size(); ++i) {
		if (i < output_1.size()-1) {
			cout << output_1[i]+ 1<< ",";
		}
		else {
			cout << output_1[i]+ 1;
		}		
	}
	cout << "\n";
	//cout << "APPROX-VC-1 Ratio: " << output_1.size()/output_0.size() << endl;

	cout << "APPROX-VC-2: ";
	for (int i = 0; i < output_2.size(); ++i) {
		if (i < output_2.size()-1) {
			cout << output_2[i] + 1<< ",";
		}
		else {
			cout << output_2[i] + 1;
		}
	}
	cout << "\n";
	//cout << "APPROX-VC-2 Ratio: " << output_2.size()/output_0.size() << endl;
	output_0.clear();
	output_1.clear();
	output_2.clear();
}

void* IOhandler(void* arg) {
	while (!::cin.eof()) {

		string input;

		while (getline(cin, input)) {
			char arr[MAX];

			strcpy(arr, input.c_str());

			if (arr[0] == 'V') {
				//cout << "vertex\n";
				//cout << line << endl; //a3: edit it to print the graph that it has read before accepting an s command.
				int num = 0;
				for (int i = 0; arr[i] != '\0'; ++i) {

					if (arr[i] >= '0' && arr[i] <= '9')

						num = num * 10 + arr[i] - '0';
				}


				for (int j = 0; j < num; ++j) {
					V.push_back(j);
				}
				for (vector<int>::iterator it = V.begin(); it != V.end(); it++) {
					//cout << *it << " ";
				}//print vector
				while (V.empty()) {
					cout << "Error: Empty Vertex" << endl;
					break;
				}
				break;
			}


			else if (arr[0] == 'E') {
				int num = 0;
				int d_edge = 0;
				//cout << line << endl;//a3: edit it to print the graph that it has read before accepting an s command.
				for (int i = 0; arr[i] != '\0'; i++) {
					//cout << arr[i] << "\n"; //Every element of line
					if (arr[i] > '0' && arr[i] <= '9') {
						//cout << arr[i] << "\n"; // Every number in line
						num = num * 10 + arr[i] - '0';
						//cout << num << endl; // Number in int num variable
					}
					else {
						if (arr[i] == '0' && num == 0) {
							E.push_back(0);
						}
						if (arr[i] == '0' && (num != 0)) {
							num = num * 10 + arr[i] - '0';
							//cout << num << endl;
							E.push_back(num - 1);
							num = 0;
						}
						if (arr[i] != '0' && (num != 0)) {
							//cout << num << endl;
							E.push_back(num - 1);
							num = 0;
						}

					}
				}
				for (int i = 0; i < E.size(); i++) {
					//cout << E[i];
					if (element_parser(V, E[i]) == false) {
						//cout << E[i];
						cerr << "Error: Vertex not supported" << endl;// check the error of edge: not a vertex
						E.erase(E.begin() + i);
						return 0;
					}
				}
				while (E.empty()) {
					cout << "Error: Empty Edge." << endl;
					//return 0;
					break;
				}
				n_vertices = V.size();
				int s;
				pthread_t threadio, thread0, thread1, thread2;

				s = pthread_create(&thread0, NULL, CNF_SAT_VChandler, NULL);
				s = pthread_create(&thread1, NULL, APPROX_VC_1handler, NULL);
				s = pthread_create(&thread2, NULL, APPROX_VC_2handler, NULL);

				pthread_join(thread0, NULL);
				pthread_join(thread1, NULL);
				pthread_join(thread2, NULL);
				
				print_output();

				E.clear();
				V.clear();

			}
			else{
				cout << "Error: Input not supported" << endl;
			}
		}
	}
	return NULL;
}





int main(int argc, char** argv) {

	int s;
	pthread_t threadio;
	//pthread_t thread0, thread1, thread2;

	//clockid_t cid0,cid1,cid2;

	//vector<int> result_cnf, result_a1, result_a2;

	s = pthread_create(&threadio, NULL, IOhandler, NULL);
	//s = pthread_create(&thread0, NULL, threadCNF_SAT_VC, NULL);
	//s = pthread_create(&thread1, NULL, threadAPPROX_VC_1, NULL);
	//s = pthread_create(&thread2, NULL, threadAPPROX_VC_2, NULL);


	pthread_join(threadio, NULL);
	//pthread_join(thread0, NULL);
	//pthread_join(thread1, NULL);
	//pthread_join(thread2, NULL);
	return 0;
}

// V 5
// E {<3,2>,<3,1>,<3,4>,<2,5>,<5,4>}
// E {<3,2>,<3,1>,<3,4>,<2,5>,<5,4>}
