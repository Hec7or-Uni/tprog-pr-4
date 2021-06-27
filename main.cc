# include <string>
# include <vector>
# include <iostream>
# include <sstream>
# include "ruta.h"

using namespace std;

int main() {
	Directorio raiz("");
	Ruta ruta(raiz);

	for (bool done=false; !done; ) {
		cout << ruta.pwd() << "> " << flush;

		// Leer linea de entrada
		string line;
		if (!getline(cin,line)) {
			done = true;
			continue;
		}

		// Separar tokens
		vector<string> cmd;
		istringstream iss(line);
		for (string tok; iss >> tok; )
			cmd.push_back(tok);

		if (cmd.size() < 1)
			continue;

		try {
			if ((cmd[0]=="exit") || (cmd[0]=="by")) {
				done = true;
			} else if (cmd[0]=="pwd") {
				cout << ruta.pwd() << endl;
			} else if (cmd[0]=="ls") {
				cout << ruta.ls() << endl;
			} else if (cmd[0]=="du") {
				cout << ruta.du() << endl;
			} else if (cmd[0]=="vi") {
				ruta.vi(cmd.at(1),stoi(cmd.at(2)));
			} else if (cmd[0]=="mkdir") {
				ruta.mkdir(cmd.at(1));
			} else if (cmd[0]=="cd") {
				ruta.cd(cmd.at(1));
			} else if (cmd[0]=="stat") {
				cout << ruta.stat(cmd.at(1)) << endl;
			} else if (cmd[0]=="ln") {
				ruta.ln(cmd.at(1),cmd.at(2));
			} else if (cmd[0]=="rm") {
				ruta.rm(cmd.at(1));
			} else {
				cerr << "Error sintactico: comando desconocido" << endl;
			}
		} catch (const arbol_ficheros_error& e) {
			cerr << e.what() << endl;
		} catch (const out_of_range& e) {
			cerr << "Error sintactico: parametros insuficientes" << endl;
			cerr << e.what() << endl;
		}
	}

	cout << endl << "Bye!!" << endl;

	return 0;
}
