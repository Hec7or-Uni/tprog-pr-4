# pragma once

class arbol_ficheros_error {
    public:
        virtual const char* what() const throw() { return ""; }
};

// TYPE: File -> Path-error
// COMS: A file can not refer to a path.
class invalid_name_f1 : public arbol_ficheros_error {
	const char* what() const throw() { return "A file can not refer to a path."; }
};
// TYPE: File -> Extension-error
// COMS: The file needs an extension.
class invalid_name_f2 : public arbol_ficheros_error {
	const char* what() const throw() { return "The file needs an extension."; }
};
// TYPE: File -> Extension-error
// COMS: The file needs a complete extension.
class invalid_name_f3 : public arbol_ficheros_error {
	const char* what() const throw() { return "The file needs a complete extension."; }
};

// TYPE: Directory -> Path-error
// COMS: Directory name can not start with '/'
class invalid_name_d1 : public arbol_ficheros_error {
	const char* what() const throw() { return "Directory name can not start with '/'"; }
};
// TYPE: Directory -> Path-error
// COMS: Directory name can not end with '/'
class invalid_name_d2 : public arbol_ficheros_error {
	const char* what() const throw() { return "Directory name can not end with '/'"; }
};
// TYPE: Directory -> Nested-error
// COMS: Create a nested directory is not available.
class invalid_name_d3 : public arbol_ficheros_error {
	const char* what() const throw() { return "Create a nested directory is not available."; }
};
// TYPE: Directory -> Extension-error
// COMS: Directory can not contain an extension
class invalid_name_d4 : public arbol_ficheros_error {
	const char* what() const throw() { return "A directory can not contain an extension"; }
};

// TYPE: Size-error
// COMS: Size must be greater than 0.
class invalid_size : public arbol_ficheros_error {
	const char* what() const throw() { return "Size must be greater than 0."; }
};
// TYPE: Directory -> Re-size-error
// COMS: The size of a directory is invariant.
class resize_unavailable : public arbol_ficheros_error {
	const char* what() const throw() { return "The size of a directory is invariant."; }
};

// TYPE: File -> Exist-error
// COMS: There is already a file with the same name on the current path.
class already_exists_f1 : public arbol_ficheros_error {
	const char* what() const throw() { return "There is already a file with the same name on the current path."; }
};
// TYPE: Directory -> Exist-error
// COMS: There is already a diretory with the same name on the current route.
class already_exists_d1 : public arbol_ficheros_error {
	const char* what() const throw() { return "There is already a diretory with the same name on the current route."; }
};

// TYPE: unknown -> Exist-error
// COMS: There is already a file with the requested name.
class exists : public arbol_ficheros_error {
	const char* what() const throw() { return "There is already a file with the requested name."; }
};
// TYPE: unknown -> Exist-error
// COMS: There is no file with the requested name.
class not_exists : public arbol_ficheros_error {
	const char* what() const throw() { return "There is no file with the requested name."; }
};
// TYPE: Directory -> Exist-error
// COMS: The directory does not exist.
class not_exists_d : public arbol_ficheros_error {
	const char* what() const throw() { return "The directory does not exist."; }
};

// TYPE: unknown -> Erase-error
// COMS: You are within the path you intend to erase.
class remove_error : public arbol_ficheros_error {
	const char* what() const throw() { return "You are within the path you intend to erase."; }
};

// TYPE: Directory -> Type-error
// COMS: This is not a directory.
class incorrect_type_d : public arbol_ficheros_error {
	const char* what() const throw() { return "This is not a directory."; }
};

