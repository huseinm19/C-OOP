#include<iostream>
#include <vector>
#include <string>
#include <mutex>
#include <regex>
#include<exception>
#include <tuple>
#include <chrono>
#include <thread>
#include <fstream>

using namespace std;



/****************************************************************************

1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR

2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"

3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA

4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.

5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.

****************************************************************************/



const char* nedozvoljena_operacija = "Nedozvoljena operacija";

const char* not_set = "NOT_SET";



enum Sortiranje { ASC, DESC };

enum Predmet { MATEMATIKA, FIZIKA, HEMIJA, GEOGRAFIJA, NOT_SET };

enum Razred { I = 1, II, III, IV };



char* AlocirajNizKaraktera(const char* sadrzaj) {

	if (sadrzaj == nullptr)

		return nullptr;

	int vel = strlen(sadrzaj) + 1;

	char* temp = new char[vel];

	strcpy_s(temp, vel, sadrzaj);

	return temp;

}

template <class T1, class T2, int max>

class Kolekcija {

	T1* _elementi1[max] = { nullptr };

	T2* _elementi2[max] = { nullptr };

	int _trenutnoElemenata;

	bool _dozvoliDupliranje;

	Sortiranje _sortiranje;

public:

	Kolekcija(Sortiranje sortiranje = ASC, bool dozvoliDupliranje = true) {

		_trenutnoElemenata = 0;

		_sortiranje = sortiranje;

		_dozvoliDupliranje = dozvoliDupliranje;

	}

	~Kolekcija() {

		for (size_t i = 0; i < _trenutnoElemenata; i++) {

			delete _elementi1[i]; _elementi1[i] = nullptr;

			delete _elementi2[i]; _elementi2[i] = nullptr;

		}

	}

	int GetTrenutno() const { return _trenutnoElemenata; }

	int GetMax() const { return max; }

	void Sortiraj() {
		bool pronadeno = false;
		T1 temp = 0;
		T2 temp2 = 0;
		for (int i = 0; i < _trenutnoElemenata; i++) {
			for (int j = 0; j < _trenutnoElemenata; j++) {
				if (_sortiranje == ASC) {
					if (*_elementi1[i] < *_elementi1[j]) {
						swap(_elementi1[i], _elementi1[j]);
						swap(_elementi2[i], _elementi2[j]);
					}
				}
				else {
					if (*_elementi1[i] > * _elementi1[j]) {
						swap(_elementi1[i], _elementi1[j]);
						swap(_elementi2[i], _elementi2[j]);
					}
				}
			}
		}
	}

	void AddElement(const T1& el1, const T2& el2) {
		if (_trenutnoElemenata >= max - 1) {
			throw exception("Prekoracen broj elemenata\n");
		}
		else {
			if (!_dozvoliDupliranje) {
				for (int i = 0; i < _trenutnoElemenata; i++) {
					if ((*_elementi1[i] == el1) && (*_elementi2[i] == el2))
						throw exception("Zabranjeno dupliranje elementa");
				}
			}
			_elementi1[_trenutnoElemenata] = new T1(el1);
			_elementi2[_trenutnoElemenata++] = new T2(el2);
			Sortiraj();
		}
	}

	void setTrenutno() { _trenutnoElemenata += 1; }

	Kolekcija(const Kolekcija& k1) {
		_trenutnoElemenata = k1._trenutnoElemenata;

		for (int i = 0; i < _trenutnoElemenata; i++) {
			_elementi1[i] = new T1(*k1._elementi1[i]);
			_elementi2[i] = new T2(*k1._elementi2[i]);
		}

		_sortiranje = k1._sortiranje;
		_dozvoliDupliranje = k1._dozvoliDupliranje;
	}

	T1& GetElement1(int lokacija) const {

		if (lokacija < 0 || lokacija >= _trenutnoElemenata)

			throw exception(nedozvoljena_operacija);

		return *_elementi1[lokacija];

	}

	T2& GetElement2(int lokacija) const {

		if (lokacija < 0 || lokacija >= _trenutnoElemenata)

			throw exception(nedozvoljena_operacija);

		return *_elementi2[lokacija];

	}

	int getTrenutno2() { return _trenutnoElemenata; }

	void SetElement2(int lokacija, const T2& ele2) {

		_elementi2[lokacija] = new T2(ele2);

	}

	void SetElement1(int lokacija, const T1& ele1) {

		_elementi1[lokacija] = new T1(ele1);

	}

	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {

		for (size_t i = 0; i < obj.GetTrenutno(); i++)

			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;

		return COUT;

	}

};



class Aktivnost {

	shared_ptr<Razred> _razred;

	string _opis;

	int _ocjena;//1-5

public:

	Aktivnost(Razred razred = I, int ocjena = 0, string opis = not_set) {

		_ocjena = ocjena;

		_opis = opis;

		_razred = make_shared<Razred>(razred);

	}

	//u okviru zasebnog thread-a se salje SMS sa sadrzajem "Uspjesno ste okoncali aktivnosti u okviru X razreda sa prosjecnom ocjenom X.X"*/

	int  GetOcjenu() const { return _ocjena; }

	string GetOpis() const { return _opis; }

	Razred GetRazred() const { return *_razred; }



	friend ostream& operator<<(ostream& COUT, const Aktivnost& obj) {

		COUT << *obj._razred << " " << obj._ocjena << " " << obj._opis << endl;

		return COUT;

	}

	friend bool operator ==(const Aktivnost& a, const Aktivnost& b);

};
bool operator ==(const Aktivnost& a, const Aktivnost& b) {
	if (a.GetOcjenu() == b.GetOcjenu() && a.GetOpis() == b.GetOpis() && a.GetRazred() == b.GetRazred())
		return true;
	return false;
}


class Polaznik {

protected:

	char* _imePrezime;

	string _brojTelefona;

public:
	Polaznik() {
		char* _imePrezime = nullptr;
		string _brojTelefona = not_set;
	}
	Polaznik(string imePrezime, string brojTelefona) : _imePrezime(AlocirajNizKaraktera(imePrezime.c_str())) {

		_brojTelefona = brojTelefona;
	}
	Polaznik(const Polaznik& p) {
		_imePrezime = AlocirajNizKaraktera(p._imePrezime);
		_brojTelefona = p._brojTelefona;
	}

	~Polaznik() { delete[] _imePrezime; }

	char* GetImePrezime() { return _imePrezime; }
	char  GetIme() { return *_imePrezime; }

	string GetTelefon() { return _imePrezime; }

	virtual void PredstaviSe() = 0;


};



class Ucenik :Polaznik {

	Kolekcija<Predmet, Aktivnost, 16>* _aktivnosti;
	float _prosijek = 0;

public:

	~Ucenik() { delete _aktivnosti; _aktivnosti = nullptr; }

	Kolekcija<Predmet, Aktivnost, 16>& GetAktivnosti() { return *_aktivnosti; };

	void SMS() {
		cout << "Uspjesno ste okoncali aktivnost u okviru" << " sa prosjecnom ocjenom " << endl;
	}

	void setProsijek(int a) { _prosijek = a; }
	int getProsijek() { return _prosijek; }

	friend ostream& operator<<(ostream& COUT, Ucenik& n)

	{

		COUT << "Ime ucenika: " << n.GetImePrezime() << endl;
		cout << " Broj telefona ucenika je " << n.Getbroj() << endl;
		cout << endl;
		cout << "Aktivnosti: " << n.GetAktivnosti() << endl;
		cout << "-----------------------------------" << endl;
		return COUT;

	}
	char* GetImep() { return _imePrezime; }
	string Getbroj() { return _brojTelefona; }

	Ucenik(string ime, string brojTelefona, int _trenutno = 0) : Polaznik(ime, brojTelefona) {
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>;
	}
	Ucenik(const Ucenik& u, int _trenutno = 0) : Polaznik(u._imePrezime, u._brojTelefona) {
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>;
	}
	friend bool operator==(char a, string s);

	void PredstaviSe() {}
};

bool operator==(const char* a, string s) {
	string x(a);
	if (s == x)
		return true;
	return false;
}

class Skola {

	char* _naziv;

	vector<Ucenik> _ucenici;

public:

	Skola(const char* naziv = nullptr) {

		_naziv = AlocirajNizKaraktera(naziv);

	}

	~Skola() {

		delete[] _naziv; _naziv = nullptr;

	}

	char* GetNaziv()const { return _naziv; }


	vector<Ucenik>& GetUcenici() { return _ucenici; };

	bool SpasiUFajl(string naziv, bool spasi = false) {
		ofstream upis(naziv);
		if (!spasi) {
			upis << "Naziv skole: " << _naziv << endl;
			for (int i = 0; i < _ucenici.size(); i++) {
				upis << "Ime ucenika: " << _ucenici[i].GetImep() << endl;
				upis << "Broj telefona ucenika" << _ucenici[i].Getbroj() << endl;
			}
			upis.close();

			ifstream ispis(naziv);
			if (ispis.fail()) {
				cout << "Nije uspjesno" << endl;
			}
			else {
				char znak;
				while (ispis.get(znak)) {
					cout << znak;
				}
				cout << endl;
			}
			return true;
		}
		else {
			upis.clear();
			upis << "Naziv škole: " << _naziv << endl;
			for (int i = 0; i < _ucenici.size(); i++) {
				upis << "Ime ucenika: " << _ucenici[i].GetImep() << endl;
				upis << "Broj telefona ucenika" << _ucenici[i].Getbroj() << endl;
				cout << endl;
			}
			upis.close();

			ifstream ispis(naziv);
			if (ispis.fail()) {
				cout << "Nije uspjesno" << endl;
			}
			else {
				char znak;
				while (ispis.get(znak)) {
					cout << znak;
				}
				cout << endl;
			}
			return true;
		}
		return false;

	}

