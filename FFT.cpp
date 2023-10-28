#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <cmath>

using namespace std;

typedef complex<double> Complex;

void fft(vector<Complex> &data) {
    int n = data.size();
    if (n <= 1) return;

    vector<Complex> even(n / 2), odd(n / 2);
    for (int i = 0; i < n / 2; i++) {
        even[i] = data[i * 2];
        odd[i] = data[i * 2 + 1];
    }

    fft(even);
    fft(odd);

    for (int i = 0; i < n / 2; i++) {
        double term = -2 * M_PI * i / n;
        Complex exp_i(cos(term), sin(term));
        Complex temp = exp_i * odd[i];
        data[i] = even[i] + temp;
        data[i + n / 2] = even[i] - temp;
    }
} 

double magnitude(Complex c) {
    return abs(c);
}

int main() {
    string filename;
    int fs = 512; // Frecuencia de muestreo

    cout << "Ingrese el nombre del archivo .txt: ";
    cin >> filename;

    ifstream file(filename.c_str());
    if (!file) {
        cout << "No se pudo abrir el archivo." << endl;
        return 1;
    }

    vector<Complex> data;
    double value;
    while (file >> value) {
        data.push_back(Complex(value, 0.0));
    }
    file.close();

    fft(data);

    string outputFilename = filename + "_FFT.txt";
    ofstream outputFile(outputFilename);
    if (!outputFile) {
        cout << "No se pudo crear el archivo de salida." << endl;
        return 1;
    }

    int n = data.size();
    for (int i = 0; i < n / 2; i++) {
        double frequency = static_cast<double>(i) * fs / n;
        double amp = (magnitude(data[i])/n) * 2;
        outputFile << "Amplitud[" << i << "]: " << scientific << amp << " Vector de frecuencias[" << i << "]: " << fixed << frequency << endl;
    }

    outputFile.close();
    return 0;
}
