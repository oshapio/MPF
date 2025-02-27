#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <string>
#include <iostream>
#include <utility>
#include <tuple>
#include <set>
#include <string>
#include <fstream>

using namespace std;

typedef long long ll;
typedef tuple<int, int, int> t3;
typedef int t;
typedef vector<t> vi;
typedef pair<double, double> pii;
typedef vector<vector<string> > vvs;
const ll MOD = 1e9 + 9;
const int CMAX = 1e5 + 100;
const t X_LIMIT = 5, Y_LIMIT = 5; // x_ij max value
ofstream fd_logging;
bool one_one = true;
const int M_DEFAULT = 2;
const int M = 2; // M x M matrices

const int REDP = 3;
struct word {
	t beta, i, j, alpha;

	void set_vals(vi vect) {
		beta = vect[0], i = vect[1], j = vect[2], alpha = vect[3];
	}
	word(vi vect) {
		set_vals(vect);
	}
	word(t a, t b, t c, t d) {
		set_vals({ a,b,c,d });
	}
	word() {}
	int get_lambda() {
		return i + alpha - beta - j;
	}
	bool medial_is_reducible() {
		if ((beta + j) > 5 && (alpha + i) > 5)
			return true;
		if (alpha == 1 && i == 4 && beta == 0 && j == 5)
			return true;
		if (beta == 1 && j == 4 && alpha == 0 && i == 5)
			return true;
		if (alpha == 1 && i == 4 && beta == 1 && j >= 5)
			return true;
		if (beta == 1 && j == 4 && alpha == 1 && i >= 5)
			return true;
		if (alpha == 0 && beta == 0 && i == 5 && j > 5)
			return true;
		if (beta == 0 && alpha == 0 && j == 5 && i > 5)
			return true;
		if (alpha == 0 && beta == 1 && i == 5 && j >= 4)
			return true;
		if (beta == 0 && alpha == 1 && j == 5 && i >= 4)
			return true;
		return false;
	}
	pii reduce_two(int j, int i, int mode = 0) {
		int mino = min(i, j), maxo = max(i, j);
		int lam = 0;
		if (i == j) lam = max(0, (maxo - 2 + mode) / 3);
		else lam = max(0, (mino - 1 + mode) / 3);
		pii nv = { i - (lam   * 3), j - lam * 3 };
		if (nv.first + 3 < 5 && nv.second + 3 < 5) {
			nv.first += 3;
			nv.second += 3;
		}
		return nv;
	}
	void medial_reduce_naive() {
		if (beta == 0 && i == 0 && j > 0)
			beta++, j--;
		if (alpha == 0 && j == 0 && i > 0)
			alpha++, i--;
		pii reduced;
		if (alpha == 0 && beta == 0) {
			//bool f1 = j >= i, f2 = i > j;
			reduced = reduce_two(j, i, -2 );
			i = reduced.first; j = reduced.second;
		}
		else if (alpha == 1 && beta == 1) {
			reduced = reduce_two(j, i, 0);
			i = reduced.first; j = reduced.second;
		}
		else if (alpha == 0 && beta == 1) {
			pii reduced;
			if (i >= REDP + 2 && beta + j >= REDP + 1) {
				reduced = reduce_two(j, i, 0);
				i = reduced.first; j = reduced.second;

			}

		}
		else if (alpha == 1 && beta == 0) {
			pii reduced;
			if (j >= REDP + 2 && alpha + i >= REDP + 1) {
				reduced = reduce_two(j, i, 0);
				i = reduced.first; j = reduced.second;

			}

		}
	}
	void updated_medial_reduce() {
		//cout << "U NEED TO CHANGE ORDER IN REDUCE TWO ";
		//return;
		if (beta == 0 && i == 0 && j > 0)
			beta++, j--;
		if (alpha == 0 && j == 0 && i > 0)
			alpha++, i--;
		pii reduced;
		if (beta + j > alpha + i || (beta + j == alpha + i && beta == 1)) {
			// form bab
			reduced = reduce_two(j - (beta == 0) , i, 0);
			reduced.second += (beta == 0);

		}
		else {
			// form aba
			reduced = reduce_two(j - (beta == 0), i - (alpha == 0), 1);
			reduced.first += (alpha == 0);
			reduced.second += (beta == 0);
		}
		i = reduced.first, j = reduced.second;
	}
	void medial_reduce() {
		//return;
		t beta2 = beta, i2 = i, j2 = j, alpha2 = alpha;

		if (beta2 == 0 && i2 == 0 && j2 >= 1) {
			j2--;
			beta2 = 1;
		}
		if (alpha2 == 0 && j2 == 0 && i2 >= 1) {
			i2--;
			alpha2 = 1;
		}
		int mino = min(i2, j2);
		int maxo = max(i2, j2);
		if (mino <= REDP || maxo <= REDP + 1) return; // not reducible
		int div = mino - mino % REDP;
		if (div == mino) {
			div -= REDP;
		}
		i2 -= div;
		j2 -= div;
		
		mino = min(i2, j2);
		maxo = max(i2, j2);

		if (maxo <= 1) {
			i2 += REDP;
			j2 += REDP;
		}
		set_vals({ beta2,i2,j2,alpha2 });


		return;
		t gamma, delta, liambda, miu, pos;
		if (!medial_is_reducible()) return;
		gamma = alpha + i;
		delta = beta + j;
		liambda = abs(gamma - delta);
		miu = min(gamma, delta);

		if (miu == gamma)pos = 2;
		else pos = 3;

		t miu_1 = miu % 3;
		if (miu_1 != 2)
			miu_1 = miu_1 + 3;
		if (miu_1 == 2 && pos == 2 && beta == 0)
			miu_1 = miu_1 + 3;
		if (miu_1 == 2 && pos == 3 && alpha == 0)
			miu_1 = miu_1 + 3;
		if (pos == 2)
			set_vals({ beta, miu_1 - alpha, miu_1 - beta + liambda, alpha });
		else
			set_vals({ beta, miu_1 - alpha + liambda, miu_1 - beta, alpha });
	}
	void medial_reduce_old() {
		t gamma, delta, liambda, miu, pos;
		if (!medial_is_reducible()) return;
		gamma = alpha + i;
		delta = beta + j;
		liambda = abs(gamma - delta);
		miu = min(gamma, delta);

		if (miu == gamma)pos = 2;
		else pos = 3;

		t miu_1 = miu % 3;
		if (miu_1 != 2)
			miu_1 = miu_1 + 3;
		if (miu_1 == 2 && pos == 2 && beta == 0)
			miu_1 = miu_1 + 3;
		if (miu_1 == 2 && pos == 3 && alpha == 0)
			miu_1 = miu_1 + 3;
		if (pos == 2)
			set_vals({ beta, miu_1 - alpha, miu_1 - beta + liambda, alpha });
		else
			set_vals({ beta, miu_1 - alpha + liambda, miu_1 - beta, alpha });
		if (i + 3 < 5 && j + 3 < 5) {
			i += 3;
			j += 3;
		}
		if (beta == 1 && i == 5 && j == 4) {
			i -= 3;
			j -= 3;
		}
		else if (beta == 0 && i == 4 && j == 5 && alpha == 1) {
			i -= 3;
			j -= 3;
		}
	}
	bool is_empty() {
		return alpha == 0 && i == 0 && j == 0 && beta == 0;
	}
	bool operator < (const word  w2) const {
		if (beta < w2.beta) return true;
		if (beta == w2.beta && i < w2.i) return true;
		if (beta == w2.beta && i == w2.i && j < w2.j) return true;
		if (beta == w2.beta && i == w2.i && j == w2.j && alpha < w2.alpha) return true;
		return false;
	}
	bool operator == ( word w2) {
		return alpha == w2.alpha && i == w2.i && j == w2.j && beta == w2.beta;
	}
	word operator*(word w2) {
		if (w2.is_empty()) return { beta, i, j, alpha };
		else if (is_empty()) return w2;
		return{ beta, i + alpha + w2.i, j + w2.beta + w2.j, w2.alpha };
	}
	word inverse_word() {
		int beta2 = (beta == 0 && i >= 1);
		int alpha2 = (alpha == 0 && j >= 1);
		int i2 = (j - alpha2) + beta;
		int j2 = (i - beta2) + alpha;

		return word(beta2, i2, j2, alpha2);
	}
};
struct complex {
	t imag, real;
	string mode;
	complex() {}
	complex(int _real, int _imag, string _mode) : imag(_imag), real(_real), mode(_mode) {}
	complex(vector<t> v, string _mode) {
		real = v[0];
		imag = v[1];
		mode = _mode;
	}
	bool operator==(complex w2) {
		return imag == w2.imag && real == w2.real && mode == w2.mode;
	}
	bool operator < (complex w2) {
		return imag < w2.imag || (imag == w2.imag && real < w2.real) || (imag == w2.imag && real == w2.real && mode < w2.mode);
	}
};