	void operator () (string ime, string brojTelefona) {
		string broj = brojTelefona;
		vector<string>::iterator it;
		//koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX
		string pravilo = "(\\+)(\\d{3})([ ])?(\\()?(\\d{2})(\\))?([ ])?(\\d{3})([- ])(\\d{3})";
		if (!regex_match(broj, regex(pravilo))) {
			brojTelefona = not_set;
		}
		for (int i = 0; i < _ucenici.size(); i++) {
			if (_ucenici[i].GetImep() == ime || brojTelefona == _ucenici[i].Getbroj())
				throw exception("Ista imena kod dva ucenika ili isti broj telefona");
		}
		Ucenik novi(ime, brojTelefona);

		_ucenici.push_back(novi);
	}

	bool DodajAktivnost(string ime, Predmet predmet, Aktivnost aktivnost) {
		int brojAktivnost = 0;


		for (int i = 0; i < _ucenici.size(); i++) {
			if (_ucenici[i].GetAktivnosti().GetTrenutno() > 0 && _ucenici[i].GetAktivnosti().GetElement2(i).GetRazred() == aktivnost.GetRazred()) {
				if (_ucenici[i].GetAktivnosti().GetElement1(i) == predmet && _ucenici[i].GetAktivnosti().GetElement2(i) == aktivnost) {
					return false;
				}
			}

			if (_ucenici[i].GetAktivnosti().GetTrenutno() > 0 && _ucenici[i].GetImep() == ime && _ucenici[i].GetAktivnosti().GetElement1(i) == predmet) {
				return false;
			}

			if (_ucenici[i].GetAktivnosti().GetTrenutno() > 0) {
				if (_ucenici[i].GetAktivnosti().GetElement2(i).GetRazred() == aktivnost.GetRazred()) {
					if (_ucenici[i].GetAktivnosti().GetTrenutno() > 4)
						return false;
				}
			}

			if ((_ucenici[i].GetAktivnosti().GetTrenutno() > 0) && (_ucenici[i].GetAktivnosti().GetElement2(i).GetOcjenu() > 1)) {
				string pravilo = "(\\+)(\\d{3})([ ])?(\\()?(\\d{2})(\\))?([ ])?(\\d{3})([- ])(\\d{3})";
				if (regex_match(_ucenici[i].Getbroj(), regex(pravilo))) {
					thread thread1(&Ucenik::SMS, ref(_ucenici[i]));
					thread1.join();
				}
			}

			if (_ucenici[i].GetImep() == ime) {
				_ucenici[i].GetAktivnosti().SetElement1(_ucenici[i].GetAktivnosti().GetTrenutno(), predmet);
				_ucenici[i].GetAktivnosti().SetElement2(_ucenici[i].GetAktivnosti().GetTrenutno(), aktivnost);
				_ucenici[i].GetAktivnosti().setTrenutno();
				return true;
			}
		}
		return false;
	}
	void Ispis(string naziv) {
		ifstream ispis(naziv);
		if (ispis.fail()) {
			cout << "Nije uspjesno" << endl;
		}
		else {
			char znak;
			while (ispis.get(znak)) {
				cout << znak;
			}
			cout << endl;
		}
	}
	pair<Polaznik*, float> GetNajboljegUcenika() {
		float prosijek = 0;
		int najIndex = -1;
		float najProsijek = 0;
		for (int i = 0; i < _ucenici.size(); i++) {
			prosijek = 0;
			for (int j = 0; j < _ucenici[i].GetAktivnosti().GetTrenutno(); j++) {
				prosijek += _ucenici[i].GetAktivnosti().GetElement2(j).GetOcjenu();

			}
			_ucenici[i].setProsijek(prosijek / _ucenici[i].GetAktivnosti().GetTrenutno());
			if (_ucenici[i].getProsijek() > najProsijek) {
				najProsijek = _ucenici[i].getProsijek();
				najIndex = i;
			}

		}
		/*Ucenik * u = new Ucenik(_ucenici[najIndex]);
		pair<Polaznik *, float> par;
		par.first = u;*/
		return make_pair((Polaznik*)(&_ucenici[najIndex]), najProsijek);

	}

