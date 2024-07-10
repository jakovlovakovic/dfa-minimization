#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>
#include <list>

using namespace std;

typedef map<pair<string, string>, string> delta_funkcija;

// dobi sva dohvatljiva stanja
set<string> dobi_dohvatljiva(const set<string>& abeceda,
	delta_funkcija& dlta, const string& pocetno_stanje, set<string> dohvatljiva_stanja) {
	// potrebne strukture
	bool stop_recursion = true;

	// iteracija
	for (const auto& stanje : dohvatljiva_stanja) {
		for (const auto& prijelaz : abeceda) {
			if (dlta.count(make_pair(stanje, prijelaz)) != 0) {
				if (dohvatljiva_stanja.count(dlta[make_pair(stanje, prijelaz)]) == 0) {
					dohvatljiva_stanja.insert(dlta[make_pair(stanje, prijelaz)]);
					stop_recursion = false;
				}
			}
		}
	}
	// rekurzija
	if (stop_recursion) {
		return dohvatljiva_stanja;
	}
	else {
		return dobi_dohvatljiva(abeceda, dlta, pocetno_stanje, dohvatljiva_stanja);
	}
}

// pomocna funkcija za rekurzivno oznacavanje parova
void rekurzivno_oznaci_parove_u_listi(map<pair<string, string>, list<pair<string, string>>>& lista_para,
	vector<pair<string, string>>& oznaceni_parovi, vector<pair<string, string>> par_za_oznaciti) {
	vector<pair<string, string>> za_iducu_iteraciju;
	bool stop_recursion = true;

	for (const auto& par : par_za_oznaciti) {
		if (find(oznaceni_parovi.begin(), oznaceni_parovi.end(), par) == oznaceni_parovi.end()) {
			oznaceni_parovi.push_back(par);
			za_iducu_iteraciju.push_back(par);
			stop_recursion = false;
		}
		for (const auto& par_iz_liste : lista_para[par]) {
			if (find(oznaceni_parovi.begin(), oznaceni_parovi.end(), par_iz_liste) == oznaceni_parovi.end()) {
				oznaceni_parovi.push_back(par_iz_liste);
				za_iducu_iteraciju.push_back(par_iz_liste);
				stop_recursion = false;
			}
		}
	}

	if (stop_recursion) {
		return;
	}
	else {
		rekurzivno_oznaci_parove_u_listi(lista_para, oznaceni_parovi, za_iducu_iteraciju);
	}
}

// istovjetna
vector<pair<string, string>> istovjetna(const set<string>& dohvatljiva, const set<string>& prihvatljiva_stanja,
	const set<string>& abeceda, delta_funkcija dlta) {
	// potrebne podatkovne strukture
	vector<string> stanja;
	vector<pair<string, string>> oznaceni_parovi;
	map<pair<string, string>, list<pair<string, string>>> lista_para;

	// stavi sve elemente iz dohvatljivih stanja u vector
	for (const auto& str : dohvatljiva) {
		stanja.push_back(str);
	}

	for (size_t i = 0; i < stanja.size() - 1; i++) {
		for (size_t j = i + 1; j < stanja.size(); j++) {
			// za svaki temp_arr[i] i temp_arr[i + 1]
			// provjeri jesu li u razlicitim skupovima F i Q
			if (prihvatljiva_stanja.count(stanja[i]) != 0 &&
				prihvatljiva_stanja.count(stanja[j]) == 0) {
				oznaceni_parovi.push_back(make_pair(stanja[i], stanja[j]));
			}
			else if (prihvatljiva_stanja.count(stanja[i]) == 0 &&
				prihvatljiva_stanja.count(stanja[j]) != 0) {
				oznaceni_parovi.push_back(make_pair(stanja[i], stanja[j]));
			}
		}
	}

	// drugi dio algoritma
	for (size_t i = 0; i < stanja.size() - 1; i++) {
		for (size_t j = i + 1; j < stanja.size(); j++) {
			// ako postoji znak za koji je (delta(p, a), delta(q, a)) je oznacen
			bool flag_is_oznacen_par = false;
			for (const auto& znak : abeceda) {
				pair<string, string> temp_par0 = make_pair(dlta[make_pair(stanja[i], znak)], dlta[make_pair(stanja[j], znak)]);
				if (find(oznaceni_parovi.begin(), oznaceni_parovi.end(), temp_par0) != oznaceni_parovi.end()) {
					flag_is_oznacen_par = true;
				}
				else {
					pair<string, string> temp_par1 = make_pair(dlta[make_pair(stanja[j], znak)], dlta[make_pair(stanja[i], znak)]);
					if (find(oznaceni_parovi.begin(), oznaceni_parovi.end(), temp_par1) != oznaceni_parovi.end()) {
						flag_is_oznacen_par = true;
					}
				}
			}
			if (flag_is_oznacen_par) {
				// oznaci (p, q)
				vector<pair<string, string>> temp_vec_za_oznaciti;
				temp_vec_za_oznaciti.push_back(make_pair(stanja[i], stanja[j]));
				// rekurzivno_oznaci_parove_u_listi
				rekurzivno_oznaci_parove_u_listi(lista_para, oznaceni_parovi, temp_vec_za_oznaciti);
			}
			if (!flag_is_oznacen_par) {
				// ako za par ne postoji znak koji vodi par u oznacena stanja
				// za sve znakove
				for (const auto& znak : abeceda) {
					string stanje_a = dlta[make_pair(stanja[i], znak)];
					string stanje_b = dlta[make_pair(stanja[j], znak)];
					if (stanje_a > stanje_b) {
						string temp_s = stanje_a;
						stanje_a = stanje_b;
						stanje_b = temp_s;
					}
					if (stanje_a != stanje_b) {
						pair<string, string> par_za_listu = make_pair(stanja[i], stanja[j]);
						// provjeri postoji li par (delta(p, a), delta(p, a)) postoji u mapi
						if (lista_para.find(make_pair(stanje_a, stanje_b)) != lista_para.end()) {
							lista_para[make_pair(stanje_a, stanje_b)].push_back(par_za_listu);
						}
						// ako ne postoji, dodaj par u mapu par (delta(p, a), delta(p, a)) i prvi element u listu
						else {
							list<pair<string, string>> temp_lista_parova;
							temp_lista_parova.push_back(par_za_listu);
							lista_para[make_pair(stanje_a, stanje_b)] = temp_lista_parova;
						}
					}
				}
			}
		}
	}

	// return
	return oznaceni_parovi;
}