template<typename T> T naive_power(T w, t pow) {
	T result = w;
	for (int i = 0; i < pow - 1; i++) {
		result = result * w;
	}
	return result;
}
template<typename T, typename T2> T raise_power(T w, T2 pow) {
	if (pow == 0) {
		return word(0, 0, 0, 0);
	}
	//return naive_power(w, pow);
	T result = w; pow--;
	while (pow > 0) {
		if (pow & 1) result = result * w;
		w = w * w;
		pow /= 2;
	}
	return result;
}

word raise_power_complex(word w, complex power) {
	word first_word = word(0, 0, 0, 0), second_word = word(0, 0, 0, 0);
	if (power.imag == 0) return raise_power(w, power.real);
	else if (power.real == 0) return raise_power(w.inverse_word(), power.imag);
	if (power.mode == "cr") {
		// complex goes first, then real
		first_word = w.inverse_word();
		first_word = raise_power(first_word, power.imag);
		second_word = raise_power(w, power.real);
	}
	else if (power.mode == "rc") {
		// real first, then complex
		first_word = raise_power(w, power.real);
		second_word = w.inverse_word();
		second_word = raise_power(second_word, power.imag);
	}
	return first_word * second_word;
}
const word EMPTY_WORD = word(0, 0, 0, 0);
const double eps = 1e-9;

ostream& operator<<(ostream & os, const word & w) {
	os << "[ " << w.beta << ", " << w.i << ", " << w.j << ", " << w.alpha << " ]";
	return os;
}
ostream& operator<<(ostream & os, const complex & w) {
	if (w.mode == "rc")
		os << "[ " << w.real << " + " << w.imag << "i " << " ]";
	else
		os << "[ " << w.imag << "i + " << w.real << " ]";
	return os;
}

template<typename T> void print_mat(T mat[M][M], ostream & os=cout) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			os << mat[i][j] << ", ";
		}
		os << endl;
	}
	os << endl << endl;
}
template<typename T> void print_mat_circulant(T mat[M], ostream & os = cout) {
	for (int i = 0; i < M; i++) {
			os << mat[i] << ", ";
	}
	os << endl;
}

void mpf_complex(complex X[][M], word W[][M], complex Y[][M], word(&res)[M][M]) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			for (int k = 0; k < M; k++) {
				if (k == 0) // first iteration
					res[i][j] = raise_power_complex(W[k][j], X[i][k]);
				else {
					word other_word = raise_power_complex(W[k][j], X[i][k]);
					res[i][j] = res[i][j] * other_word;
				}
				res[i][j].medial_reduce();
			}
		}
	}
	word res2[M][M];
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			for (int k = 0; k < M; k++) {
				if (k == 0) // first iteration
					res2[i][j] = raise_power_complex(res[i][k], Y[k][j]);
				else
					res2[i][j] = res2[i][j] * raise_power_complex(res[i][k], Y[k][j]);
				res2[i][j].medial_reduce();
			}
		}
	}
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			res[i][j] = res2[i][j];
		}
	}
}
template<typename T> void mpf(t X[][M], T W[][M], t Y[][M], T(&res)[M][M]) {
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			for (int k = 0; k < M; k++) {
				for (int l = 0; l < M; l++) {
					if (k == 0 && l == 0) // first iteration
						res[i][j] = raise_power(W[k][l], X[i][k] * Y[l][j]);
					else
						res[i][j] = res[i][j] * raise_power(W[k][l], X[i][k] * Y[l][j]);
					//res[i][j].medial_reduce();
				}
			}
			res[i][j].medial_reduce_old();
		}
	}
}

int tries = 0, success = 0;
// X, Y - cirkuliantu matricos
//t X[M][M] = { { 1, 2, 7 },{ 7, 1, 2 },{ 2, 7, 1 } }, Y[M][M] = { { 4, 5, 5 } ,{ 5, 4, 5 },{ 5, 5, 4 } };
//t X[M][M] = { { 1, 2},{ 7, 1 } }, Y[M][M] = { { 4, 5} ,{ 5, 4 } };
t X[M][M], Y[M][M];
//word W[M][M];
/*t X[M][M] = {
	{ 3, 8, 7, 4 },
{ 4, 3, 8, 7 },
{ 7, 4, 3, 8 },
{ 8, 7, 4, 3 }
};
t Y[M][M] = {
	{ 2, 5, 6, 7 },
{ 7, 2, 5, 6 },
{ 6, 7, 2, 5 },
{ 5, 6, 7, 2 }
};*/

word W[M][M];

word required_word[M][M], dummy[M][M];
t try_x[M][M], try_y[M][M];

template<typename T> void fill_circulant(T(&arr)[M][M]) {
	for (int i = 1; i < M; i++) {
		for (int j = 0; j < M; j++) {
			arr[i][j] = arr[i - 1][(j - 1 + M) % M];
		}
	}
}

