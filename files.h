# pragma once

# include <list>
# include <memory>
# include <string>

using namespace std;

class Item {
    protected:
        string name;
    public:
        Item(const string& _name) 
            : name(_name) 
        { }

        // Devuelve el nombre del fichero
        string getName() const { return name; }
        // Devuelve el tamaño del fichero
        virtual int getSize() const = 0;
};

class Directorio : public Item {
    private:
        list<shared_ptr<Item>> content;
    public:
        Directorio(const string& _name)
            : Item(_name)
        { }

        // Devuelve el tamaño de un directorio
        int getSize() const {
            int size = 0;
            for (auto i : content) size += i->getSize();
            return size;
        }
        // Inserta un nuevo archivo al directorio
        void insert(shared_ptr<Item> c) { content.push_back(c); }
        // Borra un archivo del directorio
        void erase(shared_ptr<Item> c)  { content.remove(c); }
        // Borra el directorio
        void clean() { content.clear(); }

        // Busca un archivo en el directorio
        shared_ptr<Item> find(const string& c) const {
            for (auto i : content)
                if (c == i->getName()) return i;
            return nullptr;
        }

        // Muestra el nombre de los archivos de un directorio
        string ls() const {
            string str = "";
            for (auto i : content) str += i->getName() + "\n";
            return str;
        }

        // Muestra el nombre y tamaño de los archivos de un directorio 
        string du() const {
            string str = "";
            for (auto i : content) str += i->getName() + " " + to_string(i->getSize()) + "\n";
            return str;
        }

        // Devuelve una lista con todos los archivos almacenados dentro de un directorio
        list<shared_ptr<Item>> lista() { return content; }
};

class Fichero : public Item {
    private:
        int size;
    public:
        Fichero(const string& _name, const int& _size = 0) 
            : Item(_name), size(_size)
        { }

        // Devuelve el tamaño de un fichero
        int getSize() const { return size; }
        // Establece el nuevo tamaño de un fichero
        void setSize(const int& _size) { size = _size; }
};

class Enlace : public Item {
    private:
        shared_ptr<Item> link;
    public:
        Enlace(const string& _name, shared_ptr<Item> _file) 
            : Item(_name), link(_file)
        { }

        // Devuelve el tamaño de un archivo/directorio referenciado
        int getSize() const { return link->getSize(); }
        // Devuelve el link que referencia un directorio/fichero
        shared_ptr<Item> getLink() const { return link; }
};