	friend ostream& operator<<(ostream& COUT, Skola& obj) {

		COUT << "Skola: " << obj._naziv << endl;

		COUT << "Ucenici: " << endl;

		for (size_t i = 0; i < obj._ucenici.size(); i++)

			COUT << obj._ucenici[i] << endl;

		return COUT;

	}

};



int main() {

	Kolekcija<int, int, 10> kolekcija1(DESC, false);

	try {

		kolekcija1.AddElement(1, 2);

		//dupliranje elemenata nije dozvoljeno

		kolekcija1.AddElement(1, 2);

	}

	catch (exception & ex) {

		cout << ex.what();

	}



	///*nakon svakog dodavanja, elemente sortirati prema T1 i vrijednosti atributa _sortiranje*/

	for (size_t i = 1; i < kolekcija1.GetMax() - 1; i++)

		kolekcija1.AddElement(rand(), rand());



	cout << kolekcija1 << endl;



	try {

		//prekoracen maksimalan broj elemenata

		kolekcija1.AddElement(rand(), rand());

	}

	catch (exception & ex) {

		cout << ex.what();

	}


	cout << kolekcija1 << endl;



	Kolekcija<int, int, 10> kolekcija2(kolekcija1);

	cout << kolekcija2 << endl;



	Skola gimnazijaMostar("GIMNAZIJA MOSTAR");

	////dodaje novog ucenika u skolu

	gimnazijaMostar("Jasmin Azemovic", "+387(61)111-222");

	gimnazijaMostar("Adel Handzic", "+387(61)333-444");



	///*

	//koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX

	//onemoguciti pojavljivanje samo jedne zagrade, a ukoliko format nije adekvatna koristiti vrijednost not_set

	//*/

	gimnazijaMostar("Telefon NotValidFormat", "387 61)333-444");



	try

	{

		//  /*onemoguciti dodavanje ucenika sa istim imenom i prezimenom ili brojem telefona*/

		gimnazijaMostar("Adel Handzic", "+387(61)333-444");

	}

	catch (exception & ex)

	{

		cout << ex.what() << endl;

	}


	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Priprema za takmicenje iz Matematije koje se odrzava u Konjicu 07.02.2019")))

		cout << "Aktivnost uspjesno dodana" << endl;

	if (!gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Priprema za takmicenje iz Matematije koje se odrzava u Konjicu 07.02.2019")))

		cout << "Aktivnost nije uspjesno dodana" << endl;

	///*na nivou svakog razreda se mogu evidentirati maksimalno 4 aktivnosti, a takodjer, na nivou razreda se ne smiju ponavljati aktivnosti iz istog predmeta*/

	if (!gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Aktivnosti iz matematike")))

		cout << "Aktivnost nije uspjesno dodana" << endl;

	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", HEMIJA, Aktivnost(I, 5, "Priprema otopina za vjezbe iz predmeta Hemija")))

		cout << "Aktivnost uspjesno dodana" << endl;

	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", FIZIKA, Aktivnost(I, 4, "Analiza stepena apsorpcije materijala ")))

		cout << "Aktivnost uspjesno dodana" << endl;

	///*u slucaju da je ucenik uspjesno (ocjenom vecom od 1) realizovao aktivnosti na nivou odredjenog razreda, te posjeduje validan broj telefona,

	//u okviru zasebnog thread-a se salje SMS sa sadrzajem "Uspjesno ste okoncali aktivnosti u okviru X razreda sa prosjecnom ocjenom X.X"*/

	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", GEOGRAFIJA, Aktivnost(I, 4, "Izrada 5 reljefa Mostara")))

		cout << "Aktivnost uspjesno dodana" << endl;

	if (gimnazijaMostar.DodajAktivnost("Adel Handzic", MATEMATIKA, Aktivnost(I, 5, "Izrada skripte na temu integralni racun")))

		cout << "Aktivnost uspjesno dodana" << endl;

	cout << "----------------------------------------" << endl;

	////ispisuje sve ucenike i njihove aktivnosti

	cout << gimnazijaMostar << endl;





	pair<Polaznik*, float> par = gimnazijaMostar.GetNajboljegUcenika();

	cout << "Najbolji ucenik je " << par.first->GetImePrezime() << " sa prosjekom " << par.second << endl;



	///*U fajl (npr. Gimnazija.txt) upisati podatke (podatke upisati kao obicni tekst) o skoli i svim ucenicima.

	//Nakon upisa, potrebno je ispisati sadrzaj fajla. Parametar tipa bool oznacava da li ce ranije dodani sadrzaj fajla prethodno biti pobrisan*/



	if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt"))

		cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;


	if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt", true))

		cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;




	system("pause>0");

	return 0;

}