// x ir y matricos turi determinatna 0 . viena ir abi matricos turi 0 determinanta
// 
void try_all(int col) {
	// Generating only M * 2 entries
	if (col == 2 * M) {
		// Baigiau su 3.5M
		tries++;
		if (tries < -1) return;
		// Construct circulant from first row
		fill_circulant(try_x);
		fill_circulant(try_y);
		bool ok = true;
		if (rand() % 100 == -1) {
			bool ok2 = false;
			for (int i = 0; i < M; i++) for (int j = 0; j < M; j++)if (try_x[i][j] == 8) ok2 = true;
			if (ok2) {
				print_mat(try_x);
				print_mat(try_y);
			}
		}


		mpf(try_x, W, try_y, dummy);
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < M; j++) {
				if (!(required_word[i][j] == dummy[i][j])) {
					ok = false;
					i = 1000; break;
				}
			}
		}
		if (ok) {
			cout << " ok mat is :  \n";
			print_mat(try_x);
			print_mat(try_y);
			cout << "\n\n";
			success++;
		}
		return;
	}
	if (col < M)
		for (int i = 0; i <= X_LIMIT; i++) {
			try_x[0][col] = i;
			try_all(col + 1);
		}
	else
		for (int i = 0; i <= X_LIMIT; i++) {
			try_y[0][col - M] = i;
			try_all(col + 1);
		}
}

vvs do_mpf_check(vvs X, vvs W, vvs Y) {

	vvs ans = W;
	for (int i = 0; i < ans.size(); i++) {
		for (int j = 0; j < ans[i].size(); j++) {
			ans[i][j] = "";
		}
	}
	// let's do mpf now
	for (int i = 0; i < ans.size(); i++) {
		for (int j = 0; j < ans[i].size(); j++) {
			for (int k = 0; k < ans[i].size(); k++) {
				for (int l = 0; l < ans[i].size(); l++) {
					if (k != 0 || l != 0) ans[i][j] += " * ";
					ans[i][j] += W[k][l] + " ^(" + X[i][k] + " * " + Y[l][j] + ")";
				}
			}
		}
	}
	return ans;
}
void mpf_complex_test() {
	cout << "Testing inversions..\n";
	vector<vector<word > > testing = {
		{ word(1, 0, 4, 1), word(0, 5, 1, 0) },
	{ word(0, 0, 4, 0), word(0, 3, 0, 1) },
	{ word(0, 4, 0, 0), word(1, 0, 3, 0) },
	{ word(1, 5, 2, 0), word(0, 2, 5, 1) },
	{ word(0,4,3,1), word(1,3,4,0) }, // new edit
	{ word(1,3,1,1), word(0,2,4,0) },
	{ word(0,5,4,0), word(1,3,4,1) },
	{ word(0,2,4,1), word(1,4,2,0) } };


	for (auto test : testing) {
		cout << test[0] << "  -->  " << test[0].inverse_word() << " ||  " << test[1] << "  -- " << ((test[0].inverse_word() == test[1]) ? "Ok" : "WRONG!") << "\n";
	}

	cout << "Inversion testing done.\n";
	cout << "Testing raise_power_complex..\n";
	vector< tuple< word, complex, word> > test_complex;
	test_complex.push_back({ word(1,0,4,1), complex(1, 0, "rc"), word(1,0,4,1) });
	test_complex.push_back({ word(0,4,3,1), complex(0, 1, "cr"), word(1,3,4,0) });
	test_complex.push_back({ word(1,5,2,0), complex(0, 1, "rc"), word(0,2,5,1) });
	test_complex.push_back({ word(1,0,4,1), complex(1, 0, "rc"), word(1,0,4,1) });
	test_complex.push_back({ word(0,2,3,1), complex(2, 2, "rc"), word(0, 12, 12, 0) });
	test_complex.push_back({ word(0,2,3,1), complex(2, 2, "cr"), word(1, 11, 11, 1) });
	test_complex.push_back({ word(0,2,3,1), complex(2, 3, "rc"), word(0, 15, 15, 0) });

	for (auto test : test_complex) {
		word w; complex c; word ans;
		tie(w, c, ans) = test;
		cout << w << "^" << c << "  -->  " << raise_power_complex(w, c) << " ||  " << "  -- " << ((raise_power_complex(w, c) == ans) ? "Ok" : "WRONG!") << "\n";
	}


	cout << "Done testing raise_poweR_complex.\n";


	cout << "Starting mpf_complex testing..\n";
	if (M == 3) {
		/*
		word ww[M][M] = {
			{ word(0, 4, 3, 1), word(0, 1, 0, 0), word(1,1,0,0) },
			{ word(0,1,1,0), word(1,3,1,1), word(0,5,4,0) },
			{ word(1,4,0,0), word(1, 1, 3, 0), word(0,2,4,1) }
		};
		complex xx[M][M] = {
			{ complex(1, 2, "cr"), complex(2, 1, "cr"), complex(3, 3, "cr") },
			{ complex(3, 3, "cr"), complex(1, 2, "cr"), complex(2, 1, "cr") },
			{ complex(2, 1, "cr"), complex(3, 3, "cr"), complex(1, 2, "cr") }
		};
		complex yy[M][M] = {
			{ complex(0, 3, "rc"), complex(0, 2, "rc"), complex(0, 4, "rc") },
			{ complex(0, 4, "rc"), complex(0, 3, "rc"), complex(0, 2, "rc") },
			{ complex(0, 2, "rc"), complex(0, 4, "rc"), complex(0, 3, "rc") }
		};
		word res_mat[M][M];
		mpf_complex(xx, ww, yy, res_mat);
		cout << "Resmat raised1 :\n";
		print_mat(res_mat);
		cout << "\n------------\n";
		complex uu[M][M] = {
		{ complex(2, 3, "rc"), complex(1, 1, "rc"), complex(2, 1, "rc") },
		{ complex(2, 1, "rc"), complex(2, 3, "rc"), complex(1, 1, "rc") },
		{ complex(1, 1, "rc"), complex(2, 1, "rc"), complex(2, 3, "rc") }
		};
		complex vv[M][M] = {
			{ complex(2, 2, "rc"), complex(4, 1, "rc"), complex(1, 3, "rc") },
		{ complex(1, 3, "rc"), complex(2, 2, "rc"), complex(4, 1, "rc") },
		{ complex(4, 1, "rc"), complex(1, 3, "rc"), complex(2, 2, "rc") }
		};
		word res_mat2[M][M];
		mpf_complex(uu, ww, vv, res_mat2);
		print_mat(res_mat2);

		word key1[M][M];
		word key2[M][M];
		mpf_complex(uu, res_mat, vv, key1);
		mpf_complex(xx, res_mat2, yy, key2);
		cout << "key1 : \n";
		print_mat(key1);

		cout << "\n key2: \n";
		print_mat(key2);


		cout << "mpf_complex testing done.\n";
		*/
	}
	else {
		/*
		word ww[M][M] = {
			{ word(0, 4, 3, 1), word(0, 1, 0, 0)},
			{ word(0,1,1,0), word(1,3,1,1)}
		};
		complex xx[M][M] = {
			{ complex(1, 2, "cr"), complex(2, 1, "cr")},
		{ complex(2, 1, "cr"), complex(1, 2, "cr") }
		};
		complex yy[M][M] = {
			{ complex(0, 3, "cr"), complex(0, 2, "cr")},
		{ complex(0, 2, "cr"), complex(0, 3, "cr")}
		};
		word res_mat[M][M];
		mpf_complex(xx, ww, yy, res_mat);
		cout << "mul mat1 : \n";
		print_mat(res_mat);
		cout << "\n------------\n";
		complex uu[M][M] = {
			{ complex(2, 3, "rc"), complex(1, 1, "rc") },
			{ complex(1,1 , "rc"), complex(2, 3, "rc") }
		};
		complex vv[M][M] = {
			{ complex(2, 2, "rc"), complex(4, 1, "rc")},
			{ complex(4, 1, "rc"), complex(2, 2, "rc")}
		};
		word res_mat2[M][M];
		mpf_complex(uu, ww, vv, res_mat2);
		cout << "mul mat2 : \n";

		print_mat(res_mat2);

		word key1[M][M];
		word key2[M][M];
		mpf_complex(uu, res_mat, vv, key1);
		mpf_complex(xx, res_mat2, yy, key2);
		cout << "key1 : \n";
		print_mat(key1);

		cout << "\n key2: \n";
		print_mat(key2);


		cout << "mpf_complex testing done.\n";*/

	}
	/*vvs xx = { {"2i + 1", "i + 2", "3i + 3"},
				{"3i + 3", "2i + 1", "i + 2"},
				{"i + 2", "3i + 3", "2i + 1"} };
	vvs yy = { { "3i", "2i", "4i" },
	{ "4i", "3i", "2i" },
	{ "2i", "4i", "3i" } };
	vvs ww = { { "a^4*b^3*a", "a", "b*a" },
	{ "a*b", "b*a^3*b*a", "a^5*b^4" },
	{ "b*a^4", "b*a*b^3", "a^2*b^4*a" } };
	*/
	/*vvs xx = { { "a", "b", "c" },
	{ "d", "e", "f" },
	{ "g", "h", "i" } };
	vvs yy = { { "1", "2", "3" },
	{ "4", "5", "6" },
	{ "7", "8", "9" } };
	vvs ww = { { "w_11", "w_12", "w_13" },
	{ "w_21", "w_22", "w_23" },
	{ "w_31", "w_32", "w_33" } };
	auto res = do_mpf_check(xx, ww, yy);
	for (int i = 0; i < res.size(); i++) {
		for (int j = 0; j < res[i].size(); j++) {
			cout << res[i][j] << " | ";
		}
		cout << "\n";
	}*/
	// Let's raise w by complex_compon

}
word complex_word[M][M];
word req_word_complex[M][M];
complex XC[M][M], YC[M][M];
complex try_complex_x[M][M], try_complex_y[M][M];
struct word_mat {
	word mat[M][M];
	word_mat() {}
	bool operator ==(word_mat mat2) {
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < M; j++) {
				if (!(mat2.mat[i][j] == mat[i][j])) return false;
			}
		}
		return true;
	}
};

