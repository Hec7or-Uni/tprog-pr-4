# pragma once
# include <list>
# include <memory>
# include <string.h>
# include "files.h"
# include "exception.h"

class Ruta {
	private:
		list<shared_ptr<Directorio>> route;
	public:
		Ruta(const Directorio& _root) {
			route.push_back(make_shared<Directorio>(_root));
		}

		// Devuelve la ruta completa de forma textual, con todos los 
		// nombres de los directorios desde la raíz hasta el directorio 
		// actual concatenados y separados por el separador '/'.
		string pwd() {
			string wd;
			for (auto i : route) wd += ('/' + i->getName());
			if  (route.size() > 1) { wd = wd.substr(1); }
			return wd;
		}

		// Devuelve un listado con el nombre de todos los nodos 
		// contenidos en la ruta actual, uno por línea.
		string ls() { 
			return route.back()->ls();
		}

		// Devuelve un listado con el nombre y el tamaño de todos 
		// los nodos contenidos en la ruta actual, uno por línea.
		string du() {
			return route.back()->du();
		}

		// Edita el fichero de nombre ”name” (en el directorio actual). Para simular la 
		// edición, simplemente se cambia el tamaño del fichero al valor especificado como 
		// parámetro. Si el fichero no existe, se debe crear con el nombre y tamaño especificados.
		template <typename T>
		void vi(const string& name, const T& size) {
			if ((int)name.find("/") != -1) 
				throw invalid_name_f1();
			if (((int)name.find(".") == -1) || (name[name.length()-1] == '.'))
				throw invalid_name_f2();
			if (size <= 0)
				throw invalid_size();

			shared_ptr<Item> node = route.back()->find(name);

			// Directorio = False:
			if (dynamic_pointer_cast<Directorio>(node) == nullptr) {
				// Existe = True:
				if (node != nullptr) {
					// Enlace = True:
					if (dynamic_pointer_cast<Enlace>(node) != nullptr) {
						shared_ptr<Enlace> link = dynamic_pointer_cast<Enlace>(node);
						// Linked = True:
						if (dynamic_pointer_cast<Fichero>(link->getLink()) != nullptr) {
							shared_ptr<Fichero> file = dynamic_pointer_cast<Fichero>(link->getLink());
							file->setSize(size);
						}
					}
					// Fichero = True:
					else if (dynamic_pointer_cast<Fichero>(node) != nullptr) {
						shared_ptr<Fichero> link = dynamic_pointer_cast<Fichero>(node);
						link->setSize(size);
					}
				}
				// Existe = False:
				else {
					shared_ptr<Fichero> _new = make_shared<Fichero>(Fichero(name, size));
					route.back()->insert(_new);
				}
			}
			// Directorio = True:
			else {
				throw resize_unavailable();
			}
		}

		// Crea un directorio dentro del directorio activo.
		void mkdir(string name) {
			if ((int)name.find("/") != -1) {
				if (name[0] == '/') 
					throw invalid_name_d1();
				else if (name[name.length()-1] == '/') 
					throw invalid_name_d2();
				else 
					throw invalid_name_d3();
			}
			if ((int)name.find(".") != -1) 
				throw invalid_name_d4();
			if (route.back()-> find(name) != nullptr) 
				throw already_exists_d1();

			// Crea el directorio
			shared_ptr<Directorio> _new = make_shared<Directorio>(Directorio(name));
			route.back()->insert(_new);
		}

		// cambia la ruta a otro directorio. El ”path” debe representar un directorio o
		// enlace a un directorio.
		void cd(const string& path) {
			if (path != ".") {
				// Directorio == raiz
				if (path == "/") {
					shared_ptr<Directorio> root = route.front();
					route.clear();
					route.push_back(root);
				}
				// Bajar de directorio
				else if (path == "..") {
					string back_up = pwd();
					if (pwd() != "/") { route.pop_back(); }
				}
				// ../ o ./
				else if (path[0] == '.') {
					string ruta;
					if (path[1] == '.') {
						cd("..");
						ruta = path.substr(3);
					} else {
						ruta = path.substr(2);
					}
					char *c = strtok((char*)ruta.c_str(), "/");
					while (c != nullptr) {
						cd(c);
						c = strtok(nullptr, "/");
					}				
				}
				// Desde el directorio raiz o que contenga alguna /
				else if (static_cast<int>(path.find_last_of("/")) > -1 ) {
					if (path[0] == '/') cd("/");
					char *c = strtok((char*)path.c_str(), "/");
					while (c != nullptr) {
						cd(c);
						c = strtok(nullptr, "/");
					}
				}
				else {
					shared_ptr<Item> exists = route.back()->find(path);
					if (exists == nullptr) 
						throw not_exists_d();
					if (dynamic_pointer_cast<Directorio>(exists) != nullptr) 
						route.push_back(dynamic_pointer_cast<Directorio>(exists));
					else 
						throw incorrect_type_d();
				}
			}
		}

