#include <iostream>

class VektorNd {
    int dimenzija;
    double *koordinate;
    void TestIndeksa(int indeks) const {
        if(indeks < 1 || indeks > dimenzija) throw std::range_error("Pogrešan indeks!");
    }
public:
    explicit VektorNd(int dimenzija);
    VektorNd(std::initializer_list<double> lista);
    VektorNd(const VektorNd &v);
    VektorNd(VektorNd &&v);
    ~VektorNd() { delete[] koordinate; }
    VektorNd &operator =(const VektorNd &v);
    VektorNd &operator =(VektorNd &&v);
    void PromijeniDimenziju(int nova_dimenzija);
    void PostaviKoordinatu(int indeks, double vrijednost) {
        TestIndeksa(indeks); koordinate[indeks - 1] = vrijednost;
    }
    double DajKoordinatu(int indeks) const {
        TestIndeksa(indeks); return koordinate[indeks - 1];
    }
    double &DajKoordinatu(int indeks) {
        TestIndeksa(indeks); return koordinate[indeks - 1];
    }
    void Ispisi() const;
    int DajTrenutnuDimenziju() const { return dimenzija; }
    double *begin() const { return koordinate; }
    double *end() const { return koordinate + dimenzija; }
    friend VektorNd ZbirVektora(const VektorNd& v1, const VektorNd& v2);
};

//konstruktor
VektorNd::VektorNd(int dimenzija) : dimenzija(dimenzija),
                                    koordinate(new double[dimenzija]) {
    std::fill(koordinate, koordinate + dimenzija, 0);
}

//sekvencijski konstruktor
VektorNd::VektorNd(std::initializer_list<double> lista) :
    dimenzija(lista.size()),
    koordinate(new double[lista.size()]) {
        std::copy(lista.begin(), lista.end(), koordinate);
}

//kopirajuci konstruktor
VektorNd::VektorNd(const VektorNd &v) :
    dimenzija(v.dimenzija),
    koordinate(new double [v.dimenzija]) {
        std::copy(v.koordinate, v.koordinate + v.dimenzija, koordinate);
}

//pomjerajuci konstruktor
VektorNd::VektorNd(VektorNd &&v) : dimenzija(v.dimenzija), koordinate(v.koordinate) { v.koordinate = nullptr; }

void VektorNd::PromijeniDimenziju(int nova_dimenzija) {
    if(nova_dimenzija > dimenzija) {
        double *novo_mjesto = new double[nova_dimenzija];
        std::copy(koordinate, koordinate + dimenzija, novo_mjesto);

        // od C++11 nadalje smo alternativno mogli odmah kreirati niz inicijaliziran nulama pomoću konstrukcije new double[dimenzija]{}
        std::fill(novo_mjesto + dimenzija, novo_mjesto + nova_dimenzija, 0);
        delete[] koordinate; koordinate = novo_mjesto;
    }
    dimenzija = nova_dimenzija;
}

//kopirajuci operator dodjele
VektorNd &VektorNd::operator=(const VektorNd &v) {
    if(dimenzija < v.dimenzija){
        double *novi_prostor = new double[v.dimenzija];
        delete[] koordinate; koordinate = novi_prostor;
    }
    dimenzija = v.dimenzija;
    std::copy(v.koordinate, v.koordinate + v.dimenzija, koordinate);
    return *this;
}

//pomjerajuci operator dodjele
VektorNd &VektorNd::operator=(VektorNd &&v) {
    std::swap(dimenzija, v.dimenzija);
    std::swap(koordinate, v.koordinate);
    return *this;
    //ovako bi mogao biti implementiran i kopirajuci op.dodjele ali onda ne bi bilo jasno koji bi se pozivao jer
    //oba mogu prihvatiti privremeni objekat
}

void VektorNd::Ispisi() const {
    std::cout << "{";
    for(int i = 0; i < dimenzija; i++) {
        std::cout << koordinate[i];
        if(i != dimenzija - 1) std::cout << ",";
    }
    std::cout << "}";
}

VektorNd ZbirVektora(const VektorNd& v1, const VektorNd& v2) { //kako bi izbjegli kopiranje
    if(v1.dimenzija != v2.dimenzija)
        throw std::domain_error("Nesaglasne dimenzije");
    VektorNd v3(v1.dimenzija);
    for(int i = 0; i < v1.dimenzija; i++)
        v3.koordinate[i] = v1.koordinate[i] + v2.koordinate[i];
    return v3;
}

int main() {

    try {
        VektorNd v1(5), v2(3);
        v1.PostaviKoordinatu(1, 3); v1.PostaviKoordinatu(2, 5);
        v1.PostaviKoordinatu(3, -2); v1.PostaviKoordinatu(4, 0);
        v1.PostaviKoordinatu(5, 1); v2.PostaviKoordinatu(1, 3);
        v2.PostaviKoordinatu (2, 0); v2.PostaviKoordinatu(3, 2);
        std::cout << v1.DajKoordinatu(2) << std::endl;
        v1.Ispisi(); std::cout<< " "; v2.Ispisi();

        std::cout << std::endl << "###\n";
        v2.PromijeniDimenziju(2);
        std::cout << v2.DajTrenutnuDimenziju() << std::endl;
        v2.Ispisi();
        std::cout << std::endl;

        VektorNd v3{3, 5, 2, 6, 1};
        v3.Ispisi();
        std::cout << std::endl;
        std::cout << v3.DajTrenutnuDimenziju() << std::endl;

        std::cout << "Zbir: ";
        VektorNd v4{1, 2, 3};
        VektorNd v5{4, 5, 6};
        VektorNd zbir = ZbirVektora(v4, v5); //u slucaju da nije ovakav poziv, ne bi se pozvao kopirajuci konstruktor?
        zbir.Ispisi();
        std::cout << std::endl;

        VektorNd v6{9,9};
        VektorNd v7 = std::move(v6);
        v7.Ispisi(); // 9,9
        std::cout << std::endl;

        VektorNd v8{5, 5, 5};
        VektorNd v9{7, 7, 7};
        v8 = v9;
        v8.Ispisi(); // 7,7,7
        std::cout << std::endl;

        VektorNd v10(1);
        v10 = ZbirVektora(v4,v5);
        v10.Ispisi(); // {5,7,9}
    }
    catch(std::bad_alloc const &) {
        std::cout << "Problemi sa memorijom!\n";
    }

    return 0;
}