bool operator < ( word_mat  mat1,  word_mat  mat2) {
	for (int i = 0; i < M; i++)
		for (int j = 0; j < M; j++)
			if (mat1.mat[i][j] < mat2.mat[i][j])
				return true;
			else if (mat1.mat[i][j] == mat2.mat[i][j])
				continue;
			else return false;
	return false;
}
struct complex_mat {
	complex mat[M];
	complex_mat() {}
	bool operator < (complex_mat mat2) {
		for (int i = 0; i < M; i++) if (mat[i] < mat2.mat[i]) return true;
		return false;
	}

};

struct int_mat {
	int mat[M];

	int_mat() {}
	bool operator < (int_mat m2) {
		for (int i = 0; i < M; i++) if (mat[i] < m2.mat[i]) return true;
		return false;
	}
};
word_mat mat2word_mat(word (&mat)[M][M]) {
	word_mat temp = word_mat();
	for (int i = 0; i < M; i++)for (int j = 0; j < M; j++) temp.mat[i][j] = mat[i][j];
	return temp;
}
complex_mat mat2complex_mat(complex (&mat)[M][M]) {
	complex_mat temp = complex_mat();
	for (int i = 0; i < M; i++) for (int j = 0; j < M; j++) temp.mat[i] = mat[0][i];
	return temp;
}

int_mat mat2int_mat(t(&mat)[M][M]) {
	int_mat mt = int_mat();
	for (int i = 0; i < M; i++)
		mt.mat[i] = mat[0][i];
	return mt;
}

map<word_mat, vector<complex_mat> > all_results;
map<word_mat, vector<int_mat> > all_results_regular;
map<word_mat, int> all_results_compressed;


void try_all_logging(int col, bool compressed = true, int tot_ones_x = 0, int tot_ones_y = 0) {
	// Generating only M * 2 entries
	if (col == 2 * M) {
		if (tot_ones_x == M or tot_ones_x == 0 or tot_ones_y == M or tot_ones_y == 0) {
			return;
		}
		// Baigiau su 3.5M
		tries++;
		//if (tries < -1) return;
		// Construct circulant from first row
		fill_circulant(try_x);
		fill_circulant(try_y);
		
		mpf(try_x, complex_word, try_y, dummy);

		// check dummy conds => containts of at least two distinct elements in its anti-circulant diagonals
		for (int i = 0; i < M; i++) {
			set<word> w_diag;
			for (int j = 0; j < M; j++) {
				w_diag.insert(dummy[(i - j + M) % M][(i + j) % M]);
			}
			if (w_diag.size() == 1 && M != 1) {
				return;
			}
		}

		auto matr = mat2word_mat(dummy);
		if (compressed) {
			all_results_compressed[matr]++;
			all_results_compressed[matr]++;
		}
		else {
			all_results_regular[matr].push_back(mat2int_mat(try_x));
			all_results_regular[matr].push_back(mat2int_mat(try_y));
		}
		return;
	}
	if (col < M) {
		if (col == M - 1 && tot_ones_x == 0) {
			try_x[0][col] = 1;
			try_all_logging(col + 1, compressed, tot_ones_x + 1, tot_ones_y);
			return;
		}
		for (int i = (tot_ones_x == (M - 1)) + 1; i <= X_LIMIT; i++) {
			try_x[0][col] = i;
			try_all_logging(col + 1, compressed, tot_ones_x + (i == 1));
		}
	}
	else {
		if (col == 2 * M - 1 && tot_ones_y == 0) {
			try_y[0][col - M] = 1;
			try_all_logging(col + 1, compressed, tot_ones_x, tot_ones_y + 1);
			return;
		}
		for (int i = (tot_ones_y == (M - 1)) + 1 ; i <= Y_LIMIT; i++) {
			try_y[0][col - M] = i;
			try_all_logging(col + 1, compressed, tot_ones_x, tot_ones_y + (i == 1));
		}
	}
}
void try_all_complex(int col) {

	if (col == M) {

		tries++;
		if (tries % 100000 == 0) {
			cout << "curr tries :" << tries << "\n";
		}
		// Construct circulant from first row
		fill_circulant(try_complex_x);
		fill_circulant(try_complex_y);

		//for (int i = 0; i < M; i++) for (int j = 0; j < M; j++) try_complex_x[i][j] = XC[i][j];
		//for (int i = 0; i < M; i++) for (int j = 0; j < M; j++) try_complex_y[i][j] = YC[i][j];
		bool ok = true;
		mpf_complex(try_complex_x, complex_word, try_complex_y, dummy);

		auto matr = mat2word_mat(dummy);
		all_results[matr].push_back(mat2complex_mat(try_complex_x));
		all_results[matr].push_back(mat2complex_mat(try_complex_y));
		/*for (int i = 0; i < M; i++) {
			for (int j = 0; j < M; j++) {
				if (!(req_word_complex[i][j] == dummy[i][j])) {
					ok = false;
					i = 1000; break;
				}
			}
		}
		if (ok) {
			cout << "successes: " << success << " tries: " << tries << "\n";
			cout << " ok mat is :  \n";
			print_mat(try_complex_x);
			print_mat(try_complex_y);
			fd_logging << "#" << success << " : " << endl;
			fd_logging << "Ok X Matrix is: " << endl;
			print_mat(try_complex_x, fd_logging);
			fd_logging << "Ok Y Matrix is: " << endl;
			print_mat(try_complex_y, fd_logging);
			fd_logging << endl;
			cout << "\n\n";
			success++;
		}*/
		return;
	}
	for (int i = 0; i <= X_LIMIT; i++) {
		for (int j = 0; j <= Y_LIMIT; j++) {
			for (int k = 0; k <= X_LIMIT; k++) {
				for (int l = 0; l <= Y_LIMIT; l++) {
					for (int modex = 0; modex <= 1 - ((i == 0 || k == 0) ? 1 : 0); modex++) {
						for (int modey = 0; modey <= 1 - ((j == 0 || k == 0) ? 1 : 0); modey++) {
							try_complex_x[0][col] = complex(i, k, (modex == 0) ? "rc" : "cr");
							try_complex_y[0][col] = complex(j, l, (modey == 0) ? "rc" : "cr");
							try_all_complex(col + 1);
						}
					}
				}
			}
		}
	}
}
complex getComplex(int maxDegReal, int maxDegImag) {
	complex res;
	res.imag = rand() % maxDegImag;
	res.real = rand() % maxDegReal;
	int md = rand() % 2;
	if (md == 0) res.mode = "rc";
	else res.mode = "cr";
	return res;
}