		// Crea en el directorio actual un enlace simbólico de nombre ”name” que apunta 
		// al elemento identificado mediante el nombre ”path”. El nombre ”name” no
		// puede contener una ruta completa, es un nombre simple, pero ”path” sí, de tal modo que
		// pueden crearse enlaces simbólicos a elementos en otro directorio del árbol.
		void ln(const string& path, const string& name) {
			// back-up del directorio actual
			string back_up = pwd();

			// Busqueda del ultimo directorio del path donde se encuentra el archivo del que se quiere crear el enlace
			int pos = path.find_last_of("/");

			// Busqueda del arhivo a crear el enlace
			if (pos == 0) cd("/");
			else		  cd(path.substr(0, pos));

			shared_ptr<Item> linked = route.back()->find( path.substr(pos+1, path.size()) );	// Ejemplo: /folder/c++/hola_mundo.h -> [hola_mundo.h]
			if (linked == nullptr) 
				throw not_exists();
			string file_name = path.substr(1, path.size());

			// Check si ya existe un fichero con nombre "name" en la ruta actual
			cd(back_up);
			if (route.back()->find(name) != nullptr) 
				throw exists();
			shared_ptr<Enlace> _new;

			// Check si es un fichero
			if ((int)file_name.find(".") != -1) {
				if ((int)name.find(".")   ==  -1) throw invalid_name_f2();
				if (name[name.length()-1] == '.') throw invalid_name_f3();
			}

			// Crea y añade la copia en el directorio actual
			_new = make_shared<Enlace>(Enlace(name, linked));
			route.back()->insert(_new);
		}

		// Devuelve el tamaño del nodo que referencia el path.
		int stat(const string& path) {
			// back-up del directorio actual
			string back_up = pwd();

			// Busqueda del ultimo directorio donde se encuentra el archivo a evaluar
			int pos = path.find_last_of("/");

			/* Guarda el nombre del path que referencia */
			string fichero;
			// Directorio actual
			if (pos == -1) {
				fichero = path;
			}
			// Directorio raiz
			else if (pos == 0) {
				cd("/");
				fichero = path.substr(1);
			}
			// Ruta completa
			else if (pos > 0) {
				string ruta = path.substr(0, pos);
				cd(ruta);
				fichero = path.substr(pos+1);
			}
			
			// Obtención de los datos del archivo a evaluar
			shared_ptr<Item> node;
			if (path == "/") node = route.back();
			else 			 node = route.back()->find(fichero);
			if (node == nullptr) throw not_exists();
			int size = node->getSize();

			cd(back_up);
			return size;
		}

		// Elimina un nodo. Si es un fichero, es simplemente eliminado. Si es un enlace, elimina el 
		// enlace pero no el nodo referenciado. Si es un directorio, elimina el directorio 
		// y todo su contenido. Si existen enlaces al elemento borrado, ese elemento sigue siendo 
		// accesible a traves del elace, pero no a través de su nodo original.
		void rm(const string& path) {
			// back-up del directorio actual
			string back_up = pwd();

			if (strstr(back_up.c_str(), path.c_str()) != nullptr) 
				throw remove_error();

			// Busqueda del ultimo directorio del path donde se encuentra el archivo a borrar
			int pos = path.find_last_of("/");

			if (pos != 1) {	// El fichero a borrar esta en un path especifico
        		// Se busca el elemento
				shared_ptr<Item> rmv_elem = route.back()->find( path.substr(pos+1, path.size()) );
				if (rmv_elem == nullptr) 
					throw not_exists();
 				else {
					if (dynamic_pointer_cast<Fichero>(rmv_elem)     != nullptr)				route.back()->erase(rmv_elem);
					else if (dynamic_pointer_cast<Enlace>(rmv_elem) != nullptr) 			route.back()->erase(rmv_elem);
					else {
						if (dynamic_pointer_cast<Directorio>(rmv_elem)->getSize() == 0) 	route.back()->erase(rmv_elem);
						else {
							cd(back_up + "/" + path);	// Accede a un directorio desde el path activo (guardado previamente) para eliminar recursivamente
							rmv_elem = route.back();

							// Elimina el contenido de un directorio con varias llamadas recursivas
							for ( auto i : dynamic_pointer_cast<Directorio>(rmv_elem)->lista() )rm(i->getName());

							// Borra el directorio "principal" de forma recursiva
							cd(back_up);
							rmv_elem = route.back()->find(path.substr( pos+1, path.size()) );
							route.back()->erase(rmv_elem);
						}
					}

				}
			}
			else {	// El fichero a borrar esta en el path activo
				cd(path.substr(0, pos-1));
				shared_ptr<Item> rmv_elem = route.back()->find(path.substr( pos+1, path.size()) );
				rm(rmv_elem->getName());
			}
		}
};