int main(void) {
	// potrebne strukture
	vector<string> lines;
	set<string> stanja;
	set<string> abeceda;
	set<string> prihvatljiva_stanja;
	string pocetno_stanje;
	delta_funkcija dlta;

	string linija;
	while (getline(cin, linija)) {
		lines.push_back(linija);
	}

	// odvajanje stanja
	stringstream stanjastream(lines[0]);
	string stanje;
	while (getline(stanjastream, stanje, ',')) {
		stanja.insert(stanje);
	}

	// odvajanje znakova abecede
	stringstream abecedastream(lines[1]);
	string znak;
	while (getline(abecedastream, znak, ',')) {
		abeceda.insert(znak);
	}

	// odvajanje prihvatljivih stanja
	stringstream prihvatljivastream(lines[2]);
	string prihvatljivo_stanje;
	while (getline(prihvatljivastream, prihvatljivo_stanje, ',')) {
		prihvatljiva_stanja.insert(prihvatljivo_stanje);
	}

	// pocetno stanje
	pocetno_stanje = lines[3];

	// dobi funkcije prijelaza
	for (size_t i = 4; i < lines.size(); i++) {
		stringstream prijelazistream(lines[i]);
		string stanjeprije, znak, stanjenakon, strelica;
		// dobi 'stanjeprije'
		getline(prijelazistream, stanjeprije, ',');
		// dobi 'znak'
		getline(prijelazistream, znak, '-');
		// dobi '->'
		getline(prijelazistream, strelica, '>');
		// dobi 'stanjenakon'
		getline(prijelazistream, stanjenakon);
		dlta[make_pair(stanjeprije, znak)] = stanjenakon;
	}

	// dobi sva dohvatljiva stanja i neistovjetna stanja
	set<string> dohvatljiva_stanja;
	dohvatljiva_stanja.insert(pocetno_stanje);
	dohvatljiva_stanja = dobi_dohvatljiva(abeceda, dlta, pocetno_stanje, dohvatljiva_stanja);
	delta_funkcija temp_dlta = dlta;
	for (auto const& elem : dlta) {
		string stanjef = elem.first.first;
		string stanjes = elem.second;
		if (dohvatljiva_stanja.find(stanjef) == dohvatljiva_stanja.end() ||
			dohvatljiva_stanja.find(stanjes) == dohvatljiva_stanja.end()) {
			temp_dlta.erase(make_pair(stanjef, elem.first.second));
		}
	}
	dlta = temp_dlta;
	vector<pair<string, string>> neistovjetna_stanja = istovjetna(dohvatljiva_stanja, prihvatljiva_stanja, abeceda, dlta);

	// dobi sva istovjetna stanja
	set<pair<string, string>> set_parova_dohvatljivih;
	vector<string> privremena_lista_dohvatljivih;
	for (auto const& dohvatljiva : dohvatljiva_stanja) {
		privremena_lista_dohvatljivih.push_back(dohvatljiva);
	}
	for (size_t i = 0; i < privremena_lista_dohvatljivih.size() - 1; i++) {
		for (size_t j = i + 1; j < privremena_lista_dohvatljivih.size(); j++) {
			set_parova_dohvatljivih.insert(make_pair(privremena_lista_dohvatljivih[i], privremena_lista_dohvatljivih[j]));
		}
	}
	set<pair<string, string>> istovjetna;
	for (auto const& par_dohvatljivih : set_parova_dohvatljivih) {
		if (find(neistovjetna_stanja.begin(), neistovjetna_stanja.end(), par_dohvatljivih) == neistovjetna_stanja.end()) {
			istovjetna.insert(par_dohvatljivih);
		}
	}

	// iz svih istovjetnih stanja, dobi sva stanja za removanje
	list<pair<string, string>> temp_lista_istovjetnih;
	for (auto const& element : istovjetna) {
		temp_lista_istovjetnih.push_front(element);
	}
	set<string> za_removeati;
	for (auto const& ele : temp_lista_istovjetnih) {
		za_removeati.insert(ele.second);
	}

	// odavde nadalje se pod dohvatljiva stanja misli na finalna
	// iz svih dohvatljivih removaj sva istovjetna
	for (auto const& remove : za_removeati) {
		dohvatljiva_stanja.erase(remove);
	}

	// azuriraj prihvatljiva stanja
	set<string> temp_st;
	for (auto pst : prihvatljiva_stanja) {
		for (auto const& ist_stanja : temp_lista_istovjetnih) {
			if (ist_stanja.second == pst) {
				pst = ist_stanja.first;
			}
		}
		temp_st.insert(pst);
	}
	prihvatljiva_stanja = temp_st;
	set<string> temp_sta;
	for (auto psta : prihvatljiva_stanja) {
		if (dohvatljiva_stanja.find(psta) != dohvatljiva_stanja.end()) {
			temp_sta.insert(psta);
		}
	}
	prihvatljiva_stanja = temp_sta;

	// azuriraj delta funkcije
	delta_funkcija temp_delta;
	for (auto const& value : dlta) {
		string dltst = value.first.first;
		string dltznak = value.first.second;
		string dltrez = value.second;
		for (auto const& ista_stanja : temp_lista_istovjetnih) {
			if (ista_stanja.second == dltst) {
				dltst = ista_stanja.first;
			}
		}
		for (auto const& ista_stanja : temp_lista_istovjetnih) {
			if (ista_stanja.second == dltrez) {
				dltrez = ista_stanja.first;
			}
		}
		pair<string, string> tmp_dlt_pair = make_pair(dltst, dltznak);
		temp_delta[tmp_dlt_pair] = dltrez;
	}
	dlta = temp_delta;

	// pocetna stanja azuriraj
	while (za_removeati.find(pocetno_stanje) != za_removeati.end()) {
		if (pocetno_stanje == temp_lista_istovjetnih.front().second) {
			pocetno_stanje = temp_lista_istovjetnih.front().first;
		}
		temp_lista_istovjetnih.remove(temp_lista_istovjetnih.front());
	}

	// ispis
	size_t counter_zareza = 0;
	for (auto const& dohv : dohvatljiva_stanja) {
		if (counter_zareza >= dohvatljiva_stanja.size() - 1) {
			cout << dohv << endl;
		}
		else {
			cout << dohv << ",";
		}
		counter_zareza = counter_zareza + 1;
	}
	counter_zareza = 0;
	for (auto const& abc : abeceda) {
		if (counter_zareza >= abeceda.size() - 1) {
			cout << abc << endl;
		}
		else {
			cout << abc << ",";
		}
		counter_zareza = counter_zareza + 1;
	}
	counter_zareza = 0;
	if (!prihvatljiva_stanja.empty()) {
		for (auto const& prihv : prihvatljiva_stanja) {
			if (counter_zareza >= prihvatljiva_stanja.size() - 1) {
				cout << prihv << endl;
			}
			else {
				cout << prihv << ",";
			}
			counter_zareza = counter_zareza + 1;
		}
	}
	else {
		cout << endl;
	}
	cout << pocetno_stanje << endl;
	for (auto const& dlta_red : dlta) {
		cout << dlta_red.first.first << "," << dlta_red.first.second << "->" << dlta_red.second << endl;
	}

	return 0;
}