void regular_try_all_logging(string lambda_det, string miu_det, bool logging = true, int seed = 3, bool compressed = true,string folder="2x2") {
	tries = 0;
	if (compressed)
		all_results_compressed.clear();
	else
		all_results_regular.clear();
	srand(seed);

	string file_name_logging = "results\\every_log\\anti_circulant_w\\"+folder+"\\mpf_sols_regular_from1_" + to_string(int(X_LIMIT)) + "_" + to_string((int)Y_LIMIT)
		+ "_" + lambda_det + "_" + miu_det + "_" + to_string(seed) + ".txt";
	if (logging)fd_logging.open(file_name_logging);

	fd_logging << "N,X limit, Y limit" << endl;
	fd_logging << M << ", " << X_LIMIT << ", " << Y_LIMIT << endl;

	cout << "Public param W: \n";
	//if (logging)fd_logging << "Public param W: " << endl;

	print_mat(complex_word);
	print_mat(complex_word, fd_logging);

	try_all_logging(0, compressed);

	// write all complex matrices
	if (compressed) {
		for (auto & i : all_results_compressed) {
			fd_logging << i.second << " ";
			continue;
			fd_logging << "}" << endl;
			print_mat(i.first.mat, fd_logging);

			// output all ok matrices
			fd_logging << "_";
			for (int j = 0; j < i.second; j++) {
				fd_logging << " _";
			}
		}
	}
	else {
		for (auto & i : all_results_regular) {
			fd_logging << "}" << endl;
			print_mat(i.first.mat, fd_logging);

			// output all ok matrices
			fd_logging << "_";
			for (int j = 0; j < i.second.size(); j++) {
				print_mat_circulant(i.second[j].mat, fd_logging);
				fd_logging << "_";
			}
		}
	}
	if (logging)fd_logging.close();


}
void complex_try_all_logging(string lambda_det, string miu_det, bool logging = true, int seed = 3) {
	tries = 0;
	all_results.clear();
	srand(seed);

	string file_name_logging = "results\\every_log\\mpf_sols_" + to_string(int(X_LIMIT)) + "_" + to_string((int)Y_LIMIT)
		+ "_" + lambda_det + "_" + miu_det + "_" + to_string(seed) + ".txt";
	if (logging)fd_logging.open(file_name_logging);
	
	fd_logging<< "N,X limit, Y limit" << endl;
	fd_logging << M << ", " << X_LIMIT << ", " << Y_LIMIT << endl;

	cout << "Public param W: \n";
	//if (logging)fd_logging << "Public param W: " << endl;

	print_mat(complex_word);
	print_mat(complex_word, fd_logging);


	try_all_complex(0);

	// write all complex matrices
	for (auto & i : all_results) {
		fd_logging << "}" << endl;
		print_mat(i.first.mat, fd_logging);

		// output all ok matrices
		fd_logging << "_";
		for (int j = 0; j < i.second.size(); j++) {
			print_mat_circulant(i.second[j].mat, fd_logging);
			fd_logging << "_";
		}
	}
	if (logging)fd_logging.close();


}
void complex_try(string lambda_det, string miu_det, bool logging = true) {
	int seed = 123;
	srand(seed);

	string file_name_logging = "results\\mpf_sols_" + to_string(int(X_LIMIT)) + "_" + to_string((int)Y_LIMIT)
		+ "_" + lambda_det + "_" + miu_det + "_" + to_string(seed) + ".txt";
	if(logging)fd_logging.open(file_name_logging);

	// gen random complex matrices of x and y
	for (int i = 0; i < M; i++) {
		//XC[0][i] = getComplex(X_LIMIT, Y_LIMIT);
		//YC[0][i] = getComplex(X_LIMIT, Y_LIMIT);
	}
	XC[0][0] = complex(2, 2, "cr");
	XC[0][1] = complex(2, 2, "rc");

	YC[0][0] = complex(3, 3, "rc");
	YC[0][1] = complex(3, 3, "cr");

	fill_circulant(XC);
	fill_circulant(YC);
	// display
	cout << "X_complex matrix: \n";
	print_mat(XC);

	if (logging)fd_logging << "X_complex matrix: " << endl;
	if (logging)print_mat(XC, fd_logging);
	if (logging)fd_logging.flush();

	// display
	cout << "Y_complex matrix: \n";
	if (logging)fd_logging << "Y_complex matrix: " << endl;
	print_mat(YC);
	if (logging)print_mat(YC, fd_logging);

	cout << "Public param W: \n";
	if (logging)fd_logging << "Public param W: " << endl;
	print_mat(complex_word);
	print_mat(complex_word, fd_logging);

	mpf_complex(XC, complex_word, YC, req_word_complex);

	cout << "req word: \n";
	if (logging)fd_logging << "req matrix: " << endl;

	print_mat(req_word_complex);
	if (logging)print_mat(req_word_complex, fd_logging);


	if (logging || 1 == 1)try_all_complex(0);

	// write all complex matrices



	if (logging)fd_logging.close();
	string file_name = "results\\mpf_summary_" + to_string(int(X_LIMIT)) + "_" + to_string((int)Y_LIMIT)
		+ "_" + lambda_det + "_" + miu_det + "_" + to_string(seed) + ".txt";
	ofstream fd;
	if (logging)fd.open(file_name);

	if (logging)fd << "total,successes,n_dimens,x_limit,y_limit,lambda_det,miu_det" << endl;
	if (logging)fd << tries << "," << success << "," << M << "," << X_LIMIT << "," << Y_LIMIT << "," << lambda_det << "," << miu_det << endl;

	if (logging)fd.close();

}
set<word_mat> active_mats;
map<pii, int> cnts;
word get_random_word(int limit) {
	int b, i, j, a;
	i = rand() % (limit+1);
	j = rand() % (limit+1);
	if (i > 0) {
		a = 1;
		i--;
	}
	if (j > 0) {
		b = 1;
		j--;
	}
	if (b == i && i == j && j == a && j == 0)return get_random_word(limit);
	word ret_word = word(b, i, j, a);
	ret_word.medial_reduce();
	return ret_word;
}
void generate_Ws(int cnt = 5, int numb_limit = 10) {
	srand(42);
	while (1) {
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < M; j++) {
				complex_word[i][j] = get_random_word(numb_limit);
			}
		}
		//check if doesnt exist
		if (active_mats.find(mat2word_mat(complex_word)) != active_mats.end()) {
			continue;
		}

	}

}
void order_check() {
	word w = word(1, 3, 4, 1);
	complex c1 = complex(4, 1, "rc");
	complex c2 = complex(1, 4, "cr");

	cout << raise_power_complex(w, c1) << " " << raise_power_complex(w, c2);
}

