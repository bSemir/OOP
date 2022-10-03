#include <iostream>

class VektorNd {
    int dimenzija;
    double *koordinate;
    void TestIndeksa(int indeks) const {
        if(indeks < 1 || indeks > dimenzija) throw std::range_error("Pogrešan indeks!");
    }
public:
    explicit VektorNd(int dimenzija);
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
};

//konstruktor
VektorNd::VektorNd(int dimenzija) : dimenzija(dimenzija),
                                    koordinate(new double[dimenzija]) {
    std::fill(koordinate, koordinate + dimenzija, 0);
}

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

void VektorNd::Ispisi() const {
    std::cout << "{";
    for(int i = 0; i < dimenzija; i++) {
        std::cout << koordinate[i];
        if(i != dimenzija - 1) std::cout << ",";
    }
    std::cout << "}";
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

        std::cout << "###\n";
        v2.PromijeniDimenziju(2);
        std::cout << v2.DajTrenutnuDimenziju() << std::endl;
        v2.Ispisi();
    }
    catch(std::bad_alloc) {
        std::cout << "Problemi sa memorijom!\n";
    }

    return 0;
}