void read_and_fill(string path, int order = 3, string folder = "2x2", bool use_regular=true) {
	ifstream dd(path);
	// read and populate
	int t; dd >> t;
	int iter = 0;
	while (t--) {
		cout << "Current iteration - " << iter << "\n";
		int seed = 1000 + t;
		double miu, lam; dd >> miu >> lam;
		int miuint = miu; int lamint = lam;
		for (int i = 0; i < order; i++) {
			for (int j = 0; j < order; j++) {
				// read word
				int b, ii, jj, a; dd >> b >> ii >> jj >> a;
				complex_word[i][j] = word(b, ii, jj, a);
			}
		}
		// solve eq
		if (use_regular) {
			regular_try_all_logging(to_string(lamint), to_string(miuint), 1, seed, false, folder);
		}
		else {
			complex_try_all_logging(to_string(lamint), to_string(miuint), 1, seed);
		}

		int xd = tries;
		int asd = 4;

	}
}
word random_mat[M][M];
word dummy_res[M][M];
int curr_circ[M];
void fill_random_mat() {
	// Fill random matrix assuming delta has circulant form and the sum of this circulant is 1
	int sum = 0;
	for (int i = 0; i < M - 1; i++) {
		curr_circ[i] = rand() % 16 - 8;
		sum += curr_circ[i];
	}
	if (sum < 0) {
		sum = -sum;
	}
	curr_circ[M - 1] = 1 - sum;
	int beta, ii, jj, alpha;
	int delta, reqs;
	// generate word now
	for (int i = 0; i < M; i++) {
		for (int j = 0; j < M; j++) {
			beta = rand() % 2, alpha = rand() % 2;
			ii = rand() % 16;
			delta = beta - alpha - ii;
			jj = curr_circ[(i + j) % M] - delta;
			if (jj < 0) {
				swap(ii, jj);
				swap(alpha, beta);
			}
			random_mat[i][j] = word(beta, i, j, alpha);
		}
	}
}

void find_average_maximum_distr_slow() {
	// Slow, correct

	int ITERS_W = 100;

	int ITERS_PRK = 100;

	int x[M][M], y[M][M];
	vector<int> max_vals;
	word mpf_res[M][M];
	map<int, int> cnts;
	for (int i = 0; i < ITERS_W; i++) {
		if (i % 10 == 0) {
			cout << i << " \n";
		}
		// construct random-ish w, that has its deltas sum up to 1
		int csum = 0;
		fill_random_mat();
		// generate many random private keys
		for (int j = 0; j < ITERS_PRK; j++) {
			for (int k = 0; k < M; k++) {
				x[0][k] = rand() % 8;
				y[0][k] = rand() % 8;
			}
			fill_circulant(x);
			fill_circulant(y);
			// do actual MPF
			mpf(x, random_mat, y, mpf_res);
			int max_val = 0;
			int this_delta = 0;
			// get max val
			for (int k = 0; k < M; k++) {
				for (int l = 0; l < M; l++) {
					max_val = max(max_val, mpf_res[k][l].get_lambda());
				}
			}
			cnts[max_val]++;

			//max_val = max(max_val, val);
		}
		// do actual MPF
		//max_vals.push_back(max_val);
	}

	sort(max_vals.begin(), max_vals.end());
	for (int i = 0; i < 100; i++) {
		//cout << max_vals.back() << "\n";
		//max_vals.pop_back();
	}
	// save it
	ofstream out("C:\\Users\\a\\Documents\\projects\\MPF\\algos\\results\\max_words_slow_correct\\max_word_2\\out.txt");
	out << cnts.size() << endl;
	for (auto &i : cnts) {
		out << i.first << " " << i.second << ",";
	}

}

void find_average_maximum_distr() {
	int ITERS_W = 100000;

	int ITERS_PRK = 1000;

	vector<int> x(M), y(M), w(M);
	vector<int> max_vals;
	map<int, int> cnts;
	for (int i = 0; i < ITERS_W; i++) {
		// construct random-ish w, that has its deltas sum up to 1
		int csum = 0;
		int max_val = 0;
		for (int j = 0; j < M; j++) {
			int wi = rand() % 11 - 5;
			csum += wi;
			w[j] = wi;
		}
		int needed = 1 - csum;
		w[M - 1] = needed;
		
		// generate many random private keys
		for (int j = 0; j < ITERS_PRK; j++) {
			for (int k = 0; k < M; k++) {
				x[k] = rand() % 8;
				y[k] = rand() % 8;
			}
			// get max val
			int val = 0;
			for (int k = 0; k < M; k++) {
				int sumo = 0;
				for (int l = 0; l < M; l++) {
					sumo += y[l] * w[(k - l + M) % M];
				}
				val += sumo * x[k];
			}
			max_val = max(max_val, val);
		}
		cnts[max_val]++;
		//max_vals.push_back(max_val);
	}

	sort(max_vals.begin(), max_vals.end());
	for (int i = 0; i < 100; i++) {
		//cout << max_vals.back() << "\n";
		//max_vals.pop_back();
	}
	// save it
	ofstream out("C:\\Users\\a\\Documents\\projects\\MPF\\algos\\results\\max_word_15\\out.txt");
	out << cnts.size() << endl;
	for (auto &i : cnts) {
		out << i.first << " " << i.second << ",";
	}

}

void find_average_maximum_distr_explicit() {
	int ITERS_W = 1000;

	int ITERS_PRK = 1000;

	vector<int> x(16), y(16), w(16);
	vector<int> max_vals;

	word dummy[M][M];
	word Wd[M][M];

	int xx[M][M], yy[M][M];

	for (int i = 0; i < ITERS_W; i++) {
		// construct random-ish w, that has its deltas sum up to 1
		int csum = 0;
		int max_val = 0;
		for (int j = 0; j < 16; j++) {
			int wi = rand() % 11 - 5;
			csum += wi;
			w[j] = wi;
		}
		// make it anticurclant or smth
		for (int j = 0; j < 16; j++) {
			for (int k = 0; k < 16; k++) {
				//Wd[i][j] = w[(j + k) % 16];
			}
		}
		int needed = 1 - csum;
		w[15] = needed;

		// generate many random private keys
		for (int j = 0; j < ITERS_PRK; j++) {
			for (int k = 0; k < 16; k++) {
				x[k] = rand() % 8;
				y[k] = rand() % 8;
			}
			// get max val
			int val = 0;
			for (int k = 0; k < 16; k++) {
				int sumo = 0;
				for (int l = 0; l < 16; l++) {
					sumo += y[l] * w[(k - l + 16) % 16];
				}
				val += sumo * x[k];
			}
			max_val = max(max_val, val);
		}
		max_vals.push_back(max_val);
	}

	sort(max_vals.begin(), max_vals.end());
	for (int i = 0; i < 100; i++) {
		cout << max_vals.back() << "\n";
		max_vals.pop_back();
	}
	int s = 55;

}
void do_skirst() {
	srand(40);

	word w1 = word(0, 1, 2, 0);
	word w2 = word(0, 1, 2, 0);
	for (int i = 0; i < 500; i++) {
		for (int j = 0; j < 500; j++) {
			word resWord = raise_power(w1, i) * raise_power(w2, j);
			resWord.medial_reduce_naive();
			int x = 5;
		}
	}

	vector<word> test_words = { word(0, 2, 3, 0),
								word(0, 3, 2, 0),
								word(1, 5, 2, 1),
								word(1, 2, 2, 1)
	};
	for (word w : test_words) {
		for (int i = 1; i <= 100; i++) {

		}
	}
}
int main()
{
	find_average_maximum_distr_slow();
	find_average_maximum_distr();
	return 0;
	//word testo = word(0, 2, 4, 0);
	//word gorem = raise_power(testo, 30);
	//
	//
	//
	//do_skirst();
	//return 0;
	//// generate some word w
	//word inword[M][M];
	//int lambdamat[M][M];
	//for (int i = 0; i < M; i++) {
	//	for (int j = 0; j < M; j++) {
	//		inword[i][j] = { rand() % 2, rand() % 6, rand() % 6, rand() % 2};
	//		lambdamat[i][j] = inword[i][j].get_lambda();
	//	}
	//}
	//int xmat[M][M], ymat[M][M];
	//for (int i = 0; i < M; i++) {
	//	for (int j = 0; j < M; j++) {
	//		xmat[i][j] = rand() % 16;
	//		ymat[i][j] = rand() % 16;
	//	}
	//}
	//print_mat(xmat);
	//cout << "\ny:\n";
	//print_mat(ymat);
	////int frr; cin >> frr;
	////return 0;
	//{
	//	// dump the lambdamat file
	//	char location[] = "C:\\Users\\a\\Documents\\projects\\MPF\\algos\\maple\\mat.txt";
	//
	//	freopen(location, "w", stdout);
	//	for (int i = 0; i < M; i++) {
	//		for (int j = 0; j < M; j++) {
	//			cout << lambdamat[i][j] << " ";
	//		}
	//		cout << endl;
	//	}
	//}
	//word dummyres[M][M];
	//mpf(xmat, inword, ymat, dummyres);
	//
	//
	//int lambdaA[M][M];
	//for (int i = 0; i < M; i++) {
	//	for (int j = 0; j < M; j++) {
	//		lambdaA[i][j] = dummyres[i][j].get_lambda();
	//	}
	//}
	//{
	//	// dump the lambdamat of A  file
	//	char location[] = "C:\\Users\\a\\Documents\\projects\\MPF\\algos\\maple\\matA.txt";
	//
	//	freopen(location, "w", stdout);
	//	for (int i = 0; i < M; i++) {
	//		for (int j = 0; j < M; j++) {
	//			cout << lambdaA[i][j] << " ";
	//		}
	//		cout << endl;
	//	}
	//}
	{
		//word ww[3][3] = {
		//	{word(0, 2, 2, 0),  word(0, 7, 15, 0), word(0, 3, 9, 0) },
		//	{word(0, 3, 9, 0),  word(0, 2, 2, 0),  word(0, 7,15 , 0)},
		//	{word(0, 7, 15, 0), word(0, 3, 9, 0),  word(0, 2, 2, 0)}
		//};


		word ww[3][3] = {
			{word(1, 2, 2, 0),  word(0, 4, 7, 0),  word(1, 14, 6, 0) },
			{word(0, 3, 6, 0),  word(1, 14, 6, 0),  word(1, 3, 3 , 0)},
			{word(1, 14, 6, 0),  word(0, 2, 3, 0),  word(1, 1, 4, 1)}
		};
		
		/*word ww[3][3] = {
		{word(1, 5, 2, 0),  word(0, 4, 7, 0),  word(1, 1, 4, 1) },
		{word(0, 5, 3, 0),  word(0, 4, 7, 0),  word(1, 1, 4 , 1)},
		{word(0, 4, 3, 1),  word(1, 3, 5, 0),  word(1, 1, 4, 1)}
		};*/
		cout << "init matrix : \n";
		//print_mat(ww);

		{
			t xx[3][3] = {
				{3, 6, 9},
				{9, 3, 6},
				{6, 9, 3}
			};
			t yy[3][3] = {
				{7, 2, 3},
				{3, 7, 2},
				{2, 3, 7}
			};
			word dummytest[3][3];
			cout << "public key : \n";
			//mpf(xx, ww, yy, dummytest);
			//print_mat(dummytest);
		}
	}
	//int g; cin >> g; 
	//return 0;
	//mpf(xx, ww, yy, dummytest);


	t xxx[2][2] = {
		{4 , 41},
		{41, 1}
	};
	t yyy[3][3] = {
		{2, 1},
		{1, 2}
	};

	/*
	t xxx[2][2] = {
		{1, 2},
		{2, 1}
	};
	t yyy[2][2] = {
		{40, 0},
		{0, 40}
	};
	*/
	//mpf(xxx, ww, yyy, dummytest);

	
	//print_mat(dummytest);


	vector<word> test_words = { word(0, 10, 11, 1) , word(0,1, 2, 1), // ok
								word(1, 16, 16, 1), word(1, 4, 4, 1), // ok
								word(1, 17,17, 1), word(1, 2, 2, 1),  // ok
								word(1, 5, 4, 1), word(1, 2, 1, 1),   // ok
								word(1, 4, 5, 1), word(1, 1, 2, 1),   // ok
								word(0, 4, 5, 1), word(0, 1, 2, 1),   // ok
								word(0, 5, 4, 1), word(0,5, 4, 1),    // ok
								word(1, 4 ,5 ,0), word(1, 4, 5, 0),   // ok
								word(1, 5, 4, 0), word(1, 2, 1, 0),   // ok
								word(0, 15, 20, 1), word(0, 3, 8, 1), // ok
								word(0, 20, 15, 1), word(0, 8, 3, 1), // 
								word(0, 19, 24, 1), word(0, 4, 9, 1),
								word(0, 10, 10, 0), word(0, 4, 4, 0), // ok
								word(0, 14, 15, 0), word(0, 5, 6, 0), //////////////
								word(0, 15, 15, 0), word(0, 3, 3, 0),  // ok
								word(0, 15, 12, 0), word(0, 6, 3, 0),
								word(0, 12, 15, 0), word(0, 3, 6, 0),
								word(1, 6, 4, 0), word(1, 6, 4, 0)
	};
	for (int i = 0; i < test_words.size(); i+=2) {
		auto test_word = test_words[i];
		auto ans = test_words[i + 1];
		cout << "test word : ------------ " << test_word << "\n\n";
		word tt_new = test_word;
		word test_word_old = test_word;
		word test_word_updated_reduce = test_word;
		tt_new.medial_reduce();
		test_word_old.medial_reduce_old();
		test_word_updated_reduce.medial_reduce_naive();

		cout << "             reduced_new:" << tt_new << "\n";
		cout << "             reduced_old:" << test_word_old << "\n\n";
		cout << "             reduced_upd:" << test_word_updated_reduce<< "\n\n";
		if ( !(test_word_old == ans)) {
			cout << "WRONG!\n";
		}
	}
	read_and_fill("C:\\Users\\a\\Documents\\projects\\MPF\\algos\\results\\input\\analyze2x2_circ_consraints_nobug_sigmas_sum_to_1.txt", M, "full_constrained_sum1_2x2_5");
	//complex_try_all_logging("0", "1", 1);
	cout << success << " " << tries << "\n";
	int qweqwe;
	cin >> qweqwe;
	return 0;
	/*word ww[M][M] = {
		{ word(0, 4, 3, 1), word(0, 1, 0, 0) },
		{ word(0,1,1,0), word(1,3,1,1) }
	};
	complex xx[M][M] = {
		{ complex(1, 2, "cr"), complex(2, 1, "cr") },
		{ complex(2, 1, "cr"), complex(1, 2, "cr") }
	};
	complex uu[M][M] = {
		{ complex(0, 3, "cr"), complex(0, 2, "cr") },
		{ complex(0, 2, "cr"), complex(0, 3, "cr") }
	};

	word mat_res1[M][M];
	word mat_res2[M][M];
	mpf_complex(xx, ww, xx, mat_res1);
	mpf_complex(uu, ww, uu, mat_res2);
	word w1 = raise_power_complex(word(0, 4, 3, 1), complex(1, 2, "cr")); w1.medial_reduce();
	word w2 = raise_power_complex(word(0, 1, 0, 0), complex(2, 1, "cr")); w2.medial_reduce();
	cout << "words are : " << w1 << " ..  " << w2 << "\n";
	cout << "first entry should be : " << w1 * w2  << "\n";

	cout << "\\\\\\\\n";
	word rr = raise_power_complex(word(0, 4, 3, 1), complex(1, 2,"cr"));
	cout << "raised stuff is : " << rr << "\n";
	cout << "\\\\\\\\n";
	cout << "first entry should also be: \n";
	word ww1 = word(0, 4, 3, 1);
	word ww2 = word(0, 1, 0, 0);
	word ww3 = word(0, 1, 1, 0);
	word ww4 = word(1, 3, 1, 1);


	complex cc1 = complex(1, 2, "cr");
	complex cc2 = complex(2, 1, "cr");

	complex dd1 = complex(1, 2, "cr");
	complex dd2 = complex(2, 1, "cr");

	complex outer1 = complex(2, 3, "rc");
	complex outer2 = complex(1, 1, "rc");
	word raised_word = raise_power_complex(raise_power_complex(ww1, cc1) * raise_power_complex(ww2, cc2), outer1) *
		raise_power_complex(raise_power_complex(ww3, dd1) * raise_power_complex(ww4, dd2), outer2);
	raised_word.medial_reduce();
	cout << raised_word << "\n";
	cout << "\\\\\\\\n";*/

	mpf_complex_test();



	word reduk_word = word(0, 159, 168, 1);
	reduk_word.medial_reduce();
	// TEST
	{
		word a = word(0, 1, 2, 1);
		word b = word(1, 2, 3, 0);
		word c = word(1, 0, 5, 1);
		word d = word(1, 8, 4, 1);

		t xx[M_DEFAULT][M_DEFAULT] = { {1, 2}, {3, 4} };
		t yy[M_DEFAULT][M_DEFAULT] = { {5, 6}, {7, 8} };
		word powmat[M_DEFAULT][M_DEFAULT] = { {a,b},
						{c,d} };
		word rr[M_DEFAULT][M_DEFAULT];
		word c_square = c * c; c_square.medial_reduce();
		word c_forth = raise_power(c, 4);
		word d_square = d * d; d_square.medial_reduce();
		word b_times_d_square = b * d_square; b_times_d_square.medial_reduce();
		word a_times_c_square = a * c_square; a_times_c_square.medial_reduce();

		word raised_fifth = raise_power(a * raise_power(c, 2), 5);
		word a_third = raise_power(a, 3);
		//mpf(xx, powmat, yy, rr);
		word expect_to_get = raise_power(a * raise_power(c, 2), 5) * raise_power(b * raise_power(d, 2), 7);
		word expect_to_get_last_word = raise_power((raise_power(a, 3) * raise_power(c, 4)), 6) *
			raise_power((raise_power(b, 3) * raise_power(d, 4)), 8);
		expect_to_get.medial_reduce();
		int xdddddddddddddddd = 44;
	}

	// Propogate shifts from first row(circulant thingie)

	//

	// Bazines daugybos patikra 
	word a = word(0, 0, 0, 1);
	word b = word(1, 0, 0, 0);

	word abab = a * b * a * b;
	word baba = b * a * b * a;
	word bab = b * a * b;
	//

	// Kelimo laipsniu patikra
	word init_word = word(1, 13, 6, 1);
	word raise_fast = raise_power(init_word, 75);
	word raise_slow = naive_power(init_word, 75);
	cout << raise_fast << "\n" << raise_slow << "\n";
	//

	mpf(X, W, Y, required_word);
	/*t X1[M][M] = { {1, 4},
	{4, 1} };
	t Y1[M][M] = {
	{2, 3},
	{3, 2} };

	t U1[M][M] = {
	{ 5, 4 },
	{ 4, 5 } };
	t V1[M][M] = {
	{ 7, 6 },
	{ 6, 7 } };
	word g1[M][M], key_1[M][M];

	mpf(X1, W, Y1, g1);
	mpf(U1, g1, V1, key_1);

	word g2[M][M], key_2[M][M];

	mpf(U1, W, V1, g2);
	mpf(X1, g2, Y1, key_2);

	*/
	int asd = 4;

	try_all(0);
	cout << success << " " << tries << "\n";

	return 0;
}